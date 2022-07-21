#ifndef SUPERJET_FIELDDESCRIPTOR_H
#define SUPERJET_FIELDDESCRIPTOR_H

#include <string>
#include <optional>

namespace SuperJet::Java::Archive
{
    class FieldDescriptor
    {
    public:
        enum class FieldType : char
        {
            BYTE = 'B',
            CHAR = 'C',
            DOUBLE = 'D',
            FLOAT = 'F',
            INTEGER = 'I',
            LONG = 'J',
            CLASS = 'L',
            SHORT = 'S',
            BOOLEAN = 'Z',
            ARRAY = '['
        };

        static constexpr char FIELD_TYPE_CLASS_END_TOKEN = ';';

    public:
        FieldDescriptor(const std::string& inDescriptor);

        std::string getRawLiteral() const
        {
            return rawLiteral;
        }

        std::optional<FieldDescriptor> getUnderlyingType() const;
        std::optional<std::string> getClassName() const;
        FieldType getFieldType() const;
        
    protected:
        FieldType resolve();

    protected:
        std::string rawLiteral;
        FieldType type;
    };
}

#endif //SUPERJET_FIELDDESCRIPTOR_H
