/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#include "Router/RouterTypes.hpp"
#include <string>
#include <string_view>
#include <cstdint>

namespace pupko::provider {

    // 候选词来源标记 (给后续 Ranker 差异化加权打分)
    enum class CandidateSource : uint8_t {
        Schema,       // 静态定义的命令/子命令/Flag (权威性高)
        History,      // 用户曾经执行过的历史命令 (习惯偏好高)
        AgentTool,    // Agent 工具或系统指令
        DynamicHost   // 宿主动态回调提供
    };

    // 原始候选条目
    struct RawCandidate {
        std::string text;                  // 实际补全插入的内容 (例如 "checkout")
        std::string_view displayText;      // 菜单展示的标签 (可与 text 相同)
        std::string_view description;      // 帮助文档/类型提示
        router::TextSpan replaceSpan;      // 告诉宿主从哪里开始替换
        CandidateSource source{ CandidateSource::Schema };
        float baseWeight{ 1.0f };            // 来源基础权重
    };

} // namespace pupko::provider