/*
 * main.cpp
 *
 * Copyright © 2023 AeroJet Developers. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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