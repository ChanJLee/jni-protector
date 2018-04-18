//
// Created by caspar on 6/23/16.
//

#ifndef PROTECTOR_H
#define PROTECTOR_H

#include <android/log.h>
#include "scope_obj.h"
#include "scope_string.h"
#include "scope_byte_element.h"
#include "scope_object_element.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif

#ifndef ENV_DEBUG
#define  LOG_TAG    "yarc-jni"
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, \
                   __VA_ARGS__))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, \
                   __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, \
                   __VA_ARGS__))                   
#else
#define LOGE(...) (void*) 0
#define LOGD(...) (void*) 0
#define LOGI(...) (void*) 0
#endif

const int ERROR_ADDRESS = -1;
const int ERROR_INVALID_ELF = -2;

typedef unsigned int address;

/* java function name */
#define JAVA_FUNC__(FUNC) #FUNC
#define JAVA_FUNC(FUNC) JAVA_FUNC__(FUNC)

/* jni function name */
#define JNI_FUNC__(FUNC) o0x_0x_ ## FUNC
#define JNI_FUNC(FUNC) JNI_FUNC__(FUNC)

/* define function */
#define DEFINE_FUNC__(RETURN_TYPE, FUNC, ...) \
    extern "C" { \
    JNIEXPORT RETURN_TYPE \
         JNI_FUNC(FUNC) \
            (JNIEnv* env, jobject thiz, ##__VA_ARGS__);\
    }
#define DEFINE_FUNC(RETURN_TYPE, FUNC, ...) DEFINE_FUNC__(RETURN_TYPE, FUNC, ##__VA_ARGS__)

/* implement function */
#define IMPL_FUNC__(RETURN_TYPE, FUNC, ...)  \
    JNIEXPORT RETURN_TYPE \
       JNI_FUNC(FUNC) \
            (JNIEnv* env, jobject thiz, ##__VA_ARGS__)
#define IMPL_FUNC(RETURN_TYPE, FUNC, ...) IMPL_FUNC__(RETURN_TYPE, FUNC, ##__VA_ARGS__)

/* define a reference that needs to be released(Env->DeleteLocalRef(ref)) */
#define DEFINE_REF__(RETURN_TYPE, NAME, ENV, OBJ) \
    RETURN_TYPE NAME = (OBJ); \
    ScopeLocalRef<RETURN_TYPE> NAME ## _ref(ENV, NAME);
#define DEFINE_REF(RETURN_TYPE, NAME, ENV, OBJ) DEFINE_REF__(RETURN_TYPE, NAME, ENV, OBJ)

/* define string */
#define DEFINE_STR__(NAME, ENV, JSTR) \
    ScopeString NAME ## _scope_str(ENV, JSTR); \
    const char* NAME = NAME ## _scope_str.getCString();
#define DEFINE_STR(NAME, ENV, JSTR) DEFINE_STR__(NAME, ENV, JSTR)

#define DEFINE_BYTE_ELEMENT__(NAME, ENV, ARR) \
    ScopeByteElement NAME ## _scope_byte_element(ENV, ARR); \
    const jbyte * NAME = NAME ## _scope_byte_element.getBytes();
#define DEFINE_BYTE_ELEMENT(NAME, ENV, ARR) DEFINE_BYTE_ELEMENT__(NAME, ENV, ARR)

#define DEFINE_OBJECT_ELEMENT__(NAME, ENV, ARR, INDEX) \
    ScopeObjectElement NAME ## _scope_object_element(ENV, ARR, INDEX); \
    const jobject NAME = NAME ## _scope_object_element.getObject();
#define DEFINE_OBJECT_ELEMENT(NAME, ENV, ARR, INDEX) DEFINE_OBJECT_ELEMENT__(NAME, ENV, ARR, INDEX)

DEFINE_FUNC(jint, load);

#endif //PROTECTOR_H
