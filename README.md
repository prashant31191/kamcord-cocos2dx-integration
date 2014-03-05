kamcord-cocos2sx-integration
============================

---

This repository provides a step-by-step guide for how to integrate Kamcord into a Cocos2d-x Android project. We will integrate Kamcord with one of the example Cocos2d-x games, SimpleGame.

Assumptions
-----------

This guide assumes that you have downloaded and installed the following software:
  
  1. The [Android SDK](http://developer.android.com/sdk/index.html), which includes a full version of the Eclipse IDE with built-in Android Developer Tools.
  2. The [Android NDK](https://developer.android.com/tools/sdk/ndk/index.html), which allows execution of native C/C++ code within Android.

Be sure to note where in your filesystem the SDK and NDK live, as you will need to provide their directories later on.

---

Step 1: Download Cocos2d-x
--------------------------

First download a clean installation of Cocos2d-x [here](http://www.cocos2d-x.org/download), and unzip the downloaded file. We will refer to the root directory of the unzipped file by `COCOS2DX_ROOT`. We will only be modifying a few files, so the directories you'll need to be familiar with are:

  * `COCOS2DX_ROOT`/samples/Cpp/SimpleGame/Classes/: The code containing the game logic.
  * `COCOS2DX_ROOT`/samples/Cpp/SimpleGame/proj.android/: The Android project which we will be modifying in Eclipse.
  * `COCOS2DX_ROOT`/cocos2dx/platform/android/java/src/: The Java source files for the Cocos2dx library which are used in Android.

We'll be referencing `COCOS2DX_ROOT`/samples/Cpp/SimpleGame/proj.android/ quite a bit later on, so we will denote it by 'PROJ_ROOT'. Once you've downloaded and unzipped this package, you should be in a state identical to the first commit in this repository.

---

Step 2: Set Up the Eclipse Project
----------------------------------

Open Eclipse. Select File > New > Project..., and select "Android Project from Existing Code". Browse to `PROJ_ROOT`, and then click "Finish". The provided Android project is almost ready as-is, but there are a few more things that need to be done. First, we need to tell the build script where our copy of NDK lives. To do this, add the line

	NDK_ROOT="Your/path/to/NDK"

to the file `PROJ_ROOT`/build_native.sh. Be sure to replace `Your/path/to/NDK` with the actual path to the root NDK directory.

Next, we need to link the cocos2dx Java source into our project. To do this, right click on your project in the Package Explorer in Eclipse, and select Properties. Select "Java Build Path" from the left-hand pane, and then select the "Source" tab. Click on "Link Source..." and browse to `COCOS2DX_ROOT`/cocos2dx/platform/android/java/src/. For the folder name, put something other than "src", such as "cocos2dx-src", and then click "Finish". 

The changes made in this step are reflected in the second commit to this repository. All modifications to files were made by Eclipse, with the exception of build_native.sh.

---

Step 3: Integrate Kamcord
-------------------------

#### Import Resources ####

First, we integrate all the necessary Kamcord resources such as layouts, drawables, etc. We start by modifying AndroidManifest.xml in `PROJ_ROOT`. From the MANIFEST_TEMPLATE.XML file in the Kamcord SDK, copy the lines surrounded by KEEP comments into AndroidManifest.xml. This consists of:

  1. the activities used by Kamcord,
  2. the permissions required by Kamcord, and
  3. the features required by Kamcord.

The `<activity>` tags must go inside the `<application>` tag, and the `<uses-feature>` and `<uses-permission>` tags must go outside.

Next, copy all of the Kamcord resources in the res/ folder of the Kamcord SDK into your project's res/ folder. If you already have existing files in res/values/, you will need to merge them with the resources from Kamcord. For example, in SimpleGame, we merge the contents of Kamcord's res/values/string.xml:

    <?xml version="1.0" encoding="utf-8"?>
    <resources>
        <string name="kamcordYoutube">YouTube</string>
        <string name="kamcordTitleHint">Write a title...</string>
        <string name="kamcordTwitter">Twitter</string>
        <string name="kamcordMiddleText">Share</string>
        <string name="kamcordShareTitle">SHARE</string>
        <string name="kamcordFooter">Powered by Kamcord</string>
        <string name="kamcordFacebook">Facebook</string>
        <string name="kamcordEmail">Email</string>
        <string name="kamcordShare">Share Replay</string>
        <string name="kamcordMediaDescription">Media Controls</string>
    </resources>

with the contents of the existing res/values/strings.xml:

    <?xml version="1.0" encoding="utf-8"?>
    <resources>
        <string name="app_name">SimpleGame</string>
    </resources>

to produce the merged res/values/strings.xml:

    <?xml version="1.0" encoding="utf-8"?>
    <resources>
        <string name="app_name">SimpleGame</string>
        <string name="kamcordYoutube">YouTube</string>
        <string name="kamcordTitleHint">Write a title...</string>
        <string name="kamcordTwitter">Twitter</string>
        <string name="kamcordMiddleText">Share</string>
        <string name="kamcordShareTitle">SHARE</string>
        <string name="kamcordFooter">Powered by Kamcord</string>
        <string name="kamcordFacebook">Facebook</string>
        <string name="kamcordEmail">Email</string>
        <string name="kamcordShare">Share Replay</string>
        <string name="kamcordMediaDescription">Media Controls</string>
    </resources>

#### Import Libraries/Codes ####

Now we add the necessary Kamcord libraries to the project. Perform the following copies from the Kamcord SDK into your Android project:

  * kamcord.jar -> `PROJ_ROOT`/libs/
  * MixpanelAPI.jar -> `PROJ_ROOT`/libs/ 
  * android-support-v4.jar -> `PROJ_ROOT`/libs/
  * libkamcord.so -> `PROJ_ROOT`/jni/
  * Kamcord-C-Interface.h -> `PROJ_ROOT`/jni/

To make sure Eclipse knows the new .jars are in the build path, right click on each .jar in the package explorer and select Build Path > Add To Build Path. We also need to tell the native build process to look for the new Kamcord libraries. In `PROJ_ROOT`/jni/Android.mk, add the following lines:

    LOCAL_SHARED_LIBRARIES := libkamcord
    LOCAL_HEADER_FILES := $(LOCAL_PATH)/Kamcord-C-Interface.h

somewhere within the `include $(CLEAR_VARS) ... include $(BUILD_SHARED_LIBRARY)` block, and then insert the following lines:

    include $(CLEAR_VARS)
    LOCAL_MODULE := libkamcord
    LOCAL_SRC_FILES := libkamcord.so
    include $(PREBUILT_SHARED_LIBRARY)

after the first `include $(BUILD_SHARED_LIBRARY)` but before the `$(call ...)` lines.

#### Add Integration Code ####

Finally, we add the necessary codes into the project to enable Kamcord. In `PROJ_ROOT`/jni/hellocpp/main.cpp, add the following lines to `JNI_OnLoad`:

    KamcordAndroid_InitJVM(vm);
    Kamcord_Init("Developer Key",
                 "Secret Key",
                 "App Name",
                 KC_STANDARD_VIDEO_QUALITY);

Next, in `PROJ_ROOT`/src/org/cocos2dx/simplegame/SimpleGame.java, add the following line to `onCreate`:

    Kamcord.initActivity(this);

Finally, add `Kamcord.beginDraw();` and `Kamcord.endDraw();` around the `Cocos2dxRenderer.nativeRender();` call in `COCOS2DX_ROOT`/cocos2dx/platform/android/java/src/org/cocos2dx/lib/Cocos2dxRenderer.java in the `onDrawFrame` function. At this point, Kamcord is fully integrated! Congratulations!

#### Add GUI To Record ####

We still need some way of telling the app to start/stop recording footage. We add the following code block to `COCOS2dX_ROOT`/samples/Cpp/SimpleGame/Classes/HelloWorldScene.cpp:

    // ********************** Codes for Kamcord Integration ***********************
    // Set up start recording label
    CCLabelTTF * startRecordingLabel = CCLabelTTF::create("Start Recording","Artial", 12);
    startRecordingLabel->setColor( ccc3(0, 0, 0) );
    
    CCMenuItemLabel * startRecordingMenuItem = CCMenuItemLabel::create(startRecordingLabel, this, menu_selector(HelloWorld::startRecording));
    
    // Set up stop recording label
    CCLabelTTF * stopRecordingLabel = CCLabelTTF::create("Stop Recording","Artial", 12);
    stopRecordingLabel->setColor( ccc3(0, 0, 0) );
    
    CCMenuItemLabel * stopRecordingMenuItem = CCMenuItemLabel::create(stopRecordingLabel, this, menu_selector(HelloWorld::stopRecording));
    
    // Set up show view label
    CCLabelTTF * showViewLabel = CCLabelTTF::create("Show View","Artial", 12);
    showViewLabel->setColor( ccc3(0, 0, 0) );
    
    CCMenuItemLabel * showViewMenuItem = CCMenuItemLabel::create(showViewLabel, this, menu_selector(HelloWorld::showView));
    
    // Make the menu
    CCMenu* menu = CCMenu::create( startRecordingMenuItem, stopRecordingMenuItem, showViewMenuItem, NULL);
    menu->alignItemsVertically();
    
    this->addChild(menu);
    menu->setPosition(ccp(origin.x + visibleSize.width - 60, origin.y + visibleSize.height - 50));
    // ********************** End Codes for Kamcord Integration ***********************

This code adds "Start Recording", "Stop Recording", and "Show View" buttons to the game screen.

The changes made in this step are reflected in the third commit of this repository. For more detail in what was actually changed, take a look at the diff.