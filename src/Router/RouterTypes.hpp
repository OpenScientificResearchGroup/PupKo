/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#include <string_view>
#include <vector>
#include <cstdint>

#include "Tokenizer/TokenizerTypes.hpp"

namespace pupko::router {

	// 1. 语义补全范畴 (当前用户到底在输入什么)
	enum class Scope : uint8_t {
		RootCommand,     // 正在输入根命令 (例如: "gi[光标]")
		SubCommand,      // 正在输入子命令 (例如: "git che[光标]")
		Flag,            // 正在输入参数标志 (例如: "git commit --[光标]")
		FlagArgument,    // 正在输入某个 Flag 期待的值 (例如: "git checkout -b [光标]")
		PositionalArg,   // 正在输入位置参数/普通参数 (例如: "cat file[光标]")
		FallbackToHistory// 语法树断裂/未识别命令，回退到纯历史记录匹配
	};

	// 2. 参数定义元数据
	struct ParameterSpec {
		std::string_view name;            // 参数名，如 "branch"
		std::string_view flag;            // 标志缩写或全称，如 "-b" 或 "--branch"
		std::string_view description;     // 提示文档
		bool takesValue{ false };           // 该 Flag 是否需要吃掉下一个参数作为值
		std::vector<std::string_view> choices; // 可选的枚举项（例如 ["json", "yaml", "text"]）
	};

	// 3. 命令定义节点（树状递归结构）
	struct CommandNode {
		std::string_view name;            // 命令名，如 "git", "checkout"
		std::string_view description;     // 帮助说明
		std::vector<CommandNode> subcommands; // 子命令列表
		std::vector<ParameterSpec> flags;     // 该命令支持的所有选项标志

		// 快速查找辅助（零分配）
		[[nodiscard]] const CommandNode* FindSubcommand(std::string_view subName) const noexcept;
		[[nodiscard]] const ParameterSpec* FindFlag(std::string_view flagName) const noexcept;
	};

	// 4. 路由推导结果：交给下游 Candidate Provider 的核心凭证
	struct Context {
		Scope scope{ Scope::RootCommand };

		// 语法树上下文
		const CommandNode* activeCommand{ nullptr }; // 当前命中的最深层命令 (例如 "checkout" 节点)
		const ParameterSpec* activeFlag{ nullptr };   // 若 scope == FlagArgument，指向对应的 Flag 定义

		// 继承自分词器的补全坐标
		std::string_view prefixToMatch;           // 用于前缀匹配的词 (例如 "che")
		tokenizer::TextSpan replaceSpan;                     // 宿主需要替换的文本区间
		bool isInsideQuotes{ false };             // 是否处于未闭合的引号内部
	};

} // namespace pupko::router