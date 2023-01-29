/*
 * ClassInfoUtils.cpp
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

#include "Java/ClassFile/Utils/ClassInfoUtils.hpp"

namespace AeroJet::Java::ClassFile::Utils
{
    bool ClassInfoUtils::isUnderPackage(const ClassInfo& classInfo)
    {
        return ClassInfoUtils::name(classInfo).find(CLASS_PACKAGE_DELIMITER) != std::string::npos;
    }

    std::string ClassInfoUtils::name(const ClassInfo& classInfo)
    {
        const ConstantPool& constantPool = classInfo.constantPool();

        const u2 thisClassIndex = classInfo.thisClass();
        const u2 nameIndex      = constantPool[thisClassIndex].as<ConstantPoolInfoClass>().nameIndex();
        return constantPool[nameIndex].as<ConstantPoolInfoUtf8>().asString();
    }

    std::string ClassInfoUtils::className(const ClassInfo& classInfo)
    {
        std::string className = ClassInfoUtils::name(classInfo);

        if(ClassInfoUtils::isUnderPackage(classInfo))
        {
            std::vector<std::string> split;
            {
                size_t start;
                size_t end = 0;
                while((start = className.find_first_not_of(CLASS_PACKAGE_DELIMITER, end)) != std::string::npos)
                {
                    end = className.find(CLASS_PACKAGE_DELIMITER, start);
                    split.push_back(className.substr(start, end - start));
                }
            }

            return split.back();
        }

        return className;
    }
} // namespace AeroJet::Java::ClassFile::Utils