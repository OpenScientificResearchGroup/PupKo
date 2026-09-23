/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace pupko::utils {

    // ============================================================================
    // 1. 字符级基础判断 (Character Utilities)
    // ============================================================================

    // 判断字符是否为空白字符 (空格, 制表符, 换行符, 回车符)
    [[nodiscard]] inline constexpr bool IsWhitespace(char c) noexcept {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r';
    }

    // 快速 ASCII 字符转小写 (避免 std::tolower 在部分平台的 locale 开销)
    [[nodiscard]] inline constexpr char ToLowerAscii(char c) noexcept {
        return (c >= 'A' && c <= 'Z') ? static_cast<char>(c + ('a' - 'A')) : c;
    }

    // 快速 ASCII 字符转大写
    [[nodiscard]] inline constexpr char ToUpperAscii(char c) noexcept {
        return (c >= 'a' && c <= 'z') ? static_cast<char>(c - ('a' - 'A')) : c;
    }

    // ============================================================================
    // 2. 零拷贝修剪 (Zero-Copy Trimming)
    // ============================================================================

    // 去除左侧空白字符 (零堆分配，返回原文本的切片)
    [[nodiscard]] inline constexpr std::string_view TrimLeft(std::string_view text) noexcept {
        while (!text.empty() && IsWhitespace(text.front())) {
            text.remove_prefix(1);
        }
        return text;
    }

    // 去除右侧空白字符 (零堆分配，返回原文本的切片)
    [[nodiscard]] inline constexpr std::string_view TrimRight(std::string_view text) noexcept {
        while (!text.empty() && IsWhitespace(text.back())) {
            text.remove_suffix(1);
        }
        return text;
    }

    // 去除两端空白字符 (零堆分配，返回原文本的切片)
    [[nodiscard]] inline constexpr std::string_view Trim(std::string_view text) noexcept {
        return TrimRight(TrimLeft(text));
    }

    // ============================================================================
    // 3. 大小写转换 (Case Conversions)
    // ============================================================================

    // 原地修改已有字符串为小写 (零堆分配)
    inline void ToLowerInPlace(std::string& str) noexcept {
        for (char& ch : str) {
            ch = ToLowerAscii(ch);
        }
    }

    // 原地修改已有字符串为大写 (零堆分配)
    inline void ToUpperInPlace(std::string& str) noexcept {
        for (char& ch : str) {
            ch = ToUpperAscii(ch);
        }
    }

    // 传入只读切片，构造并返回全新的小写 std::string
    [[nodiscard]] inline std::string ToLower(std::string_view text) {
        std::string result(text);
        ToLowerInPlace(result);
        return result;
    }

    // 传入只读切片，构造并返回全新的大写 std::string
    [[nodiscard]] inline std::string ToUpper(std::string_view text) {
        std::string result(text);
        ToUpperInPlace(result);
        return result;
    }

    // ============================================================================
    // 4. 前缀与包含判断 (Matching & Predicates)
    // ============================================================================

    // 判断 text 是否以字符 c 开头
    [[nodiscard]] inline constexpr bool StartsWith(std::string_view text, char c) noexcept {
        return !text.empty() && text.front() == c;
    }

    // 判断 text 是否以指定前缀 prefix 开头
    [[nodiscard]] inline constexpr bool StartsWith(std::string_view text, std::string_view prefix) noexcept {
        return text.size() >= prefix.size() && text.compare(0, prefix.size(), prefix) == 0;
    }

    // 大小写无关的前缀匹配 (IntelliSense 必备：例如输入 "git" 能匹配 "GIT")
    [[nodiscard]] inline bool StartsWithIgnoreCase(std::string_view text, std::string_view prefix) noexcept {
        if (text.size() < prefix.size()) {
            return false;
        }
        return std::equal(prefix.begin(), prefix.end(), text.begin(),
            [](char a, char b) noexcept {
                return ToLowerAscii(a) == ToLowerAscii(b);
            });
    }

    // 判断 text 是否包含子串 needle
    [[nodiscard]] inline constexpr bool Contains(std::string_view text, std::string_view needle) noexcept {
        return text.find(needle) != std::string_view::npos;
    }

    // 判断 text 是否包含单个字符 c
    [[nodiscard]] inline constexpr bool Contains(std::string_view text, char c) noexcept {
        return text.find(c) != std::string_view::npos;
    }

    // ============================================================================
    // 5. 字符串切分与规范化 (Splitting & Normalization)
    // ============================================================================

    // 零拷贝字符串切分：切片全部是 string_view，指向原 text，不拷贝字符数据
    [[nodiscard]] inline std::vector<std::string_view> Split(std::string_view text, char delim) {
        std::vector<std::string_view> result;
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

    // 规范化输入：去除两端空白并转为小写 (用于哈希查找或命令唯一键归一化)
    [[nodiscard]] inline std::string NormalizeInput(std::string_view text) {
        return ToLower(Trim(text));
    }

} // namespace pupko::utils