/*
 * FieldDescriptor.hpp
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

#include <optional>
#include <string>

namespace AeroJet::Java::ClassFile
{
    class FieldDescriptor
    {
      public:
        enum class FieldType : char
        {
            BYTE    = 'B',
            CHAR    = 'C',
            DOUBLE  = 'D',
            FLOAT   = 'F',
            INTEGER = 'I',
            LONG    = 'J',
            CLASS   = 'L',
            SHORT   = 'S',
            BOOLEAN = 'Z',
            ARRAY   = '['
        };

        static constexpr char FIELD_TYPE_CLASS_END_TOKEN = ';';

      public:
        explicit FieldDescriptor(std::string descriptor);

        [[nodiscard]] std::optional<FieldDescriptor> underlyingType() const;

        [[nodiscard]] std::optional<std::string> getClassName() const;

        [[nodiscard]] FieldType fieldType() const;

        [[nodiscard]] bool isPrimitive() const;

        [[nodiscard]] bool isClass() const;

        [[nodiscard]] bool isArray() const;

      protected:
        FieldType resolve();

      protected:
        std::string m_rawLiteral;
        FieldType   m_type;
    };
} // namespace AeroJet::Java::ClassFile
