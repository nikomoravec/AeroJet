/*
 * ClassPath.cpp
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

#include "ClassPath.hpp"

#include <utility>

namespace AeroJet::Compiler
{
    ClassPath::Entry::Entry(AeroJet::Compiler::ClassPath::EntryType type, std::filesystem::path path) :
        m_type(type), m_path(std::move(path))
    {
    }

    ClassPath::EntryType ClassPath::Entry::type() const
    {
        return m_type;
    }

    const std::filesystem::path& ClassPath::Entry::path() const
    {
        return m_path;
    }

    ClassPath::ClassPath() : m_entries()
    {
    }

    ClassPath::ClassPath(std::string classPath)
    {
        AeroJet::Utils::StringUtils::trim(classPath);
        std::vector<std::string> classPathVector = AeroJet::Utils::StringUtils::split(classPath, ':');

        m_entries.reserve(classPathVector.size());
        for (const std::string& cp : classPathVector)
        {
            std::filesystem::path path{cp};

            if (std::filesystem::exists(path))
            {
                if (std::filesystem::is_directory(path))
                {
                    m_entries.emplace_back(EntryType::FOLDER, path);
                }
                else if (path.extension() == ".jar")
                {
                    m_entries.emplace_back(EntryType::ARCHIVE, path);
                }
            }
        }
    }

    const std::vector<ClassPath::Entry>& ClassPath::entries() const
    {
        return m_entries;
    }
} // namespace AeroJet::Compiler
