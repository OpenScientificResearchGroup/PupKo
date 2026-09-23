/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#include "ProviderTypes.hpp"
#include "Router/RouterTypes.hpp"
#include <vector>

namespace pupko::provider {

    class IProvider {
    public:
        virtual ~IProvider() = default;

        // 核心契约：根据路由结果，向 outCandidates 追加原始候选 (缓冲区复用模式)
        virtual void Provide(
            const router::Context& ctx,
            std::vector<RawCandidate>& outCandidates
        ) const = 0;
    };

} // namespace pupko::provider