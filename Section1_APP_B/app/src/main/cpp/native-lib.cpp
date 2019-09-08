#include <jni.h>
#include <string>
#include "networkManager.h"
#include <android/log.h>
#include "JNITools.h"


#define LOG_TAG "Native-lib native.cpp"

#define  INTENT_SERVICCE	"com/assignment/vishalphadtare/section1_app_b/MyIntentService"
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)



void _onActivate(){
   // int x = test->getInt();
    networkManager::getInstance()->makeRequest(networkManager::NETWORK_COMMAND::NETWORK_CMD_ECO);
    ALOGE("Native registration unable to find class '%d'", 10);
}

void _onDeactivate(){
    ALOGE("Native on deactivate ");
}

void registerNativeMethods(JNIEnv* env)
{
    jclass clazz;
    clazz = env->FindClass(INTENT_SERVICCE);
    JNINativeMethod nativeMethods[] = {
            {"onInit","()V",(void*)&_onActivate},
    };
    if (clazz == NULL) {
        ALOGE("Native registration unable to find class '%s'", INTENT_SERVICCE);
    }
    if (env->RegisterNatives(clazz, nativeMethods, sizeof(nativeMethods)/ sizeof(JNINativeMethod)) < 0) {
        ALOGE("RegisterNatives failed for '%s'", INTENT_SERVICCE);
    }
}

jstring getJavaString(JNIEnv* env, std::string message)
{
    int byteCount = message.length();
    const jbyte* pNativeMessage = reinterpret_cast<const jbyte*>(message.c_str());
    jbyteArray bytes = env->NewByteArray(byteCount);
    env->SetByteArrayRegion(bytes, 0, byteCount, pNativeMessage);

    // find the Charset.forName method:
    //   javap -s java.nio.charset.Charset | egrep -A2 "forName"
    jclass charsetClass = env->FindClass("java/nio/charset/Charset");
    jmethodID forName = env->GetStaticMethodID(charsetClass, "forName", "(Ljava/lang/String;)Ljava/nio/charset/Charset;");
    jstring utf8 = env->NewStringUTF("UTF-8");
    jobject charset = env->CallStaticObjectMethod(charsetClass, forName, utf8);

    // find a String constructor that takes a Charset:
    //   javap -s java.lang.String | egrep -A2 "String\(.*charset"
    jclass stringClass = env->FindClass("java/lang/String");
    jmethodID ctor = env->GetMethodID(stringClass, "<init>", "([BLjava/nio/charset/Charset;)V");
    jstring jMessage = reinterpret_cast<jstring>(env->NewObject(stringClass, ctor, bytes, charset));

    return jMessage;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_assignment_vishalphadtare_section1_1app_1b_MyIntentService_registerFromJNI(
        JNIEnv *env,
        jobject /* this */,
        jobject activity) {
    std::string hello = "Network service app, should be called from other app";

    JNIEnv*	m_env;
    JNIEnvHandler::m_javaVM->GetEnv((void**)&m_env, JNI_VERSION_1_6);

    JNIEnvHandler::m_javaActivity = m_env->NewGlobalRef(activity);
    registerNativeMethods(env);

    std::string text = "Player ";
    //text += std::to_string(networkManager::getInstance()->getInt());
    return env->NewStringUTF(hello.data());
}

JNIEXPORT jint JNICALL JNI_OnLoad( JavaVM *vm, void *pvt )
{
    // Configure JNITools
    ALOGE("RegisterNatives JNI_OnLoad ");
    JNIEnvHandler::m_javaVM = vm;
    return JNI_VERSION_1_2;
}