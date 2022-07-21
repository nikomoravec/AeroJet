#include "cxxopts.hpp"
#include "SuperJet.h"

int main(int argc, char** argv)
{
    cxxopts::Options options("SuperJet", "Java AOT compiler");

    options.add_options()
            ("i,input", "input Java class", cxxopts::value<std::string>());

    const cxxopts::ParseResult& commandLineParse = options.parse(argc, argv);

    if (commandLineParse.count("input"))
    {
        const SuperJet::Java::Archive::ClassInfo& classInfo = SuperJet::Java::Archive::read<SuperJet::Java::Archive::ClassInfo>(commandLineParse["input"].as<std::string>());
        SuperJet::Compiler::ClassInfoResolver classInfoResolver(classInfo);
        SuperJet::Compiler::ResolvedTypes::ClassInfo resolvedClass = classInfoResolver.resolve();
        return 0;
    }

    std::cout << options.help() << std::endl;
    return 0;
}
