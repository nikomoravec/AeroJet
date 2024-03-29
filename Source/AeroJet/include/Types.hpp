/*
 * Types.hpp
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

namespace AeroJet
{
    using i1 = signed char;
    using i2 = signed short;
    using i4 = signed int;
    using i8 = signed long long;

    using u1 = unsigned char;
    using u2 = unsigned short;
    using u4 = unsigned int;
    using u8 = unsigned long long;

    static_assert(sizeof(u1) == 1);
    static_assert(sizeof(u2) == 2);
    static_assert(sizeof(u4) == 4);
    static_assert(sizeof(u8) == 8);

    static_assert(sizeof(i1) == 1);
    static_assert(sizeof(i2) == 2);
    static_assert(sizeof(i4) == 4);
    static_assert(sizeof(i8) == 8);
} // namespace AeroJet
