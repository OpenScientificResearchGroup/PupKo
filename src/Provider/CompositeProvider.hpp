/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#include "IProvider.hpp"
#include <memory>
#include <vector>

namespace pupko::provider {

    class CompositeProvider final : public IProvider {
    public:
        void AddProvider(std::unique_ptr<IProvider> provider) {
            mProviders.push_back(std::move(provider));
        }

        void Provide(
            const router::RoutingContext& ctx,
            std::vector<RawCandidate>& outCandidates
        ) const override {
            for (const auto& p : mProviders) {
                p->Provide(ctx, outCandidates);
            }
        }

    private:
        std::vector<std::unique_ptr<IProvider>> mProviders;
    };

} // namespace pupko::provider