#
# CMakeLists.txt
# Copyright © 2023 AeroJet Developers. All Rights Reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the “Software”), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
# OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

add_executable(AeroJetCompiler
        include/ClassPath.hpp
        source/ClassPath.cpp
        include/Environment.hpp
        source/Environment.cpp
        include/ErrorCodes.hpp
        include/CodeGen/LLVM/IRCodeGen.hpp
        source/CodeGen/LLVM/IRCodeGen.cpp
        include/CodeGen/LLVM/TranslationUnit.hpp
        source/CodeGen/LLVM/TranslationUnit.cpp
        main.cpp
)

target_include_directories(AeroJetCompiler PUBLIC
        include
        ${LLVM_INCLUDE_DIRS}
)

target_link_libraries(AeroJetCompiler PUBLIC
        AeroJet
        cxxopts
        fmt
        spdlog
        LLVMCore
        LLVMTransformUtils
)
