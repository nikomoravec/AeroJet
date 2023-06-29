/*
 * MethodDescriptor.cpp
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

#include "Java/ClassFile/MethodDescriptor.hpp"

#include "Exceptions/RuntimeException.hpp"
#include "fmt/format.h"
#include "Java/ClassFile/FieldDescriptor.hpp"

namespace
{
    inline bool isPrimitive(char fieldType)
    {
        switch(fieldType)
        {
            case static_cast<char>(AeroJet::Java::ClassFile::FieldDescriptor::FieldType::BYTE):
            case static_cast<char>(AeroJet::Java::ClassFile::FieldDescriptor::FieldType::CHAR):
            case static_cast<char>(AeroJet::Java::ClassFile::FieldDescriptor::FieldType::DOUBLE):
            case static_cast<char>(AeroJet::Java::ClassFile::FieldDescriptor::FieldType::FLOAT):
            case static_cast<char>(AeroJet::Java::ClassFile::FieldDescriptor::FieldType::INTEGER):
            case static_cast<char>(AeroJet::Java::ClassFile::FieldDescriptor::FieldType::LONG):
            case static_cast<char>(AeroJet::Java::ClassFile::FieldDescriptor::FieldType::SHORT):
            case static_cast<char>(AeroJet::Java::ClassFile::FieldDescriptor::FieldType::BOOLEAN):
                return true;
            case static_cast<char>(AeroJet::Java::ClassFile::FieldDescriptor::FieldType::CLASS):
            case static_cast<char>(AeroJet::Java::ClassFile::FieldDescriptor::FieldType::ARRAY):
                return false;
            default:
                throw AeroJet::Exceptions::RuntimeException(fmt::format("Unexpected FieldType token '{}'!", fieldType));
        }
    }

    inline size_t resolveArray(const std::string& literal,
                               std::vector<AeroJet::Java::ClassFile::FieldDescriptor>& arguments,
                               size_t position)
    {
        const size_t underlyingTypeIndex =
            literal.find_first_not_of(static_cast<char>(AeroJet::Java::ClassFile::FieldDescriptor::FieldType::ARRAY),
                                      position);
        if(isPrimitive(literal[underlyingTypeIndex]))
        {
            arguments.emplace_back(literal.substr(position, underlyingTypeIndex - position + 1));
            return underlyingTypeIndex;
        }

        if(literal[underlyingTypeIndex] ==
           static_cast<char>(AeroJet::Java::ClassFile::FieldDescriptor::FieldType::CLASS))
        {
            const size_t classNameEndPos =
                literal.find(AeroJet::Java::ClassFile::FieldDescriptor::FIELD_TYPE_CLASS_END_TOKEN,
                             underlyingTypeIndex);
            arguments.emplace_back(literal.substr(position, classNameEndPos - (position - 1)));
            return classNameEndPos;
        }

        throw AeroJet::Exceptions::RuntimeException(
            fmt::format("Unexpected token at position {} in \"{}\"", underlyingTypeIndex, literal));
    }

    inline size_t resolvePrimitive(const std::string& literal,
                                   std::vector<AeroJet::Java::ClassFile::FieldDescriptor>& arguments,
                                   size_t position)
    {
        arguments.emplace_back(AeroJet::Java::ClassFile::FieldDescriptor({ literal[position] }));
        return position;
    }

    inline size_t resolveClass(const std::string& literal,
                               std::vector<AeroJet::Java::ClassFile::FieldDescriptor>& arguments,
                               size_t position)
    {
        const size_t classNameEndPos =
            literal.find(AeroJet::Java::ClassFile::FieldDescriptor::FIELD_TYPE_CLASS_END_TOKEN, position);
        if(classNameEndPos != std::string::npos)
        {
            arguments.emplace_back(literal.substr(position, classNameEndPos - (position - 1)));
            return static_cast<int32_t>(classNameEndPos);
        }

        throw AeroJet::Exceptions::RuntimeException(fmt::format("Can not resolve method descriptor: \"{}\"", literal));
    }
} // namespace

namespace AeroJet::Java::ClassFile
{
    MethodDescriptor::MethodDescriptor(std::string inDescriptor) :
        m_rawLiteral(std::move(inDescriptor))
    {
        if(*m_rawLiteral.begin() == METHOD_DESCRIPTOR_ARGS_BEGIN_TOKEN)
        {
            size_t argsEndTokenPosition = m_rawLiteral.find(METHOD_DESCRIPTOR_ARGS_END_TOKEN);
            if(argsEndTokenPosition != std::string::npos)
            {
                const std::string returnTypeLiteral =
                    m_rawLiteral.substr(argsEndTokenPosition + 1, m_rawLiteral.size() - argsEndTokenPosition);
                if(returnTypeLiteral.size() == 1)
                {
                    static constexpr char RETURN_DESCRIPTOR_VOID_TYPE_TOKEN = 'V';

                    if(returnTypeLiteral[0] == RETURN_DESCRIPTOR_VOID_TYPE_TOKEN)
                    {
                        m_returnType = std::nullopt;
                    }
                }
                else
                {
                    m_returnType = FieldDescriptor({ returnTypeLiteral });
                }

                std::string argsLiteral = m_rawLiteral.substr(1, argsEndTokenPosition - 1);

                for(size_t charIndex = 0; charIndex < argsLiteral.size(); charIndex++)
                {
                    const char c = argsLiteral[charIndex];
                    if(isPrimitive(c))
                    {
                        charIndex = resolvePrimitive(argsLiteral, m_arguments, charIndex);
                    }
                    else
                    {
                        if(c == static_cast<char>(FieldDescriptor::FieldType::CLASS))
                        {
                            charIndex = resolveClass(argsLiteral, m_arguments, charIndex);
                            continue;
                        }

                        if(c == static_cast<char>(FieldDescriptor::FieldType::ARRAY))
                        {
                            charIndex = resolveArray(argsLiteral, m_arguments, charIndex);
                            continue;
                        }
                    }
                }
            }
            else
            {
                throw AeroJet::Exceptions::RuntimeException(
                    fmt::format("Can not resolve method descriptor: \"{}\"", m_rawLiteral));
            }
        }
        else
        {
            throw AeroJet::Exceptions::RuntimeException(
                fmt::format("Can not resolve method descriptor: \"{}\"", m_rawLiteral));
        }
    }

    std::optional<FieldDescriptor> MethodDescriptor::returnType() const
    {
        return m_returnType;
    }

    const std::vector<FieldDescriptor>& MethodDescriptor::arguments() const
    {
        return m_arguments;
    }

    std::string_view MethodDescriptor::rawLiteral() const
    {
        return m_rawLiteral;
    }
} // namespace AeroJet::Java::ClassFile
