#ifndef SUPERJET_CONSTANTPOOL_H
#define SUPERJET_CONSTANTPOOL_H

#include <filesystem>
#include <fstream>
#include <memory>
#include <map>

#include "fmt/format.h"
#include "Compiler/Exceptions/RuntimeException.h"
#include "Java/Archive/ConstantPoolEntry.h"
#include "Java/Types.h"

namespace SuperJet::Java::Archive
{
    class ConstantPool
    {
        friend class ClassInfo;

    public:
        ConstantPool(const JVM::u2 InSize)
        {
        }

        JVM::u2 getSize() const
        {
            return entries.size() + 1;
        }

        template<typename T = ConstantPoolEntry>
        std::shared_ptr<T> get(int64_t index) const requires std::is_base_of_v<ConstantPoolEntry, T>
        {
            if (entries.at(index) == nullptr)
            {
                throw Compiler::RuntimeException(fmt::format("Element {} is null!", index));
            }

            return std::static_pointer_cast<T>(entries.at(index));
        }

        void addEntry(int64_t index, std::shared_ptr<ConstantPoolEntry> entry)
        {
            if (entry == nullptr)
            {
                throw std::invalid_argument("parameter \'entry\' can not be null!");
            }

            entries[index] = entry;
        }

        auto begin() const
        {
            return entries.begin();
        }

        auto end() const
        {
            return entries.end();
        }


    protected:
        std::map<int64_t, std::shared_ptr<ConstantPoolEntry>> entries;
    };
}

#endif //SUPERJET_CONSTANTPOOL_H
