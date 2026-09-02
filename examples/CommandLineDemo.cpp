/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#include <iostream>
#include <string>
#include <vector>

#include "PupKo/Engine.hpp"

int main() {
    PupKo::Engine engine;
    std::cout << "PupKo CLI Demo\n";
    std::cout << "Type characters and press Enter to see candidates.\n";

    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        if (input == "exit") {
            break;
        }

        for (char ch : input) {
            PupKo::KeyEvent event;
            event.keyCode = static_cast<unsigned int>(ch);
            event.isPrintable = true;
            event.text = std::string(1, ch);
            engine.ProcessKey(event);
        }

        const auto candidates = engine.GetCandidates();
        std::cout << "Current input: " << engine.GetCurrentInput() << "\n";
        for (std::size_t index = 0; index < candidates.size(); ++index) {
            std::cout << index + 1 << ". " << candidates[index].text << "\n";
        }
    }

    return 0;
}
