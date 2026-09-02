/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#include <gtest/gtest.h>

#include "HabitTracker.hpp"

TEST(TestHabit, RecordUsageAndDecay) {
    PupKo::HabitTracker tracker;
    tracker.RecordUsage("pupko");
    EXPECT_EQ(tracker.GetUsageCount("pupko"), 1u);
    EXPECT_GT(tracker.GetTimeDecay("pupko", std::chrono::system_clock::now()), 0.0);
}
