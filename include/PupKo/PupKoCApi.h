/* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
 * If a copy of the MPL was not distributed with this file, You can obtain one at
 * https://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2026 PupKo contributors.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PUPKO_EXPORT
#ifdef _WIN32
#define PUPKO_EXPORT __declspec(dllexport)
#else
#define PUPKO_EXPORT
#endif
#endif

PUPKO_EXPORT void PupKoCreateEngine(void** engineHandle);
PUPKO_EXPORT void PupKoDestroyEngine(void* engineHandle);
PUPKO_EXPORT int PupKoProcessKey(void* engineHandle, unsigned int keyCode, const char* text, int textLength);
PUPKO_EXPORT int PupKoGetCandidateCount(void* engineHandle);
PUPKO_EXPORT const char* PupKoGetCandidateText(void* engineHandle, int index);
PUPKO_EXPORT void PupKoClearInput(void* engineHandle);

#ifdef __cplusplus
}
#endif
