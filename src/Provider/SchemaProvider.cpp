/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#include "Provider/SchemaProvider.hpp"

#include "Provider/ProviderTypes.hpp"
#include "Router/RouterTypes.hpp"

#include "Utils/StringUtils.hpp"

namespace pupko::provider {

	namespace str = pupko::utils;

	void SchemaProvider::Provide(
		const router::Context& ctx,
		std::vector<RawCandidate>& outCandidates
	) const {
		const auto prefix = ctx.prefixToMatch;

		switch (ctx.scope) {
		// 场景 1：补全主命令 (遍历根节点下的所有直属子命令)
		case router::Scope::RootCommand: {
			for (const auto& cmd : mRootCatalog.subcommands) {
				if (str::StartsWithIgnoreCase(cmd.name, prefix)) {
					outCandidates.push_back(RawCandidate{
						std::string(cmd.name),
						cmd.name,
						cmd.description,
						ctx.replaceSpan,
						CandidateSource::Schema,
						1.0f
						});
				}
			}
			break;
		}

		// 场景 2：补全子命令
		case router::Scope::SubCommand: {
			if (!ctx.activeCommand) break;

			// 1. 匹配子命令
			for (const auto& sub : ctx.activeCommand->subcommands) {
				if (str::StartsWithIgnoreCase(sub.name, prefix)) {
					outCandidates.push_back(RawCandidate{
						std::string(sub.name),
						sub.name,
						sub.description,
						ctx.replaceSpan,
						CandidateSource::Schema,
						1.0f
						});
				}
			}

			// 2. 现代 CLI 体验：用户在子命令位置打 '-' 时，同时提示可用 Flags
			if (str::StartsWith(prefix, '-')) {
				for (const auto& flag : ctx.activeCommand->flags) {
					if (str::StartsWithIgnoreCase(flag.flag, prefix)) {
						outCandidates.push_back(RawCandidate{
							std::string(flag.flag),
							flag.flag,
							flag.description,
							ctx.replaceSpan,
							CandidateSource::Schema,
							0.8f // 稍微降权
							});
					}
				}
			}
			break;
		}

		// 场景 3：纯补全 Flag 选项
		case router::Scope::Flag: {
			if (!ctx.activeCommand) break;
			for (const auto& flag : ctx.activeCommand->flags) {
				if (str::StartsWithIgnoreCase(flag.flag, prefix)) {
					outCandidates.push_back(RawCandidate{
						std::string(flag.flag),
						flag.flag,
						flag.description,
						ctx.replaceSpan,
						CandidateSource::Schema,
						1.0f
						});
				}
			}
			break;
		}

		// 场景 4：补全 Flag 对应的枚举参数值
		case router::Scope::FlagArgument: {
			if (!ctx.activeFlag) break;
			// 如果该 Flag 预先声明了固定的 choices，直接提供候选
			for (const auto& choice : ctx.activeFlag->choices) {
				if (str::StartsWithIgnoreCase(choice, prefix)) {
					outCandidates.push_back(RawCandidate{
						std::string(choice),
						choice,
						"Enum Value",
						ctx.replaceSpan,
						CandidateSource::Schema,
						1.2f // 精准参数匹配赋予较高权重
						});
				}
			}
			break;
		}

		default:
			break;
		}
	}

} // namespace pupko::provider