/*
 * SourceDebugExtension.hpp
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
     * The SourceDebugExtension attribute is an optional attribute in the attributes table of a ClassFile structure (§4.1).
     * There may be at most one SourceDebugExtension attribute in the attributes table of a ClassFile structure.
     */
    class SourceDebugExtension : public Attribute
    {
      public:
        static constexpr auto SOURCE_DEBUG_EXTENSION_ATTRIBUTE_NAME = "SourceDebugExtension";

        SourceDebugExtension(const ConstantPool& constantPool, const AttributeInfo& attributeInfo);
        [[nodiscard]] const std::vector<u1>& debugExtension() const;

      private:
        /**
         * The debug_extension array holds extended debugging information which has no semantic effect on the
         * Java Virtual Machine. The information is represented using a modified UTF-8 string (§4.4.7)
         * with no terminating zero byte.
         */
        std::vector<u1> m_debugExtension;
    };
} // namespace AeroJet::Java::ClassFile