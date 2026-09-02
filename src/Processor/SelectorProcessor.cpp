/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#include "Processor/SelectorProcessor.hpp"

namespace PupKo::Processor {

ProcessResult SelectorProcessor::ProcessKey(Composition& composition, const KeyEvent& event) {
    if (composition.segments.empty() || composition.segments.front().candidates.empty()) {
        return ProcessResult::Noop;
    }
    const std::vector<Candidate>& candidates = composition.segments.front().candidates;

    if (event.keyCode >= '1' && event.keyCode <= '9') {
        const std::size_t index = static_cast<std::size_t>(event.keyCode - '1');
        if (index >= candidates.size()) {
            return ProcessResult::Rejected;
        }
        composition.selectedIndex = index;
        return ProcessResult::Accepted;
    }

    if (event.keyCode == 32) {
        if (composition.rawInput.empty()) {
            return ProcessResult::Noop;
        }
        composition.selectedIndex = 0;
        composition.isCommitting = true;
        return ProcessResult::Accepted;
    }

    if (event.keyCode == 0x21 || event.keyCode == 0x26) {
        composition.selectedIndex = ClampIndex(composition, composition.selectedIndex + 1);
        return ProcessResult::Accepted;
    }

    if (event.keyCode == 0x22 || event.keyCode == 0x28) {
        const std::size_t count = candidates.size();
        composition.selectedIndex = ClampIndex(composition, composition.selectedIndex + count - 1);
        return ProcessResult::Accepted;
    }

    return ProcessResult::Noop;
}

std::string_view SelectorProcessor::GetName() const noexcept {
    return "SelectorProcessor";
}

std::size_t SelectorProcessor::ClampIndex(const Composition& composition, std::size_t index) const {
    if (composition.candidates.empty()) {
        return 0;
    }
    return index % composition.candidates.size();
}

}  // namespace PupKo::Processor