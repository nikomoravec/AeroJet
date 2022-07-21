#include "cxxopts.hpp"
#include "SuperJet.h"
#include "spdlog/spdlog.h"

int main(int argc, char** argv)
{
    cxxopts::Options options("SuperJet", "Java AOT compiler");

    options.add_options()
            ("jar", "input Java archive", cxxopts::value<std::string>())
            ("jhome", "JAVA_HOME", cxxopts::value<std::string>())
            ("main", "main class", cxxopts::value<std::string>());

    const cxxopts::ParseResult& commandLineParse = options.parse(argc, argv);

    if (commandLineParse.count("jar"))
    {
        if (commandLineParse.count("jhome"))
        {
            if (commandLineParse.count("main"))
            {
                SuperJet::Compiler::Environment environment;
                environment.mainClass = commandLineParse["main"].as<std::string>();

                for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(std::filesystem::path(commandLineParse["jhome"].as<std::string>()) / "jre" / "lib"))
                {
                    if(entry.is_regular_file())
                    {
                        if (entry.path().extension() == ".jar")
                        {
                            environment.libraries.emplace(entry.path());
                        }
                    }
                }

                environment.libraries.emplace(commandLineParse["jar"].as<std::string>());

                SuperJet::Compiler::ByteCodeCompiler compiler(environment);
                compiler.run();

//                SuperJet::Compiler::ClassInfoReferenceCollector collector = {environment};
//                SuperJet::Compiler::SourceDependencyGraph graph = collector.collect();

//                std::set<SuperJet::Java::Archive::ClassInfo> captured;
//                for (const SuperJet::Java::Archive::ClassInfo& classInfo : graph.nodes())
//                {
//                    for(const auto& pair : graph.connections(classInfo))
//                    {
//                        const SuperJet::Java::Archive::ClassInfo& dependency = pair.first;
//
//                        if (!captured.contains(classInfo) && !captured.contains(dependency))
//                        {
//                            if (graph.connections(dependency).contains(classInfo))
//                            {
//                                static std::set<SuperJet::Java::Archive::ClassInfo> captured;
//
//                                const SuperJet::Java::Archive::ConstantPool& constantPool = classInfo.getConstantPool();
//                                SuperJet::Java::JVM::u2 thisClassIndex = classInfo.getThisClass();
//                                SuperJet::Java::JVM::u2 thisClassNameIndex = constantPool.get<SuperJet::Java::Archive::ConstantPoolInfoClass>(thisClassIndex)->getNameIndex();
//                                const std::string& thisClassName = constantPool.get<SuperJet::Java::Archive::ConstantPoolInfoUtf8>(thisClassNameIndex)->asString();
//
//
//                                const SuperJet::Java::Archive::ConstantPool& dependencyConstantPool = dependency.getConstantPool();
//                                SuperJet::Java::JVM::u2 dependencyClassIndex = dependency.getThisClass();
//                                SuperJet::Java::JVM::u2 dependencyClassNameIndex = dependencyConstantPool.get<SuperJet::Java::Archive::ConstantPoolInfoClass>(dependencyClassIndex)->getNameIndex();
//                                const std::string& dependencyClassName = dependencyConstantPool.get<SuperJet::Java::Archive::ConstantPoolInfoUtf8>(dependencyClassNameIndex)->asString();
//
//                                spdlog::warn(fmt::format("Found cross reference '{}' <-> '{}'", thisClassName, dependencyClassName));
//
//                                captured.emplace(classInfo);
//                                captured.emplace(dependency);
//                            }
//                        }
//                    }
//                }
            }

            return 0;
        }

    }

    std::cout << options.help() << std::endl;
    return 0;
}
