/*
 * ClassInfo.cpp
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

TEST_CASE("AeroJet::Java::ClassFile::Instructions::table_switch")
{
    std::ifstream inputFileStream{"Resources/TestJavaBytecodeTableSwitch.class"};
    CHECK(inputFileStream.is_open());

    AeroJet::Java::ClassFile::ClassInfo classInfo = AeroJet::Stream::Reader::read<AeroJet::Java::ClassFile::ClassInfo>(
            inputFileStream,
            AeroJet::Stream::ByteOrder::INVERSE
    );
    const AeroJet::Java::ClassFile::ConstantPool& constantPool = classInfo.constantPool();

    SUBCASE("Check Constant Pool Size")
    {
        CHECK_EQ(constantPool.size(), 54);
    }

    SUBCASE("Check Constant Pool Entries")
    {
        AeroJet::u2 index = 1;
        for (const auto& pair : constantPool)
        {
            AeroJet::u2 entryIndex = pair.first;
            const AeroJet::Java::ClassFile::ConstantPoolEntry& entry = pair.second;
            AeroJet::Java::ClassFile::ConstantPoolInfoTag tag = entry.tag();

            switch (entryIndex)
            {
                case 1:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::METHOD_REF);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoMethodRef>().classIndex(), 14);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoMethodRef>().nameAndTypeIndex(), 29);

                    break;
                }
                case 2:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::FIELD_REF);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoFieldRef>().classIndex(), 30);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoFieldRef>().nameAndTypeIndex(), 31);

                    break;
                }
                case 3:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::STRING);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoString>().stringIndex(), 32);

                    break;
                }
                case 4:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::METHOD_REF);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoMethodRef>().classIndex(), 33);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoMethodRef>().nameAndTypeIndex(), 34);

                    break;
                }
                case 5:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::STRING);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoString>().stringIndex(), 35);

                    break;
                }
                case 6:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::STRING);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoString>().stringIndex(), 36);

                    break;
                }
                case 7:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::CLASS);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoClass>().nameIndex(), 37);

                    break;
                }
                case 8:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::METHOD_REF);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoMethodRef>().classIndex(), 7);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoMethodRef>().nameAndTypeIndex(), 29);

                    break;
                }
                case 9:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::STRING);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoString>().stringIndex(), 38);

                    break;
                }
                case 10:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::METHOD_REF);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoMethodRef>().classIndex(), 7);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoMethodRef>().nameAndTypeIndex(), 39);

                    break;
                }
                case 11:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::METHOD_REF);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoMethodRef>().classIndex(), 7);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoMethodRef>().nameAndTypeIndex(), 40);

                    break;
                }
                case 12:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::METHOD_REF);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoMethodRef>().classIndex(), 7);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoMethodRef>().nameAndTypeIndex(), 41);

                    break;
                }
                case 13:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::CLASS);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoClass>().nameIndex(), 42);

                    break;
                }
                case 14:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::CLASS);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoClass>().nameIndex(), 43);

                    break;
                }
                case 15:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "<init>");

                    break;
                }
                case 16:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "()V");

                    break;
                }
                case 17:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "Code");

                    break;
                }
                case 18:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "LineNumberTable");

                    break;
                }
                case 19:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "LocalVariableTable");

                    break;
                }
                case 20:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "this");

                    break;
                }
                case 21:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "LTestJavaBytecodeTableSwitch;");

                    break;
                }
                case 22:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "tableSwitchTest");

                    break;
                }
                case 23:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "(I)V");

                    break;
                }
                case 24:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "inputValue");

                    break;
                }
                case 25:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "I");

                    break;
                }
                case 26:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "StackMapTable");

                    break;
                }
                case 27:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "SourceFile");

                    break;
                }
                case 28:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "TestJavaBytecodeTableSwitch.java");

                    break;
                }
                case 29:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::NAME_AND_TYPE);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoNameAndType>().nameIndex(), 15);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoNameAndType>().descriptorIndex(), 16);

                    break;
                }
                case 30:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::CLASS);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoClass>().nameIndex(), 44);

                    break;
                }
                case 31:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::NAME_AND_TYPE);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoNameAndType>().nameIndex(), 45);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoNameAndType>().descriptorIndex(), 46);

                    break;
                }
                case 32:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "one!");

                    break;
                }
                case 33:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::CLASS);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoClass>().nameIndex(), 47);

                    break;
                }
                case 34:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::NAME_AND_TYPE);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoNameAndType>().nameIndex(), 48);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoNameAndType>().descriptorIndex(), 49);

                    break;
                }
                case 35:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "two!");

                    break;
                }
                case 36:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "three!");

                    break;
                }
                case 37:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "java/lang/StringBuilder");

                    break;
                }
                case 38:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "inputValue = ");

                    break;
                }
                case 39:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::NAME_AND_TYPE);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoNameAndType>().nameIndex(), 50);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoNameAndType>().descriptorIndex(), 51);

                    break;
                }
                case 40:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::NAME_AND_TYPE);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoNameAndType>().nameIndex(), 50);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoNameAndType>().descriptorIndex(), 52);

                    break;
                }
                case 41:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::NAME_AND_TYPE);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoNameAndType>().nameIndex(), 53);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoNameAndType>().descriptorIndex(), 54);

                    break;
                }
                case 42:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "TestJavaBytecodeTableSwitch");

                    break;
                }
                case 43:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "java/lang/Object");

                    break;
                }
                case 44:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "java/lang/System");

                    break;
                }
                case 45:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "out");

                    break;
                }
                case 46:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "Ljava/io/PrintStream;");

                    break;
                }
                case 47:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "java/io/PrintStream");

                    break;
                }
                case 48:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "println");

                    break;
                }
                case 49:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "(Ljava/lang/String;)V");

                    break;
                }
                case 50:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "append");

                    break;
                }
                case 51:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "(Ljava/lang/String;)Ljava/lang/StringBuilder;");

                    break;
                }
                case 52:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "(I)Ljava/lang/StringBuilder;");

                    break;
                }
                case 53:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "toString");

                    break;
                }
                case 54:
                {
                    CHECK_EQ(entry.tag(), AeroJet::Java::ClassFile::ConstantPoolInfoTag::UTF_8);
                    CHECK_EQ(entry.as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "()Ljava/lang/String;");

                    break;
                }
                default:
                    CHECK(false);
                    break;
            }

            CHECK_EQ(entryIndex, index);
            index++;

            if (tag == AeroJet::Java::ClassFile::ConstantPoolInfoTag::LONG || tag == AeroJet::Java::ClassFile::ConstantPoolInfoTag::DOUBLE)
            {
                index++;
            }
        }
    }

    SUBCASE("Check Methods")
    {
        const std::vector<AeroJet::Java::ClassFile::MethodInfo>& methods = classInfo.methods();
        CHECK_EQ(methods.size(), 2);

        SUBCASE("TestJavaBytecodeTableSwitch")
        {
            const AeroJet::Java::ClassFile::MethodInfo& methodInfo = methods[0];
            const AeroJet::u2 nameIndex = methodInfo.nameIndex();
            const AeroJet::u2 descriptorIndex = methodInfo.descriptorIndex();

            CHECK_EQ(constantPool[nameIndex].as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "<init>");
            CHECK_EQ(constantPool[descriptorIndex].as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "()V");

            AeroJet::Java::ClassFile::MethodDescriptor methodDescriptor{
                std::string{
                    constantPool[descriptorIndex].as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString()
                }
            };

            CHECK_FALSE(methodDescriptor.returnType().has_value());
            CHECK(methodDescriptor.arguments().empty());

            CHECK_EQ(methodInfo.accessFlags(), AeroJet::Java::ClassFile::MethodInfo::AccessFlags::ACC_PUBLIC);
            CHECK_EQ(methodInfo.attributes().size(), 1);

            const std::vector<AeroJet::Java::ClassFile::AttributeInfo>& attributes = methodInfo.attributes();

            SUBCASE("TestJavaBytecodeTableSwitch - Code Attribute")
            {
                const AeroJet::Java::ClassFile::AttributeInfo& attributeInfo = attributes[0];

                CHECK_EQ(constantPool[attributeInfo.attributeNameIndex()].as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "Code");
                AeroJet::Java::ClassFile::Code codeAttribute {constantPool, attributeInfo};
                CHECK_EQ(codeAttribute.maxStack(), 1);
                CHECK_EQ(codeAttribute.maxLocals(), 1);

                const std::vector<AeroJet::Java::ByteCode::Instruction>& code = codeAttribute.code();
                CHECK_EQ(code.size(), 3);

                CHECK_EQ(code[0].opCode(), AeroJet::Java::ByteCode::OperationCode::aload_0);
                CHECK_EQ(code[1].opCode(), AeroJet::Java::ByteCode::OperationCode::invokespecial);
                std::stringstream invokeSpecialDataStream = AeroJet::Stream::Utils::bytesToStream(code[1].data());
                CHECK_EQ(AeroJet::Stream::Reader::read<AeroJet::u2>(
                        invokeSpecialDataStream, AeroJet::Stream::ByteOrder::INVERSE), 1);
                CHECK_EQ(code[2].opCode(), AeroJet::Java::ByteCode::OperationCode::RETURN);

                CHECK_EQ(codeAttribute.attributes().size(), 2);
                SUBCASE("TestJavaBytecodeTableSwitch - Code Attribute - LineNumberTable")
                {
                    CHECK_EQ(constantPool[codeAttribute.attributes()[0].attributeNameIndex()].as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "LineNumberTable");
                    AeroJet::Java::ClassFile::LineNumberTable lineNumberTable {constantPool, codeAttribute.attributes()[0]};
                    CHECK_EQ(lineNumberTable.lineNumberTable().size(), 1);
                    CHECK_EQ(lineNumberTable.lineNumberTable()[0].startPc(), 0);
                    CHECK_EQ(lineNumberTable.lineNumberTable()[0].lineNumber(), 1);
                }
                SUBCASE("TestJavaBytecodeTableSwitch - Code Attribute - LocalVariableTable")
                {
                    CHECK_EQ(constantPool[codeAttribute.attributes()[1].attributeNameIndex()].as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "LocalVariableTable");
                    AeroJet::Java::ClassFile::LocalVariableTable localVariableTable {constantPool, codeAttribute.attributes()[1]};
                    CHECK_EQ(localVariableTable.localVariableTable().size(), 1);
                    CHECK_EQ(localVariableTable.localVariableTable()[0].startPc(), 0);
                    CHECK_EQ(localVariableTable.localVariableTable()[0].length(), 5);
                    CHECK_EQ(
                            constantPool[localVariableTable.localVariableTable()[0]
                            .nameIndex()].as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "this"
                    );
                    CHECK_EQ(
                            constantPool[localVariableTable.localVariableTable()[0]
                            .descriptorIndex()].as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "LTestJavaBytecodeTableSwitch;"
                    );
                }

            }
        }

        SUBCASE("tableSwitchTest(int)")
        {
            const AeroJet::Java::ClassFile::MethodInfo& methodInfo = methods[1];
            const AeroJet::u2 nameIndex = methodInfo.nameIndex();
            const AeroJet::u2 descriptorIndex = methodInfo.descriptorIndex();

            CHECK_EQ(constantPool[nameIndex].as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "tableSwitchTest");
            CHECK_EQ(constantPool[descriptorIndex].as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "(I)V");
            AeroJet::Java::ClassFile::MethodDescriptor methodDescriptor {
                std::string{
                    constantPool[descriptorIndex].as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString()
                }
            };

            CHECK_FALSE(methodDescriptor.returnType().has_value());
            CHECK_EQ(methodDescriptor.arguments().size(), 1);
            CHECK_EQ(methodDescriptor.arguments()[0].fieldType(), AeroJet::Java::ClassFile::FieldDescriptor::FieldType::INTEGER);
            CHECK_EQ(static_cast<AeroJet::u2>(methodInfo.accessFlags()), 0x0009); // ACC_PUBLIC | ACC_STATIC
            CHECK_EQ(methodInfo.attributes().size(), 1);

            SUBCASE("tableSwitchTest(int) - Code Attribute")
            {
                const AeroJet::Java::ClassFile::AttributeInfo& attributeInfo = methodInfo.attributes()[0];
                CHECK_EQ(constantPool[attributeInfo.attributeNameIndex()].as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(), "Code");

                AeroJet::Java::ClassFile::Code codeAttribute {constantPool, attributeInfo};
                CHECK_EQ(codeAttribute.maxStack(), 3);
                CHECK_EQ(codeAttribute.maxLocals(), 1);

                const std::vector<AeroJet::Java::ByteCode::Instruction>& code = codeAttribute.code();
                CHECK_EQ(code.size(), 22);

                SUBCASE("tableSwitchTest(int) - 0: iload_0")
                {
                    const AeroJet::Java::ByteCode::Instruction& instruction = code[0];
                    CHECK_EQ(instruction.opCode(), AeroJet::Java::ByteCode::OperationCode::iload_0);
                }

                SUBCASE("tableSwitchTest(int) - 1: tableswitch")
                {
                    /*
                     * 1: tableswitch   { // 1 to 3
                     *            1: 28
                     *            2: 36
                     *            3: 44
                     *      default: 52
                     * }
                     */
                    const AeroJet::Java::ByteCode::Instruction& instruction = code[1];

                    CHECK_EQ(instruction.opCode(), AeroJet::Java::ByteCode::OperationCode::tableswitch);

                    AeroJet::Stream::MemoryStream dataStream = AeroJet::Stream::Utils::bytesToStream(instruction.data());
                    AeroJet::i4 defaultValue = AeroJet::Stream::Reader::read<AeroJet::i4>(dataStream, AeroJet::Stream::ByteOrder::INVERSE);
                    AeroJet::i4 lowValue = AeroJet::Stream::Reader::read<AeroJet::i4>(dataStream, AeroJet::Stream::ByteOrder::INVERSE);
                    AeroJet::i4 highValue = AeroJet::Stream::Reader::read<AeroJet::i4>(dataStream, AeroJet::Stream::ByteOrder::INVERSE);

                    CHECK_EQ(defaultValue, 52);
                    CHECK_EQ(lowValue, 1);
                    CHECK_EQ(highValue, 3);

                    const AeroJet::i4 jumpOffsetsCount = highValue - lowValue + 1;
                    CHECK_EQ(jumpOffsetsCount, 3);

                    AeroJet::i4 jumpOffset1 = AeroJet::Stream::Reader::read<AeroJet::i4>(dataStream, AeroJet::Stream::ByteOrder::INVERSE);
                    CHECK_EQ(jumpOffset1, 28);
                    AeroJet::i4 jumpOffset2 = AeroJet::Stream::Reader::read<AeroJet::i4>(dataStream, AeroJet::Stream::ByteOrder::INVERSE);
                    CHECK_EQ(jumpOffset2, 36);
                    AeroJet::i4 jumpOffset3 = AeroJet::Stream::Reader::read<AeroJet::i4>(dataStream, AeroJet::Stream::ByteOrder::INVERSE);
                    CHECK_EQ(jumpOffset3, 44);
                }

                SUBCASE("tableSwitchTest(int) - 28: getstatic")
                {
                    // getstatic     #2                  // Field java/lang/System.out:Ljava/io/PrintStream;
                    const AeroJet::Java::ByteCode::Instruction& instruction = code[2];

                    CHECK_EQ(instruction.opCode(), AeroJet::Java::ByteCode::OperationCode::getstatic);
                    std::stringstream dataSteam = AeroJet::Stream::Utils::bytesToStream(instruction.data());
                    CHECK_EQ(AeroJet::Stream::Reader::read<AeroJet::u2>(dataSteam, AeroJet::Stream::ByteOrder::INVERSE), 2);
                }

                SUBCASE("tableSwitchTest(int) - 31: ldc")
                {
                    // ldc           #3 // String one!
                    const AeroJet::Java::ByteCode::Instruction& instruction = code[3];

                    CHECK_EQ(instruction.opCode(), AeroJet::Java::ByteCode::OperationCode::ldc);
                    std::stringstream dataSteam = AeroJet::Stream::Utils::bytesToStream(instruction.data());
                    CHECK_EQ(AeroJet::Stream::Reader::read<AeroJet::u1>(dataSteam, AeroJet::Stream::ByteOrder::INVERSE), 3);
                }

                SUBCASE("tableSwitchTest(int) - 33: invokevirtual")
                {
                    // invokevirtual #4 // Method java/io/PrintStream.println:(Ljava/lang/String;)V
                    const AeroJet::Java::ByteCode::Instruction& instruction = code[4];

                    CHECK_EQ(instruction.opCode(), AeroJet::Java::ByteCode::OperationCode::invokevirtual);
                    std::stringstream dataSteam = AeroJet::Stream::Utils::bytesToStream(instruction.data());
                    CHECK_EQ(AeroJet::Stream::Reader::read<AeroJet::u2>(dataSteam, AeroJet::Stream::ByteOrder::INVERSE), 4);
                }

                SUBCASE("tableSwitchTest(int) - 36: getstatic")
                {
                    // getstatic     #2 // Field java/lang/System.out:Ljava/io/PrintStream;
                    const AeroJet::Java::ByteCode::Instruction& instruction = code[5];

                    CHECK_EQ(instruction.opCode(), AeroJet::Java::ByteCode::OperationCode::getstatic);
                    std::stringstream dataSteam = AeroJet::Stream::Utils::bytesToStream(instruction.data());
                    CHECK_EQ(AeroJet::Stream::Reader::read<AeroJet::u2>(dataSteam, AeroJet::Stream::ByteOrder::INVERSE), 2);
                }

                SUBCASE("tableSwitchTest(int) - 39: ldc")
                {
                    // ldc           #5 // String two!
                    const AeroJet::Java::ByteCode::Instruction& instruction = code[6];

                    CHECK_EQ(instruction.opCode(), AeroJet::Java::ByteCode::OperationCode::ldc);
                    std::stringstream dataSteam = AeroJet::Stream::Utils::bytesToStream(instruction.data());
                    CHECK_EQ(AeroJet::Stream::Reader::read<AeroJet::u1>(dataSteam, AeroJet::Stream::ByteOrder::INVERSE), 5);
                }

                SUBCASE("tableSwitchTest(int) - 41: invokevirtual")
                {
                    // invokevirtual #4 // Method java/io/PrintStream.println:(Ljava/lang/String;)V
                    const AeroJet::Java::ByteCode::Instruction& instruction = code[7];

                    CHECK_EQ(instruction.opCode(), AeroJet::Java::ByteCode::OperationCode::invokevirtual);
                    std::stringstream dataSteam = AeroJet::Stream::Utils::bytesToStream(instruction.data());
                    CHECK_EQ(AeroJet::Stream::Reader::read<AeroJet::u2>(dataSteam, AeroJet::Stream::ByteOrder::INVERSE), 4);
                }

                SUBCASE("tableSwitchTest(int) - 44: getstatic")
                {
                    // getstatic     #2 // Field java/lang/System.out:Ljava/io/PrintStream;
                    const AeroJet::Java::ByteCode::Instruction& instruction = code[8];

                    CHECK_EQ(instruction.opCode(), AeroJet::Java::ByteCode::OperationCode::getstatic);
                    std::stringstream dataSteam = AeroJet::Stream::Utils::bytesToStream(instruction.data());
                    CHECK_EQ(AeroJet::Stream::Reader::read<AeroJet::u2>(dataSteam, AeroJet::Stream::ByteOrder::INVERSE), 2);
                }

                SUBCASE("tableSwitchTest(int) - 47: ldc")
                {
                    // ldc           #6 // String three!
                    const AeroJet::Java::ByteCode::Instruction& instruction = code[9];

                    CHECK_EQ(instruction.opCode(), AeroJet::Java::ByteCode::OperationCode::ldc);
                    std::stringstream dataSteam = AeroJet::Stream::Utils::bytesToStream(instruction.data());
                    CHECK_EQ(AeroJet::Stream::Reader::read<AeroJet::u1>(dataSteam, AeroJet::Stream::ByteOrder::INVERSE), 6);
                }

                SUBCASE("tableSwitchTest(int) - 49: invokevirtual")
                {
                    // invokevirtual #4 // Method java/io/PrintStream.println:(Ljava/lang/String;)V
                    const AeroJet::Java::ByteCode::Instruction& instruction = code[10];

                    CHECK_EQ(instruction.opCode(), AeroJet::Java::ByteCode::OperationCode::invokevirtual);
                    std::stringstream dataSteam = AeroJet::Stream::Utils::bytesToStream(instruction.data());
                    CHECK_EQ(AeroJet::Stream::Reader::read<AeroJet::u2>(dataSteam, AeroJet::Stream::ByteOrder::INVERSE), 4);
                }

                SUBCASE("tableSwitchTest(int) - 52: getstatic")
                {
                    // getstatic #2 // Field java/lang/System.out:Ljava/io/PrintStream;
                    const AeroJet::Java::ByteCode::Instruction& instruction = code[11];

                    CHECK_EQ(instruction.opCode(), AeroJet::Java::ByteCode::OperationCode::getstatic);
                    std::stringstream dataSteam = AeroJet::Stream::Utils::bytesToStream(instruction.data());
                    CHECK_EQ(AeroJet::Stream::Reader::read<AeroJet::u2>(dataSteam, AeroJet::Stream::ByteOrder::INVERSE), 2);
                }

                SUBCASE("tableSwitchTest(int) - 55: new")
                {
                    // 55: new #7 // class java/lang/StringBuilder
                    const AeroJet::Java::ByteCode::Instruction& instruction = code[12];
                    CHECK_EQ(instruction.opCode(), AeroJet::Java::ByteCode::OperationCode::NEW);
                    std::stringstream dataSteam = AeroJet::Stream::Utils::bytesToStream(instruction.data());
                    CHECK_EQ(AeroJet::Stream::Reader::read<AeroJet::u2>(dataSteam, AeroJet::Stream::ByteOrder::INVERSE), 7);
                }

                SUBCASE("tableSwitchTest(int) - 58: dup")
                {
                    // 58: dup
                    const AeroJet::Java::ByteCode::Instruction& instruction = code[13];
                    CHECK_EQ(instruction.opCode(), AeroJet::Java::ByteCode::OperationCode::dup);
                }

                SUBCASE("tableSwitchTest(int) - 59: invokespecial")
                {
                    // 59: invokespecial #8 // Method java/lang/StringBuilder."<init>":()V
                    const AeroJet::Java::ByteCode::Instruction& instruction = code[14];
                    CHECK_EQ(instruction.opCode(), AeroJet::Java::ByteCode::OperationCode::invokespecial);
                    std::stringstream dataSteam = AeroJet::Stream::Utils::bytesToStream(instruction.data());
                    CHECK_EQ(AeroJet::Stream::Reader::read<AeroJet::u2>(dataSteam, AeroJet::Stream::ByteOrder::INVERSE), 8);
                }

                SUBCASE("tableSwitchTest(int) - 62: ldc")
                {
                    // ldc           #9  // String inputValue =
                    const AeroJet::Java::ByteCode::Instruction& instruction = code[15];

                    CHECK_EQ(instruction.opCode(), AeroJet::Java::ByteCode::OperationCode::ldc);
                    std::stringstream dataSteam = AeroJet::Stream::Utils::bytesToStream(instruction.data());
                    CHECK_EQ(AeroJet::Stream::Reader::read<AeroJet::u1>(dataSteam, AeroJet::Stream::ByteOrder::INVERSE), 9);
                }

                SUBCASE("tableSwitchTest(int) - 64: invokevirtual")
                {
                    // invokevirtual #10  // Method java/lang/StringBuilder.append:(Ljava/lang/String;)Ljava/lang/StringBuilder;
                    const AeroJet::Java::ByteCode::Instruction& instruction = code[16];

                    CHECK_EQ(instruction.opCode(), AeroJet::Java::ByteCode::OperationCode::invokevirtual);
                    std::stringstream dataSteam = AeroJet::Stream::Utils::bytesToStream(instruction.data());
                    CHECK_EQ(AeroJet::Stream::Reader::read<AeroJet::u2>(dataSteam, AeroJet::Stream::ByteOrder::INVERSE), 10);
                }

                SUBCASE("tableSwitchTest(int) - 67: iload_0")
                {
                    const AeroJet::Java::ByteCode::Instruction& instruction = code[17];
                    CHECK_EQ(instruction.opCode(), AeroJet::Java::ByteCode::OperationCode::iload_0);
                }

                SUBCASE("tableSwitchTest(int) - 68: invokevirtual")
                {
                    // invokevirtual #11 // Method java/lang/StringBuilder.append:(I)Ljava/lang/StringBuilder;
                    const AeroJet::Java::ByteCode::Instruction& instruction = code[18];

                    CHECK_EQ(instruction.opCode(), AeroJet::Java::ByteCode::OperationCode::invokevirtual);
                    std::stringstream dataSteam = AeroJet::Stream::Utils::bytesToStream(instruction.data());
                    CHECK_EQ(AeroJet::Stream::Reader::read<AeroJet::u2>(dataSteam, AeroJet::Stream::ByteOrder::INVERSE), 11);
                }

                SUBCASE("tableSwitchTest(int) - 68: invokevirtual")
                {
                    // invokevirtual #11 // Method java/lang/StringBuilder.append:(I)Ljava/lang/StringBuilder;
                    const AeroJet::Java::ByteCode::Instruction& instruction = code[18];

                    CHECK_EQ(instruction.opCode(), AeroJet::Java::ByteCode::OperationCode::invokevirtual);
                    std::stringstream dataSteam = AeroJet::Stream::Utils::bytesToStream(instruction.data());
                    CHECK_EQ(AeroJet::Stream::Reader::read<AeroJet::u2>(dataSteam, AeroJet::Stream::ByteOrder::INVERSE), 11);
                }

                SUBCASE("tableSwitchTest(int) - 71: invokevirtual")
                {
                    // invokevirtual #12  // Method java/lang/StringBuilder.toString:()Ljava/lang/String;
                    const AeroJet::Java::ByteCode::Instruction& instruction = code[19];

                    CHECK_EQ(instruction.opCode(), AeroJet::Java::ByteCode::OperationCode::invokevirtual);
                    std::stringstream dataSteam = AeroJet::Stream::Utils::bytesToStream(instruction.data());
                    CHECK_EQ(AeroJet::Stream::Reader::read<AeroJet::u2>(dataSteam, AeroJet::Stream::ByteOrder::INVERSE), 12);
                }

                SUBCASE("tableSwitchTest(int) - 74: invokevirtual")
                {
                    // invokevirtual #4 // Method java/io/PrintStream.println:(Ljava/lang/String;)V

                    const AeroJet::Java::ByteCode::Instruction& instruction = code[20];

                    CHECK_EQ(instruction.opCode(), AeroJet::Java::ByteCode::OperationCode::invokevirtual);
                    std::stringstream dataSteam = AeroJet::Stream::Utils::bytesToStream(instruction.data());
                    CHECK_EQ(AeroJet::Stream::Reader::read<AeroJet::u2>(dataSteam, AeroJet::Stream::ByteOrder::INVERSE), 4);
                }

                SUBCASE("tableSwitchTest(int) - 77: return")
                {
                    const AeroJet::Java::ByteCode::Instruction& instruction = code[21];
                    CHECK_EQ(instruction.opCode(), AeroJet::Java::ByteCode::OperationCode::RETURN);
                }
            }
        }
    }
}