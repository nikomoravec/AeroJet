#include "Java/ClassFile/Attributes/Annotation/TypeAnnotation.hpp"

#include <utility>

namespace AeroJet::Java::ClassFile
{
    TypeParameterTarget::TypeParameterTarget(u1 typeParameterIndex) : m_typeParameterIndex(typeParameterIndex) {}

    u1 TypeParameterTarget::typeParameterIndex() const
    {
        return m_typeParameterIndex;
    }

    SuperTypeTarget::SuperTypeTarget(u2 superTypeIndex) : m_superTypeIndex(superTypeIndex) {}

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

    FormalParameterTarget::FormalParameterTarget(u1 formalParameterIndex) : m_formalParameterIndex(formalParameterIndex)
    {
    }

    u1 FormalParameterTarget::formalParameterIndex() const
    {
        return m_formalParameterIndex;
    }

    ThrowsTarget::ThrowsTarget(u2 throwsTypeIndex) : m_throwsTypeIndex(throwsTypeIndex) {}

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

    u2 LocalVarTarget::tableLength() const
    {
        return m_table.size();
    }

    const std::vector<LocalVarTarget::TableEntry>& LocalVarTarget::table() const
    {
        return m_table;
    }

    CatchTarget::CatchTarget(u2 exceptionTableIndex) : m_exceptionTableIndex(exceptionTableIndex) {}

    u2 CatchTarget::exceptionTableIndex() const
    {
        return m_exceptionTableIndex;
    }

    OffsetTarget::OffsetTarget(u2 offset) : m_offset(offset) {}

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

    u1 TypePath::pathLength() const
    {
        return m_path.size();
    }

    const std::vector<TypePath::Path>& TypePath::path() const
    {
        return m_path;
    }

    TypeAnnotation::TypeAnnotation(u1                                   targetType,
                                   TargetInfoType                       targetInfoType,
                                   TypePath                             targetPath,
                                   u2                                   typeIndex,
                                   const std::vector<ElementValuePair>& elementValuePairs) :
        m_targetType(targetType),
        m_targetInfo(std::move(targetInfoType)), m_targetPath(std::move(targetPath)), m_typeIndex(typeIndex),
        m_elementValuePairs(elementValuePairs)
    {
    }
    u1 TypeAnnotation::targetType() const
    {
        return m_targetType;
    }

    const TargetInfoType& TypeAnnotation::targetInfo() const
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