/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#include "Translator/PrefixTranslator.hpp"

#include <algorithm>
#include <cctype>

namespace PupKo::Translator {
namespace {

bool StartsWithInsensitive(std::string_view word, std::string_view input) {
    if (input.size() > word.size()) {
        return false;
    }
    return std::equal(input.begin(), input.end(), word.begin(), [](char left, char right) {
        return std::tolower(static_cast<unsigned char>(left)) == std::tolower(static_cast<unsigned char>(right));
    });
}

}  // namespace

std::vector<Candidate> PrefixTranslator::Translate(std::string_view input, const std::vector<std::string>& dictionary) const {
    std::vector<Candidate> result;
    for (std::size_t index = 0; index < dictionary.size(); ++index) {
        const std::string& word = dictionary[index];
        if (!StartsWithInsensitive(word, input)) {
            continue;
        }
        Candidate candidate;
        candidate.id = index + 1;
        candidate.text = word;
        candidate.rawText = word;
        candidate.comment = "Prefix";
        candidate.quality = 0.85f + 0.15f * static_cast<float>(input.size()) / static_cast<float>(std::max<std::size_t>(1, word.size()));
        candidate.start = 0;
        candidate.end = input.size();
        candidate.score = candidate.quality;
        candidate.source = "PrefixTranslator";
        result.push_back(std::move(candidate));
    }
    return result;
}

std::string_view PrefixTranslator::GetName() const noexcept {
    return "PrefixTranslator";
}

}  // namespace PupKo::Translator