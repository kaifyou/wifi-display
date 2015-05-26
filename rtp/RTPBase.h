<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd"><html><head><title>media/libstagefright/wifi-display/rtp/RTPBase.h - platform/frameworks/av - Git at Google</title><link rel="stylesheet" type="text/css" href="//www.google.com/css/go.css" /><link rel="stylesheet" type="text/css" href="/+static/prettify/prettify.vf-M93Ay4IiiWRQSJKPGWQ.cache.css" /><link rel="stylesheet" type="text/css" href="/+static/gitiles.JPyDgEz4Jj5aQRFU8GjJJQ.cache.css" /><script src="/+static/prettify/prettify_compiled.wQKdUVM0AfatEiTiXCI6lw.cache.js" type="text/javascript"></script></head><body onload="prettyPrint()"><h1><img src="//www.google.com/images/logo_sm.gif" alt="Google" />Git</h1><div class="menu"> <a href="https://www.google.com/accounts/ServiceLogin?service=gerritcodereview&continue=https://android.googlesource.com/login/platform/frameworks/av/%2B/android-4.3_r0.9/media/libstagefright/wifi-display/rtp/RTPBase.h">Sign in</a> </div><div class="breadcrumbs"><a href="/?format=HTML">android</a> / <a href="/platform/frameworks/av/">platform/frameworks/av</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9">android-4.3_r0.9</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/">.</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media">media</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright">libstagefright</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright/wifi-display">wifi-display</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright/wifi-display/rtp">rtp</a> / RTPBase.h</div><div class="sha1">blob: 6178f00d9309734fff6a1b01b2f739db0d96d040 [<a href="/platform/frameworks/av/+log/android-4.3_r0.9/media/libstagefright/wifi-display/rtp/RTPBase.h">file history</a>]</div><pre class="git-blob prettyprint linenums lang-h">/*
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

#ifndef RTP_BASE_H_

#define RTP_BASE_H_

namespace android {

struct RTPBase {
    enum PacketizationMode {
        PACKETIZATION_TRANSPORT_STREAM,
        PACKETIZATION_H264,
        PACKETIZATION_AAC,
        PACKETIZATION_NONE,
    };

    enum TransportMode {
        TRANSPORT_UNDEFINED,
        TRANSPORT_NONE,
        TRANSPORT_UDP,
        TRANSPORT_TCP,
        TRANSPORT_TCP_INTERLEAVED,
    };

    enum {
        // Really UDP _payload_ size
        kMaxUDPPacketSize = 1472,   // 1472 good, 1473 bad on Android@Home
    };

    static int32_t PickRandomRTPPort();
};

}  // namespace android

#endif  // RTP_BASE_H_


</pre><div class="footer">Powered by <a href="https://code.google.com/p/gitiles/">Gitiles</a></div></body></html>