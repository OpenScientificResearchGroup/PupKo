/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#include "Session.hpp"

#include <algorithm>

namespace pupko {

Session::Session() : selectedIndex_(0) {}

void Session::Clear() {
    currentInput_.clear();
    candidates_.clear();
    selectedIndex_ = 0;
}

void Session::SetCurrentInput(const std::string& value) {
    currentInput_ = value;
}

void Session::AppendKey(const KeyEvent& event) {
    if (!event.isPrintable) {
        return;
    }
    currentInput_ += event.text;
}

bool Session::SelectCandidate(std::size_t index) {
    if (index >= candidates_.size()) {
        return false;
    }
    selectedIndex_ = index;
    return true;
}

bool Session::CommitCandidate(std::size_t index) {
    if (index >= candidates_.size()) {
        return false;
    }
    selectedIndex_ = index;
    currentInput_ = candidates_[index].text;
    return true;
}

std::string Session::GetCurrentInput() const {
    return currentInput_;
}

const std::vector<Candidate>& Session::GetCandidates() const {
    return candidates_;
}

std::size_t Session::GetSelectedIndex() const {
    return selectedIndex_;
}

void Session::SetCandidates(const std::vector<Candidate>& candidates) {
    candidates_ = candidates;
    selectedIndex_ = candidates_.empty() ? 0 : std::min(selectedIndex_, candidates_.size() - 1);
}

}  // namespace PupKo
