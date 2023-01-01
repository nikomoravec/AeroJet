/*
 * Jar.cpp
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

#include "Java/Archive/Jar.hpp"
#include "Exceptions/FileNotFoundException.hpp"
#include "Exceptions/RuntimeException.hpp"
#include "fmt/format.h"
#include "Types.hpp"
#include <sstream>

namespace AeroJet::Java::Archive
{
    Jar::Entry::Entry(zip_t* zip) : m_zip(zip), m_index(0)
    {
        if (m_zip == nullptr)
        {
            throw Exceptions::RuntimeException("zip can not be null!");
        }
    }

    Jar::Entry::Entry(zip_t* zip, const std::filesystem::path& path) : Entry(zip)
    {
        i4 open = zip_entry_open(m_zip, path.string().c_str());
        if (open != 0)
        {
            throw Exceptions::RuntimeException(fmt::format("Failed to read entry \"{}\"! Error: {}", path.string(), open));
        }

        m_name = zip_entry_name(m_zip);
        m_index = zip_entry_index(m_zip);
    }

    Jar::Entry::Entry(zip_t* zip, ssize_t index) : Entry(zip)
    {
        m_index = index;

        i4 open = zip_entry_openbyindex(zip, m_index);
        if (open != 0)
        {
            throw Exceptions::RuntimeException(fmt::format("Failed to read entry at index \"{}\"! Error: {}", m_index, open));
        }

        m_name = zip_entry_name(zip);
    }

    Jar::Entry::~Entry()
    {
        zip_entry_close(m_zip);
    }

    std::stringstream Jar::Entry::read() const
    {
        const u8 bufferSize = zip_entry_size(m_zip);
        auto* const buffer = static_cast<u1*>(calloc(sizeof(u1), bufferSize));
        zip_entry_noallocread(m_zip, buffer, bufferSize);

        std::stringstream ss;
        std::move(&buffer[0], &buffer[bufferSize], std::ostream_iterator<u1>(ss));

        return ss;
    }

    std::string_view Jar::Entry::name() const
    {
        return m_name;
    }

    ssize_t Jar::Entry::index() const
    {
        return m_index;
    }

    bool Jar::Entry::isDirectory() const
    {
        return zip_entry_isdir(m_zip);
    }

    Jar::Jar(const std::filesystem::path& path) : m_zip(nullptr), m_location(path)
    {
        if (!std::filesystem::exists(path))
        {
            throw Exceptions::FileNotFoundException(path);
        }

        m_zip = zip_open(path.string().c_str(), 0, 'r');
        if (m_zip == nullptr)
        {
            throw Exceptions::RuntimeException(fmt::format("Failed to open JAR archive \"{}\"", path.string()));
        }
    }

    Jar::Jar(const Jar& other)
    {
        this->m_location = other.m_location;
        this->m_zip = zip_open(m_location.string().c_str(), 0, 'r');

        if (m_zip == nullptr)
        {
            throw Exceptions::RuntimeException(fmt::format("Failed to re-open (copy) Jar \"{}\"", m_location.string()));
        }
    }

    Jar::~Jar()
    {
        zip_close(m_zip);
    }

    const std::filesystem::path& Jar::location() const
    {
        return m_location;
    }

    ssize_t Jar::count() const
    {
        return zip_entries_total(m_zip);
    }

    Jar::Entry Jar::open(const std::filesystem::path& path) const
    {
        return {m_zip, path};
    }

    Jar::Entry Jar::open(ssize_t index) const
    {
        return {m_zip, index};
    }
}