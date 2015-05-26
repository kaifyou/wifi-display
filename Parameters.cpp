<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd"><html><head><title>media/libstagefright/wifi-display/Parameters.cpp - platform/frameworks/av - Git at Google</title><link rel="stylesheet" type="text/css" href="//www.google.com/css/go.css" /><link rel="stylesheet" type="text/css" href="/+static/prettify/prettify.vf-M93Ay4IiiWRQSJKPGWQ.cache.css" /><link rel="stylesheet" type="text/css" href="/+static/gitiles.JPyDgEz4Jj5aQRFU8GjJJQ.cache.css" /><script src="/+static/prettify/prettify_compiled.wQKdUVM0AfatEiTiXCI6lw.cache.js" type="text/javascript"></script></head><body onload="prettyPrint()"><h1><img src="//www.google.com/images/logo_sm.gif" alt="Google" />Git</h1><div class="menu"> <a href="https://www.google.com/accounts/ServiceLogin?service=gerritcodereview&continue=https://android.googlesource.com/login/platform/frameworks/av/%2B/android-4.3_r0.9/media/libstagefright/wifi-display/Parameters.cpp">Sign in</a> </div><div class="breadcrumbs"><a href="/?format=HTML">android</a> / <a href="/platform/frameworks/av/">platform/frameworks/av</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9">android-4.3_r0.9</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/">.</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media">media</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright">libstagefright</a> / <a href="/platform/frameworks/av/+/android-4.3_r0.9/media/libstagefright/wifi-display">wifi-display</a> / Parameters.cpp</div><div class="sha1">blob: d2a61ea4642eed150c0cdc8a9a0f9510ba3e276d [<a href="/platform/frameworks/av/+log/android-4.3_r0.9/media/libstagefright/wifi-display/Parameters.cpp">file history</a>]</div><pre class="git-blob prettyprint linenums lang-cpp">/*
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

#include "Parameters.h"

#include <media/stagefright/MediaErrors.h>

namespace android {

// static
sp<Parameters> Parameters::Parse(const char *data, size_t size) {
    sp<Parameters> params = new Parameters;
    status_t err = params->parse(data, size);

    if (err != OK) {
        return NULL;
    }

    return params;
}

Parameters::Parameters() {}

Parameters::~Parameters() {}

status_t Parameters::parse(const char *data, size_t size) {
    size_t i = 0;
    while (i < size) {
        size_t nameStart = i;
        while (i < size && data[i] != &#39;:&#39;) {
            ++i;
        }

        if (i == size || i == nameStart) {
            return ERROR_MALFORMED;
        }

        AString name(&data[nameStart], i - nameStart);
        name.trim();
        name.tolower();

        ++i;

        size_t valueStart = i;

        while (i + 1 < size && (data[i] != &#39;\r&#39; || data[i + 1] != &#39;\n&#39;)) {
            ++i;
        }

        AString value(&data[valueStart], i - valueStart);
        value.trim();

        mDict.add(name, value);

        while (i + 1 < size && data[i] == &#39;\r&#39; && data[i + 1] == &#39;\n&#39;) {
            i += 2;
        }
    }

    return OK;
}

bool Parameters::findParameter(const char *name, AString *value) const {
    AString key = name;
    key.tolower();

    ssize_t index = mDict.indexOfKey(key);

    if (index < 0) {
        value->clear();

        return false;
    }

    *value = mDict.valueAt(index);
    return true;
}

}  // namespace android
</pre><div class="footer">Powered by <a href="https://code.google.com/p/gitiles/">Gitiles</a></div></body></html>