#ifndef RX_CURL_HPP_
#define RX_CURL_HPP_

#include <memory>
#include <rxcpp/rx.hpp>
#include <curl/curl.h>

#include <thread>
#include <string.h>

using namespace rxcpp;
using namespace rxcpp::operators;

struct RxCurlState
{
	~RxCurlState()
	{
		if (!!curlm)
		{
			worker.as_blocking().subscribe();
			curl_multi_cleanup(curlm);
			curlm = nullptr;
		}
	}

	RxCurlState() :
		thread(observe_on_new_thread()),
		worker(),
		curlm(curl_multi_init())
	{
		worker = observable<>::create<CURLMsg*>([this](subscriber<CURLMsg*> out)
			{
				while (out.is_subscribed())
				{
					int running = 0;
					curl_multi_perform(curlm, &running);
					for (;;)
					{
						CURLMsg* message = nullptr;
						int remaining = 0;
						message = curl_multi_info_read(curlm, &remaining);
						out.on_next(message);
						if (!!message && remaining > 0)
						{
							continue;
						}
						break;
					}
					int handlecount = 0;
					curl_multi_wait(curlm, nullptr, 0, 5000, &handlecount);
					if (handlecount == 0)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					}
				}
				out.on_completed();
			}) |
			subscribe_on(thread) |
			finally([]() { std::cerr << "RxCurl worker exit" << std::endl; }) |
			publish() |
			connect_forever();
	}

	RxCurlState(const RxCurlState&) = delete;
	RxCurlState& operator=(const RxCurlState&) = delete;
	RxCurlState(RxCurlState&&) = delete;
	RxCurlState& operator=(RxCurlState&&) = delete;

	observe_on_one_worker thread;
	observable<CURLMsg*> worker;
	CURLM* curlm;

	void set_connection_timeout(long timeout)
	{
		curl_multi_timeout(curlm, &timeout);
	}
};

struct HttpRequest
{
	std::string url;
	std::string method;
	std::map<std::string, std::string> headers;
	std::string body;
};

struct HttpState
{
	~HttpState()
	{
		if (!!curl)
		{
			auto localcurl = curl;
			auto localheaders = headers;
			auto localrxcurl = rxcurl;
			auto localRequest = request;
			chunkbus.get_subscription().unsubscribe();
			subscriber<std::string>* localChunkout = chunkout.release();
			rxcurl->worker
			      .take(1)
			      .tap([=](CURLMsg*)
			      {
				      curl_multi_remove_handle(localrxcurl->curlm, localcurl);
				      curl_easy_cleanup(localcurl);
				      curl_slist_free_all(localheaders);
				      delete localChunkout;
			      })
			      .subscribe();

			curl = nullptr;
			headers = nullptr;
		}
	}

	explicit HttpState(std::shared_ptr<RxCurlState> m, HttpRequest r) : rxcurl(m), request(r), code(CURLE_OK),
	                                                                    httpStatus(0), curl(nullptr), headers(nullptr)
	{
		error.resize(CURL_ERROR_SIZE);
	}

	HttpState(const HttpState&) = delete;
	HttpState& operator=(const HttpState&) = delete;
	HttpState(HttpState&&) = delete;
	HttpState& operator=(HttpState&&) = delete;

	std::shared_ptr<RxCurlState> rxcurl;
	HttpRequest request;
	std::string error;
	CURLcode code;
	int httpStatus;
	subjects::subject<std::string> chunkbus;
	std::unique_ptr<subscriber<std::string>> chunkout;
	CURL* curl;
	struct curl_slist* headers;
	std::vector<std::string> strings;
};

struct HttpException : std::runtime_error
{
	explicit HttpException(const std::shared_ptr<HttpState>& s) : runtime_error(s->error), state(s)
	{
	}

	CURLcode code() const
	{
		return state->code;
	}

	int httpStatus() const
	{
		return state->httpStatus;
	}

	std::shared_ptr<HttpState> state;
};

struct HttpBody
{
	observable<std::string> chunks;
	observable<std::string> complete;
};

struct HttpResponse
{
	const HttpRequest request;
	HttpBody body;

	CURLcode code() const
	{
		return state->code;
	}

	int httpStatus() const
	{
		return state->httpStatus;
	}

	std::shared_ptr<HttpState> state;
};

static size_t rx_curl_http_callback(char* ptr, size_t size, size_t nmemb, subscriber<std::string>* out)
{
	int iRealSize = size * nmemb;

	std::string chunk;
	chunk.assign(ptr, iRealSize);
	std::cout << "chunk..... " << iRealSize << std::endl;
	out->on_next(chunk);

	return iRealSize;
}

static size_t read_callback(void* ptr, size_t size, size_t nmemb, void* stream)
{
	curl_off_t nread = strlen((const char*)stream);
	memcpy(ptr, stream, nread);
	return nread;
}

struct RxCurl
{
	std::shared_ptr<RxCurlState> state;

	long m_http_status = 0;

	observable<HttpResponse> create(HttpRequest request)
	{
		return observable<>::create<HttpResponse>([=](subscriber<HttpResponse>& out)
		{
			auto requestState = std::make_shared<HttpState>(state, request);

			HttpResponse r{request, HttpBody{}, requestState};

			r.body.chunks = r.state->chunkbus.get_observable()
			                 .tap([requestState](const std::string&)
			                 {
			                 });

			r.body.complete = r.state->chunkbus.get_observable()
			                   .tap([requestState](const std::string&)
			                   {
			                   })
			                   .start_with(std::string{})
			                   .sum()
			                   .replay(1)
			                   .ref_count();

			out.on_next(r);
			out.on_completed();

			auto localState = state;

			// start on worker thread
			state->worker
			     .take(1)
			     .tap([r, localState](CURLMsg*)
			     {
				     auto curl = curl_easy_init();

				     auto& request = r.state->request;

				     curl_easy_setopt(curl, CURLOPT_URL, request.url.c_str());

				     if (request.method == "POST")
				     {
					     curl_easy_setopt(curl, CURLOPT_POST, 1L);
					     curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.body.c_str());
				     }
				     else if (request.method == "PUT")
				     {
					     curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

					     const char* data_s = request.body.c_str();
					     curl_easy_setopt(curl, CURLOPT_READDATA, data_s);
					     curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
					     int len = strlen(data_s);
					     curl_easy_setopt(curl, CURLOPT_INFILESIZE, strlen(request.body.c_str()));
				     }
				     else if (request.method == "DELETE")
				     {
					     curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST,"DELETE");
					     curl_easy_setopt(curl, CURLOPT_POSTFIELDS,request.body.c_str());
				     }
                     else if( request.method == "PATCH")
                     {
                         curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST,"PATCH");
                         curl_easy_setopt(curl, CURLOPT_POSTFIELDS,request.body.c_str());
                     }

				     auto& strings = r.state->strings;
				     auto& headers = r.state->headers;
				     for (auto& h : request.headers)
				     {
					     strings.push_back(h.first + ": " + h.second);
					     headers = curl_slist_append(headers, strings.back().c_str());
				     }

				     if (!!headers)
				     {
					     curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
				     }

				     curl_easy_setopt(curl, CURLOPT_USERAGENT, "PostmanRuntime/7.29.0");
					 curl_easy_setopt(curl, CURLOPT_CAINFO, "cert/cacert.pem");
					 curl_easy_setopt(curl, CURLOPT_CAPATH, "cert/cacert.pem");
					 curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
					 curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

					struct curl_slist *inner_list = NULL;
					// inner_list = curl_slist_append(inner_list, "Accept-Encoding:gzip, deflate, br");
  					// inner_list = curl_slist_append(inner_list, "Accept:*/*");
					curl_easy_setopt(curl, CURLOPT_HTTPHEADER, inner_list);

				     curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);
				     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, rx_curl_http_callback);
				     r.state->chunkout.reset(new subscriber<std::string>(r.state->chunkbus.get_subscriber()));
				     curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)r.state->chunkout.get());
				     curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, &r.state->error[0]);
				     r.state->curl = curl;
				     curl_multi_add_handle(localState->curlm, curl);
			     })
			     .subscribe();

			std::weak_ptr<HttpState> wrs = requestState;
			// extract completion and result
			state->worker
			     .filter([wrs](CURLMsg* message)
			     {
				     auto rs = wrs.lock();
				     return !!rs && !!message && message->easy_handle == rs->curl && message->msg == CURLMSG_DONE;
			     })
			     .take(1)
			     .tap([&,wrs](CURLMsg* message)
			     {
				     auto rs = wrs.lock();
				     if (!rs)
				     {
					     return;
				     }
				     rs->error.resize(strlen(&rs->error[0]));

				     auto chunkout = rs->chunkbus.get_subscriber();

				     long httpStatus = 0;

				     curl_easy_getinfo(rs->curl, CURLINFO_RESPONSE_CODE, &m_http_status);
				     rs->httpStatus = httpStatus;

				     std::cout << "Status" << message->data.result << CURLE_OK << std::endl;
				     if (message->data.result != CURLE_OK)
				     {
					     rs->code = message->data.result;
					     if (rs->error.empty())
					     {
						     rs->error = curl_easy_strerror(message->data.result);
					     }
					     std::cerr << "RxCurl request fail: " << httpStatus << " - " << rs->error << std::endl;
					     observable<>::error<std::string>(HttpException(rs)).subscribe(chunkout);
					     return;
				     }
				     else if (httpStatus > 499)
				     {
					     std::cerr << "RxCurl request http fail: " << httpStatus << " - " << rs->error << std::endl;
					     observable<>::error<std::string>(HttpException(rs)).subscribe(chunkout);
					     return;
				     }

				     std::cerr << "RxCurl request complete: " << httpStatus << " - " << rs->error << std::endl;
				     chunkout.on_completed();
			     })
			     .subscribe();
		});
	}
};

RxCurl* create_rxcurl()
{
	auto* r = new RxCurl{std::make_shared<RxCurlState>()};
	return r;
};

#endif
