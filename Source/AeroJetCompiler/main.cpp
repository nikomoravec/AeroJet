#include "AeroJet.hpp"

#include <llvm/IR/IRBuilder.h>

int main()
{
    auto context = std::make_unique<llvm::LLVMContext>();
    llvm::IRBuilder<> builder(*context);

    auto module = std::make_unique<llvm::Module>("hello", *context);

    auto i32 = builder.getInt32Ty();
    auto prototype = llvm::FunctionType::get(i32, false);

    auto fooStruct = llvm::StructType::create(*context, "class.Foo");
    fooStruct->setBody({builder.getInt32Ty(), builder.getDoubleTy()});


    llvm::Function* main_fn = llvm::Function::Create(prototype, llvm::Function::ExternalLinkage, "main", module.get());

    llvm::BasicBlock* body = llvm::BasicBlock::Create(*context, "body", main_fn);
    builder.SetInsertPoint(body);

    auto i8p = builder.getInt8PtrTy();
    auto printf_prototype = llvm::FunctionType::get(i8p, true);
    auto printf_fn = llvm::Function::Create(printf_prototype, llvm::Function::ExternalLinkage, "printf", module.get());


    auto format_str = builder.CreateGlobalStringPtr("hello, world!\n");
    auto foo_ptr = builder.CreateAlloca(fooStruct);
    builder.CreateCall(printf_fn, { format_str, foo_ptr });

    auto ret = llvm::ConstantInt::get(i32, 0);
    builder.CreateRet(ret);

    module->print(llvm::outs(), nullptr);


    return 0;
}