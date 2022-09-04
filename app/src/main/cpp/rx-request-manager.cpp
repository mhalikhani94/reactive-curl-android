#include "rx-request-manager.hpp"

#include <iostream>
#include "rx-curl.hpp"

RxRequestManager& RxRequestManager::instance()
{
	static RxRequestManager s_instance;
	return s_instance;
}

rxcpp::observable<std::string> RxRequestManager::send_request(const std::string& url, HttpRequestMethod method, std::map<std::string, std::string> headers,
                               const std::string& body)
{
	return observable<>::create<std::string>([=](subscriber<std::string>& out)
	{

		const auto request = m_rx_curl->create(HttpRequest{url, method, std::move(headers), body})
		| rxcpp::rxo::map([&](const HttpResponse& r)
		{
			return r.body.complete;
		});

		request.subscribe(
		[=](const rxcpp::observable<std::string>& s)
		{
			s.sum().subscribe(
			[=](const std::string& response_string)
			{
				// std::cout << "response_string" << std::endl;
				out.on_next(response_string);
				out.on_completed();
			});
		}, [] ()
		{
		});
	});
}

void RxRequestManager::set_curl_config(const long timeout) const
{
	m_rx_curl->state->set_connection_timeout(timeout);
}

RxRequestManager::RxRequestManager() : m_rx_curl(create_rxcurl())
{
	std::cout << "Singleton RX Constructor" << std::endl;
	set_curl_config();
}
