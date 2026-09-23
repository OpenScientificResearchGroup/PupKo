/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#include "Provider/AgentToolProvider.hpp"
#include "Utils/StringUtils.hpp"

namespace pupko::provider {

	namespace str = pupko::utils;

	void AgentToolProvider::RegisterTool(AgentToolSpec tool) {
		mTools.push_back(std::move(tool));
	}

	void AgentToolProvider::Provide(
		const router::Context& ctx,
		std::vector<RawCandidate>& outCandidates
	) const {
		// 触发条件：前缀以 '/' 开头，或者当前处于命令根部
		const auto prefix = ctx.prefixToMatch;
		if (!str::StartsWith(prefix, '/')) {
			return; // 不是 Agent 指令前缀，直接跳过
		}

		for (const auto& tool : mTools) {
			if (str::StartsWithIgnoreCase(tool.name, prefix)) {
				outCandidates.push_back(RawCandidate{
					tool.name,
					tool.name,
					tool.description,
					ctx.replaceSpan,
					CandidateSource::AgentTool,
					1.5f // Agent 显式指令优先级最高
					});
			}
		}
	}

} // namespace pupko::provider