
#ifndef __KAMCORD_C_INTERFACE_H__
#define __KAMCORD_C_INTERFACE_H__

#ifdef __cplusplus
extern "C" {
#endif

#if __ANDROID__

#include <stdbool.h>
#include <jni.h>

typedef enum KC_VIDEO_QUALITY
{
    KC_STANDARD_VIDEO_QUALITY = 0,
    KC_TRAILER_VIDEO_QUALITY = 1,  // Trailers only. Do not release your game with this setting.
} KC_VIDEO_QUALITY;


void KamcordAndroid_InitJVMWithEnv(JNIEnv * env);
void KamcordAndroid_InitJVM(JavaVM * vm);

void KamcordAndroid_InitActivity(
    JNIEnv* env,
    jobject activity);

void KamcordAndroid_Init(
    JNIEnv* env,
    const char* developerKey,
    const char* developerSecret,
    const char* appName,
    KC_VIDEO_QUALITY videoQuality);

void KamcordAndroid_SetLoggingEnabled(JNIEnv* env, bool value);

void KamcordAndroid_WhitelistAllBoards(JNIEnv* env);

void KamcordAndroid_BlacklistAllBoards(JNIEnv* env);

void KamcordAndroid_WhitelistBoard(JNIEnv* env, const char* boardName);

void KamcordAndroid_BlacklistBoard(JNIEnv* env, const char* boardName);

const char* KamcordAndroid_GetBoard(JNIEnv* env);

const char* KamcordAndroid_Version(JNIEnv* env);

bool KamcordAndroid_IsEnabled(JNIEnv* env);

void KamcordAndroid_StartRecording(JNIEnv* env);

void KamcordAndroid_StopRecording(JNIEnv* env);

void KamcordAndroid_Pause(JNIEnv* env);

void KamcordAndroid_Resume(JNIEnv* env);

bool KamcordAndroid_IsRecording(JNIEnv* env);

bool KamcordAndroid_IsPaused(JNIEnv* env);

void KamcordAndroid_CreateFramebuffers(JNIEnv* env, int width, int height);

void KamcordAndroid_DeleteFramebuffers(JNIEnv* env);

void KamcordAndroid_BeginDraw(JNIEnv* env);

void KamcordAndroid_EndDraw(JNIEnv* env);

void KamcordAndroid_SetVideoTitle(JNIEnv* env, const char* title);

void KamcordAndroid_SetLevelAndScore(
    JNIEnv* env,
    const char* level,
    double score);

void KamcordAndroid_SetVideoQuality(
    JNIEnv* env,
    KC_VIDEO_QUALITY videoQuality);

void KamcordAndroid_ShowView(JNIEnv* env);

void KamcordAndroid_ShowWatchView(JNIEnv* env);

void KamcordAndroid_SetFacebookDescription(
    JNIEnv* env,
    const char* description);

void KamcordAndroid_SetYouTubeSettings(
    JNIEnv* env,
    const char* description,
    const char * tags);

void KamcordAndroid_SetDefaultTweet(
    JNIEnv* env,
    const char* tweet);

void KamcordAndroid_SetTwitterDescription(
    JNIEnv* env,
    const char* twitterDescription);

void KamcordAndroid_SetDefaultEmailSubject(
    JNIEnv* env,
    const char* subject);

void KamcordAndroid_SetDefaultEmailBody(
    JNIEnv* env,
    const char* body);


/*
 *
 * Kamcord initialization. Must be called before you can start recording.
 *
 * @param   developerKey            Your Kamcord developer key.
 * @param   developerSecret         Your Kamcord developerSecret.
 * @param   appName                 The name of your application.
 * @param   KC_VIDEO_QUALITY        The quality level.  Please use
 *                                  KC_STANDARD_VIDEO_QUALITY in production.
 *
 */
void Kamcord_Init(
    const char* developerKey,
    const char* developerSecret,
    const char* appName,
    KC_VIDEO_QUALITY videoQuality);

/*
 * Sets a flag that overrides the internal device whitelist for testing.
 */
void Kamcord_WhitelistAllBoards();

/*
 * Clears the device whitelist.
 */
void Kamcord_BlacklistAllBoards();

/*
 * Adds a specific board (hardware configuration) to the device whitelist.
 * Takes the same sort of board name that is returned by Kamcord_GetBoard()
 *
 * @param  boardName      Name of the board to add.
 */
void Kamcord_WhitelistBoard(const char* boardName);

/*
 * Removes a specific board (hardware configuration) from the device whitelist.
 * Takes the same sort of board name that is returned by Kamcord_GetBoard()
 *
 * @param  boardName      Name of the board to remove.
 */
void Kamcord_BlacklistBoard(const char* boardName);

/*
 * Gets the current board name.  Equivalent to the java: Build.BOARD
 */
const char* Kamcord_GetBoard();

/*
 * Gets a version string from kamcord.jar.
 */
const char* Kamcord_Version();

/*
 *
 * Returns true if and only if Kamcord is enabled. Kamcord is disabled if the version of
 * android is < 4.3, or if the device's board name does not appear on the whitelist.
 *
 */
bool Kamcord_IsEnabled();

/*
 *
 * Start video recording.
 *
 */
void Kamcord_StartRecording();

/*
 *
 * Stop video recording.
 *
 */
void Kamcord_StopRecording();

/*
 *
 * Pause video recording.
 *
 */
void Kamcord_Pause();

/*
 *
 * Resume video recording.
 *
 */
void Kamcord_Resume();

/*
 *
 * Returns true if the video is recording. Note that there might be a slight
 * delay after you call Kamcord_StartRecording() before this method returns true.
 *
 */
bool Kamcord_IsRecording();

/*
 *
 * Returns true if video recording is currently paused.
 *
 */
bool Kamcord_IsPaused();

/*
 *
 * Call this function to force the OpenGL framebuffers Kamcord uses to capture frames of
 * video to allocate.  If you omit this call, they will allocate automatically just in
 * time.
 *
 * @param  width    The width of the framebuffer's renderbuffer.
 * @param  height   The height of the framebuffer's renderbuffer.
 *
 */
void Kamcord_CreateFramebuffers(int width, int height);

/*
 *
 * Call this function to explicitly delete the OpenGL framebuffers Kamcord uses to
 * capture frames of video to allocate.
 *
 */
void Kamcord_DeleteFramebuffers();

/*
 *
 * Surround OpenGL calls that draw graphics which are meant to be captured to video with
 * BeginDraw() and EndDraw().
 *
 */
void Kamcord_BeginDraw();

/*
 *
 * Surround OpenGL calls that draw graphics which are meant to be captured to video with
 * Kamcord_BeginDraw() and Kamcord_EndDraw().
 *
 */
void Kamcord_EndDraw();

/*
 *
 * After every video is recorded (i.e. after you call StopRecording()), you should
 * call this method to set the title for the video in case it is shared.
 *
 * We suggest you set the title to contain some game-specific information such as
 * the level, score, and other relevant game metrics.
 *
 * @param  title   The title of the last recorded video.
 *
 */
void Kamcord_SetVideoTitle(const char* title);

/*
 *
 * Set the level and score for the recorded video.
 * This metadata is used to rank videos in the watch view.
 *
 * @param   level   The level for the last recorded video.
 * @param   score   The score the user just achieved on the given level.
 *
 */
void Kamcord_SetLevelAndScore(
    const char* level,
    double score);

/*
 *
 * Set the video quality to standard or trailer. Please do *NOT* release your game
 * with trailer quality, as it makes immensely large videos with only a slight
 * video quality improvement over standard.
 *
 * The default and recommended quality seting is KC_STANDARD_VIDEO_QUALITY.
 *
 * @param   quality     The desired video quality.
 *
 */
void Kamcord_SetVideoQuality(
    KC_VIDEO_QUALITY videoQuality);

/*
 *
 * Show the Kamcord view, which will let the user share the most
 * recently recorded video.
 *
 */
void Kamcord_ShowView();

/*
 *
 * Show the watch view, which has a feed of videos shared by other users.
 *
 */
void Kamcord_ShowWatchView();

/*
 *
 * Set the description for when the user shares to Facebook.
 *
 * @param   description     Your app's description when a user shares a video on Facebook.
 *
 */
void Kamcord_SetFacebookDescription(
    const char* description);

/*
 *
 * Set the video description and tags for YouTube.
 *
 * @param   description     The video's description when it's shared on YouTube.
 * @param   tags            The video's tags when it's shared on YouTube.
 *
 */
void Kamcord_SetYouTubeSettings(
    const char* description,
    const char * tags);

/*
 *
 * Set the default tweet.
 *
 * @param   tweet           The default tweet.
 *
 */
void Kamcord_SetDefaultTweet(
    const char* tweet);

/*
 *
 * Set the default tweet.
 *
 * @param   tweet           The default tweet.
 *
 */
void Kamcord_SetTwitterDescription(
    const char* twitterDescription);

/*
 *
 * Set the default email subject.
 *
 * @param   subject         The default subject if the user shares via email.
 *
 */
void Kamcord_SetDefaultEmailSubject(
    const char* subject);

/*
 *
 * Set the default email body.
 *
 * @param   body            The default email body if the user shares via email.
 *
 */
void Kamcord_SetDefaultEmailBody(
    const char* body);

#endif // __ANDROID__

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ifndef

