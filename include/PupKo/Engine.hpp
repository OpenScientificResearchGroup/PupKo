/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "PupKo/Types.hpp"

namespace pupko {

class Engine {
public:
    Engine();
    ~Engine();

    void Reset();
    void Clear();
    bool ProcessKey(KeyEvent event);
    std::vector<Candidate> GetCandidates() const;
    std::string GetCurrentInput() const;
    std::size_t GetSelectedIndex() const;
    void SelectCandidate(std::size_t index);
    void CommitCandidate(std::size_t index);

private:
    class Impl;
    std::unique_ptr<Impl> mImpl;
};

}  // namespace PupKo
