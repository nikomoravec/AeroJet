#ifndef SUPERJET_STRUCTUREPRINTER_H
#define SUPERJET_STRUCTUREPRINTER_H

#include "Compiler/Compiler.h"
#include "Compiler/Printer.h"
#include "Class.h"
#include "fmt/format.h"

namespace SuperJet::Compiler::CodeGen::Cpp
{
    class StructureDeclarationPrinter : public Compiler::Printer<>
    {
    public:
        StructureDeclarationPrinter(const SuperJet::Compiler::CodeGen::Cpp::Class& inClass) : clazz(inClass)
        {
        }

        virtual void print(std::ostream& out) override
        {
            if (clazz.hasNamespace())
            {
                out << fmt::format("namespace {} \n{{\n\t", SuperJet::toString(clazz.getNamespace()));
            }
            out << fmt::format("class {}\n{{\n", clazz.getName());


            if (clazz.hasNamespace())
            {
                out << "\t";
            }

            out << "};\n";
            if (clazz.hasNamespace())
            {
                out << "}\n";
            }
        }

    protected:
        SuperJet::Compiler::CodeGen::Cpp::Class clazz;
    };
}

#endif //SUPERJET_STRUCTUREPRINTER_H
