/*
 * InnerClasses.hpp
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
#include "Java/ClassFile/Attributes/AttributeInfo.hpp"
#include "Java/ClassFile/ConstantPool.hpp"
#include "Types.hpp"

#include <vector>

namespace AeroJet::Java::ClassFile
{
    class InnerClasses : public Attribute
    {
      public:
        static constexpr auto INNER_CLASSES_ATTRIBUTE_NAME = "InnerClasses";

        enum class InnerClassAccessFlags : u2
        {
            ACC_PUBLIC = 0x0001,
            ACC_PRIVATE = 0x0002,
            ACC_PROTECTED = 0x0004,
            ACC_STATIC = 0x0008,
            ACC_FINAL = 0x0010,
            ACC_INTERFACE = 0x0200,
            ACC_ABSTRACT = 0x0400,
            ACC_SYNTHETIC = 0x1000,
            ACC_ANNOTATION = 0x2000,
            ACC_ENUM = 0x4000
        };

        class InnerClass
        {
          public:
            InnerClass(u2 innerClassInfoIndex, u2 outerClassInfoIndex, u2 innerNameIndex, InnerClassAccessFlags innerClassAccessFlags);

            [[nodiscard]] u2 innerClassInfoIndex() const;
            [[nodiscard]] u2 outerClassInfoIndex() const;
            [[nodiscard]] u2 innerNameIndex() const;
            [[nodiscard]] InnerClassAccessFlags innerClassAccessFlags() const;

          private:
            u2 m_innerClassInfoIndex;
            u2 m_outerClassInfoIndex;
            u2 m_innerNameIndex;
            InnerClassAccessFlags m_innerClassAccessFlags;
        };

        InnerClasses(const ConstantPool& constantPool, const AttributeInfo& attributeInfo);

        [[nodiscard]] u2 numberOfClasses();
        [[nodiscard]] const std::vector<InnerClass>& innerClasses() const;

      protected:
        std::vector<InnerClass> m_InnerClasses;
    };
} // namespace AeroJet::Java::ClassFile
