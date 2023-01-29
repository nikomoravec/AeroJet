#include "Exceptions/IncorrectAttributeTypeException.hpp"

#include "fmt/format.h"

namespace AeroJet::Exceptions
{
    IncorrectAttributeTypeException::IncorrectAttributeTypeException(std::string_view expectedType,
                                                                     std::string_view givenType) :
        RuntimeException(fmt::format("Incorrect attribute type! Expected: '{}' but got '{}'", expectedType, givenType))
    {
    }
} // namespace AeroJet::Exceptions