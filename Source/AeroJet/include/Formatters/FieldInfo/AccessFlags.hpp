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

#include "Java/ClassFile/FieldInfo.hpp"
#include "fmt/format.h"
#include "sstream"

template<>
struct fmt::formatter<AeroJet::Java::ClassFile::FieldInfo::AccessFlags> : fmt::formatter<std::string>
{
  private:
    void append(AeroJet::Java::ClassFile::FieldInfo::AccessFlags accessFlags,
                AeroJet::Java::ClassFile::FieldInfo::AccessFlags value,
                std::string_view                                 valueString,
                AeroJet::u1&                                     valuesCount,
                std::stringstream&                               stream) const
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
    auto format(AeroJet::Java::ClassFile::FieldInfo::AccessFlags accessFlags, FormatContext& ctx) const
    {
        std::stringstream value;

        AeroJet::u1 valuesCount = 0;

        append(accessFlags,
               AeroJet::Java::ClassFile::FieldInfo::AccessFlags::ACC_PUBLIC,
               "ACC_PUBLIC",
               valuesCount,
               value);

        append(accessFlags,
               AeroJet::Java::ClassFile::FieldInfo::AccessFlags::ACC_PRIVATE,
               "ACC_PRIVATE",
               valuesCount,
               value);

        append(accessFlags,
               AeroJet::Java::ClassFile::FieldInfo::AccessFlags::ACC_PROTECTED,
               "ACC_PROTECTED",
               valuesCount,
               value);

        append(accessFlags,
               AeroJet::Java::ClassFile::FieldInfo::AccessFlags::ACC_STATIC,
               "ACC_STATIC",
               valuesCount,
               value);

        append(accessFlags,
               AeroJet::Java::ClassFile::FieldInfo::AccessFlags::ACC_FINAL,
               "ACC_FINAL",
               valuesCount,
               value);

        append(accessFlags,
               AeroJet::Java::ClassFile::FieldInfo::AccessFlags::ACC_VOLATILE,
               "ACC_VOLATILE",
               valuesCount,
               value);

        append(accessFlags,
               AeroJet::Java::ClassFile::FieldInfo::AccessFlags::ACC_TRANSIENT,
               "ACC_TRANSIENT",
               valuesCount,
               value);

        append(accessFlags,
               AeroJet::Java::ClassFile::FieldInfo::AccessFlags::ACC_SYNTHETIC,
               "ACC_SYNTHETIC",
               valuesCount,
               value);

        append(accessFlags,
               AeroJet::Java::ClassFile::FieldInfo::AccessFlags::ACC_ENUM,
               "ACC_ENUM",
               valuesCount,
               value);

        return fmt::formatter<std::string>::format(value.str(), ctx);
    }

};
