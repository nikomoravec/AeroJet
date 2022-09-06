#include "Compiler/Compiler.h"
#include "Compiler/ClassInfoRerenceCollector.h"
#include "Compiler/Exceptions/RuntimeException.h"
#include "Compiler/CodeGen/Cpp/Class.h"
#include "Compiler/CodeGen/Cpp/Document.h"
#include "Compiler/CodeGen/Cpp/ForwardDeclaration.h"
#include "Compiler/CodeGen/Cpp/Function.h"
#include "Compiler/CodeGen/Cpp/Variable.h"
#include "Java/Archive/FieldDescriptor.h"
#include "spdlog/spdlog.h"
#include "fmt/format.h"
#include "Java/Archive/Archive.h"
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
                std::make_shared<SuperJet::Compiler::CodeGen::Cpp::NumericValue>(0)
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
                        std::make_shared<SuperJet::Compiler::CodeGen::Cpp::NullValue>()
                    )
                );
            }

            std::vector<std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Variable>> staticVariables;
            std::vector<std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Variable>> nonStaticVariables;

            const std::vector<Java::Archive::FieldInfo>& fields = node->getFields();
            for (const Java::Archive::FieldInfo& field : fields)
            {
                Java::JVM::u2 fieldNameIndex = field.getNameIndex();
                const std::string& fieldName = constantPool.get<Java::Archive::ConstantPoolInfoUtf8>(fieldNameIndex)->asString();

                Java::JVM::u2 fieldDescriptorIndex = field.getDescriptorIndex();
                const Java::Archive::FieldDescriptor& fieldDescriptor = Java::Archive::FieldDescriptor(constantPool.get<SuperJet::Java::Archive::ConstantPoolInfoUtf8>(fieldDescriptorIndex)->asString());
                
                std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Type> fieldType = nullptr;
                SuperJet::Compiler::CodeGen::Cpp::Type::Flags fieldTypeFlags = fieldDescriptor.isPrimitive() ? SuperJet::Compiler::CodeGen::Cpp::Type::Flags::NONE : SuperJet::Compiler::CodeGen::Cpp::Type::Flags::POINTER;

                if (field.isStatic())
                {
                    if (fieldName == "MIN_RADIX")
                    {
                        std::cout << constantPool.get<Java::Archive::ConstantPoolInfoUtf8>(112)->asString() << std::endl;
                    }
                    fieldTypeFlags = fieldTypeFlags | SuperJet::Compiler::CodeGen::Cpp::Type::Flags::STATIC;
                }

                if (fieldDescriptor.isPrimitive())
                {
                    switch (fieldDescriptor.getFieldType())
                    {
                    case Java::Archive::FieldDescriptor::FieldType::BYTE:
                        fieldType = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("char", fieldTypeFlags);
                        break;
                    case Java::Archive::FieldDescriptor::FieldType::CHAR:
                        fieldType = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("wchar_t", fieldTypeFlags);
                        break;
                    case Java::Archive::FieldDescriptor::FieldType::DOUBLE:
                        fieldType = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("double", fieldTypeFlags);
                        break;
                    case Java::Archive::FieldDescriptor::FieldType::FLOAT:
                        fieldType = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("float", fieldTypeFlags);
                        break;
                    case Java::Archive::FieldDescriptor::FieldType::INTEGER:
                        fieldType = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("int", fieldTypeFlags);
                        break;
                    case Java::Archive::FieldDescriptor::FieldType::LONG:
                        fieldType = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("long long", fieldTypeFlags);
                        break;
                    case Java::Archive::FieldDescriptor::FieldType::SHORT:
                        fieldType = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("short", fieldTypeFlags);
                        break;
                    case Java::Archive::FieldDescriptor::FieldType::BOOLEAN:
                        fieldType = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("bool", fieldTypeFlags);
                        break;
                    default:
                        break;
                    }
                    
                    std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Variable> memberVariable = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Variable>(fieldType, fieldName);
                    if (fieldType->isStatic())
                    {
                        staticVariables.emplace_back(memberVariable);
                    }
                    else
                    {
                        nonStaticVariables.emplace_back(memberVariable);
                    }

                    clazz->addVariable(memberVariable);
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
                    }
                    if (fieldDescriptor.isArray())
                    {
                        std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Type> arrayRootType = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("jvm::array", fieldTypeFlags);;
                        std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Type> currentType = arrayRootType;
                        Java::Archive::FieldDescriptor fd = fieldDescriptor;
                        while (fd.getUnderlyingType().has_value())
                        {
                            Java::Archive::FieldDescriptor underlyingTypeDescriptor = fd.getUnderlyingType().value();

                            SuperJet::Compiler::CodeGen::Cpp::Type::Flags underlyingTypeFlags = underlyingTypeDescriptor.isPrimitive() ? SuperJet::Compiler::CodeGen::Cpp::Type::Flags::NONE : SuperJet::Compiler::CodeGen::Cpp::Type::Flags::POINTER;
                            std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Type> underlyingType = nullptr;
                            switch (underlyingTypeDescriptor.getFieldType())
                            {
                            case Java::Archive::FieldDescriptor::FieldType::BYTE:
                                underlyingType = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("char", underlyingTypeFlags);
                                break;
                            case Java::Archive::FieldDescriptor::FieldType::CHAR:
                                underlyingType = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("wchar_t", underlyingTypeFlags);
                                break;
                            case Java::Archive::FieldDescriptor::FieldType::DOUBLE:
                                underlyingType = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("double", underlyingTypeFlags);
                                break;
                            case Java::Archive::FieldDescriptor::FieldType::FLOAT:
                                underlyingType = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("float", underlyingTypeFlags);
                                break;
                            case Java::Archive::FieldDescriptor::FieldType::INTEGER:
                                underlyingType = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("int", underlyingTypeFlags);
                                break;
                            case Java::Archive::FieldDescriptor::FieldType::LONG:
                                underlyingType = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("long long", underlyingTypeFlags);
                                break;
                            case Java::Archive::FieldDescriptor::FieldType::SHORT:
                                underlyingType = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("short", underlyingTypeFlags);
                                break;
                            case Java::Archive::FieldDescriptor::FieldType::BOOLEAN:
                                underlyingType = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("bool", underlyingTypeFlags);
                                break;
                            case Java::Archive::FieldDescriptor::FieldType::CLASS:
                            {
                                std::string className = underlyingTypeDescriptor.getClassName().value();
                                std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Namespace> ns = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Namespace>(className);
                                std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Class> c = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Class>(SuperJet::Utils::StringUtils::split(className, '/').back());
                                ns->addClass(c);
                                underlyingType = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>(c->getFullName(), underlyingTypeFlags);
                                break;
                            }
                            case Java::Archive::FieldDescriptor::FieldType::ARRAY:
                                underlyingType = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("jvm::array", underlyingTypeFlags);
                                break;
                            default:
                                break;
                            }

                            currentType->addTemplateArgument(underlyingType);
                            currentType = underlyingType;

                            fd = fd.getUnderlyingType().value();
                        }

                        std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Variable> memberVariable = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Variable>(arrayRootType, fieldName);
                        clazz->addVariable(memberVariable);
                        if (arrayRootType->isStatic())
                        {
                            staticVariables.emplace_back(memberVariable);
                        }
                        else
                        {
                            nonStaticVariables.emplace_back(memberVariable);
                        }
                    }
                }
            }

            std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Functuon> staticInitializerFunction = 
            std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Functuon>(
                "STATIC_INIT", 
                std::vector<std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::FunctionArgument>>(), 
                std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("void", SuperJet::Compiler::CodeGen::Cpp::Type::Flags::STATIC)
            );
            
            for (const std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Variable>& staticVariable : staticVariables)
            {
                //staticInitializerFunction->addNode()
            }

            clazz->addNode(staticInitializerFunction);

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
}