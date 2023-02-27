#include "cxxopts.hpp"
#include "fmt/format.h"

int main(int argc, char** argv)
{
    static constexpr auto APPLICATION_NAME = "java";

    static constexpr auto JAR_COMMAND_LINE_OPTION = "jar";
    static constexpr auto CLASSPATH_COMMAND_LINE_OPTION = "classpath";

    cxxopts::Options options(APPLICATION_NAME, "AeroJet JVM");

    std::vector<std::string> classPath {};

    options.add_options()
        (JAR_COMMAND_LINE_OPTION, "jar file to execute", cxxopts::value<std::string>())
        (CLASSPATH_COMMAND_LINE_OPTION,"A ',' separated list of directories, JAR archives, and ZIP archives to search for class files.", cxxopts::value(classPath));

    cxxopts::ParseResult parseResult = options.parse(argc, argv);

    if(!parseResult.count(JAR_COMMAND_LINE_OPTION))
    {
        std::cout << options.help() << std::endl;
    }

    return 0;
}