/*
 * SimpleClassPrinter.cpp
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

#include "AeroJet.hpp"
#include "fmt/format.h"
#include <filesystem>
#include <fstream>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        fmt::print("usage: ./SimpleClassPrinter /path/to/some/java/class");
        return 1;
    }

    std::filesystem::path classFilePath = argv[1];
    std::ifstream fileStream(classFilePath);
    if (!fileStream.is_open())
    {
        fmt::print("Failed to open file '{}'", classFilePath.string());
        return 1;
    }

    auto classInfo = AeroJet::Stream::Reader::read<AeroJet::Java::ClassFile::ClassInfo>(fileStream, AeroJet::Stream::ByteOrder::INVERSE);
    const auto& constantPool = classInfo.constantPool();

    fmt::print("ClassFile {}\n", classFilePath.string());
    fmt::print("\tMethods:\n");
    const std::vector<AeroJet::Java::ClassFile::MethodInfo>& classMethods = classInfo.methods();
    for (const auto& methodInfo : classMethods)
    {
        const std::string methodName = constantPool[methodInfo.nameIndex()].as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString();
        fmt::print("\t\t{}\n", methodName);
        const std::vector<AeroJet::Java::ClassFile::AttributeInfo>& methodAttributes = methodInfo.attributes();
        auto codeAttributeIt = std::find_if(methodAttributes.begin(), methodAttributes.end(),
            [&constantPool](const AeroJet::Java::ClassFile::AttributeInfo& attributeInfo)
            {
                return AeroJet::Java::ClassFile::Utils::AttributeInfoUtils::extractName(constantPool, attributeInfo) == AeroJet::Java::ClassFile::Code::CODE_ATTRIBUTE_NAME;
            });

        if (codeAttributeIt != methodAttributes.end())
        {
            fmt::print("\t\t\tCode:\n");
            AeroJet::Java::ClassFile::Code codeAttribute{constantPool, *codeAttributeIt};
            for (const auto& instruction : codeAttribute.code())
            {
                fmt::print("\t\t\t\t{:#04x}\n", static_cast<AeroJet::u1>(instruction.opCode()));
            }
        }
    }
}
