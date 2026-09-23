/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#include "Router/SemanticRouter.hpp"
#include "Utils/StringUtils.hpp"

namespace pupko::router {

	namespace str = pupko::utils;

	// ============================================================================
	// SemanticRouter 核心状态机路由
	// ============================================================================
	Context SemanticRouter::Route(
		const CommandNode& rootCatalog,
		const tokenizer::Result& lexResult
	) noexcept {
		Context ctx;
		ctx.prefixToMatch = lexResult.activeToken.prefixToMatch;
		ctx.replaceSpan = lexResult.activeToken.replaceSpan;
		ctx.isInsideQuotes = lexResult.activeToken.isInsideQuotes;

		const auto& tokens = lexResult.tokens;
		const std::size_t activeIdx = lexResult.activeToken.tokenIndex;

		// 场景 1：整行为空，或光标位于第 0 个 Token 内部 (e.g. "[光标]" 或 "gi[光标]")
		if (tokens.empty() || activeIdx == 0) {
			ctx.scope = Scope::RootCommand;
			ctx.activeCommand = &rootCatalog;
			return ctx;
		}

		// -------------------------------------------------------------------------
		// 阶段 A：沿前序 Token 链进行状态机游走 (Tokens[0 ... activeIdx - 1])
		// -------------------------------------------------------------------------
		const CommandNode* currentNode = &rootCatalog;
		const ParameterSpec* pendingFlagExpectingValue = nullptr;

		for (std::size_t i = 0; i < activeIdx; ++i) {
			const auto& tok = tokens[i];

			// 1. 如果前一个 Flag 正在等待参数值，当前 Token 刚好消费掉它
			if (pendingFlagExpectingValue != nullptr) {
				pendingFlagExpectingValue = nullptr; // 参数值已被吸收
				continue;
			}

			// 2. 如果当前 Token 是一个 Flag (以 '-' 开头)
			if (tok.type == tokenizer::TokenType::Flag || str::StartsWith(tok.rawText, '-')) {
				if (const auto* flag = currentNode->FindFlag(tok.rawText); flag != nullptr) {
					if (flag->takesValue) {
						pendingFlagExpectingValue = flag; // 标记下一个 Token 必须是参数值
					}
				}
				continue;
			}

			// 3. 普通单词：尝试在当前节点向下寻找子命令
			if (const auto* nextSub = currentNode->FindSubcommand(tok.rawText); nextSub != nullptr) {
				currentNode = nextSub;
			}
			else {
				// 如果树断了（输入了未知的参数或位置参数），但不能直接退出，继续允许后续识别 Flags
			}
		}

		ctx.activeCommand = currentNode;

		// -------------------------------------------------------------------------
		// 阶段 B：裁定当前焦点 Token (Active Token) 的语义属性
		// -------------------------------------------------------------------------

		// 裁定 1：前序 Flag 仍期待参数值输入 (e.g. "git checkout -b [光标]")
		if (pendingFlagExpectingValue != nullptr) {
			ctx.scope = Scope::FlagArgument;
			ctx.activeFlag = pendingFlagExpectingValue;
			return ctx;
		}

		// 裁定 2：当前输入的焦点词以 '-' 开头，明确在输入 Flag (e.g. "git commit --[光标]")
		if (str::StartsWith(ctx.prefixToMatch, '-')) {
			ctx.scope = Scope::Flag;
			return ctx;
		}

		// 裁定 3：光标在空白处触发新 Token (e.g. "git checkout [光标]")
		if (lexResult.activeToken.isNewToken) {
			// 如果当前节点还有子命令，优先提示子命令，同时也支持输入 Flag
			if (!currentNode->subcommands.empty()) {
				ctx.scope = Scope::SubCommand;
			}
			else {
				ctx.scope = Scope::PositionalArg;
			}
			return ctx;
		}

		// 裁定 4：正在键入普通词，且当前命令有未匹配完的子命令 (e.g. "git che[光标]")
		if (!currentNode->subcommands.empty()) {
			ctx.scope = Scope::SubCommand;
			return ctx;
		}

		// 裁定 5：叶子节点且没有子命令可匹配，作为位置参数或兜底历史匹配
		ctx.scope = Scope::PositionalArg;
		return ctx;
	}

} // namespace pupko::router