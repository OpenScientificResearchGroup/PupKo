/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#include "Filter/RankerFilter.hpp"

#include <algorithm>

namespace PupKo::Filter {

std::vector<Candidate> RankerFilter::Filter(const std::vector<Candidate>& candidates) const {
    std::vector<Candidate> result = candidates;
    std::stable_sort(result.begin(), result.end(), [](const Candidate& left, const Candidate& right) {
        if (left.dynamicScore != right.dynamicScore) {
            return left.dynamicScore > right.dynamicScore;
        }
        return left.quality > right.quality;
    });
    return result;
}

std::string_view RankerFilter::GetName() const noexcept {
    return "RankerFilter";
}

}  // namespace PupKo::Filter