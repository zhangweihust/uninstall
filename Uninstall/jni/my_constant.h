#include <android/log.h>


//#define LIB_SUBSTRACE_PATH "/system/lib/libsubstrate.so"
#define LIB_SUBSTRACE_PATH "/data/yyl/libsubstrate.so"
#define LIB_GHOST_PATH "/data/yyl/libghost.so"
#define LIB_HOOK_PATH "/data/yyl/libhook.so"


#define LOGI(MY_TAG, ...)  __android_log_print(ANDROID_LOG_INFO, MY_TAG, __VA_ARGS__)
#define LOGW(MY_TAG, ...)  __android_log_print(ANDROID_LOG_WARN, MY_TAG, __VA_ARGS__)
#define LOGE(MY_TAG, ...)  __android_log_print(ANDROID_LOG_ERROR, MY_TAG, __VA_ARGS__)

//#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,  __VA_ARGS__)
//#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN,  __VA_ARGS__)
//#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, __VA_ARGS__)
//
