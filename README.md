<div align="center">

# PupKo 帕可

**PupKo: The sub-millisecond command interaction & agentic core engine.**

**帕可：亚毫秒级现代应用命令交互与智能体核心引擎。**

[![Standard](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![License: MPL 2.0](https://img.shields.io/badge/License-MPL_2.0-yellow.svg)](https://opensource.org/licenses/MPL-2.0)
[![Platform](https://img.shields.io/badge/Platform-Cross--Platform-lightgrey.svg)]()
[![Build Status](https://img.shields.io/badge/Build-Passing-brightgreen.svg)]()

[简体中文](README_zh.md)

</div>

## 🌟 Overview

**PupKo** is a headless, embeddable, ultra-high-performance command interaction and cognitive agent core engine built with **C++17**.

Designed to empower modern software, developer tools, application launchers, and operating systems, PupKo delivers a next-generation interaction hub combining **"sub-millisecond muscle-memory typing"** with **"deep autonomous AI Agent planning"**.

## ⚡ Key Features

* **🏎️ Microsecond-Level Latency**:
  * Zero heap allocations on the hot path. Single-keystroke lookup is consistently completed in **< 0.1 ms**.
* **🎯 Multi-Dimensional Matching Matrix**:
  * Prefix matching: `git co` $\to$ `git checkout`;
  * CamelCase & Acronym matching: `gCA` $\to$ `gitCommitAll`, `gc` $\to$ `git commit`;
  * Contiguous substring matching: `commit` $\to$ `git.commit.amend`;
  * Fuzzy matching & physical keyboard typo tolerance: `nihso` $\to$ `nihao`.
* **🧠 Dynamic Habit Adaptive Frequency Tuning**:
  * Adaptive scoring based on a half-life exponential time-decay formula. High-frequency commands rise to the top automatically, while stale commands decay gracefully, permanently eliminating the "historical deadlock" ranking issue.
* **📐 Interactive Parameter Prompts**:
  * Supports modal argument extraction, keyword branch interception, and transparent command scheduling.
* **🔌 Universal C-ABI**:
  * Ships with pure C export interfaces, natively supporting bindings for **Rust, Swift, C#, Python, Node.js, and WebAssembly**.

## 🏗️ Core Architecture Pipeline

`PupKo` strictly follows a decoupled 4-stage pipeline:

```
[Raw Key Input] 
       │
       ▼
1. Processors (Key Interception)   ──► State Machine Dispatching
       │
       ▼
2. Segmenters (Token Segmentation) ──► Splits command names, flags, and argument slots
       │
       ▼
3. Translators (Multi-Path Recall) ──► Concurrent Candidate Recall
       │
       ▼
4. Filters (Re-ranking & Tuning)   ──► De-duplication, Time-Decay Tuning, Context Weighting
       │
       ▼
[RenderState / Candidate List]
```

## 🚀 Quick Start

### Build Requirements
* A C++17 compliant compiler (GCC 8+, Clang 7+, MSVC 2019+)
* CMake 3.15+

### Build from Source
```bash
git clone https://github.com/OpenScientificResearchGroup/PupKo.git
cd PupKo

cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Run the terminal interactive demo
./build/examples/CommandLineDemo
```

## 💻 C++17 Usage Example

```cpp
#include <PupKo/Engine.hpp>
#include <iostream>

int main() {
    PupKo::PupKoEngine engine;
    auto sessionId = engine.CreateSession();

    // Simulate user typing 'g', 'c'
    PupKo::KeyEvent keyEvent{'g'};
    engine.ProcessKey(sessionId, keyEvent);

    auto state = engine.GetRenderState(sessionId);
    std::cout << "Preedit Buffer: " << state.preeditBuffer << std::endl;

    for (size_t i = 0; i < state.candidates.size(); ++i) {
        std::cout << (i + 1) << ". " << state.candidates[i].displayText 
                  << " [" << state.candidates[i].description << "]\n";
    }

    return 0;
}
```

## 📄 Command Manifest Specification

Declare command metadata and interactive parameter specifications via structured JSON / YAML:

```json
{
    "protocol": "1.0",
    "namespace": "git",
    "commands": [
        {
            "id": "git.commit",
            "name": "git commit",
            "aliases": ["gc", "gcm"],
            "description": "Record changes to the repository",
            "baseWeight": 100.0,
            "parameters": [
                {
                    "name": "message",
                    "flags": ["-m", "--message"],
                    "type": "string",
                    "prompt": "Specify commit message or [Undo(U)]:",
                    "keywords": ["Undo"],
                    "required": true
                }
            ]
        }
    ]
}
```

## 📜 License & Trademark Notice

* **Source Code**: The core source code of this project is licensed under the **[Mozilla Public License 2.0 (MPL-2.0)](LICENSE)**. Proprietary and open-source software can freely embed this library via dynamic or static linking, provided any modifications made to `PupKo`'s own source files are publicly contributed back.
* **Trademark**: "PupKo", "libPupKo", "帕可", and the official logo are trademarks owned by the original authors. Open-source licensing does not grant any trademark rights.

<div align="center">
Made with ❤️ by the PupKo Authors.
</div>
