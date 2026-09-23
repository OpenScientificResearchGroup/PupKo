/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#include "IProvider.hpp"
#include "Router/RouterTypes.hpp"

#include <string>
#include <vector>

namespace pupko::provider {

    struct AgentToolSpec {
        std::string name;        // e.g. "/read_file"
        std::string description; // e.g. "Read the contents of a local file into context"
        std::string signature;   // e.g. "path: string"
    };

    class AgentToolProvider final : public IProvider {
    public:
        void RegisterTool(AgentToolSpec tool);

        void Provide(
            const router::Context& ctx,
            std::vector<RawCandidate>& outCandidates
        ) const override;

    private:
        std::vector<AgentToolSpec> mTools;
    };

} // namespace pupko::provider