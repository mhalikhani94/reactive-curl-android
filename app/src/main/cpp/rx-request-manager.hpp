#ifndef RX_REQUEST_MANAGER_HPP
#define RX_REQUEST_MANAGER_HPP

#include <map>
#include <string>
#include <vector>
#include <rxcpp/rx.hpp>

struct RxCurl;

class RxRequestManager
{
public:
	static RxRequestManager& instance();

	RxRequestManager(RxRequestManager&& i) = delete;
	RxRequestManager(const RxRequestManager& i) = delete;
	RxRequestManager& operator=(RxRequestManager&& i) = delete;
	RxRequestManager operator=(const RxRequestManager& i) = delete;

	~RxRequestManager() = default;

	rxcpp::observable<rxcpp::observable<std::string>> send_request(const std::string& url, std::string method, std::map<std::string, std::string> headers,
	                  const std::string& body);

	void set_curl_config(long timeout = 500) const;


private:
	RxRequestManager();

	RxCurl* m_rx_curl;
	std::string html;
};
#endif //RX_REQUEST_MANAGER_HPP