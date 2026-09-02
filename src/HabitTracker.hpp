/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#include <chrono>
#include <cstdint>
#include <string>
#include <unordered_map>

namespace PupKo {

class HabitTracker {
public:
    HabitTracker();
    void RecordUsage(const std::string& token);
    double GetTimeDecay(const std::string& token, std::chrono::system_clock::time_point now) const;
    void SetHalfLifeDays(double halfLifeDays);
    double GetHalfLifeDays() const;
    std::uint64_t GetUsageCount(const std::string& token) const;

private:
    std::unordered_map<std::string, std::chrono::system_clock::time_point> lastUsedTimeMap_;
    std::unordered_map<std::string, std::uint64_t> usageCountMap_;
    double halfLifeDays_;
};

}  // namespace PupKo
