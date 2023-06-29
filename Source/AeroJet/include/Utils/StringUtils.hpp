/*
 * StringUtils.hpp
 *
 * Copyright © 2023 AeroJet Developers. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <string>
#include <vector>

namespace AeroJet::Utils
{
    class StringUtils
    {
      public:
        static void trimLeft(std::string& string)
        {
            string.erase(
                string.begin(),
                std::find_if(string.begin(), string.end(), [](unsigned char ch)
                             { return !std::isspace(ch); }));
        }

        static void trimRight(std::string& string)
        {
            string.erase(
                std::find_if(string.rbegin(), string.rend(), [](unsigned char ch)
                             { return !std::isspace(ch); })
                    .base(),
                string.end());
        }

        static void trim(std::string& string)
        {
            trimLeft(string);
            trimRight(string);
        }

        static std::vector<std::string> split(const std::string& string, char delimiter)
        {
            std::vector<std::string> split;

            size_t start;
            size_t end = 0;
            while((start = string.find_first_not_of(delimiter, end)) != std::string::npos)
            {
                end = string.find(delimiter, start);
                split.push_back(string.substr(start, end - start));
            }

            return split;
        }
    };
} // namespace AeroJet::Utils
