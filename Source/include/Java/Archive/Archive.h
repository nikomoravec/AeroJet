#ifndef SUPERJET_ARCHIVE_H
#define SUPERJET_ARCHIVE_H

#include <fstream>
#include <filesystem>
#include <string>
#include <stdexcept>
#include "Java/Types.h"

namespace SuperJet::Java::Archive
{
    static constexpr auto CLASS_FILE_EXTENSION = ".class";

    template<typename T>
    T read(std::istream& stream);

    template<typename T>
    T read(std::istream& stream);

    template<typename T>
    T read(std::istream& stream, JVM::u4 firstInstructionOffset, JVM::u4 firstInstructionLocalOffset);

    template<typename T>
    T read(const std::filesystem::path& classFilePath)
    {
        if (std::filesystem::exists(classFilePath))
        {
            auto stream = std::fstream(classFilePath);
            return read<T>(stream);
        }

        /*
         * TODO: replace it with dedicated exception type FileNotFoundException
         * @Author: Nikita Miroshnichenko (nikita.miroshnichenko@yahoo.com)
         */
        throw std::runtime_error("File " + classFilePath.string() + " does not exist!");
    }
}

#endif //SUPERJET_ARCHIVE_H
