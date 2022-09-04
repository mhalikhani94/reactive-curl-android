#include <jni.h>
//
//#include <yaml-cpp/yaml.h>
//#include <nlohmann/json.hpp>
//using json = nlohmann::json;

#include "rx-request-manager.hpp"
#include "types.hpp"

#include <string>
#include <iostream>
#include <chrono>
#include <thread>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_reactivex_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */)
{
    const auto post_url = "https://httpbin.org/post";
//    YAML::Node node = YAML::Load("[1, 2, 3]");
//    json ex1 = json::parse(R"(
//    {
//        "pi": 3.141,
//        "happy": true
//    }
//    )");
//    bool message_received = false;
//    std::string received_response{""};
//    RxRequestManager::instance().set_curl_config(500);
//    RxRequestManager::instance().send_request(post_url, "POST", {}, {}).subscribe(
//    [&](const rxcpp::observable<std::string>& s)
//    {
//        s.subscribe(
//                [&](const std::string& response_string)
//                {
//                    received_response = response_string;
//                    message_received = true;
//                });
//    });
//
//    while(true)
//    {
//        if(message_received)
//        {
//            return env->NewStringUTF(received_response.c_str());
//        }
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
//    }
    std::string hello_world{"Hello From C++!!"};
    return env->NewStringUTF(hello_world.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_reactivex_network_NetworkManager_sendNativePostRequest(
        JNIEnv* env,
        jobject /* this */,
        jstring inJNIStr)
{
    jboolean isOk;
    const char *inCStr = env->GetStringUTFChars(inJNIStr, &isOk);
    std::string java_url{inCStr};
    if(java_url.empty())
        return NULL;

    const auto post_url = java_url;
    bool message_received = false;
    std::string received_response{""};
    RxRequestManager::instance().set_curl_config(500);
    RxRequestManager::instance().send_request(post_url, HttpRequestMethod::kPost, {}, {}).subscribe(
        [&](const std::string& response_string)
        {
            received_response = response_string;
            message_received = true;
        },
        [](){}
    );

    while(true)
    {
        if(message_received)
        {
            return env->NewStringUTF(received_response.c_str());
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_reactivex_network_NetworkManager_sendNativeGetRequest(
        JNIEnv* env,
        jobject /* this */,
        jstring inJNIStr)
{
    jboolean isOk;
    const char *inCStr = env->GetStringUTFChars(inJNIStr, &isOk);
    std::string java_url{inCStr};
    if(java_url.empty())
        return NULL;

    const auto get_url = java_url;
    bool message_received = false;
    std::string received_response{""};
    RxRequestManager::instance().set_curl_config(500);
    RxRequestManager::instance().send_request(get_url, HttpRequestMethod::kGet, {}, {}).subscribe(
        [&](const std::string& response_string)
        {
            received_response = response_string;
            message_received = true;
        },
        [](){}
    );

    while(true)
    {
        if(message_received)
        {
            return env->NewStringUTF(received_response.c_str());
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}