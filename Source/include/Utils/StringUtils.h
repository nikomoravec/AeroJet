#ifndef SUPERJET_UTILS_STRING_H
#define SUPERJET_UTILS_STRING_H

#include <string>
#include <vector>

namespace SuperJet::Utils
{
    class StringUtils
    {
    public:
        static std::vector<std::string> split(const std::string& str, const char delim)
        {
            std::vector<std::string> result;

            size_t start;
            size_t end = 0;

            while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
            {
                end = str.find(delim, start);
                result.push_back(str.substr(start, end - start));
            }

            return result;
        }

        static std::string replaceAll(const std::string& str, const std::string& search, const std::string& replace)
        {
            std::string result = str;

            size_t pos = 0;
            for (size_t pos = 0; pos < str.size(); pos += replace.length())
            {
                pos = result.find( search, pos);
                if (pos == std::string::npos )
                {
                    break;
                }

                result.erase(pos, search.length());
                result.insert(pos, replace);
            }

            return result;
        }
    };
}

#endif // SUPERJET_UTILS_STRING_H