/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#include "HabitTracker.hpp"

#include <cmath>
#include <utility>

namespace PupKo {

HabitTracker::HabitTracker() : halfLifeDays_(7.0) {}

void HabitTracker::RecordUsage(const std::string& token) {
    const auto now = std::chrono::system_clock::now();
    lastUsedTimeMap_[token] = now;
    const auto usageIt = usageCountMap_.find(token);
    if (usageIt == usageCountMap_.end()) {
        usageCountMap_[token] = 1;
    } else {
        usageCountMap_[token] = usageIt->second + 1;
    }
}

double HabitTracker::GetTimeDecay(const std::string& token, std::chrono::system_clock::time_point now) const {
    const auto it = lastUsedTimeMap_.find(token);
    if (it == lastUsedTimeMap_.end()) {
        return 1.0;
    }

    const auto deltaSeconds = std::chrono::duration<double>(now - it->second).count();
    const double elapsedDays = deltaSeconds / 86400.0;
    const double exponent = elapsedDays / halfLifeDays_;
    return std::pow(0.5, exponent);
}

void HabitTracker::SetHalfLifeDays(double halfLifeDays) {
    if (halfLifeDays > 0.0) {
        halfLifeDays_ = halfLifeDays;
    }
}

double HabitTracker::GetHalfLifeDays() const {
    return halfLifeDays_;
}

std::uint64_t HabitTracker::GetUsageCount(const std::string& token) const {
    const auto it = usageCountMap_.find(token);
    if (it == usageCountMap_.end()) {
        return 0;
    }
    return it->second;
}

}  // namespace PupKo
