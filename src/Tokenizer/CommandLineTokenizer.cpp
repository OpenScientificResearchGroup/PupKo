/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#include "CommandLineTokenizer.hpp"

#include <algorithm>
#include <string>

#include "TokenizerTypes.hpp"
#include "Utils/StringUtils.hpp"

namespace pupko::tokenizer {

	namespace str = pupko::utils;

	Result CommandLineTokenizer::Tokenize(std::string_view line, std::size_t cursor) noexcept {
		Result result;
		Tokenize(line, cursor, result);
		return result;
	}

	void CommandLineTokenizer::Tokenize(std::string_view line, std::size_t cursor, Result& outResult) noexcept {
		outResult.tokens.clear();

		// 边界保护：限制 cursor 最大不超过 line.size()
		cursor = std::min(cursor, line.size());

		enum class State : uint8_t {
			ScanningWhitespace,
			InWord,
			InDoubleQuote,
			InSingleQuote
		};

		auto state = State::ScanningWhitespace;
		std::size_t tokenStart = 0;
		bool isEscaped = false;

		// -------------------------------------------------------------------------
		// 阶段 1：单趟有限状态机扫描（Single-Pass FSM）
		// -------------------------------------------------------------------------
		for (std::size_t i = 0; i < line.size(); ++i) {
			const char c = line[i];

			if (isEscaped) {
				isEscaped = false;
				continue;
			}

			// 在单引号内部，反斜杠通常不作为转义符处理
			if (c == '\\' && state != State::InSingleQuote) {
				isEscaped = true;
				continue;
			}

			switch (state) {
			case State::ScanningWhitespace: {
				if (!str::IsWhitespace(c)) {
					tokenStart = i;
					if (c == '"') {
						state = State::InDoubleQuote;
					}
					else if (c == '\'') {
						state = State::InSingleQuote;
					}
					else {
						state = State::InWord;
					}
				}
				break;
			}

			case State::InWord: {
				if (str::IsWhitespace(c)) {
					const std::string_view text = line.substr(tokenStart, i - tokenStart);
					const auto type = str::StartsWith(text, '-') ? TokenType::Flag : TokenType::Word;
					outResult.tokens.push_back(Token{ text, tokenStart, i, type, true });
					state = State::ScanningWhitespace;
				}
				break;
			}

			case State::InDoubleQuote: {
				if (c == '"') {
					const std::string_view text = line.substr(tokenStart, i + 1 - tokenStart);
					outResult.tokens.push_back(Token{ text, tokenStart, i + 1, TokenType::StringLiteral, true });
					state = State::ScanningWhitespace;
				}
				break;
			}

			case State::InSingleQuote: {
				if (c == '\'') {
					const std::string_view text = line.substr(tokenStart, i + 1 - tokenStart);
					outResult.tokens.push_back(Token{ text, tokenStart, i + 1, TokenType::StringLiteral, true });
					state = State::ScanningWhitespace;
				}
				break;
			}
			}
		}

		// 处理行尾悬挂/未闭合的最后一个 Token
		if (state != State::ScanningWhitespace) {
			const std::size_t end = line.size();
			const std::string_view text = line.substr(tokenStart, end - tokenStart);
			const bool isClosed = (state == State::InWord);
			const auto type = (state == State::InWord)
				? (str::StartsWith(text, '-') ? TokenType::Flag : TokenType::Word)
				: TokenType::StringLiteral;

			outResult.tokens.push_back(Token{ text, tokenStart, end, type, isClosed });
		}

		// -------------------------------------------------------------------------
		// 阶段 2：光标焦点解析（Cursor-Aware Resolution）
		// -------------------------------------------------------------------------
		ActiveTokenInfo& active = outResult.activeToken;
		active.isNewToken = false;
		active.isInsideQuotes = (state == State::InDoubleQuote || state == State::InSingleQuote);

		// 场景 A：整行为空或仅有空白字符
		if (outResult.tokens.empty()) {
			active.tokenIndex = 0;
			active.prefixToMatch = std::string_view{};
			active.replaceSpan = TextSpan{ cursor, 0 };
			active.isNewToken = true;
			return;
		}

		// 场景 B：判断光标落在哪个已有的 Token 范围内 [start, end]
		for (std::size_t i = 0; i < outResult.tokens.size(); ++i) {
			const auto& t = outResult.tokens[i];

			// 光标落在当前 Token 内部或紧贴其末尾
			if (cursor >= t.start && cursor <= t.end) {
				active.tokenIndex = i;

				// C++17：带初始化的 if 语句计算匹配前缀
				if (const std::size_t prefixLen = cursor - t.start; prefixLen > 0) {
					const std::string_view rawPrefix = line.substr(t.start, prefixLen);

					// 如果是引号字面量，去除前导引号后用于前缀匹配
					if (t.type == TokenType::StringLiteral && (str::StartsWith(rawPrefix, '"') || str::StartsWith(rawPrefix, '\''))) {
						active.prefixToMatch = rawPrefix.substr(1);
					}
					else {
						active.prefixToMatch = rawPrefix;
					}
				}
				else {
					active.prefixToMatch = std::string_view{};
				}

				// 工业级标准：替换区间应覆盖整个词（整词替换，避免中间编辑时残留后半截）
				active.replaceSpan = TextSpan{ t.start, t.end - t.start };
				return;
			}
		}

		// 场景 C：光标落在 Token 之间的空白处或整行末尾 (例如: "git checkout  [cursor]")
		active.tokenIndex = outResult.tokens.size();
		active.prefixToMatch = std::string_view{};
		active.replaceSpan = TextSpan{ cursor, 0 }; // 长度为 0 表示就地插入
		active.isNewToken = true;
	}
} // namespace pupko::tokenizer