/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#include "Tokenizer/TokenizerTypes.hpp"
#include "Router/RoutingTypes.hpp"
#include "Provider/CandidateTypes.hpp"
#include "Ranker/CandidateRanker.hpp"
#include <string_view>
#include <vector>
#include <cstdint>

namespace pupko {

	struct Context {
		Context() {
			// 工业级预留：启动时一次性预分配容量，运行时彻底杜绝扩容
			tokens.reserve(16);
			rawCandidates.reserve(128);
			finalItems.reserve(32);
		}

		// 工业级军规：显式禁止任何隐式拷贝，防止传参疏忽导致性能灾难
		Context(const Context&) = delete;
		Context& operator=(const Context&) = delete;

		// 显式允许移动（移动语义是 noexcept 的，零成本）
		Context(Context&&) noexcept = default;
		Context& operator=(Context&&) noexcept = default;

		// 每次补全触发时调用：快速重置状态，但保留底层分配的内存
		void Reset(std::string_view currentLine, std::size_t currentCursor,
			uint64_t currentTimeMs, std::size_t requestedTopK = 10) noexcept {
			// 1. 初始化只读输入
			line = currentLine;
			cursor = currentCursor;
			nowMs = currentTimeMs;
			topK = requestedTopK;

			// 2. 擦除中间数据 (保留 capacity，0 次系统分配)
			tokens.clear();
			rawCandidates.clear();
			finalItems.clear();

			// 3. 重置光标与路由状态
			activeToken = tokenizer::ActiveTokenInfo{};
			route = router::RoutingContext{};
		}

		// =========================================================================
		// 阶段 0: 只读输入环境 (Inputs)
		// =========================================================================
		std::string_view line;            // 命令行整行
		std::size_t cursor{ 0 };            // 光标位置
		uint64_t nowMs{ 0 };                // 当前时间戳 (用于时间半衰期计算)
		std::size_t topK{ 10 };             // 期望截取的最大候选数量
		// 扩展点：std::string_view cwd;  // 宿主工作目录
		// 扩展点：bool isAgentMode;      // 是否处于智能体对话模式

		// =========================================================================
		// 阶段 1: Tokenizer 产物 (Lexical Stage)
		// =========================================================================
		std::vector<tokenizer::Token> tokens;
		tokenizer::ActiveTokenInfo activeToken;

		// =========================================================================
		// 阶段 2: Router 产物 (Semantic Context Stage)
		// =========================================================================
		router::RoutingContext route;

		// =========================================================================
		// 阶段 3: Provider 产物 (Candidate Generation Stage)
		// =========================================================================
		std::vector<provider::RawCandidate> rawCandidates;

		// =========================================================================
		// 阶段 4: Ranker 产物 (Final Outputs)
		// =========================================================================
		std::vector<ranker::CompletionItem> finalItems;
	};

} // namespace pupko