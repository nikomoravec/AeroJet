/*
 * FieldDescriptor.cpp
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

#include "Java/ClassFile/FieldDescriptor.hpp"

#include "Exceptions/RuntimeException.hpp"
#include "fmt/format.h"

namespace AeroJet::Java::ClassFile
{
    FieldDescriptor::FieldDescriptor(std::string descriptor) : m_rawLiteral(std::move(descriptor)), m_type(resolve()) {}

    std::optional<FieldDescriptor> FieldDescriptor::underlyingType() const
    {
        if(m_type == FieldType::ARRAY)
        {
            return FieldDescriptor(std::string(++m_rawLiteral.begin(), m_rawLiteral.end()));
        }

        return std::nullopt;
    }

    std::optional<std::string> FieldDescriptor::getClassName() const
    {
        if(m_type == FieldType::CLASS)
        {
            return std::string(++m_rawLiteral.begin(), --m_rawLiteral.end());
        }

        return std::nullopt;
    }

    FieldDescriptor::FieldType FieldDescriptor::fieldType() const
    {
        return m_type;
    }

    bool FieldDescriptor::isPrimitive() const
    {
        return m_type != FieldType::CLASS && m_type != FieldType::ARRAY;
    }

    bool FieldDescriptor::isClass() const
    {
        return m_type == FieldType::CLASS;
    }

    bool FieldDescriptor::isArray() const
    {
        return m_type == FieldType::ARRAY;
    }

    FieldDescriptor::FieldType FieldDescriptor::resolve()
    {
        if(m_rawLiteral.empty())
        {
            throw Exceptions::RuntimeException("Field descriptor can not be empty!");
        }

        const char first = *m_rawLiteral.begin();
        if(m_rawLiteral.size() > 1)
        {
            if(first == static_cast<char>(FieldType::CLASS))
            {
                if(m_rawLiteral[m_rawLiteral.size() - 1] == FIELD_TYPE_CLASS_END_TOKEN)
                {
                    return FieldType::CLASS;
                }
            }

            if(first == static_cast<char>(FieldType::ARRAY))
            {
                return FieldType::ARRAY;
            }
        }

        if(m_rawLiteral.size() != 1)
        {
            throw Exceptions::RuntimeException(fmt::format("Incorrect Field Descriptor literal: \"{}\"", m_rawLiteral));
        }

        switch(first)
        {
            case static_cast<char>(FieldType::BYTE):
                return FieldType::BYTE;
            case static_cast<char>(FieldType::CHAR):
                return FieldType::CHAR;
            case static_cast<char>(FieldType::DOUBLE):
                return FieldType::DOUBLE;
            case static_cast<char>(FieldType::FLOAT):
                return FieldType::FLOAT;
            case static_cast<char>(FieldType::INTEGER):
                return FieldType::INTEGER;
            case static_cast<char>(FieldType::LONG):
                return FieldType::LONG;
            case static_cast<char>(FieldType::SHORT):
                return FieldType::SHORT;
            case static_cast<char>(FieldType::BOOLEAN):
                return FieldType::BOOLEAN;
            default:
                throw Exceptions::RuntimeException(
                    fmt::format("Can not resolve type of field descriptor: \"{}\"", m_rawLiteral));
        }
    }
} // namespace AeroJet::Java::ClassFile