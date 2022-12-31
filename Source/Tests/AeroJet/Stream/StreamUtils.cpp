/*
 * StreamUtils.cpp
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

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "AeroJet.hpp"
#include "doctest.h"

TEST_CASE("AeroJet::Stream::Utils::bytesToStream")
{
    std::stringstream stream = AeroJet::Stream::Utils::bytesToStream({0xDE, 0xAD, 0xFA, 0xCE});

    REQUIRE_EQ(stream.get(), 0xDE);
    REQUIRE_EQ(stream.get(), 0xAD);
    REQUIRE_EQ(stream.get(), 0xFA);
    REQUIRE_EQ(stream.get(), 0xCE);
}

TEST_CASE("AeroJet::Stream::Utils::streamToBytes")
{
    std::stringstream stream;
    AeroJet::Stream::Writer::write(stream, AeroJet::u1{0xDE});
    AeroJet::Stream::Writer::write(stream, AeroJet::u1{0xAD});
    AeroJet::Stream::Writer::write(stream, AeroJet::u1{0xFA});
    AeroJet::Stream::Writer::write(stream, AeroJet::u1{0xCE});

    std::vector<AeroJet::u1> bytes = AeroJet::Stream::Utils::streamToBytes(stream);

    REQUIRE_EQ(bytes.size(), 4);

    REQUIRE_EQ(bytes[0], 0xDE);
    REQUIRE_EQ(bytes[1], 0xAD);
    REQUIRE_EQ(bytes[2], 0xFA);
    REQUIRE_EQ(bytes[3], 0xCE);
}
