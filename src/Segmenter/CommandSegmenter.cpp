/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#include "Segmenter/CommandSegmenter.hpp"

#include "Utils/StringUtils.hpp"

namespace PupKo::Segmenter {

bool CommandSegmenter::Proceed(Composition& composition) {
    if (composition.rawInput.empty()) {
        composition.segments.clear();
        return false;
    }

    composition.segments.clear();
    composition.segments.push_back(ParseSegment(composition.rawInput));
    return true;
}

Segment CommandSegmenter::ParseSegment(std::string_view input) const {
    Segment segment;
    if (input.empty()) {
        return segment;
    }

    segment.start = 0;
    segment.end = input.size();
    segment.raw.assign(input.data(), input.size());
    segment.typed = segment.raw;
    segment.normalized = PupKo::Utils::NormalizeInput(segment.raw);
    return segment;
}

std::string_view CommandSegmenter::GetName() const noexcept {
    return "CommandSegmenter";
}

}  // namespace PupKo::Segmenter