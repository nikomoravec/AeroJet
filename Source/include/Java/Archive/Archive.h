#ifndef SUPERJET_ARCHIVE_H
#define SUPERJET_ARCHIVE_H

#include <fstream>
#include <filesystem>
#include <string>
#include <stdexcept>

namespace SuperJet::Java::Archive
{
    template<typename T>
    T read(const std::basic_istream<char>& stream);

    template<typename T>
    T read(const std::filesystem::path& classFilePath)
    {
        if (std::filesystem::exists(classFilePath))
        {
            return read<T>(std::fstream(classFilePath));
        }

        /*
         * TODO: replace it with dedicated exception type FileNotFoundException
         * @Author: Nikita Miroshnichenko (nikita.miroshnichenko@yahoo.com)
         */
        throw std::runtime_error("File " + classFilePath.string() + " does not exist!");
    }
}

#endif //SUPERJET_ARCHIVE_H
