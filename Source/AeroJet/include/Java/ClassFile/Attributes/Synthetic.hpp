/*
 * Synthetic.hpp
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

#pragma once

#include "Java/ClassFile/Attributes/Attribute.hpp"
#include "Java/ClassFile/Attributes/AttributeInfo.hpp"
#include "Java/ClassFile/ConstantPool.hpp"
#include "Types.hpp"

namespace AeroJet::Java::ClassFile
{
    /**
     * The Synthetic attribute is a fixed-length attribute in the attributes table of a ClassFile,
     * field_info, or method_info structure (§4.1, §4.5, §4.6). A class member that does not appear in the source
     * code must be marked using a Synthetic attribute, or else it must have its ACC_SYNTHETIC flag set. The only
     * exceptions to this requirement are compiler-generated methods which are not considered implementation artifacts,
     * namely the instance initialization method representing a default constructor of the Java
     * programming language (§2.9), the class initialization method (§2.9), and the Enum.values()
     * and Enum.valueOf() methods.
     */
    class Synthetic : public Attribute
    {
      public:
        static constexpr auto SYNTHETIC_ATTRIBUTE_NAME = "Synthetic";

        Synthetic(const ConstantPool& constantPool, const AttributeInfo& attributeInfo);
    };
} // namespace AeroJet::Java::ClassFile