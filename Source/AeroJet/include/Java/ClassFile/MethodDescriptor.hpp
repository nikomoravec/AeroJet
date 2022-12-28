/*
 * MethodDescriptor.hpp
 *
 * Copyright © 2022 AeroJet Developers. All Rights Reserved.
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

#include "Java/ClassFile/FieldDescriptor.hpp"
#include "Types.hpp"
#include <string>
#include <string_view>
#include <optional>
#include <vector>

namespace AeroJet::Java::ClassFile
{
    class MethodDescriptor
    {
    public:
        static constexpr char METHOD_DESCRIPTOR_ARGS_BEGIN_TOKEN = '(';
        static constexpr char METHOD_DESCRIPTOR_ARGS_END_TOKEN = ')';

    public:
        explicit MethodDescriptor(std::string inDescriptor);

        [[nodiscard]]
        std::optional<FieldDescriptor> returnType() const;

        [[nodiscard]]
        std::vector<FieldDescriptor> arguments() const;

        [[nodiscard]]
        std::string_view rawLiteral() const;

    protected:
        std::optional<FieldDescriptor> m_returnType;
        std::vector<FieldDescriptor> m_arguments;
        std::string m_rawLiteral;
    };
}
