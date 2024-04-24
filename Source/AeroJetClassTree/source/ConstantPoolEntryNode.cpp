/*
 * ConstantPoolEntryNode.cpp
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

#include "ConstantPoolEntryNode.hpp"

#include "Assertion.hpp"
#include "ConstantPoolNode.hpp"
#include "Exceptions/RuntimeException.hpp"

namespace AeroJet::Tree
{

    ConstantPoolEntryNode::ConstantPoolEntryNode(const std::shared_ptr<Node>& parent, u2 index) :
        IndexedNode(Type::CONSTANT_POOL_ENTRY, parent, index)
    {
        AEROJET_VERIFY_THROW(parent->type() == Type::CONSTANT_POOL, AeroJet::Exceptions::RuntimeException, "Parent node for CONSTANT_POOL_ENTRY can be only node of type CONSTANT_POOL");
    }

    const AeroJet::Java::ClassFile::ConstantPoolEntry& ConstantPoolEntryNode::entry() const
    {
        const Java::ClassFile::ConstantPool& constantPool = std::static_pointer_cast<ConstantPoolNode>(m_parent)->constantPool();
        return constantPool.at(m_index);
    }
} // namespace AeroJet::Tree
