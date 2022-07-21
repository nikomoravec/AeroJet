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
    ClassInfoReferenceCollector::ClassInfoReferenceCollector(const Context& ctx) : context(ctx)
    {
    }

    SourceDependencyGraph ClassInfoReferenceCollector::collect(const std::filesystem::path& className)
    {
        spdlog::info(fmt::format("Gathering dependencies for class '{}'", className.string()));

        std::shared_ptr<Java::Archive::Jar> jar = context.jarForClass(className);
        const Java::Archive::Jar::Entry& entry = jar->openClass(className.string());

        std::stringstream stream = entry.read();
        std::unique_ptr<Java::Archive::ClassInfo> classInfo = std::make_unique<Java::Archive::ClassInfo>(Java::Archive::read<Java::Archive::ClassInfo>(stream));
        const Java::Archive::ConstantPool& constantPool = classInfo->getConstantPool();
        Java::JVM::u2 thisClassIndex = classInfo->getThisClass();
        Java::JVM::u2 thisClassNameIndex = constantPool.get<Java::Archive::ConstantPoolInfoClass>(thisClassIndex)->getNameIndex();
        const std::string& thisClassName = constantPool.get<Java::Archive::ConstantPoolInfoUtf8>(thisClassNameIndex)->asString();

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

                graph.add(thisClassName, dependencyClassName, ConnectionType::HARD);
            }
        }

        visited.emplace(thisClassName);

        for (const auto& pair : graph.connections(thisClassName))
        {
            if (!visited.contains(pair.first))
            {
                collect(pair.first);
            }
        }

        return graph;
    }
}