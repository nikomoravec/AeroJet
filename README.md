[![Build](https://github.com/nikitamiroshnichenko/AeroJet/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/nikitamiroshnichenko/AeroJet/actions/workflows/c-cpp.yml)
[![Tests](https://github.com/nikitamiroshnichenko/AeroJet/actions/workflows/run-tests.yml/badge.svg)](https://github.com/nikitamiroshnichenko/AeroJet/actions/workflows/run-tests.yml)

**Aero Jet** - Java AOT Compiler (WIP)

### A bit of history
In year 2019 the Java World has fall into darkness because 
[Excelsior JET](https://en.wikipedia.org/wiki/Excelsior_JET) project has gone 
into nowhere.
Java Community loosed possibility to build native applications from Java code.

Since these days I'm thinking about resurrecting of native 
[AOT](https://en.wikipedia.org/wiki/Ahead-of-time_compilation) 
compilation of Java byte  code

### What is done already
At the beginning AeroJet project aims to build AOT compiler for Java Bytecode up to Java Class Major Version 52 (Java 8).  

According to [JVM specification](https://docs.oracle.com/javase/specs/jvms/se8/html/index.html) libAeroJet is able 
to read the following Java Class Structures:

- [x] [Class File](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.1)
  - [x] [Constant Pool](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.4)
    - [x] [Class Info](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.4.1)
    - [x] [Field Ref](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.4.2)
    - [x] [Method Ref](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.4.2)
    - [x] [Interface Method Ref](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.4.2)
    - [x] [String](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.4.3)
    - [x] [Integer](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.4.4)
    - [x] [Float](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.4.4)
    - [x] [Long](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.4.5)
    - [x] [Double](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.4.5)
    - [x] [Name And Type](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.4.6)
    - [x] [UTF-8](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.4.7)
    - [x] [Method Handle](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.4.8)
    - [x] [Method Type](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.4.9)
    - [x] [Invoke Dynamic](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.4.10)
  - [x] [Fields](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.5)
  - [x] [Methods](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.6)
  - [x] [Attributes](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7)
    - [x] [ConstantValue](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.2)
    - [x] [Code](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.3)
    - [ ] [StackMapTable](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.4)
    - [ ] [Exceptions](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.5)
    - [ ] [InnerClasses](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.6)
    - [ ] [EnclosingMethod](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.7)
    - [ ] [Synthetic](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.8)
    - [ ] [Signature](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.9)
    - [x] [SourceFile](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.10)
    - [ ] [SourceDebugExtension](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.11)
    - [x] [LineNumberTable](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.12)
    - [x] [LocalVariableTable](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.13)
    - [ ] [LocalVariableTypeTable](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.14)
    - [ ] [Deprecated](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.15)
    - [ ] [RuntimeVisibleAnnotations](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.16)
    - [ ] [RuntimeInvisibleAnnotations](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.17)
    - [ ] [RuntimeVisibleParameterAnnotations](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.18)
    - [ ] [RuntimeInvisibleParameterAnnotations](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.19)
    - [ ] [RuntimeVisibleTypeAnnotations](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.20)
    - [ ] [RuntimeInvisibleTypeAnnotations](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.21)
    - [ ] [AnnotationDefault](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.22)
    - [ ] [BootstrapMethods](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.23)
    - [ ] [MethodParameters](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.24)

libAeroJet is also able to read Java classes from JAR archives.

### Build Instructions
1. Clone AeroJet project with dependencies via `git clone --recurse-submodules https://github.com/nikitamiroshnichenko/AeroJet.git`
2. Navigate to the AeroJet folder you just clone `cd AeroJet `
3. Create build folder `mkdir .build`
4. Navigate to the `.build` directory with `cd .build` command
5. Run `cmake ..`
   1. Optionally you can provide additional cmake or AeroJet build options (see [Build Options](#build-options))
6. Build AeroJet with command `cmake --build . --target AeroJet`
    
#### Build Options
`CMAKE_DISABLE_TESTING=ON|OFF` - If set to `ON` - no tests will be included to the project.  
`CMAKE_BUILD_TYPE=Debug|Release|MinSizeRel|RelWithDebInfo` - For more information - see [this page](https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html)
`CMAKE_INSTALL_PREFIX=/my/install/prefix` - - For more information - see [this page](https://cmake.org/cmake/help/latest/variable/CMAKE_INSTALL_PREFIX.html)  
`AEROJET_SHARED=ON|OFF` - If set to `ON` - AeroJet libraries will be compiled as shared libs.

### Example
Here is simple program that prints given java class methods and their bytecode :)

```cpp
#include "AeroJet.hpp"
#include "fmt/format.h"
#include <filesystem>
#include <fstream>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        fmt::print("usage: ./SimpleClassPrinter /path/to/some/java/class");
        return 1;
    }

    std::filesystem::path classFilePath = argv[1];
    std::ifstream fileStream(classFilePath);
    if (!fileStream.is_open())
    {
        fmt::print("Failed to open file '{}'", classFilePath.string());
        return 1;
    }

    auto classInfo = AeroJet::Stream::Reader::read<AeroJet::Java::ClassFile::ClassInfo>(fileStream, AeroJet::Stream::ByteOrder::INVERSE);
    const auto& constantPool = classInfo.constantPool();

    fmt::print("ClassFile {}\n", classFilePath.string());
    fmt::print("\tMethods:\n");
    const std::vector<AeroJet::Java::ClassFile::MethodInfo>& classMethods = classInfo.methods();
    for (const auto& methodInfo : classMethods)
    {
        const std::string methodName = constantPool[methodInfo.nameIndex()].as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString();
        fmt::print("\t\t{}\n", methodName);
        const std::vector<AeroJet::Java::ClassFile::AttributeInfo>& methodAttributes = methodInfo.attributes();
        auto codeAttributeIt = std::find_if(methodAttributes.begin(), methodAttributes.end(),
            [&constantPool](const AeroJet::Java::ClassFile::AttributeInfo& attributeInfo)
            {
                return AeroJet::Java::ClassFile::Utils::AttributeInfoUtils::extractName(constantPool, attributeInfo) == AeroJet::Java::ClassFile::Code::CODE_ATTRIBUTE_NAME;
            });

        if (codeAttributeIt != methodAttributes.end())
        {
            fmt::print("\t\t\tCode:\n");
            AeroJet::Java::ClassFile::Code codeAttribute{constantPool, *codeAttributeIt};
            for (const auto& instruction : codeAttribute.code())
            {
                fmt::print("\t\t\t\t{:#04x}\n", static_cast<AeroJet::u1>(instruction.opCode()));
            }
        }
    }
}
```

For more examples - see [Examples](https://github.com/nikitamiroshnichenko/AeroJet/tree/master/Source/Examples) page!

