#ifndef SUPERJET_TYPES_H
#define SUPERJET_TYPES_H

namespace SuperJet::Java::JVM
{
    using i1 = char;
    using i2 = short;
    using i4 = int;

    using u1 = unsigned char;
    using u2 = unsigned short;
    using u4 = unsigned int;

    static_assert(sizeof(u1) == 1);
    static_assert(sizeof(u2) == 2);
    static_assert(sizeof(u4) == 4);

    static_assert(sizeof(i1) == 1);
    static_assert(sizeof(i2) == 2);
    static_assert(sizeof(i4) == 4);
}

#endif //SUPERJET_TYPES_H
