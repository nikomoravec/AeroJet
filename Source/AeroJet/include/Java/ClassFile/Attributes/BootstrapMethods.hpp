/*
 * BootstrapMethods.hpp
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

#pragma once

#include "Java/ClassFile/Attributes/Attribute.hpp"

#include <vector>

namespace AeroJet::Java::ClassFile
{
    /**
     * The BootstrapMethods attribute is a variable-length attribute in the attributes table of a
     * ClassFile structure (§4.1). The BootstrapMethods attribute records bootstrap method specifiers referenced by
     * invokedynamic instructions (§invokedynamic).
     *
     * There must be exactly one BootstrapMethods attribute in the attributes table of a ClassFile structure if the
     * constant_pool table of the ClassFile structure has at least one CONSTANT_InvokeDynamic_info entry (§4.4.10).
     *
     * There may be at most one BootstrapMethods attribute in the attributes table of a ClassFile structure.
     */
    class BootstrapMethods : public Attribute
    {
      public:
        static constexpr auto BOOTSTRAP_METHODS_ATTRIBUTE_NAME = "BootstrapMethods";

        class BootstrapMethodsEntry
        {
          public:
            BootstrapMethodsEntry(u2 bootstrapMethodRef, const std::vector<u2>& bootstrapArguments);

            /**
             * The value of the bootstrap_method_ref item must be a valid index into the constant_pool table.
             * The constant_pool entry at that index must be a CONSTANT_MethodHandle_info structure (§4.4.8).
             *
             * The form of the method handle is driven by the continuing resolution of the call site specifier in
             * §invokedynamic, where execution of invoke in java.lang.invoke.MethodHandle requires that the
             * bootstrap method handle be adjustable to the actual arguments being passed, as if by a call to
             * java.lang.invoke.MethodHandle.asType. Accordingly, the reference_kind item of the
             * CONSTANT_MethodHandle_info structure should have the value 6 or 8 (§5.4.3.5), and the
             * reference_index item should specify a static method or constructor that takes three arguments of type
             * java.lang.invoke.MethodHandles.Lookup, String, and java.lang.invoke.MethodType, in that order.
             * Otherwise, invocation of the bootstrap method handle during call site specifier resolution will
             * complete abruptly.
             */
            [[nodiscard]] u2 bootstrapMethodRef() const;

            [[nodiscard]] u2 numBootstrapArguments() const;

            /**
             * Each entry in the bootstrap_arguments array must be a valid index into the constant_pool table.
             * The constant_pool entry at that index must be a CONSTANT_String_info, CONSTANT_Class_info,
             * CONSTANT_Integer_info, CONSTANT_Long_info, CONSTANT_Float_info, CONSTANT_Double_info,
             * CONSTANT_MethodHandle_info, or
             * CONSTANT_MethodType_info structure (§4.4.3, §4.4.1, §4.4.4, §4.4.5, §4.4.8, §4.4.9).
             */
            [[nodiscard]] const std::vector<u2>& bootstrapArguments() const;

          private:
            u2 m_bootstrapMethodRef;
            std::vector<u2> m_bootstrapArguments;
        };

        BootstrapMethods(const ConstantPool& constantPool, const AttributeInfo& attributeInfo);

        [[nodiscard]] u2 numBootstrapMethods() const;

        /**
         * The value of the num_bootstrap_methods item determines the number of bootstrap method specifiers
         * in the bootstrap_methods array.
         */
        [[nodiscard]] const std::vector<BootstrapMethodsEntry>& bootstrapMethods() const;

      private:
        std::vector<BootstrapMethodsEntry> m_bootstrapMethods;
    };
} // namespace AeroJet::Java::ClassFile