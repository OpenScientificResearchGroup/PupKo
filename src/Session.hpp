/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#include <string>
#include <vector>

#include "PupKo/Types.hpp"

namespace PupKo {

class Session {
public:
    Session();

    void Clear();
    void SetCurrentInput(const std::string& value);
    void AppendKey(const KeyEvent& event);
    bool SelectCandidate(std::size_t index);
    bool CommitCandidate(std::size_t index);
    std::string GetCurrentInput() const;
    const std::vector<Candidate>& GetCandidates() const;
    std::size_t GetSelectedIndex() const;
    void SetCandidates(const std::vector<Candidate>& candidates);

private:
    std::string currentInput_;
    std::vector<Candidate> candidates_;
    std::size_t selectedIndex_;
};

}  // namespace PupKo
