#
# CMakeLists.txt
# Copyright © 2024 AeroJet Developers. All Rights Reserved.
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


option(AEROJET_SHARED "Build dynamic AeroJet shared lib" OFF)
option(AEROJET_ENABLE_ASSERTIONS "Enable assertions" ON)

if(AEROJET_SHARED)
    set(AEROJET_LIB_TYPE SHARED)
else()
    set(AEROJET_LIB_TYPE STATIC)
endif()

add_library(AeroJet ${AEROJET_LIB_TYPE}
        include/AeroJet.hpp
        include/Assertion.hpp
        include/Types.hpp
        include/Formatters/Formatters.hpp
        include/Formatters/ClassInfo/AccessFlags.hpp
        include/Formatters/FieldInfo/AccessFlags.hpp
        include/Java/Archive/Jar.hpp
        source/Java/Archive/Jar.cpp
        include/Java/ByteCode/Instruction.hpp
        source/Java/ByteCode/Instruction.cpp
        include/Java/ByteCode/OpCodes.hpp
        include/Java/ClassFile/ClassInfo.hpp
        source/Java/ClassFile/ClassInfo.cpp
        include/Java/ClassFile/ConstantPool.hpp
        include/Java/ClassFile/ConstantPoolEntry.hpp
        source/Java/ClassFile/ConstantPoolEntry.cpp
        include/Java/ClassFile/FieldDescriptor.hpp
        source/Java/ClassFile/FieldDescriptor.cpp
        include/Java/ClassFile/FieldInfo.hpp
        source/Java/ClassFile/FieldInfo.cpp
        include/Java/ClassFile/MethodDescriptor.hpp
        source/Java/ClassFile/MethodDescriptor.cpp
        include/Java/ClassFile/MethodInfo.hpp
        source/Java/ClassFile/MethodInfo.cpp
        include/Java/ClassFile/Attributes/Annotation/Annotation.hpp
        source/Java/ClassFile/Attributes/Annotation/Annotation.cpp
        include/Java/ClassFile/Attributes/Annotation/ElementValue.hpp
        source/Java/ClassFile/Attributes/Annotation/ElementValue.cpp
        include/Java/ClassFile/Attributes/Annotation/ElementValuePair.hpp
        source/Java/ClassFile/Attributes/Annotation/ElementValuePair.cpp
        include/Java/ClassFile/Attributes/Annotation/ParameterAnnotation.hpp
        source/Java/ClassFile/Attributes/Annotation/ParameterAnnotation.cpp
        include/Java/ClassFile/Attributes/Annotation/TypeAnnotation.hpp
        source/Java/ClassFile/Attributes/Annotation/TypeAnnotation.cpp
        include/Java/ClassFile/Attributes/AnnotationDefault.hpp
        source/Java/ClassFile/Attributes/AnnotationDefault.cpp
        include/Java/ClassFile/Attributes/Attribute.hpp
        source/Java/ClassFile/Attributes/Attribute.cpp
        include/Java/ClassFile/Attributes/AttributeInfo.hpp
        source/Java/ClassFile/Attributes/AttributeInfo.cpp
        include/Java/ClassFile/Attributes/BootstrapMethods.hpp
        source/Java/ClassFile/Attributes/BootstrapMethods.cpp
        include/Java/ClassFile/Attributes/Code.hpp
        source/Java/ClassFile/Attributes/Code.cpp
        include/Java/ClassFile/Attributes/EnclosingMethod.hpp
        source/Java/ClassFile/Attributes/EnclosingMethod.cpp
        include/Java/ClassFile/Attributes/Exceptions.hpp
        source/Java/ClassFile/Attributes/Exceptions.cpp
        include/Java/ClassFile/Attributes/ConstantValue.hpp
        source/Java/ClassFile/Attributes/ConstantValue.cpp
        include/Java/ClassFile/Attributes/Deprecated.hpp
        source/Java/ClassFile/Attributes/Deprecated.cpp
        include/Java/ClassFile/Attributes/InnerClasses.hpp
        source/Java/ClassFile/Attributes/InnerClasses.cpp
        include/Java/ClassFile/Attributes/LineNumberTable.hpp
        source/Java/ClassFile/Attributes/LineNumberTable.cpp
        include/Java/ClassFile/Attributes/LocalVariableTable.hpp
        source/Java/ClassFile/Attributes/LocalVariableTable.cpp
        include/Java/ClassFile/Attributes/LocalVariableTypeTable.hpp
        source/Java/ClassFile/Attributes/LocalVariableTypeTable.cpp
        include/Java/ClassFile/Attributes/MethodParameters.hpp
        source/Java/ClassFile/Attributes/MethodParameters.cpp
        include/Java/ClassFile/Attributes/RuntimeInvisibleAnnotations.hpp
        source/Java/ClassFile/Attributes/RuntimeInvisibleAnnotations.cpp
        include/Java/ClassFile/Attributes/RuntimeInvisibleParameterAnnotations.hpp
        source/Java/ClassFile/Attributes/RuntimeInvisibleParameterAnnotations.cpp
        include/Java/ClassFile/Attributes/RuntimeVisibleAnnotations.hpp
        source/Java/ClassFile/Attributes/RuntimeVisibleAnnotations.cpp
        include/Java/ClassFile/Attributes/RuntimeVisibleParameterAnnotations.hpp
        source/Java/ClassFile/Attributes/RuntimeVisibleParameterAnnotations.cpp
        include/Java/ClassFile/Attributes/RuntimeVisibleTypeAnnotations.hpp
        source/Java/ClassFile/Attributes/RuntimeVisibleTypeAnnotations.cpp
        include/Java/ClassFile/Attributes/RuntimeInvisibleTypeAnnotations.hpp
        source/Java/ClassFile/Attributes/RuntimeInvisibleTyperAnnotations.cpp
        include/Java/ClassFile/Attributes/Signature.hpp
        source/Java/ClassFile/Attributes/Signature.cpp
        include/Java/ClassFile/Attributes/SourceDebugExtension.hpp
        source/Java/ClassFile/Attributes/SourceDebugExtension.cpp
        include/Java/ClassFile/Attributes/SourceFile.hpp
        source/Java/ClassFile/Attributes/SourceFile.cpp
        include/Java/ClassFile/Attributes/StackMapTable.hpp
        source/Java/ClassFile/Attributes/StackMapTable.cpp
        include/Java/ClassFile/Attributes/Synthetic.hpp
        source/Java/ClassFile/Attributes/Synthetic.cpp
        include/Java/ClassFile/Utils/AttributeInfoUtils.hpp
        source/Java/ClassFile/Utils/AttributeInfoUtils.cpp
        include/Java/ClassFile/Utils/ClassInfoUtils.hpp
        source/Java/ClassFile/Utils/ClassInfoUtils.cpp
        include/Java/ClassFile/Utils/ConstantPoolEntryUtils.hpp
        source/Java/ClassFile/Utils/ConstantPoolEntryUtils.cpp
        include/Exceptions/FileNotFoundException.hpp
        source/Exceptions/FileNotFoundException.cpp
        include/Exceptions/IncorrectAttributeTypeException.hpp
        source/Exceptions/IncorrectAttributeTypeException.cpp
        include/Exceptions/OperationNotSupportedException.hpp
        source/Exceptions/OperationNotSupportedException.cpp
        include/Exceptions/RuntimeException.hpp
        source/Exceptions/RuntimeException.cpp
        include/Stream/JavaClassStream.hpp
        include/Stream/StandardStreamWrapper.hpp
        include/Utils/StringUtils.hpp
)

target_include_directories(AeroJet PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(AeroJet PUBLIC
        fmt
        backward
        zip::zip
)

if (AEROJET_ENABLE_ASSERTIONS)
        target_compile_definitions(AeroJet PUBLIC
                AEROJET_ENABLE_ASSERTIONS
        )
endif()
