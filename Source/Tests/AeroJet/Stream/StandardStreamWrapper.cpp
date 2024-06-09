#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "Stream/StandardStreamWrapper.hpp"

#include "doctest.h"

#include <filesystem>
#include <sstream>

TEST_CASE("AeroJet::StanardStreamWrapper::type")
{
    SUBCASE("std::ifstream")
    {
        AeroJet::Stream::StandardStreamWrapper<std::ifstream> stream{};
        REQUIRE_EQ(stream.type(), AeroJet::Stream::StreamType::INPUT);
    }

    SUBCASE("std::istringstream")
    {
        AeroJet::Stream::StandardStreamWrapper<std::istringstream> stream{};
        REQUIRE_EQ(stream.type(), AeroJet::Stream::StreamType::INPUT);
    }

    SUBCASE("std::wistringstream")
    {
        AeroJet::Stream::StandardStreamWrapper<std::wistringstream> stream{};
        REQUIRE_EQ(stream.type(), AeroJet::Stream::StreamType::INPUT);
    }

    SUBCASE("std::wifstream")
    {
        AeroJet::Stream::StandardStreamWrapper<std::wifstream> stream{};
        REQUIRE_EQ(stream.type(), AeroJet::Stream::StreamType::INPUT);
    }

    SUBCASE("std::ofstream")
    {
        AeroJet::Stream::StandardStreamWrapper<std::ofstream> stream{};
        REQUIRE_EQ(stream.type(), AeroJet::Stream::StreamType::OUTPUT);
    }

    SUBCASE("std::ostringstream")
    {
        AeroJet::Stream::StandardStreamWrapper<std::ostringstream> stream{};
        REQUIRE_EQ(stream.type(), AeroJet::Stream::StreamType::OUTPUT);
    }

    SUBCASE("std::wofstream")
    {
        AeroJet::Stream::StandardStreamWrapper<std::wofstream> stream{};
        REQUIRE_EQ(stream.type(), AeroJet::Stream::StreamType::OUTPUT);
    }

    SUBCASE("std::wostringstream")
    {
        AeroJet::Stream::StandardStreamWrapper<std::wostringstream> stream{};
        REQUIRE_EQ(stream.type(), AeroJet::Stream::StreamType::OUTPUT);
    }

    SUBCASE("std::stringstream")
    {
        AeroJet::Stream::StandardStreamWrapper<std::stringstream> stream{};
        REQUIRE_EQ(stream.type(), AeroJet::Stream::StreamType::INPUT_OUTPUT);
    }

    SUBCASE("std::fstream")
    {
        AeroJet::Stream::StandardStreamWrapper<std::fstream> stream{};
        REQUIRE_EQ(stream.type(), AeroJet::Stream::StreamType::INPUT_OUTPUT);
    }

    SUBCASE("std::wstringstream")
    {
        AeroJet::Stream::StandardStreamWrapper<std::wstringstream> stream{};
        REQUIRE_EQ(stream.type(), AeroJet::Stream::StreamType::INPUT_OUTPUT);
    }

    SUBCASE("std::wfstream")
    {
        AeroJet::Stream::StandardStreamWrapper<std::wfstream> stream{};
        REQUIRE_EQ(stream.type(), AeroJet::Stream::StreamType::INPUT_OUTPUT);
    }
}

TEST_CASE("AeroJet::StanardStreamWrapper::size")
{
    static constexpr auto FILE_PATH = "Resources/HelloWorld.class";

    const uintmax_t size = std::filesystem::file_size(FILE_PATH);
    AeroJet::Stream::StandardStreamWrapper<std::ifstream> stream{ FILE_PATH };

    REQUIRE_EQ(stream.size(), size);
}

TEST_CASE("AeroJet::StanardStreamWrapper::isOpen")
{
    static constexpr auto FILE_PATH = "Resources/HelloWorld.class";

    SUBCASE("std::ifstream")
    {
        SUBCASE("NO_FILE")
        {
            AeroJet::Stream::StandardStreamWrapper<std::ifstream> stream{};
            REQUIRE_EQ(stream.isOpen(), false);
        }

        SUBCASE("REAL_FILE")
        {
            AeroJet::Stream::StandardStreamWrapper<std::ifstream> stream{ FILE_PATH };
            REQUIRE_EQ(stream.isOpen(), true);
        }
    }

    SUBCASE("std::wifstream")
    {
        SUBCASE("NO_FILE")
        {
            AeroJet::Stream::StandardStreamWrapper<std::wifstream> stream{};
            REQUIRE_EQ(stream.isOpen(), false);
        }

        SUBCASE("REAL_FILE")
        {
            AeroJet::Stream::StandardStreamWrapper<std::wifstream> stream{ FILE_PATH };
            REQUIRE_EQ(stream.isOpen(), true);
        }
    }
}

TEST_CASE("AeroJet::StanardStreamWrapper::readSome")
{
    static constexpr auto FILE_PATH = "Resources/HelloWorld.class";

    SUBCASE("array")
    {
        AeroJet::Stream::StandardStreamWrapper<std::ifstream> stream{ FILE_PATH };
        std::array<AeroJet::u1, 4> bytes = stream.readSome<AeroJet::u1, 4>();
        REQUIRE_EQ(bytes[0], 0xCA);
        REQUIRE_EQ(bytes[1], 0xFE);
        REQUIRE_EQ(bytes[2], 0xBA);
        REQUIRE_EQ(bytes[3], 0xBE);
    }

    SUBCASE("vector")
    {
        AeroJet::Stream::StandardStreamWrapper<std::ifstream, AeroJet::Stream::ByteOrder::REVERSE> stream{ FILE_PATH };
        std::vector<AeroJet::u1> bytes = stream.readSome<AeroJet::u1>(4);
        REQUIRE_EQ(bytes[0], 0xCA);
        REQUIRE_EQ(bytes[1], 0xFE);
        REQUIRE_EQ(bytes[2], 0xBA);
        REQUIRE_EQ(bytes[3], 0xBE);
    }
}
