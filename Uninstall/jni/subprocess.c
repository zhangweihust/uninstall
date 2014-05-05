/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <string.h>
#include <jni.h>

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <android/log.h>
#include <unistd.h>
#include <dirent.h>


#include <sys/inotify.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> 

#include <errno.h>


/* 宏定义begin */
//清0宏
#define MEM_ZERO(pDest, destSize) memset(pDest, 0, destSize)

//LOG宏定义
//#define LOG_INFO(tag, msg) __android_log_write(ANDROID_LOG_INFO, tag, msg)
//#define LOG_DEBUG(tag, msg) __android_log_write(ANDROID_LOG_DEBUG, tag, msg)
//#define LOG_WARN(tag, msg) __android_log_write(ANDROID_LOG_WARN, tag, msg)
//#define LOG_ERROR(tag, msg) __android_log_write(ANDROID_LOG_ERROR, tag, msg)

#define LOG_INFO(MY_TAG, ...)  __android_log_print(ANDROID_LOG_INFO, MY_TAG, __VA_ARGS__)
#define LOG_DEBUG(MY_TAG, ...)  __android_log_print(ANDROID_LOG_DEBUG, MY_TAG, __VA_ARGS__)
#define LOG_WARN(MY_TAG, ...)  __android_log_print(ANDROID_LOG_WARN, MY_TAG, __VA_ARGS__)
#define LOG_ERROR(MY_TAG, ...)  __android_log_print(ANDROID_LOG_ERROR, MY_TAG, __VA_ARGS__)


#define MY_TAG  "SubProcess"
#define BUFLEN 1024

/* 内全局变量begin */
static char c_TAG[] = "SubProcess";
static jboolean b_IS_COPY = JNI_TRUE;



//$0 path url version
int main(int argc, char*argv[]){ 
        if(argc!=4){
            LOG_ERROR(MY_TAG,  "argc check failed!!!");
            exit(1);
        }

       char* path = argv[1];
       char* url = argv[2];
       int version = atoi(argv[3]);
       LOG_DEBUG(MY_TAG,"main - path:%s, url:%s, version:%d", path, url, version);

#if 0
        //子进程注册目录监听器
        int fileDescriptor = inotify_init();
        if (fileDescriptor < 0) {
            LOG_ERROR(MY_TAG,  "inotify_init failed !!!");
            exit(1);
        }

        int watchDescriptor;

        watchDescriptor = inotify_add_watch(fileDescriptor, path, IN_DELETE);
        if (watchDescriptor < 0) {
            LOG_ERROR(MY_TAG,  "inotify_add_watch failed !!!!");
            exit(1);
        }

        //分配缓存，以便读取event，缓存大小=一个struct inotify_event的大小，这样一次处理一个event
        void *p_buf = malloc(sizeof(struct inotify_event));
        if (p_buf == NULL) {
            LOG_ERROR(MY_TAG,  "malloc failed !!!");
            exit(1);
        }
        
        //开始监听
        LOG_DEBUG(MY_TAG,  "start observer ... ");
        
        //read会阻塞进程，
        size_t readBytes = read(fileDescriptor, p_buf, sizeof(struct inotify_event));

        //走到这里说明收到目录被删除的事件，注销监听器
        free(p_buf);
        inotify_rm_watch(fileDescriptor, IN_DELETE);
#else
         int fileDescriptor;
         int watchDescriptor;
         void *p_buf;
         size_t readBytes;

         while(1){
               if(access(path, F_OK)){
                    LOG_DEBUG(MY_TAG,  "file(%s) does not exist", path);
                    break;
               }else{
                    //LOG_DEBUG(MY_TAG,  "file(%s)  exists yet", path);
               }
               sleep(10);

               fileDescriptor = inotify_init();
               if (fileDescriptor < 0) {
                   LOG_ERROR(MY_TAG,  "inotify_init failed !!!");
                   exit(1);
               }

               watchDescriptor = inotify_add_watch(fileDescriptor, path, IN_DELETE);
               if (watchDescriptor < 0) {
                   LOG_ERROR(MY_TAG,  "inotify_add_watch failed !!!!");
                   exit(1);
               }

               //分配缓存，以便读取event，缓存大小=一个struct inotify_event的大小，这样一次处理一个event
               p_buf = malloc(sizeof(struct inotify_event));
               if (p_buf == NULL) {
                   LOG_ERROR(MY_TAG,  "malloc failed !!!");
                   exit(1);
               }
               
               //开始监听
               LOG_DEBUG(MY_TAG,  "start observer ... ");
               
               //read会阻塞进程，
               readBytes = read(fileDescriptor, p_buf, sizeof(struct inotify_event));

               //走到这里说明收到目录被删除的事件，注销监听器
               free(p_buf);
               inotify_rm_watch(fileDescriptor, IN_DELETE);
         }
        
#endif

        //目录不存在log
        LOG_DEBUG(MY_TAG,  "uninstalled!");

        /*execl("/system/bin/am", "am", "start", "--user", "0", "-a",
                    "android.intent.action.VIEW", "-d",
                    url, (char *) NULL);*/

        char system_str[200];

        if (version >=17){
            sprintf(system_str, "/system/bin/am start --user 0  -a android.intent.action.VIEW -d %s",  url);

            system(system_str);
        }else{
            sprintf(system_str, "/system/bin/am start  -a android.intent.action.VIEW -d %s",  url);

            system(system_str);
        }


        /*if (version > =17) {
            execl("/system/bin/am", "am", "start", "--user", "0", "-a",
                    "android.intent.action.VIEW", "-d",
                    url, (char *) NULL);
        } else {
            execl("/system/bin/am", "am", "start", "-a", "android.intent.action.VIEW",
                    "-d", url,
                    (char *) NULL);
        }*/
        //扩展：可以执行其他shell命令，am(即activity manager)，可以打开某程序、服务，broadcast intent，等等


}


