//
// Created by vishal.phadtare on 9/6/2019.
//

#ifndef SECTION1_APP_B_NETWORKREQUEST_H
#define SECTION1_APP_B_NETWORKREQUEST_H


#include <string>
#include "NetworkDataListener.h"


struct NetworkRequest {

    enum{
        METHOD_DEFAULT	= 1,
        METHOD_DELETE	= 2,
        METHOD_PUT		= 3,
        METHOD_HEAD		= 4,
    };
    enum {
        STATUS_QUEUED		= 1,
        STATUS_ACTIVE		= 2,
        STATUS_CANCELLED	= 3,
    };

    ~NetworkRequest(){
        // reset
    }

    NetworkRequest(){
        m_hashCode = 0;
        m_cacheExpireTime = 0;
        m_userData = 0;
        m_userId   = 0;
        m_method   = METHOD_DEFAULT;
        m_status   = STATUS_QUEUED;
    }

    std::string	m_url;
    int			m_hashCode;
    int			m_cacheExpireTime;
    int			m_userData;
    int     	m_userId;
    NetworkDataListener*	m_listener;
    int				m_method;
    int				m_status;
};


#endif //SECTION1_APP_B_NETWORKREQUEST_H
