/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#include "ITranslator.hpp"

namespace PupKo::Translator {

class FuzzyTypoTranslator final : public ITranslator {
public:
    std::vector<Candidate> Translate(std::string_view input, const std::vector<std::string>& dictionary) const override;
    std::string_view GetName() const noexcept override;
};

}  // namespace PupKo::Translator