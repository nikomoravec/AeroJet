#include "Compiler/Compiler.h"
#include "Compiler/ClassInfoRerenceCollector.h"
#include "Compiler/Exceptions/RuntimeException.h"
#include "Compiler/CodeGen/Cpp/AssignOperation.h"
#include "Compiler/CodeGen/Cpp/Boolean.h"
#include "Compiler/CodeGen/Cpp/Class.h"
#include "Compiler/CodeGen/Cpp/Document.h"
#include "Compiler/CodeGen/Cpp/ForwardDeclaration.h"
#include "Compiler/CodeGen/Cpp/Function.h"
#include "Compiler/CodeGen/Cpp/NullPtr.h"
#include "Compiler/CodeGen/Cpp/Number.h"
#include "Compiler/CodeGen/Cpp/Variable.h"
#include "Compiler/CodeGen/Cpp/WideCharacter.h"
#include "Java/Archive/FieldDescriptor.h"
#include "Java/Archive/Attributes/ConstantValue.h"
#include "spdlog/spdlog.h"
#include "fmt/format.h"
#include "Java/Archive/Archive.h"
#include "Java/Archive/Attributes/Code.h"
#include "Utils/StringUtils.h"

#include <fstream>

namespace SuperJet::Compiler
{
    Context::Context(const Environment& env) : environment(env)
    {
        for(const std::string& library : environment.libraries)
        {
            std::shared_ptr<SuperJet::Java::Archive::Jar> jar = std::make_shared<SuperJet::Java::Archive::Jar>(Java::Archive::Jar(library));

            for (ssize_t entryIndex = 0; entryIndex < jar->count(); entryIndex++)
            {
                const SuperJet::Java::Archive::Jar::Entry& entry = jar->open(entryIndex);
                if (!entry.isDirectory())
                {
                    if (std::filesystem::path(entry.getName()).extension() == Java::Archive::CLASS_FILE_EXTENSION)
                    {
                        const size_t extensionPosition = entry.getName().find(Java::Archive::CLASS_FILE_EXTENSION);
                        std::string entryNameWithoutExtension = entry.getName().substr(0, extensionPosition);

                        spdlog::debug(fmt::format("Found class \'{}\' in archive \'{}\'", entryNameWithoutExtension, jar->getLocation().string()));
                        classMap.emplace(entryNameWithoutExtension, jar);
                    }
                }
            }
        }
    }

    const std::filesystem::path& Context::getMainClass() const
    {
        return environment.mainClass;
    }

    std::shared_ptr<Java::Archive::Jar> Context::jarForClass(const std::filesystem::path& javaClassName) const
    {
        auto find = classMap.find(javaClassName);
        if (find != classMap.end())
        {
            return classMap.at(javaClassName);
        }

        throw SuperJet::Compiler::RuntimeException(fmt::format("Can't find class \'{}\' in any of jar archives!", javaClassName.string()));
    }

    void ByteCodeCompiler::run()
    {
        std::shared_ptr<Compiler::CodeGen::Cpp::Document> document = std::make_shared<Compiler::CodeGen::Cpp::Document>();
        std::queue<std::shared_ptr<Java::Archive::ClassInfo>> nodes = dependencyGraph.topology();

        for(std::shared_ptr<Java::Archive::ClassInfo> node : dependencyGraph.nodes())
        {
            const std::string& fullJavaClassName = node->getName();

            if (node->isUnderPackage())
            {
                
                std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Namespace> ns = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Namespace>(fullJavaClassName);

                std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Class> clazz = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Class>(node->getClassName());
                ns->addForwardDeclaration(std::make_shared<SuperJet::Compiler::CodeGen::Cpp::ForwardDeclaration>((clazz)));
                document->addNameSpace(ns);
            }
            else
            {
                std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Class> clazz = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Class>(fullJavaClassName);
                document->addForwardDeclaration(std::make_shared<SuperJet::Compiler::CodeGen::Cpp::ForwardDeclaration>((clazz)));
            }
        }

        std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Namespace> jvmNameSpace = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Namespace>("jvm", false);
        std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Class> arrayClass =  std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Class>("array");
        
        std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Template> arrayClassTemplate = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Template>();
        arrayClassTemplate->addTypeName(std::make_shared<SuperJet::Compiler::CodeGen::Cpp::TypeName>("T"));
        arrayClass->setTemplate(arrayClassTemplate);

        arrayClass->addVariable(
            std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Variable>(
                std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("T", SuperJet::Compiler::CodeGen::Cpp::Type::Flags::POINTER), "data"
        ));

        arrayClass->addVariable(
            std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Variable>(
                std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("int"), "length",
                std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Number>(0)
            )
        );


        jvmNameSpace->addClass(arrayClass);
        document->addNameSpace(jvmNameSpace);

        while (!nodes.empty())
        {
            std::shared_ptr<Java::Archive::ClassInfo> node = nodes.front();
            
            std::shared_ptr<SuperJet::Compiler::CodeGen::Node> codegenNode = document;
            if (node->isUnderPackage())
            {
                std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Namespace> ns = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Namespace>(node->getName());
                document->addNameSpace(ns);
                codegenNode = ns;
            }

            const SuperJet::Java::Archive::ConstantPool& constantPool = node->getConstantPool();

            std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Class> clazz = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Class>(node->getClassName());
            codegenNode->addNode(clazz);
            
            if (node->isSuperClassPresented())
            {
                Java::JVM::u2 superClassIndex = node->getSuperClass();
                std::shared_ptr<SuperJet::Java::Archive::ConstantPoolInfoClass> superClassInfo = constantPool.get<SuperJet::Java::Archive::ConstantPoolInfoClass>(superClassIndex);
                Java::JVM::u2 superClassNameIndex = superClassInfo->getNameIndex();
                std::string superClassName = constantPool.get<SuperJet::Java::Archive::ConstantPoolInfoUtf8>(superClassNameIndex)->asString();

                std::string superClassTypeName = SuperJet::Utils::StringUtils::replaceAll(superClassName, "/", "::");

                std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Type> superClassType = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>(superClassTypeName, SuperJet::Compiler::CodeGen::Cpp::Type::Flags::POINTER);

                clazz->addVariable(
                    std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Variable>(
                        superClassType, 
                        "super", 
                        std::make_shared<SuperJet::Compiler::CodeGen::Cpp::NullPtr>()
                    )
                );
            }

            std::map<std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Variable>, std::shared_ptr<SuperJet::Compiler::CodeGen::Node>> staticVariables;
            std::map<std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Variable>, std::shared_ptr<SuperJet::Compiler::CodeGen::Node>> nonStaticVariables;

            const std::vector<Java::Archive::FieldInfo>& fields = node->getFields();
            for (const Java::Archive::FieldInfo& field : fields)
            {
                Java::JVM::u2 fieldNameIndex = field.getNameIndex();
                const std::string& fieldName = constantPool.get<Java::Archive::ConstantPoolInfoUtf8>(fieldNameIndex)->asString();

                Java::JVM::u2 fieldDescriptorIndex = field.getDescriptorIndex();
                const Java::Archive::FieldDescriptor& fieldDescriptor = Java::Archive::FieldDescriptor(constantPool.get<SuperJet::Java::Archive::ConstantPoolInfoUtf8>(fieldDescriptorIndex)->asString());
                
                std::shared_ptr<SuperJet::Compiler::CodeGen::Node> fieldDefaultValue = nullptr;
                const std::vector<Java::Archive::AttributeInfo>& fieldAttributes = field.getAttributes();
                if (!fieldAttributes.empty())
                {
                    for (const Java::Archive::AttributeInfo& fieldAttributeInfo : fieldAttributes)
                    {
                        const std::string& fieldAttributeName = constantPool.get<SuperJet::Java::Archive::ConstantPoolInfoUtf8>(fieldAttributeInfo.getAttributeNameIndex())->asString();
                        if (fieldAttributeName == Java::Archive::Attributes::ConstantValue::CONSTANT_VALUE_ATTRIBUTE_NAME)
                        {
                            const Java::Archive::Attributes::ConstantValue& constantValueAttribute = Java::Archive::Attributes::ConstantValue(constantPool, fieldAttributeInfo);
                            std::shared_ptr<Java::Archive::ConstantPoolEntry> valueEntry = constantPool.get(constantValueAttribute.getConstantValueIndex());
                            
                            switch (fieldDescriptor.getFieldType())
                            {
                                case Java::Archive::FieldDescriptor::FieldType::LONG:
                                    fieldDefaultValue = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Number>(std::static_pointer_cast<Java::Archive::ConstantPoolInfoLong>(valueEntry)->asLong());
                                    break;
                                case Java::Archive::FieldDescriptor::FieldType::FLOAT:
                                    fieldDefaultValue = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Number>(std::static_pointer_cast<Java::Archive::ConstantPoolFloat>(valueEntry)->asFloat());
                                    break;
                                case Java::Archive::FieldDescriptor::FieldType::DOUBLE:
                                    fieldDefaultValue = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Number>(std::static_pointer_cast<Java::Archive::ConstantPoolInfoDouble>(valueEntry)->asDouble());
                                    break;
                                case Java::Archive::FieldDescriptor::FieldType::INTEGER:
                                    fieldDefaultValue = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Number>(std::static_pointer_cast<Java::Archive::ConstantPoolInfoInteger>(valueEntry)->asInteger());
                                    break;
                                case Java::Archive::FieldDescriptor::FieldType::BOOLEAN:
                                    fieldDefaultValue = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Boolean>(std::static_pointer_cast<Java::Archive::ConstantPoolInfoInteger>(valueEntry)->asBool());
                                    break;
                                case Java::Archive::FieldDescriptor::FieldType::BYTE:
                                    fieldDefaultValue = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Number>(std::static_pointer_cast<Java::Archive::ConstantPoolInfoInteger>(valueEntry)->asByte());
                                    break;
                                case Java::Archive::FieldDescriptor::FieldType::SHORT:
                                    fieldDefaultValue = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Number>(std::static_pointer_cast<Java::Archive::ConstantPoolInfoInteger>(valueEntry)->asShort());
                                    break;
                                case Java::Archive::FieldDescriptor::FieldType::CHAR:
                                    fieldDefaultValue = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Character>(std::static_pointer_cast<Java::Archive::ConstantPoolInfoInteger>(valueEntry)->asChar());
                                    break;
                                case Java::Archive::FieldDescriptor::FieldType::CLASS:
                                {
                                    assert(fieldDescriptor.getClassName() == "java/lang/String");

                                    const Java::JVM::u2 stringIndex = std::static_pointer_cast<Java::Archive::ConstantPoolInfoString>(valueEntry)->getStringIndex();
                                    const std::string& str = constantPool.get<SuperJet::Java::Archive::ConstantPoolInfoUtf8>(stringIndex)->asString();
                                    
                                    /*
                                     * TODO: Unfortunately string in not just string literal.
                                     * It should be somehting like function call on JVM object instance
                                     * for example jvm->allocate<java::lang::String>(classInfo) 
                                     * @Author: Nikita Miroshnichenko (nikita.miroshnichenko@yahoo.com)
                                    */

                                    break;
                                }
                                case Java::Archive::FieldDescriptor::FieldType::ARRAY:
                                default:
                                    throw Compiler::RuntimeException(fmt::format("Type {} can not have ConstantValue attribute!", fieldDescriptor.getRawLiteral()));
                            }
                        }
                    }
                }

                std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Type> fieldType = nullptr;
                SuperJet::Compiler::CodeGen::Cpp::Type::Flags fieldTypeFlags = fieldDescriptor.isPrimitive() ? SuperJet::Compiler::CodeGen::Cpp::Type::Flags::NONE : SuperJet::Compiler::CodeGen::Cpp::Type::Flags::POINTER;

                if (field.isStatic())
                {
                    fieldTypeFlags = fieldTypeFlags | SuperJet::Compiler::CodeGen::Cpp::Type::Flags::STATIC;
                }

                std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Variable> memberVariable = nullptr;
                if (fieldDescriptor.isPrimitive())
                {
                    fieldType = javaPrimitiveToCppType(fieldDescriptor, fieldTypeFlags);
                }
                else
                {
                    fieldTypeFlags = fieldTypeFlags | SuperJet::Compiler::CodeGen::Cpp::Type::Flags::POINTER;

                    if (fieldDescriptor.isClass())
                    {
                        std::string className = fieldDescriptor.getClassName().value();
                        std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Namespace> ns = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Namespace>(className);
                        std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Class> memberClass = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Class>(SuperJet::Utils::StringUtils::split(className, '/').back());
                        ns->addClass(memberClass);
                        fieldType = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>(memberClass->getFullName(), fieldTypeFlags);
                        memberVariable = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Variable>(fieldType, className);
                    }
                    if (fieldDescriptor.isArray())
                    {
                        fieldType = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("jvm::array", fieldTypeFlags);;
                        std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Type> currentType = fieldType;
                        Java::Archive::FieldDescriptor fd = fieldDescriptor;
                        while (fd.getUnderlyingType().has_value())
                        {
                            Java::Archive::FieldDescriptor underlyingTypeDescriptor = fd.getUnderlyingType().value();
                            std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Type> underlyingType = nullptr;
                            SuperJet::Compiler::CodeGen::Cpp::Type::Flags underlyingTypeFlags = underlyingTypeDescriptor.isPrimitive() ? SuperJet::Compiler::CodeGen::Cpp::Type::Flags::NONE : SuperJet::Compiler::CodeGen::Cpp::Type::Flags::POINTER;

                            if (underlyingTypeDescriptor.isPrimitive())
                            {
                                underlyingType = javaPrimitiveToCppType(underlyingTypeDescriptor);
                            }
                            else if (underlyingTypeDescriptor.isClass())
                            {
                                std::string className = underlyingTypeDescriptor.getClassName().value();
                                std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Namespace> ns = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Namespace>(className);
                                std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Class> c = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Class>(SuperJet::Utils::StringUtils::split(className, '/').back());
                                ns->addClass(c);
                                underlyingType = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>(c->getFullName(), underlyingTypeFlags);
                            }
                            else if (underlyingTypeDescriptor.isArray())
                            {
                                underlyingType = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("jvm::array", underlyingTypeFlags);
                            }

                            currentType->addTemplateArgument(underlyingType);
                            currentType = underlyingType;

                            fd = fd.getUnderlyingType().value();
                        }                        
                    }
                }

                memberVariable = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Variable>(fieldType, fieldName);
                clazz->addVariable(memberVariable);

                if (fieldDefaultValue == nullptr)
                {
                    fieldDefaultValue = getDefaultValue(fieldDescriptor);
                }

                if (fieldType->isStatic())
                {
                    staticVariables.insert({memberVariable, fieldDefaultValue});
                }
                else
                {
                    nonStaticVariables.insert({memberVariable, fieldDefaultValue});
                }
            }

            std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Functuon> staticInitializerFunction = 
            std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Functuon>(
                "DEFAULT_STATIC_INIT", 
                std::vector<std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::FunctionArgument>>(), 
                std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("void", SuperJet::Compiler::CodeGen::Cpp::Type::Flags::STATIC)
            );

            for (const auto& pair : staticVariables)
            {
                staticInitializerFunction->addNode(std::make_shared<SuperJet::Compiler::CodeGen::Cpp::VarialeAssignOperation>(pair.first, pair.second));
            }

            clazz->addNode(staticInitializerFunction);


            std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Functuon> nonStaticInitializerFunction = 
            std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Functuon>(
                "DEFAULT_INIT", 
                std::vector<std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::FunctionArgument>>(), 
                std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("void")
            );

            for (const auto& pair : nonStaticVariables)
            {
                nonStaticInitializerFunction->addNode(std::make_shared<SuperJet::Compiler::CodeGen::Cpp::VarialeAssignOperation>(pair.first, pair.second));
            }

            clazz->addNode(nonStaticInitializerFunction);

            for (const Java::Archive::MethodInfo& methodInfo : node->getMethods())
            {
                const Java::Archive::MethodInfo::AccessFlags accessFlags = methodInfo.getAccessFlags();

                const Java::JVM::u2 nameIndex = methodInfo.getNameIndex();
                const std::string& methodName = constantPool.get<SuperJet::Java::Archive::ConstantPoolInfoUtf8>(nameIndex)->asString();

                const Java::JVM::u2 descriptorIndex = methodInfo.getDescriptorIndex();
                const std::string& descriptorString = constantPool.get<SuperJet::Java::Archive::ConstantPoolInfoUtf8>(descriptorIndex)->asString();

                const std::vector<Java::Archive::AttributeInfo>& methodAttributes = methodInfo.getAttributes();
                for (const Java::Archive::AttributeInfo& methodAttribute : methodAttributes)
                {
                    if (constantPool.get<SuperJet::Java::Archive::ConstantPoolInfoUtf8>(methodAttribute.getAttributeNameIndex())->asString() == Java::Archive::Attributes::Code::CODE_ATTRIBUTE_NAME)
                    {
                        const Java::Archive::Attributes::Code& codeAttribute = Java::Archive::Attributes::Code(constantPool, methodAttribute);
                        const std::vector<std::shared_ptr<Java::JVM::Runtime::Operation>>& code = codeAttribute.getCode();
                    }
                }
            }

            nodes.pop();
        }
        
        std::ofstream outputStream("codegen.cpp");
        document->dump(outputStream);

        return;
    }

    ByteCodeCompiler::ByteCodeCompiler(const Environment& env) : context(env)
    {
        ClassInfoReferenceCollector collector = ClassInfoReferenceCollector(context);
        dependencyGraph = collector.collect(context.getMainClass());
    }

    std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Type> ByteCodeCompiler::javaPrimitiveToCppType(const Java::Archive::FieldDescriptor& fieldDescriptor, SuperJet::Compiler::CodeGen::Cpp::Type::Flags flags)
    {
        std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Type> type = nullptr;

        switch (fieldDescriptor.getFieldType())
        {
        case Java::Archive::FieldDescriptor::FieldType::BYTE:
            type = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("char", flags);
            break;
        case Java::Archive::FieldDescriptor::FieldType::CHAR:
            type = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("wchar_t", flags);
            break;
        case Java::Archive::FieldDescriptor::FieldType::DOUBLE:
            type = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("double", flags);
            break;
        case Java::Archive::FieldDescriptor::FieldType::FLOAT:
            type = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("float", flags);
            break;
        case Java::Archive::FieldDescriptor::FieldType::INTEGER:
            type = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("int", flags);
            break;
        case Java::Archive::FieldDescriptor::FieldType::LONG:
            type = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("long long", flags);
            break;
        case Java::Archive::FieldDescriptor::FieldType::SHORT:
            type = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("short", flags);
            break;
        case Java::Archive::FieldDescriptor::FieldType::BOOLEAN:
            type = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("bool", flags);
            break;
        case Java::Archive::FieldDescriptor::FieldType::CLASS:
        case Java::Archive::FieldDescriptor::FieldType::ARRAY:
        default:
            throw Compiler::RuntimeException(fmt::format("fieldDescriptor contains not primitive type '{}'!", fieldDescriptor.getRawLiteral()));

        }

        return type;
    }

    std::shared_ptr<SuperJet::Compiler::CodeGen::Node> ByteCodeCompiler::getDefaultValue(const Java::Archive::FieldDescriptor& fieldDescriptor)
    {
        switch(fieldDescriptor.getFieldType())
        {
            case Java::Archive::FieldDescriptor::FieldType::LONG:
            case Java::Archive::FieldDescriptor::FieldType::INTEGER:
            case Java::Archive::FieldDescriptor::FieldType::SHORT:
            case Java::Archive::FieldDescriptor::FieldType::BYTE:
            case Java::Archive::FieldDescriptor::FieldType::DOUBLE:
            case Java::Archive::FieldDescriptor::FieldType::FLOAT:
                return std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Number>(0);
            case Java::Archive::FieldDescriptor::FieldType::BOOLEAN:
                return std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Boolean>(false);
            case Java::Archive::FieldDescriptor::FieldType::CHAR:
                return std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Character>();
            case Java::Archive::FieldDescriptor::FieldType::ARRAY:
            case Java::Archive::FieldDescriptor::FieldType::CLASS:
                return std::make_shared<SuperJet::Compiler::CodeGen::Cpp::NullPtr>();
        }
    }
}
