#include "Java/Archive/MethodDescriptor.h"
#include "Java/Archive/FieldDescriptor.h"
#include "fmt/format.h"

namespace SuperJet::Java::Archive
{
    static bool isPrimitive(char fieldType)
    {
        switch (fieldType)
        {
            case static_cast<char>(FieldDescriptor::FieldType::BYTE):
            case static_cast<char>(FieldDescriptor::FieldType::CHAR):
            case static_cast<char>(FieldDescriptor::FieldType::DOUBLE):
            case static_cast<char>(FieldDescriptor::FieldType::FLOAT):
            case static_cast<char>(FieldDescriptor::FieldType::INTEGER):
            case static_cast<char>(FieldDescriptor::FieldType::LONG):
            case static_cast<char>(FieldDescriptor::FieldType::SHORT):
            case static_cast<char>(FieldDescriptor::FieldType::BOOLEAN):
                return true;
            case static_cast<char>(FieldDescriptor::FieldType::CLASS):
            case static_cast<char>(FieldDescriptor::FieldType::ARRAY):
            default:
                return false;
        }
    }

    static size_t resolveArray(const std::string& literal, std::vector<FieldDescriptor>& arguments, size_t position)
    {
        const size_t underlyingTypeIndex = literal.find_first_not_of(static_cast<char>(FieldDescriptor::FieldType::ARRAY), position);
        if (isPrimitive(literal[underlyingTypeIndex]))
        {
            arguments.emplace_back(FieldDescriptor(literal.substr(position, underlyingTypeIndex - position + 1)));
            return underlyingTypeIndex;
        }

        if (literal[underlyingTypeIndex] == static_cast<char>(FieldDescriptor::FieldType::CLASS))
        {
            const size_t classNameEndPos = literal.find(FieldDescriptor::FIELD_TYPE_CLASS_END_TOKEN, underlyingTypeIndex);
            arguments.emplace_back(FieldDescriptor(literal.substr(position, classNameEndPos - (position - 1))));
            return classNameEndPos;
        }

        throw std::runtime_error(fmt::format("Unexpected token at position {} in \"{}\"", underlyingTypeIndex, literal));
    }

    static size_t resolvePrimitive(const std::string& literal, std::vector<FieldDescriptor>& arguments, size_t position)
    {
        arguments.emplace_back(FieldDescriptor({literal[position]}));
        return position;
    }

    static size_t resolveClass(const std::string& literal, std::vector<FieldDescriptor>& arguments, size_t position)
    {
        const size_t classNameEndPos = literal.find(FieldDescriptor::FIELD_TYPE_CLASS_END_TOKEN, position);
        if (classNameEndPos != std::string::npos)
        {
            arguments.emplace_back(FieldDescriptor(literal.substr(position, classNameEndPos - (position - 1))));
            return static_cast<int32_t>(classNameEndPos);
        }

        throw std::runtime_error(fmt::format("Can not resolve method descriptor: \"{}\"", literal));
    }

    MethodDescriptor::MethodDescriptor(const std::string& inDescriptor) : rawLiteral(inDescriptor)
    {
        if (*rawLiteral.begin() == METHOD_DESCRIPTOR_ARGS_BEGIN_TOKEN)
        {
            size_t argsEndTokenPosition = rawLiteral.find(METHOD_DESCRIPTOR_ARGS_END_TOKEN);
            if (argsEndTokenPosition != std::string::npos)
            {
                const std::string returnTypeLiteral = rawLiteral.substr(argsEndTokenPosition + 1, rawLiteral.size() - argsEndTokenPosition);
                if (returnTypeLiteral.size() == 1)
                {
                    if (returnTypeLiteral[0] == static_cast<char>(Type::VOID))
                    {
                        returnType = std::nullopt;
                    }
                }
                else
                {
                    returnType = FieldDescriptor({returnTypeLiteral});
                }


                std::string argsLiteral = rawLiteral.substr(1, argsEndTokenPosition - 1);

                for (size_t charIndex = 0; charIndex < argsLiteral.size(); charIndex++)
                {
                    const char c = argsLiteral[charIndex];
                    if (isPrimitive(c))
                    {
                        charIndex = resolvePrimitive(argsLiteral, arguments, charIndex);
                    }
                    else
                    {
                        if (c == static_cast<char>(FieldDescriptor::FieldType::CLASS))
                        {
                            charIndex = resolveClass(argsLiteral, arguments, charIndex);
                            continue;
                        }

                        if (c == static_cast<char>(FieldDescriptor::FieldType::ARRAY))
                        {
                            charIndex = resolveArray(argsLiteral, arguments, charIndex);
                            continue;
                        }
                    }
                }
            }
            else
            {
                throw std::runtime_error(fmt::format("Can not resolve method descriptor: \"{}\"", rawLiteral));
            }
        }
        else
        {
            throw std::runtime_error(fmt::format("Can not resolve method descriptor: \"{}\"", rawLiteral));
        }
    }
}