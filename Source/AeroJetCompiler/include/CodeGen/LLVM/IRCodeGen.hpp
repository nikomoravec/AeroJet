/*
 * IRCodeGen.hpp
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

#pragma once

#include "AeroJet.hpp"
#include "Environment.hpp"
#include "TranslationUnit.hpp"

#include <llvm/IR/IRBuilder.h>
#include <memory>

namespace AeroJet::Compiler::LLVM
{
    class IRCodeGen final
    {
      public:
        explicit IRCodeGen(Environment environment);

        TranslationUnit translate(const Java::ClassFile::ClassInfo& classInfo);
        i4 run();

        static llvm::LLVMContext& llvmContext();

      private:
        Environment m_environment;
        static llvm::LLVMContext s_llvmContext;
    };
} // namespace AeroJet::Compiler::LLVM
