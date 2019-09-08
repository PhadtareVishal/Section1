//
// Created by vishal.phadtare on 9/6/2019.
//

#ifndef SECTION1_APP_B_NETWORKDATALISTENER_H
#define SECTION1_APP_B_NETWORKDATALISTENER_H


class NetworkDataListener {

    public:
        virtual ~NetworkDataListener();
        virtual void onNetworkError(/*MZ_NETWORK_ERROR*/ int error, int errorId, int userData,void *userPtr = 0) = 0;
        virtual void onNetworkServerTimeUpdated(int currentTime) {}
        virtual bool onNetworkDataReceived(char* buffer, int bufferSize, int userData, int messageId=0,void *userPtr = 0) = 0;
};

#endif //SECTION1_APP_B_NETWORKDATALISTENER_H
