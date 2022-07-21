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
            }

            return 0;
        }

    }

    std::cout << options.help() << std::endl;
    return 0;
}
