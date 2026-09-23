/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#include "Ranker/CandidateRanker.hpp"
#include "Provider/ProviderTypes.hpp"
#include "Utils/StringUtils.hpp"
#include <algorithm>

namespace pupko::ranker {

	namespace str = pupko::utils;

	float CandidateRanker::ScoreCandidate(
		const provider::RawCandidate& cand,
		std::string_view prefix,
		uint64_t nowMs
	) const noexcept {
		float matchScore = 0.0f;

		// 1. 匹配度评定 (Match Quality)
		if (cand.text == prefix) {
			matchScore = 100.0f; // 完全命中
		}
		else if (str::StartsWith(cand.text, prefix)) {
			matchScore = 80.0f;  // 大小写一致前缀
		}
		else if (str::StartsWithIgnoreCase(cand.text, prefix)) {
			matchScore = 60.0f;  // 大小写不敏感前缀
		}
		else {
			matchScore = 30.0f;  // 模糊/子串匹配底分
		}

		// 2. 长度惩罚因子：过长的命令轻微扣分
		if (cand.text.size() > prefix.size()) {
			const float lengthDelta = static_cast<float>(cand.text.size() - prefix.size());
			const float penalty = 1.0f / (1.0f + 0.05f * lengthDelta);
			matchScore *= penalty;
		}

		// 3. 来源加权
		const float baseScore = matchScore * cand.baseWeight;

		// 4. 动态习惯与时间衰减得分
		const float habitScore = mHabitTracker.CalculateHabitScore(cand.text, nowMs);

		return baseScore + habitScore;
	}

	void CandidateRanker::Rank(
		std::vector<provider::RawCandidate>& rawCandidates,
		std::string_view prefix,
		uint64_t nowMs,
		std::size_t topK,
		std::vector<CompletionItem>& outItems
	) const {
		outItems.clear();

		if (rawCandidates.empty() || topK == 0) {
			return;
		}

		// 步骤 1：第一遍并行或快速打分，更新到每个 Candidate 的 baseWeight（借用做临时得分存储）
		// 或者构造打分对 (Score Pairs)
		struct ScoredRef {
			std::size_t index;
			float score;
		};

		std::vector<ScoredRef> scoredIndices;
		scoredIndices.reserve(rawCandidates.size());

		for (std::size_t i = 0; i < rawCandidates.size(); ++i) {
			float finalScore = ScoreCandidate(rawCandidates[i], prefix, nowMs);
			scoredIndices.push_back(ScoredRef{ i, finalScore });
		}

		// 步骤 2：工业级 Top-K 优化 —— 使用 std::partial_sort 替代全量 std::sort
		// 复杂度由 O(N log N) 降为 O(N log K)，当候选项达到数百条时性能提升一个数量级
		const std::size_t actualK = std::min(topK, scoredIndices.size());
		std::partial_sort(
			scoredIndices.begin(),
			scoredIndices.begin() + actualK,
			scoredIndices.end(),
			[](const ScoredRef& a, const ScoredRef& b) noexcept {
				return a.score > b.score; // 降序排序
			}
		);

		// 步骤 3：取出 Top-K 构建最终呈现的 CompletionItem
		outItems.reserve(actualK);
		for (std::size_t i = 0; i < actualK; ++i) {
			const auto& scored = scoredIndices[i];
			const auto& raw = rawCandidates[scored.index];

			CompletionItem item;
			item.text = raw.text;
			item.displayText = std::string(raw.displayText);
			item.documentation = std::string(raw.description);
			item.replaceSpan = raw.replaceSpan;
			item.score = scored.score;

			// 标注类型来源
			switch (raw.source) {
			case provider::CandidateSource::Schema:
				item.detail = "Command";
				break;
			case provider::CandidateSource::History:
				item.detail = "History";
				break;
			case provider::CandidateSource::AgentTool:
				item.detail = "Agent Tool";
				break;
			default:
				item.detail = "Suggestion";
				break;
			}

			outItems.push_back(std::move(item));
		}
	}

} // namespace pupko::ranker