#undef CASE

#ifdef USELABELS
#define CASE(opcode) opc ## opcode
#define DEFAULT opc_default
#else
#define CASE(opcode) case Bytecodes:: opcode
#define DEFAULT default
#endif

CASE(_nop):
    UPDATE_PC_AND_CONTINUE(2);

    /* Push miscellaneous constants onto the stack. */

CASE(_aconst_null):
    SET_STACK_OBJECT(NULL, 0);
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, 1);

#undef  OPC_CONST_n
#define OPC_CONST_n(opcode, const_type, value)                          \
CASE(opcode):                                                     \
    SET_STACK_ ## const_type(value, 0);                           \
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, 1);

    OPC_CONST_n(_iconst_m1,   INT,       -1);
    OPC_CONST_n(_iconst_0,    INT,        0);
    OPC_CONST_n(_iconst_1,    INT,        1);
    OPC_CONST_n(_iconst_2,    INT,        2);
    OPC_CONST_n(_iconst_3,    INT,        3);
    OPC_CONST_n(_iconst_4,    INT,        4);
    OPC_CONST_n(_iconst_5,    INT,        5);
    OPC_CONST_n(_fconst_0,    FLOAT,      0.0);
    OPC_CONST_n(_fconst_1,    FLOAT,      1.0);
    OPC_CONST_n(_fconst_2,    FLOAT,      2.0);

#undef  OPC_CONST2_n
#define OPC_CONST2_n(opcname, value, key, kind)                         \
CASE(_##opcname):                                                 \
{                                                                 \
    SET_STACK_ ## kind(VM##key##Const##value(), 1);               \
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, 2);                         \
}
   OPC_CONST2_n(dconst_0, Zero, double, DOUBLE);
   OPC_CONST2_n(dconst_1, One,  double, DOUBLE);
   OPC_CONST2_n(lconst_0, Zero, long, LONG);
   OPC_CONST2_n(lconst_1, One,  long, LONG);

   /* Load constant from constant pool: */

    /* Push a 1-byte signed integer value onto the stack. */
CASE(_bipush):
    SET_STACK_INT((jbyte)(pc[2]), 0);
    UPDATE_PC_AND_TOS_AND_CONTINUE(3, 1);

    /* Push a 2-byte signed integer constant onto the stack. */
CASE(_sipush):
    SET_STACK_INT((int16_t)Bytes::get_Java_u2(pc + 2), 0);
    UPDATE_PC_AND_TOS_AND_CONTINUE(4, 1);

    /* load from local variable */

CASE(_aload):
    VERIFY_OOP(LOCALS_OBJECT(pc[2]));
    SET_STACK_OBJECT(LOCALS_OBJECT(pc[2]), 0);
    UPDATE_PC_AND_TOS_AND_CONTINUE(3, 1);

CASE(_iload):
CASE(_fload):
    SET_STACK_SLOT(LOCALS_SLOT(pc[2]), 0);
    UPDATE_PC_AND_TOS_AND_CONTINUE(3, 1);

CASE(_lload):
    SET_STACK_LONG_FROM_ADDR(LOCALS_LONG_AT(pc[2]), 1);
    UPDATE_PC_AND_TOS_AND_CONTINUE(3, 2);

CASE(_dload):
    SET_STACK_DOUBLE_FROM_ADDR(LOCALS_DOUBLE_AT(pc[2]), 1);
    UPDATE_PC_AND_TOS_AND_CONTINUE(3, 2);

#undef  OPC_LOAD_n
#define OPC_LOAD_n(num)                                                 \
CASE(_aload_##num):                                               \
    VERIFY_OOP(LOCALS_OBJECT(num));                               \
    SET_STACK_OBJECT(LOCALS_OBJECT(num), 0);                      \
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, 1);                         \
                                                                  \
CASE(_iload_##num):                                               \
CASE(_fload_##num):                                               \
    SET_STACK_SLOT(LOCALS_SLOT(num), 0);                          \
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, 1);                         \
                                                                  \
CASE(_lload_##num):                                               \
    SET_STACK_LONG_FROM_ADDR(LOCALS_LONG_AT(num), 1);             \
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, 2);                         \
CASE(_dload_##num):                                               \
    SET_STACK_DOUBLE_FROM_ADDR(LOCALS_DOUBLE_AT(num), 1);         \
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, 2);

    OPC_LOAD_n(0);
    OPC_LOAD_n(1);
    OPC_LOAD_n(2);
    OPC_LOAD_n(3);

    /* store to a local variable */

CASE(_astore):
    astore(topOfStack, -1, locals, pc[2]);
    UPDATE_PC_AND_TOS_AND_CONTINUE(3, -1);

CASE(_istore):
CASE(_fstore):
    SET_LOCALS_SLOT(STACK_SLOT(-1), pc[2]);
    UPDATE_PC_AND_TOS_AND_CONTINUE(3, -1);

CASE(_lstore):
    SET_LOCALS_LONG(STACK_LONG(-1), pc[2]);
    UPDATE_PC_AND_TOS_AND_CONTINUE(3, -2);

CASE(_dstore):
    SET_LOCALS_DOUBLE(STACK_DOUBLE(-1), pc[2]);
    UPDATE_PC_AND_TOS_AND_CONTINUE(3, -2);

CASE(_wide): {
    uint16_t reg = Bytes::get_Java_u2(pc + 4);

    opcode = READ_BYTECODE(pc + 2);

    // Wide and it's sub-bytecode are counted as separate instructions. If we
    // don't account for this here, the bytecode trace skips the next bytecode.
    DO_UPDATE_INSTRUCTION_COUNT(opcode);

    switch(opcode) {
        case Bytecodes::_aload:
            VERIFY_OOP(LOCALS_OBJECT(reg));
            SET_STACK_OBJECT(LOCALS_OBJECT(reg), 0);
            UPDATE_PC_AND_TOS_AND_CONTINUE(6, 1);

        case Bytecodes::_iload:
        case Bytecodes::_fload:
            SET_STACK_SLOT(LOCALS_SLOT(reg), 0);
            UPDATE_PC_AND_TOS_AND_CONTINUE(6, 1);

        case Bytecodes::_lload:
            SET_STACK_LONG_FROM_ADDR(LOCALS_LONG_AT(reg), 1);
            UPDATE_PC_AND_TOS_AND_CONTINUE(6, 2);

        case Bytecodes::_dload:
            SET_STACK_DOUBLE_FROM_ADDR(LOCALS_LONG_AT(reg), 1);
            UPDATE_PC_AND_TOS_AND_CONTINUE(6, 2);

        case Bytecodes::_astore:
            astore(topOfStack, -1, locals, reg);
            UPDATE_PC_AND_TOS_AND_CONTINUE(6, -1);

        case Bytecodes::_istore:
        case Bytecodes::_fstore:
            SET_LOCALS_SLOT(STACK_SLOT(-1), reg);
            UPDATE_PC_AND_TOS_AND_CONTINUE(6, -1);

        case Bytecodes::_lstore:
            SET_LOCALS_LONG(STACK_LONG(-1), reg);
            UPDATE_PC_AND_TOS_AND_CONTINUE(6, -2);

        case Bytecodes::_dstore:
            SET_LOCALS_DOUBLE(STACK_DOUBLE(-1), reg);
            UPDATE_PC_AND_TOS_AND_CONTINUE(6, -2);

        case Bytecodes::_iinc: {
            int16_t offset = (int16_t)Bytes::get_Java_u2(pc+6);
            // Be nice to see what this generates.... QQQ
            SET_LOCALS_INT(LOCALS_INT(reg) + offset, reg);
            UPDATE_PC_AND_CONTINUE(8);
        }
        case Bytecodes::_ret:
            // Profile ret.
            BI_PROFILE_UPDATE_RET(/*bci=*/((int)(intptr_t)(LOCALS_ADDR(reg))));
            // Now, update the pc.
            pc = istate->method()->code_base() + (intptr_t)(LOCALS_ADDR(reg));
            UPDATE_PC_AND_CONTINUE(0);
        default:
            VM_JAVA_ERROR(vmSymbols::java_lang_InternalError(), "undefined opcode", note_no_trap);
    }
}


#undef  OPC_STORE_n
#define OPC_STORE_n(num)                                                \
CASE(_astore_##num):                                              \
    astore(topOfStack, -1, locals, num);                          \
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, -1);                        \
CASE(_istore_##num):                                              \
CASE(_fstore_##num):                                              \
    SET_LOCALS_SLOT(STACK_SLOT(-1), num);                         \
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, -1);

    OPC_STORE_n(0);
    OPC_STORE_n(1);
    OPC_STORE_n(2);
    OPC_STORE_n(3);

#undef  OPC_DSTORE_n
#define OPC_DSTORE_n(num)                                               \
CASE(_dstore_##num):                                              \
    SET_LOCALS_DOUBLE(STACK_DOUBLE(-1), num);                     \
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, -2);                        \
CASE(_lstore_##num):                                              \
    SET_LOCALS_LONG(STACK_LONG(-1), num);                         \
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, -2);

    OPC_DSTORE_n(0);
    OPC_DSTORE_n(1);
    OPC_DSTORE_n(2);
    OPC_DSTORE_n(3);

    /* stack pop, dup, and insert opcodes */


CASE(_pop):                /* Discard the top item on the stack */
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, -1);


CASE(_pop2):               /* Discard the top 2 items on the stack */
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, -2);


CASE(_dup):               /* Duplicate the top item on the stack */
    dup(topOfStack);
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, 1);

CASE(_dup2):              /* Duplicate the top 2 items on the stack */
    dup2(topOfStack);
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, 2);

CASE(_dup_x1):    /* insert top word two down */
    dup_x1(topOfStack);
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, 1);

CASE(_dup_x2):    /* insert top word three down  */
    dup_x2(topOfStack);
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, 1);

CASE(_dup2_x1):   /* insert top 2 slots three down */
    dup2_x1(topOfStack);
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, 2);

CASE(_dup2_x2):   /* insert top 2 slots four down */
    dup2_x2(topOfStack);
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, 2);

CASE(_swap): {        /* swap top two elements on the stack */
    swap(topOfStack);
    UPDATE_PC_AND_CONTINUE(2);
}

    /* Perform various binary integer operations */

#undef  OPC_INT_BINARY
#define OPC_INT_BINARY(opcname, opname, test)                           \
CASE(_i##opcname):                                                \
    if (test && (STACK_INT(-1) == 0)) {                           \
        VM_JAVA_ERROR(vmSymbols::java_lang_ArithmeticException(), \
                      "/ by zero", note_div0Check_trap);          \
    }                                                             \
    SET_STACK_INT(VMint##opname(STACK_INT(-2),                    \
                                STACK_INT(-1)),                   \
                                -2);                              \
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, -1);                        \
CASE(_l##opcname):                                                \
{                                                                 \
    if (test) {                                                   \
      jlong l1 = STACK_LONG(-1);                                  \
      if (VMlongEqz(l1)) {                                        \
        VM_JAVA_ERROR(vmSymbols::java_lang_ArithmeticException(), \
                      "/ by long zero", note_div0Check_trap);     \
      }                                                           \
    }                                                             \
    /* First long at (-1,-2) next long at (-3,-4) */              \
    SET_STACK_LONG(VMlong##opname(STACK_LONG(-3),                 \
                                  STACK_LONG(-1)),                \
                                  -3);                            \
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, -2);                        \
}

OPC_INT_BINARY(add, Add, 0);
OPC_INT_BINARY(sub, Sub, 0);
OPC_INT_BINARY(mul, Mul, 0);
OPC_INT_BINARY(and, And, 0);
OPC_INT_BINARY(or,  Or,  0);
OPC_INT_BINARY(xor, Xor, 0);
OPC_INT_BINARY(div, Div, 1);
OPC_INT_BINARY(rem, Rem, 1);


/* Perform various binary floating number operations */
/* On some machine/platforms/compilers div zero check can be implicit */

#undef  OPC_FLOAT_BINARY
#define OPC_FLOAT_BINARY(opcname, opname)                                  \
CASE(_d##opcname): {                                                 \
    SET_STACK_DOUBLE(VMdouble##opname(STACK_DOUBLE(-3),              \
                                      STACK_DOUBLE(-1)),             \
                                      -3);                           \
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, -2);                           \
}                                                                    \
CASE(_f##opcname):                                                   \
    SET_STACK_FLOAT(VMfloat##opname(STACK_FLOAT(-2),                 \
                                    STACK_FLOAT(-1)),                \
                                    -2);                             \
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, -1);


OPC_FLOAT_BINARY(add, Add);
OPC_FLOAT_BINARY(sub, Sub);
OPC_FLOAT_BINARY(mul, Mul);
OPC_FLOAT_BINARY(div, Div);
OPC_FLOAT_BINARY(rem, Rem);

/* Shift operations
 * Shift left int and long: ishl, lshl
 * Logical shift right int and long w/zero extension: iushr, lushr
 * Arithmetic shift right int and long w/sign extension: ishr, lshr
 */

#undef  OPC_SHIFT_BINARY
#define OPC_SHIFT_BINARY(opcname, opname)                               \
CASE(_i##opcname):                                                \
   SET_STACK_INT(VMint##opname(STACK_INT(-2),                     \
                               STACK_INT(-1)),                    \
                               -2);                               \
   UPDATE_PC_AND_TOS_AND_CONTINUE(2, -1);                         \
CASE(_l##opcname):                                                \
{                                                                 \
   SET_STACK_LONG(VMlong##opname(STACK_LONG(-2),                  \
                                 STACK_INT(-1)),                  \
                                 -2);                             \
   UPDATE_PC_AND_TOS_AND_CONTINUE(2, -1);                         \
}

OPC_SHIFT_BINARY(shl, Shl);
OPC_SHIFT_BINARY(shr, Shr);
OPC_SHIFT_BINARY(ushr, Ushr);

/* Increment local variable by constant */
CASE(_iinc):
{
    // locals[pc[1]].j.i += (jbyte)(pc[2]);
    SET_LOCALS_INT(LOCALS_INT(pc[2]) + (jbyte)(pc[3]), pc[2]);
    UPDATE_PC_AND_CONTINUE(4);
}

/* negate the value on the top of the stack */

CASE(_ineg):
   SET_STACK_INT(VMintNeg(STACK_INT(-1)), -1);
   UPDATE_PC_AND_CONTINUE(2);

CASE(_fneg):
   SET_STACK_FLOAT(VMfloatNeg(STACK_FLOAT(-1)), -1);
   UPDATE_PC_AND_CONTINUE(2);

CASE(_lneg):
{
   SET_STACK_LONG(VMlongNeg(STACK_LONG(-1)), -1);
   UPDATE_PC_AND_CONTINUE(2);
}

CASE(_dneg):
{
   SET_STACK_DOUBLE(VMdoubleNeg(STACK_DOUBLE(-1)), -1);
   UPDATE_PC_AND_CONTINUE(2);
}

/* Conversion operations */

CASE(_i2f):       /* convert top of stack int to float */
   SET_STACK_FLOAT(VMint2Float(STACK_INT(-1)), -1);
   UPDATE_PC_AND_CONTINUE(2);

CASE(_i2l):       /* convert top of stack int to long */
{
    // this is ugly QQQ
    jlong r = VMint2Long(STACK_INT(-1));
    MORE_STACK(-1); // Pop
    SET_STACK_LONG(r, 1);

    UPDATE_PC_AND_TOS_AND_CONTINUE(2, 2);
}

CASE(_i2d):       /* convert top of stack int to double */
{
    // this is ugly QQQ (why cast to jlong?? )
    jdouble r = (jlong)STACK_INT(-1);
    MORE_STACK(-1); // Pop
    SET_STACK_DOUBLE(r, 1);

    UPDATE_PC_AND_TOS_AND_CONTINUE(2, 2);
}

CASE(_l2i):       /* convert top of stack long to int */
{
    jint r = VMlong2Int(STACK_LONG(-1));
    MORE_STACK(-2); // Pop
    SET_STACK_INT(r, 0);
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, 1);
}

CASE(_l2f):   /* convert top of stack long to float */
{
    jlong r = STACK_LONG(-1);
    MORE_STACK(-2); // Pop
    SET_STACK_FLOAT(VMlong2Float(r), 0);
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, 1);
}

CASE(_l2d):       /* convert top of stack long to double */
{
    jlong r = STACK_LONG(-1);
    MORE_STACK(-2); // Pop
    SET_STACK_DOUBLE(VMlong2Double(r), 1);
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, 2);
}

CASE(_f2i):  /* Convert top of stack float to int */
    SET_STACK_INT(SharedRuntime::f2i(STACK_FLOAT(-1)), -1);
    UPDATE_PC_AND_CONTINUE(2);

CASE(_f2l):  /* convert top of stack float to long */
{
    jlong r = SharedRuntime::f2l(STACK_FLOAT(-1));
    MORE_STACK(-1); // POP
    SET_STACK_LONG(r, 1);
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, 2);
}

CASE(_f2d):  /* convert top of stack float to double */
{
    jfloat f;
    jdouble r;
    f = STACK_FLOAT(-1);
    r = (jdouble) f;
    MORE_STACK(-1); // POP
    SET_STACK_DOUBLE(r, 1);
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, 2);
}

CASE(_d2i): /* convert top of stack double to int */
{
    jint r1 = SharedRuntime::d2i(STACK_DOUBLE(-1));
    MORE_STACK(-2);
    SET_STACK_INT(r1, 0);
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, 1);
}

CASE(_d2f): /* convert top of stack double to float */
{
    jfloat r1 = VMdouble2Float(STACK_DOUBLE(-1));
    MORE_STACK(-2);
    SET_STACK_FLOAT(r1, 0);
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, 1);
}

CASE(_d2l): /* convert top of stack double to long */
{
    jlong r1 = SharedRuntime::d2l(STACK_DOUBLE(-1));
    MORE_STACK(-2);
    SET_STACK_LONG(r1, 1);
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, 2);
}

CASE(_i2b):
    SET_STACK_INT(VMint2Byte(STACK_INT(-1)), -1);
    UPDATE_PC_AND_CONTINUE(2);

CASE(_i2c):
    SET_STACK_INT(VMint2Char(STACK_INT(-1)), -1);
    UPDATE_PC_AND_CONTINUE(2);

CASE(_i2s):
    SET_STACK_INT(VMint2Short(STACK_INT(-1)), -1);
    UPDATE_PC_AND_CONTINUE(2);

/* comparison operators */


#define COMPARISON_OP(name, comparison)                                      \
CASE(_if_icmp##name): {                                                \
    const bool cmp = (STACK_INT(-2) comparison STACK_INT(-1));         \
    int skip = cmp                                                     \
                ? (int16_t)Bytes::get_Java_u2(pc + 2) : 4;             \
    address branch_pc = pc;                                            \
    /* Profile branch. */                                              \
    BI_PROFILE_UPDATE_BRANCH(/*is_taken=*/cmp);                        \
    UPDATE_PC_AND_TOS(skip, -2);                                       \
    DO_BACKEDGE_CHECKS(skip, branch_pc);                               \
    CONTINUE;                                                          \
}                                                                      \
CASE(_if##name): {                                                     \
    const bool cmp = (STACK_INT(-1) comparison 0);                     \
    int skip = cmp                                                     \
                ? (int16_t)Bytes::get_Java_u2(pc + 2) : 4;             \
    address branch_pc = pc;                                            \
    /* Profile branch. */                                              \
    BI_PROFILE_UPDATE_BRANCH(/*is_taken=*/cmp);                        \
    UPDATE_PC_AND_TOS(skip, -1);                                       \
    DO_BACKEDGE_CHECKS(skip, branch_pc);                               \
    CONTINUE;                                                          \
}

#define COMPARISON_OP2(name, comparison)                                     \
COMPARISON_OP(name, comparison)                                        \
CASE(_if_acmp##name): {                                                \
    const bool cmp = (STACK_OBJECT(-2) comparison STACK_OBJECT(-1));   \
    int skip = cmp                                                     \
                 ? (int16_t)Bytes::get_Java_u2(pc + 2) : 4;            \
    address branch_pc = pc;                                            \
    /* Profile branch. */                                              \
    BI_PROFILE_UPDATE_BRANCH(/*is_taken=*/cmp);                        \
    UPDATE_PC_AND_TOS(skip, -2);                                       \
    DO_BACKEDGE_CHECKS(skip, branch_pc);                               \
    CONTINUE;                                                          \
}

#define NULL_COMPARISON_NOT_OP(name)                                         \
CASE(_if##name): {                                                     \
    const bool cmp = (!(STACK_OBJECT(-1) == NULL));                    \
    int skip = cmp                                                     \
                ? (int16_t)Bytes::get_Java_u2(pc + 2) : 4;             \
    address branch_pc = pc;                                            \
    /* Profile branch. */                                              \
    BI_PROFILE_UPDATE_BRANCH(/*is_taken=*/cmp);                        \
    UPDATE_PC_AND_TOS(skip, -1);                                       \
    DO_BACKEDGE_CHECKS(skip, branch_pc);                               \
    CONTINUE;                                                          \
}

#define NULL_COMPARISON_OP(name)                                             \
CASE(_if##name): {                                                     \
    const bool cmp = ((STACK_OBJECT(-1) == NULL));                     \
    int skip = cmp                                                     \
                ? (int16_t)Bytes::get_Java_u2(pc + 2) : 4;             \
    address branch_pc = pc;                                            \
    /* Profile branch. */                                              \
    BI_PROFILE_UPDATE_BRANCH(/*is_taken=*/cmp);                        \
    UPDATE_PC_AND_TOS(skip, -1);                                       \
    DO_BACKEDGE_CHECKS(skip, branch_pc);                               \
    CONTINUE;                                                          \
}
COMPARISON_OP(lt, <);
COMPARISON_OP(gt, >);
COMPARISON_OP(le, <=);
COMPARISON_OP(ge, >=);
COMPARISON_OP2(eq, ==);  /* include ref comparison */
COMPARISON_OP2(ne, !=);  /* include ref comparison */
NULL_COMPARISON_OP(null);
NULL_COMPARISON_NOT_OP(nonnull);

/* Goto pc at specified offset in switch table. */

CASE(_tableswitch): {
    jint* lpc  = (jint*)VMalignWordUp(pc+2);
    int32_t  key  = STACK_INT(-1);
    int32_t  low  = Bytes::get_Java_u4((address)&lpc[1]);
    int32_t  high = Bytes::get_Java_u4((address)&lpc[2]);
    int32_t  skip;
    key -= low;
    if (((uint32_t) key > (uint32_t)(high - low))) {
      key = -1;
      skip = Bytes::get_Java_u4((address)&lpc[0]);
    } else {
      skip = Bytes::get_Java_u4((address)&lpc[key + 3]);
    }
    // Profile switch.
    BI_PROFILE_UPDATE_SWITCH(/*switch_index=*/key);
    // Does this really need a full backedge check (osr)?
    address branch_pc = pc;
    UPDATE_PC_AND_TOS(skip, -1);
    DO_BACKEDGE_CHECKS(skip, branch_pc);
    CONTINUE;
}

/* Goto pc whose table entry matches specified key. */

CASE(_lookupswitch): {
    jint* lpc  = (jint*)VMalignWordUp(pc+2);
    int32_t  key  = STACK_INT(-1);
    int32_t  skip = Bytes::get_Java_u4((address) lpc); /* default amount */
    // Remember index.
    int      index = -1;
    int      newindex = 0;
    int32_t  npairs = Bytes::get_Java_u4((address) &lpc[1]);
    while (--npairs >= 0) {
      lpc += 2;
      if (key == (int32_t)Bytes::get_Java_u4((address)lpc)) {
        skip = Bytes::get_Java_u4((address)&lpc[1]);
        index = newindex;
        break;
      }
      newindex += 1;
    }
    // Profile switch.
    BI_PROFILE_UPDATE_SWITCH(/*switch_index=*/index);
    address branch_pc = pc;
    UPDATE_PC_AND_TOS(skip, -1);
    DO_BACKEDGE_CHECKS(skip, branch_pc);
    CONTINUE;
}

CASE(_fcmpl):
CASE(_fcmpg):
{
    SET_STACK_INT(VMfloatCompare(STACK_FLOAT(-2),
                                  STACK_FLOAT(-1),
                                  (opcode == Bytecodes::_fcmpl ? -1 : 1)),
                  -2);
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, -1);
}

CASE(_dcmpl):
CASE(_dcmpg):
{
    int r = VMdoubleCompare(STACK_DOUBLE(-3),
                            STACK_DOUBLE(-1),
                            (opcode == Bytecodes::_dcmpl ? -1 : 1));
    MORE_STACK(-4); // Pop
    SET_STACK_INT(r, 0);
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, 1);
}

CASE(_lcmp):
{
    int r = VMlongCompare(STACK_LONG(-3), STACK_LONG(-1));
    MORE_STACK(-4);
    SET_STACK_INT(r, 0);
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, 1);
}


/* Return from a method */

CASE(_areturn):
CASE(_ireturn):
CASE(_freturn):
{
    // Allow a safepoint before returning to frame manager.
    SAFEPOINT;

    goto handle_return;
}

CASE(_lreturn):
CASE(_dreturn):
{
    // Allow a safepoint before returning to frame manager.
    SAFEPOINT;
    goto handle_return;
}

CASE(_return_register_finalizer): {

    oop rcvr = LOCALS_OBJECT(0);
    VERIFY_OOP(rcvr);
    if (rcvr->klass()->has_finalizer()) {
      CALL_VM(InterpreterRuntime::register_finalizer(THREAD, rcvr), handle_exception);
    }
    goto handle_return;
}
CASE(_return): {

    // Allow a safepoint before returning to frame manager.
    SAFEPOINT;
    goto handle_return;
}

/* Array access byte-codes */

/* Every array access byte-code starts out like this */
//        arrayOopDesc* arrObj = (arrayOopDesc*)STACK_OBJECT(arrayOff);
#undef ARRAY_INTRO
#define ARRAY_INTRO(arrayOff)                                                  \
arrayOop arrObj = (arrayOop)STACK_OBJECT(arrayOff);                      \
jint     index  = STACK_INT(arrayOff + 1);                               \
char message[jintAsStringSize];                                          \
CHECK_NULL(arrObj);                                                      \
if ((uint32_t)index >= (uint32_t)arrObj->length()) {                     \
    sprintf(message, "%d", index);                                       \
    VM_JAVA_ERROR(vmSymbols::java_lang_ArrayIndexOutOfBoundsException(), \
                  message, note_rangeCheck_trap);                        \
}

/* 32-bit loads. These handle conversion from < 32-bit types */
#undef ARRAY_LOADTO32
#define ARRAY_LOADTO32(T, T2, format, stackRes, extra)                                \
{                                                                               \
    ARRAY_INTRO(-2);                                                            \
    (void)extra;                                                                \
    SET_ ## stackRes(*(T2 *)(((address) arrObj->base(T)) + index * sizeof(T2)), \
                     -2);                                                       \
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, -1);                                      \
}

/* 64-bit loads */
#undef ARRAY_LOADTO64
#define ARRAY_LOADTO64(T,T2, stackRes, extra)                                              \
{                                                                                    \
    ARRAY_INTRO(-2);                                                                 \
    SET_ ## stackRes(*(T2 *)(((address) arrObj->base(T)) + index * sizeof(T2)), -1); \
    (void)extra;                                                                     \
    UPDATE_PC_AND_CONTINUE(2);                                                       \
}

CASE(_iaload):
    ARRAY_LOADTO32(T_INT, jint,   "%d",   STACK_INT, 0);
CASE(_faload):
    ARRAY_LOADTO32(T_FLOAT, jfloat, "%f",   STACK_FLOAT, 0);
CASE(_aaload): {
    ARRAY_INTRO(-2);
    SET_STACK_OBJECT(((objArrayOop) arrObj)->obj_at(index), -2);
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, -1);
}
CASE(_baload):
    ARRAY_LOADTO32(T_BYTE, jbyte,  "%d",   STACK_INT, 0);
CASE(_caload):
    ARRAY_LOADTO32(T_CHAR,  jchar, "%d",   STACK_INT, 0);
CASE(_saload):
    ARRAY_LOADTO32(T_SHORT, jshort, "%d",   STACK_INT, 0);
CASE(_laload):
    ARRAY_LOADTO64(T_LONG, jlong, STACK_LONG, 0);
CASE(_daload):
    ARRAY_LOADTO64(T_DOUBLE, jdouble, STACK_DOUBLE, 0);

/* 32-bit stores. These handle conversion to < 32-bit types */
#undef ARRAY_STOREFROM32
#define ARRAY_STOREFROM32(T, T2, format, stackSrc, extra)                            \
{                                                                              \
    ARRAY_INTRO(-3);                                                           \
    (void)extra;                                                               \
    *(T2 *)(((address) arrObj->base(T)) + index * sizeof(T2)) = stackSrc( -1); \
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, -3);                                     \
}

/* 64-bit stores */
#undef ARRAY_STOREFROM64
#define ARRAY_STOREFROM64(T, T2, stackSrc, extra)                                    \
{                                                                              \
    ARRAY_INTRO(-4);                                                           \
    (void)extra;                                                               \
    *(T2 *)(((address) arrObj->base(T)) + index * sizeof(T2)) = stackSrc( -1); \
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, -4);                                     \
}

CASE(_iastore):
    ARRAY_STOREFROM32(T_INT, jint,   "%d",   STACK_INT, 0);
CASE(_fastore):
    ARRAY_STOREFROM32(T_FLOAT, jfloat, "%f",   STACK_FLOAT, 0);
/*
 * This one looks different because of the assignability check
 */
CASE(_aastore): {
    oop rhsObject = STACK_OBJECT(-1);
    VERIFY_OOP(rhsObject);
    ARRAY_INTRO( -3);
    // arrObj, index are set
    if (rhsObject != NULL) {
      /* Check assignability of rhsObject into arrObj */
      Klass* rhsKlass = rhsObject->klass(); // EBX (subclass)
      Klass* elemKlass = ObjArrayKlass::cast(arrObj->klass())->element_klass(); // superklass EAX
      //
      // Check for compatibilty. This check must not GC!!
      // Seems way more expensive now that we must dispatch
      //
      if (rhsKlass != elemKlass && !rhsKlass->is_subtype_of(elemKlass)) { // ebx->is...
        // Decrement counter if subtype check failed.
        BI_PROFILE_SUBTYPECHECK_FAILED(rhsKlass);
        VM_JAVA_ERROR(vmSymbols::java_lang_ArrayStoreException(), "", note_arrayCheck_trap);
      }
      // Profile checkcast with null_seen and receiver.
      BI_PROFILE_UPDATE_CHECKCAST(/*null_seen=*/false, rhsKlass);
    } else {
      // Profile checkcast with null_seen and receiver.
      BI_PROFILE_UPDATE_CHECKCAST(/*null_seen=*/true, NULL);
    }
    ((objArrayOop) arrObj)->obj_at_put(index, rhsObject);
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, -3);
}
CASE(_bastore): {
    ARRAY_INTRO(-3);
    int item = STACK_INT(-1);
    // if it is a T_BOOLEAN array, mask the stored value to 0/1
    if (arrObj->klass() == Universe::boolArrayKlassObj()) {
      item &= 1;
    } else {
      assert(arrObj->klass() == Universe::byteArrayKlassObj(),
             "should be byte array otherwise");
    }
    ((typeArrayOop)arrObj)->byte_at_put(index, item);
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, -3);
}
CASE(_castore):
    ARRAY_STOREFROM32(T_CHAR, jchar,  "%d",   STACK_INT, 0);
CASE(_sastore):
    ARRAY_STOREFROM32(T_SHORT, jshort, "%d",   STACK_INT, 0);
CASE(_lastore):
    ARRAY_STOREFROM64(T_LONG, jlong, STACK_LONG, 0);
CASE(_dastore):
    ARRAY_STOREFROM64(T_DOUBLE, jdouble, STACK_DOUBLE, 0);

CASE(_arraylength):
{
    arrayOop ary = (arrayOop) STACK_OBJECT(-1);
    CHECK_NULL(ary);
    SET_STACK_INT(ary->length(), -1);
    UPDATE_PC_AND_CONTINUE(2);
}

/* monitorenter and monitorexit for locking/unlocking an object */

CASE(_monitorenter): {
  oop lockee = STACK_OBJECT(-1);
  // derefing's lockee ought to provoke implicit null check
  CHECK_NULL(lockee);
  // find a free monitor or one already allocated for this object
  // if we find a matching object then we need a new monitor
  // since this is recursive enter
  BasicObjectLock* limit = istate->monitor_base();
  BasicObjectLock* most_recent = (BasicObjectLock*) istate->stack_base();
  BasicObjectLock* entry = NULL;
  while (most_recent != limit ) {
    if (most_recent->obj() == NULL) entry = most_recent;
    else if (most_recent->obj() == lockee) break;
    most_recent++;
  }
  if (entry != NULL) {
    entry->set_obj(lockee);
    int success = false;
    uintptr_t epoch_mask_in_place = (uintptr_t)markOopDesc::epoch_mask_in_place;

    markOop mark = lockee->mark();
    intptr_t hash = (intptr_t) markOopDesc::no_hash;
    // implies UseBiasedLocking
    if (mark->has_bias_pattern()) {
      uintptr_t thread_ident;
      uintptr_t anticipated_bias_locking_value;
      thread_ident = (uintptr_t)istate->thread();
      anticipated_bias_locking_value =
        (((uintptr_t)lockee->klass()->prototype_header() | thread_ident) ^ (uintptr_t)mark) &
        ~((uintptr_t) markOopDesc::age_mask_in_place);

      if  (anticipated_bias_locking_value == 0) {
        // already biased towards this thread, nothing to do
        if (PrintBiasedLockingStatistics) {
          (* BiasedLocking::biased_lock_entry_count_addr())++;
        }
        success = true;
      }
      else if ((anticipated_bias_locking_value & markOopDesc::biased_lock_mask_in_place) != 0) {
        // try revoke bias
        markOop header = lockee->klass()->prototype_header();
        if (hash != markOopDesc::no_hash) {
          header = header->copy_set_hash(hash);
        }
        if (lockee->cas_set_mark(header, mark) == mark) {
          if (PrintBiasedLockingStatistics)
            (*BiasedLocking::revoked_lock_entry_count_addr())++;
        }
      }
      else if ((anticipated_bias_locking_value & epoch_mask_in_place) !=0) {
        // try rebias
        markOop new_header = (markOop) ( (intptr_t) lockee->klass()->prototype_header() | thread_ident);
        if (hash != markOopDesc::no_hash) {
          new_header = new_header->copy_set_hash(hash);
        }
        if (lockee->cas_set_mark(new_header, mark) == mark) {
          if (PrintBiasedLockingStatistics)
            (* BiasedLocking::rebiased_lock_entry_count_addr())++;
        }
        else {
          CALL_VM(InterpreterRuntime::monitorenter(THREAD, entry), handle_exception);
        }
        success = true;
      }
      else {
        // try to bias towards thread in case object is anonymously biased
        markOop header = (markOop) ((uintptr_t) mark & ((uintptr_t)markOopDesc::biased_lock_mask_in_place |
                                                        (uintptr_t)markOopDesc::age_mask_in_place |
                                                        epoch_mask_in_place));
        if (hash != markOopDesc::no_hash) {
          header = header->copy_set_hash(hash);
        }
        markOop new_header = (markOop) ((uintptr_t) header | thread_ident);
        // debugging hint
        DEBUG_ONLY(entry->lock()->set_displaced_header((markOop) (uintptr_t) 0xdeaddead);)
        if (lockee->cas_set_mark(new_header, header) == header) {
          if (PrintBiasedLockingStatistics)
            (* BiasedLocking::anonymously_biased_lock_entry_count_addr())++;
        }
        else {
          CALL_VM(InterpreterRuntime::monitorenter(THREAD, entry), handle_exception);
        }
        success = true;
      }
    }

    // traditional lightweight locking
    if (!success) {
      markOop displaced = lockee->mark()->set_unlocked();
      entry->lock()->set_displaced_header(displaced);
      bool call_vm = UseHeavyMonitors;
      if (call_vm || lockee->cas_set_mark((markOop)entry, displaced) != displaced) {
        // Is it simple recursive case?
        if (!call_vm && THREAD->is_lock_owned((address) displaced->clear_lock_bits())) {
          entry->lock()->set_displaced_header(NULL);
        } else {
          CALL_VM(InterpreterRuntime::monitorenter(THREAD, entry), handle_exception);
        }
      }
    }
    UPDATE_PC_AND_TOS_AND_CONTINUE(2, -1);
  } else {
    istate->set_msg(more_monitors);
    UPDATE_PC_AND_RETURN(0); // Re-execute
  }
}

CASE(_monitorexit): {
  oop lockee = STACK_OBJECT(-1);
  CHECK_NULL(lockee);
  // derefing's lockee ought to provoke implicit null check
  // find our monitor slot
  BasicObjectLock* limit = istate->monitor_base();
  BasicObjectLock* most_recent = (BasicObjectLock*) istate->stack_base();
  while (most_recent != limit ) {
    if ((most_recent)->obj() == lockee) {
      BasicLock* lock = most_recent->lock();
      markOop header = lock->displaced_header();
      most_recent->set_obj(NULL);
      if (!lockee->mark()->has_bias_pattern()) {
        bool call_vm = UseHeavyMonitors;
        // If it isn't recursive we either must swap old header or call the runtime
        if (header != NULL || call_vm) {
          markOop old_header = markOopDesc::encode(lock);
          if (call_vm || lockee->cas_set_mark(header, old_header) != old_header) {
            // restore object for the slow case
            most_recent->set_obj(lockee);
            CALL_VM(InterpreterRuntime::monitorexit(THREAD, most_recent), handle_exception);
          }
        }
      }
      UPDATE_PC_AND_TOS_AND_CONTINUE(2, -1);
    }
    most_recent++;
  }
  // Need to throw illegal monitor state exception
  CALL_VM(InterpreterRuntime::throw_illegal_monitor_state_exception(THREAD), handle_exception);
  ShouldNotReachHere();
}

/* All of the non-quick opcodes. */

/* -Set clobbersCpIndex true if the quickened opcode clobbers the
 *  constant pool index in the instruction.
 */
CASE(_getfield):
CASE(_getstatic):
  {
    u2 index;
    ConstantPoolCacheEntry* cache;
    index = Bytes::get_native_u2(pc+2);

    // QQQ Need to make this as inlined as possible. Probably need to
    // split all the bytecode cases out so c++ compiler has a chance
    // for constant prop to fold everything possible away.

    cache = cp->entry_at(index);
    if (!cache->is_resolved((Bytecodes::Code)opcode)) {
      CALL_VM(InterpreterRuntime::resolve_from_cache(THREAD, (Bytecodes::Code)opcode),
              handle_exception);
      cache = cp->entry_at(index);
    }

#ifdef VM_JVMTI
    if (_jvmti_interp_events) {
      int *count_addr;
      oop obj;
      // Check to see if a field modification watch has been set
      // before we take the time to call into the VM.
      count_addr = (int *)JvmtiExport::get_field_access_count_addr();
      if ( *count_addr > 0 ) {
        if ((Bytecodes::Code)opcode == Bytecodes::_getstatic) {
          obj = (oop)NULL;
        } else {
          obj = (oop) STACK_OBJECT(-1);
          VERIFY_OOP(obj);
        }
        CALL_VM(InterpreterRuntime::post_field_access(THREAD,
                                    obj,
                                    cache),
                                    handle_exception);
      }
    }
#endif /* VM_JVMTI */

    oop obj;
    if ((Bytecodes::Code)opcode == Bytecodes::_getstatic) {
      Klass* k = cache->f1_as_klass();
      obj = k->java_mirror();
      MORE_STACK(1);  // Assume single slot push
    } else {
      obj = (oop) STACK_OBJECT(-1);
      CHECK_NULL(obj);
    }

    //
    // Now store the result on the stack
    //
    TosState tos_type = cache->flag_state();
    int field_offset = cache->f2_as_index();
    if (cache->is_volatile()) {
      if (support_IRIW_for_not_multiple_copy_atomic_cpu) {
        OrderAccess::fence();
      }
      if (tos_type == atos) {
        VERIFY_OOP(obj->obj_field_acquire(field_offset));
        SET_STACK_OBJECT(obj->obj_field_acquire(field_offset), -1);
      } else if (tos_type == itos) {
        SET_STACK_INT(obj->int_field_acquire(field_offset), -1);
      } else if (tos_type == ltos) {
        SET_STACK_LONG(obj->long_field_acquire(field_offset), 0);
        MORE_STACK(1);
      } else if (tos_type == btos || tos_type == ztos) {
        SET_STACK_INT(obj->byte_field_acquire(field_offset), -1);
      } else if (tos_type == ctos) {
        SET_STACK_INT(obj->char_field_acquire(field_offset), -1);
      } else if (tos_type == stos) {
        SET_STACK_INT(obj->short_field_acquire(field_offset), -1);
      } else if (tos_type == ftos) {
        SET_STACK_FLOAT(obj->float_field_acquire(field_offset), -1);
      } else {
        SET_STACK_DOUBLE(obj->double_field_acquire(field_offset), 0);
        MORE_STACK(1);
      }
    } else {
      if (tos_type == atos) {
        VERIFY_OOP(obj->obj_field(field_offset));
        SET_STACK_OBJECT(obj->obj_field(field_offset), -1);
      } else if (tos_type == itos) {
        SET_STACK_INT(obj->int_field(field_offset), -1);
      } else if (tos_type == ltos) {
        SET_STACK_LONG(obj->long_field(field_offset), 0);
        MORE_STACK(1);
      } else if (tos_type == btos || tos_type == ztos) {
        SET_STACK_INT(obj->byte_field(field_offset), -1);
      } else if (tos_type == ctos) {
        SET_STACK_INT(obj->char_field(field_offset), -1);
      } else if (tos_type == stos) {
        SET_STACK_INT(obj->short_field(field_offset), -1);
      } else if (tos_type == ftos) {
        SET_STACK_FLOAT(obj->float_field(field_offset), -1);
      } else {
        SET_STACK_DOUBLE(obj->double_field(field_offset), 0);
        MORE_STACK(1);
      }
    }

    UPDATE_PC_AND_CONTINUE(4);
   }

CASE(_putfield):
CASE(_putstatic):
  {
    u2 index = Bytes::get_native_u2(pc+2);
    ConstantPoolCacheEntry* cache = cp->entry_at(index);
    if (!cache->is_resolved((Bytecodes::Code)opcode)) {
      CALL_VM(InterpreterRuntime::resolve_from_cache(THREAD, (Bytecodes::Code)opcode),
              handle_exception);
      cache = cp->entry_at(index);
    }

#ifdef VM_JVMTI
    if (_jvmti_interp_events) {
      int *count_addr;
      oop obj;
      // Check to see if a field modification watch has been set
      // before we take the time to call into the VM.
      count_addr = (int *)JvmtiExport::get_field_modification_count_addr();
      if ( *count_addr > 0 ) {
        if ((Bytecodes::Code)opcode == Bytecodes::_putstatic) {
          obj = (oop)NULL;
        }
        else {
          if (cache->is_long() || cache->is_double()) {
            obj = (oop) STACK_OBJECT(-3);
          } else {
            obj = (oop) STACK_OBJECT(-2);
          }
          VERIFY_OOP(obj);
        }

        CALL_VM(InterpreterRuntime::post_field_modification(THREAD,
                                    obj,
                                    cache,
                                    (jvalue *)STACK_SLOT(-1)),
                                    handle_exception);
      }
    }
#endif /* VM_JVMTI */

    // QQQ Need to make this as inlined as possible. Probably need to split all the bytecode cases
    // out so c++ compiler has a chance for constant prop to fold everything possible away.

    oop obj;
    int count;
    TosState tos_type = cache->flag_state();

    count = -1;
    if (tos_type == ltos || tos_type == dtos) {
      --count;
    }
    if ((Bytecodes::Code)opcode == Bytecodes::_putstatic) {
      Klass* k = cache->f1_as_klass();
      obj = k->java_mirror();
    } else {
      --count;
      obj = (oop) STACK_OBJECT(count);
      CHECK_NULL(obj);
    }

    //
    // Now store the result
    //
    int field_offset = cache->f2_as_index();
    if (cache->is_volatile()) {
      if (tos_type == itos) {
        obj->release_int_field_put(field_offset, STACK_INT(-1));
      } else if (tos_type == atos) {
        VERIFY_OOP(STACK_OBJECT(-1));
        obj->release_obj_field_put(field_offset, STACK_OBJECT(-1));
      } else if (tos_type == btos) {
        obj->release_byte_field_put(field_offset, STACK_INT(-1));
      } else if (tos_type == ztos) {
        int bool_field = STACK_INT(-1);  // only store LSB
        obj->release_byte_field_put(field_offset, (bool_field & 1));
      } else if (tos_type == ltos) {
        obj->release_long_field_put(field_offset, STACK_LONG(-1));
      } else if (tos_type == ctos) {
        obj->release_char_field_put(field_offset, STACK_INT(-1));
      } else if (tos_type == stos) {
        obj->release_short_field_put(field_offset, STACK_INT(-1));
      } else if (tos_type == ftos) {
        obj->release_float_field_put(field_offset, STACK_FLOAT(-1));
      } else {
        obj->release_double_field_put(field_offset, STACK_DOUBLE(-1));
      }
      OrderAccess::storeload();
    } else {
      if (tos_type == itos) {
        obj->int_field_put(field_offset, STACK_INT(-1));
      } else if (tos_type == atos) {
        VERIFY_OOP(STACK_OBJECT(-1));
        obj->obj_field_put(field_offset, STACK_OBJECT(-1));
      } else if (tos_type == btos) {
        obj->byte_field_put(field_offset, STACK_INT(-1));
      } else if (tos_type == ztos) {
        int bool_field = STACK_INT(-1);  // only store LSB
        obj->byte_field_put(field_offset, (bool_field & 1));
      } else if (tos_type == ltos) {
        obj->long_field_put(field_offset, STACK_LONG(-1));
      } else if (tos_type == ctos) {
        obj->char_field_put(field_offset, STACK_INT(-1));
      } else if (tos_type == stos) {
        obj->short_field_put(field_offset, STACK_INT(-1));
      } else if (tos_type == ftos) {
        obj->float_field_put(field_offset, STACK_FLOAT(-1));
      } else {
        obj->double_field_put(field_offset, STACK_DOUBLE(-1));
      }
    }

    UPDATE_PC_AND_TOS_AND_CONTINUE(4, count);
  }

CASE(_new): {
  u2 index = Bytes::get_Java_u2(pc+2);
  ConstantPool* constants = istate->method()->constants();
  if (!constants->tag_at(index).is_unresolved_klass()) {
    // Make sure klass is initialized and doesn't have a finalizer
    Klass* entry = constants->resolved_klass_at(index);
    InstanceKlass* ik = InstanceKlass::cast(entry);
    if (ik->is_initialized() && ik->can_be_fastpath_allocated() ) {
      size_t obj_size = ik->size_helper();
      oop result = NULL;
      // If the TLAB isn't pre-zeroed then we'll have to do it
      bool need_zero = !ZeroTLAB;
      if (UseTLAB) {
        result = (oop) THREAD->tlab().allocate(obj_size);
      }
      // Disable non-TLAB-based fast-path, because profiling requires that all
      // allocations go through InterpreterRuntime::_new() if THREAD->tlab().allocate
      // returns NULL.
#ifndef CC_INTERP_PROFILE
      if (result == NULL) {
        need_zero = true;
        // Try allocate in shared eden
      retry:
        HeapWord* compare_to = *Universe::heap()->top_addr();
        HeapWord* new_top = compare_to + obj_size;
        if (new_top <= *Universe::heap()->end_addr()) {
          if (Atomic::cmpxchg(new_top, Universe::heap()->top_addr(), compare_to) != compare_to) {
            goto retry;
          }
          result = (oop) compare_to;
        }
      }
#endif
      if (result != NULL) {
        // Initialize object (if nonzero size and need) and then the header
        if (need_zero ) {
          HeapWord* to_zero = (HeapWord*) result + sizeof(oopDesc) / oopSize;
          obj_size -= sizeof(oopDesc) / oopSize;
          if (obj_size > 0 ) {
            memset(to_zero, 0, obj_size * HeapWordSize);
          }
        }
        if (UseBiasedLocking) {
          result->set_mark(ik->prototype_header());
        } else {
          result->set_mark(markOopDesc::prototype());
        }
        result->set_klass_gap(0);
        result->set_klass(ik);
        // Must prevent reordering of stores for object initialization
        // with stores that publish the new object.
        OrderAccess::storestore();
        SET_STACK_OBJECT(result, 0);
        UPDATE_PC_AND_TOS_AND_CONTINUE(4, 1);
      }
    }
  }
  // Slow case allocation
  CALL_VM(InterpreterRuntime::_new(THREAD, METHOD->constants(), index),
          handle_exception);
  // Must prevent reordering of stores for object initialization
  // with stores that publish the new object.
  OrderAccess::storestore();
  SET_STACK_OBJECT(THREAD->vm_result(), 0);
  THREAD->set_vm_result(NULL);
  UPDATE_PC_AND_TOS_AND_CONTINUE(4, 1);
}
CASE(_anewarray): {
  u2 index = Bytes::get_Java_u2(pc+2);
  jint size = STACK_INT(-1);
  CALL_VM(InterpreterRuntime::anewarray(THREAD, METHOD->constants(), index, size),
          handle_exception);
  // Must prevent reordering of stores for object initialization
  // with stores that publish the new object.
  OrderAccess::storestore();
  SET_STACK_OBJECT(THREAD->vm_result(), -1);
  THREAD->set_vm_result(NULL);
  UPDATE_PC_AND_CONTINUE(4);
}
CASE(_multianewarray): {
  jint dims = *(pc+4);
  jint size = STACK_INT(-1);
  // stack grows down, dimensions are up!
  jint *dimarray =
             (jint*)&topOfStack[dims * Interpreter::stackElementWords+
                                Interpreter::stackElementWords-1];
  //adjust pointer to start of stack element
  CALL_VM(InterpreterRuntime::multianewarray(THREAD, dimarray),
          handle_exception);
  // Must prevent reordering of stores for object initialization
  // with stores that publish the new object.
  OrderAccess::storestore();
  SET_STACK_OBJECT(THREAD->vm_result(), -dims);
  THREAD->set_vm_result(NULL);
  UPDATE_PC_AND_TOS_AND_CONTINUE(5, -(dims-1));
}
CASE(_checkcast):
    if (STACK_OBJECT(-1) != NULL) {
      VERIFY_OOP(STACK_OBJECT(-1));
      u2 index = Bytes::get_Java_u2(pc+2);
      // Constant pool may have actual klass or unresolved klass. If it is
      // unresolved we must resolve it.
      if (METHOD->constants()->tag_at(index).is_unresolved_klass()) {
        CALL_VM(InterpreterRuntime::quicken_io_cc(THREAD), handle_exception);
      }
      Klass* klassOf = (Klass*) METHOD->constants()->resolved_klass_at(index);
      Klass* objKlass = STACK_OBJECT(-1)->klass(); // ebx
      //
      // Check for compatibilty. This check must not GC!!
      // Seems way more expensive now that we must dispatch.
      //
      if (objKlass != klassOf && !objKlass->is_subtype_of(klassOf)) {
        // Decrement counter at checkcast.
        BI_PROFILE_SUBTYPECHECK_FAILED(objKlass);
        ResourceMark rm(THREAD);
        char* message = SharedRuntime::generate_class_cast_message(
          objKlass, klassOf);
        VM_JAVA_ERROR(vmSymbols::java_lang_ClassCastException(), message, note_classCheck_trap);
      }
      // Profile checkcast with null_seen and receiver.
      BI_PROFILE_UPDATE_CHECKCAST(/*null_seen=*/false, objKlass);
    } else {
      // Profile checkcast with null_seen and receiver.
      BI_PROFILE_UPDATE_CHECKCAST(/*null_seen=*/true, NULL);
    }
    UPDATE_PC_AND_CONTINUE(4);

CASE(_instanceof):
    if (STACK_OBJECT(-1) == NULL) {
      SET_STACK_INT(0, -1);
      // Profile instanceof with null_seen and receiver.
      BI_PROFILE_UPDATE_INSTANCEOF(/*null_seen=*/true, NULL);
    } else {
      VERIFY_OOP(STACK_OBJECT(-1));
      u2 index = Bytes::get_Java_u2(pc+2);
      // Constant pool may have actual klass or unresolved klass. If it is
      // unresolved we must resolve it.
      if (METHOD->constants()->tag_at(index).is_unresolved_klass()) {
        CALL_VM(InterpreterRuntime::quicken_io_cc(THREAD), handle_exception);
      }
      Klass* klassOf = (Klass*) METHOD->constants()->resolved_klass_at(index);
      Klass* objKlass = STACK_OBJECT(-1)->klass();
      //
      // Check for compatibilty. This check must not GC!!
      // Seems way more expensive now that we must dispatch.
      //
      if ( objKlass == klassOf || objKlass->is_subtype_of(klassOf)) {
        SET_STACK_INT(1, -1);
      } else {
        SET_STACK_INT(0, -1);
        // Decrement counter at checkcast.
        BI_PROFILE_SUBTYPECHECK_FAILED(objKlass);
      }
      // Profile instanceof with null_seen and receiver.
      BI_PROFILE_UPDATE_INSTANCEOF(/*null_seen=*/false, objKlass);
    }
    UPDATE_PC_AND_CONTINUE(4);

CASE(_ldc_w):
CASE(_ldc):
  {
    u2 index;
    bool wide = false;
    int incr = 3; // frequent case
    if (opcode == Bytecodes::_ldc) {
      index = pc[2];
    } else {
      index = Bytes::get_Java_u2(pc+2);
      incr = 4;
      wide = true;
    }

    ConstantPool* constants = METHOD->constants();
    switch (constants->tag_at(index).value()) {
    case JVM_CONSTANT_Integer:
      SET_STACK_INT(constants->int_at(index), 0);
      break;

    case JVM_CONSTANT_Float:
      SET_STACK_FLOAT(constants->float_at(index), 0);
      break;

    case JVM_CONSTANT_String:
      {
        oop result = constants->resolved_references()->obj_at(index);
        if (result == NULL) {
          CALL_VM(InterpreterRuntime::resolve_ldc(THREAD, (Bytecodes::Code) opcode), handle_exception);
          SET_STACK_OBJECT(THREAD->vm_result(), 0);
          THREAD->set_vm_result(NULL);
        } else {
          VERIFY_OOP(result);
          SET_STACK_OBJECT(result, 0);
        }
      break;
      }

    case JVM_CONSTANT_Class:
      VERIFY_OOP(constants->resolved_klass_at(index)->java_mirror());
      SET_STACK_OBJECT(constants->resolved_klass_at(index)->java_mirror(), 0);
      break;

    case JVM_CONSTANT_UnresolvedClass:
    case JVM_CONSTANT_UnresolvedClassInError:
      CALL_VM(InterpreterRuntime::ldc(THREAD, wide), handle_exception);
      SET_STACK_OBJECT(THREAD->vm_result(), 0);
      THREAD->set_vm_result(NULL);
      break;

    case JVM_CONSTANT_Dynamic:
      {
        oop result = constants->resolved_references()->obj_at(index);
        if (result == NULL) {
          CALL_VM(InterpreterRuntime::resolve_ldc(THREAD, (Bytecodes::Code) opcode), handle_exception);
          result = THREAD->vm_result();
        }
        VERIFY_OOP(result);

        jvalue value;
        BasicType type = java_lang_boxing_object::get_value(result, &value);
        switch (type) {
        case T_FLOAT:   SET_STACK_FLOAT(value.f, 0); break;
        case T_INT:     SET_STACK_INT(value.i, 0); break;
        case T_SHORT:   SET_STACK_INT(value.s, 0); break;
        case T_BYTE:    SET_STACK_INT(value.b, 0); break;
        case T_CHAR:    SET_STACK_INT(value.c, 0); break;
        case T_BOOLEAN: SET_STACK_INT(value.z, 0); break;
        default:  ShouldNotReachHere();
        }

        break;
      }

    default:  ShouldNotReachHere();
    }
    UPDATE_PC_AND_TOS_AND_CONTINUE(incr, 1);
  }

CASE(_ldc2_w):
  {
    u2 index = Bytes::get_Java_u2(pc+2);

    ConstantPool* constants = METHOD->constants();
    switch (constants->tag_at(index).value()) {

    case JVM_CONSTANT_Long:
       SET_STACK_LONG(constants->long_at(index), 1);
      break;

    case JVM_CONSTANT_Double:
       SET_STACK_DOUBLE(constants->double_at(index), 1);
      break;

    case JVM_CONSTANT_Dynamic:
      {
        oop result = constants->resolved_references()->obj_at(index);
        if (result == NULL) {
          CALL_VM(InterpreterRuntime::resolve_ldc(THREAD, (Bytecodes::Code) opcode), handle_exception);
          result = THREAD->vm_result();
        }
        VERIFY_OOP(result);

        jvalue value;
        BasicType type = java_lang_boxing_object::get_value(result, &value);
        switch (type) {
        case T_DOUBLE: SET_STACK_DOUBLE(value.d, 1); break;
        case T_LONG:   SET_STACK_LONG(value.j, 1); break;
        default:  ShouldNotReachHere();
        }

        break;
      }

    default:  ShouldNotReachHere();
    }
    UPDATE_PC_AND_TOS_AND_CONTINUE(4, 2);
  }

CASE(_fast_aldc_w):
CASE(_fast_aldc): {
  u2 index;
  int incr;
  if (opcode == Bytecodes::_fast_aldc) {
    index = pc[2];
    incr = 3;
  } else {
    index = Bytes::get_native_u2(pc+2);
    incr = 4;
  }

  // We are resolved if the resolved_references array contains a non-null object (CallSite, etc.)
  // This kind of CP cache entry does not need to match the flags byte, because
  // there is a 1-1 relation between bytecode type and CP entry type.
  ConstantPool* constants = METHOD->constants();
  oop result = constants->resolved_references()->obj_at(index);
  if (result == NULL) {
    CALL_VM(InterpreterRuntime::resolve_ldc(THREAD, (Bytecodes::Code) opcode),
            handle_exception);
    result = THREAD->vm_result();
  }
  if (oopDesc::equals(result, Universe::the_null_sentinel()))
    result = NULL;

  VERIFY_OOP(result);
  SET_STACK_OBJECT(result, 0);
  UPDATE_PC_AND_TOS_AND_CONTINUE(incr, 1);
}

CASE(_invokedynamic): {

  u4 index = Bytes::get_native_u4(pc+2);
  ConstantPoolCacheEntry* cache = cp->constant_pool()->invokedynamic_cp_cache_entry_at(index);

  // We are resolved if the resolved_references array contains a non-null object (CallSite, etc.)
  // This kind of CP cache entry does not need to match the flags byte, because
  // there is a 1-1 relation between bytecode type and CP entry type.
  if (! cache->is_resolved((Bytecodes::Code) opcode)) {
    CALL_VM(InterpreterRuntime::resolve_from_cache(THREAD, (Bytecodes::Code)opcode),
            handle_exception);
    cache = cp->constant_pool()->invokedynamic_cp_cache_entry_at(index);
  }

  Method* method = cache->f1_as_method();
  if (VerifyOops) method->verify();

  if (cache->has_appendix()) {
    ConstantPool* constants = METHOD->constants();
    SET_STACK_OBJECT(cache->appendix_if_resolved(constants), 0);
    MORE_STACK(1);
  }

  istate->set_msg(call_method);
  istate->set_callee(method);
  istate->set_callee_entry_point(method->from_interpreted_entry());
  istate->set_bcp_advance(6);

  // Invokedynamic has got a call counter, just like an invokestatic -> increment!
  BI_PROFILE_UPDATE_CALL();

  UPDATE_PC_AND_RETURN(0); // I'll be back...
}

CASE(_invokehandle): {

  u2 index = Bytes::get_native_u2(pc+2);
  ConstantPoolCacheEntry* cache = cp->entry_at(index);

  if (! cache->is_resolved((Bytecodes::Code) opcode)) {
    CALL_VM(InterpreterRuntime::resolve_from_cache(THREAD, (Bytecodes::Code)opcode),
            handle_exception);
    cache = cp->entry_at(index);
  }

  Method* method = cache->f1_as_method();
  if (VerifyOops) method->verify();

  if (cache->has_appendix()) {
    ConstantPool* constants = METHOD->constants();
    SET_STACK_OBJECT(cache->appendix_if_resolved(constants), 0);
    MORE_STACK(1);
  }

  istate->set_msg(call_method);
  istate->set_callee(method);
  istate->set_callee_entry_point(method->from_interpreted_entry());
  istate->set_bcp_advance(4);

  // Invokehandle has got a call counter, just like a final call -> increment!
  BI_PROFILE_UPDATE_FINALCALL();

  UPDATE_PC_AND_RETURN(0); // I'll be back...
}

CASE(_invokeinterface): {
  u2 index = Bytes::get_native_u2(pc+2);

  // QQQ Need to make this as inlined as possible. Probably need to split all the bytecode cases
  // out so c++ compiler has a chance for constant prop to fold everything possible away.

  ConstantPoolCacheEntry* cache = cp->entry_at(index);
  if (!cache->is_resolved((Bytecodes::Code)opcode)) {
    CALL_VM(InterpreterRuntime::resolve_from_cache(THREAD, (Bytecodes::Code)opcode),
            handle_exception);
    cache = cp->entry_at(index);
  }

  istate->set_msg(call_method);

  // Special case of invokeinterface called for virtual method of
  // java.lang.Object.  See cpCache.cpp for details.
  Method* callee = NULL;
  if (cache->is_forced_virtual()) {
    CHECK_NULL(STACK_OBJECT(-(cache->parameter_size())));
    if (cache->is_vfinal()) {
      callee = cache->f2_as_vfinal_method();
      // Profile 'special case of invokeinterface' final call.
      BI_PROFILE_UPDATE_FINALCALL();
    } else {
      // Get receiver.
      int parms = cache->parameter_size();
      // Same comments as invokevirtual apply here.
      oop rcvr = STACK_OBJECT(-parms);
      VERIFY_OOP(rcvr);
      Klass* rcvrKlass = rcvr->klass();
      callee = (Method*) rcvrKlass->method_at_vtable(cache->f2_as_index());
      // Profile 'special case of invokeinterface' virtual call.
      BI_PROFILE_UPDATE_VIRTUALCALL(rcvrKlass);
    }
  } else if (cache->is_vfinal()) {
    // private interface method invocations
    //
    // Ensure receiver class actually implements
    // the resolved interface class. The link resolver
    // does this, but only for the first time this
    // interface is being called.
    int parms = cache->parameter_size();
    oop rcvr = STACK_OBJECT(-parms);
    CHECK_NULL(rcvr);
    Klass* recv_klass = rcvr->klass();
    Klass* resolved_klass = cache->f1_as_klass();
    if (!recv_klass->is_subtype_of(resolved_klass)) {
      ResourceMark rm(THREAD);
      char buf[200];
      jio_snprintf(buf, sizeof(buf), "Class %s does not implement the requested interface %s",
        recv_klass->external_name(),
        resolved_klass->external_name());
      VM_JAVA_ERROR(vmSymbols::java_lang_IncompatibleClassChangeError(), buf, note_no_trap);
    }
    callee = cache->f2_as_vfinal_method();
  }
  if (callee != NULL) {
    istate->set_callee(callee);
    istate->set_callee_entry_point(callee->from_interpreted_entry());
#ifdef VM_JVMTI
    if (JvmtiExport::can_post_interpreter_events() && THREAD->is_interp_only_mode()) {
      istate->set_callee_entry_point(callee->interpreter_entry());
    }
#endif /* VM_JVMTI */
    istate->set_bcp_advance(6);
    UPDATE_PC_AND_RETURN(0); // I'll be back...
  }

  // this could definitely be cleaned up QQQ
  Method *interface_method = cache->f2_as_interface_method();
  InstanceKlass* iclass = interface_method->method_holder();

  // get receiver
  int parms = cache->parameter_size();
  oop rcvr = STACK_OBJECT(-parms);
  CHECK_NULL(rcvr);
  InstanceKlass* int2 = (InstanceKlass*) rcvr->klass();

  // Receiver subtype check against resolved interface klass (REFC).
  {
    Klass* refc = cache->f1_as_klass();
    itableOffsetEntry* scan;
    for (scan = (itableOffsetEntry*) int2->start_of_itable();
         scan->interface_klass() != NULL;
         scan++) {
      if (scan->interface_klass() == refc) {
        break;
      }
    }
    // Check that the entry is non-null.  A null entry means
    // that the receiver class doesn't implement the
    // interface, and wasn't the same as when the caller was
    // compiled.
    if (scan->interface_klass() == NULL) {
      VM_JAVA_ERROR(vmSymbols::java_lang_IncompatibleClassChangeError(), "", note_no_trap);
    }
  }

  itableOffsetEntry* ki = (itableOffsetEntry*) int2->start_of_itable();
  int i;
  for ( i = 0 ; i < int2->itable_length() ; i++, ki++ ) {
    if (ki->interface_klass() == iclass) break;
  }
  // If the interface isn't found, this class doesn't implement this
  // interface. The link resolver checks this but only for the first
  // time this interface is called.
  if (i == int2->itable_length()) {
    CALL_VM(InterpreterRuntime::throw_IncompatibleClassChangeErrorVerbose(THREAD, rcvr->klass(), iclass),
            handle_exception);
  }
  int mindex = interface_method->itable_index();

  itableMethodEntry* im = ki->first_method_entry(rcvr->klass());
  callee = im[mindex].method();
  if (callee == NULL) {
    CALL_VM(InterpreterRuntime::throw_AbstractMethodErrorVerbose(THREAD, rcvr->klass(), interface_method),
            handle_exception);
  }

  // Profile virtual call.
  BI_PROFILE_UPDATE_VIRTUALCALL(rcvr->klass());

  istate->set_callee(callee);
  istate->set_callee_entry_point(callee->from_interpreted_entry());
#ifdef VM_JVMTI
  if (JvmtiExport::can_post_interpreter_events() && THREAD->is_interp_only_mode()) {
    istate->set_callee_entry_point(callee->interpreter_entry());
  }
#endif /* VM_JVMTI */
  istate->set_bcp_advance(6);
  UPDATE_PC_AND_RETURN(0); // I'll be back...
}

CASE(_invokevirtual):
CASE(_invokespecial):
CASE(_invokestatic): {
  u2 index = Bytes::get_native_u2(pc+2);

  ConstantPoolCacheEntry* cache = cp->entry_at(index);
  // QQQ Need to make this as inlined as possible. Probably need to split all the bytecode cases
  // out so c++ compiler has a chance for constant prop to fold everything possible away.

  if (!cache->is_resolved((Bytecodes::Code)opcode)) {
    CALL_VM(InterpreterRuntime::resolve_from_cache(THREAD, (Bytecodes::Code)opcode),
            handle_exception);
    cache = cp->entry_at(index);
  }

  istate->set_msg(call_method);
  {
    Method* callee;
    if ((Bytecodes::Code)opcode == Bytecodes::_invokevirtual) {
      CHECK_NULL(STACK_OBJECT(-(cache->parameter_size())));
      if (cache->is_vfinal()) {
        callee = cache->f2_as_vfinal_method();
        // Profile final call.
        BI_PROFILE_UPDATE_FINALCALL();
      } else {
        // get receiver
        int parms = cache->parameter_size();
        // this works but needs a resourcemark and seems to create a vtable on every call:
        // Method* callee = rcvr->klass()->vtable()->method_at(cache->f2_as_index());
        //
        // this fails with an assert
        // InstanceKlass* rcvrKlass = InstanceKlass::cast(STACK_OBJECT(-parms)->klass());
        // but this works
        oop rcvr = STACK_OBJECT(-parms);
        VERIFY_OOP(rcvr);
        Klass* rcvrKlass = rcvr->klass();
        /*
          Executing this code in java.lang.String:
              public String(char value[]) {
                    this.count = value.length;
                    this.value = (char[])value.clone();
               }

           a find on rcvr->klass() reports:
           {type array char}{type array class}
            - klass: {other class}

            but using InstanceKlass::cast(STACK_OBJECT(-parms)->klass()) causes in assertion failure
            because rcvr->klass()->is_instance_klass() == 0
            However it seems to have a vtable in the right location. Huh?
            Because vtables have the same offset for ArrayKlass and InstanceKlass.
        */
        callee = (Method*) rcvrKlass->method_at_vtable(cache->f2_as_index());
        // Profile virtual call.
        BI_PROFILE_UPDATE_VIRTUALCALL(rcvrKlass);
      }
    } else {
      if ((Bytecodes::Code)opcode == Bytecodes::_invokespecial) {
        CHECK_NULL(STACK_OBJECT(-(cache->parameter_size())));
      }
      callee = cache->f1_as_method();

      // Profile call.
      BI_PROFILE_UPDATE_CALL();
    }

    istate->set_callee(callee);
    istate->set_callee_entry_point(callee->from_interpreted_entry());
#ifdef VM_JVMTI
    if (JvmtiExport::can_post_interpreter_events() && THREAD->is_interp_only_mode()) {
      istate->set_callee_entry_point(callee->interpreter_entry());
    }
#endif /* VM_JVMTI */
    istate->set_bcp_advance(4);
    UPDATE_PC_AND_RETURN(0); // I'll be back...
  }
}

/* Allocate memory for a new java object. */

CASE(_newarray): {
  BasicType atype = (BasicType) *(pc+2);
  jint size = STACK_INT(-1);
  CALL_VM(InterpreterRuntime::newarray(THREAD, atype, size),
          handle_exception);
  // Must prevent reordering of stores for object initialization
  // with stores that publish the new object.
  OrderAccess::storestore();
  SET_STACK_OBJECT(THREAD->vm_result(), -1);
  THREAD->set_vm_result(NULL);

  UPDATE_PC_AND_CONTINUE(3);
}

/* Throw an exception. */

CASE(_athrow): {
    oop except_oop = STACK_OBJECT(-1);
    CHECK_NULL(except_oop);
    // set pending_exception so we use common code
    THREAD->set_pending_exception(except_oop, NULL, 0);
    goto handle_exception;
}

/* goto and jsr. They are exactly the same except jsr pushes
 * the address of the next instruction first.
 */

CASE(_jsr): {
    /* push bytecode index on stack */
    SET_STACK_ADDR(((address)pc - (intptr_t)(istate->method()->code_base()) + 4), 0);
    MORE_STACK(1);
    /* FALL THROUGH */
}

CASE(_goto):
{
    int16_t offset = (int16_t)Bytes::get_Java_u2(pc + 2);
    // Profile jump.
    BI_PROFILE_UPDATE_JUMP();
    address branch_pc = pc;
    UPDATE_PC(offset);
    DO_BACKEDGE_CHECKS(offset, branch_pc);
    CONTINUE;
}

CASE(_jsr_w): {
    /* push return address on the stack */
    SET_STACK_ADDR(((address)pc - (intptr_t)(istate->method()->code_base()) + 6), 0);
    MORE_STACK(1);
    /* FALL THROUGH */
}

CASE(_goto_w):
{
    int32_t offset = Bytes::get_Java_u4(pc + 2);
    // Profile jump.
    BI_PROFILE_UPDATE_JUMP();
    address branch_pc = pc;
    UPDATE_PC(offset);
    DO_BACKEDGE_CHECKS(offset, branch_pc);
    CONTINUE;
}

/* return from a jsr or jsr_w */

CASE(_ret): {
    // Profile ret.
    BI_PROFILE_UPDATE_RET(/*bci=*/((int)(intptr_t)(LOCALS_ADDR(pc[2]))));
    // Now, update the pc.
    pc = istate->method()->code_base() + (intptr_t)(LOCALS_ADDR(pc[2]));
    UPDATE_PC_AND_CONTINUE(0);
}

/* debugger breakpoint */

CASE(_breakpoint): {
    Bytecodes::Code original_bytecode;
    DECACHE_STATE();
    SET_LAST_JAVA_FRAME();
    original_bytecode = InterpreterRuntime::get_original_bytecode_at(THREAD,
                        METHOD, pc);
    RESET_LAST_JAVA_FRAME();
    CACHE_STATE();
    if (THREAD->has_pending_exception()) goto handle_exception;
      CALL_VM(InterpreterRuntime::_breakpoint(THREAD, METHOD, pc),
                                              handle_exception);

    opcode = (jubyte)original_bytecode;
    goto opcode_switch;
}
