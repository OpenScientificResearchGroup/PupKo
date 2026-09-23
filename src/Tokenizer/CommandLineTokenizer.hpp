/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#include <string_view>
#include <cstdint>

#include "TokenizerTypes.hpp"

namespace pupko::tokenizer {

    class CommandLineTokenizer {
    public:
        CommandLineTokenizer() = default;

        // 纯函数接口：创建并返回结果
        [[nodiscard]] static Result Tokenize(std::string_view line, std::size_t cursor) noexcept;

        // 工业级复用接口：复用已有 vector 缓冲，实现热路径零堆分配 (Zero-Allocation)
        static void Tokenize(std::string_view line, std::size_t cursor, Result& outResult) noexcept;
    };

} // namespace pupko::tokenizer