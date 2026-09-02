<div align="center">

# PupKo 帕可

**PupKo: The sub-millisecond command interaction & agentic core engine.**

**帕可：亚毫秒级现代应用命令交互与智能体核心引擎。**

[![Standard](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![License: MPL 2.0](https://img.shields.io/badge/License-MPL_2.0-yellow.svg)](https://opensource.org/licenses/MPL-2.0)
[![Platform](https://img.shields.io/badge/Platform-Cross--Platform-lightgrey.svg)]()
[![Build Status](https://img.shields.io/badge/Build-Passing-brightgreen.svg)]()

[English](README.md)

</div>

## 🌟 概述

**帕可** 是一个基于 **C++17** 编写的无头、可嵌入、超高性能的现代应用命令交互与智能体核心引擎。

项目旨在为现代软件、开发工具、启动器和操作系统提供兼具**“极致肌肉记忆打字响应”**与**“深度 AI Agent 自主规划”**的下一代交互中枢。

## ⚡ 核心特性

* **🏎️ 微秒级极致响应**：
  * 热路径严格零堆内存分配，单次按键检索耗时稳定在 **< 0.1 毫秒**。
* **🎯 多维全能检索矩阵**：
  * 前缀匹配：`git co` $\to$ `git checkout`；
  * 驼峰与首字母：`gCA` $\to$ `gitCommitAll`，`gc` $\to$ `git commit`；
  * 连续子串匹配：`commit` $\to$ `git.commit.amend`；
  * 模糊匹配与物理按键容错：`nihso` $\to$ `nihao`。
* **🧠 动态习惯自适应调频**：
  * 基于物理半衰期的时间衰减调频公式，高频命令自动置顶，久未使用命令平滑降频，彻底解决“历史死锁霸榜”痛点。
* **📐 交互式参数提示**：
  * 支持模态命令提取、关键字分支拦截与透明命令调度。
* **🔌 通用 C-ABI**：
  * 提供纯 C 导出接口，天然支持 **Rust、Swift、C#、Python、Node.js 及 WebAssembly** 绑定。

## 🏗️ 核心架构流水线

`帕可` 严格遵循以下解耦四部曲流水线：

```
[原始按键输入] 
       │
       ▼
1. Processors (按键拦截)  ──► 状态机分发
       │
       ▼
2. Segmenters (分词切分)  ──► 切分命令名、选项标志与参数槽位
       │
       ▼
3. Translators (多路召回) ──► 并发召回
       │
       ▼
4. Filters (重排与调频)   ──► 跨路去重、时间衰减调频、环境上下文加权
       │
       ▼
[RenderState / 最终候选列表]
```

## 🚀 快速开始

### 编译依赖
* 支持 C++17 的编译器（GCC 8+, Clang 7+, MSVC 2019+）
* CMake 3.15+

### 源码编译
```bash
git clone https://github.com/OpenScientificResearchGroup/PupKo.git
cd PupKo

cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# 运行终端交互式 Demo
./build/examples/CommandLineDemo
```

## 💻 C++17 调用示例

```cpp
#include <PupKo/Engine.hpp>
#include <iostream>

int main() {
    PupKo::PupKoEngine engine;
    auto sessionId = engine.CreateSession();

    // 模拟用户键入 'g', 'c'
    PupKo::KeyEvent keyEvent{'g'};
    engine.ProcessKey(sessionId, keyEvent);

    auto state = engine.GetRenderState(sessionId);
    std::cout << "预编辑区: " << state.preeditBuffer << std::endl;

    for (size_t i = 0; i < state.candidates.size(); ++i) {
        std::cout << (i + 1) << ". " << state.candidates[i].displayText 
                  << " [" << state.candidates[i].description << "]\n";
    }

    return 0;
}
```

## 📄 命令配置清单规范

通过声明式 JSON/YAML 定义命令元数据与参数交互规范：

```json
{
    "protocol": "1.0",
    "namespace": "git",
    "commands": [
        {
            "id": "git.commit",
            "name": "git commit",
            "aliases": ["gc", "gcm"],
            "description": "记录暂存区的更改到仓库",
            "baseWeight": 100.0,
            "parameters": [
                {
                    "name": "message",
                    "flags": ["-m", "--message"],
                    "type": "string",
                    "prompt": "指定提交信息或[放弃(U)]:",
                    "keywords": ["Undo"],
                    "required": true
                }
            ]
        }
    ]
}
```

## 📜 开源协议与商标声明

* **源码授权**：本项目核心源码采用 **[Mozilla Public License 2.0 (MPL-2.0)](LICENSE)** 开源。商业软件可自由嵌入本库动态链接或静态链接，但对 `帕可` 本身源文件的修改必须公开发布。
* **商标声明**：“PupKo”、“libPupKo”、“帕可”及官方标识为项目原作者所有。开源授权不包含任何商标权。

<div align="center">
由 帕可 团队倾情打造 ❤️
</div>
