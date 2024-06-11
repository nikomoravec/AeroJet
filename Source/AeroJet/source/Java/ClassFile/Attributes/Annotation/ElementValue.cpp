/*
 * ElementValue.cpp
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

#include "Java/ClassFile/Attributes/Annotation/ElementValue.hpp"

#include "Exceptions/RuntimeException.hpp"
#include "fmt/format.h"
#include "Java/ClassFile/Attributes/Annotation/Annotation.hpp"

#include <utility>

namespace AeroJet::Java::ClassFile
{
    ElementValue::EnumConstValue::EnumConstValue(u2 typeNameIndex, u2 constNameIndex) :
        m_typeNameIndex(typeNameIndex), m_constNameIndex(constNameIndex)
    {
    }

    u2 ElementValue::EnumConstValue::typeNameIndex() const
    {
        return m_typeNameIndex;
    }

    u2 ElementValue::EnumConstValue::constNameIndex() const
    {
        return m_constNameIndex;
    }

    ElementValue::ArrayValue::ArrayValue(const std::vector<ElementValue>& values) :
        m_values(values) {}

    u2 ElementValue::ArrayValue::numValues() const
    {
        return m_values.size();
    }

    const std::vector<ElementValue>& ElementValue::ArrayValue::values() const
    {
        return m_values;
    }

    ElementValue::ElementValue(ElementValue::Tag tag, ElementValue::Value value) :
        m_tag(tag), m_value(std::move(value))
    {
    }

    ElementValue::Tag ElementValue::tag() const
    {
        return m_tag;
    }

    const ElementValue::Value& ElementValue::value() const
    {
        return m_value;
    }
} // namespace AeroJet::Java::ClassFile
