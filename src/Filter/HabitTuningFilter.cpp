/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#include "Filter/HabitTuningFilter.hpp"

#include <cmath>

namespace PupKo::Filter {

HabitTuningFilter::HabitTuningFilter(double timeDecay) : mTimeDecay(timeDecay > 0.0 ? timeDecay : 0.5) {}

std::vector<Candidate> HabitTuningFilter::Filter(const std::vector<Candidate>& candidates) const {
    std::vector<Candidate> result = candidates;
    // The seven-day half-life attenuates remembered preference as elapsed time grows.
    const double attenuation = std::pow(mTimeDecay, 1.0 / 7.0);
    for (Candidate& candidate : result) {
        const double baseScore = candidate.dynamicScore != 0.0 ? candidate.dynamicScore : candidate.score;
        candidate.dynamicScore = baseScore * attenuation + static_cast<double>(candidate.frequency) * 0.1;
    }
    return result;
}

std::string_view HabitTuningFilter::GetName() const noexcept {
    return "HabitTuningFilter";
}

}  // namespace PupKo::Filter