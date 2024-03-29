/*
 * TranslationUnit.cpp
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

#include "CodeGen/LLVM/TranslationUnit.hpp"

#include "CodeGen/LLVM/IRCodeGen.hpp"
#include "ErrorCodes.hpp"
#include "Exceptions/CompilerException.hpp"
#include "fmt/format.h"

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/Transforms/Utils/Cloning.h>

namespace AeroJet::Compiler::LLVM
{
    TranslationUnit::TranslationUnit(std::string name) :
        m_name(std::move(name)), m_module(std::make_unique<llvm::Module>(m_name, IRCodeGen::llvmContext())),
        m_builder(IRCodeGen::llvmContext())
    {
    }

    llvm::GlobalVariable* TranslationUnit::addGlobalVariable(const std::string& name, llvm::Type* type)
    {
        m_module->getOrInsertGlobal(name, type);
        return m_module->getNamedGlobal(name);
    }

    TranslationUnit::TranslationUnit(const TranslationUnit& other) :
        m_name(other.m_name), m_module(llvm::CloneModule(*other.m_module)), m_builder(IRCodeGen::llvmContext())
    {
    }

    const std::string& TranslationUnit::name() const
    {
        return m_name;
    }

    void TranslationUnit::toObjectFile(const std::filesystem::path& outputFilePath) const
    {
        static constexpr auto cpu = "generic";

        llvm::InitializeNativeTarget();
        llvm::InitializeNativeTargetAsmParser();
        llvm::InitializeNativeTargetAsmPrinter();

        const std::string targetTriple = llvm::sys::getDefaultTargetTriple();

        std::string errorMessage{};
        const llvm::Target* target = llvm::TargetRegistry::lookupTarget(targetTriple, errorMessage);
        if(!target)
        {
            throw Exceptions::CompilerException{ errorMessage, ErrorCodes::EXIT_CODE_LLVM_LOOKUP_TARGET_ERROR };
        }

        llvm::TargetMachine* targetMachine = target->createTargetMachine(targetTriple, cpu, {}, {}, {});

        m_module->setDataLayout(targetMachine->createDataLayout());
        m_module->setTargetTriple(targetTriple);

        std::error_code errorCode;
        llvm::raw_fd_ostream ofstream(outputFilePath.string(), errorCode, llvm::sys::fs::OF_None);
        if(errorCode)
        {
            throw Exceptions::CompilerException{ fmt::format("Could not open file: {}", errorCode.message()), ErrorCodes::EXIT_CODE_IO_ERROR };
        }

        llvm::legacy::PassManager pass;
        static constexpr llvm::CodeGenFileType fileType = llvm::CGFT_ObjectFile;

        if(targetMachine->addPassesToEmitFile(pass, ofstream, nullptr, fileType))
        {
            throw Exceptions::CompilerException{ fmt::format("TargetMachine ({}) can't emit a object file type", targetMachine->getTargetCPU()), ErrorCodes::EXIT_CODE_LLVM_OBJECT_FILE_NOT_SUPPORTED };
        }

        pass.run(*m_module);
        ofstream.flush();
    }

    std::unique_ptr<llvm::Module>& TranslationUnit::module()
    {
        return m_module;
    }

    llvm::IRBuilder<>& TranslationUnit::builder()
    {
        return m_builder;
    }

    void TranslationUnit::print(std::ostream& outputStream)
    {
        std::string data;
        llvm::raw_string_ostream llvmStream = llvm::raw_string_ostream{ data };
        m_module->print(llvmStream, nullptr);

        outputStream << data;
    }

} // namespace AeroJet::Compiler::LLVM