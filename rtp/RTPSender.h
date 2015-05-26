<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd"><html><head><title>media/libstagefright/wifi-display/rtp/RTPSender.h - platform/frameworks/av - Git at Google</title><link rel="stylesheet" type="text/css" href="//www.google.com/css/go.css" /><link rel="stylesheet" type="text/css" href="/+static/prettify/prettify.vf-M93Ay4IiiWRQSJKPGWQ.cache.css" /><link rel="stylesheet" type="text/css" href="/+static/gitiles.JPyDgEz4Jj5aQRFU8GjJJQ.cache.css" /><script src="/+static/prettify/prettify_compiled.wQKdUVM0AfatEiTiXCI6lw.cache.js" type="text/javascript"></script></head><body onload="prettyPrint()"><h1><img src="//www.google.com/images/logo_sm.gif" alt="Google" />Git</h1><div class="menu"> <a href="https://www.google.com/accounts/ServiceLogin?service=gerritcodereview&continue=https://android.googlesource.com/login/platform/frameworks/av/%2B/android-4.3_r0.9/media/libstagefright/wifi-display/rtp/RTPSender.h">Sign in</a> </div><div class="breadcrumbs"><a href="/?format=HTML">android</a> / <a href="/platform/frameworks/av/">platform/frameworks/av</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9">android-4.3_r0.9</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/">.</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media">media</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright">libstagefright</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright/wifi-display">wifi-display</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright/wifi-display/rtp">rtp</a> / RTPSender.h</div><div class="sha1">blob: 7dc138a63aa7ea9464378628a5d3d7f996e7978e [<a href="/platform/frameworks/av/+log/android-4.3_r0.9/media/libstagefright/wifi-display/rtp/RTPSender.h">file history</a>]</div><pre class="git-blob prettyprint linenums lang-h">/*
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

#ifndef RTP_SENDER_H_

#define RTP_SENDER_H_

#include "RTPBase.h"

#include <media/stagefright/foundation/AHandler.h>

namespace android {

struct ABuffer;
struct ANetworkSession;

// An object of this class facilitates sending of media data over an RTP
// channel. The channel is established over a UDP or TCP connection depending
// on which "TransportMode" was chosen. In addition different RTP packetization
// schemes are supported such as "Transport Stream Packets over RTP",
// or "AVC/H.264 encapsulation as specified in RFC 3984 (non-interleaved mode)"
struct RTPSender : public RTPBase, public AHandler {
    enum {
        kWhatInitDone,
        kWhatError,
        kWhatNetworkStall,
    };
    RTPSender(
            const sp<ANetworkSession> &netSession,
            const sp<AMessage> &notify);

    status_t initAsync(
              const char *remoteHost,
              int32_t remoteRTPPort,
              TransportMode rtpMode,
              int32_t remoteRTCPPort,
              TransportMode rtcpMode,
              int32_t *outLocalRTPPort);

    status_t queueBuffer(
            const sp<ABuffer> &buffer,
            uint8_t packetType,
            PacketizationMode mode);

protected:
    virtual ~RTPSender();
    virtual void onMessageReceived(const sp<AMessage> &msg);

private:
    enum {
        kWhatRTPNotify,
        kWhatRTCPNotify,
    };

    enum {
        kMaxNumTSPacketsPerRTPPacket = (kMaxUDPPacketSize - 12) / 188,
        kMaxHistorySize              = 1024,
        kSourceID                    = 0xdeadbeef,
    };

    sp<ANetworkSession> mNetSession;
    sp<AMessage> mNotify;
    TransportMode mRTPMode;
    TransportMode mRTCPMode;
    int32_t mRTPSessionID;
    int32_t mRTCPSessionID;
    bool mRTPConnected;
    bool mRTCPConnected;

    uint64_t mLastNTPTime;
    uint32_t mLastRTPTime;
    uint32_t mNumRTPSent;
    uint32_t mNumRTPOctetsSent;
    uint32_t mNumSRsSent;

    uint32_t mRTPSeqNo;

    List<sp<ABuffer> > mHistory;
    size_t mHistorySize;

    static uint64_t GetNowNTP();

    status_t queueRawPacket(const sp<ABuffer> &tsPackets, uint8_t packetType);
    status_t queueTSPackets(const sp<ABuffer> &tsPackets, uint8_t packetType);
    status_t queueAVCBuffer(const sp<ABuffer> &accessUnit, uint8_t packetType);

    status_t sendRTPPacket(
            const sp<ABuffer> &packet, bool storeInHistory,
            bool timeValid = false, int64_t timeUs = -1ll);

    void onNetNotify(bool isRTP, const sp<AMessage> &msg);

    status_t onRTCPData(const sp<ABuffer> &data);
    status_t parseReceiverReport(const uint8_t *data, size_t size);
    status_t parseTSFB(const uint8_t *data, size_t size);
    status_t parseAPP(const uint8_t *data, size_t size);

    void notifyInitDone(status_t err);
    void notifyError(status_t err);
    void notifyNetworkStall(size_t numBytesQueued);

    DISALLOW_EVIL_CONSTRUCTORS(RTPSender);
};

}  // namespace android

#endif  // RTP_SENDER_H_
</pre><div class="footer">Powered by <a href="https://code.google.com/p/gitiles/">Gitiles</a></div></body></html>