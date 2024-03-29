/*
 * IRCodeGen.cpp
 *
 * Copyright © 2024 AeroJet Developers. All Rights Reserved.
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

#include "CodeGen/LLVM/IRCodeGen.hpp"

#include "ErrorCodes.hpp"
#include "spdlog/spdlog.h"

#include <fstream>
#include <iostream>
#include <utility>

namespace AeroJet::Compiler::LLVM
{

    llvm::LLVMContext IRCodeGen::s_llvmContext = llvm::LLVMContext{};

    IRCodeGen::IRCodeGen(Environment environment) :
        m_environment{ std::move(environment) }
    {
    }

    void translateConstantPool(TranslationUnit& tu, const Java::ClassFile::ConstantPool& constantPool)
    {
        llvm::StructType* fieldRefStructTy = llvm::StructType::create(IRCodeGen::llvmContext(), fmt::format("{}.CONSTANT_POOL.FIELD_REF", tu.name()));
        fieldRefStructTy->setBody(tu.builder().getInt16Ty(), tu.builder().getInt16Ty());

        llvm::StructType* methodRefStructTy =
            llvm::StructType::create(IRCodeGen::llvmContext(), fmt::format("{}.CONSTANT_POOL.METHOD_REF", tu.name()));
        methodRefStructTy->setBody(tu.builder().getInt16Ty(), tu.builder().getInt16Ty());

        llvm::StructType* nameAndTypeStructTy =
            llvm::StructType::create(IRCodeGen::llvmContext(), fmt::format("{}.CONSTANT_POOL.NAME_AND_TYPE", tu.name()));
        nameAndTypeStructTy->setBody(tu.builder().getInt16Ty(), tu.builder().getInt16Ty());

        llvm::StructType* interfaceMethodRefStructTy =
            llvm::StructType::create(IRCodeGen::llvmContext(), fmt::format("{}.CONSTANT_POOL.INTERFACE_METHOD_REF", tu.name()));
        interfaceMethodRefStructTy->setBody(tu.builder().getInt16Ty(), tu.builder().getInt16Ty());

        llvm::StructType* methodHandleStructTy = llvm::StructType::create(IRCodeGen::llvmContext(), fmt::format("{}.CONSTANT_POOL.METHOD_HANDLE", tu.name()));
        methodHandleStructTy->setBody(tu.builder().getInt8Ty(), tu.builder().getInt16Ty());

        llvm::StructType* invokeDynamicStructTy =
            llvm::StructType::create(IRCodeGen::llvmContext(), fmt::format("{}.CONSTANT_POOL.INVOKE_DYNAMIC", tu.name()));
        invokeDynamicStructTy->setBody(tu.builder().getInt16Ty(), tu.builder().getInt16Ty());

        for(const auto& pair : constantPool)
        {
            const u2 index = pair.first;
            const Java::ClassFile::ConstantPoolEntry& entry = pair.second;
            const std::string constantPoolGlobalEntryName = fmt::format("{}_CONSTANT_POOL_ENTRY_{}", tu.name(), index);

            llvm::GlobalVariable* entryTypeGlobalVar = tu.addGlobalVariable(fmt::format("{}_CONSTANT_POOL_ENTRY_TYPE_{}", tu.name(), index), tu.builder().getInt16Ty());
            entryTypeGlobalVar->setConstant(true);
            entryTypeGlobalVar->setInitializer(tu.builder().getInt16(static_cast<AeroJet::u2>(entry.tag())));
            entryTypeGlobalVar->setLinkage(llvm::GlobalVariable::PrivateLinkage);

            switch(entry.tag())
            {
                case Java::ClassFile::ConstantPoolInfoTag::UTF_8:
                {
                    const std::string val = entry.as<Java::ClassFile::ConstantPoolInfoUtf8>().asString();
                    tu.builder().CreateGlobalString(val, constantPoolGlobalEntryName, 0, tu.module().get());
                    break;
                }
                case Java::ClassFile::ConstantPoolInfoTag::INTEGER:
                {
                    llvm::GlobalVariable* gVar = tu.addGlobalVariable(constantPoolGlobalEntryName, tu.builder().getInt32Ty());
                    gVar->setConstant(true);
                    gVar->setInitializer(tu.builder().getInt32(entry.as<Java::ClassFile::ConstantPoolInfoInteger>().bytes()));
                    gVar->setLinkage(llvm::GlobalVariable::PrivateLinkage);
                    break;
                }
                case Java::ClassFile::ConstantPoolInfoTag::FLOAT:
                {
                    llvm::GlobalVariable* gVar = tu.addGlobalVariable(constantPoolGlobalEntryName, tu.builder().getFloatTy());
                    gVar->setConstant(true);
                    gVar->setInitializer(llvm::ConstantFP::get(tu.builder().getFloatTy(), entry.as<Java::ClassFile::ConstantPoolInfoFloat>().bytes()));
                    gVar->setLinkage(llvm::GlobalVariable::PrivateLinkage);
                    break;
                }
                case Java::ClassFile::ConstantPoolInfoTag::LONG:
                {
                    i8 value = AeroJet::Java::ClassFile::Utils::ConstantPoolEntryUtils::toLong(entry.as<Java::ClassFile::ConstantPoolInfoLong>());

                    llvm::GlobalVariable* gVar = tu.addGlobalVariable(constantPoolGlobalEntryName, tu.builder().getInt64Ty());
                    gVar->setConstant(true);
                    gVar->setInitializer(tu.builder().getInt64(value));
                    gVar->setLinkage(llvm::GlobalVariable::PrivateLinkage);
                    break;
                }
                case Java::ClassFile::ConstantPoolInfoTag::DOUBLE:
                {
                    double value = AeroJet::Java::ClassFile::Utils::ConstantPoolEntryUtils::toDouble(entry.as<Java::ClassFile::ConstantPoolInfoDouble>());

                    llvm::GlobalVariable* gVar = tu.addGlobalVariable(constantPoolGlobalEntryName, tu.builder().getDoubleTy());
                    gVar->setConstant(true);
                    gVar->setInitializer(llvm::ConstantFP::get(tu.builder().getDoubleTy(), value));
                    gVar->setLinkage(llvm::GlobalVariable::PrivateLinkage);
                    break;
                }
                case Java::ClassFile::ConstantPoolInfoTag::CLASS:
                {
                    const Java::ClassFile::ConstantPoolInfoClass& value = entry.as<Java::ClassFile::ConstantPoolInfoClass>();

                    tu.module()->getOrInsertGlobal(constantPoolGlobalEntryName, tu.builder().getInt16Ty());
                    llvm::GlobalVariable* gVar = tu.module()->getNamedGlobal(constantPoolGlobalEntryName);
                    gVar->setConstant(true);
                    gVar->setInitializer(tu.builder().getInt16(value.nameIndex()));
                    gVar->setLinkage(llvm::GlobalVariable::PrivateLinkage);
                }
                case Java::ClassFile::ConstantPoolInfoTag::STRING:
                {
                    llvm::GlobalVariable* gVar = tu.addGlobalVariable(constantPoolGlobalEntryName, tu.builder().getInt16Ty());
                    gVar->setConstant(true);
                    gVar->setInitializer(tu.builder().getInt16(entry.as<Java::ClassFile::ConstantPoolInfoString>().stringIndex()));
                    gVar->setLinkage(llvm::GlobalVariable::PrivateLinkage);
                    break;
                }
                case Java::ClassFile::ConstantPoolInfoTag::FIELD_REF:
                {
                    const Java::ClassFile::ConstantPoolInfoFieldRef& fieldRef = entry.as<Java::ClassFile::ConstantPoolInfoFieldRef>();
                    llvm::GlobalVariable* gVar = tu.addGlobalVariable(constantPoolGlobalEntryName, fieldRefStructTy);

                    llvm::Constant* initValue =
                        llvm::ConstantStruct::get(fieldRefStructTy,
                                                  tu.builder().getInt16(fieldRef.classIndex()),
                                                  tu.builder().getInt16(fieldRef.nameAndTypeIndex()));

                    gVar->setInitializer(initValue);

                    gVar->setConstant(true);
                    gVar->setLinkage(llvm::GlobalVariable::PrivateLinkage);

                    break;
                }
                case Java::ClassFile::ConstantPoolInfoTag::METHOD_REF:
                {
                    const Java::ClassFile::ConstantPoolInfoMethodRef& methodRef =
                        entry.as<Java::ClassFile::ConstantPoolInfoMethodRef>();

                    llvm::GlobalVariable* gVar = tu.addGlobalVariable(constantPoolGlobalEntryName, methodRefStructTy);

                    llvm::Constant* initValue =
                        llvm::ConstantStruct::get(methodRefStructTy,
                                                  tu.builder().getInt16(methodRef.classIndex()),
                                                  tu.builder().getInt16(methodRef.nameAndTypeIndex()));

                    gVar->setInitializer(initValue);

                    gVar->setConstant(true);
                    gVar->setLinkage(llvm::GlobalVariable::PrivateLinkage);

                    break;
                }
                case Java::ClassFile::ConstantPoolInfoTag::INTERFACE_METHOD_REF:
                {
                    const Java::ClassFile::ConstantPoolInfoInterfaceMethodRef& interfaceMethodRef =
                        entry.as<Java::ClassFile::ConstantPoolInfoInterfaceMethodRef>();

                    llvm::GlobalVariable* gVar = tu.addGlobalVariable(constantPoolGlobalEntryName, interfaceMethodRefStructTy);

                    llvm::Constant* initValue =
                        llvm::ConstantStruct::get(methodRefStructTy,
                                                  tu.builder().getInt16(interfaceMethodRef.classIndex()),
                                                  tu.builder().getInt16(interfaceMethodRef.nameAndTypeIndex()));

                    gVar->setInitializer(initValue);

                    gVar->setConstant(true);
                    gVar->setLinkage(llvm::GlobalVariable::PrivateLinkage);

                    break;
                }
                case Java::ClassFile::ConstantPoolInfoTag::NAME_AND_TYPE:
                {
                    const Java::ClassFile::ConstantPoolInfoNameAndType& nameAndType =
                        entry.as<Java::ClassFile::ConstantPoolInfoNameAndType>();

                    llvm::GlobalVariable* gVar = tu.addGlobalVariable(constantPoolGlobalEntryName, nameAndTypeStructTy);
                    llvm::Constant* initValue =
                        llvm::ConstantStruct::get(nameAndTypeStructTy,
                                                  tu.builder().getInt16(nameAndType.nameIndex()),
                                                  tu.builder().getInt16(nameAndType.descriptorIndex()));

                    gVar->setInitializer(initValue);

                    gVar->setConstant(true);
                    gVar->setLinkage(llvm::GlobalVariable::PrivateLinkage);
                    break;
                }
                case Java::ClassFile::ConstantPoolInfoTag::METHOD_HANDLE:
                {
                    const Java::ClassFile::ConstantPoolInfoMethodHandle& methodHandle = entry.as<Java::ClassFile::ConstantPoolInfoMethodHandle>();
                    llvm::GlobalVariable* gVar = tu.addGlobalVariable(constantPoolGlobalEntryName, methodHandleStructTy);

                    llvm::Constant* initValue =
                        llvm::ConstantStruct::get(methodHandleStructTy,
                                                  tu.builder().getInt8(static_cast<uint8_t>(methodHandle.referenceKind())),
                                                  tu.builder().getInt16(methodHandle.referenceIndex()));
                    gVar->setInitializer(initValue);

                    gVar->setConstant(true);
                    gVar->setLinkage(llvm::GlobalVariable::PrivateLinkage);
                    break;
                }
                case Java::ClassFile::ConstantPoolInfoTag::METHOD_TYPE:
                {
                    llvm::GlobalVariable* gVar = tu.addGlobalVariable(constantPoolGlobalEntryName, tu.builder().getInt16Ty());
                    gVar->setConstant(true);
                    gVar->setInitializer(tu.builder().getInt16(entry.as<Java::ClassFile::ConstantPoolInfoMethodType>().descriptorIndex()));
                    gVar->setLinkage(llvm::GlobalVariable::PrivateLinkage);
                    break;
                }
                case Java::ClassFile::ConstantPoolInfoTag::INVOKE_DYNAMIC:
                {
                    const Java::ClassFile::ConstantPoolInfoInvokeDynamic& invokeDynamic =
                        entry.as<Java::ClassFile::ConstantPoolInfoInvokeDynamic>();

                    llvm::GlobalVariable* gVar = tu.addGlobalVariable(constantPoolGlobalEntryName, nameAndTypeStructTy);
                    llvm::Constant* initValue =
                        llvm::ConstantStruct::get(nameAndTypeStructTy,
                                                  tu.builder().getInt16(invokeDynamic.nameAndTypeIndex()),
                                                  tu.builder().getInt16(invokeDynamic.bootstrapMethodAttributeIndex()));

                    gVar->setInitializer(initValue);

                    gVar->setConstant(true);
                    gVar->setLinkage(llvm::GlobalVariable::PrivateLinkage);
                    break;
                }
            }
        }
    }

    TranslationUnit IRCodeGen::translate(const Java::ClassFile::ClassInfo& classInfo)
    {
        const std::string javaClassFullName = Java::ClassFile::Utils::ClassInfoUtils::javaName(classInfo);
        TranslationUnit tu{ javaClassFullName };

        const Java::ClassFile::ConstantPool& constantPool = classInfo.constantPool();
        translateConstantPool(tu, constantPool);

        tu.print(std::cout);
        tu.toObjectFile(std::filesystem::path{ m_environment.output() } / fmt::format("{}.o", javaClassFullName));
        return tu;
    }

    i4 IRCodeGen::run()
    {
        const MainClassStorage& environmentMainClass = m_environment.mainClass();
        const std::filesystem::path mainClassPath = environmentMainClass.path();

        std::optional<Java::ClassFile::ClassInfo> mainClassInfo{};

        if(environmentMainClass.kind() == MainClassStorage::Kind::CLASS_FILE)
        {
            std::fstream mainClassFileStream{ mainClassPath };
            if(!mainClassFileStream.is_open())
            {
                spdlog::error(fmt::format("Could not open main class file {}", mainClassPath.string()));
                return ErrorCodes::EXIT_CODE_FAILED_TO_OPEN_MAIN_CLASS;
            }

            mainClassInfo = Stream::Reader::read<Java::ClassFile::ClassInfo>(mainClassFileStream, Stream::ByteOrder::INVERSE);
        }
        else if(environmentMainClass.kind() == MainClassStorage::Kind::ARCHIVE)
        {
            if(const std::vector<std::string> split = Utils::StringUtils::split(mainClassPath, ':'); split.size() == 2)
            {
                const std::string& archivePath = split[0];
                const std::string& relativeMainPath = split[1];

                AeroJet::Java::Archive::Jar archive{ archivePath };
                AeroJet::Java::Archive::Jar::Entry archiveEntry = archive.open(relativeMainPath);

                Stream::MemoryStream mainClassStream = archiveEntry.read();
                mainClassInfo = Stream::Reader::read<Java::ClassFile::ClassInfo>(mainClassStream, Stream::ByteOrder::INVERSE);
            }
        }

        if(!mainClassInfo.has_value())
        {
            spdlog::error(fmt::format("Failed to read main class", mainClassPath.string()));
            return ErrorCodes::EXIT_CODE_FAILED_TO_READ_MAIN_CLASS;
        }

        const TranslationUnit mainClassTranslationUnit = translate(mainClassInfo.value());
        return 0;
    }

    llvm::LLVMContext& IRCodeGen::llvmContext()
    {
        return s_llvmContext;
    }

} // namespace AeroJet::Compiler::LLVM
