/*
 * ClassInfo.hpp
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

#include "Java/ClassFile/Attributes/AttributeInfo.hpp"
#include "Java/ClassFile/ConstantPool.hpp"
#include "Java/ClassFile/FieldInfo.hpp"
#include "Java/ClassFile/MethodInfo.hpp"
#include "Stream/JavaClassStream.hpp"
#include "Types.hpp"

#include <optional>
#include <vector>

namespace AeroJet::Java::ClassFile
{
    class ClassInfo
    {
      public:
        static constexpr u4 JAVA_CLASS_MAGIC = 0xCAFEBABE;

        enum class AccessFlags : u2
        {
            ACC_PUBLIC = 0x0001,
            ACC_FINAL = 0x0010,
            ACC_SUPER = 0x0020,
            ACC_INTERFACE = 0x0200,
            ACC_ABSTRACT = 0x0400,
            ACC_SYNTHETIC = 0x1000,
            ACC_ANNOTATION = 0x2000,
            ACC_ENUM = 0x4000
        };

        ClassInfo(u2 minorVersion,
                  u2 majorVersion,
                  ConstantPool& constantPool,
                  u2 accessFlags,
                  u2 thisClass,
                  std::optional<u2> superClass,
                  const std::vector<u2>& interfaces,
                  const std::vector<FieldInfo>& fields,
                  const std::vector<MethodInfo>& methods,
                  const std::vector<AttributeInfo>& attributes);

        /**
         * The values of the minor_version and major_version items are the minor and major version numbers of this
         * class file. Together, a major and a minor version number determine the version of the class file format.
         * If a class file has major version number M and minor version number m, we denote the version of its class
         * file format as M.m. Thus, class file format versions may be ordered lexicographically,
         * for example, 1.5 < 2.0 < 2.1.
         *
         * A Java Virtual Machine implementation can support a class file format of version v if and only if v lies in
         * some contiguous range Mi.0 ≤ v ≤ Mj.m. The release level of the Java SE platform to which a Java Virtual
         * Machine implementation conforms is responsible for determining the range.
         */
        [[nodiscard]] u2 minorVersion() const;

        [[nodiscard]] u2 majorVersion() const;

        /**
         * The constant_pool is a table of structures (§4.4) representing various string constants, class and interface
         * names, field names, and other constants that are referred to within the ClassFile structure and its
         * substructures. The format of each constant_pool table entry is indicated by its first "tag" byte.
         *
         * The constant_pool table is indexed from 1 to constant_pool_count - 1.
         */
        [[nodiscard]] const ConstantPool& constantPool() const;

        /**
         * The value of the access_flags item is a mask of flags used to denote access permissions to and properties of
         * this class or interface. The interpretation of each flag, when set, is specified in Table 4.1-A.
         * --------------------------------------------------------------------------------------------------------------
         * |Flag Name 	   | Value 	|   Interpretation                                                                  |
         * ----------------|--------|-----------------------------------------------------------------------------------|
         * |ACC_PUBLIC 	   | 0x0001 |	Declared public; may be accessed from outside its package.                      |
         * ----------------|--------|-----------------------------------------------------------------------------------|
         * |ACC_FINAL 	   | 0x0010 |	Declared final; no subclasses allowed.                                          |
         * ----------------|--------|-----------------------------------------------------------------------------------|
         * |ACC_SUPER 	   | 0x0020 |	Treat superclass methods specially when invoked by the invokespecial instruction|
         * ----------------|--------|-----------------------------------------------------------------------------------|
         * |ACC_INTERFACE  | 0x0200 |	Is an interface, not a class.                                                   |
         * ----------------|--------|-----------------------------------------------------------------------------------|
         * |ACC_ABSTRACT   | 0x0400 |	Declared abstract; must not be instantiated.                                    |
         * ----------------|--------|-----------------------------------------------------------------------------------|
         * |ACC_SYNTHETIC  | 0x1000 |	Declared synthetic; not present in the source code.                             |
         * ----------------|--------|-----------------------------------------------------------------------------------|
         * |ACC_ANNOTATION | 0x2000 |	Declared as an annotation type.                                                 |
         * ----------------|--------|-----------------------------------------------------------------------------------|
         * |ACC_ENUM 	   | 0x4000 |	Declared as an enum type.                                                       |
         * ----------------|--------|------------------------------------------------------------------------------------
         * An interface is distinguished by the ACC_INTERFACE flag being set. If the ACC_INTERFACE flag is not set,
         * this class file defines a class, not an interface.
         *
         * If the ACC_INTERFACE flag is set, the ACC_ABSTRACT flag must also be set, and the ACC_FINAL, ACC_SUPER,
         * and ACC_ENUM flags set must not be set.
         *
         * If the ACC_INTERFACE flag is not set, any of the other flags in Table 4.1-A may be set except
         * ACC_ANNOTATION. However, such a class file must not have both its ACC_FINAL and ACC_ABSTRACT
         * flags set (JLS §8.1.1.2).
         *
         * The ACC_SUPER flag indicates which of two alternative semantics is to be expressed by the invokespecial
         * instruction (§invokespecial) if it appears in this class or interface. Compilers to the instruction set of
         * the Java Virtual Machine should set the ACC_SUPER flag. In Java SE 8 and above, the Java Virtual Machine
         * considers the ACC_SUPER flag to be set in every class file, regardless of the actual value of the flag
         * in the class file and the version of the class file.
         *
         * The ACC_SUPER flag exists for backward compatibility with code compiled by older compilers for the Java
         * programming language. In JDK releases prior to 1.0.2, the compiler generated access_flags in which the flag
         * now representing ACC_SUPER had no assigned meaning, and Oracle's Java Virtual Machine implementation
         * ignored the flag if it was set.
         *
         * The ACC_SYNTHETIC flag indicates that this class or interface was generated by a compiler and does
         * not appear in source code.
         *
         * An annotation type must have its ACC_ANNOTATION flag set. If the ACC_ANNOTATION flag is set,
         * the ACC_INTERFACE flag must also be set.
         *
         * The ACC_ENUM flag indicates that this class or its superclass is declared as an enumerated type.
         *
         * All bits of the access_flags item not assigned in Table 4.1-A are reserved for future use.
         * They should be set to zero in generated class files and should be ignored by
         * Java Virtual Machine implementations.
         */
        [[nodiscard]] AccessFlags accessFlags() const;

        /**
         * The value of the this_class item must be a valid index into the constant_pool table.
         * The constant_pool entry at that index must be a CONSTANT_Class_info structure (§4.4.1)
         * representing the class or interface defined by this class file.
         */
        [[nodiscard]] u2 thisClass() const;

        [[nodiscard]] bool isSuperClassPresented() const;

        /**
         * For a class, the value of the super_class item either must be zero or must be a valid index into the
         * constant_pool table. If the value of the super_class item is nonzero, the constant_pool entry at that
         * index must be a CONSTANT_Class_info structure representing the direct superclass of the class defined by
         * this class file. Neither the direct superclass nor any of its superclasses may have the ACC_FINAL
         * flag set in the access_flags item of its ClassFile structure.
         *
         * If the value of the super_class item is zero, then this class file must represent the class Object,
         * the only class or interface without a direct superclass.
         *
         * For an interface, the value of the super_class item must always be a valid index into the constant_pool table.
         * The constant_pool entry at that index must be a CONSTANT_Class_info structure representing the class Object.
         */
        [[nodiscard]] u2 superClass() const;

        /**
         * Each value in the interfaces array must be a valid index into the constant_pool table.
         * The constant_pool entry at each value of interfaces[i], where 0 ≤ i < interfaces_count, must be
         * a CONSTANT_Class_info structure representing an interface that is a direct superinterface of this class
         * or interface type, in the left-to-right order given in the source for the type.
         */
        [[nodiscard]] const std::vector<u2>& interfaces() const;

        /**
         * Each value in the fields table must be a field_info structure (§4.5) giving a complete description of a field
         * in this class or interface. The fields table includes only those fields that are declared by this class or
         * interface. It does not include items representing fields that are inherited from superclasses or superinterfaces.
         */
        [[nodiscard]] const std::vector<FieldInfo>& fields() const;

        /**
         * Each value in the methods table must be a method_info structure (§4.6) giving a complete description of a
         * method in this class or interface. If neither of the ACC_NATIVE and ACC_ABSTRACT flags are set in the
         * access_flags item of a method_info structure, the Java Virtual Machine instructions
         * implementing the method are also supplied.
         *
         * The method_info structures represent all methods declared by this class or interface type, including
         * instance methods, class methods, instance initialization methods (§2.9), and any class or interface
         * initialization method (§2.9). The methods table does not include items representing methods that are
         * inherited from superclasses or superinterfaces.
         */
        [[nodiscard]] const std::vector<MethodInfo>& methods() const;

        /**
         * Each value of the attributes table must be an attribute_info structure (§4.7).
         * The attributes defined by this specification as appearing in the attributes table of a ClassFile
         * structure are listed in Table 4.7-C.
         *
         * The rules concerning attributes defined to appear in the attributes table of a ClassFile structure
         * are given in §4.7.
         *
         * The rules concerning non-predefined attributes in the attributes table of a ClassFile structure
         * are given in §4.7.1.
         */
        [[nodiscard]] const std::vector<AttributeInfo>& attributes() const;

        template<typename T>
        static ClassInfo read(Stream::JavaClassStream<T>& stream)
        {
            static constexpr u2 MAX_JAVA_CLASS_MAJOR_VERSION = 52;

            const u4 magic = stream.template read<u4>();
            AEROJET_VERIFY_THROW(magic == AeroJet::Java::ClassFile::ClassInfo::JAVA_CLASS_MAGIC, Exceptions::RuntimeException, "Not a Java class!");

            const u2 minorVersion = stream.template read<u2>();
            const u2 majorVersion = stream.template read<u2>();
            AEROJET_VERIFY_THROW(majorVersion <= MAX_JAVA_CLASS_MAJOR_VERSION, Exceptions::RuntimeException, fmt::format("Unsupported java class version file {}! Maximum supported version is: {}", majorVersion, MAX_JAVA_CLASS_MAJOR_VERSION));

            const u2 constantPoolSize = stream.template read<u2>();
            Java::ClassFile::ConstantPool constantPool;
            for(std::size_t constantPoolEntryIndex = 1; constantPoolEntryIndex < constantPoolSize; constantPoolEntryIndex++)
            {
                Java::ClassFile::ConstantPoolEntry entry = stream.template read<Java::ClassFile::ConstantPoolEntry>();
                const Java::ClassFile::ConstantPoolInfoTag tag = entry.tag();

                constantPool.insert({ constantPoolEntryIndex, std::move(entry) });

                if(tag == Java::ClassFile::ConstantPoolInfoTag::LONG || tag == Java::ClassFile::ConstantPoolInfoTag::DOUBLE)
                {
                    constantPoolEntryIndex++;
                }
            }

            const u2 accessFlags = stream.template read<u2>();
            const u2 thisClass = stream.template read<u2>();
            const u2 superClass = stream.template read<u2>();

            const u2 interfacesCount = stream.read<u2>();
            std::vector<u2> interfaces = stream.template readSome<u2>(interfacesCount);

            const u2 fieldsCount = stream.template read<u2>();
            std::vector<Java::ClassFile::FieldInfo> fields = stream.template readSome<Java::ClassFile::FieldInfo>(fieldsCount);

            const u2 methodsCount = stream.template read<u2>();
            std::vector<Java::ClassFile::MethodInfo> methods = stream.template readSome<Java::ClassFile::MethodInfo>(methodsCount);

            const u2 attributesCount = stream.template read<u2>();
            std::vector<Java::ClassFile::AttributeInfo> attributes = stream.template readSome<Java::ClassFile::AttributeInfo>(attributesCount);

            return {
                minorVersion,
                majorVersion,
                constantPool,
                accessFlags,
                thisClass,
                superClass == 0 ? std::nullopt : std::optional<u2>(superClass),
                interfaces,
                fields,
                methods,
                attributes
            };
        }

      protected:
        u2 m_minorVersion;
        u2 m_majorVersion;
        ConstantPool m_constantPool;
        u2 m_accessFlags;
        u2 m_thisClass;
        std::optional<u2> m_superClass;
        std::vector<u2> m_interfaces;
        std::vector<FieldInfo> m_fields;
        std::vector<MethodInfo> m_methods;
        std::vector<AttributeInfo> m_attributes;
    };
} // namespace AeroJet::Java::ClassFile

inline bool operator&(AeroJet::Java::ClassFile::ClassInfo::AccessFlags first, AeroJet::Java::ClassFile::ClassInfo::AccessFlags second)
{
    return static_cast<AeroJet::u2>(first) & static_cast<AeroJet::u2>(second);
}
