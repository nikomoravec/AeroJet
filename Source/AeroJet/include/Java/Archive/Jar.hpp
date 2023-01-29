/*
 * Jar.hpp
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

#pragma once

#include "Stream/Stream.hpp"
#include "zip.h"

#include <filesystem>
#include <string_view>

namespace AeroJet::Java::Archive
{
    class Jar
    {
      public:
        class Entry
        {
          public:
            Entry(zip_t* zip, const std::filesystem::path& path);
            Entry(zip_t* zip, ssize_t index);
            ~Entry();

            [[nodiscard]] Stream::MemoryStream read() const;

            [[nodiscard]] std::string_view name() const;

            [[nodiscard]] ssize_t index() const;

            [[nodiscard]] bool isDirectory() const;

          protected:
            explicit Entry(zip_t* inZip);

          protected:
            zip_t*      m_zip;
            std::string m_name;
            ssize_t     m_index;
        };

      public:
        explicit Jar(const std::filesystem::path& path);
        Jar(const Jar& other);
        ~Jar();

        [[nodiscard]] Jar::Entry open(const std::filesystem::path& path) const;

        [[nodiscard]] Jar::Entry open(ssize_t index) const;

        [[nodiscard]] const std::filesystem::path& location() const;

        [[nodiscard]] ssize_t count() const;

      protected:
        std::filesystem::path m_location;
        zip_t*                m_zip;
    };
} // namespace AeroJet::Java::Archive