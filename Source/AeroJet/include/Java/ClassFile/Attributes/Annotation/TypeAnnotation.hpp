/*
 * TypeAnnotation.hpp
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

#include "Java/ClassFile/Attributes/Annotation/ElementValue.hpp"
#include "Java/ClassFile/Attributes/Annotation/ElementValuePair.hpp"
#include "Stream/JavaClassStream.hpp"

#include <variant>
#include <vector>

namespace AeroJet::Java::ClassFile
{
    /**
     * The type_parameter_target item indicates that an annotation appears on the declaration of the i'th type parameter
     * of a generic class, generic interface, generic method, or generic constructor.
     */
    class TypeParameterTarget
    {
      public:
        explicit TypeParameterTarget(u1 typeParameterIndex);

        /**
         * The value of the type_parameter_index item specifies which type parameter declaration is annotated.
         * A type_parameter_index value of 0 specifies the first type parameter declaration.
         */
        [[nodiscard]] u1 typeParameterIndex() const;

        template<typename T>
        [[nodiscard]] static TypeParameterTarget read(Stream::JavaClassStream<T>& stream)
        {
            const u1 typeParameterIndex = stream.template read<u1>();
            return AeroJet::Java::ClassFile::TypeParameterTarget{ typeParameterIndex };
        }

      private:
        u1 m_typeParameterIndex;
    };

    /**
     * The supertype_target item indicates that an annotation appears on a type in the extends or implements clause
     * of a class or interface declaration.
     */
    class SuperTypeTarget
    {
      public:
        explicit SuperTypeTarget(u2 superTypeIndex);

        /**
         * A supertype_index value of 65535 specifies that the annotation appears on the superclass in an extends
         * clause of a class declaration.
         *
         * Any other supertype_index value is an index into the interfaces array of the enclosing ClassFile structure,
         * and specifies that the annotation appears on that superinterface in either the implements clause of a class
         * declaration or the extends clause of an interface declaration.
         */
        [[nodiscard]] u2 superTypeIndex() const;

        template<typename T>
        [[nodiscard]] static SuperTypeTarget read(Stream::JavaClassStream<T>& stream)
        {
            const u2 superTypeIndex = stream.template read<u2>();
            return AeroJet::Java::ClassFile::SuperTypeTarget{ superTypeIndex };
        }

      private:
        u2 m_superTypeIndex;
    };

    /**
     * The type_parameter_bound_target item indicates that an annotation appears on the i'th bound of the j'th type
     * parameter declaration of a generic class, interface, method, or constructor.
     */
    class TypeParameterBoundTarget
    {
      public:
        TypeParameterBoundTarget(u1 typeParameterIndex, u1 boundIndex);

        /**
         * The value of the of type_parameter_index item specifies which type parameter declaration has an annotated
         * bound.
         *
         * A type_parameter_index value of 0 specifies the first type parameter declaration.
         */
        [[nodiscard]] u1 typeParameterIndex() const;

        /**
         * The value of the bound_index item specifies which bound of the type parameter declaration indicated by
         * type_parameter_index is annotated. A bound_index value of 0 specifies the first bound of
         * a type parameter declaration.
         */
        [[nodiscard]] u1 boundIndex() const;

        template<typename T>
        [[nodiscard]] static TypeParameterBoundTarget read(Stream::JavaClassStream<T>& stream)
        {
            const u1 typeParameterIndex = stream.template read<u1>();
            const u1 boundIndex = stream.template read<u1>();

            return AeroJet::Java::ClassFile::TypeParameterBoundTarget{ typeParameterIndex, boundIndex };
        }

      private:
        u1 m_typeParameterIndex;
        u1 m_boundIndex;
    };

    /**
     * The empty_target item indicates that an annotation appears on either the type in a field declaration,
     * the return type of a method, the type of a newly constructed object, or the receiver type of a
     * method or constructor.
     */
    class EmptyTarget
    {
    };

    /**
     * The formal_parameter_target item indicates that an annotation appears on the type in a formal parameter
     * declaration of a method, constructor, or lambda expression.
     */
    class FormalParameterTarget
    {
      public:
        explicit FormalParameterTarget(u1 formalParameterIndex);

        /**
         * The value of the formal_parameter_index item specifies which formal parameter declaration has an annotated
         * type. A formal_parameter_index value of 0 specifies the first formal parameter declaration.
         *
         * The formal_parameter_target item records that a formal parameter's type is annotated, but does not record
         * the type itself. The type may be found by inspecting the method descriptor (§4.3.3) of
         * the method_info structure enclosing the RuntimeVisibleTypeAnnotations attribute. A formal_parameter_index
         * value of 0 indicates the first parameter descriptor in the method descriptor.
         */
        [[nodiscard]] u1 formalParameterIndex() const;

        template<typename T>
        [[nodiscard]] static FormalParameterTarget read(Stream::JavaClassStream<T>& stream)
        {
            const u1 formalParameterIndex = stream.template read<u1>();
            return AeroJet::Java::ClassFile::FormalParameterTarget{ formalParameterIndex };
        }

      private:
        u1 m_formalParameterIndex;
    };

    /**
     *  The throws_target item indicates that an annotation appears on the i'th type in the throws clause of a
     *  method or constructor declaration.
     */
    class ThrowsTarget
    {
      public:
        explicit ThrowsTarget(u2 throwsTypeIndex);

        /**
         * The value of the throws_type_index item is an index into the exception_index_table array of the
         * Exceptions attribute of the method_info structure enclosing the RuntimeVisibleTypeAnnotations attribute.
         */
        [[nodiscard]] u2 throwsTypeIndex() const;

        template<typename T>
        [[nodiscard]] static ThrowsTarget read(Stream::JavaClassStream<T>& stream)
        {
            const u1 throwsTypeIndex = stream.template read<u2>();
            return AeroJet::Java::ClassFile::ThrowsTarget{ throwsTypeIndex };
        }

      private:
        u2 m_throwsTypeIndex;
    };

    /**
     * The localvar_target item indicates that an annotation appears on the type in a local variable declaration,
     * including a variable declared as a resource in a try-with-resources statement.
     */
    class LocalVarTarget
    {
      public:
        /**
         * Each entry indicates a range of code array offsets within which a local variable has a value.
         * It also indicates the index into the local variable array of the current frame at which that local
         * variable can be found.
         */
        class TableEntry
        {
          public:
            TableEntry(u2 startPc, u2 length, u2 index);

            /**
             * The given local variable has a value at indices into the code array in the interval
             * [start_pc, start_pc + length), that is, between start_pc inclusive and start_pc + length exclusive.
             */
            [[nodiscard]] u2 startPc() const;

            [[nodiscard]] u2 length() const;

            /**
             * The given local variable must be at index in the local variable array of the current frame.
             *
             * If the local variable at index is of type double or long, it occupies both index and index + 1.
             */
            [[nodiscard]] u2 index() const;

            template<typename T>
            [[nodiscard]] static TableEntry read(Stream::JavaClassStream<T>& stream)
            {
                const u2 startPc = stream.template read<u2>();
                const u2 length = stream.template read<u2>();
                const u2 index = stream.template read<u2>();

                return { startPc, length, index };
            }

          private:
            u2 m_startPc;
            u2 m_length;
            u2 m_index;
        };

        explicit LocalVarTarget(std::vector<TableEntry> table);

        [[nodiscard]] u2 tableLength() const;

        [[nodiscard]] const std::vector<TableEntry>& table() const;

        template<typename T>
        [[nodiscard]] static LocalVarTarget read(Stream::JavaClassStream<T>& stream)
        {
            const u2 tableLength = stream.template read<u2>();
            std::vector<LocalVarTarget::TableEntry> table = stream.template readSome<LocalVarTarget::TableEntry>(tableLength);

            return AeroJet::Java::ClassFile::LocalVarTarget{ table };
        }

      private:
        std::vector<TableEntry> m_table;
    };

    /**
     * The catch_target item indicates that an annotation appears on the i'th type in an exception parameter
     * declaration.
     */
    class CatchTarget
    {
      public:
        explicit CatchTarget(u2 exceptionTableIndex);

        /**
         * The value of the exception_table_index item is an index into the exception_table array of the Code
         * attribute enclosing the RuntimeVisibleTypeAnnotations attribute.
         *
         * The possibility of more than one type in an exception parameter declaration arises from the multi-catch
         * clause of the try statement, where the type of the exception parameter is a union of types (JLS §14.20).
         * A compiler usually creates one exception_table entry for each type in the union, which allows the
         * catch_target item to distinguish them. This preserves the correspondence between a type and its annotations.
         */
        [[nodiscard]] u2 exceptionTableIndex() const;

        template<typename T>
        [[nodiscard]] static CatchTarget read(Stream::JavaClassStream<T>& stream)
        {
            const u2 exceptionTableIndex = stream.template read<u2>();
            return AeroJet::Java::ClassFile::CatchTarget{ exceptionTableIndex };
        }

      private:
        u2 m_exceptionTableIndex;
    };

    /**
     * The offset_target item indicates that an annotation appears on either the type in an instanceof expression or a
     * new expression, or the type before the :: in a method reference expression.
     */
    class OffsetTarget
    {
      public:
        explicit OffsetTarget(u2 offset);

        /**
         * The value of the offset item specifies the code array offset of either the instanceof bytecode instruction
         * corresponding to the instanceof expression, the new bytecode instruction corresponding to the new expression,
         * or the bytecode instruction corresponding to the method reference expression.
         */
        [[nodiscard]] u2 offset() const;

        template<typename T>
        [[nodiscard]] static OffsetTarget read(Stream::JavaClassStream<T>& stream)
        {
            const u2 offset = stream.template read<u2>();
            return AeroJet::Java::ClassFile::OffsetTarget{ offset };
        }

      private:
        u2 m_offset;
    };

    /**
     * The type_argument_target item indicates that an annotation appears either on the i'th type in a cast expression,
     * or on the i'th type argument in the explicit type argument list for any of the following: a new expression,
     * an explicit constructor invocation statement, a method invocation expression, or a method reference expression.
     */
    class TypeArgumentTarget
    {
      public:
        TypeArgumentTarget(u2 offset, u1 typeArgumentIndex);

        /**
         * The value of the offset item specifies the code array offset of either the bytecode instruction corresponding
         * to the cast expression, the new bytecode instruction corresponding to the new expression, the bytecode
         * instruction corresponding to the explicit constructor invocation statement, the bytecode instruction
         * corresponding to the method invocation expression, or the bytecode instruction corresponding to the
         * method reference expression.
         *
         * For a cast expression, the value of the type_argument_index item specifies which type in the cast operator
         * is annotated. A type_argument_index value of 0 specifies the first (or only) type in the cast operator.
         *
         * The possibility of more than one type in a cast expression arises from a cast to an intersection type.
         */
        [[nodiscard]] u2 offset() const;

        /**
         * For an explicit type argument list, the value of the type_argument_index item specifies which type argument
         * is annotated. A type_argument_index value of 0 specifies the first type argument.
         */
        [[nodiscard]] u1 typeArgumentIndex() const;

      private:
        u2 m_offset;
        u1 m_typeArgumentIndex;
    };

    using TargetInfo = std::variant<TypeParameterTarget,
                                    SuperTypeTarget,
                                    TypeParameterBoundTarget,
                                    EmptyTarget,
                                    FormalParameterTarget,
                                    ThrowsTarget,
                                    LocalVarTarget,
                                    CatchTarget,
                                    OffsetTarget,
                                    TypeArgumentTarget>;

    /**
     * Wherever a type is used in a declaration or expression, the type_path structure identifies which part of the
     * type is annotated. An annotation may appear on the type itself, but if the type is a reference type,
     * then there are additional locations where an annotation may appear:
     *
     * If an array type T[] is used in a declaration or expression, then an annotation may appear on any component
     * type of the array type, including the element type.
     *
     * If a nested type T1.T2 is used in a declaration or expression, then an annotation may appear on the
     * name of the top level type or any member type.
     *
     * If a parameterized type T<A> or T<? extends A> or T<? super A> is used in a declaration or expression,
     * then an annotation may appear on any type argument or on the bound of any wildcard type argument.
     */
    class TypePath
    {
      public:
        /**
         * The legal values for the type_path_kind item are listed in Table 4.7.20.2-A.
         *
         * Table 4.7.20.2-A. Interpretation of type_path_kind values
         *
         * Value	Interpretation
         * 0	    Annotation is deeper in an array type
         * 1	    Annotation is deeper in a nested type
         * 2	    Annotation is on the bound of a wildcard type argument of a parameterized type
         * 3	    Annotation is on a type argument of a parameterized type
         * */
        enum class TypePathKind : u1
        {
            DEEPER_IN_ARRAY_TYPE = 0,
            DEEPER_IN_NESTED_TYPE = 1,
            ON_THE_MOUND_OF_A_WILDCARD = 2,
            ON_A_TYPE_ARGUMENT_OF_A_PARAMETERIZED_TYPE = 3
        };

        class Path
        {
          public:
            Path(TypePathKind typePathKind, u1 typeArgumentIndex);

            [[nodiscard]] TypePathKind typePathKind() const;

            /**
             * If the value of the type_path_kind item is 0, 1, or 2, then the value of the type_argument_index item is
             * 0.
             *
             * If the value of the type_path_kind item is 3, then the value of the type_argument_index item specifies
             * which type argument of a parameterized type is annotated, where 0 indicates the first type argument of
             * a parameterized type.
             */
            [[nodiscard]] u1 typeArgumentIndex() const;

          private:
            TypePathKind m_typePathKind;
            u1 m_typeArgumentIndex;
        };

        explicit TypePath(std::vector<Path> path);

        /**
         * The value of the path_length item gives the number of entries in the path array:
         *
         *  If the value of path_length is 0, then the annotation appears directly on the type itself.
         *
         *   If the value of path_length is non-zero, then each entry in the path array represents an iterative,
         *   left-to-right step towards the precise location of the annotation in an array type, nested type,
         *   or parameterized type. (In an array type, the iteration visits the array type itself, then its
         *   component type, then the component type of that component type, and so on, until the element
         *   type is reached.)
         */
        [[nodiscard]] u1 pathLength() const;

        [[nodiscard]] const std::vector<Path>& path() const;

      private:
        std::vector<Path> m_path;
    };

    class TypeAnnotation
    {
      public:
        TypeAnnotation(u1 targetType,
                       TargetInfo targetInfo,
                       TypePath targetPath,
                       u2 typeIndex,
                       const std::vector<ElementValuePair>& elementValuePairs);

        /**
         * The value of the target_type item denotes the kind of target on which the annotation appears.
         * The various kinds of target correspond to the type contexts of the Java programming language where types
         * are used in declarations and expressions (JLS §4.11).
         *
         * The legal values of target_type are specified in Table 4.7.20-A and Table 4.7.20-B. Each value is a one-byte
         * tag indicating which item of the target_info union follows the target_type item to give more information
         * about the target.
         *
         * The kinds of target in Table 4.7.20-A and Table 4.7.20-B correspond to the type contexts in JLS §4.11.
         * Namely, target_type values 0x10-0x17 and 0x40-0x42 correspond to type contexts 1-10, while target_type
         * values 0x43-0x4B correspond to type contexts 11-16.
         *
         * The value of the target_type item determines whether the type_annotation structure appears in a
         * RuntimeVisibleTypeAnnotations attribute in a ClassFile structure, a field_info structure, a method_info
         * structure, or a Code attribute. Table 4.7.20-C gives the location of the RuntimeVisibleTypeAnnotations
         * attribute for a type_annotation structure with each legal target_type value.
         */
        [[nodiscard]] u1 targetType() const;

        /**
         * The value of the target_info item denotes precisely which type in a declaration or expression is annotated.
         * The items of the target_info union are specified in §4.7.20.1.
         */
        [[nodiscard]] const TargetInfo& targetInfo() const;

        /**
         * The value of the target_path item denotes precisely which part of the type indicated by target_info is
         * annotated.
         *
         * The format of the type_path structure is specified in §4.7.20.2.
         */
        [[nodiscard]] const TypePath& targetPath() const;

        /**
         * type_index, num_element_value_pairs, element_value_pairs[]
         *
         * The meaning of these items in the type_annotation structure is the same as their meaning in
         * the annotation structure (§4.7.16).
         */
        [[nodiscard]] u2 typeIndex() const;
        [[nodiscard]] u2 numElementValuePairs() const;
        [[nodiscard]] const std::vector<ElementValuePair>& elementValuePairs() const;

      private:
        u1 m_targetType;
        TargetInfo m_targetInfo;
        TypePath m_targetPath;
        u2 m_typeIndex;
        std::vector<ElementValuePair> m_elementValuePairs;
    };
} // namespace AeroJet::Java::ClassFile
