#ifndef SUPERJET_TYPES_H
#define SUPERJET_TYPES_H

namespace SuperJet::Java::JVM
{
    using i1 = signed char;
    using i2 = signed short;
    using i4 = signed int;
    using i8 = signed long long;

    using u1 = unsigned char;
    using u2 = unsigned short;
    using u4 = unsigned int;
    using u8 = unsigned long long;

    static_assert(sizeof(u1) == 1);
    static_assert(sizeof(u2) == 2);
    static_assert(sizeof(u4) == 4);
    static_assert(sizeof(u8) == 8);

    static_assert(sizeof(i1) == 1);
    static_assert(sizeof(i2) == 2);
    static_assert(sizeof(i4) == 4);
    static_assert(sizeof(i8) == 8);
}

#endif //SUPERJET_TYPES_H
