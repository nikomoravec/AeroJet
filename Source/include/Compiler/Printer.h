#ifndef SUPERJET_PRINTER_H
#define SUPERJET_PRINTER_H

#include <ostream>

namespace SuperJet::Compiler
{
    template<typename T = char>
    class Printer
    {
    public:
        virtual ~Printer() = default;
        virtual void print(std::basic_ostream<T>& out) = 0;
    };
}

#endif //SUPERJET_PRINTER_H
