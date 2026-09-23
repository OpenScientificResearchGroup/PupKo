/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#include <string_view>
#include <vector>
#include <cstdint>
#include <cstddef>

namespace pupko::tokenizer {

    // 替换区间：通知宿主如何修改文本框
    struct TextSpan {
        std::size_t start{ 0 };   // 宿主文本框替换的起始下标
        std::size_t length{ 0 };  // 替换的字符长度
    };

    // Token 类型
    enum class TokenType : uint8_t {
        Word,          // 普通命令或参数: "git", "checkout"
        Flag,          // 选项标志: "-m", "--branch"
        StringLiteral, // 引号字符串: "commit message", 'path with spaces'
    };

    // 基础 Token
    struct Token {
        std::string_view rawText;    // 原始切片（含引号/反斜杠）
        std::size_t start{ 0 };      // 在整行中的起始偏移
        std::size_t end{ 0 };        // 在整行中的结束偏移 (exclusive)
        TokenType type{ TokenType::Word };
        bool isClosed{ true };       // 针对引号字符串：是否正确闭合
    };

    // 光标焦点的核心上下文（专供 Router 和 Matcher 使用）
    struct ActiveTokenInfo {
        std::size_t tokenIndex{ 0 };       // 焦点 Token 在 tokens 列表中的索引
        std::string_view prefixToMatch;    // 光标左侧用于匹配的前缀文本 (如 "che")
        TextSpan replaceSpan;              // 选中候选词后，宿主应该替换的区间
        bool isNewToken{ false };          // 光标是否在空白处（准备敲入全新的 Token）
        bool isInsideQuotes{ false };      // 光标是否处于未闭合的引号内
    };

    // Tokenizer 最终产物
    struct Result {
        std::vector<Token> tokens;
        ActiveTokenInfo activeToken;
    };

} // namespace pupko::tokenizer