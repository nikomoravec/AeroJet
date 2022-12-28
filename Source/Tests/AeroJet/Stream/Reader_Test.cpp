#include "AeroJet.hpp"

int main()
{
    std::stringstream ss;

    AeroJet::Stream::Writer::write(ss, AeroJet::u1{1});
    AeroJet::Stream::Writer::write(ss, AeroJet::u2{2});
    AeroJet::Stream::Writer::write(ss, AeroJet::u4{4});
    AeroJet::Stream::Writer::write(ss, AeroJet::u8{8});

    AeroJet::Stream::Writer::write(ss, AeroJet::i1{-1});
    AeroJet::Stream::Writer::write(ss, AeroJet::i2{-2});
    AeroJet::Stream::Writer::write(ss, AeroJet::i4{-4});
    AeroJet::Stream::Writer::write(ss, AeroJet::i8{-8});

    assert(ss.tellp() == sizeof(AeroJet::u1) +
                         sizeof(AeroJet::u2) +
                         sizeof(AeroJet::u4) +
                         sizeof(AeroJet::u8) +
                         sizeof(AeroJet::i1) +
                         sizeof(AeroJet::i2) +
                         sizeof(AeroJet::i4) +
                         sizeof(AeroJet::i8));

    {
        AeroJet::u1 read = 0;
        ss.read((char*)&read, sizeof(AeroJet::u1));

        if (read == 1)
        {
            return 0;
        }
    }

    {
        AeroJet::u2 read = 0;
        ss.read((char*)&read, sizeof(AeroJet::u2));

        if (read == 2)
        {
            return 0;
        }
    }

    {
        AeroJet::u2 read = 0;
        ss.read((char*)&read, sizeof(AeroJet::u4));

        if (read == 4)
        {
            return 0;
        }
    }

    {
        AeroJet::u2 read = 0;
        ss.read((char*)&read, sizeof(AeroJet::u8));

        if (read == 8)
        {
            return 0;
        }
    }

    return 1;
}