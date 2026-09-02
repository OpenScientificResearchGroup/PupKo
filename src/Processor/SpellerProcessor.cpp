/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#include "Processor/SpellerProcessor.hpp"

namespace PupKo::Processor {

ProcessResult SpellerProcessor::ProcessKey(Composition& composition, const KeyEvent& event) {
    if (event.keyCode == 8 || event.keyCode == 127) {
        if (composition.rawInput.empty()) {
            return ProcessResult::Noop;
        }
        composition.rawInput.pop_back();
        composition.cursorPosition = composition.rawInput.size();
        return ProcessResult::Accepted;
    }

    const bool isAsciiPrintable = event.keyCode >= 32 && event.keyCode <= 126;
    if (!isAsciiPrintable) {
        return ProcessResult::Noop;
    }

    if (!event.text.empty()) {
        composition.rawInput.append(event.text.data(), event.text.size());
    } else {
        composition.rawInput.push_back(static_cast<char>(event.keyCode));
    }
    composition.cursorPosition = composition.rawInput.size();
    return ProcessResult::Accepted;
}

std::string_view SpellerProcessor::GetName() const noexcept {
    return "SpellerProcessor";
}

}  // namespace PupKo::Processor