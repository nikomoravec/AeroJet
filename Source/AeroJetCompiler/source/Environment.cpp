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
        m_output    = builder.m_output;
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

    MainClassStorage::MainClassStorage() : m_kind(Kind::CLASS_FILE), m_path()
    {
    }

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