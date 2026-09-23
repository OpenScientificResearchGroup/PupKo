/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#include "IProvider.hpp"

namespace pupko::provider {

    /// <summary>
    /// 根据 RoutingContext::scope，从已注册的 CommandNode 树中做前缀检索（Prefix Matching）。
    /// </summary>
    class SchemaProvider final : public IProvider {
    public:
        explicit SchemaProvider(const router::CommandNode& rootCatalog) noexcept
            : mRootCatalog(rootCatalog) {
        }

        void Provide(
            const router::Context& ctx,
            std::vector<RawCandidate>& outCandidates
        ) const override;

    private:
        const router::CommandNode& mRootCatalog;
    };

} // namespace pupko::provider