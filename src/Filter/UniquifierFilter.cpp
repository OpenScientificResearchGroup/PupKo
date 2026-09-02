/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#include "Filter/UniquifierFilter.hpp"

#include <unordered_map>

namespace PupKo::Filter {

std::vector<Candidate> UniquifierFilter::Filter(const std::vector<Candidate>& candidates) const {
    std::vector<Candidate> result;
    std::unordered_map<std::string, std::size_t> positions;
    result.reserve(candidates.size());
    for (const Candidate& candidate : candidates) {
        const auto position = positions.find(candidate.text);
        if (position == positions.end()) {
            positions.emplace(candidate.text, result.size());
            result.push_back(candidate);
            continue;
        }
        Candidate& existing = result[position->second];
        const double candidateScore = candidate.dynamicScore != 0.0 ? candidate.dynamicScore : candidate.score;
        const double existingScore = existing.dynamicScore != 0.0 ? existing.dynamicScore : existing.score;
        if (candidateScore > existingScore || (candidateScore == existingScore && candidate.quality > existing.quality)) {
            existing = candidate;
        }
    }
    return result;
}

std::string_view UniquifierFilter::GetName() const noexcept {
    return "UniquifierFilter";
}

}  // namespace PupKo::Filter