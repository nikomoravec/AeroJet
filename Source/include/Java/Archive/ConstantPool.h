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
        friend class Class;

    public:
        ConstantPool(const JVM::u2 InSize)
        {
            entries.reserve(InSize - 1);
        }

        JVM::u2 getSize() const
        {
            return entries.size();
        }

        const std::shared_ptr<ConstantPoolEntry> get(uint32_t index) const
        {
            if (index >= getSize())
            {
                /*
                 * TODO: replace it with dedicated exception type IndexOutOfException
                 * @Author: Nikita Miroshnichenko (nikita.miroshnichenko@yahoo.com)
                 */
                throw std::runtime_error("Index out of bound!");
            }

            return entries[index];
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
