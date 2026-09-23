/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#include "RouterTypes.hpp"
#include "Tokenizer/TokenizerTypes.hpp"

namespace pupko::router {

    class SemanticRouter {
    public:
        SemanticRouter() = delete; // 纯静态工具类，禁止实例化

        // 核心接口：输入根命令树与分词结果，计算出路由上下文
        [[nodiscard]] static router::Context Route(
            const CommandNode& rootCatalog,
            const tokenizer::Result& lexResult
        ) noexcept;
    };

} // namespace pupko::router