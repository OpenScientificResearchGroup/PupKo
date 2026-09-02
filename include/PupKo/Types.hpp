/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace PupKo {

using CandidateId = std::uint64_t;

struct KeyEvent {
    std::uint32_t keyCode = 0;
    bool isPrintable = false;
    bool isCtrl = false;
    bool isAlt = false;
    bool isShift = false;
    std::string_view text;
};

struct Candidate {
    CandidateId id = 0;
    std::string text;
    std::string rawText;
    std::string comment;
    float quality = 0.0f;
    double dynamicScore = 0.0;
    std::size_t start = 0;
    std::size_t end = 0;
    double score = 0.0;
    int frequency = 0;
    std::string source;
};

struct Segment {
    std::size_t start = 0;
    std::size_t end = 0;
    std::string raw;
    std::string typed;
    std::string normalized;
    std::vector<Candidate> candidates;
};

struct Composition {
    std::string rawInput;
    std::size_t cursorPosition = 0;
    std::vector<Segment> segments;
    std::vector<Candidate> candidates;
    std::size_t selectedIndex = 0;
    bool isCommitting = false;

    void Clear() {
        rawInput.clear();
        cursorPosition = 0;
        segments.clear();
        candidates.clear();
        selectedIndex = 0;
        isCommitting = false;
    }
};

struct FilterResult {
    std::vector<Candidate> filtered;
    double totalScore = 0.0;
};

}  // namespace PupKo
