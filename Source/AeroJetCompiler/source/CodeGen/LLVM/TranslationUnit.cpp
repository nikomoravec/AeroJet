/*
 * TranslationUnit.cpp
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

#include "CodeGen/LLVM/TranslationUnit.hpp"

#include "CodeGen/LLVM/IRCodeGen.hpp"

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
        llvm::raw_string_ostream llvmStream = llvm::raw_string_ostream{data};
        m_module->print(llvmStream, nullptr);

        outputStream << data;
    }
} // namespace AeroJet::Compiler::LLVM