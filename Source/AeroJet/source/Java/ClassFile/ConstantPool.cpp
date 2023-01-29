/*
 * ConstantPool.cpp
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

#include "Java/ClassFile/ConstantPool.hpp"

namespace AeroJet::Java::ClassFile
{
    u2 ConstantPool::size() const
    {
        return m_entries.size();
    }

    void ConstantPool::addEntry(u2 index, ConstantPoolEntry&& entry)
    {
        m_entries.insert({ index, entry });
    }

    const ConstantPoolEntry& ConstantPool::getEntry(u2 index) const
    {
        return m_entries.at(index);
    }

    const ConstantPoolEntry& ConstantPool::operator[](u2 index) const
    {
        return getEntry(index);
    }

    std::map<u2, ConstantPoolEntry>::const_iterator ConstantPool::begin() const
    {
        return m_entries.begin();
    }

    std::map<u2, ConstantPoolEntry>::const_iterator ConstantPool::end() const
    {
        return m_entries.end();
    }
} // namespace AeroJet::Java::ClassFile
