#include "Java/Archive/Jar.h"
#include "Compiler/Exceptions/FileNotFoundException.h"
#include <sstream>
#include "spdlog/spdlog.h"

namespace SuperJet::Java::Archive
{
    Jar::Entry::Entry(zip_t* inZip) : zip(inZip)
    {
        if (zip == nullptr)
        {
            throw Compiler::RuntimeException("zip can not be null!");
        }
    }

    Jar::Entry::Entry(zip_t* inZip, const std::filesystem::path& path) : Entry(inZip)
    {
        const int open = zip_entry_open(zip, path.c_str());
        if (open != 0)
        {
            throw Compiler::RuntimeException(fmt::format("Failed to read entry \"{}\"! Error: {}", path.string(), open));
        }

        name = zip_entry_name(zip);
        index = zip_entry_index(zip);
    }

    Jar::Entry::Entry(zip_t* inZip, ssize_t inIndex) : Entry(inZip)
    {
        index = inIndex;

        const int open = zip_entry_openbyindex(zip, index);
        if (open != 0)
        {
            throw Compiler::RuntimeException(fmt::format("Failed to read entry at index \"{}\"! Error: {}", inIndex, open));
        }

        name = zip_entry_name(zip);
    }

    Jar::Entry::~Entry()
    {
        zip_entry_close(zip);
    }

    std::stringstream Jar::Entry::read() const
    {
        const unsigned long long bufferSize = zip_entry_size(zip);
        auto* const buffer = static_cast<unsigned char*>(calloc(sizeof(unsigned char), bufferSize));
        zip_entry_noallocread(zip, buffer, bufferSize);

        std::stringstream ss;
        std::move(&buffer[0], &buffer[bufferSize], std::ostream_iterator<unsigned char>(ss));

        return ss;
    }

    const std::string& Jar::Entry::getName() const
    {
        return name;
    }

    ssize_t Jar::Entry::getIndex() const
    {
        return index;
    }

    bool Jar::Entry::isDirectory() const
    {
        return zip_entry_isdir(zip);
    }

    Jar::Jar(const std::filesystem::path& path) : zip(nullptr), location(path)
    {
        if (!std::filesystem::exists(path))
        {
            throw Compiler::FileNotFoundException(path);
        }

        zip = zip_open(path.c_str(), 0, 'r');
        if (zip == nullptr)
        {
            throw Compiler::RuntimeException(fmt::format("Failed to open JAR archive \"{}\"", path.string()));
        }
    }

    Jar::Jar(const Jar& other)
    {
        this->location = other.location;
        this->zip = zip_open(location.c_str(), 0, 'r');

        if (zip == nullptr)
        {
            throw Compiler::RuntimeException(fmt::format("Failed to re-open (copy) Jar \"{}\"", location.string()));
        }
    }

    Jar::~Jar()
    {
        zip_close(zip);
        zip = nullptr;
    }

    const std::filesystem::path& Jar::getLocation() const
    {
        return location;
    }

    ssize_t Jar::count() const
    {
        return zip_entries_total(zip);
    }

    Jar::Entry Jar::open(const std::filesystem::path& path) const
    {
        return {zip, path};
    }

    Jar::Entry Jar::open(ssize_t index) const
    {
        return {zip, index};
    }
}