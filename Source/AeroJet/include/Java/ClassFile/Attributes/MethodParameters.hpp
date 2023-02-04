/*
 * MethodParameters.hpp
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

#include "Java/ClassFile/Attributes/Attribute.hpp"

#include <vector>

namespace AeroJet::Java::ClassFile
{
    /**
     * The MethodParameters attribute is a variable-length attribute in the attributes table of a
     * method_info structure (§4.6). A MethodParameters attribute records information about the formal parameters of a
     * method, such as their names.
     * There may be at most one MethodParameters attribute in the attributes table of a method_info structure.
     */
    class MethodParameters : Attribute
    {
      public:
        enum class MethodParameterAccessFlags : u2
        {
            /**
             * Indicates that the formal parameter was declared final.
             */
            ACC_FINAL = 0x0010,

            /**
             * Indicates that the formal parameter was not explicitly or implicitly declared in source code,
             * according to the specification of the language in which the source code was written (JLS §13.1).
             * (The formal parameter is an implementation artifact of the compiler which produced this class file.)
             */
            ACC_SYNTHETIC = 0x1000,

            /**
             * Indicates that the formal parameter was implicitly declared in source code, according to the
             * specification of the language in which the source code was written (JLS §13.1).
             * (The formal parameter is mandated by a language specification, so all compilers for the language must emit it.)
             */
            ACC_MANDATED = 0x8000
        };

        class MethodParameter
        {
          public:
            MethodParameter(u2 nameIndex, MethodParameterAccessFlags accessFlags);

            [[nodiscard]] u2 nameIndex() const;
            [[nodiscard]] MethodParameterAccessFlags accessFlags() const;

          private:
            /*
             * The value of the name_index item must either be zero or a valid index into the constant_pool table.
             * If the value of the name_index item is zero, then this parameters element indicates a formal
             * parameter with no name.
             *
             * If the value of the name_index item is nonzero, the constant_pool entry at that index must be a
             * CONSTANT_Utf8_info structure representing a valid unqualified name denoting a formal parameter (§4.2.2).
             */
            u2 m_nameIndex;

            MethodParameterAccessFlags m_accessFlags;
        };

        static constexpr auto METHOD_PARAMETERS_ATTRIBUTE_NAME = "MethodParameters";

        MethodParameters(const ConstantPool& constantPool, const AttributeInfo& attributeInfo);
        [[nodiscard]] u1 parametersCount() const;
        [[nodiscard]] const std::vector<MethodParameter>& methodParameters() const;

      private:
        std::vector<MethodParameter> m_methodParameters;
    };
} // namespace AeroJet::Java::ClassFile