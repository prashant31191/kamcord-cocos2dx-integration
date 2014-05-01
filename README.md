kamcord-cocos2dx-integration
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

<p align="center">
<img src="https://s3.amazonaws.com/kamcord-static/img/documentation/Android/cocos2dx-build_native.sh-step2diff1.png" alt="Changes to build_native.sh" />
</p>

Next, we need to link the cocos2dx Java source into our project. To do this, right click on your project in the Package Explorer in Eclipse, and select Properties. Select "Java Build Path" from the left-hand pane, and then select the "Source" tab. Click on "Link Source..." and browse to `COCOS2DX_ROOT`/cocos2dx/platform/android/java/src/. For the folder name, put something other than "src", such as "cocos2dx-src", and then click "Finish". 

To see the changes that are made in this step, use

	git diff 8db60e787a4b90808f8d403c7569f35d4d097e88 b08c9cd47e4f490628202093aefac9bd175ca02d

All modifications to files were made by Eclipse, with the exception of build_native.sh.

---

Step 3: Integrate Kamcord
-------------------------

#### Import Resources ####

First, we integrate all the necessary Kamcord resources such as layouts, drawables, etc. We start by modifying AndroidManifest.xml in `PROJ_ROOT`. From the MANIFEST_TEMPLATE.XML file in the Kamcord SDK, copy the lines surrounded by KEEP comments into AndroidManifest.xml. This consists of:

  1. the activities used by Kamcord,
  2. the permissions required by Kamcord, and
  3. the features required by Kamcord.

The `<activity>` tags must go inside the `<application>` tag, and the `<uses-feature>` and `<uses-permission>` tags must go outside.

Next, copy all of the Kamcord resources in the res/ folder of the Kamcord SDK into your project's res/ folder.

#### Import Libraries/Codes ####

Now we add the necessary Kamcord libraries to the project. Perform the following copies from the Kamcord SDK into your Android project:

  * kamcord.jar -> `PROJ_ROOT`/libs/
  * libkamcord.so -> `PROJ_ROOT`/jni/
  * Kamcord-C-Interface.h -> `PROJ_ROOT`/jni/

To make sure Eclipse knows the new .jar is in the build path, right click on kamcord.jar in the package explorer and select Build Path > Add To Build Path. We also need to tell the native build process to look for the new Kamcord libraries. In `PROJ_ROOT`/jni/Android.mk, add the following lines:

    LOCAL_SHARED_LIBRARIES := libkamcord
    LOCAL_HEADER_FILES := $(LOCAL_PATH)/Kamcord-C-Interface.h

somewhere within the `include $(CLEAR_VARS) ... include $(BUILD_SHARED_LIBRARY)` block, and then insert the following lines:

    include $(CLEAR_VARS)
    LOCAL_MODULE := libkamcord
    LOCAL_SRC_FILES := libkamcord.so
    include $(PREBUILT_SHARED_LIBRARY)

after the first `include $(BUILD_SHARED_LIBRARY)` but before the `$(call ...)` lines.

<p align="center">
<img src="https://s3.amazonaws.com/kamcord-static/img/documentation/Android/cocos2dx-Android.mk-step3diff8.png" alt="Changes to Android.mk" />
</p>

<p align="center">
<img src="https://s3.amazonaws.com/kamcord-static/img/documentation/Android/cocos2dx-Android.mk-step3diff9.png" alt="Changes to Android.mk" />
</p>

#### Add Integration Code ####

Finally, we add the necessary codes into the project to enable Kamcord. In `PROJ_ROOT`/jni/hellocpp/main.cpp, add the following lines to `JNI_OnLoad`:

    KamcordAndroid_InitJVM(vm);

<p align="center">
<img src="https://s3.amazonaws.com/kamcord-static/img/documentation/Android/cocos2dx-main.cpp-step3diff10.png" alt="Changes to main.cpp" />
</p>

Next, in `PROJ_ROOT`/src/org/cocos2dx/simplegame/SimpleGame.java, add the following line to `onCreate`:

    Kamcord.initActivity(this);
    Kamcord.initKeyAndSecret("e5Wh6FfCEq0eqjoZJ4Ksqcql0v0j4Qkxr7dX7QI0ALV",
        "7PVfe087qj20lq13Y16UJ6cre3be49afKFKAiO6VYOH",
        "RippleTest",
        Kamcord.STANDARD);
        
<p align="center">
<img src="https://s3.amazonaws.com/kamcord-static/img/documentation/Android/cocos2dx-SimpleGame.java-step3diff11.png" alt="Changes to SimpleGame.java" />
</p>

Finally, add `Kamcord.beginDraw();` and `Kamcord.endDraw();` around the `Cocos2dxRenderer.nativeRender();` call in `COCOS2DX_ROOT`/cocos2dx/platform/android/java/src/org/cocos2dx/lib/Cocos2dxRenderer.java in the `onDrawFrame` function.

<p align="center">
<img src="https://s3.amazonaws.com/kamcord-static/img/documentation/Android/cocos2dx-Cocos2dxRenderer.java-step3diff1.png" alt="Changes to Cocos2dxRenderer.java" />
</p>

At this point, Kamcord is fully integrated! Congratulations!

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

<p align="center">
<img src="https://s3.amazonaws.com/kamcord-static/img/documentation/Android/cocos2dx-HelloWorldScene.cpp-step3diff3.png" alt="Changes to HelloWorldScene.cpp" />
</p>

This code adds "Start Recording", "Stop Recording", and "Show View" buttons to the game screen. The last thing we need to do is add the calls to the correct Kamcord functions for starting and stopping a recording, and presenting the Kamcord UI. Inside the HelloWorld class, we put the following lines:

    void HelloWorld::startRecording(CCObject * pSender)
    {
        Kamcord_StartRecording();
    }
    
    void HelloWorld::stopRecording(CCObject * pSender)
    {
        Kamcord_StopRecording();
    }
    
    void HelloWorld::showView(CCObject * pSender)
    {
        Kamcord_ShowView();
    }
    
<p align="center">
<img src="https://s3.amazonaws.com/kamcord-static/img/documentation/Android/cocos2dx-HelloWorldScene.cpp-step3diff4.png" alt="Changes to HelloWorldScene.cpp" />
</p>
    
Finally, we added the corresponding function headers to HelloWorld.h:

    void startRecording(CCObject * pSender);
    void stopRecording(CCObject * pSender);
    void showView(CCObject * pSender);
    
<p align="center">
<img src="https://s3.amazonaws.com/kamcord-static/img/documentation/Android/cocos2dx-HelloWorldScene.h-step3diff5.png" alt="Changes to HelloWorldScene.h" />
</p>

To see the changes that are made in this step, use

	git diff b08c9cd47e4f490628202093aefac9bd175ca02d ae3bcb0b1cca3edaabacd354aa9be00ee606f14c

