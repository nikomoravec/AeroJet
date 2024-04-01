#include "ClassInfoNode.hpp"

namespace AeroJet::Tree
{
    ClassInfoNode::ClassInfoNode(Java::ClassFile::ClassInfo classInfo) :
        Node(Type::CLASS_INFO),
        m_classInfo(std::move(classInfo))
    {
    }

    const Java::ClassFile::ClassInfo& ClassInfoNode::classInfo() const
    {
        return m_classInfo;
    }

} // namespace AeroJet::Tree