/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#include "PupKo/PupKoCApi.h"

#include <cstring>
#include <string>
#include <vector>

#include "PupKo/Engine.hpp"

namespace {

std::string ToUtf8String(const char* text, int textLength) {
    if (text == nullptr || textLength <= 0) {
        return {};
    }
    return std::string(text, static_cast<std::size_t>(textLength));
}

}  // namespace

extern "C" {

void PupKoCreateEngine(void** engineHandle) {
    if (engineHandle == nullptr) {
        return;
    }
    *engineHandle = reinterpret_cast<void*>(new PupKo::Engine());
}

void PupKoDestroyEngine(void* engineHandle) {
    if (engineHandle == nullptr) {
        return;
    }
    delete reinterpret_cast<PupKo::Engine*>(engineHandle);
}

int PupKoProcessKey(void* engineHandle, unsigned int keyCode, const char* text, int textLength) {
    if (engineHandle == nullptr) {
        return 0;
    }
    auto* engine = reinterpret_cast<PupKo::Engine*>(engineHandle);
    PupKo::KeyEvent event;
    event.keyCode = keyCode;
    event.isPrintable = text != nullptr && textLength > 0;
    event.text = ToUtf8String(text, textLength);
    return engine->ProcessKey(event) ? 1 : 0;
}

int PupKoGetCandidateCount(void* engineHandle) {
    if (engineHandle == nullptr) {
        return 0;
    }
    auto* engine = reinterpret_cast<PupKo::Engine*>(engineHandle);
    return static_cast<int>(engine->GetCandidates().size());
}

const char* PupKoGetCandidateText(void* engineHandle, int index) {
    static std::vector<std::string> storage;
    storage.clear();
    if (engineHandle == nullptr || index < 0) {
        return nullptr;
    }
    auto* engine = reinterpret_cast<PupKo::Engine*>(engineHandle);
    const auto candidates = engine->GetCandidates();
    if (static_cast<std::size_t>(index) >= candidates.size()) {
        return nullptr;
    }
    storage.emplace_back(candidates[static_cast<std::size_t>(index)].text);
    return storage.back().c_str();
}

void PupKoClearInput(void* engineHandle) {
    if (engineHandle == nullptr) {
        return;
    }
    auto* engine = reinterpret_cast<PupKo::Engine*>(engineHandle);
    engine->Clear();
}

}  // extern "C"
