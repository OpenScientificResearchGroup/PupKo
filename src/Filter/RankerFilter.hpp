/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#include "Filter/IFilter.hpp"

namespace PupKo::Filter {

class RankerFilter final : public IFilter {
public:
    std::vector<Candidate> Filter(const std::vector<Candidate>& candidates) const override;
    std::string_view GetName() const noexcept override;
};

}  // namespace PupKo::Filter