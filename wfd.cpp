<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd"><html><head><title>media/libstagefright/wifi-display/wfd.cpp - platform/frameworks/av - Git at Google</title><link rel="stylesheet" type="text/css" href="//www.google.com/css/go.css" /><link rel="stylesheet" type="text/css" href="/+static/prettify/prettify.vf-M93Ay4IiiWRQSJKPGWQ.cache.css" /><link rel="stylesheet" type="text/css" href="/+static/gitiles.JPyDgEz4Jj5aQRFU8GjJJQ.cache.css" /><script src="/+static/prettify/prettify_compiled.wQKdUVM0AfatEiTiXCI6lw.cache.js" type="text/javascript"></script></head><body onload="prettyPrint()"><h1><img src="//www.google.com/images/logo_sm.gif" alt="Google" />Git</h1><div class="menu"> <a href="https://www.google.com/accounts/ServiceLogin?service=gerritcodereview&continue=https://android.googlesource.com/login/platform/frameworks/av/%2B/android-4.3_r0.9/media/libstagefright/wifi-display/wfd.cpp">Sign in</a> </div><div class="breadcrumbs"><a href="/?format=HTML">android</a> / <a href="/platform/frameworks/av/">platform/frameworks/av</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9">android-4.3_r0.9</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/">.</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media">media</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright">libstagefright</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright/wifi-display">wifi-display</a> / wfd.cpp</div><div class="sha1">blob: c9477651e865814bc4d8ef443846b2914af07871 [<a href="/platform/frameworks/av/+log/android-4.3_r0.9/media/libstagefright/wifi-display/wfd.cpp">file history</a>]</div><pre class="git-blob prettyprint linenums lang-cpp">/*
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

//#define LOG_NDEBUG 0
#define LOG_TAG "wfd"
#include <utils/Log.h>

#include "source/WifiDisplaySource.h"

#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <gui/ISurfaceComposer.h>
#include <gui/SurfaceComposerClient.h>
#include <media/AudioSystem.h>
#include <media/IMediaPlayerService.h>
#include <media/IRemoteDisplay.h>
#include <media/IRemoteDisplayClient.h>
#include <media/stagefright/DataSource.h>
#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/foundation/AMessage.h>
#include <ui/DisplayInfo.h>

namespace android {

static void usage(const char *me) {
    fprintf(stderr,
            "usage:\n"
            "           %s -l iface[:port]\tcreate a wifi display source\n"
            "               -f(ilename)  \tstream media\n",
            me);
}

struct RemoteDisplayClient : public BnRemoteDisplayClient {
    RemoteDisplayClient();

    virtual void onDisplayConnected(
            const sp<IGraphicBufferProducer> &bufferProducer,
            uint32_t width,
            uint32_t height,
            uint32_t flags);

    virtual void onDisplayDisconnected();
    virtual void onDisplayError(int32_t error);

    void waitUntilDone();

protected:
    virtual ~RemoteDisplayClient();

private:
    Mutex mLock;
    Condition mCondition;

    bool mDone;

    sp<SurfaceComposerClient> mComposerClient;
    sp<IGraphicBufferProducer> mSurfaceTexture;
    sp<IBinder> mDisplayBinder;

    DISALLOW_EVIL_CONSTRUCTORS(RemoteDisplayClient);
};

RemoteDisplayClient::RemoteDisplayClient()
    : mDone(false) {
    mComposerClient = new SurfaceComposerClient;
    CHECK_EQ(mComposerClient->initCheck(), (status_t)OK);
}

RemoteDisplayClient::~RemoteDisplayClient() {
}

void RemoteDisplayClient::onDisplayConnected(
        const sp<IGraphicBufferProducer> &bufferProducer,
        uint32_t width,
        uint32_t height,
        uint32_t flags) {
    ALOGI("onDisplayConnected width=%u, height=%u, flags = 0x%08x",
          width, height, flags);

    if (bufferProducer != NULL) {
        mSurfaceTexture = bufferProducer;
        mDisplayBinder = mComposerClient->createDisplay(
                String8("foo"), false /* secure */);

        SurfaceComposerClient::openGlobalTransaction();
        mComposerClient->setDisplaySurface(mDisplayBinder, mSurfaceTexture);

        Rect layerStackRect(1280, 720);  // XXX fix this.
        Rect displayRect(1280, 720);

        mComposerClient->setDisplayProjection(
                mDisplayBinder, 0 /* 0 degree rotation */,
                layerStackRect,
                displayRect);

        SurfaceComposerClient::closeGlobalTransaction();
    }
}

void RemoteDisplayClient::onDisplayDisconnected() {
    ALOGI("onDisplayDisconnected");

    Mutex::Autolock autoLock(mLock);
    mDone = true;
    mCondition.broadcast();
}

void RemoteDisplayClient::onDisplayError(int32_t error) {
    ALOGI("onDisplayError error=%d", error);

    Mutex::Autolock autoLock(mLock);
    mDone = true;
    mCondition.broadcast();
}

void RemoteDisplayClient::waitUntilDone() {
    Mutex::Autolock autoLock(mLock);
    while (!mDone) {
        mCondition.wait(mLock);
    }
}

static status_t enableAudioSubmix(bool enable) {
    status_t err = AudioSystem::setDeviceConnectionState(
            AUDIO_DEVICE_IN_REMOTE_SUBMIX,
            enable
                ? AUDIO_POLICY_DEVICE_STATE_AVAILABLE
                : AUDIO_POLICY_DEVICE_STATE_UNAVAILABLE,
            NULL /* device_address */);

    if (err != OK) {
        return err;
    }

    err = AudioSystem::setDeviceConnectionState(
            AUDIO_DEVICE_OUT_REMOTE_SUBMIX,
            enable
                ? AUDIO_POLICY_DEVICE_STATE_AVAILABLE
                : AUDIO_POLICY_DEVICE_STATE_UNAVAILABLE,
            NULL /* device_address */);

    return err;
}

static void createSource(const AString &addr, int32_t port) {
    sp<IServiceManager> sm = defaultServiceManager();
    sp<IBinder> binder = sm->getService(String16("media.player"));
    sp<IMediaPlayerService> service =
        interface_cast<IMediaPlayerService>(binder);

    CHECK(service.get() != NULL);

    enableAudioSubmix(true /* enable */);

    String8 iface;
    iface.append(addr.c_str());
    iface.append(StringPrintf(":%d", port).c_str());

    sp<RemoteDisplayClient> client = new RemoteDisplayClient;
    sp<IRemoteDisplay> display = service->listenForRemoteDisplay(client, iface);

    client->waitUntilDone();

    display->dispose();
    display.clear();

    enableAudioSubmix(false /* enable */);
}

static void createFileSource(
        const AString &addr, int32_t port, const char *path) {
    sp<ANetworkSession> session = new ANetworkSession;
    session->start();

    sp<ALooper> looper = new ALooper;
    looper->start();

    sp<RemoteDisplayClient> client = new RemoteDisplayClient;
    sp<WifiDisplaySource> source = new WifiDisplaySource(session, client, path);
    looper->registerHandler(source);

    AString iface = StringPrintf("%s:%d", addr.c_str(), port);
    CHECK_EQ((status_t)OK, source->start(iface.c_str()));

    client->waitUntilDone();

    source->stop();
}

}  // namespace android

int main(int argc, char **argv) {
    using namespace android;

    ProcessState::self()->startThreadPool();

    DataSource::RegisterDefaultSniffers();

    AString listenOnAddr;
    int32_t listenOnPort = -1;

    AString path;

    int res;
    while ((res = getopt(argc, argv, "hl:f:")) >= 0) {
        switch (res) {
            case &#39;f&#39;:
            {
                path = optarg;
                break;
            }

            case &#39;l&#39;:
            {
                const char *colonPos = strrchr(optarg, &#39;:&#39;);

                if (colonPos == NULL) {
                    listenOnAddr = optarg;
                    listenOnPort = WifiDisplaySource::kWifiDisplayDefaultPort;
                } else {
                    listenOnAddr.setTo(optarg, colonPos - optarg);

                    char *end;
                    listenOnPort = strtol(colonPos + 1, &end, 10);

                    if (*end != &#39;\0&#39; || end == colonPos + 1
                            || listenOnPort < 1 || listenOnPort > 65535) {
                        fprintf(stderr, "Illegal port specified.\n");
                        exit(1);
                    }
                }
                break;
            }

            case &#39;?&#39;:
            case &#39;h&#39;:
            default:
                usage(argv[0]);
                exit(1);
        }
    }

    if (listenOnPort >= 0) {
        if (path.empty()) {
            createSource(listenOnAddr, listenOnPort);
        } else {
            createFileSource(listenOnAddr, listenOnPort, path.c_str());
        }

        exit(0);
    }

    usage(argv[0]);

    return 0;
}
</pre><div class="footer">Powered by <a href="https://code.google.com/p/gitiles/">Gitiles</a></div></body></html>