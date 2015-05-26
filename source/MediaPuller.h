<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd"><html><head><title>media/libstagefright/wifi-display/source/MediaPuller.h - platform/frameworks/av - Git at Google</title><link rel="stylesheet" type="text/css" href="//www.google.com/css/go.css" /><link rel="stylesheet" type="text/css" href="/+static/prettify/prettify.vf-M93Ay4IiiWRQSJKPGWQ.cache.css" /><link rel="stylesheet" type="text/css" href="/+static/gitiles.JPyDgEz4Jj5aQRFU8GjJJQ.cache.css" /><script src="/+static/prettify/prettify_compiled.wQKdUVM0AfatEiTiXCI6lw.cache.js" type="text/javascript"></script></head><body onload="prettyPrint()"><h1><img src="//www.google.com/images/logo_sm.gif" alt="Google" />Git</h1><div class="menu"> <a href="https://www.google.com/accounts/ServiceLogin?service=gerritcodereview&continue=https://android.googlesource.com/login/platform/frameworks/av/%2B/android-4.3_r0.9/media/libstagefright/wifi-display/source/MediaPuller.h">Sign in</a> </div><div class="breadcrumbs"><a href="/?format=HTML">android</a> / <a href="/platform/frameworks/av/">platform/frameworks/av</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9">android-4.3_r0.9</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/">.</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media">media</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright">libstagefright</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright/wifi-display">wifi-display</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright/wifi-display/source">source</a> / MediaPuller.h</div><div class="sha1">blob: 1291bb3f9ea50410dd5b97479899407e87e0fbc5 [<a href="/platform/frameworks/av/+log/android-4.3_r0.9/media/libstagefright/wifi-display/source/MediaPuller.h">file history</a>]</div><pre class="git-blob prettyprint linenums lang-h">/*
 * Copyright 2012, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MEDIA_PULLER_H_

#define MEDIA_PULLER_H_

#include <media/stagefright/foundation/AHandler.h>

namespace android {

struct MediaSource;

struct MediaPuller : public AHandler {
    enum {
        kWhatEOS,
        kWhatAccessUnit
    };

    MediaPuller(const sp<MediaSource> &source, const sp<AMessage> &notify);

    status_t start();
    void stopAsync(const sp<AMessage> &notify);

    void pause();
    void resume();

protected:
    virtual void onMessageReceived(const sp<AMessage> &msg);
    virtual ~MediaPuller();

private:
    enum {
        kWhatStart,
        kWhatStop,
        kWhatPull,
        kWhatPause,
        kWhatResume,
    };

    sp<MediaSource> mSource;
    sp<AMessage> mNotify;
    int32_t mPullGeneration;
    bool mIsAudio;
    bool mPaused;

    status_t postSynchronouslyAndReturnError(const sp<AMessage> &msg);
    void schedulePull();

    DISALLOW_EVIL_CONSTRUCTORS(MediaPuller);
};

}  // namespace android

#endif  // MEDIA_PULLER_H_
</pre><div class="footer">Powered by <a href="https://code.google.com/p/gitiles/">Gitiles</a></div></body></html>