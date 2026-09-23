/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#include <string>
#include "Router/RouterTypes.hpp"

namespace pupko::ranker {

	// 最终暴露给宿主的补全条目
	struct CompletionItem {
		std::string text;             // 待插入文本 (e.g. "checkout")
		std::string displayText;      // 菜单展示文本 (e.g. "checkout")
		std::string detail;           // 类型详情 (e.g. "SubCommand", "History")
		std::string documentation;    // 帮助文档说明
		router::TextSpan replaceSpan; // 替换区间
		float score{ 0.0f };            // 最终综合打分
	};
}