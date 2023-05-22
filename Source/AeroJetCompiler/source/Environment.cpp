#include "Environment.hpp"

namespace AeroJet::Compiler
{
    Environment::Builder::Builder() : m_classPath() {}

    Environment::Builder& Environment::Builder::classPath(const AeroJet::Compiler::ClassPath& classPath)
    {
        m_classPath = classPath;
        return *this;
    }

    Environment Environment::Builder::build()
    {
        return Environment{*this};
    }

    Environment::Builder& Environment::Builder::mainClass(const std::string& mainClass)
    {
        m_mainClass = mainClass;
        return *this;
    }

    Environment::Environment(const Environment::Builder& builder)
    {
        m_classPath = builder.m_classPath;
        m_mainClass = builder.m_mainClass;
    }

    const ClassPath& Environment::classPath() const
    {
        return m_classPath;
    }
} // namespace AeroJet::Compiler