/*
 * MethodInfo.hpp
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

#include "Java/ClassFile/Attributes/AttributeInfo.hpp"
#include "Types.hpp"
#include <vector>

namespace AeroJet::Java::ClassFile
{
    /*
     * 4.6 Methods
     *
     * Each method, including each instance initialization method (§3.9) and the class or interface
     * initialization method (§3.9), is described by a method_info structure.
     * No two methods in one class file may have the same name and descriptor (§4.3.3).
     *
     * The structure has the following format:
     * method_info {
     *	u2 access_flags;
     *	u2 name_index;
     *	u2 descriptor_index;
     *	u2 attributes_count;
     *	attribute_info attributes[attributes_count];
     * }
     */

    class MethodInfo
    {
    public:
        /*
         * access_flags
         * The value of the access_flags item is a mask of flags used to denote access permission to and properties
         * of this method. The interpretation of each flag, when set, is as shown in Table 4.5.
         *
         * Flag Name	       Value	Interpretation
         * ACC_PUBLIC	       0x0001	Declared public; may be accessed from outside its package.
         * ACC_PRIVATE	       0x0002	Declared private; accessible only within the defining class.
         * ACC_PROTECTED	   0x0004	Declared protected; may be accessed within subclasses.
         * ACC_STATIC	       0x0008	Declared static.
         * ACC_FINAL	       0x0010	Declared final; may not be overridden.
         * ACC_SYNCHRONIZED	   0x0020	Declared synchronized; invocation is wrapped in a monitor lock.
         * ACC_NATIVE	       0x0100	Declared native; implemented in a language other than Java.
         * ACC_ABSTRACT	       0x0400	Declared abstract; no implementation is provided.
         * ACC_STRICT	       0x0800	Declared strictfp; floating-point mode is FP-strict
         */
        enum class AccessFlags : u2
        {
            ACC_PUBLIC = 0x0001,
            ACC_PRIVATE = 0x0002,
            ACC_PROTECTED = 0x0004,
            ACC_STATIC = 0x0008,
            ACC_FINAL = 0x0010,
            ACC_SYNCHRONIZED = 0x0020,
            ACC_BRIDGE = 0x0040,
            ACC_VARARGS = 0x0080,
            ACC_NATIVE = 0x0100,
            ACC_ABSTRACT = 0x0400,
            ACC_STRICT = 0x0800,
            ACC_SYNTHETIC = 0x1000
        };

        MethodInfo(u2 accessFlags, u2 nameIndex, u2 descriptorIndex, const std::vector<AttributeInfo>& attributes);

        [[nodiscard]]
        AccessFlags accessFlags() const;

        [[nodiscard]]
        u2 nameIndex() const;

        [[nodiscard]]
        u2 descriptorIndex() const;

        [[nodiscard]]
        const std::vector<AttributeInfo>& attributes() const;

    protected:
        u2 m_accessFlags;
        u2 m_nameIndex;
        u2 m_descriptorIndex;
        std::vector<AttributeInfo> m_attributes;
    };
}
