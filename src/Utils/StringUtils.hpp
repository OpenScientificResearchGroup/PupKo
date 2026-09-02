/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace PupKo::Utils {

inline std::string ToLower(std::string value) {
    for (char& ch : value) {
        if (ch >= 'A' && ch <= 'Z') {
            ch = static_cast<char>(ch + ('a' - 'A'));
        }
    }
    return value;
}

inline std::string ToUpper(std::string value) {
    for (char& ch : value) {
        if (ch >= 'a' && ch <= 'z') {
            ch = static_cast<char>(ch + ('A' - 'a'));
        }
    }
    return value;
}

inline std::string Trim(std::string value) {
    const auto start = value.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    const auto end = value.find_last_not_of(" \t\r\n");
    return value.substr(start, end - start + 1);
}

inline std::vector<std::string> Split(std::string_view text, char delim) {
    std::vector<std::string> result;
    std::size_t start = 0;
    while (start <= text.size()) {
        const auto pos = text.find(delim, start);
        if (pos == std::string_view::npos) {
            result.emplace_back(text.substr(start));
            break;
        }
        result.emplace_back(text.substr(start, pos - start));
        start = pos + 1;
    }
    return result;
}

inline bool StartsWith(std::string_view text, std::string_view prefix) {
    return text.rfind(prefix, 0) == 0;
}

inline bool Contains(std::string_view text, std::string_view needle) {
    return text.find(needle) != std::string_view::npos;
}

inline std::string NormalizeInput(std::string value) {
    return Trim(ToLower(value));
}

}  // namespace PupKo::Utils
