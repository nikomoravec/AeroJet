#ifndef SUPERJET_JAR_H
#define SUPERJET_JAR_H

#include "Java/Types.h"
#include "zip.h"

#include <filesystem>
#include <strstream>

namespace SuperJet::Java::Archive
{
    class Jar
    {
    public:
        Jar(const std::filesystem::path& path);
        ~Jar();

        bool contains(const std::filesystem::path& path) const;
        std::stringstream read(const std::filesystem::path& path) const;

    protected:
        zip_t* zip;
    };
}

#endif //SUPERJET_JAR_H
