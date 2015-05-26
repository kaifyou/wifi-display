<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd"><html><head><title>media/libstagefright/wifi-display/source/Converter.h - platform/frameworks/av - Git at Google</title><link rel="stylesheet" type="text/css" href="//www.google.com/css/go.css" /><link rel="stylesheet" type="text/css" href="/+static/prettify/prettify.vf-M93Ay4IiiWRQSJKPGWQ.cache.css" /><link rel="stylesheet" type="text/css" href="/+static/gitiles.JPyDgEz4Jj5aQRFU8GjJJQ.cache.css" /><script src="/+static/prettify/prettify_compiled.wQKdUVM0AfatEiTiXCI6lw.cache.js" type="text/javascript"></script></head><body onload="prettyPrint()"><h1><img src="//www.google.com/images/logo_sm.gif" alt="Google" />Git</h1><div class="menu"> <a href="https://www.google.com/accounts/ServiceLogin?service=gerritcodereview&continue=https://android.googlesource.com/login/platform/frameworks/av/%2B/android-4.3_r0.9/media/libstagefright/wifi-display/source/Converter.h">Sign in</a> </div><div class="breadcrumbs"><a href="/?format=HTML">android</a> / <a href="/platform/frameworks/av/">platform/frameworks/av</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9">android-4.3_r0.9</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/">.</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media">media</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright">libstagefright</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright/wifi-display">wifi-display</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright/wifi-display/source">source</a> / Converter.h</div><div class="sha1">blob: ba297c4227813800c153e316261a389ca490e39a [<a href="/platform/frameworks/av/+log/android-4.3_r0.9/media/libstagefright/wifi-display/source/Converter.h">file history</a>]</div><pre class="git-blob prettyprint linenums lang-h">/*
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

#ifndef CONVERTER_H_

#define CONVERTER_H_

#include "WifiDisplaySource.h"

#include <media/stagefright/foundation/AHandler.h>

namespace android {

struct ABuffer;
struct MediaCodec;

#define ENABLE_SILENCE_DETECTION        0

// Utility class that receives media access units and converts them into
// media access unit of a different format.
// Right now this&#39;ll convert raw video into H.264 and raw audio into AAC.
struct Converter : public AHandler {
    Converter(
            const sp<AMessage> &notify,
            const sp<ALooper> &codecLooper,
            const sp<AMessage> &format,
            bool usePCMAudio);

    status_t initCheck() const;

    size_t getInputBufferCount() const;

    sp<AMessage> getOutputFormat() const;
    bool needToManuallyPrependSPSPPS() const;

    void feedAccessUnit(const sp<ABuffer> &accessUnit);
    void signalEOS();

    void requestIDRFrame();

    void dropAFrame();

    enum {
        kWhatAccessUnit,
        kWhatEOS,
        kWhatError,
    };

    enum {
        kWhatDoMoreWork,
        kWhatRequestIDRFrame,
        kWhatShutdown,
        kWhatMediaPullerNotify,
        kWhatEncoderActivity,
        kWhatDropAFrame,
    };

    void shutdownAsync();

    int32_t getVideoBitrate() const;
    void setVideoBitrate(int32_t bitrate);

    static int32_t GetInt32Property(const char *propName, int32_t defaultValue);

protected:
    virtual ~Converter();
    virtual void onMessageReceived(const sp<AMessage> &msg);

private:
    status_t mInitCheck;
    sp<AMessage> mNotify;
    sp<ALooper> mCodecLooper;
    sp<AMessage> mInputFormat;
    bool mIsVideo;
    bool mIsPCMAudio;
    sp<AMessage> mOutputFormat;
    bool mNeedToManuallyPrependSPSPPS;

    sp<MediaCodec> mEncoder;
    sp<AMessage> mEncoderActivityNotify;

    Vector<sp<ABuffer> > mEncoderInputBuffers;
    Vector<sp<ABuffer> > mEncoderOutputBuffers;

    List<size_t> mAvailEncoderInputIndices;

    List<sp<ABuffer> > mInputBufferQueue;

    bool mDoMoreWorkPending;

#if ENABLE_SILENCE_DETECTION
    int64_t mFirstSilentFrameUs;
    bool mInSilentMode;
#endif

    sp<ABuffer> mPartialAudioAU;

    int32_t mPrevVideoBitrate;

    int32_t mNumFramesToDrop;

    status_t initEncoder();
    void releaseEncoder();

    status_t feedEncoderInputBuffers();

    void scheduleDoMoreWork();
    status_t doMoreWork();

    void notifyError(status_t err);

    // Packetizes raw PCM audio data available in mInputBufferQueue
    // into a format suitable for transport stream inclusion and
    // notifies the observer.
    status_t feedRawAudioInputBuffers();

    static bool IsSilence(const sp<ABuffer> &accessUnit);

    DISALLOW_EVIL_CONSTRUCTORS(Converter);
};

}  // namespace android

#endif  // CONVERTER_H_
</pre><div class="footer">Powered by <a href="https://code.google.com/p/gitiles/">Gitiles</a></div></body></html>