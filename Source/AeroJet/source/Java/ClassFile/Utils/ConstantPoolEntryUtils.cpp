/*
 * ConstantPoolEntryUtils.cpp
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

#include "Java/ClassFile/Utils/ConstantPoolEntryUtils.hpp"

namespace AeroJet::Java::ClassFile::Utils
{
    union BigNumber
    {
      public:
        BigNumber(u4 hBytes, u4 lBytes)
        {
            m_data[0] = hBytes;
            m_data[1] = lBytes;
        }

        i8 longValue()
        {
            return m_longValue;
        }

        double doubleValue()
        {
            return m_doubleValue;
        }

      private:
        i4 m_data[2];
        i8 m_longValue;
        double m_doubleValue;
    };

    i8 ConstantPoolEntryUtils::toLong(const AeroJet::Java::ClassFile::ConstantPoolInfoLong& constantPoolInfoLong)
    {
        BigNumber bigNumber{ constantPoolInfoLong.highBytes(), constantPoolInfoLong.lowBytes() };
        return bigNumber.longValue();
    }

    double ConstantPoolEntryUtils::toDouble(const ConstantPoolInfoLong& constantPoolInfoLong)
    {
        BigNumber bigNumber{ constantPoolInfoLong.highBytes(), constantPoolInfoLong.lowBytes() };
        return bigNumber.doubleValue();
    }
} // namespace AeroJet::Java::ClassFile::Utils