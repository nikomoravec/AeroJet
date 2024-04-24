/*
 * ConstantPoolNode.cpp
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

#include "ConstantPoolNode.hpp"

#include "Assertion.hpp"
#include "ClassInfoNode.hpp"

namespace AeroJet::Tree
{
    ConstantPoolNode::ConstantPoolNode(const std::shared_ptr<Node>& parent) :
        Node(Type::CONSTANT_POOL, parent)
    {
        AEROJET_VERIFY_THROW(parent->type() == Type::CLASS_INFO, AeroJet::Exceptions::RuntimeException, "Parent node for CONSTANT_POOL can be only node of type CLASS");
    }

    const Java::ClassFile::ConstantPool& ConstantPoolNode::constantPool() const
    {
        return std::static_pointer_cast<ClassInfoNode>(m_parent)->classInfo().constantPool();
    }
} // namespace AeroJet::Tree
