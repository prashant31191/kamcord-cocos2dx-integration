LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/HelloWorldScene.cpp \
                   ../../Classes/GameOverScene.cpp
                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_SHARED_LIBRARIES := libkamcord
LOCAL_HEADER_FILES := $(LOCAL_PATH)/Kamcord-C-Interface.h

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static
            
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libkamcord
LOCAL_SRC_FILES := libkamcord.so
include $(PREBUILT_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android)
$(call import-module,cocos2dx)
