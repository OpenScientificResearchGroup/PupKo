/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#include "Processor/IProcessor.hpp"

namespace PupKo::Processor {

class SpellerProcessor final : public IProcessor {
public:
    ProcessResult ProcessKey(Composition& composition, const KeyEvent& event) override;
    std::string_view GetName() const noexcept override;
};

}  // namespace PupKo::Processor