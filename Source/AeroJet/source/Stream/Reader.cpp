/*
 * Reader.cpp
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

#include "Stream/Reader.hpp"

#include "Exceptions/RuntimeException.hpp"
#include "fmt/format.h"

namespace AeroJet::Stream::Reader
{
    template<typename T>
    static T swapEndian(const T& data)
    {
        static_assert(CHAR_BIT == 8, "CHAR_BIT != 8");

        union ByteObjectRepresentation
        {
            T             object;
            unsigned char bytes[sizeof(T)];
        };

        ByteObjectRepresentation source{};
        ByteObjectRepresentation dst{};

        source.object = data;

        for(size_t k = 0; k < sizeof(T); k++)
        {
            dst.bytes[k] = source.bytes[sizeof(T) - k - 1];
        }

        return dst.object;
    }

    template<typename T>
    static inline T readInternal(std::istream& stream, ByteOrder byteOrder)
    {
        const std::size_t readSize = sizeof(T);

        if(stream.eof())
        {
            throw Exceptions::RuntimeException(
                fmt::format("stream EOF at {:#08x}! Read size was {}", stream.tellg(), readSize));
        }

        const std::istream::pos_type currentPos = stream.tellg();

        stream.seekg(0, std::ios::end);
        const std::istream::pos_type endPos = stream.tellg();
        stream.seekg(currentPos);

        if((static_cast<std::size_t>(currentPos) + readSize) > static_cast<std::size_t>(endPos))
        {
            throw Exceptions::RuntimeException(
                fmt::format("Attempt to read {} bytes while {} is available", readSize, endPos - currentPos));
        }

        T read{};
        stream.read((char*)&read, readSize);

        if(byteOrder == ByteOrder::INVERSE && readSize > 1)
        {
            read = swapEndian(read);
        }

        return read;
    }

    template<>
    i1 read(std::istream& stream, ByteOrder byteOrder)
    {
        return readInternal<i1>(stream, byteOrder);
    }

    template<>
    i2 read(std::istream& stream, ByteOrder byteOrder)
    {
        return readInternal<i2>(stream, byteOrder);
    }

    template<>
    i4 read(std::istream& stream, ByteOrder byteOrder)
    {
        return readInternal<i4>(stream, byteOrder);
    }

    template<>
    i8 read(std::istream& stream, ByteOrder byteOrder)
    {
        return readInternal<i8>(stream, byteOrder);
    }

    template<>
    u1 read(std::istream& stream, ByteOrder byteOrder)
    {
        return readInternal<u1>(stream, byteOrder);
    }

    template<>
    u2 read(std::istream& stream, ByteOrder byteOrder)
    {
        return readInternal<u2>(stream, byteOrder);
    }

    template<>
    u4 read(std::istream& stream, ByteOrder byteOrder)
    {
        return readInternal<u4>(stream, byteOrder);
    }

    template<>
    u8 read(std::istream& stream, ByteOrder byteOrder)
    {
        return readInternal<u8>(stream, byteOrder);
    }
} // namespace AeroJet::Stream::Reader
