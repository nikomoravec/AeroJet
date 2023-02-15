#include "Java/ClassFile/Attributes/Annotation/ElementValuePair.hpp"

namespace AeroJet::Java::ClassFile
{
    ElementValuePair::ElementValuePair(u2 elementNameIndex, const ElementValue& value) :
        m_elementNameIndex(elementNameIndex), m_value(value)
    {
    }

    u2 ElementValuePair::elementNameIndex() const
    {
        return m_elementNameIndex;
    }

    const ElementValue& ElementValuePair::value() const
    {
        return m_value;
    }
}