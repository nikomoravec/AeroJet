#include "AeroJet.hpp"
#include "ClassPath.hpp"
#include "CodeGen/LLVM/IRCodeGen.hpp"
#include "cxxopts.hpp"
#include "Environment.hpp"
#include "ErrorCodes.hpp"
#include "fmt/format.h"

int main(int argc, char** argv)
{
    static constexpr auto TARGET_COMMAND_LINE_OPTION     = "target";
    static constexpr auto CLASS_PATH_COMMAND_LINE_OPTION = "classpath";
    static constexpr auto MAIN_CLASS_COMMAND_LINE_OPTION = "mainclass";
    static constexpr auto OUTPUT_COMMAND_LINE_OPTION = "output";

    static constexpr auto JAR_FILE_EXTENSION        = ".jar";
    static constexpr auto JAVA_CLASS_FILE_EXTENSION = ".class";

    cxxopts::Options options("AeroJetCompiler", "AeroJet Java AoT compiler");
    options.add_options()
        (TARGET_COMMAND_LINE_OPTION, "target jar or java class file", cxxopts::value<std::string>())
        (CLASS_PATH_COMMAND_LINE_OPTION,"class path",cxxopts::value<std::string>())
        (MAIN_CLASS_COMMAND_LINE_OPTION, "main class", cxxopts::value<std::string>())
        (OUTPUT_COMMAND_LINE_OPTION, "base output directory", cxxopts::value<std::string>());

    cxxopts::ParseResult parseResult = options.parse(argc, argv);

    AeroJet::Compiler::Environment::Builder compilerEnvironmentBuilder{};
    compilerEnvironmentBuilder.output(parseResult[OUTPUT_COMMAND_LINE_OPTION].as<std::string>());

    if(!parseResult.count(OUTPUT_COMMAND_LINE_OPTION))
    {
        fmt::print(stderr, "No output directory specified\n");
        fmt::print("{}\n", options.help());
        return AeroJet::Compiler::ErrorCodes::EXIT_CODE_INVALID_ARGS;
    }

    if(!parseResult.count(TARGET_COMMAND_LINE_OPTION))
    {
        fmt::print(stderr, "No target file specified.\n");
        fmt::print("{}\n", options.help());
        return AeroJet::Compiler::ErrorCodes::EXIT_CODE_INVALID_ARGS;
    }

    std::string targetPathString = parseResult[TARGET_COMMAND_LINE_OPTION].as<std::string>();
    AeroJet::Utils::StringUtils::trim(targetPathString);

    std::string                  classPathString = parseResult[CLASS_PATH_COMMAND_LINE_OPTION].as<std::string>();
    AeroJet::Compiler::ClassPath cp{ classPathString };

    std::filesystem::path targetPath = std::filesystem::path(targetPathString);

    compilerEnvironmentBuilder.classPath(cp);
    if(targetPath.extension() == JAVA_CLASS_FILE_EXTENSION)
    {
        compilerEnvironmentBuilder.mainClass(AeroJet::Compiler::MainClassStorage{
            AeroJet::Compiler::MainClassStorage::Kind::CLASS_FILE,
            targetPath
        });
    }
    else if(targetPath.extension() == JAR_FILE_EXTENSION)
    {
        if(!parseResult.count(MAIN_CLASS_COMMAND_LINE_OPTION))
        {
            fmt::print(stderr, "No main class specified.\n");
            fmt::print("{}\n", options.help());
            return AeroJet::Compiler::ErrorCodes::EXIT_CODE_INVALID_ARGS;
        }

        compilerEnvironmentBuilder.mainClass({
            AeroJet::Compiler::MainClassStorage::Kind::ARCHIVE,
            fmt::format("{}:{}", targetPath.string(), parseResult[MAIN_CLASS_COMMAND_LINE_OPTION].as<std::string>())
        });
    }
    else
    {
        fmt::print(stderr, "Target file has unknown file format {}.\n", targetPath.extension().string());
        return AeroJet::Compiler::ErrorCodes::EXIT_CODE_INVALID_ARGS;
    }

    std::unique_ptr<AeroJet::Compiler::LLVM::IRCodeGen> codeGen =
        std::make_unique<AeroJet::Compiler::LLVM::IRCodeGen>(compilerEnvironmentBuilder.build());
    return codeGen->run();
}