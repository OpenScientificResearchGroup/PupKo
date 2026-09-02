/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "PupKo/Types.hpp"

namespace PupKo::Translator {

class ITranslator {
public:
    virtual ~ITranslator() = default;
    virtual std::vector<Candidate> Translate(std::string_view input, const std::vector<std::string>& dictionary) const = 0;
    virtual std::string_view GetName() const noexcept = 0;
};

}  // namespace PupKo::Translator