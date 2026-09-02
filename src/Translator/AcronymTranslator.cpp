/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#include "Translator/AcronymTranslator.hpp"

#include <algorithm>
#include <cctype>

namespace PupKo::Translator {
namespace {

std::string BuildAcronym(std::string_view word) {
    std::string acronym;
    bool afterSeparator = true;
    for (char character : word) {
        const unsigned char value = static_cast<unsigned char>(character);
        if (character == '-' || character == '_' || character == '.' || character == ' ') {
            afterSeparator = true;
            continue;
        }
        if (std::isalpha(value) && (afterSeparator || std::isupper(value))) {
            acronym.push_back(static_cast<char>(std::tolower(value)));
        }
        afterSeparator = false;
    }
    return acronym;
}

bool StartsWithInsensitive(std::string_view value, std::string_view prefix) {
    if (prefix.size() > value.size()) {
        return false;
    }
    return std::equal(prefix.begin(), prefix.end(), value.begin(), [](char left, char right) {
        return std::tolower(static_cast<unsigned char>(left)) == std::tolower(static_cast<unsigned char>(right));
    });
}

}  // namespace

std::vector<Candidate> AcronymTranslator::Translate(std::string_view input, const std::vector<std::string>& dictionary) const {
    std::vector<Candidate> result;
    for (std::size_t index = 0; index < dictionary.size(); ++index) {
        const std::string acronym = BuildAcronym(dictionary[index]);
        if (!StartsWithInsensitive(acronym, input)) {
            continue;
        }
        Candidate candidate;
        candidate.id = index + 1;
        candidate.text = dictionary[index];
        candidate.rawText = dictionary[index];
        candidate.comment = "Acronym";
        candidate.quality = 0.80f + 0.10f * static_cast<float>(input.size()) / static_cast<float>(std::max<std::size_t>(1, acronym.size()));
        candidate.start = 0;
        candidate.end = input.size();
        candidate.score = candidate.quality;
        candidate.source = "AcronymTranslator";
        result.push_back(std::move(candidate));
    }
    return result;
}

std::string_view AcronymTranslator::GetName() const noexcept {
    return "AcronymTranslator";
}

}  // namespace PupKo::Translator