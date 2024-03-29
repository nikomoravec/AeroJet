/*
 * Environment.hpp
 *
 * Copyright © 2024 AeroJet Developers. All Rights Reserved.
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

#include "ClassPath.hpp"

#include <filesystem>
#include <set>

namespace AeroJet::Compiler
{
    class MainClassStorage
    {
      public:
        enum class Kind : AeroJet::u1
        {
            CLASS_FILE,
            ARCHIVE
        };

      public:
        MainClassStorage();
        MainClassStorage(Kind kind, std::filesystem::path path);

        [[nodiscard]] Kind kind() const;
        [[nodiscard]] const std::filesystem::path& path() const;

      private:
      private:
        Kind m_kind;
        std::filesystem::path m_path;
    };

    class Environment
    {
      public:
        class Builder
        {
            friend Environment;

          public:
            Builder();

            Builder& classPath(const ClassPath& classPath);
            Builder& mainClass(const MainClassStorage& mainClass);
            Builder& output(const std::string& mainClass);
            Environment build();

          private:
            ClassPath m_classPath;
            MainClassStorage m_mainClass;
            std::string m_output;
        };

      public:
        [[nodiscard]] const ClassPath& classPath() const;
        [[nodiscard]] const MainClassStorage& mainClass() const;
        [[nodiscard]] const std::string& output() const;

      protected:
        explicit Environment(const Builder& builder);

      protected:
        ClassPath m_classPath;
        MainClassStorage m_mainClass;
        std::string m_output;
    };
} // namespace AeroJet::Compiler
