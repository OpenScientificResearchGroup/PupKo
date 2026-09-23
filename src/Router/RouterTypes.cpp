/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#include "RouterTypes.hpp"

#include <string_view>

#include "Utils/StringUtils.hpp"

namespace pupko::router {

    namespace str = pupko::utils;

    const CommandNode* CommandNode::FindSubcommand(std::string_view subName) const noexcept {
        for (const auto& sub : subcommands) {
            if (str::StartsWithIgnoreCase(sub.name, subName) && sub.name.size() == subName.size()) {
                return &sub;
            }
        }
        return nullptr;
    }

    const ParameterSpec* CommandNode::FindFlag(std::string_view flagName) const noexcept {
        for (const auto& f : flags) {
            if (f.flag == flagName) {
                return &f;
            }
        }
        return nullptr;
    }

}