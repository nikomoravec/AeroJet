#pragma once

#include "Assertion.hpp"
#include "Exceptions/RuntimeException.hpp"
#include "fmt/format.h"
#include "Types.hpp"

#include <array>
#include <vector>

namespace AeroJet::Stream
{
    template<typename T>
    constexpr bool isStandardInputStream()
    {
        return std::is_same_v<T, std::istream> ||
               std::is_same_v<T, std::istringstream> ||
               std::is_same_v<T, std::ifstream> ||
               std::is_same_v<T, std::wistream> ||
               std::is_same_v<T, std::wistringstream> ||
               std::is_same_v<T, std::wifstream>;
    }

    template<typename T>
    constexpr bool isStandardOutputStream()
    {
        return std::is_same_v<T, std::ostream> ||
               std::is_same_v<T, std::ostringstream> ||
               std::is_same_v<T, std::ofstream> ||
               std::is_same_v<T, std::wostream> ||
               std::is_same_v<T, std::wostringstream> ||
               std::is_same_v<T, std::wofstream>;
    }

    template<typename T>
    constexpr bool isStandardInputOutputStream()
    {
        return std::is_same_v<T, std::stringstream> ||
               std::is_same_v<T, std::fstream> ||
               std::is_same_v<T, std::wstringstream> ||
               std::is_same_v<T, std::wfstream>;
    }

    enum class StreamType : u1
    {
        INPUT,
        OUTPUT,
        INPUT_OUTPUT
    };

    enum class ByteOrder : u1
    {
        NORMAL,
        REVERSE
    };

    template<class T>
    concept StandardInputStream = isStandardInputStream<T>();

    template<class T>
    concept StandardOutputStream = isStandardOutputStream<T>();

    template<class T>
    concept StandardInputOutputStream = isStandardInputOutputStream<T>();

    template<typename TStream, ByteOrder ReadMode = ByteOrder::NORMAL, ByteOrder WriteMode = ByteOrder::NORMAL>
    class StandardStreamWrapper;

    template<typename T, typename TStream, ByteOrder ReadMode = ByteOrder::NORMAL, ByteOrder WriteMode = ByteOrder::NORMAL>
    concept Readable = requires(StandardStreamWrapper<TStream, ReadMode, WriteMode>& stream) {
        {
            T::read(stream)
        } -> std::same_as<T>;
    };

    template<typename T, typename TStream, ByteOrder ReadMode = ByteOrder::NORMAL, ByteOrder WriteMode = ByteOrder::NORMAL>
    concept Writable = requires(T& object, StandardStreamWrapper<TStream, ReadMode, WriteMode>& stream) {
        {
            object.write(stream)
        } -> std::same_as<void>;
    };

    template<typename TStream>
    concept ClosableStream = requires(TStream& stream) {
        {
            stream.close()
        } -> std::same_as<void>;
    };

    template<typename TStream>
    concept OpenableStream = requires(TStream& stream) {
        {
            stream.is_open()
        } -> std::same_as<bool>;
    };

    template<typename TStream, ByteOrder ReadMode, ByteOrder WriteMode>
    class StandardStreamWrapper final
    {
      public:
        using CharType = typename TStream::char_type;

        template<class... Args>
        constexpr StandardStreamWrapper(Args&&... args) requires StandardInputStream<TStream> || StandardInputOutputStream<TStream>
            :
            m_stream(args...)
        {
            seekRead(0, std::ios::end);
            m_size = readPosition();
            seekRead(0, std::ios::beg);
        }

        template<class... Args>
        constexpr StandardStreamWrapper(Args&&... args) requires StandardOutputStream<TStream>
            :
            m_stream(args...),
            m_size(0)
        {
        }

        ~StandardStreamWrapper() = default;

        ~StandardStreamWrapper() requires ClosableStream<TStream>
        {
            close();
        }

        [[nodiscard]] constexpr StreamType type() const requires StandardInputStream<TStream>
        {
            return StreamType::INPUT;
        }

        [[nodiscard]] constexpr StreamType type() const requires StandardOutputStream<TStream>
        {
            return StreamType::OUTPUT;
        }

        [[nodiscard]] constexpr StreamType type() const requires StandardInputOutputStream<TStream>
        {
            return StreamType::INPUT_OUTPUT;
        }

        [[nodiscard]] inline bool isOpen() requires OpenableStream<TStream>
        {
            return m_stream.is_open();
        }

        inline void close() requires ClosableStream<TStream>
        {
            if(isOpen())
            {
                m_stream.close();
            }
        }

        [[nodiscard]] inline std::size_t readPosition() requires StandardInputStream<TStream> || StandardInputOutputStream<TStream>
        {
            return m_stream.tellg();
        }

        inline void seekRead(std::size_t position, std::ios_base::seekdir seekType = std::ios::ios_base::cur) requires StandardInputStream<TStream> || StandardInputOutputStream<TStream>
        {
            m_stream.seekg(position, seekType);
        }

        [[nodiscard]] inline std::size_t size() const
        {
            return m_size;
        }

        [[nodiscard]] inline bool good()
        {
            return m_stream.good();
        }

        [[nodiscard]] inline bool bad()
        {
            return m_stream.bad();
        }

        [[nodiscard]] inline bool fail()
        {
            return m_stream.fail();
        }

        inline void clear()
        {
            return m_stream.clear();
        }

        [[nodiscard]] inline bool eof()
        {
            return m_stream.eof();
        }

        template<typename T>
        [[nodiscard]] inline T read() requires(StandardInputStream<TStream> || StandardInputOutputStream<TStream>) && std::is_fundamental_v<T>
        {
            return readInternal<T>();
        }

        template<typename T, std::size_t size>
        [[nodiscard]] inline std::array<T, size> readSome() requires(StandardInputStream<TStream> || StandardInputOutputStream<TStream>) && std::is_fundamental_v<T>
        {
            std::array<T, size> array{};
            for(std::size_t index = 0; index < size; index++)
            {
                array[index] = read<T>();
            }

            return array;
        }

        template<typename T>
        [[nodiscard]] inline std::vector<T> readSome(std::size_t count)
            requires(StandardInputStream<TStream> || StandardInputOutputStream<TStream>) && std::is_fundamental_v<T>
        {
            std::vector<T> vector{};
            vector.reserve(count);
            for(std::size_t index = 0; index < count; index++)
            {
                vector.emplace_back(read<T>());
            }

            return vector;
        }

        template<typename T>
        [[nodiscard]] inline std::vector<T> readSome(std::size_t count)
            requires(StandardInputStream<TStream> || StandardInputOutputStream<TStream>) && Readable<T, TStream, ReadMode, WriteMode>
        {
            std::vector<T> vector{};
            vector.reserve(count);
            for(std::size_t index = 0; index < count; index++)
            {
                vector.emplace_back(T::read(*this));
            }

            return vector;
        }

        template<typename T, std::size_t size>
        [[nodiscard]] inline std::array<T, size> readSome() requires(StandardInputStream<TStream> || StandardInputOutputStream<TStream>) && Readable<T, TStream, ReadMode, WriteMode>
        {
            std::array<T, size> array{};
            for(std::size_t index = 0; index < size; index++)
            {
                array[index] = T::read(*this);
            }

            return array;
        }

        template<typename T>
        [[nodiscard]] inline T read() requires(StandardInputStream<TStream> || StandardInputOutputStream<TStream>) && Readable<T, TStream, ReadMode, WriteMode>
        {
            return T::read(*this);
        }

      private:
        template<typename T>
        [[nodiscard]] static T swapEndian(const T& data)
        {
            static_assert(CHAR_BIT == 8, "CHAR_BIT != 8");

            union ByteObjectRepresentation
            {
                T object;
                std::array<u1, sizeof(T)> bytes;
            };

            ByteObjectRepresentation source{};
            ByteObjectRepresentation dst{};

            source.object = data;

            for(size_t k = 0; k < sizeof(T); k++)
            {
                dst.bytes[k] = source.bytes[sizeof(T) - k - 1];
            }

            return dst.object;
        }

        template<typename T>
        [[nodiscard]] inline T readInternal() requires std::is_fundamental_v<T>
        {
            AEROJET_VERIFY_THROW(isOpen(), Exceptions::RuntimeException, "Stream is not open!");
            AEROJET_VERIFY_THROW(good(), Exceptions::RuntimeException, "Internal stream fatal error occured!");

            const std::size_t readSize = sizeof(T);
            const std::size_t readPos = readPosition();

            AEROJET_VERIFY_THROW(!eof(), Exceptions::RuntimeException, fmt::format("stream EOF at {:#08x}! Read size was {}", readPosition(), readSize));
            AEROJET_VERIFY_THROW(readPos + readSize <= m_size, Exceptions::RuntimeException, fmt::format("Attempt to read {} bytes while {} is available", readSize, m_size - readPos));

            T read{};
            m_stream.read(reinterpret_cast<CharType*>(&read), readSize);
            AEROJET_VERIFY_THROW(!fail(), Exceptions::RuntimeException, "Stream failed to perform last read operation!");

            if constexpr(ReadMode == ByteOrder::REVERSE)
            {
                read = swapEndian(read);
            }

            return read;
        }

        template<typename T>
        inline void writeInternal(T data) requires StandardOutputStream<TStream> || StandardInputOutputStream<TStream>
        {
            if constexpr(WriteMode == ByteOrder::REVERSE)
            {
                data = swapEndian(data);
            }

            std::size_t size = sizeof(data);

            AEROJET_VERIFY_THROW(isOpen(), Exceptions::RuntimeException, "Stream is not open!");
            AEROJET_VERIFY_THROW(good(), Exceptions::RuntimeException, "Internal stream fatal error occured!");

            m_stream.write(reinterpret_cast<CharType*>(&data), size);
            m_size += size;

            AEROJET_VERIFY_THROW(!fail(), Exceptions::RuntimeException, "Stream failed to perform last write operation!");
        }

        template<typename T>
        inline void write(T object) requires(StandardOutputStream<TStream> || StandardInputOutputStream<TStream>) && std::is_fundamental_v<T>
        {
            writeInternal<T>(object);
        }

        template<typename T>
        inline void write(T object) requires(StandardOutputStream<TStream> || StandardInputOutputStream<TStream>) && Writable<T, TStream, ReadMode, WriteMode>
        {
            object.write(*this);
        }

        template<typename T>
        inline void writeSome(const std::vector<T>& items) requires(StandardOutputStream<TStream> || StandardInputOutputStream<TStream>) && Writable<T, TStream, ReadMode, WriteMode>
        {
            for(const T& item : items)
            {
                item.write(*this);
            }
        }

        template<typename T>
        inline void writeSome(const std::vector<T>& items) requires(StandardOutputStream<TStream> || StandardInputOutputStream<TStream>) && std::is_fundamental_v<T>
        {
            for(T item : items)
            {
                writeInternal<T>(item);
            }
        }

      private:
        TStream m_stream;
        std::size_t m_size;
    };
} // namespace AeroJet::Stream
