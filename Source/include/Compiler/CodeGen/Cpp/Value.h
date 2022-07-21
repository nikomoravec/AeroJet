#ifndef SUPERJET_VALUE_TYPE_H
#define SUPERJET_VALUE_TYPE_H

#include "Compiler/CodeGen/Node.h"
#include "fmt/format.h"

#include <variant>

namespace SuperJet::Compiler::CodeGen::Cpp
{
    class Value : public SuperJet::Compiler::CodeGen::Node
    {
    protected:
        Value(Type inType) : type(inType)
        {
        }

        Type type;
    };

    class NumericValue : public Value
    {
    public:
        NumericValue(int8_t val) : value({val})
        {
        }

        NumericValue(int16_t val) : value({val})
        {
        }

        NumericValue(int32_t val) : value({val})
        {
        }

        NumericValue(int64_t val) : value({val})
        {
        }

        NumericValue(float val) : value({val})
        {
        }

        NumericValue(double val) : value({val})
        {
        }

        virtual void dump(std::ostream& outputStream) override
        {
            switch (value.index())
            {
                case 0:
                    outputStream << fmt::format("{}", std::get<int8_t>(value));
                    break;
                case 1:
                    outputStream << fmt::format("{}", std::get<int16_t>(value));
                    break;
                case 2:
                    outputStream << fmt::format("{}", std::get<int32_t>(value));
                    break;
                case 3:
                    outputStream << fmt::format("{}", std::get<int64_t>(value));
                    break;
                case 4:
                    outputStream << fmt::format("{}f", std::get<float>(value));
                    break;
                case 5:
                    outputStream << fmt::format("{}d", std::get<double>(value));
                    break;
            }
        }

    protected:
        std::variant<int8_t, int16_t, int32_t, int64_t, float, double> value;
    };

    class NullValue : public Value
    {
    public:
        virtual void dump(std::ostream& outputStream) override
        {
            outputStream << "nullptr";
        }
    };
}

#endif // SUPERJET_VALUE_TYPE_H