/*
 * ErrorCodes.hpp
 *
 * Copyright © 2024 AeroJet Developers. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include "Types.hpp"

namespace AeroJet::Compiler::ErrorCodes
{
    static constexpr i4 EXIT_CODE_OK = 0;
    static constexpr i4 EXIT_CODE_INVALID_ARGS = 1;
    static constexpr i4 EXIT_CODE_FAILED_TO_OPEN_MAIN_CLASS = 2;
    static constexpr i4 EXIT_CODE_FAILED_TO_READ_MAIN_CLASS = 3;
    static constexpr i4 EXIT_CODE_LLVM_LOOKUP_TARGET_ERROR = 4;
    static constexpr i4 EXIT_CODE_IO_ERROR = 5;
    static constexpr i4 EXIT_CODE_LLVM_OBJECT_FILE_NOT_SUPPORTED = 6;
    static constexpr i4 EXIT_CODE_AERO_LIB_ERROR = 254;
    static constexpr i4 EXIT_CODE_ERROR_UNKNOWN = 255;
} // namespace AeroJet::Compiler::ErrorCodes
