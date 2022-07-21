#include "Java/Archive/FieldDescriptor.h"
#include "fmt/format.h"
#include "Compiler/Exceptions/RuntimeException.h"

namespace SuperJet::Java::Archive
{
    FieldDescriptor::FieldDescriptor(const std::string& inDescriptor) : rawLiteral(inDescriptor), type(resolve())
    {
        if (rawLiteral.empty())
        {
            throw SuperJet::Compiler::RuntimeException("Field descriptor could not be empty!");
        }
    }

    std::optional<FieldDescriptor> FieldDescriptor::getUnderlyingType() const
    {
        if (type == FieldType::ARRAY)
        {
            return FieldDescriptor(std::string(++rawLiteral.begin(), rawLiteral.end()));
        }

        return std::nullopt;
    }

    std::optional<std::string> FieldDescriptor::getClassName() const
    {
        if (type == FieldType::CLASS)
        {
            return std::string(++rawLiteral.begin(), --rawLiteral.end());
        }

        return std::nullopt;
    }

    FieldDescriptor::FieldType getFieldType() const
    {
        return type;
    }

    FieldDescriptor::FieldType FieldDescriptor::resolve()
    {
        const char first = *rawLiteral.begin();
        if (rawLiteral.size() > 1)
        {
            if (first == static_cast<char>(FieldType::CLASS))
            {
                if (rawLiteral[rawLiteral.size() - 1] == FIELD_TYPE_CLASS_END_TOKEN)
                {
                    return FieldType::CLASS;
                }
            }

            if (first == static_cast<char>(FieldType::ARRAY))
            {
                return FieldType::ARRAY;
            }
        }

        if (rawLiteral.size() != 1)
        {
            throw SuperJet::Compiler::RuntimeException(fmt::format("Incorrect Field Descriptor literal: \"{}\"", rawLiteral));
        }

        switch(first)
        {
            case static_cast<char>(FieldType::BYTE):
                return FieldType::BYTE;
            case static_cast<char>(FieldType::CHAR):
                return FieldType::CHAR;
            case static_cast<char>(FieldType::DOUBLE):
                return FieldType::DOUBLE;
            case static_cast<char>(FieldType::FLOAT):
                return FieldType::FLOAT;
            case static_cast<char>(FieldType::INTEGER):
                return FieldType::INTEGER;
            case static_cast<char>(FieldType::LONG):
                return FieldType::LONG;
            case static_cast<char>(FieldType::SHORT):
                return FieldType::SHORT;
            case static_cast<char>(FieldType::BOOLEAN):
                return FieldType::BOOLEAN;
            default:
                throw SuperJet::Compiler::RuntimeException(fmt::format("Can not resolve type of field descriptor: \"{}\"", rawLiteral));
        }
    }
}