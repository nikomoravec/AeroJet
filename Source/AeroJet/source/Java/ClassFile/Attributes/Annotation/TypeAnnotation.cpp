/*
 * TypeAnnotations.cpp
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

#include "Java/ClassFile/Attributes/Annotation/TypeAnnotation.hpp"

#include "Exceptions/RuntimeException.hpp"
#include "fmt/format.h"
#include "Stream/StandardStreamWrapper.hpp"

#include <utility>

namespace AeroJet::Java::ClassFile
{
    TypeParameterTarget::TypeParameterTarget(u1 typeParameterIndex) :
        m_typeParameterIndex(typeParameterIndex) {}

    u1 TypeParameterTarget::typeParameterIndex() const
    {
        return m_typeParameterIndex;
    }

    SuperTypeTarget::SuperTypeTarget(u2 superTypeIndex) :
        m_superTypeIndex(superTypeIndex) {}

    u2 SuperTypeTarget::superTypeIndex() const
    {
        return m_superTypeIndex;
    }

    TypeParameterBoundTarget::TypeParameterBoundTarget(u1 typeParameterIndex, u1 boundIndex) :
        m_typeParameterIndex(typeParameterIndex), m_boundIndex(boundIndex)
    {
    }

    u1 TypeParameterBoundTarget::typeParameterIndex() const
    {
        return m_typeParameterIndex;
    }

    u1 TypeParameterBoundTarget::boundIndex() const
    {
        return m_boundIndex;
    }

    FormalParameterTarget::FormalParameterTarget(u1 formalParameterIndex) :
        m_formalParameterIndex(formalParameterIndex)
    {
    }

    u1 FormalParameterTarget::formalParameterIndex() const
    {
        return m_formalParameterIndex;
    }

    ThrowsTarget::ThrowsTarget(u2 throwsTypeIndex) :
        m_throwsTypeIndex(throwsTypeIndex) {}

    u2 ThrowsTarget::throwsTypeIndex() const
    {
        return m_throwsTypeIndex;
    }

    LocalVarTarget::TableEntry::TableEntry(u2 startPc, u2 length, u2 index) :
        m_startPc(startPc), m_length(length), m_index(index)
    {
    }

    u2 LocalVarTarget::TableEntry::startPc() const
    {
        return m_startPc;
    }

    u2 LocalVarTarget::TableEntry::length() const
    {
        return m_length;
    }

    u2 LocalVarTarget::TableEntry::index() const
    {
        return m_index;
    }

    LocalVarTarget::LocalVarTarget(std::vector<TableEntry> table) :
        m_table(std::move(table)) {}

    u2 LocalVarTarget::tableLength() const
    {
        return m_table.size();
    }

    const std::vector<LocalVarTarget::TableEntry>& LocalVarTarget::table() const
    {
        return m_table;
    }

    CatchTarget::CatchTarget(u2 exceptionTableIndex) :
        m_exceptionTableIndex(exceptionTableIndex) {}

    u2 CatchTarget::exceptionTableIndex() const
    {
        return m_exceptionTableIndex;
    }

    OffsetTarget::OffsetTarget(u2 offset) :
        m_offset(offset) {}

    u2 OffsetTarget::offset() const
    {
        return m_offset;
    }

    TypeArgumentTarget::TypeArgumentTarget(u2 offset, u1 typeArgumentIndex) :
        m_offset(offset), m_typeArgumentIndex(typeArgumentIndex)
    {
    }

    u2 TypeArgumentTarget::offset() const
    {
        return m_offset;
    }

    u1 TypeArgumentTarget::typeArgumentIndex() const
    {
        return m_typeArgumentIndex;
    }

    TypePath::Path::Path(TypePath::TypePathKind typePathKind, u1 typeArgumentIndex) :
        m_typePathKind(typePathKind), m_typeArgumentIndex(typeArgumentIndex)
    {
    }

    TypePath::TypePathKind TypePath::Path::typePathKind() const
    {
        return m_typePathKind;
    }

    u1 TypePath::Path::typeArgumentIndex() const
    {
        return m_typeArgumentIndex;
    }

    TypePath::TypePath(std::vector<Path> path) :
        m_path(std::move(path)) {}

    u1 TypePath::pathLength() const
    {
        return m_path.size();
    }

    const std::vector<TypePath::Path>& TypePath::path() const
    {
        return m_path;
    }

    TypeAnnotation::TypeAnnotation(u1 targetType,
                                   TargetInfo targetInfo,
                                   TypePath targetPath,
                                   u2 typeIndex,
                                   const std::vector<ElementValuePair>& elementValuePairs) :
        m_targetType(targetType),
        m_targetInfo(std::move(targetInfo)), m_targetPath(std::move(targetPath)), m_typeIndex(typeIndex),
        m_elementValuePairs(elementValuePairs)
    {
    }
    u1 TypeAnnotation::targetType() const
    {
        return m_targetType;
    }

    const TargetInfo& TypeAnnotation::targetInfo() const
    {
        return m_targetInfo;
    }

    const TypePath& TypeAnnotation::targetPath() const
    {
        return m_targetPath;
    }

    u2 TypeAnnotation::typeIndex() const
    {
        return m_typeIndex;
    }

    u2 TypeAnnotation::numElementValuePairs() const
    {
        return m_elementValuePairs.size();
    }

    const std::vector<ElementValuePair>& TypeAnnotation::elementValuePairs() const
    {
        return m_elementValuePairs;
    }
} // namespace AeroJet::Java::ClassFile

template<>
AeroJet::Java::ClassFile::TypeAnnotation AeroJet::Stream::Reader::read(std::istream& stream, ByteOrder byteOrder)
{
    const u1 targetType = AeroJet::Stream::Reader::read<u1>(stream, byteOrder);

    Java::ClassFile::TargetInfo targetInfo{ Java::ClassFile::EmptyTarget{} };
    switch(targetType)
    {
        case 0x00: // type parameter declaration of generic class or interface
        case 0x01: // type parameter declaration of generic method or constructor
        {
            targetInfo =
                AeroJet::Stream::Reader::read<AeroJet::Java::ClassFile::TypeParameterTarget>(stream, byteOrder);
            break;
        }
        case 0x10: // type in extends or implements clause of class declaration
        {
            targetInfo = AeroJet::Stream::Reader::read<AeroJet::Java::ClassFile::SuperTypeTarget>(stream, byteOrder);
            break;
        }
        case 0x11: // type in bound of type parameter declaration of generic class or interface
        case 0x12: // type in bound of type parameter declaration of generic method or constructor
        {
            targetInfo =
                AeroJet::Stream::Reader::read<AeroJet::Java::ClassFile::TypeParameterBoundTarget>(stream, byteOrder);
            break;
        }
        case 0x13: // type in field declaration
        case 0x14: // return type of method, or type of newly constructed object
        case 0x15: // receiver type of method or constructor
        {
            targetInfo = Java::ClassFile::EmptyTarget{};
            break;
        }
        case 0x16: // type in formal parameter declaration of method, constructor, or lambda expression
        {
            targetInfo =
                AeroJet::Stream::Reader::read<AeroJet::Java::ClassFile::FormalParameterTarget>(stream, byteOrder);
            break;
        }
        case 0x17: // type in throws clause of method or constructor
        {
            targetInfo = AeroJet::Stream::Reader::read<AeroJet::Java::ClassFile::ThrowsTarget>(stream, byteOrder);
            break;
        }
        case 0x40:
        case 0x41:
        {
            targetInfo = AeroJet::Stream::Reader::read<AeroJet::Java::ClassFile::LocalVarTarget>(stream, byteOrder);
            break;
        }
        case 0x42:
        {
            targetInfo = AeroJet::Stream::Reader::read<AeroJet::Java::ClassFile::CatchTarget>(stream, byteOrder);
            break;
        }
        case 0x43:
        case 0x44:
        case 0x45:
        case 0x46:
        {
            targetInfo = AeroJet::Stream::Reader::read<AeroJet::Java::ClassFile::OffsetTarget>(stream, byteOrder);
            break;
        }
        case 0x47:
        case 0x48:
        case 0x49:
        case 0x4A:
        case 0x4B:
        {
            targetInfo = AeroJet::Stream::Reader::read<AeroJet::Java::ClassFile::TypeArgumentTarget>(stream, byteOrder);
            break;
        }
        default:
            throw AeroJet::Exceptions::RuntimeException("Unknown targetType value {:#04x}!");
    }

    const AeroJet::Java::ClassFile::TypePath targetPath =
        AeroJet::Stream::Reader::read<AeroJet::Java::ClassFile::TypePath>(stream, byteOrder);

    const u2 typeIndex = AeroJet::Stream::Reader::read<u2>(stream, byteOrder);

    const u2 numElementValuePairs = AeroJet::Stream::Reader::read<u2>(stream, byteOrder);

    std::vector<AeroJet::Java::ClassFile::ElementValuePair> elementValuePairs;
    elementValuePairs.reserve(numElementValuePairs);
    for(u2 elementValuePairIndex = 0; elementValuePairIndex < numElementValuePairs; elementValuePairIndex++)
    {
        const AeroJet::Java::ClassFile::ElementValuePair elementValuePair =
            AeroJet::Stream::Reader::read<AeroJet::Java::ClassFile::ElementValuePair>(stream, byteOrder);
        elementValuePairs.emplace_back(elementValuePair);
    }

    return Java::ClassFile::TypeAnnotation{ targetType, targetInfo, targetPath, typeIndex, elementValuePairs };
}
