/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#include "Translator/FuzzyTypoTranslator.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>

namespace PupKo::Translator {
namespace {

bool IsNeighbor(char left, char right) {
    constexpr std::array<std::string_view, 3> rows = {"qwertyuiop", "asdfghjkl", "zxcvbnm"};
    const char lhs = static_cast<char>(std::tolower(static_cast<unsigned char>(left)));
    const char rhs = static_cast<char>(std::tolower(static_cast<unsigned char>(right)));
    for (std::string_view row : rows) {
        const auto leftPosition = row.find(lhs);
        const auto rightPosition = row.find(rhs);
        if (leftPosition != std::string_view::npos && rightPosition != std::string_view::npos &&
            leftPosition > rightPosition ? leftPosition - rightPosition <= 1 : rightPosition - leftPosition <= 1) {
            return true;
        }
    }
    return false;
}

bool MatchesWithOneDrift(std::string_view input, std::string_view word) {
    std::size_t inputIndex = 0;
    std::size_t wordIndex = 0;
    int driftCount = 0;
    int matchedCharacters = 0;
    while (inputIndex < input.size() && wordIndex < word.size()) {
        const char inputCharacter = static_cast<char>(std::tolower(static_cast<unsigned char>(input[inputIndex])));
        const char wordCharacter = static_cast<char>(std::tolower(static_cast<unsigned char>(word[wordIndex])));
        if (inputCharacter == wordCharacter) {
            ++inputIndex;
            ++wordIndex;
            ++matchedCharacters;
            continue;
        }
        if (driftCount != 0) {
            return false;
        }
        if (IsNeighbor(inputCharacter, wordCharacter)) {
            ++inputIndex;
            ++wordIndex;
            ++driftCount;
            ++matchedCharacters;
            continue;
        }
        if (wordIndex + 1 < word.size() && inputCharacter == static_cast<char>(std::tolower(static_cast<unsigned char>(word[wordIndex + 1])))) {
            ++wordIndex;
            ++driftCount;
            continue;
        }
        if (inputIndex + 1 < input.size() && static_cast<char>(std::tolower(static_cast<unsigned char>(input[inputIndex + 1]))) == wordCharacter) {
            ++inputIndex;
            ++driftCount;
            continue;
        }
        return false;
    }
    return inputIndex == input.size() && matchedCharacters > 0;
}

}  // namespace

std::vector<Candidate> FuzzyTypoTranslator::Translate(std::string_view input, const std::vector<std::string>& dictionary) const {
    std::vector<Candidate> result;
    if (input.empty()) {
        return result;
    }
    for (std::size_t index = 0; index < dictionary.size(); ++index) {
        const std::string& word = dictionary[index];
        if (!MatchesWithOneDrift(input, word)) {
            continue;
        }
        const float coverage = static_cast<float>(input.size()) / static_cast<float>(std::max<std::size_t>(input.size(), word.size()));
        Candidate candidate;
        candidate.id = index + 1;
        candidate.text = word;
        candidate.rawText = word;
        candidate.comment = "Fuzzy/Typo";
        candidate.quality = std::clamp(0.20f + 0.40f * coverage, 0.20f, 0.60f);
        candidate.start = 0;
        candidate.end = input.size();
        candidate.score = candidate.quality;
        candidate.source = "FuzzyTypoTranslator";
        result.push_back(std::move(candidate));
    }
    return result;
}

std::string_view FuzzyTypoTranslator::GetName() const noexcept {
    return "FuzzyTypoTranslator";
}

}  // namespace PupKo::Translator