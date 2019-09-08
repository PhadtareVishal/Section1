//
// Created by vishal.phadtare on 9/6/2019.
//

#ifndef SECTION1_APP_B_NETWORKMANAGER_H
#define SECTION1_APP_B_NETWORKMANAGER_H

#include "NetworkEngine.h"



class networkManager : public NetworkDataListener{

public:
    enum NETWORK_COMMAND{
        NETWORK_CMD_ECO,
        NETWORK_CMD_SUM,
        NETWORK_CMD_MULTIPLY,
        NETWORK_CMD_SUB
    };
    int getInt(){return i;}
    int getVariable(){ return x;}
    static networkManager *getInstance();
    void makeRequest(NETWORK_COMMAND command);

    void init();

private:
    networkManager(){};
    static int i;
    int x;
    static networkManager* m_instance;
    NetworkEngine* m_engine{};
    virtual void onNetworkError(/*MZ_NETWORK_ERROR*/ int error, int errorId, int userData,void *userPtr = 0);
    virtual bool onNetworkDataReceived(char* buffer, int bufferSize, int userData, int messageId=0,void *userPtr = 0);
};

#endif //SECTION1_APP_B_NETWORKMANAGER_H
