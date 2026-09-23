/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#include "IProvider.hpp"

#include <string>
//#include <span> // C++17 可使用 const std::vector<std::string>& 替代

#include "Router/RouterTypes.hpp"

namespace pupko::provider {

    /// <summary>
    /// 从宿主提供的命令历史（Ring Buffer / Vector）中匹配用户敲过的完整命令。支持类似 Fish Shell / Zsh 的整行建议（Ghost Text）。
    /// </summary>
    class HistoryProvider final : public IProvider {
    public:
        // 注入历史记录的只读引用（通常为最近执行的命令环形队列）
        explicit HistoryProvider(const std::vector<std::string>& historyBuffer) noexcept
            : mHistoryBuffer(historyBuffer) {
        }

        void Provide(
            const router::Context& ctx,
            std::vector<RawCandidate>& outCandidates
        ) const override;

    private:
        const std::vector<std::string>& mHistoryBuffer;
    };

} // namespace pupko::provider