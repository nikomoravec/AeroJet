#ifndef SUPERJET_FILENOTFOUNDEXCEPTION_H
#define SUPERJET_FILENOTFOUNDEXCEPTION_H

#include "RuntimeException.h"
#include "fmt/format.h"

#include <filesystem>

namespace SuperJet::Compiler
{
    class FileNotFoundException : public RuntimeException
    {
    public:
        FileNotFoundException(const std::filesystem::path& path) : RuntimeException(fmt::format("File \"{}\" not found!", path.string()))
        {
        }
    };
}

#endif //SUPERJET_FILENOTFOUNDEXCEPTION_H
