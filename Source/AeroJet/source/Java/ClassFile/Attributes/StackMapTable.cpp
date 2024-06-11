/*
 * StackMapTable.cpp
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

#include "Java/ClassFile/Attributes/StackMapTable.hpp"

#include "Exceptions/RuntimeException.hpp"
#include "fmt/format.h"
#include "Stream/StandardStreamWrapper.hpp"

namespace AeroJet::Java::ClassFile
{
    SameFrame::SameFrame(u1 frameType) :
        m_frameType(frameType) {}

    u1 SameFrame::frameType() const
    {
        return m_frameType;
    }

    SameLocals1StackItemFrame::SameLocals1StackItemFrame(u1 frameType, VerificationTypeInfo stack) :
        m_frameType(frameType), m_stack(stack)
    {
    }

    u1 SameLocals1StackItemFrame::frameType() const
    {
        return m_frameType;
    }

    VerificationTypeInfo SameLocals1StackItemFrame::stack() const
    {
        return m_stack;
    }

    u2 SameLocals1StackItemFrame::offsetDelta() const
    {
        return m_frameType - 64;
    }

    SameLocals1StackItemFrameExtended::SameLocals1StackItemFrameExtended(u2 offsetDelta, VerificationTypeInfo stack) :
        m_offsetDelta(offsetDelta), m_stack(stack)
    {
    }

    u1 SameLocals1StackItemFrameExtended::frameType() const
    {
        return SAME_LOCALS_1_STACK_ITEM_EXTENDED_TAG_VALUE;
    }

    u2 SameLocals1StackItemFrameExtended::offsetDelta() const
    {
        return m_offsetDelta;
    }

    VerificationTypeInfo SameLocals1StackItemFrameExtended::stack() const
    {
        return m_stack;
    }

    ChopFrame::ChopFrame(u1 frameType, u2 offsetDelta) :
        m_frameType(frameType), m_offsetDelta(offsetDelta) {}

    u1 ChopFrame::frameType() const
    {
        return m_frameType;
    }

    u2 ChopFrame::offsetDelta() const
    {
        return m_offsetDelta;
    }

    SameFrameExtended::SameFrameExtended(u2 offsetDelta) :
        m_offsetDelta(offsetDelta) {}

    u1 SameFrameExtended::frameType() const
    {
        return SAME_FRAME_EXTENDED_TAG_VALUE;
    }

    u2 SameFrameExtended::offsetDelta() const
    {
        return m_offsetDelta;
    }

    AppendFrame::AppendFrame(u1 frameType, u2 offsetDelta, std::vector<VerificationTypeInfo> locals) :
        m_frameType(frameType), m_offsetDelta(offsetDelta), m_locals(std::move(locals))
    {
    }

    u1 AppendFrame::frameType() const
    {
        return m_frameType;
    }

    u2 AppendFrame::offsetDelta() const
    {
        return m_offsetDelta;
    }

    const std::vector<VerificationTypeInfo>& AppendFrame::locals() const
    {
        return m_locals;
    }

    FullFrame::FullFrame(u2 offsetDelta,
                         std::vector<VerificationTypeInfo> locals,
                         std::vector<VerificationTypeInfo> stack) :
        m_offsetDelta(offsetDelta),
        m_locals(std::move(locals)), m_stack(std::move(stack))
    {
    }

    u1 FullFrame::frameType() const
    {
        return FULL_FRAME_TAG_VALUE;
    }

    u2 FullFrame::offsetDelta() const
    {
        return m_offsetDelta;
    }

    u2 FullFrame::numberOfLocals() const
    {
        return m_locals.size();
    }

    const std::vector<VerificationTypeInfo>& FullFrame::locals() const
    {
        return m_locals;
    }

    u2 FullFrame::numberOfStackItems() const
    {
        return m_stack.size();
    }

    const std::vector<VerificationTypeInfo>& FullFrame::stack() const
    {
        return m_stack;
    }

    StackMapTable::StackMapTable(const ConstantPool& constantPool, const AttributeInfo& attributeInfo) :
        Attribute(constantPool, attributeInfo, STACK_MAP_TABLE_ATTRIBUTE_NAME)
    {
        const u2 numberOfEntries = m_infoDataStream.template read<u2>();
        for(u2 entryIndex = 0; entryIndex < numberOfEntries; entryIndex++)
        {
            const u1 frameType = m_infoDataStream.template read<u1>();
            if(frameType >= SameFrame::SAME_FRAME_MIN_TAG_VALUE && frameType <= SameFrame::SAME_FRAME_MAX_TAG_VALUE)
            {
                m_entries.emplace_back(StackMapFrame{ SameFrame{ frameType } });
            }
            else if(frameType >= SameLocals1StackItemFrame::SAME_LOCALS_1_STACK_ITEM_MIN_TAG_VALUE && frameType <= SameLocals1StackItemFrame::SAME_LOCALS_1_STACK_ITEM_MAX_TAG_VALUE)
            {
                const VerificationTypeInfo verificationTypeInfo = m_infoDataStream.template read<VerificationTypeInfo>();
                m_entries.emplace_back(StackMapFrame{ SameLocals1StackItemFrame{ frameType, verificationTypeInfo } });
            }
            else if(frameType == SameLocals1StackItemFrameExtended::SAME_LOCALS_1_STACK_ITEM_EXTENDED_TAG_VALUE)
            {
                const u2 offsetDelta = m_infoDataStream.template read<u2>();
                const VerificationTypeInfo verificationTypeInfo = m_infoDataStream.template read<VerificationTypeInfo>();
                m_entries.emplace_back(StackMapFrame{ SameLocals1StackItemFrameExtended{ offsetDelta, verificationTypeInfo } });
            }
            else if(frameType >= ChopFrame::CHOP_FRAME_MIN_TAG_VALUE && frameType <= ChopFrame::CHOP_FRAME_MAX_TAG_VALUE)
            {
                const u2 offsetDelta = m_infoDataStream.template read<u2>();
                m_entries.emplace_back(StackMapFrame{ ChopFrame{ frameType, offsetDelta } });
            }
            else if(frameType == SameFrameExtended::SAME_FRAME_EXTENDED_TAG_VALUE)
            {
                const u2 offsetDelta = m_infoDataStream.template read<u2>();
                m_entries.emplace_back(StackMapFrame{ SameFrameExtended{ offsetDelta } });
            }
            else if(frameType >= AppendFrame::APPEND_FRAME_MIN_TAG_VALUE && frameType <= AppendFrame::APPEND_FRAME_MAX_TAG_VALUE)
            {
                const u2 offsetDelta = m_infoDataStream.template read<u2>();

                const u1 localsSize = frameType - 251;
                std::vector<VerificationTypeInfo> locals;
                locals.reserve(localsSize);

                for(u1 localsIndex = 0; localsIndex < localsSize; localsIndex++)
                {
                    VerificationTypeInfo verificationTypeInfo = m_infoDataStream.template read<VerificationTypeInfo>();
                    locals.emplace_back(verificationTypeInfo);
                }

                m_entries.emplace_back(StackMapFrame{ AppendFrame{ frameType, offsetDelta, locals } });
            }
            else if(frameType == FullFrame::FULL_FRAME_TAG_VALUE)
            {
                const u2 offsetDelta = m_infoDataStream.template read<u2>();
                const u2 numberOfLocals = m_infoDataStream.template read<u2>();

                std::vector<VerificationTypeInfo> locals = m_infoDataStream.template readSome<VerificationTypeInfo>(numberOfLocals);

                const u2 numberOfStackItems = m_infoDataStream.template read<u2>();
                std::vector<VerificationTypeInfo> stack = m_infoDataStream.template readSome<VerificationTypeInfo>(numberOfStackItems);

                m_entries.emplace_back(StackMapFrame{ FullFrame{ offsetDelta, locals, stack } });
            }
            else
            {
                throw Exceptions::RuntimeException(fmt::format("Unknown frame type value: {}!", frameType));
            }
        }
    }

    u2 StackMapTable::numberOfEntries() const
    {
        return m_entries.size();
    }

    const std::vector<StackMapFrame>& StackMapTable::entries() const
    {
        return m_entries;
    }
} // namespace AeroJet::Java::ClassFile
