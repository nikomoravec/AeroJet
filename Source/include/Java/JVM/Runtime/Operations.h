#ifndef SUPERJET_OPERATIONS_H
#define SUPERJET_OPERATIONS_H

#include "fmt/format.h"
#include "Java/Types.h"
#include "Compiler/Exceptions/IncorrectDataSizeException.h"
#include <vector>

namespace SuperJet::Java::JVM::Runtime
{
    enum class OperationCode : JVM::u1
    {
        aaload = 0x32,
        aastore = 0x53,
        aconst_null = 0x1,
        aload = 0x19,
        aload_0 = 0x2a,
        aload_1 = 0x2b,
        aload_2 = 0x2c,
        aload_3 = 0x2d,
        anewarray = 0xbd,
        areturn = 0xb0,
        arraylength = 0xbe,
        astore = 0x3a,
        astore_0 = 0x4b,
        astore_1 = 0x4c,
        astore_2 = 0x4d,
        astore_3 = 0x4e,
        athrow = 0xbf,
        baload = 0x33,
        bastore = 0x54,
        bipush = 0x10,
        caload = 0x34,
        castore = 0x55,
        checkcast = 0xc0,
        d2f = 0x90,
        d2i = 0x8e,
        d2l = 0x8f,
        dadd = 0x63,
        daload = 0x31,
        dastore = 0x52,
        dcmpg = 0x98,
        dcmpl = 0x97,
        dconst_0 = 0xe,
        dconst_1 = 0xf,
        ddiv = 0x6f,
        dload = 0x18,
        dload_0 = 0x26,
        dload_1 = 0x27,
        dload_2 = 0x28,
        dload_3 = 0x29,
        dmul = 0x6b,
        dneg = 0x77,
        drem = 0x73,
        dreturn = 0xaf,
        dstore = 0x39,
        dstore_0 = 0x47,
        dstore_1 = 0x48,
        dstore_2 = 0x49,
        dstore_3 = 0x4a,
        dsub = 0x67,
        dup = 0x59,
        dup_x1 = 0x5a,
        dup_x2 = 0x5b,
        dup2 = 0x5c,
        dup2_x1 = 0x5d,
        dup2_x2 = 0x5e,
        f2i = 0x8b,
        f2d = 0x8d,
        f2l = 0x8c,
        fadd = 0x62,
        faload = 0x30,
        fastore = 0x51,
        fcmpg = 0x96,
        fcmpl = 0x95,
        fconst_0 = 0xb,
        fconst_1 = 0xc,
        fconst_2 = 0xd,
        fdiv = 0x6e,
        fload = 0x17,
        fload_0 = 0x22,
        fload_1 = 0x23,
        fload_2 = 0x24,
        fload_3 = 0x25,
        fmul = 0x6a,
        fneg = 0x76,
        frem = 0x72,
        freturn = 0xae,
        fstore = 0x38,
        fstore_0 = 0x43,
        fstore_1 = 0x44,
        fstore_2 = 0x45,
        fstore_3 = 0x46,
        fsub = 0x66,
        getfield = 0xb4,
        getstatic = 0xb2,
        GOTO = 0xa7,
        goto_w = 0xc8,
        i2b = 0x91,
        i2c = 0x92,
        i2d = 0x87,
        i2f = 0x86,
        i2l = 0x85,
        i2s = 0x93,
        iadd = 0x60,
        iaload = 0x2e,
        iand = 0x7e,
        iastore = 0x4f,
        iconst_m1 = 0x2,
        iconst_0 = 0x3,
        iconst_1 = 0x4,
        iconst_2 = 0x5,
        iconst_3 = 0x6,
        iconst_4 = 0x7,
        iconst_5 = 0x8,
        idiv = 0x6c,
        if_acmpeq = 0xa5,
        if_acmpne = 0xa6,
        if_icmpeq = 0x9f,
        if_icmpne = 0xa0,
        if_icmplt = 0xa1,
        if_icmpge = 0xa2,
        if_icmpgt = 0xa3,
        if_icmple = 0xa4,
        ifeq = 0x99,
        ifne = 0x9a,
        iflt = 0x9b,
        ifge = 0x9c,
        ifgt = 0x9d,
        ifle = 0x9e,
        ifnonnull = 0xc7,
        ifnull = 0xc6,
        iinc = 0x84,
        iload = 0x15,
        iload_0 = 0x1a,
        iload_1 = 0x1b,
        iload_2 = 0x1c,
        iload_3 = 0x1d,
        imul = 0x68,
        ineg = 0x74,
        instanceof = 0xc1,
        invokedynamic = 0xba,
        invokeinterface = 0xb9,
        invokespecial = 0xb7,
        invokestatic = 0xb8,
        invokevirtual = 0xb6,
        ior = 0x80,
        irem = 0x70,
        ireturn = 0xac,
        ishl = 0x78,
        ishr = 0x7a,
        istore = 0x36,
        istore_0 = 0x3b,
        istore_1 = 0x3c,
        istore_2 = 0x3d,
        istore_3 = 0x3e,
        isub = 0x64,
        iushr = 0x7c,
        ixor = 0x82,
        jsr = 0xa8,
        jsr_w = 0xc9,
        l2d = 0x8a,
        l2f = 0x89,
        l2i = 0x88,
        ladd = 0x61,
        laload = 0x2f,
        land = 0x7f,
        lastore = 0x50,
        lcmp = 0x94,
        lconst_0 = 0x9,
        lconst_1 =  0xa,
        ldc = 0x12,
        ldc_w = 0x13,
        ldc2_w = 0x14,
        ldiv = 0x6d,
        lload = 0x16,
        lload_0 = 0x1e,
        lload_1 = 0x1f,
        lload_2 = 0x20,
        lload_3 = 0x21,
        lmul = 0x69,
        lneg = 0x75,
        lookupswitch = 0xab,
        lor = 0x81,
        lrem = 0x71,
        lreturn = 0xad,
        lshl = 0x79,
        lshr = 0x7b,
        lstore = 0x37,
        lstore_0 = 0x3f,
        lstore_1 = 0x40,
        lstore_2 = 0x41,
        lstore_3 = 0x42,
        lsub = 0x65,
        lushr = 0x7d,
        lxor = 0x83,
        monitorenter = 0xc2,
        monitorexit = 0xc3,
        multianewarray = 0xc5,
        NEW = 0xbb,
        newarray = 0xbc,
        nop = 0x0,
        pop = 0x57,
        pop2 = 0x58,
        putfield = 0xb5,
        putstatic = 0xb3,
        ret = 0xa9,
        RETURN = 0xb1,
        saload = 0x35,
        sastore = 0x56,
        sipush = 0x11,
        swap = 0x5f,
        tableswitch = 0xaa,
        wide = 0xc4
    };

    using OperationData = std::vector<JVM::u1>;

    class Operation
    {
    public:
        Operation(){}
        OperationCode getOperationCode() const
        {
            return operationCode;
        }

        const std::vector<JVM::u1>& getData() const
        {
            return data;
        }

    protected:
        Operation(const OperationCode inOperationCode) : operationCode(inOperationCode), data({})
        {
        }

        Operation(const OperationCode inOperationCode, const OperationData& inOperationData) : operationCode(inOperationCode), data(inOperationData)
        {
        }

    protected:
        OperationCode operationCode;
        std::vector<JVM::u1> data;
    };

    class NoDataOperation : public Operation
    {
        static constexpr int32_t DATA_SIZE = 0;

    protected:
        NoDataOperation(const OperationCode inOperationCode, const OperationData& inOperationData) : Operation(inOperationCode, inOperationData)
        {
            if (data.size() != DATA_SIZE)
            {
                throw Compiler::IncorrectDataSizeException(DATA_SIZE, data.size());
            }
        }

        NoDataOperation(const OperationCode inOperationCode) : NoDataOperation(inOperationCode, {})
        {
        }
    };

    class SingleByteDataOperation : public Operation
    {
        static constexpr int32_t DATA_SIZE = 1;

    protected:
        SingleByteDataOperation(const OperationCode inOperationCode, const OperationData& inOperationData) : Operation(inOperationCode, inOperationData)
        {
            if (data.size() != DATA_SIZE)
            {
                throw Compiler::IncorrectDataSizeException(DATA_SIZE, data.size());
            }

            byte = data[0];
        }

        JVM::u1 getByte() const
        {
            return byte;
        }

    protected:
        JVM::u1 byte;
    };

    class BiByteDataOperation : public Operation
    {
        static constexpr int32_t DATA_SIZE = 2;

    public:
        JVM::u1 getIndexByte1() const
        {
            return indexByte1;
        }

        JVM::u1 getIndexByte2() const
        {
            return indexByte2;
        }

    protected:
        BiByteDataOperation(const OperationCode inOperationCode, const OperationData& inOperationData) : Operation(inOperationCode, inOperationData)
        {
            if (data.size() != DATA_SIZE)
            {
                throw Compiler::IncorrectDataSizeException(DATA_SIZE, data.size());
            }

            indexByte1 = data[0];
            indexByte2 = data[1];
        }

    protected:
        JVM::u1 indexByte1;
        JVM::u1 indexByte2;
    };

    class BiByteIndexDataOperation : public BiByteDataOperation
    {
    public:
        virtual JVM::u1 getIndex()
        {
            return ((indexByte1 << 8) | indexByte2);
        }

    protected:
        BiByteIndexDataOperation(const OperationCode inOperationCode, const OperationData& inOperationData) : BiByteDataOperation(inOperationCode, inOperationData)
        {
        }
    };

    class FourByteDataOperation : public Operation
    {
        static constexpr int32_t DATA_SIZE = 4;

    public:
        JVM::u1 getIndexByte1() const
        {
            return indexByte1;
        }

        JVM::u1 getIndexByte2() const
        {
            return indexByte2;
        }

        JVM::u1 getIndexByte3() const
        {
            return indexByte3;
        }

        JVM::u1 getIndexByte4() const
        {
            return indexByte4;
        }

    protected:
        FourByteDataOperation(const OperationCode inOperationCode, const OperationData& inOperationData) : Operation(inOperationCode, inOperationData)
        {
            if (data.size() != DATA_SIZE)
            {
                throw Compiler::IncorrectDataSizeException(DATA_SIZE, data.size());
            }

            indexByte1 = data[0];
            indexByte2 = data[1];
            indexByte3 = data[2];
            indexByte4 = data[3];
        }

    protected:
        JVM::u1 indexByte1;
        JVM::u1 indexByte2;
        JVM::u1 indexByte3;
        JVM::u1 indexByte4;
    };

    class FourByteIndexDataOperation : public FourByteDataOperation
    {
    public:
        virtual JVM::u1 getIndex()
        {
            return ((indexByte1 << 24) | (indexByte2 << 16) | (indexByte3 << 8) | indexByte4);
        }

    protected:
        FourByteIndexDataOperation(const OperationCode inOperationCode, const OperationData& inOperationData) : FourByteDataOperation(inOperationCode, inOperationData)
        {
        }
    };

    class aaload : public NoDataOperation
    {
    public:
        aaload() : NoDataOperation(OperationCode::aaload)
        {
        }
    };

    class aastore : public NoDataOperation
    {
    public:
        aastore() : NoDataOperation(OperationCode::aastore)
        {
        }
    };

    class aconst_null : public NoDataOperation
    {
    public:
        aconst_null() : NoDataOperation(OperationCode::aconst_null)
        {
        }
    };

    class aload : public SingleByteDataOperation
    {
    public:
        aload(const OperationData& inOperationData) : SingleByteDataOperation(OperationCode::aload, inOperationData)
        {
        }

    protected:
        aload(const OperationCode inOperationCode, const OperationData& inOperationData) : SingleByteDataOperation(inOperationCode, inOperationData)
        {
        }

    };

    class aload_0 : public aload
    {
    public:
        aload_0() : aload(OperationCode::aload_0, {0})
        {
        }
    };

    class aload_1 : public aload
    {
    public:
        aload_1() : aload(OperationCode::aload_1, {1})
        {

        }
    };

    class aload_2 : public aload
    {
    public:
        aload_2() : aload(OperationCode::aload_2, {2})
        {

        }
    };

    class aload_3 : public aload
    {
    public:
        aload_3() : aload(OperationCode::aload_3, {3})
        {

        }
    };

    class anewarray : public BiByteIndexDataOperation
    {
    public:
        anewarray(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::anewarray, inOperationData)
        {
        }
    };

    class areturn : public NoDataOperation
    {
    public:
        areturn() : NoDataOperation(OperationCode::areturn)
        {
        }
    };

    class arraylength : public NoDataOperation
    {
    public:
        arraylength() : NoDataOperation(OperationCode::arraylength)
        {
        }
    };

    class astore : public SingleByteDataOperation
    {
    public:
        astore(const OperationData& inOperationData) : SingleByteDataOperation(OperationCode::astore, inOperationData)
        {
        }
        

    protected:
        astore(const OperationCode inOperation, const OperationData& inOperationData) : SingleByteDataOperation(inOperation, inOperationData)
        {
        }
    };

    class astore_0 : public astore
    {
    public:
        astore_0() : astore(OperationCode::astore_0, {0})
        {
        }
    };

    class astore_1 : public astore
    {
    public:
        astore_1() : astore(OperationCode::astore_1, {1})
        {
        }
    };

    class astore_2 : public astore
    {
    public:
        astore_2() : astore(OperationCode::astore_2, {2})
        {
        }
    };

    class astore_3 : public astore
    {
    public:
        astore_3() : astore(OperationCode::astore_3, {3})
        {
        }
    };

    class athrow : public NoDataOperation
    {
    public:
        athrow() : NoDataOperation(OperationCode::athrow)
        {
        }
    };

    class baload : public NoDataOperation
    {
    public:
        baload() : NoDataOperation(OperationCode::baload)
        {
        }
    };

    class bastore : public NoDataOperation
    {
    public:
        bastore() : NoDataOperation(OperationCode::bastore)
        {
        }
    };

    class bipush : public SingleByteDataOperation
    {
    public:
        bipush(const OperationData& inOperationData) : SingleByteDataOperation(OperationCode::bipush, inOperationData)
        {

        }
    };

    class caload : public NoDataOperation
    {
    public:
        caload() : NoDataOperation(OperationCode::caload)
        {
        }
    };

    class castore : public NoDataOperation
    {
    public:
        castore() : NoDataOperation(OperationCode::castore)
        {
        }
    };

    class checkcast : public BiByteIndexDataOperation
    {
    public:
        checkcast(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::checkcast, inOperationData)
        {
        }
    };

    class d2f : public NoDataOperation
    {
    public:
        d2f() : NoDataOperation(OperationCode::d2f)
        {
        }
    };

    class d2i : public NoDataOperation
    {
    public:
        d2i() : NoDataOperation(OperationCode::d2i)
        {
        }
    };

    class d2l : public NoDataOperation
    {
    public:
        d2l() : NoDataOperation(OperationCode::d2l)
        {
        }
    };

    class dadd : public NoDataOperation
    {
    public:
        dadd() : NoDataOperation(OperationCode::dadd)
        {
        }
    };

    class daload : public NoDataOperation
    {
    public:
        daload() : NoDataOperation(OperationCode::daload)
        {
        }
    };

    class dastore : public NoDataOperation
    {
    public:
        dastore() : NoDataOperation(OperationCode::dastore)
        {
        }
    };

    class dcmpg : public NoDataOperation
    {
    public:
        dcmpg() : NoDataOperation(OperationCode::dcmpg)
        {
        }
    };

    class dcmpl : public NoDataOperation
    {
    public:
        dcmpl() : NoDataOperation(OperationCode::dcmpl)
        {
        }
    };

    class dconst_0 : public NoDataOperation
    {
    public:
        dconst_0() : NoDataOperation(OperationCode::dconst_0)
        {
        }
    };

    class dconst_1 : public NoDataOperation
    {
    public:
        dconst_1() : NoDataOperation(OperationCode::dconst_1)
        {
        }
    };

    class ddiv : public NoDataOperation
    {
    public:
        ddiv() : NoDataOperation(OperationCode::ddiv)
        {
        }
    };

    class dload : public SingleByteDataOperation
    {
    public:
        dload(const OperationData& inOperationData) : SingleByteDataOperation(OperationCode::dload, inOperationData)
        {
        }

    protected:
        dload(const OperationCode inOperationCode, const OperationData& inOperationData) : SingleByteDataOperation(inOperationCode, inOperationData)
        {
        }
    };

    class dload_0 : public dload
    {
    public:
        dload_0() : dload(OperationCode::dload_0, {0})
        {
        }
    };

    class dload_1 : public dload
    {
    public:
        dload_1() : dload(OperationCode::dload_1, {1})
        {
        }
    };

    class dload_2 : public dload
    {
    public:
        dload_2() : dload(OperationCode::dload_2, {1})
        {
        }
    };

    class dload_3 : public dload
    {
    public:
        dload_3() : dload(OperationCode::dload_3, {1})
        {
        }
    };

    class dmul : public NoDataOperation
    {
    public:
        dmul() : NoDataOperation(OperationCode::dmul)
        {
        }
    };

    class dneg : public NoDataOperation
    {
    public:
        dneg() : NoDataOperation(OperationCode::dneg)
        {
        }
    };

    class drem : public NoDataOperation
    {
    public:
        drem() : NoDataOperation(OperationCode::drem)
        {
        }
    };

    class dreturn : public NoDataOperation
    {
    public:
        dreturn() : NoDataOperation(OperationCode::dreturn)
        {
        }
    };

    class dstore : public SingleByteDataOperation
    {
    public:
        dstore(const OperationData& inOperationData) : SingleByteDataOperation(OperationCode::dstore, inOperationData)
        {
        }

    protected:
        dstore(const OperationCode inOperationCode, const OperationData& inOperationData) : SingleByteDataOperation(inOperationCode, inOperationData)
        {
        }
    };

    class dstore_0 : public dstore
    {
    public:
        dstore_0() : dstore(OperationCode::dstore_0, {0})
        {
        }
    };

    class dstore_1 : public dstore
    {
    public:
        dstore_1() : dstore(OperationCode::dstore_1, {1})
        {
        }
    };

    class dstore_2 : public dstore
    {
    public:
        dstore_2() : dstore(OperationCode::dstore_2, {2})
        {
        }
    };

    class dstore_3 : public dstore
    {
    public:
        dstore_3() : dstore(OperationCode::dstore_3, {3})
        {
        }
    };

    class dsub : public NoDataOperation
    {
    public:
        dsub() : NoDataOperation(OperationCode::dsub)
        {
        }
    };

    class dup : public NoDataOperation
    {
    public:
        dup() : NoDataOperation(OperationCode::dup)
        {
        }
    };

    class dup_x1 : public NoDataOperation
    {
    public:
        dup_x1() : NoDataOperation(OperationCode::dup_x1)
        {
        }
    };

    class dup_x2 : public NoDataOperation
    {
    public:
        dup_x2() : NoDataOperation(OperationCode::dup_x2)
        {
        }
    };

    class dup2 : public NoDataOperation
    {
    public:
        dup2() : NoDataOperation(OperationCode::dup2)
        {
        }
    };

    class dup2_x1 : public NoDataOperation
    {
    public:
        dup2_x1() : NoDataOperation(OperationCode::dup2_x1)
        {
        }
    };

    class dup2_x2 : public NoDataOperation
    {
    public:
        dup2_x2() : NoDataOperation(OperationCode::dup2_x2)
        {
        }
    };

    class f2d : public NoDataOperation
    {
    public:
        f2d() : NoDataOperation(OperationCode::f2d)
        {
        }
    };

    class f2i : public NoDataOperation
    {
    public:
        f2i() : NoDataOperation(OperationCode::f2i)
        {
        }
    };

    class f2l : public NoDataOperation
    {
    public:
        f2l() : NoDataOperation(OperationCode::f2l)
        {
        }
    };

    class fadd : public NoDataOperation
    {
    public:
        fadd() : NoDataOperation(OperationCode::fadd)
        {
        }
    };

    class faload : public NoDataOperation
    {
    public:
        faload() : NoDataOperation(OperationCode::faload)
        {
        }
    };

    class fastore : public NoDataOperation
    {
    public:
        fastore() : NoDataOperation(OperationCode::fastore)
        {
        }
    };

    class fcmpg : public NoDataOperation
    {
    public:
        fcmpg() : NoDataOperation(OperationCode::fcmpg)
        {
        }
    };

    class fcmpl : public NoDataOperation
    {
    public:
        fcmpl() : NoDataOperation(OperationCode::fcmpl)
        {
        }
    };

    class fconst_0 : public NoDataOperation
    {
    public:
        fconst_0() : NoDataOperation(OperationCode::fconst_0)
        {
        }
    };

    class fconst_1 : public NoDataOperation
    {
    public:
        fconst_1() : NoDataOperation(OperationCode::fconst_1)
        {
        }
    };

    class fconst_2 : public NoDataOperation
    {
    public:
        fconst_2() : NoDataOperation(OperationCode::fconst_2)
        {
        }
    };

    class fdiv : public NoDataOperation
    {
    public:
        fdiv() : NoDataOperation(OperationCode::fdiv)
        {
        }
    };

    class fload : public SingleByteDataOperation
    {
    public:
        fload(const OperationData& inOperationData) : SingleByteDataOperation(OperationCode::fload, inOperationData)
        {
        }

    protected:
        fload(const OperationCode inOperationCode, const OperationData& inOperationData) : SingleByteDataOperation(inOperationCode, inOperationData)
        {
        }
    };

    class fload_0 : public fload
    {
    public:
        fload_0() : fload(OperationCode::fload_0, {0})
        {
        }
    };

    class fload_1 : public fload
    {
    public:
        fload_1() : fload(OperationCode::fload_1, {1})
        {
        }
    };

    class fload_2 : public fload
    {
    public:
        fload_2() : fload(OperationCode::fload_2, {2})
        {
        }
    };

    class fload_3 : public fload
    {
    public:
        fload_3() : fload(OperationCode::fload_3, {3})
        {
        }
    };

    class fmul : public NoDataOperation
    {
    public:
        fmul() : NoDataOperation(OperationCode::fmul)
        {
        }
    };

    class fneg : public NoDataOperation
    {
    public:
        fneg() : NoDataOperation(OperationCode::fneg)
        {
        }
    };

    class frem : public NoDataOperation
    {
    public:
        frem() : NoDataOperation(OperationCode::frem)
        {
        }
    };

    class freturn : public NoDataOperation
    {
    public:
        freturn() : NoDataOperation(OperationCode::freturn)
        {
        }
    };

    class fstore : public SingleByteDataOperation
    {
    public:
        fstore(const OperationData& inOperationData) : SingleByteDataOperation(OperationCode::fstore, inOperationData)
        {
        }

    protected:
        fstore(const OperationCode inOperationCode, const OperationData& inOperationData) : SingleByteDataOperation(inOperationCode, inOperationData)
        {
        }
    };

    class fstore_0 : public fstore
    {
    public:
        fstore_0() : fstore(OperationCode::fstore_0, {0})
        {
        }
    };

    class fstore_1 : public fstore
    {
    public:
        fstore_1() : fstore(OperationCode::fstore_1, {1})
        {
        }
    };

    class fstore_2 : public fstore
    {
    public:
        fstore_2() : fstore(OperationCode::fstore_2, {2})
        {
        }
    };

    class fstore_3 : public fstore
    {
    public:
        fstore_3() : fstore(OperationCode::fstore_3, {3})
        {
        }
    };

    class fsub : public NoDataOperation
    {
    public:
        fsub() : NoDataOperation(OperationCode::fsub)
        {
        }
    };

    class getfield : public BiByteIndexDataOperation
    {
    public:
        getfield(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::getfield, inOperationData)
        {
        }
    };

    class getstatic : public BiByteIndexDataOperation
    {
    public:
        getstatic(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::getstatic, inOperationData)
        {
        }
    };

    class GOTO : public BiByteIndexDataOperation
    {
    public:
        GOTO(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::GOTO, inOperationData)
        {
        }
    };

    class goto_w : public FourByteIndexDataOperation
    {
    public:
        goto_w(const OperationData& inOperationData) : FourByteIndexDataOperation(OperationCode::goto_w, inOperationData)
        {
        }
    };

    class i2b : public NoDataOperation
    {
    public:
        i2b() : NoDataOperation(OperationCode::i2b)
        {
        }
    };

    class i2c : public NoDataOperation
    {
    public:
        i2c() : NoDataOperation(OperationCode::i2c)
        {
        }
    };

    class i2d : public NoDataOperation
    {
    public:
        i2d() : NoDataOperation(OperationCode::i2d)
        {
        }
    };

    class i2f : public NoDataOperation
    {
    public:
        i2f() : NoDataOperation(OperationCode::i2f)
        {
        }
    };

    class i2l : public NoDataOperation
    {
    public:
        i2l() : NoDataOperation(OperationCode::i2l)
        {
        }
    };

    class i2s : public NoDataOperation
    {
    public:
        i2s() : NoDataOperation(OperationCode::i2s)
        {
        }
    };

    class iadd : public NoDataOperation
    {
    public:
        iadd() : NoDataOperation(OperationCode::iadd)
        {
        }
    };

    class iaload : public NoDataOperation
    {
    public:
        iaload() : NoDataOperation(OperationCode::iaload)
        {
        }
    };

    class iand : public NoDataOperation
    {
    public:
        iand() : NoDataOperation(OperationCode::iand)
        {
        }
    };

    class iastore : public NoDataOperation
    {
    public:
        iastore() : NoDataOperation(OperationCode::iastore)
        {
        }
    };

    class iconst_m1 : public SingleByteDataOperation
    {
    public:
        iconst_m1() : SingleByteDataOperation(OperationCode::iconst_m1, {static_cast<JVM::u1>(-1)})
        {
        }
    };

    class iconst_0 : public SingleByteDataOperation
    {
    public:
        iconst_0() : SingleByteDataOperation(OperationCode::iconst_0, {0})
        {
        }
    };

    class iconst_1 : public SingleByteDataOperation
    {
    public:
        iconst_1() : SingleByteDataOperation(OperationCode::iconst_1, {1})
        {
        }
    };

    class iconst_2 : public SingleByteDataOperation
    {
    public:
        iconst_2() : SingleByteDataOperation(OperationCode::iconst_2, {2})
        {
        }
    };

    class iconst_3 : public SingleByteDataOperation
    {
    public:
        iconst_3() : SingleByteDataOperation(OperationCode::iconst_3, {3})
        {
        }
    };

    class iconst_4 : public SingleByteDataOperation
    {
    public:
        iconst_4() : SingleByteDataOperation(OperationCode::iconst_4, {4})
        {
        }
    };

    class iconst_5 : public SingleByteDataOperation
    {
    public:
        iconst_5() : SingleByteDataOperation(OperationCode::iconst_5, {5})
        {
        }
    };

    class idiv : public NoDataOperation
    {
    public:
        idiv() : NoDataOperation(OperationCode::idiv)
        {
        }
    };

    class if_acmpeq : public BiByteIndexDataOperation
    {
    public:
        if_acmpeq(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::if_acmpeq, inOperationData)
        {
        }
    };

    class if_acmpne : public BiByteIndexDataOperation
    {
    public:
        if_acmpne(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::if_acmpne, inOperationData)
        {
        }
    };

    class if_icmpeq : public BiByteIndexDataOperation
    {
    public:
        if_icmpeq(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::if_icmpeq, inOperationData)
        {
        }
    };

    class if_icmpne : public BiByteIndexDataOperation
    {
    public:
        if_icmpne(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::if_icmpne, inOperationData)
        {
        }
    };

    class if_icmplt : public BiByteIndexDataOperation
    {
    public:
        if_icmplt(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::if_icmplt, inOperationData)
        {
        }
    };

    class if_icmpge : public BiByteIndexDataOperation
    {
    public:
        if_icmpge(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::if_icmpge, inOperationData)
        {
        }
    };

    class if_icmpgt : public BiByteIndexDataOperation
    {
    public:
        if_icmpgt(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::if_icmpgt, inOperationData)
        {
        }
    };

    class if_icmple : public BiByteIndexDataOperation
    {
    public:
        if_icmple(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::if_icmple, inOperationData)
        {
        }
    };

    class ifeq : public BiByteIndexDataOperation
    {
    public:
        ifeq(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::ifeq, inOperationData)
        {
        }
    };

    class ifne : public BiByteIndexDataOperation
    {
    public:
        ifne(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::ifne, inOperationData)
        {
        }
    };

    class iflt : public BiByteIndexDataOperation
    {
    public:
        iflt(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::iflt, inOperationData)
        {
        }
    };

    class ifge : public BiByteIndexDataOperation
    {
    public:
        ifge(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::ifge, inOperationData)
        {
        }
    };

    class ifgt : public BiByteIndexDataOperation
    {
    public:
        ifgt(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::ifgt, inOperationData)
        {
        }
    };

    class ifle : public BiByteIndexDataOperation
    {
    public:
        ifle(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::ifle, inOperationData)
        {
        }
    };

    class ifnonnull : public BiByteIndexDataOperation
    {
    public:
        ifnonnull(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::ifnonnull, inOperationData)
        {
        }
    };

    class ifnull : public BiByteIndexDataOperation
    {
    public:
        ifnull(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::ifnull, inOperationData)
        {
        }
    };

    class iinc : public BiByteDataOperation
    {
    public:
        iinc(const OperationData& inOperationData) : BiByteDataOperation(OperationCode::iinc, inOperationData)
        {
        }
    };

    class iload : public SingleByteDataOperation
    {
    public:
        iload(const OperationData& inOperationData) : SingleByteDataOperation(OperationCode::iload, inOperationData)
        {
        }

    protected:
        iload(const OperationCode inOperationCode, const OperationData& inOperationData) : SingleByteDataOperation(inOperationCode, inOperationData)
        {
        }
    };

    class iload_0 : public iload
    {
    public:
        iload_0() : iload(OperationCode::iload_0, {0})
        {
        }
    };

    class iload_1 : public iload
    {
    public:
        iload_1() : iload(OperationCode::iload_1, {1})
        {
        }
    };

    class iload_2 : public iload
    {
    public:
        iload_2() : iload(OperationCode::iload_2, {2})
        {
        }
    };

    class iload_3 : public iload
    {
    public:
        iload_3() : iload(OperationCode::iload_3, {3})
        {
        }
    };

    class imul : public NoDataOperation
    {
    public:
        imul() : NoDataOperation(OperationCode::imul)
        {
        }
    };

    class ineg : public NoDataOperation
    {
    public:
        ineg() : NoDataOperation(OperationCode::ineg)
        {
        }
    };

    class instanceof : public BiByteIndexDataOperation
    {
    public:
        instanceof(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::instanceof, inOperationData)
        {
        }
    };

    class invokedynamic : public FourByteIndexDataOperation
    {
    public:
        invokedynamic(const OperationData& inOperationData) : FourByteIndexDataOperation(OperationCode::invokedynamic, inOperationData)
        {
        }

        virtual JVM::u1 getIndex() override
        {
            return ((indexByte1 << 8) | indexByte2);
        }
    };

    class invokeinterface : public FourByteIndexDataOperation
    {
    public:
        invokeinterface(const OperationData& inOperationData) : FourByteIndexDataOperation(OperationCode::invokeinterface, inOperationData)
        {
        }

        virtual JVM::u1 getIndex() override
        {
            return ((indexByte1 << 8) | indexByte2);
        }
    };

    class invokespecial : public BiByteIndexDataOperation
    {
    public:
        invokespecial(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::invokespecial, inOperationData)
        {
        }
    };

    class invokestatic : public BiByteIndexDataOperation
    {
    public:
        invokestatic(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::invokestatic, inOperationData)
        {
        }
    };

    class invokevirtual : public BiByteIndexDataOperation
    {
    public:
        invokevirtual(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::invokevirtual, inOperationData)
        {
        }
    };

    class ior : public NoDataOperation
    {
    public:
        ior() : NoDataOperation(OperationCode::ior)
        {
        }
    };

    class irem : public NoDataOperation
    {
    public:
        irem() : NoDataOperation(OperationCode::irem)
        {
        }
    };

    class ireturn : public NoDataOperation
    {
    public:
        ireturn() : NoDataOperation(OperationCode::ireturn)
        {
        }
    };

    class ishl : public NoDataOperation
    {
    public:
        ishl() : NoDataOperation(OperationCode::ishl)
        {
        }
    };

    class ishr : public NoDataOperation
    {
    public:
        ishr() : NoDataOperation(OperationCode::ishr)
        {
        }
    };

    class istore : public SingleByteDataOperation
    {
    public:
        istore(const OperationData& inOperationData) : SingleByteDataOperation(OperationCode::istore, inOperationData)
        {
        }

    protected:
        istore(const OperationCode inOperationCode, const OperationData& inOperationData) : SingleByteDataOperation(inOperationCode, inOperationData)
        {
        }
    };

    class istore_0 : public istore
    {
    public:
        istore_0() : istore(OperationCode::istore_0, {0})
        {
        }
    };

    class istore_1 : public istore
    {
    public:
        istore_1() : istore(OperationCode::istore_1, {1})
        {
        }
    };

    class istore_2 : public istore
    {
    public:
        istore_2() : istore(OperationCode::istore_2, {2})
        {
        }
    };

    class istore_3 : public istore
    {
    public:
        istore_3() : istore(OperationCode::istore_3, {3})
        {
        }
    };

    class isub : public NoDataOperation
    {
    public:
        isub() : NoDataOperation(OperationCode::isub)
        {
        }
    };

    class iushr : public NoDataOperation
    {
    public:
        iushr() : NoDataOperation(OperationCode::iushr)
        {
        }
    };

    class ixor : public NoDataOperation
    {
    public:
        ixor() : NoDataOperation(OperationCode::ixor)
        {
        }
    };

    class jsr : public BiByteIndexDataOperation
    {
    public:
        jsr(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::jsr, inOperationData)
        {
        }
    };

    class jsr_w : public FourByteIndexDataOperation
    {
    public:
        jsr_w(const OperationData& inOperationData) : FourByteIndexDataOperation(OperationCode::jsr_w, inOperationData)
        {
        }
    };

    class l2d : public NoDataOperation
    {
    public:
        l2d() : NoDataOperation(OperationCode::l2d)
        {
        }
    };

    class l2f : public NoDataOperation
    {
    public:
        l2f() : NoDataOperation(OperationCode::l2f)
        {
        }
    };

    class l2i : public NoDataOperation
    {
    public:
        l2i() : NoDataOperation(OperationCode::l2i)
        {
        }
    };

    class ladd : public NoDataOperation
    {
    public:
        ladd() : NoDataOperation(OperationCode::ladd)
        {
        }
    };

    class laload : public NoDataOperation
    {
    public:
        laload() : NoDataOperation(OperationCode::laload)
        {
        }
    };

    class land : public NoDataOperation
    {
    public:
        land() : NoDataOperation(OperationCode::land)
        {
        }
    };

    class lastore : public NoDataOperation
    {
    public:
        lastore() : NoDataOperation(OperationCode::lastore)
        {
        }
    };

    class lcmp : public NoDataOperation
    {
    public:
        lcmp() : NoDataOperation(OperationCode::lcmp)
        {
        }
    };

    class lconst_0 : public NoDataOperation
    {
    public:
        lconst_0() : NoDataOperation(OperationCode::lconst_0)
        {
        }
    };

    class lconst_1 : public NoDataOperation
    {
    public:
        lconst_1() : NoDataOperation(OperationCode::lconst_1)
        {
        }
    };

    class ldc : public SingleByteDataOperation
    {
    public:
        ldc(const OperationData& inOperationData) : SingleByteDataOperation(OperationCode::ldc, inOperationData)
        {
        }
    };

    class ldc_w : public BiByteIndexDataOperation
    {
    public:
        ldc_w(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::ldc_w, inOperationData)
        {
        }
    };

    class ldc2_w : public BiByteIndexDataOperation
    {
    public:
        ldc2_w(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::ldc2_w, inOperationData)
        {
        }
    };

    class ldiv : public NoDataOperation
    {
    public:
        ldiv() : NoDataOperation(OperationCode::ldiv)
        {
        }
    };

    class lload : public SingleByteDataOperation
    {
    public:
        lload(const OperationData& inOperationData) : SingleByteDataOperation(OperationCode::lload, inOperationData)
        {
        }

    protected:
        lload(const OperationCode inOperationCode, const OperationData& inOperationData) : SingleByteDataOperation(inOperationCode, inOperationData)
        {
        }
    };

    class lload_0 : public lload
    {
    public:
        lload_0() : lload(OperationCode::lload_0, {0})
        {
        }
    };

    class lload_1 : public lload
    {
    public:
        lload_1() : lload(OperationCode::lload_1, {1})
        {
        }
    };

    class lload_2 : public lload
    {
    public:
        lload_2() : lload(OperationCode::lload_2, {2})
        {
        }
    };

    class lload_3 : public lload
    {
    public:
        lload_3() : lload(OperationCode::lload_3, {3})
        {
        }
    };

    class lmul : public SingleByteDataOperation
    {
    public:
        lmul(const OperationData& inOperationData) : SingleByteDataOperation(OperationCode::lmul, inOperationData)
        {
        }
    };

    class lneg : public SingleByteDataOperation
    {
    public:
        lneg(const OperationData& inOperationData) : SingleByteDataOperation(OperationCode::lneg, inOperationData)
        {
        }
    };

    class lookupswitch : public Operation
    {
    public:
        lookupswitch(const OperationData& inOperationData) : Operation(OperationCode::lookupswitch, inOperationData)
        {
        }
    };

    class lor : public NoDataOperation
    {
    public:
        lor() : NoDataOperation(OperationCode::lor)
        {
        }
    };

    class lrem : public NoDataOperation
    {
    public:
        lrem() : NoDataOperation(OperationCode::lrem)
        {
        }
    };

    class lreturn : public NoDataOperation
    {
    public:
        lreturn() : NoDataOperation(OperationCode::lreturn)
        {
        }
    };

    class lshl : public NoDataOperation
    {
    public:
        lshl() : NoDataOperation(OperationCode::lshl)
        {
        }
    };

    class lshr : public NoDataOperation
    {
    public:
        lshr() : NoDataOperation(OperationCode::lshr)
        {
        }
    };

    class lstore : public SingleByteDataOperation
    {
    public:
        lstore(const OperationData& inOperationData) : SingleByteDataOperation(OperationCode::lstore, inOperationData)
        {
        }

    protected:
        lstore(const OperationCode inOperationCode, const OperationData& inOperationData) : SingleByteDataOperation(inOperationCode, inOperationData)
        {
        }
    };

    class lstore_0 : public lstore
    {
    public:
        lstore_0() : lstore(OperationCode::lstore, {0})
        {
        }
    };

    class lstore_1 : public lstore
    {
    public:
        lstore_1() : lstore(OperationCode::lstore_1, {1})
        {
        }
    };

    class lstore_2 : public lstore
    {
    public:
        lstore_2() : lstore(OperationCode::lstore_2, {2})
        {
        }
    };

    class lstore_3 : public lstore
    {
    public:
        lstore_3() : lstore(OperationCode::lstore_3, {3})
        {
        }
    };

    class lsub : public NoDataOperation
    {
    public:
        lsub() : NoDataOperation(OperationCode::lsub)
        {
        }
    };

    class lushr : public NoDataOperation
    {
    public:
        lushr() : NoDataOperation(OperationCode::lushr)
        {
        }
    };

    class lxor : public NoDataOperation
    {
    public:
        lxor() : NoDataOperation(OperationCode::lxor)
        {
        }
    };

    class monitorenter : public NoDataOperation
    {
    public:
        monitorenter() : NoDataOperation(OperationCode::monitorenter)
        {
        }
    };

    class monitorexit : public NoDataOperation
    {
    public:
        monitorexit() : NoDataOperation(OperationCode::monitorexit)
        {
        }
    };

    class multianewarray : public Operation
    {
        static constexpr int32_t DATA_SIZE = 3;

    public:
        multianewarray(const OperationData& inOperationData) : Operation(OperationCode::multianewarray, inOperationData)
        {
            if (data.size() != DATA_SIZE)
            {
                throw Compiler::IncorrectDataSizeException(DATA_SIZE, data.size());
            }

            indexByte1 = data[0];
            indexByte2 = data[1];
            dimensions = data[2];
        }

        JVM::u1 getIndex() const
        {
            return ((indexByte1 << 8) | indexByte2);
        }

        JVM::u1 getIndexByte1() const
        {
            return indexByte1;
        }

        JVM::u1 getIndexByte2() const
        {
            return indexByte2;
        }

        JVM::u1 getDimensions() const
        {
            return dimensions;
        }

    protected:
        JVM::u1 indexByte1;
        JVM::u1 indexByte2;
        JVM::u1 dimensions;
    };

    class NEW : public BiByteIndexDataOperation
    {
    public:
        NEW(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::NEW, inOperationData)
        {
        }
    };

    class newarray : public SingleByteDataOperation
    {
        enum class ArrayType : JVM::u1
        {
            T_BOOLEAN = 4,
            T_CHAR = 5,
            T_FLOAT = 6,
            T_DOUBLE = 7,
            T_BYTE = 8,
            T_SHORT = 9,
            T_INT = 10,
            T_LONG = 11,
        };

    public:
        newarray(const OperationData& inOperationData) : SingleByteDataOperation(OperationCode::newarray, inOperationData)
        {
            if (data[0] < static_cast<JVM::u1>(ArrayType::T_BOOLEAN) || static_cast<JVM::u1>(data[0]) > 11)
            {
                throw Compiler::RuntimeException(fmt::format("Incorrect array type {:#06x}", data[0]));
            }
        }
    };

    class nop : public NoDataOperation
    {
    public:
        nop() : NoDataOperation(OperationCode::nop)
        {
        }
    };

    class pop : public NoDataOperation
    {
    public:
        pop() : NoDataOperation(OperationCode::pop)
        {
        }
    };

    class pop2 : public NoDataOperation
    {
    public:
        pop2() : NoDataOperation(OperationCode::pop2)
        {
        }
    };

    class putfield : public BiByteIndexDataOperation
    {
    public:
        putfield(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::putfield, inOperationData)
        {
        }
    };

    class putstatic : public BiByteIndexDataOperation
    {
    public:
        putstatic(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::putstatic, inOperationData)
        {
        }
    };

    class ret : public SingleByteDataOperation
    {
    public:
        ret(const OperationData& inOperationData) : SingleByteDataOperation(OperationCode::ret, inOperationData)
        {
        }
    };

    class RETURN : public NoDataOperation
    {
    public:
        RETURN() : NoDataOperation(OperationCode::RETURN)
        {
        }
    };

    class saload : public NoDataOperation
    {
    public:
        saload() : NoDataOperation(OperationCode::saload)
        {
        }
    };

    class sastore : public NoDataOperation
    {
    public:
        sastore() : NoDataOperation(OperationCode::sastore)
        {
        }
    };

    class sipush : public BiByteIndexDataOperation
    {
    public:
        sipush(const OperationData& inOperationData) : BiByteIndexDataOperation(OperationCode::sipush, inOperationData)
        {
        }
    };

    class swap : public NoDataOperation
    {
    public:
        swap() : NoDataOperation(OperationCode::swap)
        {
        }
    };

    class tableswitch : public Operation
    {
    public:
        tableswitch(const OperationData& inOperationData) : Operation(OperationCode::tableswitch, inOperationData)
        {
        }
    };

    class wide : public Operation
    {
    public:
        wide(const OperationData& inOperationData) : Operation(OperationCode::wide, inOperationData)
        {
        }
    };
}

#endif //SUPERJET_OPERATIONS_H
