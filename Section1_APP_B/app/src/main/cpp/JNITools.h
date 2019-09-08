//
// Created by vishal.phadtare on 9/7/2019.
//

#ifndef SECTION1_APP_B_JNITOOLS_H
#define SECTION1_APP_B_JNITOOLS_H


#include <string>
#include <jni.h>


// JNIEnvHandler provides 2 functionalities in a function scope
// 1. Obtains a correct JNIEnv* according to the thread the function is called from
//    If that thread was not attached as a Java thread,
//	  JNIEnvHandler will attach and deattach as necessary
// 2. Pushes and pops a Java frame so that local references will be cleaned
//    (this is very important for native activities or threads with a native entry point)
//
//	Usage:
//  void foo() {
//    JNIEnvHandler envHandler(32); //16 default, increment as necessary
//    JNIEnv* env = envHandler.Get();
//  }
//
//  If you create functions that return a local java reference:
//	  e.g.   jclass FindClass(JNIEnv* env, const char* className);
//  you should use JNIEnvHandler in the parent scope and pass in the JNIEnv*
//  otherwise you leave the VM the oportunity to collect your returned result
//
class JNIEnvHandler
{
public:
    static JavaVM* m_javaVM;
    static jobject m_javaActivity;

    JNIEnvHandler(long localFrameCount = 16);
    ~JNIEnvHandler();

    inline JNIEnv* Get()		{ return m_env; }
    inline operator JNIEnv* ()	{ return Get(); }
    bool	m_obtainedByAttach;

protected:
    JNIEnv*	m_env;
};


jobject ObtainClassLoader(JNIEnv* env, jobject activity);
jobject ObtainSystemClassLoader(JNIEnv* env);

// worst case Java local reference use count: 8
jclass FindClass(JNIEnv* env, jobject activity, const char* className);

jclass FindClassFromClassLoader(JNIEnv* env, jobject classLoader, const char* className);

std::string GetClassName(JNIEnv* env, jclass clazz);

jstring getJavaString(JNIEnv* env, const char* message);
#endif //SECTION1_APP_B_JNITOOLS_H
