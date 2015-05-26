<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd"><html><head><title>media/libstagefright/wifi-display/VideoFormats.h - platform/frameworks/av - Git at Google</title><link rel="stylesheet" type="text/css" href="//www.google.com/css/go.css" /><link rel="stylesheet" type="text/css" href="/+static/prettify/prettify.vf-M93Ay4IiiWRQSJKPGWQ.cache.css" /><link rel="stylesheet" type="text/css" href="/+static/gitiles.JPyDgEz4Jj5aQRFU8GjJJQ.cache.css" /><script src="/+static/prettify/prettify_compiled.wQKdUVM0AfatEiTiXCI6lw.cache.js" type="text/javascript"></script></head><body onload="prettyPrint()"><h1><img src="//www.google.com/images/logo_sm.gif" alt="Google" />Git</h1><div class="menu"> <a href="https://www.google.com/accounts/ServiceLogin?service=gerritcodereview&continue=https://android.googlesource.com/login/platform/frameworks/av/%2B/android-4.3_r0.9/media/libstagefright/wifi-display/VideoFormats.h">Sign in</a> </div><div class="breadcrumbs"><a href="/?format=HTML">android</a> / <a href="/platform/frameworks/av/">platform/frameworks/av</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9">android-4.3_r0.9</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/">.</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media">media</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright">libstagefright</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright/wifi-display">wifi-display</a> / VideoFormats.h</div><div class="sha1">blob: 01de24638d8d93a66f6b613b163025c3c06455dc [<a href="/platform/frameworks/av/+log/android-4.3_r0.9/media/libstagefright/wifi-display/VideoFormats.h">file history</a>]</div><pre class="git-blob prettyprint linenums lang-h">/*
 * Copyright 2013, The Android Open Source Project
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

#ifndef VIDEO_FORMATS_H_

#define VIDEO_FORMATS_H_

#include <media/stagefright/foundation/ABase.h>

#include <stdint.h>

namespace android {

struct AString;

// This class encapsulates that video resolution capabilities of a wfd source
// or sink as outlined in the wfd specs. Currently three sets of resolutions
// are specified, each of which supports up to 32 resolutions.
// In addition to its capabilities each sink/source also publishes its
// "native" resolution, presumably one that is preferred among all others
// because it wouldn&#39;t require any scaling and directly corresponds to the
// display capabilities/pixels.
struct VideoFormats {
    VideoFormats();

    struct config_t {
        size_t width, height, framesPerSecond;
        bool interlaced;
        unsigned char profile, level;
    };

    enum ProfileType {
        PROFILE_CBP = 0,
        PROFILE_CHP,
        kNumProfileTypes,
    };

    enum LevelType {
        LEVEL_31 = 0,
        LEVEL_32,
        LEVEL_40,
        LEVEL_41,
        LEVEL_42,
        kNumLevelTypes,
    };

    enum ResolutionType {
        RESOLUTION_CEA,
        RESOLUTION_VESA,
        RESOLUTION_HH,
        kNumResolutionTypes,
    };

    void setNativeResolution(ResolutionType type, size_t index);
    void getNativeResolution(ResolutionType *type, size_t *index) const;

    void disableAll();
    void enableAll();

    void setResolutionEnabled(
            ResolutionType type, size_t index, bool enabled = true);

    bool isResolutionEnabled(ResolutionType type, size_t index) const;

    static bool GetConfiguration(
            ResolutionType type, size_t index,
            size_t *width, size_t *height, size_t *framesPerSecond,
            bool *interlaced);

    bool parseFormatSpec(const char *spec);
    AString getFormatSpec(bool forM4Message = false) const;

    static bool PickBestFormat(
            const VideoFormats &sinkSupported,
            const VideoFormats &sourceSupported,
            ResolutionType *chosenType,
            size_t *chosenIndex);

private:
    bool parseH264Codec(const char *spec);
    ResolutionType mNativeType;
    size_t mNativeIndex;

    uint32_t mResolutionEnabled[kNumResolutionTypes];
    static config_t mConfigs[kNumResolutionTypes][32];

    DISALLOW_EVIL_CONSTRUCTORS(VideoFormats);
};

}  // namespace android

#endif  // VIDEO_FORMATS_H_

</pre><div class="footer">Powered by <a href="https://code.google.com/p/gitiles/">Gitiles</a></div></body></html>