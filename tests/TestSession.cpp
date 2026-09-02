/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#include <gtest/gtest.h>

#include "Session.hpp"

TEST(TestSession, AppendAndClear) {
    PupKo::Session session;
    PupKo::KeyEvent event;
    event.isPrintable = true;
    event.text = "p";
    session.AppendKey(event);
    EXPECT_EQ(session.GetCurrentInput(), "p");
    session.Clear();
    EXPECT_TRUE(session.GetCurrentInput().empty());
}
