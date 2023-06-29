/*
 * ClassInfoUtils.hpp
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

#include "Java/ClassFile/ClassInfo.hpp"

#include <filesystem>
#include <string>

namespace AeroJet::Java::ClassFile::Utils
{
    static constexpr char CLASS_PACKAGE_DELIMITER = '/';
    static constexpr char JAVA_PACKAGE_DELIMITER = '.';

    class ClassInfoUtils
    {
      public:
        /**
         * @brief Checks if class under given ClassInfo is under java package
         * @return true if class is under package and false if it's not
         */
        [[nodiscard]] static bool isUnderPackage(const ClassInfo& classInfo);

        /**
         * @brief Extracts full name of class from ClassInfo
         * @param classInfo
         * @return full name of given ClassInfo without a package
         */
        [[nodiscard]] static std::string name(const ClassInfo& classInfo);

        /**
         * @brief Extracts short name of class from ClassInfo
         * @param classInfo
         * @return short name of given ClassInfo including package
         */
        [[nodiscard]] static std::string className(const ClassInfo& classInfo);

        [[nodiscard]] static std::string javaNameFromPath(const std::filesystem::path& path);

        /**
         * @brief Extracts short name of class from ClassInfo
         * @param classInfo
         * @return full name of given ClassInfo including package in Java format like org.project.ClassName
         */
        [[nodiscard]] static std::string javaName(const ClassInfo& classInfo);
    };
} // namespace AeroJet::Java::ClassFile::Utils