/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <cstddef>
#include <string_view>

namespace PupKo::Utils {

inline bool IsKeyboardNeighbor(char lhs, char rhs) {
    if (lhs == 0 || rhs == 0) {
        return false;
    }

    const std::array<char, 26> letters = {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
                                         'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
                                         'z', 'x', 'c', 'v', 'b', 'n', 'm'};

    const char left = static_cast<char>(std::tolower(static_cast<unsigned char>(lhs)));
    const char right = static_cast<char>(std::tolower(static_cast<unsigned char>(rhs)));
    const auto lhsIt = std::find(letters.begin(), letters.end(), left);
    const auto rhsIt = std::find(letters.begin(), letters.end(), right);
    if (lhsIt == letters.end() || rhsIt == letters.end()) {
        return false;
    }

    const auto lhsIndex = static_cast<std::ptrdiff_t>(std::distance(letters.begin(), lhsIt));
    const auto rhsIndex = static_cast<std::ptrdiff_t>(std::distance(letters.begin(), rhsIt));
    return std::abs(lhsIndex - rhsIndex) <= 1;
}

inline int ComputeFuzzyDistance(std::string_view lhs, std::string_view rhs) {
    if (lhs == rhs) {
        return 0;
    }
    const std::size_t commonLength = std::min(lhs.size(), rhs.size());
    int distance = 0;
    for (std::size_t index = 0; index < commonLength; ++index) {
        if (lhs[index] != rhs[index]) {
            ++distance;
        }
    }
    distance += static_cast<int>(std::max(lhs.size(), rhs.size()) - commonLength);
    return distance;
}

}  // namespace PupKo::Utils
