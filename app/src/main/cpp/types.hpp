#ifndef REACTIVE_CURL_HPP
#define REACTIVE_CURL_HPP
#include <string>

enum class HttpRequestMethod
{
	kGet,
	kPost,
	kPatch,
	kPut,
	kDelete,
};

struct Response
{
	int http_status;
	std::string http_response;
};

#endif //REACTIVE_CURL_HPP