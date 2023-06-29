/*
 * AccessFlags.hpp
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

#include "fmt/format.h"
#include "Java/ClassFile/ClassInfo.hpp"

#include <sstream>
#include <string_view>

template<>
struct fmt::formatter<AeroJet::Java::ClassFile::ClassInfo::AccessFlags> : fmt::formatter<std::string>
{
  private:
    void append(AeroJet::Java::ClassFile::ClassInfo::AccessFlags accessFlags,
                AeroJet::Java::ClassFile::ClassInfo::AccessFlags value,
                std::string_view valueString,
                AeroJet::u1& valuesCount,
                std::stringstream& stream) const
    {
        if(accessFlags & value)
        {
            if(valuesCount > 0)
            {
                stream << " | ";
            }

            valuesCount++;
            stream << valueString;
        }
    }

  public:
    template<typename FormatContext>
    auto format(AeroJet::Java::ClassFile::ClassInfo::AccessFlags accessFlags, FormatContext& ctx) const
    {
        std::stringstream value;

        AeroJet::u1 valuesCount = 0;

        append(accessFlags,
               AeroJet::Java::ClassFile::ClassInfo::AccessFlags::ACC_PUBLIC,
               "ACC_PUBLIC",
               valuesCount,
               value);

        append(accessFlags,
               AeroJet::Java::ClassFile::ClassInfo::AccessFlags::ACC_FINAL,
               "ACC_FINAL",
               valuesCount,
               value);

        append(accessFlags,
               AeroJet::Java::ClassFile::ClassInfo::AccessFlags::ACC_SUPER,
               "ACC_SUPER",
               valuesCount,
               value);

        append(accessFlags,
               AeroJet::Java::ClassFile::ClassInfo::AccessFlags::ACC_INTERFACE,
               "ACC_INTERFACE",
               valuesCount,
               value);

        append(accessFlags,
               AeroJet::Java::ClassFile::ClassInfo::AccessFlags::ACC_ABSTRACT,
               "ACC_ABSTRACT",
               valuesCount,
               value);

        append(accessFlags,
               AeroJet::Java::ClassFile::ClassInfo::AccessFlags::ACC_SYNTHETIC,
               "ACC_SYNTHETIC",
               valuesCount,
               value);

        append(accessFlags,
               AeroJet::Java::ClassFile::ClassInfo::AccessFlags::ACC_ANNOTATION,
               "ACC_ANNOTATION",
               valuesCount,
               value);

        append(accessFlags,
               AeroJet::Java::ClassFile::ClassInfo::AccessFlags::ACC_ENUM,
               "ACC_ENUM",
               valuesCount,
               value);

        return fmt::formatter<std::string>::format(value.str(), ctx);
    }
};
