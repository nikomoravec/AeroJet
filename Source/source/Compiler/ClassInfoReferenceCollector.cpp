#include "Compiler/ClassInfoRerenceCollector.h"
#include "Compiler/Type/Traits.h"
#include "Java/Archive/Archive.h"
#include "spdlog/spdlog.h"
#include "fmt/format.h"
#include "Java/Archive/FieldDescriptor.h"
#include "Java/Types.h"

template <>
struct arrow_traits<SuperJet::Compiler::ConnectionType>
{
    static SuperJet::Compiler::ConnectionType combine(SuperJet::Compiler::ConnectionType x, SuperJet::Compiler::ConnectionType y)
    {
        return (static_cast<SuperJet::Java::JVM::u1>(x) < static_cast<SuperJet::Java::JVM::u1>(y)) ? x : y;
    }

    static int empty() { return INT32_MAX; }
    static bool isEmpty(int d) { return d == INT32_MAX; }
    static int  mindist(int d) { return d; }

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
        return collect(Compiler::loadClass(context, className));
    }

    SourceDependencyGraph ClassInfoReferenceCollector::collect(const std::shared_ptr<Java::Archive::ClassInfo>& rootClass)
    {
        const Java::Archive::ConstantPool& constantPool = rootClass->getConstantPool();
        Java::JVM::u2 thisClassIndex = rootClass->getThisClass();
        Java::JVM::u2 thisClassNameIndex = constantPool.get<Java::Archive::ConstantPoolInfoClass>(thisClassIndex)->getNameIndex();
        const std::string& thisClassName = constantPool.get<Java::Archive::ConstantPoolInfoUtf8>(thisClassNameIndex)->asString();

        spdlog::info(fmt::format("Gathering dependencies for class '{}'", thisClassName));

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

                std::shared_ptr<Java::Archive::ClassInfo> dependencyClassInfo = Compiler::loadClass(context, dependencyClassName);

                graph.add(rootClass, dependencyClassInfo, Type::Traits::isBaseOf(context, dependencyClassInfo, rootClass) ? ConnectionType::HARD : ConnectionType::SOFT);
            }

            if (constantPoolEntry->getTag() == Java::Archive::ConstantPoolInfoTag::NAME_AND_TYPE)
            {
                std::shared_ptr<Java::Archive::ConstantPoolInfoNameAndType> constantPoolClassEntry = std::static_pointer_cast<Java::Archive::ConstantPoolInfoNameAndType>(constantPoolEntry);
                Java::JVM::u2 descriptorIndex = constantPoolClassEntry->getDescriptorIndex();
                const std::string& rawDescriptor = constantPool.get<Java::Archive::ConstantPoolInfoUtf8>(descriptorIndex)->asString();
                if (rawDescriptor.starts_with(static_cast<char>(Java::Archive::FieldDescriptor::FieldType::CLASS)))
                {
                    Java::Archive::FieldDescriptor fieldDescriptor = Java::Archive::FieldDescriptor(rawDescriptor);

                    std::shared_ptr<Java::Archive::ClassInfo> dependencyClassInfo = Compiler::loadClass(context, *fieldDescriptor.getClassName());
                    graph.add(rootClass, dependencyClassInfo, ConnectionType::HARD);
                }
            }
        }

        visited.emplace(rootClass);

        for (const auto& pair : graph.connections(rootClass))
        {
            if (!visited.contains(pair.first))
            {
                collect(pair.first);
            }
        }

        return graph;
    }
}