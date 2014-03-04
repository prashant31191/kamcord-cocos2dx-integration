#include "AppDelegate.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>

#include "HelloWorldScene.h"
#include "Kamcord-C-Interface.h"

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

using namespace cocos2d;

extern "C"
{

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JniHelper::setJavaVM(vm);

    KamcordAndroid_InitJVM(vm);
    Kamcord_Init("e5Wh6FfCEq0eqjoZJ4Ksqcql0v0j4Qkxr7dX7QI0ALV",
                 "7PVfe087qj20lq13Y16UJ6cre3be49afKFKAiO6VYOH",
                 "RippleTest",
                 KC_STANDARD_VIDEO_QUALITY);

    return JNI_VERSION_1_4;
}

void Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInit(JNIEnv*  env, jobject thiz, jint w, jint h)
{
    if (!CCDirector::sharedDirector()->getOpenGLView())
    {
        CCEGLView *view = CCEGLView::sharedOpenGLView();
        view->setFrameSize(w, h);

        AppDelegate *pAppDelegate = new AppDelegate();
        CCApplication::sharedApplication()->run();
    }
    else
    {
        ccGLInvalidateStateCache();
        CCShaderCache::sharedShaderCache()->reloadDefaultShaders();
        ccDrawInit();
        CCTextureCache::reloadAllTextures();
        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_COME_TO_FOREGROUND, NULL);
        CCDirector::sharedDirector()->setGLDefaultValues(); 
    }
}

}
