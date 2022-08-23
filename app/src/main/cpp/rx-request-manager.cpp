#include "rx-request-manager.hpp"

#include <iostream>
#include "rx-curl.hpp"

RxRequestManager& RxRequestManager::instance()
{
	static RxRequestManager s_instance;
	return s_instance;
}

rxcpp::observable<rxcpp::observable<std::string>> RxRequestManager::send_request(const std::string& url, std::string method, std::map<std::string, std::string> headers,
                               const std::string& body)
{
	return observable<>::create<rxcpp::observable<std::string>>([=](subscriber<rxcpp::observable<std::string>>& out)
	{
		const auto request = m_rx_curl->create(HttpRequest{url, std::move(method), std::move(headers), body})
		| rxcpp::rxo::map([&](const HttpResponse& r)
		{
			return r.body.complete;
		});
		request.subscribe(
		[&](const rxcpp::observable<std::string>& s)
		{
			// response_message = s.sum();
			out.on_next(s.sum());
			out.on_completed();
		}, []
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
