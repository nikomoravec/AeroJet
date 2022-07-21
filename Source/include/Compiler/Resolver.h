#ifndef SUPERJET_RESOLVER_H
#define SUPERJET_RESOLVER_H

namespace SuperJet::Compiler
{
    template<typename TResolvingType, typename TResolvedType>
    class Resolver
    {
    public:
        Resolver(const TResolvingType& inResolvingObject) : resolvingObject(inResolvingObject)
        {
        }

        virtual ~Resolver() = default;

        virtual TResolvedType resolve() const = 0;

    protected:
        TResolvingType resolvingObject;
    };
}

#endif //SUPERJET_RESOLVER_H
