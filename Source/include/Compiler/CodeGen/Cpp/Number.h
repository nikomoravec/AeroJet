#ifndef SUPERJET_CODEGEN_NUMBER_H
#define SUPERJET_CODEGEN_NUMBER_H

#include "Compiler/CodeGen/Node.h"
#include "fmt/format.h"

#include <variant>

namespace SuperJet::Compiler::CodeGen::Cpp
{
    class Number : public SuperJet::Compiler::CodeGen::Node
    {
    public:
        Number(int8_t val) : value({val})
        {
        }

        Number(int16_t val) : value({val})
        {
        }

        Number(int32_t val) : value({val})
        {
        }

        Number(int64_t val) : value({val})
        {
        }

        Number(float val) : value({val})
        {
        }

        Number(double val) : value({val})
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
                    outputStream << fmt::format("{:f}f", std::get<float>(value));
                    break;
                case 5:
                    outputStream << fmt::format("{:f}", std::get<double>(value));
                    break;
            }
        }

    protected:
        std::variant<int8_t, int16_t, int32_t, int64_t, float, double> value;
    };
}

#endif // SUPERJET_CODEGEN_NUMBER_H