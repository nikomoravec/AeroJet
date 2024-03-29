/*
 * Environment.cpp
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

#include "Environment.hpp"

namespace AeroJet::Compiler
{
    Environment::Builder::Builder() :
        m_classPath() {}

    Environment::Builder& Environment::Builder::classPath(const AeroJet::Compiler::ClassPath& classPath)
    {
        m_classPath = classPath;
        return *this;
    }

    Environment Environment::Builder::build()
    {
        return Environment{ *this };
    }

    Environment::Builder& Environment::Builder::mainClass(const MainClassStorage& mainClass)
    {
        m_mainClass = mainClass;
        return *this;
    }

    Environment::Builder& Environment::Builder::output(const std::string& output)
    {
        m_output = output;
        return *this;
    }

    Environment::Environment(const Environment::Builder& builder)
    {
        m_classPath = builder.m_classPath;
        m_mainClass = builder.m_mainClass;
        m_output = builder.m_output;
    }

    const ClassPath& Environment::classPath() const
    {
        return m_classPath;
    }

    const MainClassStorage& Environment::mainClass() const
    {
        return m_mainClass;
    }

    const std::string& Environment::output() const
    {
        return m_output;
    }

    MainClassStorage::MainClassStorage() :
        m_kind(Kind::CLASS_FILE), m_path() {}

    MainClassStorage::MainClassStorage(MainClassStorage::Kind kind, std::filesystem::path path) :
        m_kind(kind), m_path(std::move(path))
    {
    }

    MainClassStorage::Kind MainClassStorage::kind() const
    {
        return m_kind;
    }

    const std::filesystem::path& MainClassStorage::path() const
    {
        return m_path;
    }

} // namespace AeroJet::Compiler