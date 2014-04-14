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


#define UNIQUE_SEM "mysem"
#define BUFLEN 1024

#define PROCESS_NAME "uninstall_watcher"

/* 内全局变量begin */
static char c_TAG[] = "onEvent";
static jboolean b_IS_COPY = JNI_TRUE;

int isNum(char* str){
    if(str!=NULL){
        int len = strlen(str);
        int i = 0;
        for(i=0; i<len; i++){
            if(str[i]>'9' || str[i]<'0'){
                return 0;
            }
        }

        return 1;
    }

    return 0;
}


int checkCmdLine(char* processName, char* input){
    //LOG_ERROR("zhw", "checkCmdLine - processName:%s, input:%s", processName, input);
    int p_len = strlen(processName);
    int i_len = strlen(input);
    if(p_len>0 && i_len>=p_len){
        int ret = strncmp(processName, input, p_len);
        if(ret==0){
            if(i_len>p_len){
                if(input[p_len]==':'){
                    return 0;
                }else{
                    return 1;
                }
            }else{
               return 1;
            }
        }
    }

    return 0;
}


int getMatchProcessNum(char* process_name){

    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;

    //char* path_t = malloc(100*sizeof(char));
    char path_t[100];
    char buffer[BUFLEN] ;


    char* dir = "/proc";
    int count = 0;

    if((dp=opendir(dir)) == NULL) {
        LOG_ERROR("zhw", "cannot open directory:%s", dir);
        return 0;
    }

    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode) && isNum(entry->d_name)) {
            //  /proc/pid/cmdline
            memset(path_t, 0 , sizeof(path_t));
            strcat(path_t, "/proc/");
            strcat(path_t, entry->d_name);
            strcat(path_t, "/cmdline");
            //LOG_ERROR("zhw", "path:%s", path_t);
            int fd_t = open(path_t, O_RDONLY);
            if(fd_t>0){
                int read_count = read(fd_t, buffer, BUFLEN);
                if(read_count>0){
                    if(checkCmdLine(process_name, buffer)){
                        count++;
                    }
                }
                
                
                    
            }


        }
    }



    return count;


}

/**
*   eg:
*   path :  /data/data/<package>/lib
*   url:    http://www.baidu.com
*   version:  16
*/
jstring Java_com_example_uninstall_UninstallObserver_startWork(JNIEnv* env,
        jobject thiz, jstring command_path, jstring watch_path , jstring url, jint 
        _version) {
        jstring tag = (*env)->NewStringUTF(env, c_TAG);

  int version = _version;
   LOG_DEBUG("zhw", "startWork - command_path:%s, watch_path:%s, url:%s,   version:%d",
           (*env)->GetStringUTFChars(env, command_path, (char *)NULL), 
           (*env)->GetStringUTFChars(env, watch_path, (char *)NULL),
           (*env)->GetStringUTFChars(env, url, (char *)NULL),
           version);

    int num = getMatchProcessNum(PROCESS_NAME);
    LOG_ERROR("zhw", "getMatchProcessNum ret:%d", num);

    if(num>=1){
        return (*env)->NewStringUTF(env, "Hello from JNI !");
    }

    //初始化log
    LOG_DEBUG((*env)->GetStringUTFChars(env, tag, (char *)NULL),
            (*env)->GetStringUTFChars(env, (*env)->NewStringUTF(env, "init OK"),
                    (char *)NULL));

    //fork子进程，以执行轮询任务
    pid_t pid = fork();
    if (pid < 0) {
        //出错log
        LOG_ERROR((*env)->GetStringUTFChars(env, tag, (char *)NULL),
                (*env)->GetStringUTFChars(env,
                        (*env)->NewStringUTF(env, "fork failed !!!"),
                        (char *)NULL));
    } else if (pid == 0) {
         LOG_DEBUG("zhw", "in child process");
#if 1
        char command[100];
        memset(command, 0 , sizeof(command));

        LOG_DEBUG("zhw", "in child process2");
        strcat(command, (*env)->GetStringUTFChars(env, command_path, &b_IS_COPY));
        strcat(command, "/");
        strcat(command, PROCESS_NAME);
        LOG_DEBUG("zhw", "%s %s %s %d", command, 
                                              (*env)->GetStringUTFChars(env,  watch_path, &b_IS_COPY),  
                                              (*env)->GetStringUTFChars(env,  url, &b_IS_COPY), 
                                              version);
        LOG_DEBUG("zhw", "in child process3");
        char version_str[20];
        sprintf(version_str,"%d", version);
        execl(command, PROCESS_NAME ,  (*env)->GetStringUTFChars(env, watch_path, (char *)NULL), 
                                                                   (*env)->GetStringUTFChars(env, url, (char *)NULL), 
                                                                   version_str, 
                                                                   (char *)NULL);
       ///data/data/com.example.uninstall/files/uninstall_watcher /data/data/com.example.uninstall/lib http://www.baidu.com 16
       /*execl("/data/data/com.example.uninstall/files/uninstall_watcher", 
                                 "uninstall_watcher" ,  
                                 "/data/data/com.example.uninstall/lib", 
                                 "http://www.baidu.com", 
                                  "16", 
                                  (char *)NULL);*/


         LOG_DEBUG("zhw", "execl:%s", command);
         exit(1);
#endif

    } else {
        //父进程直接退出，使子进程被init进程领养，以避免子进程僵死
    }


    return (*env)->NewStringUTF(env, "Hello from JNI !");
}

