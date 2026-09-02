/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#include "Segmenter/ISegmenter.hpp"

namespace PupKo::Segmenter {

class CommandSegmenter final : public ISegmenter {
public:
    bool Proceed(Composition& composition) override;
    Segment ParseSegment(std::string_view input) const override;
    std::string_view GetName() const noexcept override;
};

}  // namespace PupKo::Segmenter