/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#include "Processor/KeyBinderProcessor.hpp"

namespace PupKo::Processor {

ProcessResult KeyBinderProcessor::ProcessKey(Composition& composition, const KeyEvent& event) {
    if (event.keyCode != 27) {
        return ProcessResult::Noop;
    }
    if (composition.rawInput.empty() && composition.segments.empty() && composition.candidates.empty()) {
        return ProcessResult::Rejected;
    }
    composition.Clear();
    return ProcessResult::Accepted;
}

std::string_view KeyBinderProcessor::GetName() const noexcept {
    return "KeyBinderProcessor";
}

}  // namespace PupKo::Processor