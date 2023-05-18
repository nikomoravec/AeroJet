#pragma once

#include <filesystem>
#include <set>

namespace AeroJet::Compiler
{
    using ClassPath = std::set<std::filesystem::path>;

    class Environment
    {
      public:
        class Builder
        {
            friend Environment;

          public:
            Builder();

            Builder&    classPath(const ClassPath& classPath);
            Environment build();

          private:
            ClassPath m_classPath;
        };

      protected:
        explicit Environment(const Builder& builder);

      protected:
        ClassPath m_classPath;
    };
} // namespace AeroJet::Compiler
