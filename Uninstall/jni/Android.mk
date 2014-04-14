# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
APP_OPTIM:= debug

LOCAL_PATH := $(call my-dir)
LOCAL_CFLAGS := -g
LOCAL_CFLAGS += -ggdb
LOCAL_CFLAGS += -fPIC


include $(CLEAR_VARS)
LOCAL_MODULE    := observer 
LOCAL_SRC_FILES	:=	observer.c
#LOCAL_C_INCLUDES += system/core/include/cutils
#LOCAL_SHARED_LIBRARIES := libcutil
#LOCAL_SHARED_LIBRARIES := substrate
LOCAL_LDLIBS := -L$(LOCAL_PATH) -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := uninstall_watcher
LOCAL_SRC_FILES :=  subprocess.c 
LOCAL_LDLIBS := -L$(LOCAL_PATH) -llog
include $(BUILD_EXECUTABLE)




