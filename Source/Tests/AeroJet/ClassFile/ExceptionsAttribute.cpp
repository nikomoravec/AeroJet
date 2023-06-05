/*
 * ExceptionsAttribute.cpp
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

/*
 * public class TestExceptionsAttribute
 * {
 *   void foo() throws RuntimeException
 *   {
 *     throw new RuntimeException("foo!");
 *   }
 *
 *   void bar() throws NoSuchAttributeException
 *   {
 *     throw new NoSuchAttributeException("bar");
 *   }
 */

TEST_CASE("AeroJet::Java::Attributes::Exceptions")
{
    std::ifstream inputFileStream{"Resources/TestExceptionsAttribute.class"};
    REQUIRE(inputFileStream.is_open());

    AeroJet::Java::ClassFile::ClassInfo classInfo = AeroJet::Stream::Reader::read<AeroJet::Java::ClassFile::ClassInfo>(
            inputFileStream,
            AeroJet::Stream::ByteOrder::INVERSE
    );

    const AeroJet::Java::ClassFile::ConstantPool& constantPool = classInfo.constantPool();

    /* classInfo.methods {
     *  [0] public TestExceptionsAttribute();
     *  [1] void foo() throws java.lang.RuntimeException;
     *  [2] void bar() throws javax.naming.directory.NoSuchAttributeException;
     * }
    */
    SUBCASE("foo")
    {
        const AeroJet::Java::ClassFile::MethodInfo& methodInfo = classInfo.methods()[1];
        CHECK_EQ(
                constantPool.at(methodInfo.nameIndex()).as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(),
                "foo"
        );

        const AeroJet::Java::ClassFile::AttributeInfo& attributeInfo = methodInfo.attributes()[1];
        CHECK_EQ(
                AeroJet::Java::ClassFile::Utils::AttributeInfoUtils::extractName(constantPool, attributeInfo),
                "Exceptions"
        );

        AeroJet::Java::ClassFile::Exceptions exceptionsAttribute{constantPool, attributeInfo};
        CHECK_EQ(exceptionsAttribute.numberOfExceptions(), 1);
        const AeroJet::u2 exceptionClassNameIndex = constantPool.at(exceptionsAttribute.exceptionIndexTable()[0]).as<AeroJet::Java::ClassFile::ConstantPoolInfoClass>().nameIndex();
        CHECK_EQ(
             constantPool.at(exceptionClassNameIndex).as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(),
            "java/lang/RuntimeException"
        );
    }

    SUBCASE("foo")
    {
        const AeroJet::Java::ClassFile::MethodInfo& methodInfo = classInfo.methods()[2];
        CHECK_EQ(
                constantPool.at(methodInfo.nameIndex()).as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(),
                "bar"
        );

        const AeroJet::Java::ClassFile::AttributeInfo& attributeInfo = methodInfo.attributes()[1];
        CHECK_EQ(
                AeroJet::Java::ClassFile::Utils::AttributeInfoUtils::extractName(constantPool, attributeInfo),
                "Exceptions"
        );

        AeroJet::Java::ClassFile::Exceptions exceptionsAttribute{constantPool, attributeInfo};
        CHECK_EQ(exceptionsAttribute.numberOfExceptions(), 1);
        const AeroJet::u2 exceptionClassNameIndex = constantPool.at(exceptionsAttribute.exceptionIndexTable()[0]).as<AeroJet::Java::ClassFile::ConstantPoolInfoClass>().nameIndex();
        CHECK_EQ(
                constantPool.at(exceptionClassNameIndex).as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(),
                "javax/naming/directory/NoSuchAttributeException"
        );
    }
}
