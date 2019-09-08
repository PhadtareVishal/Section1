//
// Created by vishal.phadtare on 9/7/2019.
//

#include <android/log.h>
#include "JNITools.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "jni-tools", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "jni-tools", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "jni-tools", __VA_ARGS__))

// ----------------------------- JNIEnvHandler --------------------------------
JavaVM* JNIEnvHandler::m_javaVM = NULL;
jobject JNIEnvHandler::m_javaActivity = NULL;

//extern jstring getJavaString(JNIEnv* env, const char* message);
jstring getJavaString(JNIEnv* env, const char* message)
{
    if(message == nullptr)
        return nullptr;

    int byteCount = strlen(message);
    const jbyte* pNativeMessage = reinterpret_cast<const jbyte*>(message);
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

JNIEnvHandler::JNIEnvHandler(long localFrameCount /*= 16*/)
        :	m_env(NULL)
        ,	m_obtainedByAttach(false)
{
    int status = m_javaVM->GetEnv((void**)&m_env, JNI_VERSION_1_6);

    if (status < 0)
    {
        status = m_javaVM->AttachCurrentThread(&m_env, NULL);

        m_obtainedByAttach = true;
    }

    m_env->PushLocalFrame(localFrameCount);
}

JNIEnvHandler::~JNIEnvHandler()
{
    m_env->PopLocalFrame(NULL);

    if (m_obtainedByAttach)
    {
        m_javaVM->DetachCurrentThread();
    }

    m_env = NULL;
}

jobject ObtainClassLoader(JNIEnv* env, jobject activity)
{
    if (!activity)
    {
        //Logging::Log(Logging::ERR, "Supplied activity jobject not valid.");
        return 0;
    }

    jclass activityClass = env->GetObjectClass(activity);
    jmethodID getClassLoader = env->GetMethodID(activityClass, "getClassLoader",
                                                "()Ljava/lang/ClassLoader;");
    jobject classLoader = env->CallObjectMethod(activity, getClassLoader);

    return classLoader;
}

jobject ObtainSystemClassLoader(JNIEnv* env)
{
    jclass classClassLoader = env->FindClass("java/lang/ClassLoader");
    jmethodID getSystemClassLoader = env->GetStaticMethodID(classClassLoader,
                                                            "getSystemClassLoader", "()Ljava/lang/ClassLoader;");
    jobject systemClassLoader = env->CallStaticObjectMethod(classClassLoader,
                                                            getSystemClassLoader);

    return systemClassLoader;
}


// ----------------------------- FindClass ------------------------------------

jclass FindClass(JNIEnv* env, jobject activity, const char* className)
{
    if(env->ExceptionCheck())
    {
        LOGI("inside ExceptionCheck for %s",className);
        jthrowable exc = env->ExceptionOccurred();
        env->ExceptionDescribe();
        env->ExceptionClear();

        jclass classClassNotFoundException = env->FindClass("java/lang/ClassNotFoundException");

        if (env->IsInstanceOf(exc, classClassNotFoundException))
        {
            LOGI("[FindClass] Exception caught is of type java/lang/ClassNotFoundException");
        }
    }

    jclass normalFindClassResult = env->FindClass(className);
    if (normalFindClassResult)
    {
        return normalFindClassResult;
    }
    else
    {
        if (!normalFindClassResult || env->ExceptionCheck())
        {
            jthrowable exc = env->ExceptionOccurred();
            env->ExceptionDescribe();
            env->ExceptionClear();
            static_cast<void>(exc);

            LOGI("jclass for %s will not be found by JNIEnv.FindClass.", className);
            //Logging::Log(Logging::INFORMATION, "jclass for %s will not be found by JNIEnv.FindClass.", className);
        }
    }

    if (!activity)
    {
        LOGE("Supplied activity jobject not valid.");
        //Logging::Log(Logging::ERR, "Supplied activity jobject not valid.");
        return 0;
    }

    LOGI("Class couldnt find using normal search. Using Activity now %p", activity);

    jclass activityClass = env->GetObjectClass(activity);
    LOGI("Found Activity class %p", activityClass);

    jmethodID getClassLoader = env->GetMethodID(activityClass, "getClassLoader", "()Ljava/lang/ClassLoader;");
    LOGI("Found class loader method id %d", getClassLoader);

    jobject classLoader = env->CallObjectMethod(activity, getClassLoader);
    LOGI("Class loader method instance %p", classLoader);

    jclass classClassLoader = env->GetObjectClass(classLoader);//env->FindClass("java/lang/ClassLoader");
    LOGI("Class loader class %p", classClassLoader);

    jmethodID loadClass = env->GetMethodID(classClassLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
    LOGI("ClassLoader loadClass() method id %d", loadClass);

    jstring classNameString = getJavaString(env, className);
    //jstring classNameString = env->NewStringUTF(className);

    jclass wantedClass = (jclass) env->CallObjectMethod(classLoader, loadClass, classNameString); // Activity.getClassLoader().loadClass(classNameString);
    LOGI("Wanted class found %p", wantedClass);

    if (!wantedClass || env->ExceptionCheck())
    {
        jthrowable exc = env->ExceptionOccurred();
        env->ExceptionDescribe();
        env->ExceptionClear();

        jclass classClassNotFoundException = env->FindClass("java/lang/ClassNotFoundException");
        if (env->IsInstanceOf(exc, classClassNotFoundException))
        {
            LOGE("[FindClass] Exception caught is of type java/lang/ClassNotFoundException");
            //Logging::Log(Logging::ERR, "[FindClass] Exception caught is of type java/lang/ClassNotFoundException");
        }

        LOGE("[FindClass] failed to find class %s", className);
        //Logging::Log(Logging::ERR, "[FindClass] failed to find class %s", className);
    }

    return wantedClass;
}

jclass FindClassFromClassLoader(JNIEnv* env, jobject classLoader, const char* className)
{
    jclass classClassLoader = env->FindClass("java/lang/ClassLoader");
    jmethodID loadClass = env->GetMethodID(classClassLoader, "loadClass",
                                           "(Ljava/lang/String;)Ljava/lang/Class;");

    jstring classNameString = getJavaString(env, className);
    //jstring classNameString = env->NewStringUTF(className);

    jclass wantedClass = (jclass) env->CallObjectMethod(classLoader, loadClass,
                                                        classNameString);

    if (!wantedClass || env->ExceptionCheck())
    {
        jthrowable exc = env->ExceptionOccurred();
        env->ExceptionDescribe();
        env->ExceptionClear();

        jclass classClassNotFoundException = env->FindClass("java/lang/ClassNotFoundException");
        if (env->IsInstanceOf(exc, classClassNotFoundException))
        {
            //Logging::Log(Logging::ERR, "[FindClassFromClassLoader] failed to find class %s.\n", className);
        }
    }

    return wantedClass;
}

std::string GetClassName(JNIEnv* env, jclass clazz)
{
    jclass classClass = env->FindClass("java/lang/Class");
    jmethodID getCanonicalName = env->GetMethodID(classClass,
                                                  "getCanonicalName", "()Ljava/lang/String;");

    jstring canonicalName = (jstring) env->CallObjectMethod((jobject) clazz,
                                                            getCanonicalName);

    const char* canonicalNameChars = env->GetStringUTFChars(canonicalName, NULL);

    std::string result(canonicalNameChars);

    env->ReleaseStringUTFChars(canonicalName, canonicalNameChars);

    return result;
}

