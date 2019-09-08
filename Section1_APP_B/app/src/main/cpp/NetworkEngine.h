//
// Created by vishal.phadtare on 9/6/2019.
//

#ifndef SECTION1_APP_B_NETWORKENGINE_H
#define SECTION1_APP_B_NETWORKENGINE_H

#include "NetworkRequest.h"
#include <iostream>
#include <list>
#include <iterator>

class NetworkEngine {
   std::list<NetworkRequest*> m_queue;
public:
    void addNewRequest(const std::string &url, int expireTime, NetworkDataListener* listener, int userData);
};


#endif //SECTION1_APP_B_NETWORKENGINE_H
