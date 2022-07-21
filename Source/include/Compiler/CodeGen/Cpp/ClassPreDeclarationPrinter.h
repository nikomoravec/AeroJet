#ifndef SUPERJET_CLASSPREDECLARATIONPRINTER_H
#define SUPERJET_CLASSPREDECLARATIONPRINTER_H

#include "Compiler/Compiler.h"
#include "Compiler/Printer.h"
#include "Compiler/CodeGen/Cpp/Class.h"


namespace SuperJet::Compiler::CodeGen::Cpp
{
    class ClassPreDeclarationPrinter : public Compiler::Printer<>
    {
    public:
        ClassPreDeclarationPrinter(const SuperJet::Compiler::CodeGen::Cpp::Class& inClass) : clazz(inClass)
        {
        }

        virtual void print(std::ostream& out) override
        {
            if (clazz.hasNamespace())
            {
                out << fmt::format("namespace {} \n{{\n\tclass {};\n}}\n", SuperJet::toString(clazz.getNamespace()), clazz.getName());
            }
            else
            {
                out << fmt::format("class {};\n", clazz.getName());
            }
        }

    protected:
        SuperJet::Compiler::CodeGen::Cpp::Class clazz;
    };
}

#endif //SUPERJET_CLASSPREDECLARATIONPRINTER_H
