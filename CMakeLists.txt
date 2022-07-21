cmake_minimum_required(VERSION 3.21)
project(SuperJet)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Source/third-party/fmtlib)
add_subdirectory(Source/third-party/backwandcpp)
#add_subdirectory(Source/third-party/llvm/llvm)
add_subdirectory(Source/third-party/cxxopts)
add_subdirectory(Source/third-party/spdlog)
add_subdirectory(Source/third-party/zip)

add_executable(SuperJet
        SuperJet.cpp
        Source/source/Java/Archive/Archive.cpp
        Source/source/Java/Archive/FieldDescriptor.cpp
        Source/source/Java/Archive/MethodDescriptor.cpp
        Source/source/Java/Archive/Attributes/Code.cpp
        Source/source/Java/Archive/Attributes/LineNumberTable.cpp
        Source/source/Compiler/MethodInfoResolver.cpp
        Source/source/Compiler/AttributeInfoResolver.cpp
        Source/source/Compiler/ClassInfoResolver.cpp
        Source/source/Compiler/FieldInfoResolver.cpp
        Source/source/Java/Archive/Attributes/SourceFile.cpp
        Source/source/Java/Archive/Jar.cpp
        Source/source/Compiler/ClassInfoReferenceCollector.cpp
        Source/source/Compiler/Context.cpp
        Source/source/Java/Archive/Attributes/LocalVariableTable.cpp
        Source/source/Java/Archive/ClassInfo.cpp
)

#add_dependencies(SuperJet
#        intrinsics_gen # for LLVM
#)

target_include_directories(SuperJet PUBLIC
        Source/include
        Source/third-party/fmtlib/include
        Source/third-party/backwandcpp
        #${LLVM_BINARY_DIR}/include
        #Source/third-party/llvm/llvm/include
        Source/third-party/cxxopts/include
        Source/third-party/spdlog/include
        Source/third-party/zip/src
        Source/third-party/digraph
)

target_link_libraries(SuperJet PUBLIC
        fmt
        backward
        #LLVMCodeGen
        spdlog
        zip
)
