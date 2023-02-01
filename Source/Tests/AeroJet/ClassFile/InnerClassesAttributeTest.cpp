/*
* InnerClassesAttributeTest.cpp
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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "AeroJet.hpp"
#include "doctest.h"

/**
 * class TestInnerClassesAttribute
 *{
 *   class InnerClass
 *   {
 *   }
 *}
 */

TEST_CASE("AeroJet::Java::Attributes::InnerClasses")
{
    std::ifstream inputFileStream{ "Resources/TestInnerClassesAttribute.class" };
    REQUIRE(inputFileStream.is_open());

    AeroJet::Java::ClassFile::ClassInfo classInfo =
        AeroJet::Stream::Reader::read<AeroJet::Java::ClassFile::ClassInfo>(inputFileStream,
                                                                           AeroJet::Stream::ByteOrder::INVERSE);

    const AeroJet::Java::ClassFile::ConstantPool&               constantPool    = classInfo.constantPool();
    const std::vector<AeroJet::Java::ClassFile::AttributeInfo>& classAttributes = classInfo.attributes();

    // [0] - SourceFile
    // [1] - InnerClasses
    REQUIRE_EQ(classAttributes.size(), 2);

    AeroJet::Java::ClassFile::AttributeInfo innerClassesAttributeInfo = classAttributes[1];
    REQUIRE_EQ(
        AeroJet::Java::ClassFile::Utils::AttributeInfoUtils::extractName(constantPool, innerClassesAttributeInfo),
        AeroJet::Java::ClassFile::InnerClasses::INNER_CLASSES_ATTRIBUTE_NAME);

    AeroJet::Java::ClassFile::InnerClasses innerClassesAttribute =
        AeroJet::Java::ClassFile::InnerClasses{ constantPool, innerClassesAttributeInfo };
    REQUIRE_EQ(innerClassesAttribute.numberOfClasses(), 1);

    AeroJet::Java::ClassFile::InnerClasses::InnerClass innerClass = innerClassesAttribute.innerClasses()[0];
    REQUIRE_EQ(innerClass.innerNameIndex(), 5);
    REQUIRE_EQ(innerClass.outerClassInfoIndex(), 2);
    REQUIRE_EQ(innerClass.innerClassInfoIndex(), 4);
    REQUIRE_EQ(static_cast<AeroJet::u2>(innerClass.innerClassAccessFlags()), 0);
}