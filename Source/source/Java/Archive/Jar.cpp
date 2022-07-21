#include "Java/Archive/Jar.h"
#include "Compiler/Exceptions/FileNotFoundException.h"
#include <sstream>

namespace SuperJet::Java::Archive
{
    Jar::Jar(const std::filesystem::path& path) : zip(nullptr)
    {
        if (!std::filesystem::exists(path))
        {
            throw Compiler::FileNotFoundException(path);
        }

        zip = zip_open(path.c_str(), 0, 'r');
        if (zip == nullptr)
        {
            throw Compiler::RuntimeException(fmt::format("Failed to open JAR archive \"{}\""));
        }
    }

    Jar::~Jar()
    {
        zip_close(zip);
    }

    bool Jar::contains(const std::filesystem::path& path) const
    {
        const int open = zip_entry_open(zip, path.c_str());
        zip_entry_close(zip);

        return open == 0;
    }

    std::stringstream Jar::read(const std::filesystem::path& path) const
    {
        const int open = zip_entry_open(zip, path.c_str());

        if (open == 0)
        {
            const unsigned long long bufferSize = zip_entry_size(zip);
            auto* const buffer = static_cast<unsigned char*>(calloc(sizeof(unsigned char), bufferSize));
            zip_entry_noallocread(zip, buffer, bufferSize);

            std::stringstream ss;
            std::move(&buffer[0], &buffer[bufferSize], std::ostream_iterator<unsigned char>(ss));

            zip_entry_close(zip);

            return ss;
        }

        zip_entry_close(zip);

        throw Compiler::RuntimeException(fmt::format("Failed to read entry \"{}\"! Error: {}", path.string(), open));
    }
}