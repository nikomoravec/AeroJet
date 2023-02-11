/*
 * ParameterAnnotation.cpp
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

#include "Java/ClassFile/Attributes/Annotation/ParameterAnnotation.hpp"

#include "Stream/Reader.hpp"

namespace AeroJet::Java::ClassFile
{
    ParameterAnnotation::ParameterAnnotation(const std::vector<Annotation>& annotations) : m_annotations(annotations) {}

    u2 ParameterAnnotation::numAnnotations() const
    {
        return m_annotations.size();
    }
    const std::vector<Annotation>& ParameterAnnotation::annotations() const
    {
        return m_annotations;
    }
} // namespace AeroJet::Java::ClassFile

template<>
AeroJet::Java::ClassFile::ParameterAnnotation AeroJet::Stream::Reader::read(std::istream& stream, ByteOrder byteOrder)
{
    std::vector<AeroJet::Java::ClassFile::Annotation> annotations;

    const u2 numAnnotations = AeroJet::Stream::Reader::read<u2>(stream, byteOrder);
    annotations.reserve(numAnnotations);

    for(u2 annotationIndex = 0; annotationIndex < numAnnotations; annotationIndex++)
    {
        annotations.emplace_back(
            AeroJet::Stream::Reader::read<AeroJet::Java::ClassFile::Annotation>(stream, byteOrder));
    }

    return AeroJet::Java::ClassFile::ParameterAnnotation{annotations};
}
