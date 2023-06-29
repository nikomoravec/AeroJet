/*
 * ElementValue.hpp
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

#include "Types.hpp"

#include <memory>
#include <variant>
#include <vector>

namespace AeroJet::Java::ClassFile
{
    class Annotation;

    class ElementValue
    {
      public:
        enum class Tag : u1
        {
            BYTE = 'B',
            CHAR = 'C',
            DOUBLE = 'D',
            FLOAT = 'F',
            INT = 'I',
            LONG = 'J',
            SHORT = 'S',
            BOOLEAN = 'Z',
            STRING = 's',
            ENUM_TYPE = 'e',
            CLASS = 'c',
            ANNOTATION_TYPE = '@',
            ARRAY_TYPE = '['
        };

        class EnumConstValue
        {
          public:
            EnumConstValue(u2 typeNameIndex, u2 constNameIndex);

            [[nodiscard]] u2 typeNameIndex() const;
            [[nodiscard]] u2 constNameIndex() const;

          private:
            u2 m_typeNameIndex;
            u2 m_constNameIndex;
        };

        class ArrayValue
        {
          public:
            explicit ArrayValue(const std::vector<ElementValue>& values);

            [[nodiscard]] u2 numValues() const;
            [[nodiscard]] const std::vector<ElementValue>& values() const;

          private:
            std::vector<ElementValue> m_values;
        };

        using Value = std::variant<u2, EnumConstValue, std::shared_ptr<Annotation>, ArrayValue>;

        ElementValue(Tag tag, Value value);

        [[nodiscard]] Tag tag() const;
        [[nodiscard]] const Value& value() const;

      private:
        Tag m_tag;
        Value m_value;
    };
} // namespace AeroJet::Java::ClassFile