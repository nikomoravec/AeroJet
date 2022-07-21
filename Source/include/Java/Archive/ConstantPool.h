#ifndef SUPERJET_CONSTANTPOOL_H
#define SUPERJET_CONSTANTPOOL_H

#include <filesystem>
#include <fstream>
#include <memory>

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
            entries.reserve(InSize - 1);
        }

        JVM::u2 getSize() const
        {
            return entries.size() + 1;
        }

        template<typename T = ConstantPoolEntry>
        std::shared_ptr<T> get(int32_t index) const requires std::is_base_of_v<ConstantPoolEntry, T>
        {
            index = index - 1; // indexes in Constant Pool in Java starts from 1
            if (index >= getSize() || index < 0)
            {
                /*
                 * TODO: replace it with dedicated exception type IndexOutOfException
                 * @Author: Nikita Miroshnichenko (nikita.miroshnichenko@yahoo.com)
                 */
                throw std::runtime_error("Index out of bound!");
            }

            return std::static_pointer_cast<T>(entries[index]);
        }

        void addEntry(std::shared_ptr<ConstantPoolEntry> entry)
        {
            if (entry == nullptr)
            {
                throw std::invalid_argument("parameter \'entry\' can not be null!");
            }

            entries.emplace_back(entry);
        }

    protected:
        std::vector<std::shared_ptr<ConstantPoolEntry>> entries;
    };
}

#endif //SUPERJET_CONSTANTPOOL_H
