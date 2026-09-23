/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#include "HabitTracker.hpp"
#include "Provider/ProviderTypes.hpp"
#include "Router/RouterTypes.hpp"
#include "Ranker/RankerTypes.hpp"
#include <vector>
#include <string>

namespace pupko::ranker {

	class CandidateRanker {
	public:
		explicit CandidateRanker(const HabitTracker& habitTracker) noexcept
			: mHabitTracker(habitTracker) {
		}

		// 核心接口：打分、Top-K 排序并输出最终条目
		void Rank(
			std::vector<provider::RawCandidate>& rawCandidates,
			std::string_view prefix,
			uint64_t nowMs,
			std::size_t topK,
			std::vector<CompletionItem>& outItems
		) const;

	private:
		[[nodiscard]] float ScoreCandidate(
			const provider::RawCandidate& cand,
			std::string_view prefix,
			uint64_t nowMs
		) const noexcept;

		const HabitTracker& mHabitTracker;
	};

} // namespace pupko::ranker