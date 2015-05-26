<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd"><html><head><title>media/libstagefright/wifi-display/ANetworkSession.h - platform/frameworks/av - Git at Google</title><link rel="stylesheet" type="text/css" href="//www.google.com/css/go.css" /><link rel="stylesheet" type="text/css" href="/+static/prettify/prettify.vf-M93Ay4IiiWRQSJKPGWQ.cache.css" /><link rel="stylesheet" type="text/css" href="/+static/gitiles.JPyDgEz4Jj5aQRFU8GjJJQ.cache.css" /><script src="/+static/prettify/prettify_compiled.wQKdUVM0AfatEiTiXCI6lw.cache.js" type="text/javascript"></script></head><body onload="prettyPrint()"><h1><img src="//www.google.com/images/logo_sm.gif" alt="Google" />Git</h1><div class="menu"> <a href="https://www.google.com/accounts/ServiceLogin?service=gerritcodereview&continue=https://android.googlesource.com/login/platform/frameworks/av/%2B/android-4.3_r0.9/media/libstagefright/wifi-display/ANetworkSession.h">Sign in</a> </div><div class="breadcrumbs"><a href="/?format=HTML">android</a> / <a href="/platform/frameworks/av/">platform/frameworks/av</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9">android-4.3_r0.9</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/">.</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media">media</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright">libstagefright</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright/wifi-display">wifi-display</a> / ANetworkSession.h</div><div class="sha1">blob: 7c62b292636b75d76c0d3a9fe63acb14409914f7 [<a href="/platform/frameworks/av/+log/android-4.3_r0.9/media/libstagefright/wifi-display/ANetworkSession.h">file history</a>]</div><pre class="git-blob prettyprint linenums lang-h">/*
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

#ifndef A_NETWORK_SESSION_H_

#define A_NETWORK_SESSION_H_

#include <media/stagefright/foundation/ABase.h>
#include <utils/KeyedVector.h>
#include <utils/RefBase.h>
#include <utils/Thread.h>

#include <netinet/in.h>

namespace android {

struct AMessage;

// Helper class to manage a number of live sockets (datagram and stream-based)
// on a single thread. Clients are notified about activity through AMessages.
struct ANetworkSession : public RefBase {
    ANetworkSession();

    status_t start();
    status_t stop();

    status_t createRTSPClient(
            const char *host, unsigned port, const sp<AMessage> &notify,
            int32_t *sessionID);

    status_t createRTSPServer(
            const struct in_addr &addr, unsigned port,
            const sp<AMessage> &notify, int32_t *sessionID);

    status_t createUDPSession(
            unsigned localPort, const sp<AMessage> &notify, int32_t *sessionID);

    status_t createUDPSession(
            unsigned localPort,
            const char *remoteHost,
            unsigned remotePort,
            const sp<AMessage> &notify,
            int32_t *sessionID);

    status_t connectUDPSession(
            int32_t sessionID, const char *remoteHost, unsigned remotePort);

    // passive
    status_t createTCPDatagramSession(
            const struct in_addr &addr, unsigned port,
            const sp<AMessage> &notify, int32_t *sessionID);

    // active
    status_t createTCPDatagramSession(
            unsigned localPort,
            const char *remoteHost,
            unsigned remotePort,
            const sp<AMessage> &notify,
            int32_t *sessionID);

    status_t destroySession(int32_t sessionID);

    status_t sendRequest(
            int32_t sessionID, const void *data, ssize_t size = -1,
            bool timeValid = false, int64_t timeUs = -1ll);

    enum NotificationReason {
        kWhatError,
        kWhatConnected,
        kWhatClientConnected,
        kWhatData,
        kWhatDatagram,
        kWhatBinaryData,
        kWhatNetworkStall,
    };

protected:
    virtual ~ANetworkSession();

private:
    struct NetworkThread;
    struct Session;

    Mutex mLock;
    sp<Thread> mThread;

    int32_t mNextSessionID;

    int mPipeFd[2];

    KeyedVector<int32_t, sp<Session> > mSessions;

    enum Mode {
        kModeCreateUDPSession,
        kModeCreateTCPDatagramSessionPassive,
        kModeCreateTCPDatagramSessionActive,
        kModeCreateRTSPServer,
        kModeCreateRTSPClient,
    };
    status_t createClientOrServer(
            Mode mode,
            const struct in_addr *addr,
            unsigned port,
            const char *remoteHost,
            unsigned remotePort,
            const sp<AMessage> &notify,
            int32_t *sessionID);

    void threadLoop();
    void interrupt();

    static status_t MakeSocketNonBlocking(int s);

    DISALLOW_EVIL_CONSTRUCTORS(ANetworkSession);
};

}  // namespace android

#endif  // A_NETWORK_SESSION_H_
</pre><div class="footer">Powered by <a href="https://code.google.com/p/gitiles/">Gitiles</a></div></body></html>