#include "Exceptions/RuntimeException.hpp"

#include "backward.hpp"

namespace AeroJet::Exceptions
{
    RuntimeException::RuntimeException(std::string_view message) : std::runtime_error(message.data())
    {
        ::backward::StackTrace stackTrace;
        stackTrace.load_here();
        ::backward::Printer stackTracePrinter;
        stackTracePrinter.print(stackTrace);
    }
} // namespace AeroJet::Exceptions