/*
 * ClassRegistry.cpp
 *
 * Copyright © 2023 AeroJet Developers. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "ClassRegistry.hpp"

#include "Exceptions/CompilerException.hpp"
#include "fmt/format.h"
#include "ErrorCodes.hpp"

#include <fstream>
#include <utility>

namespace AeroJet::Compiler
{
    ClassRegistry::ClassRegistry(AeroJet::Compiler::ClassPath classPath) : m_classPath(std::move(classPath))
    {
    }

    const AeroJet::Java::ClassFile::ClassInfo& ClassRegistry::get(const std::filesystem::path& path)
    {
        if (contains(path))
        {
            return at(path);
        }

        for (const ClassPath::Entry& entry : m_classPath.entries())
        {
            const ClassPath::EntryType entryType = entry.type();
            switch(entryType)
            {
                case ClassPath::EntryType::FOLDER:
                {
                    const std::filesystem::path classFilePath = path.string() + ".class";
                    const std::filesystem::path classFileFullPath = entry.path() / classFilePath;

                    if (std::filesystem::exists(classFileFullPath))
                    {
                        std::fstream fileStream{classFileFullPath};
                        if (fileStream.is_open())
                        {
                            Java::ClassFile::ClassInfo classInfo = Stream::Reader::read<Java::ClassFile::ClassInfo>(fileStream, Stream::ByteOrder::INVERSE);
                            insert({path, std::move(classInfo)});

                            return at(path);
                        }

                        throw Exceptions::CompilerException{fmt::format("Failed to open class file '{}'!", classFileFullPath.string()), ErrorCodes::EXIT_CODE_IO_ERROR};
                    }
                }
                case ClassPath::EntryType::ARCHIVE:
                {
                    Java::Archive::Jar jar{entry.path()};
                    for (ssize_t entryIndex = 0; entryIndex < jar.count(); entryIndex++)
                    {
                        Java::Archive::Jar::Entry jarEntry = jar.open(entryIndex);
                        if (!jarEntry.isDirectory() && jarEntry.name() == path.string() + ".class")
                        {
                            Stream::MemoryStream entryStream = jarEntry.read();
                            Java::ClassFile::ClassInfo classInfo = Stream::Reader::read<Java::ClassFile::ClassInfo>(entryStream, Stream::ByteOrder::INVERSE);
                            insert({path, std::move(classInfo)});

                            return at(path);
                        }
                    }
                }
            }
        }

        throw Exceptions::CompilerException{fmt::format("Can not find requested class '{}' in any of class path entries!", path.string()), ErrorCodes::EXIT_CODE_CLASS_NOT_FOUND};
    }
} // namespace AeroJet::Compiler