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

#include "Java/ClassFile/Attributes/InnerClasses.hpp"

#include "Stream/Reader.hpp"

namespace AeroJet::Java::ClassFile
{
    InnerClasses::InnerClass::InnerClass(u2                                  innerClassInfoIndex,
                                         u2                                  outerClassInfoIndex,
                                         u2                                  innerNameIndex,
                                         InnerClasses::InnerClassAccessFlags innerClassAccessFlags) :
        m_innerClassInfoIndex(innerClassInfoIndex),
        m_outerClassInfoIndex(outerClassInfoIndex),
        m_innerNameIndex(innerNameIndex),
        m_innerClassAccessFlags(innerClassAccessFlags)
    {
    }

    u2 InnerClasses::InnerClass::innerClassInfoIndex() const
    {
        return m_innerClassInfoIndex;
    }

    u2 InnerClasses::InnerClass::outerClassInfoIndex() const
    {
        return m_outerClassInfoIndex;
    }

    u2 InnerClasses::InnerClass::innerNameIndex() const
    {
        return m_innerNameIndex;
    }

    InnerClasses::InnerClassAccessFlags InnerClasses::InnerClass::innerClassAccessFlags() const
    {
        return m_innerClassAccessFlags;
    }

    InnerClasses::InnerClasses(const ConstantPool& constantPool, const AttributeInfo& attributeInfo) :
        Attribute(constantPool, attributeInfo, INNER_CLASSES_ATTRIBUTE_NAME)
    {
        const u2 numberOfClasses = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);
        m_InnerClasses.reserve(numberOfClasses);
        for(size_t classIndex = 0; classIndex < numberOfClasses; classIndex++)
        {
            u2 innerClassInfoIndex = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);
            u2 outerClassInfoIndex = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);
            u2 innerNameIndex      = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);
            InnerClassAccessFlags innerClassAccessFlags = static_cast<InnerClassAccessFlags>(
                Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE));

            m_InnerClasses.emplace_back(innerClassInfoIndex,
                                        outerClassInfoIndex,
                                        innerNameIndex,
                                        innerClassAccessFlags);
        }
    }

    u2 InnerClasses::numberOfClasses()
    {
        return m_InnerClasses.size();
    }

    const std::vector<InnerClasses::InnerClass>& InnerClasses::innerClasses() const
    {
        return m_InnerClasses;
    }
} // namespace AeroJet::Java::ClassFile