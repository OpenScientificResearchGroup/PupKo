/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#include "PupKo/Engine.hpp"

#include <algorithm>
#include <string>
#include <vector>

#include "Filter/HabitTuningFilter.hpp"
#include "Filter/RankerFilter.hpp"
#include "Filter/UniquifierFilter.hpp"
#include "Processor/KeyBinderProcessor.hpp"
#include "Processor/SelectorProcessor.hpp"
#include "Processor/SpellerProcessor.hpp"
#include "Translator/AcronymTranslator.hpp"
#include "Translator/FuzzyTypoTranslator.hpp"
#include "Translator/PrefixTranslator.hpp"
#include "Translator/SubstringTranslator.hpp"
#include "Session.hpp"
#include "Utils/StringUtils.hpp"

namespace pupko {

class Engine::Impl {
public:
    Session session;
    std::vector<std::string> dictionary = {"pupko", "pup", "ko", "camel", "code", "prefix", "substring", "fuzzy", "habit", "score"};
    Processor::SpellerProcessor speller;
    Processor::SelectorProcessor selector;
    Processor::KeyBinderProcessor binder;
    Translator::PrefixTranslator prefixTranslator;
    Translator::AcronymTranslator acronymTranslator;
    Translator::SubstringTranslator substringTranslator;
    Translator::FuzzyTypoTranslator fuzzyTranslator;
    Filter::UniquifierFilter uniquifier;
    Filter::HabitTuningFilter habitTuning;
    Filter::RankerFilter ranker;

    void RefreshCandidates() {
        std::vector<Candidate> combined;
        auto prefixMatches = prefixTranslator.Translate(session.GetCurrentInput(), dictionary);
        auto acronymMatches = acronymTranslator.Translate(session.GetCurrentInput(), dictionary);
        auto substringMatches = substringTranslator.Translate(session.GetCurrentInput(), dictionary);
        auto fuzzyMatches = fuzzyTranslator.Translate(session.GetCurrentInput(), dictionary);

        combined.insert(combined.end(), prefixMatches.begin(), prefixMatches.end());
        combined.insert(combined.end(), acronymMatches.begin(), acronymMatches.end());
        combined.insert(combined.end(), substringMatches.begin(), substringMatches.end());
        combined.insert(combined.end(), fuzzyMatches.begin(), fuzzyMatches.end());

        auto unique = uniquifier.Filter(combined);
        auto tuned = habitTuning.Filter(unique);
        auto ranked = ranker.Filter(tuned);
        session.SetCandidates(ranked);
    }
};

Engine::Engine() : mImpl(std::make_unique<Impl>()) {}

Engine::~Engine() = default;

void Engine::Reset() {
    mImpl->session.Clear();
}

void Engine::Clear() {
    mImpl->session.Clear();
}

bool Engine::ProcessKey(KeyEvent event) {
    Composition composition;
    composition.rawInput = mImpl->session.GetCurrentInput();
    composition.candidates = mImpl->session.GetCandidates();
    composition.selectedIndex = mImpl->session.GetSelectedIndex();
    if (!composition.candidates.empty()) {
        Segment segment;
        segment.candidates = composition.candidates;
        composition.segments.push_back(segment);
    }

    const Processor::Result binderResult = mImpl->binder.ProcessKey(composition, event);
    if (binderResult != Processor::Result::NoOperation) {
        mImpl->session.SetCurrentInput(composition.rawInput);
        mImpl->session.SetCandidates(composition.candidates);
        mImpl->session.SelectCandidate(composition.selectedIndex);
        return binderResult == Processor::Result::Accepted;
    }

    const Processor::Result spellerResult = mImpl->speller.ProcessKey(composition, event);
    if (spellerResult == Processor::Result::Accepted) {
        mImpl->session.SetCurrentInput(composition.rawInput);
        mImpl->session.SetCandidates(composition.candidates);
        mImpl->session.SelectCandidate(composition.selectedIndex);
        mImpl->RefreshCandidates();
        return true;
    }

    return false;
}

std::vector<Candidate> Engine::GetCandidates() const {
    return mImpl->session.GetCandidates();
}

std::string Engine::GetCurrentInput() const {
    return mImpl->session.GetCurrentInput();
}

std::size_t Engine::GetSelectedIndex() const {
    return mImpl->session.GetSelectedIndex();
}

void Engine::SelectCandidate(std::size_t index) {
    mImpl->session.SelectCandidate(index);
}

void Engine::CommitCandidate(std::size_t index) {
    mImpl->session.CommitCandidate(index);
}

}  // namespace PupKo
