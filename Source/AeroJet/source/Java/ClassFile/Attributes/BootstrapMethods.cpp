/*
 * BootstrapMethods.hpp
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

#include "Java/ClassFile/Attributes/BootstrapMethods.hpp"

#include "Stream/Reader.hpp"

namespace AeroJet::Java::ClassFile
{

    BootstrapMethods::BootstrapMethodsEntry::BootstrapMethodsEntry(u2                     bootstrapMethodRef,
                                                                   const std::vector<u2>& bootstrapArguments) :
        m_bootstrapMethodRef(bootstrapMethodRef),
        m_bootstrapArguments(bootstrapArguments)
    {
    }

    u2 BootstrapMethods::BootstrapMethodsEntry::bootstrapMethodRef() const
    {
        return m_bootstrapMethodRef;
    }

    u2 BootstrapMethods::BootstrapMethodsEntry::numBootstrapArguments() const
    {
        return m_bootstrapArguments.size();
    }

    const std::vector<u2>& BootstrapMethods::BootstrapMethodsEntry::bootstrapArguments() const
    {
        return m_bootstrapArguments;
    }

    BootstrapMethods::BootstrapMethods(const ConstantPool& constantPool, const AttributeInfo& attributeInfo) :
        Attribute(constantPool, attributeInfo, BOOTSTRAP_METHODS_ATTRIBUTE_NAME)
    {
        const u2 numBootstrapMethods = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);
        m_bootstrapMethods.reserve(numBootstrapMethods);
        for(u2 bootstrapMethodIndex = 0; bootstrapMethodIndex < numBootstrapMethods; bootstrapMethodIndex++)
        {
            const u2 bootstrapMethodRef = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);

            const u2 numBootstrapArguments = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);
            std::vector<u2> boostrapArguments {};
            boostrapArguments.reserve(numBootstrapArguments);
            for(u2 bootstrapArgumentIndex = 0; bootstrapArgumentIndex < numBootstrapArguments; bootstrapArgumentIndex++)
            {
                const u2 bootstrapArgument = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);
                boostrapArguments.emplace_back(bootstrapArgument);
            }

            m_bootstrapMethods.emplace_back(bootstrapMethodRef, boostrapArguments);
        }
    }

    u2 BootstrapMethods::numBootstrapMethods() const
    {
        return m_bootstrapMethods.size();
    }

    const std::vector<BootstrapMethods::BootstrapMethodsEntry>& BootstrapMethods::bootstrapMethods() const
    {
        return m_bootstrapMethods;
    }
} // namespace AeroJet::Java::ClassFile
