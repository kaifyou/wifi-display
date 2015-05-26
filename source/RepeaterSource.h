<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd"><html><head><title>media/libstagefright/wifi-display/source/RepeaterSource.h - platform/frameworks/av - Git at Google</title><link rel="stylesheet" type="text/css" href="//www.google.com/css/go.css" /><link rel="stylesheet" type="text/css" href="/+static/prettify/prettify.vf-M93Ay4IiiWRQSJKPGWQ.cache.css" /><link rel="stylesheet" type="text/css" href="/+static/gitiles.JPyDgEz4Jj5aQRFU8GjJJQ.cache.css" /><script src="/+static/prettify/prettify_compiled.wQKdUVM0AfatEiTiXCI6lw.cache.js" type="text/javascript"></script></head><body onload="prettyPrint()"><h1><img src="//www.google.com/images/logo_sm.gif" alt="Google" />Git</h1><div class="menu"> <a href="https://www.google.com/accounts/ServiceLogin?service=gerritcodereview&continue=https://android.googlesource.com/login/platform/frameworks/av/%2B/android-4.3_r0.9/media/libstagefright/wifi-display/source/RepeaterSource.h">Sign in</a> </div><div class="breadcrumbs"><a href="/?format=HTML">android</a> / <a href="/platform/frameworks/av/">platform/frameworks/av</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9">android-4.3_r0.9</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/">.</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media">media</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright">libstagefright</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright/wifi-display">wifi-display</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright/wifi-display/source">source</a> / RepeaterSource.h</div><div class="sha1">blob: 8d414fd538e9cac068b1e17d157e605b32fa3796 [<a href="/platform/frameworks/av/+log/android-4.3_r0.9/media/libstagefright/wifi-display/source/RepeaterSource.h">file history</a>]</div><pre class="git-blob prettyprint linenums lang-h">#ifndef REPEATER_SOURCE_H_

#define REPEATER_SOURCE_H_

#include <media/stagefright/foundation/ABase.h>
#include <media/stagefright/foundation/AHandlerReflector.h>
#include <media/stagefright/MediaSource.h>

#define SUSPEND_VIDEO_IF_IDLE   0

namespace android {

// This MediaSource delivers frames at a constant rate by repeating buffers
// if necessary.
struct RepeaterSource : public MediaSource {
    RepeaterSource(const sp<MediaSource> &source, double rateHz);

    virtual status_t start(MetaData *params);
    virtual status_t stop();
    virtual sp<MetaData> getFormat();

    virtual status_t read(
            MediaBuffer **buffer, const ReadOptions *options);

    void onMessageReceived(const sp<AMessage> &msg);

    // If RepeaterSource is currently dormant, because SurfaceFlinger didn&#39;t
    // send updates in a while, this is its wakeup call.
    void wakeUp();

    double getFrameRate() const;
    void setFrameRate(double rateHz);

protected:
    virtual ~RepeaterSource();

private:
    enum {
        kWhatRead,
    };

    Mutex mLock;
    Condition mCondition;

    bool mStarted;

    sp<MediaSource> mSource;
    double mRateHz;

    sp<ALooper> mLooper;
    sp<AHandlerReflector<RepeaterSource> > mReflector;

    MediaBuffer *mBuffer;
    status_t mResult;
    int64_t mLastBufferUpdateUs;

    int64_t mStartTimeUs;
    int32_t mFrameCount;

    void postRead();

    DISALLOW_EVIL_CONSTRUCTORS(RepeaterSource);
};

}  // namespace android

#endif // REPEATER_SOURCE_H_
</pre><div class="footer">Powered by <a href="https://code.google.com/p/gitiles/">Gitiles</a></div></body></html>