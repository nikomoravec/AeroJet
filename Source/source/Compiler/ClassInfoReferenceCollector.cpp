#include "Compiler/ClassInfoRerenceCollector.h"
#include "Java/Archive/Archive.h"
#include "spdlog/spdlog.h"
#include "fmt/format.h"
#include "Compiler/Exceptions/RuntimeException.h"
#include "Java/Archive/FieldDescriptor.h"
#include "Java/Types.h"

template <>
struct arrow_traits<SuperJet::Compiler::ConnectionType>
{
    static SuperJet::Compiler::ConnectionType combine(SuperJet::Compiler::ConnectionType x, SuperJet::Compiler::ConnectionType y)
    {
        return (static_cast<SuperJet::Java::JVM::u1>(x) < static_cast<SuperJet::Java::JVM::u1>(y)) ? x : y;
    }

    static int         empty() { return INT32_MAX; }
    static bool        isEmpty(int d) { return d == INT32_MAX; }
    static int         mindist(int d) { return d; }

    static std::string label(int d)
    {
        return std::to_string(d);
    }
};





namespace SuperJet::Compiler
{
    ClassInfoReferenceCollector::ClassInfoReferenceCollector(const Environment& compilerEnvironment) : environment(compilerEnvironment)
    {
        for(const std::filesystem::path& library : environment.libraries)
        {
            std::shared_ptr<SuperJet::Java::Archive::Jar> jar = std::make_shared<SuperJet::Java::Archive::Jar>(Java::Archive::Jar(library));

            for (ssize_t entryIndex = 0; entryIndex < jar->count(); entryIndex++)
            {
                const SuperJet::Java::Archive::Jar::Entry& entry = jar->open(entryIndex);
                if (!entry.isDirectory())
                {
                    if (std::filesystem::path(entry.getName()).extension() == Java::Archive::CLASS_FILE_EXTENSION)
                    {
                        const size_t extensionPosition = entry.getName().find(Java::Archive::CLASS_FILE_EXTENSION);
                        std::string entryNameWithoutExtension = entry.getName().substr(0, extensionPosition);

                        spdlog::info(fmt::format("Found class \'{}\' in archive \'{}\'", entryNameWithoutExtension, jar->getLocation().string()));
                        classMap.emplace(entryNameWithoutExtension, jar);
                    }
                }
            }
        }
    }

    SourceDependencyGraph ClassInfoReferenceCollector::collect()
    {
        std::shared_ptr<Java::Archive::Jar> jar = findJarForClass(environment.mainClass);
        const Java::Archive::Jar::Entry& entry = jar->openClass(environment.mainClass.string());

        std::stringstream stream = entry.read();
        const Java::Archive::ClassInfo& mainClassInfo = Java::Archive::read<Java::Archive::ClassInfo>(stream);

        processClass(mainClassInfo);

        return graph;
    }

    std::shared_ptr<Java::Archive::Jar> ClassInfoReferenceCollector::findJarForClass(const std::string& className)
    {
        auto find = classMap.find(className);
        if (find != classMap.end())
        {
            return classMap.at(className);
        }

        throw SuperJet::Compiler::RuntimeException(fmt::format("Can't find class \'{}\' in any of jar archives!", className));
    }

    void ClassInfoReferenceCollector::processClass(const Java::Archive::ClassInfo& classInfo)
    {
        const Java::Archive::ConstantPool& constantPool = classInfo.getConstantPool();
        Java::JVM::u2 thisClassIndex = classInfo.getThisClass();
        Java::JVM::u2 thisClassNameIndex = constantPool.get<Java::Archive::ConstantPoolInfoClass>(thisClassIndex)->getNameIndex();
        const std::string& thisClassName = constantPool.get<Java::Archive::ConstantPoolInfoUtf8>(thisClassNameIndex)->asString();

        spdlog::info(fmt::format("Gathering dependencies for class \'{}\'...", thisClassName));

        for(const auto& pair : constantPool)
        {

            const std::shared_ptr<Java::Archive::ConstantPoolEntry>& constantPoolEntry = pair.second;
            if (constantPoolEntry->getTag() == Java::Archive::ConstantPoolInfoTag::CLASS)
            {
                std::shared_ptr<Java::Archive::ConstantPoolInfoClass> constantPoolClassEntry = std::static_pointer_cast<Java::Archive::ConstantPoolInfoClass>(constantPoolEntry);
                Java::JVM::u2 dependencyClassNameIndex = constantPoolClassEntry->getNameIndex();
                std::string dependencyClassName = constantPool.get<Java::Archive::ConstantPoolInfoUtf8>(dependencyClassNameIndex)->asString();

                if (dependencyClassName == thisClassName)
                {
                    continue;
                }

                if (dependencyClassName.starts_with(static_cast<char>(Java::Archive::FieldDescriptor::FieldType::ARRAY)))
                {
                    Java::Archive::FieldDescriptor fieldDescriptor(dependencyClassName);
                    std::optional<std::string> optionalClassName = fieldDescriptor.getUnderlyingType()->getClassName();
                    if (!optionalClassName.has_value())
                    {
                        continue;
                    }

                    dependencyClassName = std::string(optionalClassName.value());
                }

                std::shared_ptr<Java::Archive::Jar> jar = findJarForClass(dependencyClassName);

                Java::Archive::Jar::Entry dependencyClassJarEntry = jar->openClass(dependencyClassName);
                std::stringstream dependencyClassJarEntryStream = dependencyClassJarEntry.read();

                const Java::Archive::ClassInfo& dependencyClassInfo = Java::Archive::read<Java::Archive::ClassInfo>(dependencyClassJarEntryStream);

                graph.add(classInfo, dependencyClassInfo, ConnectionType::HARD);
            }
        }

        visited.emplace(classInfo);

        for (const auto& pair : graph.connections(classInfo))
        {
            if (!visited.contains(pair.first))
                processClass(pair.first);
        }
    }
}