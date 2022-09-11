#ifndef SUPERJET_COMPILER_H
#define SUPERJET_COMPILER_H

#include "Compiler/Environment.h"
#include "Compiler/Context.h"
#include "Compiler/SourceDependencyGraph.h"
#include "Compiler/CodeGen/Cpp/Type.h"
#include "Java/Archive/Archive.h"
#include "Java/Archive/FieldDescriptor.h"

#include <string>
#include <filesystem>
#include <optional>
#include <map>
#include <memory>
#include <unordered_map>

namespace SuperJet
{
    template<typename T>
    std::string toString(const T& object);

    namespace Compiler
    {
        class ByteCodeCompiler
        {
        public:
            ByteCodeCompiler(const Environment& env);
            void run();

        protected:
            std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Type> javaPrimitiveToCppType(const Java::Archive::FieldDescriptor& fieldDescriptor, SuperJet::Compiler::CodeGen::Cpp::Type::Flags flags = SuperJet::Compiler::CodeGen::Cpp::Type::Flags::NONE);
            std::shared_ptr<SuperJet::Compiler::CodeGen::Node> getDefaultValue(const Java::Archive::FieldDescriptor& fieldDescriptor);

        protected:
            Context context;
            SourceDependencyGraph dependencyGraph;
        };

        static std::shared_ptr<Java::Archive::ClassInfo> loadClass(const Context& context, const std::filesystem::path& clazz)
        {
            static std::unordered_map<std::string, std::shared_ptr<Java::Archive::ClassInfo>> loadedClasses;

            if (loadedClasses.contains(clazz))
            {
                return loadedClasses[clazz];
            }

            std::shared_ptr<Java::Archive::Jar> jar = context.jarForClass(clazz);
            const Java::Archive::Jar::Entry& entry = jar->openClass(clazz.string());

            std::stringstream stream = entry.read();
            std::unique_ptr<Java::Archive::ClassInfo> classInfo = std::make_unique<Java::Archive::ClassInfo>(Java::Archive::read<Java::Archive::ClassInfo>(stream));
            loadedClasses[clazz.string()] = std::move(classInfo);

            return loadedClasses[clazz];
        }
    }
}

#endif //SUPERJET_COMPILER_H
