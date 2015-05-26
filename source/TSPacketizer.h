<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd"><html><head><title>media/libstagefright/wifi-display/source/TSPacketizer.h - platform/frameworks/av - Git at Google</title><link rel="stylesheet" type="text/css" href="//www.google.com/css/go.css" /><link rel="stylesheet" type="text/css" href="/+static/prettify/prettify.vf-M93Ay4IiiWRQSJKPGWQ.cache.css" /><link rel="stylesheet" type="text/css" href="/+static/gitiles.JPyDgEz4Jj5aQRFU8GjJJQ.cache.css" /><script src="/+static/prettify/prettify_compiled.wQKdUVM0AfatEiTiXCI6lw.cache.js" type="text/javascript"></script></head><body onload="prettyPrint()"><h1><img src="//www.google.com/images/logo_sm.gif" alt="Google" />Git</h1><div class="menu"> <a href="https://www.google.com/accounts/ServiceLogin?service=gerritcodereview&continue=https://android.googlesource.com/login/platform/frameworks/av/%2B/android-4.3_r0.9/media/libstagefright/wifi-display/source/TSPacketizer.h">Sign in</a> </div><div class="breadcrumbs"><a href="/?format=HTML">android</a> / <a href="/platform/frameworks/av/">platform/frameworks/av</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9">android-4.3_r0.9</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/">.</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media">media</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright">libstagefright</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright/wifi-display">wifi-display</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright/wifi-display/source">source</a> / TSPacketizer.h</div><div class="sha1">blob: 4a664ee279196bf3e152629ef0e335b3b62355f9 [<a href="/platform/frameworks/av/+log/android-4.3_r0.9/media/libstagefright/wifi-display/source/TSPacketizer.h">file history</a>]</div><pre class="git-blob prettyprint linenums lang-h">/*
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

#ifndef TS_PACKETIZER_H_

#define TS_PACKETIZER_H_

#include <media/stagefright/foundation/ABase.h>
#include <utils/Errors.h>
#include <utils/RefBase.h>
#include <utils/Vector.h>

namespace android {

struct ABuffer;
struct AMessage;

// Forms the packets of a transport stream given access units.
// Emits metadata tables (PAT and PMT) and timestamp stream (PCR) based
// on flags.
struct TSPacketizer : public RefBase {
    enum {
        EMIT_HDCP20_DESCRIPTOR = 1,
        EMIT_HDCP21_DESCRIPTOR = 2,
    };
    TSPacketizer(uint32_t flags);

    // Returns trackIndex or error.
    ssize_t addTrack(const sp<AMessage> &format);

    enum {
        EMIT_PAT_AND_PMT                = 1,
        EMIT_PCR                        = 2,
        IS_ENCRYPTED                    = 4,
        PREPEND_SPS_PPS_TO_IDR_FRAMES   = 8,
    };
    status_t packetize(
            size_t trackIndex, const sp<ABuffer> &accessUnit,
            sp<ABuffer> *packets,
            uint32_t flags,
            const uint8_t *PES_private_data, size_t PES_private_data_len,
            size_t numStuffingBytes = 0);

    status_t extractCSDIfNecessary(size_t trackIndex);

    // XXX to be removed once encoder config option takes care of this for
    // encrypted mode.
    sp<ABuffer> prependCSD(
            size_t trackIndex, const sp<ABuffer> &accessUnit) const;

protected:
    virtual ~TSPacketizer();

private:
    enum {
        kPID_PMT = 0x100,
        kPID_PCR = 0x1000,
    };

    struct Track;

    uint32_t mFlags;
    Vector<sp<Track> > mTracks;

    Vector<sp<ABuffer> > mProgramInfoDescriptors;

    unsigned mPATContinuityCounter;
    unsigned mPMTContinuityCounter;

    uint32_t mCrcTable[256];

    void initCrcTable();
    uint32_t crc32(const uint8_t *start, size_t size) const;

    DISALLOW_EVIL_CONSTRUCTORS(TSPacketizer);
};

}  // namespace android

#endif  // TS_PACKETIZER_H_

</pre><div class="footer">Powered by <a href="https://code.google.com/p/gitiles/">Gitiles</a></div></body></html>