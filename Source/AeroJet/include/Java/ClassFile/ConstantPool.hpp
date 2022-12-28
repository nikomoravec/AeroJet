/*
 * ConstantPool.hpp
 *
 * Copyright © 2022 AeroJet Developers. All Rights Reserved.
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

#include "ConstantPoolEntry.hpp"
#include "Types.hpp"
#include <map>

namespace AeroJet::Java::ClassFile
{
    class ConstantPool final
    {
    public:
        [[nodiscard]]
        u2 size() const;

        [[nodiscard]]
        const ConstantPoolEntry& getEntry(u2 index) const;

        const ConstantPoolEntry& operator[](u2 index) const;

        void addEntry(u2 index, ConstantPoolEntry&& entry);

        [[nodiscard]]
        auto begin() const;

        [[nodiscard]]
        auto end() const;

    private:
        std::map<u2, ConstantPoolEntry> m_entries;
    };
}
