/* GENERATED HANDLER FILE -- DO NOT EDIT (2019-11-13T05:12:07.226552Z) */
#include "bytecodeInterpreter.base.hpp"

CASE(_nop): { // nop
  { INSTR_nop(pc+0,0); }
  { INSTR_end_sequence(pc+2,0); }
  puts("Overrun of nop handler");
}
CASE(_aconst_null): { // aconst_null
  { INSTR_aconst_null(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of aconst_null handler");
}
CASE(_iconst_m1): { // iconst_m1
  { INSTR_iconst_m1(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of iconst_m1 handler");
}
CASE(_iconst_0): { // iconst_0
  { INSTR_iconst_0(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of iconst_0 handler");
}
CASE(_iconst_1): { // iconst_1
  { INSTR_iconst_1(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of iconst_1 handler");
}
CASE(_iconst_2): { // iconst_2
  { INSTR_iconst_2(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of iconst_2 handler");
}
CASE(_iconst_3): { // iconst_3
  { INSTR_iconst_3(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of iconst_3 handler");
}
CASE(_iconst_4): { // iconst_4
  { INSTR_iconst_4(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of iconst_4 handler");
}
CASE(_iconst_5): { // iconst_5
  { INSTR_iconst_5(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of iconst_5 handler");
}
CASE(_lconst_0): { // lconst_0
  { INSTR_lconst_0(pc+0,0); }
  { INSTR_end_sequence(pc+2,2); }
  puts("Overrun of lconst_0 handler");
}
CASE(_lconst_1): { // lconst_1
  { INSTR_lconst_1(pc+0,0); }
  { INSTR_end_sequence(pc+2,2); }
  puts("Overrun of lconst_1 handler");
}
CASE(_fconst_0): { // fconst_0
  { INSTR_fconst_0(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of fconst_0 handler");
}
CASE(_fconst_1): { // fconst_1
  { INSTR_fconst_1(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of fconst_1 handler");
}
CASE(_fconst_2): { // fconst_2
  { INSTR_fconst_2(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of fconst_2 handler");
}
CASE(_dconst_0): { // dconst_0
  { INSTR_dconst_0(pc+0,0); }
  { INSTR_end_sequence(pc+2,2); }
  puts("Overrun of dconst_0 handler");
}
CASE(_dconst_1): { // dconst_1
  { INSTR_dconst_1(pc+0,0); }
  { INSTR_end_sequence(pc+2,2); }
  puts("Overrun of dconst_1 handler");
}
CASE(_bipush): { // bipush
  { INSTR_bipush(pc+0,0); }
  { INSTR_end_sequence(pc+3,1); }
  puts("Overrun of bipush handler");
}
CASE(_sipush): { // sipush
  { INSTR_sipush(pc+0,0); }
  { INSTR_end_sequence(pc+4,1); }
  puts("Overrun of sipush handler");
}
CASE(_ldc): { // ldc
  { INSTR_ldc(pc+0,0); }
  { INSTR_end_sequence(pc+3,1); }
  puts("Overrun of ldc handler");
}
CASE(_ldc_w): { // ldc_w
  { INSTR_ldc_w(pc+0,0); }
  { INSTR_end_sequence(pc+4,1); }
  puts("Overrun of ldc_w handler");
}
CASE(_ldc2_w): { // ldc2_w
  { INSTR_ldc2_w(pc+0,0); }
  { INSTR_end_sequence(pc+4,2); }
  puts("Overrun of ldc2_w handler");
}
CASE(_iload): { // iload
  { INSTR_iload(pc+0,0); }
  { INSTR_end_sequence(pc+3,1); }
  puts("Overrun of iload handler");
}
CASE(_lload): { // lload
  { INSTR_lload(pc+0,0); }
  { INSTR_end_sequence(pc+3,2); }
  puts("Overrun of lload handler");
}
CASE(_fload): { // fload
  { INSTR_fload(pc+0,0); }
  { INSTR_end_sequence(pc+3,1); }
  puts("Overrun of fload handler");
}
CASE(_dload): { // dload
  { INSTR_dload(pc+0,0); }
  { INSTR_end_sequence(pc+3,2); }
  puts("Overrun of dload handler");
}
CASE(_aload): { // aload
  { INSTR_aload(pc+0,0); }
  { INSTR_end_sequence(pc+3,1); }
  puts("Overrun of aload handler");
}
CASE(_iload_0): { // iload_0
  { INSTR_iload_0(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of iload_0 handler");
}
CASE(_iload_1): { // iload_1
  { INSTR_iload_1(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of iload_1 handler");
}
CASE(_iload_2): { // iload_2
  { INSTR_iload_2(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of iload_2 handler");
}
CASE(_iload_3): { // iload_3
  { INSTR_iload_3(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of iload_3 handler");
}
CASE(_lload_0): { // lload_0
  { INSTR_lload_0(pc+0,0); }
  { INSTR_end_sequence(pc+2,2); }
  puts("Overrun of lload_0 handler");
}
CASE(_lload_1): { // lload_1
  { INSTR_lload_1(pc+0,0); }
  { INSTR_end_sequence(pc+2,2); }
  puts("Overrun of lload_1 handler");
}
CASE(_lload_2): { // lload_2
  { INSTR_lload_2(pc+0,0); }
  { INSTR_end_sequence(pc+2,2); }
  puts("Overrun of lload_2 handler");
}
CASE(_lload_3): { // lload_3
  { INSTR_lload_3(pc+0,0); }
  { INSTR_end_sequence(pc+2,2); }
  puts("Overrun of lload_3 handler");
}
CASE(_fload_0): { // fload_0
  { INSTR_fload_0(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of fload_0 handler");
}
CASE(_fload_1): { // fload_1
  { INSTR_fload_1(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of fload_1 handler");
}
CASE(_fload_2): { // fload_2
  { INSTR_fload_2(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of fload_2 handler");
}
CASE(_fload_3): { // fload_3
  { INSTR_fload_3(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of fload_3 handler");
}
CASE(_dload_0): { // dload_0
  { INSTR_dload_0(pc+0,0); }
  { INSTR_end_sequence(pc+2,2); }
  puts("Overrun of dload_0 handler");
}
CASE(_dload_1): { // dload_1
  { INSTR_dload_1(pc+0,0); }
  { INSTR_end_sequence(pc+2,2); }
  puts("Overrun of dload_1 handler");
}
CASE(_dload_2): { // dload_2
  { INSTR_dload_2(pc+0,0); }
  { INSTR_end_sequence(pc+2,2); }
  puts("Overrun of dload_2 handler");
}
CASE(_dload_3): { // dload_3
  { INSTR_dload_3(pc+0,0); }
  { INSTR_end_sequence(pc+2,2); }
  puts("Overrun of dload_3 handler");
}
CASE(_aload_0): { // aload_0
  { INSTR_aload_0(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of aload_0 handler");
}
CASE(_aload_1): { // aload_1
  { INSTR_aload_1(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of aload_1 handler");
}
CASE(_aload_2): { // aload_2
  { INSTR_aload_2(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of aload_2 handler");
}
CASE(_aload_3): { // aload_3
  { INSTR_aload_3(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of aload_3 handler");
}
CASE(_iaload): { // iaload
  { INSTR_iaload(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of iaload handler");
}
CASE(_laload): { // laload
  { INSTR_laload(pc+0,0); }
  { INSTR_end_sequence(pc+2,0); }
  puts("Overrun of laload handler");
}
CASE(_faload): { // faload
  { INSTR_faload(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of faload handler");
}
CASE(_daload): { // daload
  { INSTR_daload(pc+0,0); }
  { INSTR_end_sequence(pc+2,0); }
  puts("Overrun of daload handler");
}
CASE(_aaload): { // aaload
  { INSTR_aaload(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of aaload handler");
}
CASE(_baload): { // baload
  { INSTR_baload(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of baload handler");
}
CASE(_caload): { // caload
  { INSTR_caload(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of caload handler");
}
CASE(_saload): { // saload
  { INSTR_saload(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of saload handler");
}
CASE(_istore): { // istore
  { INSTR_istore(pc+0,0); }
  { INSTR_end_sequence(pc+3,-1); }
  puts("Overrun of istore handler");
}
CASE(_lstore): { // lstore
  { INSTR_lstore(pc+0,0); }
  { INSTR_end_sequence(pc+3,-2); }
  puts("Overrun of lstore handler");
}
CASE(_fstore): { // fstore
  { INSTR_fstore(pc+0,0); }
  { INSTR_end_sequence(pc+3,-1); }
  puts("Overrun of fstore handler");
}
CASE(_dstore): { // dstore
  { INSTR_dstore(pc+0,0); }
  { INSTR_end_sequence(pc+3,-2); }
  puts("Overrun of dstore handler");
}
CASE(_astore): { // astore
  { INSTR_astore(pc+0,0); }
  { INSTR_end_sequence(pc+3,-1); }
  puts("Overrun of astore handler");
}
CASE(_istore_0): { // istore_0
  { INSTR_istore_0(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of istore_0 handler");
}
CASE(_istore_1): { // istore_1
  { INSTR_istore_1(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of istore_1 handler");
}
CASE(_istore_2): { // istore_2
  { INSTR_istore_2(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of istore_2 handler");
}
CASE(_istore_3): { // istore_3
  { INSTR_istore_3(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of istore_3 handler");
}
CASE(_lstore_0): { // lstore_0
  { INSTR_lstore_0(pc+0,0); }
  { INSTR_end_sequence(pc+2,-2); }
  puts("Overrun of lstore_0 handler");
}
CASE(_lstore_1): { // lstore_1
  { INSTR_lstore_1(pc+0,0); }
  { INSTR_end_sequence(pc+2,-2); }
  puts("Overrun of lstore_1 handler");
}
CASE(_lstore_2): { // lstore_2
  { INSTR_lstore_2(pc+0,0); }
  { INSTR_end_sequence(pc+2,-2); }
  puts("Overrun of lstore_2 handler");
}
CASE(_lstore_3): { // lstore_3
  { INSTR_lstore_3(pc+0,0); }
  { INSTR_end_sequence(pc+2,-2); }
  puts("Overrun of lstore_3 handler");
}
CASE(_fstore_0): { // fstore_0
  { INSTR_fstore_0(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of fstore_0 handler");
}
CASE(_fstore_1): { // fstore_1
  { INSTR_fstore_1(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of fstore_1 handler");
}
CASE(_fstore_2): { // fstore_2
  { INSTR_fstore_2(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of fstore_2 handler");
}
CASE(_fstore_3): { // fstore_3
  { INSTR_fstore_3(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of fstore_3 handler");
}
CASE(_dstore_0): { // dstore_0
  { INSTR_dstore_0(pc+0,0); }
  { INSTR_end_sequence(pc+2,-2); }
  puts("Overrun of dstore_0 handler");
}
CASE(_dstore_1): { // dstore_1
  { INSTR_dstore_1(pc+0,0); }
  { INSTR_end_sequence(pc+2,-2); }
  puts("Overrun of dstore_1 handler");
}
CASE(_dstore_2): { // dstore_2
  { INSTR_dstore_2(pc+0,0); }
  { INSTR_end_sequence(pc+2,-2); }
  puts("Overrun of dstore_2 handler");
}
CASE(_dstore_3): { // dstore_3
  { INSTR_dstore_3(pc+0,0); }
  { INSTR_end_sequence(pc+2,-2); }
  puts("Overrun of dstore_3 handler");
}
CASE(_astore_0): { // astore_0
  { INSTR_astore_0(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of astore_0 handler");
}
CASE(_astore_1): { // astore_1
  { INSTR_astore_1(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of astore_1 handler");
}
CASE(_astore_2): { // astore_2
  { INSTR_astore_2(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of astore_2 handler");
}
CASE(_astore_3): { // astore_3
  { INSTR_astore_3(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of astore_3 handler");
}
CASE(_iastore): { // iastore
  { INSTR_iastore(pc+0,0); }
  { INSTR_end_sequence(pc+2,-3); }
  puts("Overrun of iastore handler");
}
CASE(_lastore): { // lastore
  { INSTR_lastore(pc+0,0); }
  { INSTR_end_sequence(pc+2,-4); }
  puts("Overrun of lastore handler");
}
CASE(_fastore): { // fastore
  { INSTR_fastore(pc+0,0); }
  { INSTR_end_sequence(pc+2,-3); }
  puts("Overrun of fastore handler");
}
CASE(_dastore): { // dastore
  { INSTR_dastore(pc+0,0); }
  { INSTR_end_sequence(pc+2,-4); }
  puts("Overrun of dastore handler");
}
CASE(_aastore): { // aastore
  { INSTR_aastore(pc+0,0); }
  { INSTR_end_sequence(pc+2,-3); }
  puts("Overrun of aastore handler");
}
CASE(_bastore): { // bastore
  { INSTR_bastore(pc+0,0); }
  { INSTR_end_sequence(pc+2,-3); }
  puts("Overrun of bastore handler");
}
CASE(_castore): { // castore
  { INSTR_castore(pc+0,0); }
  { INSTR_end_sequence(pc+2,-3); }
  puts("Overrun of castore handler");
}
CASE(_sastore): { // sastore
  { INSTR_sastore(pc+0,0); }
  { INSTR_end_sequence(pc+2,-3); }
  puts("Overrun of sastore handler");
}
CASE(_pop): { // pop
  { INSTR_pop(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of pop handler");
}
CASE(_pop2): { // pop2
  { INSTR_pop2(pc+0,0); }
  { INSTR_end_sequence(pc+2,-2); }
  puts("Overrun of pop2 handler");
}
CASE(_dup): { // dup
  { INSTR_dup(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of dup handler");
}
CASE(_dup_x1): { // dup_x1
  { INSTR_dup_x1(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of dup_x1 handler");
}
CASE(_dup_x2): { // dup_x2
  { INSTR_dup_x2(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of dup_x2 handler");
}
CASE(_dup2): { // dup2
  { INSTR_dup2(pc+0,0); }
  { INSTR_end_sequence(pc+2,2); }
  puts("Overrun of dup2 handler");
}
CASE(_dup2_x1): { // dup2_x1
  { INSTR_dup2_x1(pc+0,0); }
  { INSTR_end_sequence(pc+2,2); }
  puts("Overrun of dup2_x1 handler");
}
CASE(_dup2_x2): { // dup2_x2
  { INSTR_dup2_x2(pc+0,0); }
  { INSTR_end_sequence(pc+2,2); }
  puts("Overrun of dup2_x2 handler");
}
CASE(_swap): { // swap
  { INSTR_swap(pc+0,0); }
  { INSTR_end_sequence(pc+2,0); }
  puts("Overrun of swap handler");
}
CASE(_iadd): { // iadd
  { INSTR_iadd(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of iadd handler");
}
CASE(_ladd): { // ladd
  { INSTR_ladd(pc+0,0); }
  { INSTR_end_sequence(pc+2,-2); }
  puts("Overrun of ladd handler");
}
CASE(_fadd): { // fadd
  { INSTR_fadd(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of fadd handler");
}
CASE(_dadd): { // dadd
  { INSTR_dadd(pc+0,0); }
  { INSTR_end_sequence(pc+2,-2); }
  puts("Overrun of dadd handler");
}
CASE(_isub): { // isub
  { INSTR_isub(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of isub handler");
}
CASE(_lsub): { // lsub
  { INSTR_lsub(pc+0,0); }
  { INSTR_end_sequence(pc+2,-2); }
  puts("Overrun of lsub handler");
}
CASE(_fsub): { // fsub
  { INSTR_fsub(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of fsub handler");
}
CASE(_dsub): { // dsub
  { INSTR_dsub(pc+0,0); }
  { INSTR_end_sequence(pc+2,-2); }
  puts("Overrun of dsub handler");
}
CASE(_imul): { // imul
  { INSTR_imul(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of imul handler");
}
CASE(_lmul): { // lmul
  { INSTR_lmul(pc+0,0); }
  { INSTR_end_sequence(pc+2,-2); }
  puts("Overrun of lmul handler");
}
CASE(_fmul): { // fmul
  { INSTR_fmul(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of fmul handler");
}
CASE(_dmul): { // dmul
  { INSTR_dmul(pc+0,0); }
  { INSTR_end_sequence(pc+2,-2); }
  puts("Overrun of dmul handler");
}
CASE(_idiv): { // idiv
  { INSTR_idiv(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of idiv handler");
}
CASE(_ldiv): { // ldiv
  { INSTR_ldiv(pc+0,0); }
  { INSTR_end_sequence(pc+2,-2); }
  puts("Overrun of ldiv handler");
}
CASE(_fdiv): { // fdiv
  { INSTR_fdiv(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of fdiv handler");
}
CASE(_ddiv): { // ddiv
  { INSTR_ddiv(pc+0,0); }
  { INSTR_end_sequence(pc+2,-2); }
  puts("Overrun of ddiv handler");
}
CASE(_irem): { // irem
  { INSTR_irem(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of irem handler");
}
CASE(_lrem): { // lrem
  { INSTR_lrem(pc+0,0); }
  { INSTR_end_sequence(pc+2,-2); }
  puts("Overrun of lrem handler");
}
CASE(_frem): { // frem
  { INSTR_frem(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of frem handler");
}
CASE(_drem): { // drem
  { INSTR_drem(pc+0,0); }
  { INSTR_end_sequence(pc+2,-2); }
  puts("Overrun of drem handler");
}
CASE(_ineg): { // ineg
  { INSTR_ineg(pc+0,0); }
  { INSTR_end_sequence(pc+2,0); }
  puts("Overrun of ineg handler");
}
CASE(_lneg): { // lneg
  { INSTR_lneg(pc+0,0); }
  { INSTR_end_sequence(pc+2,0); }
  puts("Overrun of lneg handler");
}
CASE(_fneg): { // fneg
  { INSTR_fneg(pc+0,0); }
  { INSTR_end_sequence(pc+2,0); }
  puts("Overrun of fneg handler");
}
CASE(_dneg): { // dneg
  { INSTR_dneg(pc+0,0); }
  { INSTR_end_sequence(pc+2,0); }
  puts("Overrun of dneg handler");
}
CASE(_ishl): { // ishl
  { INSTR_ishl(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of ishl handler");
}
CASE(_lshl): { // lshl
  { INSTR_lshl(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of lshl handler");
}
CASE(_ishr): { // ishr
  { INSTR_ishr(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of ishr handler");
}
CASE(_lshr): { // lshr
  { INSTR_lshr(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of lshr handler");
}
CASE(_iushr): { // iushr
  { INSTR_iushr(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of iushr handler");
}
CASE(_lushr): { // lushr
  { INSTR_lushr(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of lushr handler");
}
CASE(_iand): { // iand
  { INSTR_iand(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of iand handler");
}
CASE(_land): { // land
  { INSTR_land(pc+0,0); }
  { INSTR_end_sequence(pc+2,-2); }
  puts("Overrun of land handler");
}
CASE(_ior): { // ior
  { INSTR_ior(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of ior handler");
}
CASE(_lor): { // lor
  { INSTR_lor(pc+0,0); }
  { INSTR_end_sequence(pc+2,-2); }
  puts("Overrun of lor handler");
}
CASE(_ixor): { // ixor
  { INSTR_ixor(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of ixor handler");
}
CASE(_lxor): { // lxor
  { INSTR_lxor(pc+0,0); }
  { INSTR_end_sequence(pc+2,-2); }
  puts("Overrun of lxor handler");
}
CASE(_iinc): { // iinc
  { INSTR_iinc(pc+0,0); }
  { INSTR_end_sequence(pc+4,0); }
  puts("Overrun of iinc handler");
}
CASE(_i2l): { // i2l
  { INSTR_i2l(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of i2l handler");
}
CASE(_i2f): { // i2f
  { INSTR_i2f(pc+0,0); }
  { INSTR_end_sequence(pc+2,0); }
  puts("Overrun of i2f handler");
}
CASE(_i2d): { // i2d
  { INSTR_i2d(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of i2d handler");
}
CASE(_l2i): { // l2i
  { INSTR_l2i(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of l2i handler");
}
CASE(_l2f): { // l2f
  { INSTR_l2f(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of l2f handler");
}
CASE(_l2d): { // l2d
  { INSTR_l2d(pc+0,0); }
  { INSTR_end_sequence(pc+2,0); }
  puts("Overrun of l2d handler");
}
CASE(_f2i): { // f2i
  { INSTR_f2i(pc+0,0); }
  { INSTR_end_sequence(pc+2,0); }
  puts("Overrun of f2i handler");
}
CASE(_f2l): { // f2l
  { INSTR_f2l(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of f2l handler");
}
CASE(_f2d): { // f2d
  { INSTR_f2d(pc+0,0); }
  { INSTR_end_sequence(pc+2,1); }
  puts("Overrun of f2d handler");
}
CASE(_d2i): { // d2i
  { INSTR_d2i(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of d2i handler");
}
CASE(_d2l): { // d2l
  { INSTR_d2l(pc+0,0); }
  { INSTR_end_sequence(pc+2,0); }
  puts("Overrun of d2l handler");
}
CASE(_d2f): { // d2f
  { INSTR_d2f(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of d2f handler");
}
CASE(_i2b): { // i2b
  { INSTR_i2b(pc+0,0); }
  { INSTR_end_sequence(pc+2,0); }
  puts("Overrun of i2b handler");
}
CASE(_i2c): { // i2c
  { INSTR_i2c(pc+0,0); }
  { INSTR_end_sequence(pc+2,0); }
  puts("Overrun of i2c handler");
}
CASE(_i2s): { // i2s
  { INSTR_i2s(pc+0,0); }
  { INSTR_end_sequence(pc+2,0); }
  puts("Overrun of i2s handler");
}
CASE(_lcmp): { // lcmp
  { INSTR_lcmp(pc+0,0); }
  { INSTR_end_sequence(pc+2,-3); }
  puts("Overrun of lcmp handler");
}
CASE(_fcmpl): { // fcmpl
  { INSTR_fcmpl(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of fcmpl handler");
}
CASE(_fcmpg): { // fcmpg
  { INSTR_fcmpg(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of fcmpg handler");
}
CASE(_dcmpl): { // dcmpl
  { INSTR_dcmpl(pc+0,0); }
  { INSTR_end_sequence(pc+2,-3); }
  puts("Overrun of dcmpl handler");
}
CASE(_dcmpg): { // dcmpg
  { INSTR_dcmpg(pc+0,0); }
  { INSTR_end_sequence(pc+2,-3); }
  puts("Overrun of dcmpg handler");
}
CASE(_ifeq): { // ifeq
  { INSTR_ifeq(pc+0,0); }
  { INSTR_end_sequence(pc+4,-1); }
  puts("Overrun of ifeq handler");
}
CASE(_ifne): { // ifne
  { INSTR_ifne(pc+0,0); }
  { INSTR_end_sequence(pc+4,-1); }
  puts("Overrun of ifne handler");
}
CASE(_iflt): { // iflt
  { INSTR_iflt(pc+0,0); }
  { INSTR_end_sequence(pc+4,-1); }
  puts("Overrun of iflt handler");
}
CASE(_ifge): { // ifge
  { INSTR_ifge(pc+0,0); }
  { INSTR_end_sequence(pc+4,-1); }
  puts("Overrun of ifge handler");
}
CASE(_ifgt): { // ifgt
  { INSTR_ifgt(pc+0,0); }
  { INSTR_end_sequence(pc+4,-1); }
  puts("Overrun of ifgt handler");
}
CASE(_ifle): { // ifle
  { INSTR_ifle(pc+0,0); }
  { INSTR_end_sequence(pc+4,-1); }
  puts("Overrun of ifle handler");
}
CASE(_if_icmpeq): { // if_icmpeq
  { INSTR_if_icmpeq(pc+0,0); }
  { INSTR_end_sequence(pc+4,-2); }
  puts("Overrun of if_icmpeq handler");
}
CASE(_if_icmpne): { // if_icmpne
  { INSTR_if_icmpne(pc+0,0); }
  { INSTR_end_sequence(pc+4,-2); }
  puts("Overrun of if_icmpne handler");
}
CASE(_if_icmplt): { // if_icmplt
  { INSTR_if_icmplt(pc+0,0); }
  { INSTR_end_sequence(pc+4,-2); }
  puts("Overrun of if_icmplt handler");
}
CASE(_if_icmpge): { // if_icmpge
  { INSTR_if_icmpge(pc+0,0); }
  { INSTR_end_sequence(pc+4,-2); }
  puts("Overrun of if_icmpge handler");
}
CASE(_if_icmpgt): { // if_icmpgt
  { INSTR_if_icmpgt(pc+0,0); }
  { INSTR_end_sequence(pc+4,-2); }
  puts("Overrun of if_icmpgt handler");
}
CASE(_if_icmple): { // if_icmple
  { INSTR_if_icmple(pc+0,0); }
  { INSTR_end_sequence(pc+4,-2); }
  puts("Overrun of if_icmple handler");
}
CASE(_if_acmpeq): { // if_acmpeq
  { INSTR_if_acmpeq(pc+0,0); }
  { INSTR_end_sequence(pc+4,-2); }
  puts("Overrun of if_acmpeq handler");
}
CASE(_if_acmpne): { // if_acmpne
  { INSTR_if_acmpne(pc+0,0); }
  { INSTR_end_sequence(pc+4,-2); }
  puts("Overrun of if_acmpne handler");
}
CASE(_goto): { // goto
  { INSTR_goto(pc+0,0); }
  puts("Overrun of goto handler");
}
CASE(_jsr): { // jsr
  { INSTR_jsr(pc+0,0); }
  puts("Overrun of jsr handler");
}
CASE(_ret): { // ret
  { INSTR_ret(pc+0,0); }
  puts("Overrun of ret handler");
}
CASE(_tableswitch): { // tableswitch
  { INSTR_tableswitch(pc+0,0); }
  puts("Overrun of tableswitch handler");
}
CASE(_lookupswitch): { // lookupswitch
  { INSTR_lookupswitch(pc+0,0); }
  puts("Overrun of lookupswitch handler");
}
CASE(_ireturn): { // ireturn
  { INSTR_ireturn(pc+0,0); }
  puts("Overrun of ireturn handler");
}
CASE(_lreturn): { // lreturn
  { INSTR_lreturn(pc+0,0); }
  puts("Overrun of lreturn handler");
}
CASE(_freturn): { // freturn
  { INSTR_freturn(pc+0,0); }
  puts("Overrun of freturn handler");
}
CASE(_dreturn): { // dreturn
  { INSTR_dreturn(pc+0,0); }
  puts("Overrun of dreturn handler");
}
CASE(_areturn): { // areturn
  { INSTR_areturn(pc+0,0); }
  puts("Overrun of areturn handler");
}
CASE(_return): { // return
  { INSTR_return(pc+0,0); }
  puts("Overrun of return handler");
}
CASE(_getstatic): { // getstatic
  { INSTR_getstatic(pc+0,0); }
  { INSTR_end_sequence(pc+4,0); }
  puts("Overrun of getstatic handler");
}
CASE(_putstatic): { // putstatic
  { INSTR_putstatic(pc+0,0); }
  { INSTR_end_sequence(pc+4,0); }
  puts("Overrun of putstatic handler");
}
CASE(_getfield): { // getfield
  { INSTR_getfield(pc+0,0); }
  { INSTR_end_sequence(pc+4,0); }
  puts("Overrun of getfield handler");
}
CASE(_putfield): { // putfield
  { INSTR_putfield(pc+0,0); }
  { INSTR_end_sequence(pc+4,0); }
  puts("Overrun of putfield handler");
}
CASE(_invokevirtual): { // invokevirtual
  { INSTR_invokevirtual(pc+0,0); }
  puts("Overrun of invokevirtual handler");
}
CASE(_invokespecial): { // invokespecial
  { INSTR_invokespecial(pc+0,0); }
  puts("Overrun of invokespecial handler");
}
CASE(_invokestatic): { // invokestatic
  { INSTR_invokestatic(pc+0,0); }
  puts("Overrun of invokestatic handler");
}
CASE(_invokeinterface): { // invokeinterface
  { INSTR_invokeinterface(pc+0,0); }
  puts("Overrun of invokeinterface handler");
}
CASE(_invokedynamic): { // invokedynamic
  { INSTR_invokedynamic(pc+0,0); }
  puts("Overrun of invokedynamic handler");
}
CASE(_new): { // new
  { INSTR_new(pc+0,0); }
  { INSTR_end_sequence(pc+4,1); }
  puts("Overrun of new handler");
}
CASE(_newarray): { // newarray
  { INSTR_newarray(pc+0,0); }
  { INSTR_end_sequence(pc+3,0); }
  puts("Overrun of newarray handler");
}
CASE(_anewarray): { // anewarray
  { INSTR_anewarray(pc+0,0); }
  { INSTR_end_sequence(pc+4,0); }
  puts("Overrun of anewarray handler");
}
CASE(_arraylength): { // arraylength
  { INSTR_arraylength(pc+0,0); }
  { INSTR_end_sequence(pc+2,0); }
  puts("Overrun of arraylength handler");
}
CASE(_athrow): { // athrow
  { INSTR_athrow(pc+0,0); }
  puts("Overrun of athrow handler");
}
CASE(_checkcast): { // checkcast
  { INSTR_checkcast(pc+0,0); }
  { INSTR_end_sequence(pc+4,0); }
  puts("Overrun of checkcast handler");
}
CASE(_instanceof): { // instanceof
  { INSTR_instanceof(pc+0,0); }
  { INSTR_end_sequence(pc+4,0); }
  puts("Overrun of instanceof handler");
}
CASE(_monitorenter): { // monitorenter
  { INSTR_monitorenter(pc+0,0); }
  puts("Overrun of monitorenter handler");
}
CASE(_monitorexit): { // monitorexit
  { INSTR_monitorexit(pc+0,0); }
  { INSTR_end_sequence(pc+2,-1); }
  puts("Overrun of monitorexit handler");
}
CASE(_wide): { // wide
  { INSTR_wide(pc+0,0); }
  { INSTR_end_sequence(pc+0,0); }
  puts("Overrun of wide handler");
}
CASE(_multianewarray): { // multianewarray
  { INSTR_multianewarray(pc+0,0); }
  { INSTR_end_sequence(pc+5,0); }
  puts("Overrun of multianewarray handler");
}
CASE(_ifnull): { // ifnull
  { INSTR_ifnull(pc+0,0); }
  { INSTR_end_sequence(pc+4,-1); }
  puts("Overrun of ifnull handler");
}
CASE(_ifnonnull): { // ifnonnull
  { INSTR_ifnonnull(pc+0,0); }
  { INSTR_end_sequence(pc+4,-1); }
  puts("Overrun of ifnonnull handler");
}
CASE(_goto_w): { // goto_w
  { INSTR_goto_w(pc+0,0); }
  puts("Overrun of goto_w handler");
}
CASE(_jsr_w): { // jsr_w
  { INSTR_jsr_w(pc+0,0); }
  puts("Overrun of jsr_w handler");
}
CASE(_breakpoint): { // breakpoint
  { INSTR_breakpoint(pc+0,0); }
  puts("Overrun of breakpoint handler");
}
CASE(_fast_aldc): { // fast_aldc
  { INSTR_fast_aldc(pc+0,0); }
  { INSTR_end_sequence(pc+3,1); }
  puts("Overrun of fast_aldc handler");
}
CASE(_fast_aldc_w): { // fast_aldc_w
  { INSTR_fast_aldc_w(pc+0,0); }
  { INSTR_end_sequence(pc+4,1); }
  puts("Overrun of fast_aldc_w handler");
}
CASE(_return_register_finalizer): { // return_register_finalizer
  { INSTR_return_register_finalizer(pc+0,0); }
  puts("Overrun of return_register_finalizer handler");
}
CASE(_invokehandle): { // invokehandle
  { INSTR_invokehandle(pc+0,0); }
  puts("Overrun of invokehandle handler");
}
