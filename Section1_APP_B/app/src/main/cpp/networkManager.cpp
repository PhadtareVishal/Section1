//
// Created by vishal.phadtare on 9/6/2019.
//

#include "networkManager.h"
#include "NetworkRequest.h"
#include <jni.h>
#include "JNITools.h"

networkManager* networkManager::m_instance = nullptr;
int networkManager::i = 0;

networkManager *networkManager::getInstance() {
    if (!m_instance) {
        i =0;
        m_instance = new networkManager();
    }
    i++;
    return m_instance;
}

void networkManager::init(){

    if(m_engine == nullptr)
        m_engine = new NetworkEngine();
}

void networkManager::makeRequest(NETWORK_COMMAND command){

    init(); // check if network engine already initialized

    m_engine->addNewRequest("https://postman-echo.com/post",0, this, 0);
    /*std::string data = "Vishal Phadtare";

    JNIEnvHandler jniHandler;
    JNIEnv* env = jniHandler.Get();

    jclass callbackClass = FindClass(env, JNIEnvHandler::m_javaActivity, "com/assignment/vishalphadtare/section1_app_b/MyIntentService");

    jmethodID sendID = env->GetStaticMethodID(callbackClass , "returnValueReceived", "(Ljava/lang/String;)V");


    jstring jRawURL	= env->NewStringUTF(data.data());
    //jstring jRawURL	= getJavaString(env,data.data());

    env->CallStaticVoidMethod(callbackClass, sendID, jRawURL);

    env->DeleteLocalRef(jRawURL);*/
}

void networkManager::onNetworkError(/*MZ_NETWORK_ERROR*/ int error, int errorId, int userData,void *userPtr){



}

bool networkManager::onNetworkDataReceived(char* buffer, int bufferSize, int userData, int messageId,void *userPtr){

return true;

}