#ifndef SUPERJET_JAR_H
#define SUPERJET_JAR_H

#include "Java/Types.h"
#include "zip.h"

#include <filesystem>
#include <optional>
#include <strstream>
#include <vector>

namespace SuperJet::Java::Archive
{
    class Jar
    {
    public:
        class Entry
        {
        public:
            Entry(zip_t* inZip, const std::filesystem::path& path);
            Entry(zip_t* inZip, ssize_t inIndex);
            ~Entry();

            std::stringstream read() const;
            const std::string& getName() const;
            ssize_t getIndex() const;
            bool isDirectory() const;

        protected:
            Entry(zip_t* inZip);

        protected:
            zip_t* zip;

            std::string name;
            ssize_t index = 0;
        };

    public:
        Jar(const std::filesystem::path& path);
        Jar(const Jar& other);
        ~Jar();

        Jar::Entry openClass(const std::filesystem::path& path) const;

        Jar::Entry open(const std::filesystem::path& path) const;
        Jar::Entry open(ssize_t index) const;

        const std::filesystem::path& getLocation() const;
        ssize_t count() const;

    protected:
        std::filesystem::path location;
        zip_t* zip;
    };
}

#endif //SUPERJET_JAR_H
