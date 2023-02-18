/*
 * InclusiveRange.hpp
 *
 * Copyright © 2023 AeroJet Developers. All Rights Reserved.
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

#include "Exceptions/RuntimeException.hpp"
#include "fmt/format.h"
#include "Types.hpp"

#include <type_traits>

namespace AeroJet::Math
{
    template<typename NumericType = i8> requires std::is_arithmetic_v<NumericType>
    class InclusiveRange
    {
      public:
        InclusiveRange(NumericType low, NumericType high) : m_low(low), m_high(high)
        {
            if(m_low > m_high)
            {
                throw Exceptions::RuntimeException(
                    fmt::format("Incorrect bounds! low({}) > high({})", m_low, m_high));
            }
        }

        [[nodiscard]] bool isInRange(NumericType value)
        {
            return value >= m_low && value <= m_high;
        }

      private:
        NumericType m_low;
        NumericType m_high;
    };
}