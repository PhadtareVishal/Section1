//
// Created by vishal.phadtare on 9/6/2019.
//

#include "NetworkEngine.h"
#include "NetworkDataListener.h"
#include <jni.h>
#include <iostream>
#include <string>
#include "curl/curl.h"
#include <android/log.h>

std::string response;

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up) {

    for (int c = 0; c<size*nmemb; c++) {
        response.push_back(buf[c]);
    }

    return size*nmemb;
}

void NetworkEngine::addNewRequest(const std::string &url, int expireTime, NetworkDataListener* listener, int userData){

    NetworkRequest* request = new NetworkRequest();
    request->m_url = url;
    request->m_listener = listener;
    request->m_cacheExpireTime = expireTime;
    request->m_userId = userData;
    /// add other details here
    m_queue.push_back(request);

    CURL* curl;

    curl_global_init(CURL_GLOBAL_ALL);

    curl=curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, "http://jsonplaceholder.typicode.com/posts/1");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);

    curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    curl_easy_setopt(curl, CURLOPT_PROXY, "pun-net-mwg.ubisoft.org:3128");
    curl_global_cleanup();

}