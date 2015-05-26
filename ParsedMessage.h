<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd"><html><head><title>media/libstagefright/wifi-display/ParsedMessage.h - platform/frameworks/av - Git at Google</title><link rel="stylesheet" type="text/css" href="//www.google.com/css/go.css" /><link rel="stylesheet" type="text/css" href="/+static/prettify/prettify.vf-M93Ay4IiiWRQSJKPGWQ.cache.css" /><link rel="stylesheet" type="text/css" href="/+static/gitiles.JPyDgEz4Jj5aQRFU8GjJJQ.cache.css" /><script src="/+static/prettify/prettify_compiled.wQKdUVM0AfatEiTiXCI6lw.cache.js" type="text/javascript"></script></head><body onload="prettyPrint()"><h1><img src="//www.google.com/images/logo_sm.gif" alt="Google" />Git</h1><div class="menu"> <a href="https://www.google.com/accounts/ServiceLogin?service=gerritcodereview&continue=https://android.googlesource.com/login/platform/frameworks/av/%2B/android-4.3_r0.9/media/libstagefright/wifi-display/ParsedMessage.h">Sign in</a> </div><div class="breadcrumbs"><a href="/?format=HTML">android</a> / <a href="/platform/frameworks/av/">platform/frameworks/av</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9">android-4.3_r0.9</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/">.</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media">media</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright">libstagefright</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright/wifi-display">wifi-display</a> / ParsedMessage.h</div><div class="sha1">blob: e9a18599c513a5ce117d1a2e9bdc40cb167de530 [<a href="/platform/frameworks/av/+log/android-4.3_r0.9/media/libstagefright/wifi-display/ParsedMessage.h">file history</a>]</div><pre class="git-blob prettyprint linenums lang-h">/*
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

#include <media/stagefright/foundation/ABase.h>
#include <media/stagefright/foundation/AString.h>
#include <utils/KeyedVector.h>
#include <utils/RefBase.h>

namespace android {

// Encapsulates an "HTTP/RTSP style" response, i.e. a status line,
// key/value pairs making up the headers and an optional body/content.
struct ParsedMessage : public RefBase {
    static sp<ParsedMessage> Parse(
            const char *data, size_t size, bool noMoreData, size_t *length);

    bool findString(const char *name, AString *value) const;
    bool findInt32(const char *name, int32_t *value) const;

    const char *getContent() const;

    void getRequestField(size_t index, AString *field) const;
    bool getStatusCode(int32_t *statusCode) const;

    AString debugString() const;

    static bool GetAttribute(const char *s, const char *key, AString *value);

    static bool GetInt32Attribute(
            const char *s, const char *key, int32_t *value);


protected:
    virtual ~ParsedMessage();

private:
    KeyedVector<AString, AString> mDict;
    AString mContent;

    ParsedMessage();

    ssize_t parse(const char *data, size_t size, bool noMoreData);

    DISALLOW_EVIL_CONSTRUCTORS(ParsedMessage);
};

}  // namespace android
</pre><div class="footer">Powered by <a href="https://code.google.com/p/gitiles/">Gitiles</a></div></body></html>