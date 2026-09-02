/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "Translator/PrefixTranslator.hpp"
#include "Utils/KeyboardUtils.hpp"

TEST(TestMatcher, PrefixMatch) {
    PupKo::Translator::PrefixTranslator translator;
    std::vector<std::string> dictionary = {"pupko", "pup", "ko", "camel"};
    auto matches = translator.Translate("pu", dictionary);
    EXPECT_FALSE(matches.empty());
    EXPECT_EQ(matches[0].text, "pupko");
}

TEST(TestMatcher, KeyboardNeighbor) {
    EXPECT_TRUE(PupKo::Utils::IsKeyboardNeighbor('q', 'w'));
    EXPECT_FALSE(PupKo::Utils::IsKeyboardNeighbor('q', 'm'));
}
