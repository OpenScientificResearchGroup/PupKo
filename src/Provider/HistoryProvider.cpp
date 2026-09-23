/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#include "Provider/HistoryProvider.hpp"

#include <unordered_set>

#include "Utils/StringUtils.hpp"

namespace pupko::provider {

	namespace str = pupko::utils;

	void HistoryProvider::Provide(
		const router::Context& ctx,
		std::vector<RawCandidate>& outCandidates
	) const {
		if (ctx.prefixToMatch.empty() && ctx.scope != router::Scope::FallbackToHistory) {
			return; // 空前缀时不泛滥推送历史
		}

		std::unordered_set<std::string_view> seen; // 去重防重复
		std::size_t matchedCount = 0;
		constexpr std::size_t kMaxHistorySuggestions = 5;

		// 从后向前遍历（逆序：优先推荐最近执行的命令）
		for (auto it = mHistoryBuffer.rbegin(); it != mHistoryBuffer.rend(); ++it) {
			const std::string_view histLine = *it;

			if (seen.find(histLine) != seen.end()) {
				continue;
			}

			// 匹配规则：历史命令以当前输入的前缀开头
			if (str::StartsWithIgnoreCase(histLine, ctx.prefixToMatch)) {
				seen.insert(histLine);
				outCandidates.push_back(RawCandidate{
					std::string(histLine),
					histLine,
					"History",
					// 历史记录替换：覆盖整个词或整行
					ctx.replaceSpan,
					CandidateSource::History,
					0.9f // 初始权重
					});

				if (++matchedCount >= kMaxHistorySuggestions) {
					break;
				}
			}
		}
	}

} // namespace pupko::provider