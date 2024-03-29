/*
 * StackMapTable.hpp
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

#include "Java/ClassFile/Attributes/Attribute.hpp"

#include <variant>
#include <vector>

namespace AeroJet::Java::ClassFile
{
    enum class VerificationTypeTag : u1
    {
        /**
         * indicates that the local variable has the verification type top
         */
        ITEM_TOP = 0,

        /**
         * indicates that the location has the verification type int
         */
        ITEM_INTEGER = 1,

        /**
         * indicates that the location has the verification type float
         */
        ITEM_FLOAT = 2,

        /**
         * indicates that the location has the verification type null
         */
        ITEM_NULL = 5,

        /**
         * indicates that the location has the verification type uninitializedThis
         */
        ITEM_UNINITIALIZED_THIS = 6,

        /**
         * indicates that the location has the verification type which is the class represented by the
         * CONSTANT_Class_info structure (§4.4.1) found in the constant_pool table at the index given by cpool_index
         */
        ITEM_OBJECT = 7,

        /**
         * indicates that the location has the verification type uninitialized(Offset).
         * The Offset item indicates the offset, in the code array of the Code attribute that contains this
         * StackMapTable attribute, of the new instruction (§new) that created the object being stored in the
         * location.
         */
        ITEM_UNINITIALIZED = 8,

        /**
         * indicates that the first of two locations has the verification type long
         */
        ITEM_LONG = 4,

        /**
         * indicates that the first of two locations has the verification type double
         */
        ITEM_DOUBLE = 3
    };

    class TopVariableInfo
    {
      public:
        TopVariableInfo();

        [[nodiscard]] VerificationTypeTag tag() const;

      private:
        VerificationTypeTag m_tag;
    };

    class IntegerVariableInfo
    {
      public:
        IntegerVariableInfo();

        [[nodiscard]] VerificationTypeTag tag() const;

      private:
        VerificationTypeTag m_tag;
    };

    class FloatVariableInfo
    {
      public:
        FloatVariableInfo();

        [[nodiscard]] VerificationTypeTag tag() const;

      private:
        VerificationTypeTag m_tag;
    };

    class NullVariableInfo
    {
      public:
        NullVariableInfo();

        [[nodiscard]] VerificationTypeTag tag() const;

      private:
        VerificationTypeTag m_tag;
    };

    class UninitializedThisVariableInfo
    {
      public:
        UninitializedThisVariableInfo();

        [[nodiscard]] VerificationTypeTag tag() const;

      private:
        VerificationTypeTag m_tag;
    };

    class ObjectVariableInfo
    {
      public:
        explicit ObjectVariableInfo(u2 constantPoolIndex);

        [[nodiscard]] VerificationTypeTag tag() const;
        [[nodiscard]] u2 constantPoolIndex() const;

      private:
        VerificationTypeTag m_tag;
        u2 m_constantPoolIndex;
    };

    class UninitializedVariableInfo
    {
      public:
        explicit UninitializedVariableInfo(u2 offset);

        [[nodiscard]] VerificationTypeTag tag() const;
        [[nodiscard]] u2 offset() const;

      private:
        u2 m_offset;
        VerificationTypeTag m_tag;
    };

    class LongVariableInfo
    {
      public:
        LongVariableInfo();

        [[nodiscard]] VerificationTypeTag tag() const;

      private:
        VerificationTypeTag m_tag;
    };

    class DoubleVariableInfo
    {
      public:
        DoubleVariableInfo();

        [[nodiscard]] VerificationTypeTag tag();

      private:
        VerificationTypeTag m_tag;
    };

    using VerificationTypeInfo = std::variant<TopVariableInfo,
                                              IntegerVariableInfo,
                                              FloatVariableInfo,
                                              LongVariableInfo,
                                              DoubleVariableInfo,
                                              NullVariableInfo,
                                              UninitializedThisVariableInfo,
                                              ObjectVariableInfo,
                                              UninitializedVariableInfo>;

    /**
     * The frame type same_frame is represented by tags in the range [0-63]. This frame type indicates that the frame
     * has exactly the same local variables as the previous frame and that the operand stack is empty. The offset_delta
     * value for the frame is the value of the tag item, frame_type.
     */
    class SameFrame
    {
      public:
        static constexpr u1 SAME_FRAME_MIN_TAG_VALUE = 0;
        static constexpr u1 SAME_FRAME_MAX_TAG_VALUE = 63;

        explicit SameFrame(u1 frameType);

        [[nodiscard]] u1 frameType() const;

      private:
        u1 m_frameType;
    };

    /**
     * The frame type same_locals_1_stack_item_frame is represented by tags in the range [64, 127].
     * This frame type indicates that the frame has exactly the same local variables as the previous frame and that
     * the operand stack has one entry. The offset_delta value for the frame is given by the formula frame_type - 64.
     * The verification type of the one stack entry appears after the frame type.
     */
    class SameLocals1StackItemFrame
    {
      public:
        static constexpr u1 SAME_LOCALS_1_STACK_ITEM_MIN_TAG_VALUE = 64;
        static constexpr u1 SAME_LOCALS_1_STACK_ITEM_MAX_TAG_VALUE = 127;

        explicit SameLocals1StackItemFrame(u1 frameType, VerificationTypeInfo stack);

        [[nodiscard]] u1 frameType() const;
        [[nodiscard]] VerificationTypeInfo stack() const;
        [[nodiscard]] u2 offsetDelta() const;

      private:
        u1 m_frameType;
        VerificationTypeInfo m_stack;
    };

    /**
     * The frame type same_locals_1_stack_item_frame_extended is represented by the tag 247.
     * This frame type indicates that the frame has exactly the same local variables as the previous frame and that the
     * operand stack has one entry. The offset_delta value for the frame is given explicitly, unlike in the frame
     * type same_locals_1_stack_item_frame. The verification type of the one stack entry appears after offset_delta.
     */
    class SameLocals1StackItemFrameExtended
    {
      public:
        static constexpr u1 SAME_LOCALS_1_STACK_ITEM_EXTENDED_TAG_VALUE = 247;

        SameLocals1StackItemFrameExtended(u2 offsetDelta, VerificationTypeInfo stack);

        [[nodiscard]] u1 frameType() const;
        [[nodiscard]] u2 offsetDelta() const;
        [[nodiscard]] VerificationTypeInfo stack() const;

      private:
        u2 m_offsetDelta;
        VerificationTypeInfo m_stack;
    };

    /**
     * The frame type chop_frame is represented by tags in the range [248-250]. This frame type indicates that the
     * frame has the same local variables as the previous frame except that the last k local variables are absent,
     * and that the operand stack is empty. The value of k is given by the formula 251 - frame_type. The offset_delta
     * value for the frame is given explicitly.
     */
    class ChopFrame
    {
      public:
        static constexpr u1 CHOP_FRAME_MIN_TAG_VALUE = 248;
        static constexpr u1 CHOP_FRAME_MAX_TAG_VALUE = 250;

        ChopFrame(u1 frameType, u2 offsetDelta);

        [[nodiscard]] u1 frameType() const;
        [[nodiscard]] u2 offsetDelta() const;

      private:
        u1 m_frameType;
        u2 m_offsetDelta;
    };

    class SameFrameExtended
    {
      public:
        static constexpr u1 SAME_FRAME_EXTENDED_TAG_VALUE = 251;

        explicit SameFrameExtended(u2 offsetDelta);

        [[nodiscard]] u1 frameType() const;
        [[nodiscard]] u2 offsetDelta() const;

      private:
        u2 m_offsetDelta;
    };

    /**
     * The frame type append_frame is represented by tags in the range [252-254]. This frame type indicates that the
     * frame has the same locals as the previous frame except that k additional locals are defined, and that the operand
     * stack is empty. The value of k is given by the formula frame_type - 251. The offset_delta value for the frame
     * is given explicitly.
     */
    class AppendFrame
    {
      public:
        static constexpr u1 APPEND_FRAME_MIN_TAG_VALUE = 252;
        static constexpr u1 APPEND_FRAME_MAX_TAG_VALUE = 254;

        AppendFrame(u1 frameType, u2 offsetDelta, std::vector<VerificationTypeInfo> locals);

        [[nodiscard]] u1 frameType() const;
        [[nodiscard]] u2 offsetDelta() const;

        /**
         * The 0th entry in locals represents the verification type of the first additional local variable. If locals[M]
         * represents local variable N, then:
         *
         * locals[M+1] represents local variable N+1 if locals[M] is one of Top_variable_info, Integer_variable_info,
         * Float_variable_info, Null_variable_info, UninitializedThis_variable_info, Object_variable_info, or
         * Uninitialized_variable_info; and
         *
         * locals[M+1] represents local variable N+2 if locals[M] is either Long_variable_info or Double_variable_info.
         *
         * It is an error if, for any index i, locals[i] represents a local variable whose index is greater than the
         * maximum number of local variables for the method.
         */
        [[nodiscard]] const std::vector<VerificationTypeInfo>& locals() const;

      private:
        u1 m_frameType;
        u2 m_offsetDelta;
        std::vector<VerificationTypeInfo> m_locals;
    };

    class FullFrame
    {
      public:
        static constexpr u1 FULL_FRAME_TAG_VALUE = 255;

        FullFrame(u2 offsetDelta, std::vector<VerificationTypeInfo> locals, std::vector<VerificationTypeInfo> stack);

        [[nodiscard]] u1 frameType() const;
        [[nodiscard]] u2 offsetDelta() const;
        [[nodiscard]] u2 numberOfLocals() const;
        [[nodiscard]] const std::vector<VerificationTypeInfo>& locals() const;
        [[nodiscard]] u2 numberOfStackItems() const;
        [[nodiscard]] const std::vector<VerificationTypeInfo>& stack() const;

      private:
        u2 m_offsetDelta;
        std::vector<VerificationTypeInfo> m_locals;
        std::vector<VerificationTypeInfo> m_stack;
    };

    /**
     * A stack map frame is represented by a discriminated union, stack_map_frame, which consists of a one-byte tag,
     * indicating which item of the union is in use, followed by zero or more bytes, giving more information about the
     * tag.
     */
    using StackMapFrame = std::variant<SameFrame,
                                       SameLocals1StackItemFrame,
                                       SameLocals1StackItemFrameExtended,
                                       ChopFrame,
                                       SameFrameExtended,
                                       AppendFrame,
                                       FullFrame>;

    /**
     * The StackMapTable attribute is a variable-length attribute in the attributes table of a Code attribute (§4.7.3).
     * A StackMapTable attribute is used during the process of verification by type checking (§4.10.1).
     *
     * There may be at most one StackMapTable attribute in the attributes table of a Code attribute.
     *
     * In a class file whose version number is 50.0 or above, if a method's Code attribute does not have a
     * StackMapTable attribute, it has an implicit stack map attribute (§4.10.1). This implicit stack map attribute is
     * equivalent to a StackMapTable attribute with number_of_entries equal to zero.
     */
    class StackMapTable : public Attribute
    {
      public:
        static constexpr auto STACK_MAP_TABLE_ATTRIBUTE_NAME = "StackMapTable";

        StackMapTable(const ConstantPool& constantPool, const AttributeInfo& attributeInfo);

        /**
         * The value of the number_of_entries item gives the number of stack_map_frame entries in the entries table.
         */
        [[nodiscard]] u2 numberOfEntries() const;

        /**
         * Each entry in the entries table describes one stack map frame of the method. The order of the stack map
         * frames in the entries table is significant.
         */
        [[nodiscard]] const std::vector<StackMapFrame>& entries() const;

      private:
        std::vector<StackMapFrame> m_entries;
    };
} // namespace AeroJet::Java::ClassFile