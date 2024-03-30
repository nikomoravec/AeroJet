/*
 * StreamUtils.cpp
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

#include "Stream/StreamUtils.hpp"

#include <iterator>

namespace AeroJet::Stream
{
    std::vector<u1> Utils::streamToBytes(Stream::MemoryStream& stream)
    {
        stream.seekg(0, std::ios::end);
        std::streampos dataSize = stream.tellg();
        stream.seekg(0, std::ios::beg);

        std::vector<u1> bytes;
        bytes.reserve(dataSize);

        bytes.assign(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
        return bytes;
    }

    Stream::MemoryStream Utils::bytesToStream(const std::vector<u1>& bytes)
    {
        Stream::MemoryStream stream;
        std::copy(bytes.begin(), bytes.end(), std::ostream_iterator<u1>(stream));

        return stream;
    }

    Stream::MemoryStream Utils::bytesToStream(std::vector<u1>&& bytes)
    {
        Stream::MemoryStream stream;
        std::move(bytes.begin(), bytes.end(), std::ostream_iterator<u1>(stream));

        return stream;
    }
} // namespace AeroJet::Stream
