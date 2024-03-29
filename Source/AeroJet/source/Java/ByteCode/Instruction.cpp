/*
 * Instruction.cpp
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

#include "Java/ByteCode/Instruction.hpp"

#include "Exceptions/OperationNotSupportedException.hpp"
#include "Exceptions/RuntimeException.hpp"
#include "fmt/format.h"
#include "Java/ByteCode/OpCodes.hpp"
#include "Stream/Reader.hpp"
#include "Stream/StreamUtils.hpp"
#include "Stream/Writer.hpp"

namespace AeroJet::Java::ByteCode
{
    Instruction::Instruction(OperationCode opCode) :
        m_opCode(opCode) {}

    Instruction::Instruction(OperationCode opCode, std::vector<u1>&& data) :
        m_opCode(opCode), m_data(std::move(data))
    {
    }

    OperationCode Instruction::opCode() const
    {
        return m_opCode;
    }

    const std::vector<u1>& Instruction::data() const
    {
        return m_data;
    }
} // namespace AeroJet::Java::ByteCode

template<>
AeroJet::Java::ByteCode::Instruction AeroJet::Stream::Reader::read(std::istream& stream, ByteOrder byteOrder)
{
    Stream::MemoryStream dataStream;

    const AeroJet::Java::ByteCode::OperationCode opCode =
        static_cast<AeroJet::Java::ByteCode::OperationCode>(AeroJet::Stream::Reader::read<u1>(stream, byteOrder));

    switch(opCode)
    {
        case AeroJet::Java::ByteCode::OperationCode::aaload:
        case AeroJet::Java::ByteCode::OperationCode::aastore:
        case AeroJet::Java::ByteCode::OperationCode::aconst_null:
        case AeroJet::Java::ByteCode::OperationCode::areturn:
        case AeroJet::Java::ByteCode::OperationCode::arraylength:
        case AeroJet::Java::ByteCode::OperationCode::athrow:
        case AeroJet::Java::ByteCode::OperationCode::baload:
        case AeroJet::Java::ByteCode::OperationCode::bastore:
        case AeroJet::Java::ByteCode::OperationCode::caload:
        case AeroJet::Java::ByteCode::OperationCode::castore:
        case AeroJet::Java::ByteCode::OperationCode::d2f:
        case AeroJet::Java::ByteCode::OperationCode::d2i:
        case AeroJet::Java::ByteCode::OperationCode::d2l:
        case AeroJet::Java::ByteCode::OperationCode::dadd:
        case AeroJet::Java::ByteCode::OperationCode::daload:
        case AeroJet::Java::ByteCode::OperationCode::dastore:
        case AeroJet::Java::ByteCode::OperationCode::dcmpg:
        case AeroJet::Java::ByteCode::OperationCode::dcmpl:
        case AeroJet::Java::ByteCode::OperationCode::ddiv:
        case AeroJet::Java::ByteCode::OperationCode::dmul:
        case AeroJet::Java::ByteCode::OperationCode::dneg:
        case AeroJet::Java::ByteCode::OperationCode::drem:
        case AeroJet::Java::ByteCode::OperationCode::dreturn:
        case AeroJet::Java::ByteCode::OperationCode::dsub:
        case AeroJet::Java::ByteCode::OperationCode::dup:
        case AeroJet::Java::ByteCode::OperationCode::dup_x1:
        case AeroJet::Java::ByteCode::OperationCode::dup_x2:
        case AeroJet::Java::ByteCode::OperationCode::dup2:
        case AeroJet::Java::ByteCode::OperationCode::dup2_x1:
        case AeroJet::Java::ByteCode::OperationCode::dup2_x2:
        case AeroJet::Java::ByteCode::OperationCode::f2i:
        case AeroJet::Java::ByteCode::OperationCode::f2d:
        case AeroJet::Java::ByteCode::OperationCode::f2l:
        case AeroJet::Java::ByteCode::OperationCode::fadd:
        case AeroJet::Java::ByteCode::OperationCode::faload:
        case AeroJet::Java::ByteCode::OperationCode::fastore:
        case AeroJet::Java::ByteCode::OperationCode::fcmpg:
        case AeroJet::Java::ByteCode::OperationCode::fcmpl:
        case AeroJet::Java::ByteCode::OperationCode::fdiv:
        case AeroJet::Java::ByteCode::OperationCode::fmul:
        case AeroJet::Java::ByteCode::OperationCode::fneg:
        case AeroJet::Java::ByteCode::OperationCode::frem:
        case AeroJet::Java::ByteCode::OperationCode::freturn:
        case AeroJet::Java::ByteCode::OperationCode::fsub:
        case AeroJet::Java::ByteCode::OperationCode::i2b:
        case AeroJet::Java::ByteCode::OperationCode::i2c:
        case AeroJet::Java::ByteCode::OperationCode::i2d:
        case AeroJet::Java::ByteCode::OperationCode::i2f:
        case AeroJet::Java::ByteCode::OperationCode::i2l:
        case AeroJet::Java::ByteCode::OperationCode::i2s:
        case AeroJet::Java::ByteCode::OperationCode::iadd:
        case AeroJet::Java::ByteCode::OperationCode::iaload:
        case AeroJet::Java::ByteCode::OperationCode::iand:
        case AeroJet::Java::ByteCode::OperationCode::iastore:
        case AeroJet::Java::ByteCode::OperationCode::aload_0:
        case AeroJet::Java::ByteCode::OperationCode::astore_0:
        case AeroJet::Java::ByteCode::OperationCode::dload_0:
        case AeroJet::Java::ByteCode::OperationCode::dstore_0:
        case AeroJet::Java::ByteCode::OperationCode::fconst_0:
        case AeroJet::Java::ByteCode::OperationCode::fload_0:
        case AeroJet::Java::ByteCode::OperationCode::fstore_0:
        case AeroJet::Java::ByteCode::OperationCode::aload_1:
        case AeroJet::Java::ByteCode::OperationCode::astore_1:
        case AeroJet::Java::ByteCode::OperationCode::dload_1:
        case AeroJet::Java::ByteCode::OperationCode::dstore_1:
        case AeroJet::Java::ByteCode::OperationCode::fconst_1:
        case AeroJet::Java::ByteCode::OperationCode::fload_1:
        case AeroJet::Java::ByteCode::OperationCode::fstore_1:
        case AeroJet::Java::ByteCode::OperationCode::aload_2:
        case AeroJet::Java::ByteCode::OperationCode::astore_2:
        case AeroJet::Java::ByteCode::OperationCode::dload_2:
        case AeroJet::Java::ByteCode::OperationCode::dstore_2:
        case AeroJet::Java::ByteCode::OperationCode::fconst_2:
        case AeroJet::Java::ByteCode::OperationCode::fload_2:
        case AeroJet::Java::ByteCode::OperationCode::fstore_2:
        case AeroJet::Java::ByteCode::OperationCode::iconst_m1:
        case AeroJet::Java::ByteCode::OperationCode::aload_3:
        case AeroJet::Java::ByteCode::OperationCode::astore_3:
        case AeroJet::Java::ByteCode::OperationCode::dconst_0:
        case AeroJet::Java::ByteCode::OperationCode::dconst_1:
        case AeroJet::Java::ByteCode::OperationCode::dload_3:
        case AeroJet::Java::ByteCode::OperationCode::dstore_3:
        case AeroJet::Java::ByteCode::OperationCode::fload_3:
        case AeroJet::Java::ByteCode::OperationCode::fstore_3:
        case AeroJet::Java::ByteCode::OperationCode::iconst_0:
        case AeroJet::Java::ByteCode::OperationCode::iconst_1:
        case AeroJet::Java::ByteCode::OperationCode::iconst_2:
        case AeroJet::Java::ByteCode::OperationCode::iconst_3:
        case AeroJet::Java::ByteCode::OperationCode::iconst_4:
        case AeroJet::Java::ByteCode::OperationCode::iconst_5:
        case AeroJet::Java::ByteCode::OperationCode::idiv:
        case AeroJet::Java::ByteCode::OperationCode::ior:
        case AeroJet::Java::ByteCode::OperationCode::iload_0:
        case AeroJet::Java::ByteCode::OperationCode::iload_1:
        case AeroJet::Java::ByteCode::OperationCode::iload_2:
        case AeroJet::Java::ByteCode::OperationCode::iload_3:
        case AeroJet::Java::ByteCode::OperationCode::imul:
        case AeroJet::Java::ByteCode::OperationCode::ineg:
        case AeroJet::Java::ByteCode::OperationCode::irem:
        case AeroJet::Java::ByteCode::OperationCode::ireturn:
        case AeroJet::Java::ByteCode::OperationCode::ishl:
        case AeroJet::Java::ByteCode::OperationCode::ishr:
        case AeroJet::Java::ByteCode::OperationCode::istore_0:
        case AeroJet::Java::ByteCode::OperationCode::istore_1:
        case AeroJet::Java::ByteCode::OperationCode::istore_2:
        case AeroJet::Java::ByteCode::OperationCode::istore_3:
        case AeroJet::Java::ByteCode::OperationCode::isub:
        case AeroJet::Java::ByteCode::OperationCode::iushr:
        case AeroJet::Java::ByteCode::OperationCode::ixor:
        case AeroJet::Java::ByteCode::OperationCode::l2d:
        case AeroJet::Java::ByteCode::OperationCode::l2f:
        case AeroJet::Java::ByteCode::OperationCode::l2i:
        case AeroJet::Java::ByteCode::OperationCode::ladd:
        case AeroJet::Java::ByteCode::OperationCode::laload:
        case AeroJet::Java::ByteCode::OperationCode::land:
        case AeroJet::Java::ByteCode::OperationCode::lastore:
        case AeroJet::Java::ByteCode::OperationCode::lcmp:
        case AeroJet::Java::ByteCode::OperationCode::lconst_0:
        case AeroJet::Java::ByteCode::OperationCode::lconst_1:
        case AeroJet::Java::ByteCode::OperationCode::ldiv:
        case AeroJet::Java::ByteCode::OperationCode::lload_0:
        case AeroJet::Java::ByteCode::OperationCode::lload_1:
        case AeroJet::Java::ByteCode::OperationCode::lload_2:
        case AeroJet::Java::ByteCode::OperationCode::lload_3:
        case AeroJet::Java::ByteCode::OperationCode::lor:
        case AeroJet::Java::ByteCode::OperationCode::lrem:
        case AeroJet::Java::ByteCode::OperationCode::lreturn:
        case AeroJet::Java::ByteCode::OperationCode::lshl:
        case AeroJet::Java::ByteCode::OperationCode::lshr:
        case AeroJet::Java::ByteCode::OperationCode::lstore_0:
        case AeroJet::Java::ByteCode::OperationCode::lstore_1:
        case AeroJet::Java::ByteCode::OperationCode::lstore_2:
        case AeroJet::Java::ByteCode::OperationCode::lstore_3:
        case AeroJet::Java::ByteCode::OperationCode::lsub:
        case AeroJet::Java::ByteCode::OperationCode::lushr:
        case AeroJet::Java::ByteCode::OperationCode::lxor:
        case AeroJet::Java::ByteCode::OperationCode::monitorenter:
        case AeroJet::Java::ByteCode::OperationCode::monitorexit:
        case AeroJet::Java::ByteCode::OperationCode::nop:
        case AeroJet::Java::ByteCode::OperationCode::pop:
        case AeroJet::Java::ByteCode::OperationCode::pop2:
        case AeroJet::Java::ByteCode::OperationCode::RETURN:
        case AeroJet::Java::ByteCode::OperationCode::saload:
        case AeroJet::Java::ByteCode::OperationCode::sastore:
        case AeroJet::Java::ByteCode::OperationCode::swap:
            break;
        case AeroJet::Java::ByteCode::OperationCode::aload:
        case AeroJet::Java::ByteCode::OperationCode::astore:
        case AeroJet::Java::ByteCode::OperationCode::bipush:
        case AeroJet::Java::ByteCode::OperationCode::dload:
        case AeroJet::Java::ByteCode::OperationCode::dstore:
        case AeroJet::Java::ByteCode::OperationCode::fload:
        case AeroJet::Java::ByteCode::OperationCode::fstore:
        case AeroJet::Java::ByteCode::OperationCode::iload:
        case AeroJet::Java::ByteCode::OperationCode::istore:
        case AeroJet::Java::ByteCode::OperationCode::ldc:
        case AeroJet::Java::ByteCode::OperationCode::lload:
        case AeroJet::Java::ByteCode::OperationCode::lmul:
        case AeroJet::Java::ByteCode::OperationCode::lneg:
        case AeroJet::Java::ByteCode::OperationCode::lstore:
        case AeroJet::Java::ByteCode::OperationCode::newarray:
        case AeroJet::Java::ByteCode::OperationCode::ret:
            AeroJet::Stream::Writer::write(dataStream, AeroJet::Stream::Reader::read<u1>(stream, byteOrder));
            break;
        case AeroJet::Java::ByteCode::OperationCode::anewarray:
        case AeroJet::Java::ByteCode::OperationCode::checkcast:
        case AeroJet::Java::ByteCode::OperationCode::getfield:
        case AeroJet::Java::ByteCode::OperationCode::getstatic:
        case AeroJet::Java::ByteCode::OperationCode::GOTO:
        case AeroJet::Java::ByteCode::OperationCode::if_acmpeq:
        case AeroJet::Java::ByteCode::OperationCode::if_acmpne:
        case AeroJet::Java::ByteCode::OperationCode::if_icmpeq:
        case AeroJet::Java::ByteCode::OperationCode::if_icmpne:
        case AeroJet::Java::ByteCode::OperationCode::if_icmpge:
        case AeroJet::Java::ByteCode::OperationCode::if_icmpgt:
        case AeroJet::Java::ByteCode::OperationCode::if_icmple:
        case AeroJet::Java::ByteCode::OperationCode::ifeq:
        case AeroJet::Java::ByteCode::OperationCode::ifne:
        case AeroJet::Java::ByteCode::OperationCode::iflt:
        case AeroJet::Java::ByteCode::OperationCode::ifge:
        case AeroJet::Java::ByteCode::OperationCode::ifgt:
        case AeroJet::Java::ByteCode::OperationCode::ifle:
        case AeroJet::Java::ByteCode::OperationCode::ifnonnull:
        case AeroJet::Java::ByteCode::OperationCode::ifnull:
        case AeroJet::Java::ByteCode::OperationCode::if_icmplt:
        case AeroJet::Java::ByteCode::OperationCode::iinc:
        case AeroJet::Java::ByteCode::OperationCode:: instanceof:
        case AeroJet::Java::ByteCode::OperationCode::invokespecial:
        case AeroJet::Java::ByteCode::OperationCode::invokestatic:
        case AeroJet::Java::ByteCode::OperationCode::invokevirtual:
        case AeroJet::Java::ByteCode::OperationCode::jsr:
        case AeroJet::Java::ByteCode::OperationCode::ldc_w:
        case AeroJet::Java::ByteCode::OperationCode::ldc2_w:
        case AeroJet::Java::ByteCode::OperationCode::NEW:
        case AeroJet::Java::ByteCode::OperationCode::putfield:
        case AeroJet::Java::ByteCode::OperationCode::putstatic:
        case AeroJet::Java::ByteCode::OperationCode::sipush:
        {
            AeroJet::Stream::Writer::write(dataStream, AeroJet::Stream::Reader::read<u1>(stream, byteOrder));
            AeroJet::Stream::Writer::write(dataStream, AeroJet::Stream::Reader::read<u1>(stream, byteOrder));
            break;
        }
        case AeroJet::Java::ByteCode::OperationCode::multianewarray:
        {
            AeroJet::Stream::Writer::write(dataStream, AeroJet::Stream::Reader::read<u1>(stream, byteOrder));
            AeroJet::Stream::Writer::write(dataStream, AeroJet::Stream::Reader::read<u1>(stream, byteOrder));
            AeroJet::Stream::Writer::write(dataStream, AeroJet::Stream::Reader::read<u1>(stream, byteOrder));
            break;
        }
        case AeroJet::Java::ByteCode::OperationCode::goto_w:
        case AeroJet::Java::ByteCode::OperationCode::invokedynamic:
        case AeroJet::Java::ByteCode::OperationCode::invokeinterface:
        case AeroJet::Java::ByteCode::OperationCode::jsr_w:
        {
            AeroJet::Stream::Writer::write(dataStream, AeroJet::Stream::Reader::read<AeroJet::u1>(stream, byteOrder));
            AeroJet::Stream::Writer::write(dataStream, AeroJet::Stream::Reader::read<AeroJet::u1>(stream, byteOrder));
            AeroJet::Stream::Writer::write(dataStream, AeroJet::Stream::Reader::read<AeroJet::u1>(stream, byteOrder));
            AeroJet::Stream::Writer::write(dataStream, AeroJet::Stream::Reader::read<AeroJet::u1>(stream, byteOrder));
            break;
        }
        case AeroJet::Java::ByteCode::OperationCode::tableswitch:
        case AeroJet::Java::ByteCode::OperationCode::lookupswitch:
        {
            const i4 localOffset = (static_cast<i4>(stream.tellg()) - 9);

            /*
             * JVM specification: A tableswitch/lookupswitch is a variable-length instruction.
             * Immediately after the tableswitch/lookupswitch opcode, between zero and three
             * null bytes (zeroed bytes, not the null object) are inserted as padding.
             * The number of null bytes is chosen so that the defaultbyte1 begins at an address
             * that is a multiple of four bytes from the start of the current method
             * (the opcode of its first instruction).
             */
            {
                const u4 padding = (((localOffset + 1) + 3) & ~3) - localOffset;
                u4 paddingIndex = padding;
                while((paddingIndex - 1) > 0)
                {
                    AeroJet::Stream::Reader::read<u1>(stream, byteOrder);
                    paddingIndex--;
                }
            }

            if(opCode == AeroJet::Java::ByteCode::OperationCode::tableswitch)
            {
                const AeroJet::i4 defaultValue =
                    localOffset + AeroJet::Stream::Reader::read<AeroJet::i4>(stream, byteOrder);
                const AeroJet::i4 lowValue = AeroJet::Stream::Reader::read<AeroJet::i4>(stream, byteOrder);
                const AeroJet::i4 highValue = AeroJet::Stream::Reader::read<AeroJet::i4>(stream, byteOrder);

                AeroJet::Stream::Writer::write(dataStream, defaultValue, AeroJet::Stream::ByteOrder::INVERSE);
                AeroJet::Stream::Writer::write(dataStream, lowValue, AeroJet::Stream::ByteOrder::INVERSE);
                AeroJet::Stream::Writer::write(dataStream, highValue, AeroJet::Stream::ByteOrder::INVERSE);

                const AeroJet::i4 jumpOffsetsCount = highValue - lowValue + 1;
                for(size_t jumpOffsetIndex = 0; jumpOffsetIndex < jumpOffsetsCount; jumpOffsetIndex++)
                {
                    AeroJet::Stream::Writer::write(dataStream,
                                                   localOffset +
                                                       AeroJet::Stream::Reader::read<AeroJet::i4>(stream, byteOrder),
                                                   AeroJet::Stream::ByteOrder::INVERSE);
                }

                break;
            }

            if(opCode == AeroJet::Java::ByteCode::OperationCode::lookupswitch)
            {
                const AeroJet::i4 defaultValue =
                    localOffset + AeroJet::Stream::Reader::read<AeroJet::i4>(stream, byteOrder);
                AeroJet::Stream::Writer::write(dataStream, defaultValue, AeroJet::Stream::ByteOrder::INVERSE);

                const AeroJet::i4 npairsCount = AeroJet::Stream::Reader::read<AeroJet::i4>(stream, byteOrder);
                AeroJet::Stream::Writer::write(dataStream, npairsCount, AeroJet::Stream::ByteOrder::INVERSE);

                for(AeroJet::i4 npairIndex = 0; npairIndex < npairsCount; npairIndex++)
                {
                    AeroJet::Stream::Writer::write(dataStream,
                                                   AeroJet::Stream::Reader::read<AeroJet::i4>(stream, byteOrder),
                                                   AeroJet::Stream::ByteOrder::INVERSE);
                    AeroJet::Stream::Writer::write(dataStream,
                                                   AeroJet::Stream::Reader::read<AeroJet::i4>(stream, byteOrder),
                                                   AeroJet::Stream::ByteOrder::INVERSE);
                }

                break;
            }
        }
        case AeroJet::Java::ByteCode::OperationCode::wide:
        {
            const AeroJet::Java::ByteCode::OperationCode nextOpCode =
                static_cast<AeroJet::Java::ByteCode::OperationCode>(
                    AeroJet::Stream::Reader::read<AeroJet::u1>(stream, byteOrder));
            AeroJet::Stream::Writer::write(dataStream, static_cast<AeroJet::u1>(nextOpCode));

            switch(nextOpCode)
            {
                case AeroJet::Java::ByteCode::OperationCode::iload:
                case AeroJet::Java::ByteCode::OperationCode::fload:
                case AeroJet::Java::ByteCode::OperationCode::aload:
                case AeroJet::Java::ByteCode::OperationCode::lload:
                case AeroJet::Java::ByteCode::OperationCode::dload:
                case AeroJet::Java::ByteCode::OperationCode::istore:
                case AeroJet::Java::ByteCode::OperationCode::fstore:
                case AeroJet::Java::ByteCode::OperationCode::astore:
                case AeroJet::Java::ByteCode::OperationCode::lstore:
                case AeroJet::Java::ByteCode::OperationCode::dstore:
                case AeroJet::Java::ByteCode::OperationCode::ret:
                {
                    AeroJet::Stream::Writer::write(dataStream,
                                                   AeroJet::Stream::Reader::read<AeroJet::u1>(stream, byteOrder));
                    AeroJet::Stream::Writer::write(dataStream,
                                                   AeroJet::Stream::Reader::read<AeroJet::u1>(stream, byteOrder));
                    break;
                }
                case AeroJet::Java::ByteCode::OperationCode::iinc:
                {
                    AeroJet::Stream::Writer::write(dataStream,
                                                   AeroJet::Stream::Reader::read<AeroJet::u1>(stream, byteOrder));
                    AeroJet::Stream::Writer::write(dataStream,
                                                   AeroJet::Stream::Reader::read<AeroJet::u1>(stream, byteOrder));
                    AeroJet::Stream::Writer::write(dataStream,
                                                   AeroJet::Stream::Reader::read<AeroJet::u1>(stream, byteOrder));
                    AeroJet::Stream::Writer::write(dataStream,
                                                   AeroJet::Stream::Reader::read<AeroJet::u1>(stream, byteOrder));
                    break;
                }
                default:
                    throw AeroJet::Exceptions::RuntimeException(
                        fmt::format("Unexpected OpCode ({:#04x}) after 'wide'!", static_cast<AeroJet::u1>(nextOpCode)));
            }
            break;
        }
        default:
            throw AeroJet::Exceptions::OperationNotSupportedException(opCode);
    }

    std::vector<AeroJet::u1> dataBytes = AeroJet::Stream::Utils::streamToBytes(dataStream);
    return { opCode, std::move(dataBytes) };
}
