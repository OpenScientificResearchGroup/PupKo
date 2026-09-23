/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#include <cmath>
#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>

namespace pupko::ranker {

	struct CommandStat {
		uint32_t callCount{ 0 };           // 执行总次数
		uint64_t lastUsedTimestampMs{ 0 }; // 上次使用时间戳 (Epoch 毫秒)
	};

	/// <summary>
	/// HabitTracker 负责内存中维护各项命令的使用频次与时间戳：
	/// </summary>
	class HabitTracker {
	public:
		// 记录一次命令使用
		void RecordUse(std::string_view commandKey, uint64_t timestampMs) noexcept {
			auto& stat = stats_[std::string(commandKey)];
			stat.callCount++;
			stat.lastUsedTimestampMs = timestampMs;
		}

		// 计算某个候选的时间衰减得分
		[[nodiscard]] float CalculateHabitScore(
			std::string_view commandKey,
			uint64_t nowMs,
			uint64_t halfLifeMs = 7 * 24 * 3600 * 1000ULL // 默认 7 天半衰期
		) const noexcept {
			const auto it = stats_.find(std::string(commandKey));
			if (it == stats_.end() || it->second.callCount == 0) {
				return 0.0f;
			}

			const auto& stat = it->second;
			if (nowMs < stat.lastUsedTimestampMs) {
				return 0.0f; // 防时钟回拨保护
			}

			const uint64_t deltaMs = nowMs - stat.lastUsedTimestampMs;
			// 半衰期衰减率: 2^(-delta / halfLife)
			const double decay = std::exp2(-static_cast<double>(deltaMs) / static_cast<double>(halfLifeMs));

			// 频次饱和增长: log(1 + count)
			const double frequencyFactor = std::log1p(static_cast<double>(stat.callCount));

			return static_cast<float>(frequencyFactor * decay * 20.0); // 映射到 0~20 分的加分区间
		}

	private:
		std::unordered_map<std::string, CommandStat> stats_;
	};

} // namespace pupko::ranker