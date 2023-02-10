#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "AeroJet.hpp"
#include "doctest.h"

/*
 * @Retention(RetentionPolicy.RUNTIME)
 * @interface MyAnnotation
 * {
 *   String stringValue() default "defaultString";
 *   int intValue() default 101;
 * }
 *
 * class RuntimeVisibleAnnotationsTest
 * {
 *   @MyAnnotation
 *   int myInt = 0;
 * }
 */

TEST_CASE("AeroJet::Java::Attributes::RuntimeVisibleAnnotations")
{
    std::ifstream inputFileStream{ "Resources/RuntimeVisibleAnnotationsTest.class" };
    REQUIRE(inputFileStream.is_open());

    AeroJet::Java::ClassFile::ClassInfo classInfo =
        AeroJet::Stream::Reader::read<AeroJet::Java::ClassFile::ClassInfo>(inputFileStream,
                                                                           AeroJet::Stream::ByteOrder::INVERSE);

    const AeroJet::Java::ClassFile::ConstantPool& constantPool = classInfo.constantPool();
    REQUIRE_EQ(classInfo.fields().size(), 1);

    const AeroJet::Java::ClassFile::FieldInfo& fieldInfo = classInfo.fields()[0];
    REQUIRE_EQ(fieldInfo.attributes().size(), 1);

    const AeroJet::Java::ClassFile::AttributeInfo& attributeInfo = fieldInfo.attributes()[0];
    REQUIRE_EQ(AeroJet::Java::ClassFile::Utils::AttributeInfoUtils::extractName(constantPool, attributeInfo),
               AeroJet::Java::ClassFile::RuntimeVisibleAnnotations::RUNTIME_VISIBLE_ANNOTATIONS_ATTRIBUTE_NAME);

    AeroJet::Java::ClassFile::RuntimeVisibleAnnotations runtimeVisibleAnnotationsAttribute =
        AeroJet::Java::ClassFile::RuntimeVisibleAnnotations{ constantPool, attributeInfo };

    REQUIRE_EQ(runtimeVisibleAnnotationsAttribute.numAnnotation(), 1);

    const AeroJet::Java::ClassFile::Annotation annotation = runtimeVisibleAnnotationsAttribute.annotations()[0];

    // #8 = Utf8               LMyAnnotation;
    REQUIRE_EQ(annotation.typeIndex(), 8);
    REQUIRE_EQ(
        constantPool.getEntry(annotation.typeIndex()).as<AeroJet::Java::ClassFile::ConstantPoolInfoUtf8>().asString(),
        "LMyAnnotation;");
    REQUIRE_EQ(annotation.numElementValuePairs(), 0);
}