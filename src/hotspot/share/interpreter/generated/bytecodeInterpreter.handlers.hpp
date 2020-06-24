/* GENERATED HANDLER FILE -- DO NOT EDIT (2020-03-26T09:40:29.039700Z) */
#include "../bytecodeInterpreter.base.hpp"
#include "../../si/profiler.hpp"

#define WITH_PROFILE false // default for all instructions

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
CASE(_profile): { // profile
  { INSTR_profile(pc+0,0); }
  { INSTR_end_sequence(pc+6,0); }
  puts("Overrun of profile handler");
}
CASE(_super_istore_iload): { // super_istore_iload
  { INSTR_istore(pc+0,0); }
  { INSTR_iload(pc+3,-1); }
  { INSTR_end_sequence(pc+6,0); }
  puts("Overrun of super_istore_iload handler");
}
CASE(_super_lload_3_lload_3_bipush_lshr_lxor_lmul_iload_2_i2l): { // super_lload_3_lload_3_bipush_lshr_lxor_lmul_iload_2_i2l
  { INSTR_lload_3(pc+0,0); }
  { INSTR_lload_3(pc+2,2); }
  { INSTR_bipush(pc+4,4); }
  { INSTR_lshr(pc+7,5); }
  { INSTR_lxor(pc+9,4); }
  { INSTR_lmul(pc+11,2); }
  { INSTR_iload_2(pc+13,0); }
  { INSTR_i2l(pc+15,1); }
  { INSTR_end_sequence(pc+17,2); }
  puts("Overrun of super_lload_3_lload_3_bipush_lshr_lxor_lmul_iload_2_i2l handler");
}
CASE(_super_iload_aload_1_arraylength): { // super_iload_aload_1_arraylength
  { INSTR_iload(pc+0,0); }
  { INSTR_aload_1(pc+3,1); }
  { INSTR_arraylength(pc+5,2); }
  { INSTR_end_sequence(pc+7,2); }
  puts("Overrun of super_iload_aload_1_arraylength handler");
}
CASE(_super_aload_iload): { // super_aload_iload
  { INSTR_aload(pc+0,0); }
  { INSTR_iload(pc+3,1); }
  { INSTR_end_sequence(pc+6,2); }
  puts("Overrun of super_aload_iload handler");
}
CASE(_super_aload_iload_3_iinc_iload_i2b_bastore): { // super_aload_iload_3_iinc_iload_i2b_bastore
  { INSTR_aload(pc+0,0); }
  { INSTR_iload_3(pc+3,1); }
  { INSTR_iinc(pc+5,2); }
  { INSTR_iload(pc+9,2); }
  { INSTR_i2b(pc+12,3); }
  { INSTR_bastore(pc+14,3); }
  { INSTR_end_sequence(pc+16,0); }
  puts("Overrun of super_aload_iload_3_iinc_iload_i2b_bastore handler");
}
CASE(_super_arraylength_aload_0): { // super_arraylength_aload_0
  { INSTR_arraylength(pc+0,0); }
  { INSTR_aload_0(pc+2,0); }
  { INSTR_end_sequence(pc+4,1); }
  puts("Overrun of super_arraylength_aload_0 handler");
}
CASE(_super_iload_2_iload_1): { // super_iload_2_iload_1
  { INSTR_iload_2(pc+0,0); }
  { INSTR_iload_1(pc+2,1); }
  { INSTR_end_sequence(pc+4,2); }
  puts("Overrun of super_iload_2_iload_1 handler");
}
CASE(_super_aload_0_iload_3): { // super_aload_0_iload_3
  { INSTR_aload_0(pc+0,0); }
  { INSTR_iload_3(pc+2,1); }
  { INSTR_end_sequence(pc+4,2); }
  puts("Overrun of super_aload_0_iload_3 handler");
}
CASE(_super_astore_aload): { // super_astore_aload
  { INSTR_astore(pc+0,0); }
  { INSTR_aload(pc+3,-1); }
  { INSTR_end_sequence(pc+6,0); }
  puts("Overrun of super_astore_aload handler");
}
CASE(_super_iload_bipush_iadd_iload): { // super_iload_bipush_iadd_iload
  { INSTR_iload(pc+0,0); }
  { INSTR_bipush(pc+3,1); }
  { INSTR_iadd(pc+6,2); }
  { INSTR_iload(pc+8,1); }
  { INSTR_end_sequence(pc+11,2); }
  puts("Overrun of super_iload_bipush_iadd_iload handler");
}
CASE(_super_istore_3_iload_3): { // super_istore_3_iload_3
  { INSTR_istore_3(pc+0,0); }
  { INSTR_iload_3(pc+2,-1); }
  { INSTR_end_sequence(pc+4,0); }
  puts("Overrun of super_istore_3_iload_3 handler");
}
CASE(_super_iload_1_iload_2_iadd_istore_3_new_dup_aload_0_iload_1_iconst_1_ishl_iload_3_iconst_1): { // super_iload_1_iload_2_iadd_istore_3_new_dup_aload_0_iload_1_iconst_1_ishl_iload_3_iconst_1
  { INSTR_iload_1(pc+0,0); }
  { INSTR_iload_2(pc+2,1); }
  { INSTR_iadd(pc+4,2); }
  { INSTR_istore_3(pc+6,1); }
  { INSTR_new(pc+8,0); }
  { INSTR_dup(pc+12,1); }
  { INSTR_aload_0(pc+14,2); }
  { INSTR_iload_1(pc+16,3); }
  { INSTR_iconst_1(pc+18,4); }
  { INSTR_ishl(pc+20,5); }
  { INSTR_iload_3(pc+22,4); }
  { INSTR_iconst_1(pc+24,5); }
  { INSTR_end_sequence(pc+26,6); }
  puts("Overrun of super_iload_1_iload_2_iadd_istore_3_new_dup_aload_0_iload_1_iconst_1_ishl_iload_3_iconst_1 handler");
}
CASE(_super_dstore_dload): { // super_dstore_dload
  { INSTR_dstore(pc+0,0); }
  { INSTR_dload(pc+3,-2); }
  { INSTR_end_sequence(pc+6,0); }
  puts("Overrun of super_dstore_dload handler");
}
CASE(_super_iload_3_newarray_astore_aload_0_iload_1_aload_iconst_0_aload_0_arraylength_iload_1_isub): { // super_iload_3_newarray_astore_aload_0_iload_1_aload_iconst_0_aload_0_arraylength_iload_1_isub
  { INSTR_iload_3(pc+0,0); }
  { INSTR_newarray(pc+2,1); }
  { INSTR_astore(pc+5,1); }
  { INSTR_aload_0(pc+8,0); }
  { INSTR_iload_1(pc+10,1); }
  { INSTR_aload(pc+12,2); }
  { INSTR_iconst_0(pc+15,3); }
  { INSTR_aload_0(pc+17,4); }
  { INSTR_arraylength(pc+19,5); }
  { INSTR_iload_1(pc+21,5); }
  { INSTR_isub(pc+23,6); }
  { INSTR_end_sequence(pc+25,5); }
  puts("Overrun of super_iload_3_newarray_astore_aload_0_iload_1_aload_iconst_0_aload_0_arraylength_iload_1_isub handler");
}
CASE(_super_bipush_iload_3_imul_aload_0_iload_1_iinc_baload_iadd_istore_3): { // super_bipush_iload_3_imul_aload_0_iload_1_iinc_baload_iadd_istore_3
  { INSTR_bipush(pc+0,0); }
  { INSTR_iload_3(pc+3,1); }
  { INSTR_imul(pc+5,2); }
  { INSTR_aload_0(pc+7,1); }
  { INSTR_iload_1(pc+9,2); }
  { INSTR_iinc(pc+11,3); }
  { INSTR_baload(pc+15,3); }
  { INSTR_iadd(pc+17,2); }
  { INSTR_istore_3(pc+19,1); }
  { INSTR_end_sequence(pc+21,0); }
  puts("Overrun of super_bipush_iload_3_imul_aload_0_iload_1_iinc_baload_iadd_istore_3 handler");
}
CASE(_super_iconst_1_iadd): { // super_iconst_1_iadd
  { INSTR_iconst_1(pc+0,0); }
  { INSTR_iadd(pc+2,1); }
  { INSTR_end_sequence(pc+4,0); }
  puts("Overrun of super_iconst_1_iadd handler");
}
CASE(_super_aload_0_dup): { // super_aload_0_dup
  { INSTR_aload_0(pc+0,0); }
  { INSTR_dup(pc+2,1); }
  { INSTR_end_sequence(pc+4,2); }
  puts("Overrun of super_aload_0_dup handler");
}
CASE(_super_land_lload_lmul_lload_ladd_lstore_aload_3_iload_lload_l2i_iastore_lload_bipush_lushr_lstore): { // super_land_lload_lmul_lload_ladd_lstore_aload_3_iload_lload_l2i_iastore_lload_bipush_lushr_lstore
  { INSTR_land(pc+0,0); }
  { INSTR_lload(pc+2,-2); }
  { INSTR_lmul(pc+5,0); }
  { INSTR_lload(pc+7,-2); }
  { INSTR_ladd(pc+10,0); }
  { INSTR_lstore(pc+12,-2); }
  { INSTR_aload_3(pc+15,-4); }
  { INSTR_iload(pc+17,-3); }
  { INSTR_lload(pc+20,-2); }
  { INSTR_l2i(pc+23,0); }
  { INSTR_iastore(pc+25,-1); }
  { INSTR_lload(pc+27,-4); }
  { INSTR_bipush(pc+30,-2); }
  { INSTR_lushr(pc+33,-1); }
  { INSTR_lstore(pc+35,-2); }
  { INSTR_end_sequence(pc+38,-4); }
  puts("Overrun of super_land_lload_lmul_lload_ladd_lstore_aload_3_iload_lload_l2i_iastore_lload_bipush_lushr_lstore handler");
}
CASE(_super_aload_iload_iinc_aload_3_iload_iinc_caload_i2b_bastore): { // super_aload_iload_iinc_aload_3_iload_iinc_caload_i2b_bastore
  { INSTR_aload(pc+0,0); }
  { INSTR_iload(pc+3,1); }
  { INSTR_iinc(pc+6,2); }
  { INSTR_aload_3(pc+10,2); }
  { INSTR_iload(pc+12,3); }
  { INSTR_iinc(pc+15,4); }
  { INSTR_caload(pc+19,4); }
  { INSTR_i2b(pc+21,3); }
  { INSTR_bastore(pc+23,3); }
  { INSTR_end_sequence(pc+25,0); }
  puts("Overrun of super_aload_iload_iinc_aload_3_iload_iinc_caload_i2b_bastore handler");
}
CASE(_super_aload_0_aload_2): { // super_aload_0_aload_2
  { INSTR_aload_0(pc+0,0); }
  { INSTR_aload_2(pc+2,1); }
  { INSTR_end_sequence(pc+4,2); }
  puts("Overrun of super_aload_0_aload_2 handler");
}
CASE(_super_iload_iload_iload_3_iadd_iload_irem_iastore): { // super_iload_iload_iload_3_iadd_iload_irem_iastore
  { INSTR_iload(pc+0,0); }
  { INSTR_iload(pc+3,1); }
  { INSTR_iload_3(pc+6,2); }
  { INSTR_iadd(pc+8,3); }
  { INSTR_iload(pc+10,2); }
  { INSTR_irem(pc+13,3); }
  { INSTR_iastore(pc+15,2); }
  { INSTR_end_sequence(pc+17,-1); }
  puts("Overrun of super_iload_iload_iload_3_iadd_iload_irem_iastore handler");
}
CASE(_super_iconst_0_istore_1_aload_0_arraylength_iconst_1_ishr_istore_2_iconst_0_istore_3): { // super_iconst_0_istore_1_aload_0_arraylength_iconst_1_ishr_istore_2_iconst_0_istore_3
  { INSTR_iconst_0(pc+0,0); }
  { INSTR_istore_1(pc+2,1); }
  { INSTR_aload_0(pc+4,0); }
  { INSTR_arraylength(pc+6,1); }
  { INSTR_iconst_1(pc+8,1); }
  { INSTR_ishr(pc+10,2); }
  { INSTR_istore_2(pc+12,1); }
  { INSTR_iconst_0(pc+14,0); }
  { INSTR_istore_3(pc+16,1); }
  { INSTR_end_sequence(pc+18,0); }
  puts("Overrun of super_iconst_0_istore_1_aload_0_arraylength_iconst_1_ishr_istore_2_iconst_0_istore_3 handler");
}
CASE(_super_i2l_ladd): { // super_i2l_ladd
  { INSTR_i2l(pc+0,0); }
  { INSTR_ladd(pc+2,1); }
  { INSTR_end_sequence(pc+4,-1); }
  puts("Overrun of super_i2l_ladd handler");
}
CASE(_super_iload_0_iload_0_bipush_iushr_ixor): { // super_iload_0_iload_0_bipush_iushr_ixor
  { INSTR_iload_0(pc+0,0); }
  { INSTR_iload_0(pc+2,1); }
  { INSTR_bipush(pc+4,2); }
  { INSTR_iushr(pc+7,3); }
  { INSTR_ixor(pc+9,2); }
  { INSTR_end_sequence(pc+11,1); }
  puts("Overrun of super_iload_0_iload_0_bipush_iushr_ixor handler");
}
CASE(_super_istore_1_iload_1): { // super_istore_1_iload_1
  { INSTR_istore_1(pc+0,0); }
  { INSTR_iload_1(pc+2,-1); }
  { INSTR_end_sequence(pc+4,0); }
  puts("Overrun of super_istore_1_iload_1 handler");
}
CASE(_super_istore_iload_iconst_1): { // super_istore_iload_iconst_1
  { INSTR_istore(pc+0,0); }
  { INSTR_iload(pc+3,-1); }
  { INSTR_iconst_1(pc+6,0); }
  { INSTR_end_sequence(pc+8,1); }
  puts("Overrun of super_istore_iload_iconst_1 handler");
}
CASE(_super_iload_iinc_baload): { // super_iload_iinc_baload
  { INSTR_iload(pc+0,0); }
  { INSTR_iinc(pc+3,1); }
  { INSTR_baload(pc+7,1); }
  { INSTR_end_sequence(pc+9,0); }
  puts("Overrun of super_iload_iinc_baload handler");
}
CASE(_super_aload_0_arraylength_iconst_1_ishr): { // super_aload_0_arraylength_iconst_1_ishr
  { INSTR_aload_0(pc+0,0); }
  { INSTR_arraylength(pc+2,1); }
  { INSTR_iconst_1(pc+4,1); }
  { INSTR_ishr(pc+6,2); }
  { INSTR_end_sequence(pc+8,1); }
  puts("Overrun of super_aload_0_arraylength_iconst_1_ishr handler");
}
CASE(_super_iload_1_iconst_1_ishl_istore_1_aload_0_iload_1_iinc_iload_2): { // super_iload_1_iconst_1_ishl_istore_1_aload_0_iload_1_iinc_iload_2
  { INSTR_iload_1(pc+0,0); }
  { INSTR_iconst_1(pc+2,1); }
  { INSTR_ishl(pc+4,2); }
  { INSTR_istore_1(pc+6,1); }
  { INSTR_aload_0(pc+8,0); }
  { INSTR_iload_1(pc+10,1); }
  { INSTR_iinc(pc+12,2); }
  { INSTR_iload_2(pc+16,2); }
  { INSTR_end_sequence(pc+18,3); }
  puts("Overrun of super_iload_1_iconst_1_ishl_istore_1_aload_0_iload_1_iinc_iload_2 handler");
}
CASE(_super_dup_istore_1_iload_1_bipush_iushr_ixor): { // super_dup_istore_1_iload_1_bipush_iushr_ixor
  { INSTR_dup(pc+0,0); }
  { INSTR_istore_1(pc+2,1); }
  { INSTR_iload_1(pc+4,0); }
  { INSTR_bipush(pc+6,1); }
  { INSTR_iushr(pc+9,2); }
  { INSTR_ixor(pc+11,1); }
  { INSTR_end_sequence(pc+13,0); }
  puts("Overrun of super_dup_istore_1_iload_1_bipush_iushr_ixor handler");
}
CASE(_super_ishl_aload_0_iload_1_baload_sipush_iand): { // super_ishl_aload_0_iload_1_baload_sipush_iand
  { INSTR_ishl(pc+0,0); }
  { INSTR_aload_0(pc+2,-1); }
  { INSTR_iload_1(pc+4,0); }
  { INSTR_baload(pc+6,1); }
  { INSTR_sipush(pc+8,0); }
  { INSTR_iand(pc+12,1); }
  { INSTR_end_sequence(pc+14,0); }
  puts("Overrun of super_ishl_aload_0_iload_1_baload_sipush_iand handler");
}
CASE(_super_aload_0_aload): { // super_aload_0_aload
  { INSTR_aload_0(pc+0,0); }
  { INSTR_aload(pc+2,1); }
  { INSTR_end_sequence(pc+5,2); }
  puts("Overrun of super_aload_0_aload handler");
}
CASE(_super_iload_1_aload_0): { // super_iload_1_aload_0
  { INSTR_iload_1(pc+0,0); }
  { INSTR_aload_0(pc+2,1); }
  { INSTR_end_sequence(pc+4,2); }
  puts("Overrun of super_iload_1_aload_0 handler");
}
CASE(_super_iload_2_iload_3): { // super_iload_2_iload_3
  { INSTR_iload_2(pc+0,0); }
  { INSTR_iload_3(pc+2,1); }
  { INSTR_end_sequence(pc+4,2); }
  puts("Overrun of super_iload_2_iload_3 handler");
}
CASE(_super_iload_3_iconst_5_ishl_bipush_ishr_istore_iload_1_iload_isub_istore_2): { // super_iload_3_iconst_5_ishl_bipush_ishr_istore_iload_1_iload_isub_istore_2
  { INSTR_iload_3(pc+0,0); }
  { INSTR_iconst_5(pc+2,1); }
  { INSTR_ishl(pc+4,2); }
  { INSTR_bipush(pc+6,1); }
  { INSTR_ishr(pc+9,2); }
  { INSTR_istore(pc+11,1); }
  { INSTR_iload_1(pc+14,0); }
  { INSTR_iload(pc+16,1); }
  { INSTR_isub(pc+19,2); }
  { INSTR_istore_2(pc+21,1); }
  { INSTR_end_sequence(pc+23,0); }
  puts("Overrun of super_iload_3_iconst_5_ishl_bipush_ishr_istore_iload_1_iload_isub_istore_2 handler");
}
CASE(_super_iconst_0_aload_1_iload_2_iload_3_ishl_aload_0): { // super_iconst_0_aload_1_iload_2_iload_3_ishl_aload_0
  { INSTR_iconst_0(pc+0,0); }
  { INSTR_aload_1(pc+2,1); }
  { INSTR_iload_2(pc+4,2); }
  { INSTR_iload_3(pc+6,3); }
  { INSTR_ishl(pc+8,4); }
  { INSTR_aload_0(pc+10,3); }
  { INSTR_end_sequence(pc+12,4); }
  puts("Overrun of super_iconst_0_aload_1_iload_2_iload_3_ishl_aload_0 handler");
}
CASE(_super_iconst_2_istore_2): { // super_iconst_2_istore_2
  { INSTR_iconst_2(pc+0,0); }
  { INSTR_istore_2(pc+2,1); }
  { INSTR_end_sequence(pc+4,0); }
  puts("Overrun of super_iconst_2_istore_2 handler");
}
CASE(_super_aload_0_iload): { // super_aload_0_iload
  { INSTR_aload_0(pc+0,0); }
  { INSTR_iload(pc+2,1); }
  { INSTR_end_sequence(pc+5,2); }
  puts("Overrun of super_aload_0_iload handler");
}
CASE(_super_iload_1_iinc_iload_iastore_iload_1): { // super_iload_1_iinc_iload_iastore_iload_1
  { INSTR_iload_1(pc+0,0); }
  { INSTR_iinc(pc+2,1); }
  { INSTR_iload(pc+6,1); }
  { INSTR_iastore(pc+9,2); }
  { INSTR_iload_1(pc+11,-1); }
  { INSTR_end_sequence(pc+13,0); }
  puts("Overrun of super_iload_1_iinc_iload_iastore_iload_1 handler");
}
CASE(_super_iand_istore_iload_bipush_iand): { // super_iand_istore_iload_bipush_iand
  { INSTR_iand(pc+0,0); }
  { INSTR_istore(pc+2,-1); }
  { INSTR_iload(pc+5,-2); }
  { INSTR_bipush(pc+8,-1); }
  { INSTR_iand(pc+11,0); }
  { INSTR_end_sequence(pc+13,-1); }
  puts("Overrun of super_iand_istore_iload_bipush_iand handler");
}
CASE(_super_aload_2_iload_aaload_astore_bipush_iload_1_imul_aload): { // super_aload_2_iload_aaload_astore_bipush_iload_1_imul_aload
  { INSTR_aload_2(pc+0,0); }
  { INSTR_iload(pc+2,1); }
  { INSTR_aaload(pc+5,2); }
  { INSTR_astore(pc+7,1); }
  { INSTR_bipush(pc+10,0); }
  { INSTR_iload_1(pc+13,1); }
  { INSTR_imul(pc+15,2); }
  { INSTR_aload(pc+17,1); }
  { INSTR_end_sequence(pc+20,2); }
  puts("Overrun of super_aload_2_iload_aaload_astore_bipush_iload_1_imul_aload handler");
}
CASE(_super_iload_0_iload_1): { // super_iload_0_iload_1
  { INSTR_iload_0(pc+0,0); }
  { INSTR_iload_1(pc+2,1); }
  { INSTR_end_sequence(pc+4,2); }
  puts("Overrun of super_iload_0_iload_1 handler");
}
CASE(_super_aload_1_iload_baload_aload_0): { // super_aload_1_iload_baload_aload_0
  { INSTR_aload_1(pc+0,0); }
  { INSTR_iload(pc+2,1); }
  { INSTR_baload(pc+5,2); }
  { INSTR_aload_0(pc+7,1); }
  { INSTR_end_sequence(pc+9,2); }
  puts("Overrun of super_aload_1_iload_baload_aload_0 handler");
}
CASE(_super_iload_bipush): { // super_iload_bipush
  { INSTR_iload(pc+0,0); }
  { INSTR_bipush(pc+3,1); }
  { INSTR_end_sequence(pc+6,2); }
  puts("Overrun of super_iload_bipush handler");
}
CASE(_super_istore_aload_iload_iload_iload_1): { // super_istore_aload_iload_iload_iload_1
  { INSTR_istore(pc+0,0); }
  { INSTR_aload(pc+3,-1); }
  { INSTR_iload(pc+6,0); }
  { INSTR_iload(pc+9,1); }
  { INSTR_iload_1(pc+12,2); }
  { INSTR_end_sequence(pc+14,3); }
  puts("Overrun of super_istore_aload_iload_iload_iload_1 handler");
}
CASE(_super_istore_aload_0): { // super_istore_aload_0
  { INSTR_istore(pc+0,0); }
  { INSTR_aload_0(pc+3,-1); }
  { INSTR_end_sequence(pc+5,0); }
  puts("Overrun of super_istore_aload_0 handler");
}
CASE(_super_bipush_ishl_ior_i2l): { // super_bipush_ishl_ior_i2l
  { INSTR_bipush(pc+0,0); }
  { INSTR_ishl(pc+3,1); }
  { INSTR_ior(pc+5,0); }
  { INSTR_i2l(pc+7,-1); }
  { INSTR_end_sequence(pc+9,0); }
  puts("Overrun of super_bipush_ishl_ior_i2l handler");
}
CASE(_super_iload_0_aload_1): { // super_iload_0_aload_1
  { INSTR_iload_0(pc+0,0); }
  { INSTR_aload_1(pc+2,1); }
  { INSTR_end_sequence(pc+4,2); }
  puts("Overrun of super_iload_0_aload_1 handler");
}
CASE(_super_aload_2_aload_3): { // super_aload_2_aload_3
  { INSTR_aload_2(pc+0,0); }
  { INSTR_aload_3(pc+2,1); }
  { INSTR_end_sequence(pc+4,2); }
  puts("Overrun of super_aload_2_aload_3 handler");
}
CASE(_super_iload_bipush_iload_iadd_iload_iadd_iload_iadd_iadd_istore_iinc): { // super_iload_bipush_iload_iadd_iload_iadd_iload_iadd_iadd_istore_iinc
  { INSTR_iload(pc+0,0); }
  { INSTR_bipush(pc+3,1); }
  { INSTR_iload(pc+6,2); }
  { INSTR_iadd(pc+9,3); }
  { INSTR_iload(pc+11,2); }
  { INSTR_iadd(pc+14,3); }
  { INSTR_iload(pc+16,2); }
  { INSTR_iadd(pc+19,3); }
  { INSTR_iadd(pc+21,2); }
  { INSTR_istore(pc+23,1); }
  { INSTR_iinc(pc+26,0); }
  { INSTR_end_sequence(pc+30,0); }
  puts("Overrun of super_iload_bipush_iload_iadd_iload_iadd_iload_iadd_iadd_istore_iinc handler");
}
CASE(_super_ishl_ior_i2c): { // super_ishl_ior_i2c
  { INSTR_ishl(pc+0,0); }
  { INSTR_ior(pc+2,-1); }
  { INSTR_i2c(pc+4,-2); }
  { INSTR_end_sequence(pc+6,-2); }
  puts("Overrun of super_ishl_ior_i2c handler");
}
CASE(_super_iload_iload_iload_iadd_iconst_2_isub_iload_irem): { // super_iload_iload_iload_iadd_iconst_2_isub_iload_irem
  { INSTR_iload(pc+0,0); }
  { INSTR_iload(pc+3,1); }
  { INSTR_iload(pc+6,2); }
  { INSTR_iadd(pc+9,3); }
  { INSTR_iconst_2(pc+11,2); }
  { INSTR_isub(pc+13,3); }
  { INSTR_iload(pc+15,2); }
  { INSTR_irem(pc+18,3); }
  { INSTR_end_sequence(pc+20,2); }
  puts("Overrun of super_iload_iload_iload_iadd_iconst_2_isub_iload_irem handler");
}
CASE(_super_iload_3_iload): { // super_iload_3_iload
  { INSTR_iload_3(pc+0,0); }
  { INSTR_iload(pc+2,1); }
  { INSTR_end_sequence(pc+5,2); }
  puts("Overrun of super_iload_3_iload handler");
}
CASE(_super_iload_iload_2): { // super_iload_iload_2
  { INSTR_iload(pc+0,0); }
  { INSTR_iload_2(pc+3,1); }
  { INSTR_end_sequence(pc+5,2); }
  puts("Overrun of super_iload_iload_2 handler");
}
CASE(_super_iload_1_newarray_astore_2_aload_0_iconst_0_aload_2_iconst_0_aload_0_arraylength_iload_1): { // super_iload_1_newarray_astore_2_aload_0_iconst_0_aload_2_iconst_0_aload_0_arraylength_iload_1
  { INSTR_iload_1(pc+0,0); }
  { INSTR_newarray(pc+2,1); }
  { INSTR_astore_2(pc+5,1); }
  { INSTR_aload_0(pc+7,0); }
  { INSTR_iconst_0(pc+9,1); }
  { INSTR_aload_2(pc+11,2); }
  { INSTR_iconst_0(pc+13,3); }
  { INSTR_aload_0(pc+15,4); }
  { INSTR_arraylength(pc+17,5); }
  { INSTR_iload_1(pc+19,5); }
  { INSTR_end_sequence(pc+21,6); }
  puts("Overrun of super_iload_1_newarray_astore_2_aload_0_iconst_0_aload_2_iconst_0_aload_0_arraylength_iload_1 handler");
}
CASE(_super_aload_2_iload_aaload_astore_aload): { // super_aload_2_iload_aaload_astore_aload
  { INSTR_aload_2(pc+0,0); }
  { INSTR_iload(pc+2,1); }
  { INSTR_aaload(pc+5,2); }
  { INSTR_astore(pc+7,1); }
  { INSTR_aload(pc+10,0); }
  { INSTR_end_sequence(pc+13,1); }
  puts("Overrun of super_aload_2_iload_aaload_astore_aload handler");
}
CASE(_super_dup_astore): { // super_dup_astore
  { INSTR_dup(pc+0,0); }
  { INSTR_astore(pc+2,1); }
  { INSTR_end_sequence(pc+5,0); }
  puts("Overrun of super_dup_astore handler");
}
CASE(_super_aload_2_iload_iaload_istore_aload_1_aload_0_iload): { // super_aload_2_iload_iaload_istore_aload_1_aload_0_iload
  { INSTR_aload_2(pc+0,0); }
  { INSTR_iload(pc+2,1); }
  { INSTR_iaload(pc+5,2); }
  { INSTR_istore(pc+7,1); }
  { INSTR_aload_1(pc+10,0); }
  { INSTR_aload_0(pc+12,1); }
  { INSTR_iload(pc+14,2); }
  { INSTR_end_sequence(pc+17,3); }
  puts("Overrun of super_aload_2_iload_iaload_istore_aload_1_aload_0_iload handler");
}
CASE(_super_iload_2_aaload_astore_3_aload_3): { // super_iload_2_aaload_astore_3_aload_3
  { INSTR_iload_2(pc+0,0); }
  { INSTR_aaload(pc+2,1); }
  { INSTR_astore_3(pc+4,0); }
  { INSTR_aload_3(pc+6,-1); }
  { INSTR_end_sequence(pc+8,0); }
  puts("Overrun of super_iload_2_aaload_astore_3_aload_3 handler");
}
CASE(_super_istore_2_aload_0): { // super_istore_2_aload_0
  { INSTR_istore_2(pc+0,0); }
  { INSTR_aload_0(pc+2,-1); }
  { INSTR_end_sequence(pc+4,0); }
  puts("Overrun of super_istore_2_aload_0 handler");
}
CASE(_super_iload_2_aload_1_arraylength_isub_iaload_i2l_lstore_3_aload_0): { // super_iload_2_aload_1_arraylength_isub_iaload_i2l_lstore_3_aload_0
  { INSTR_iload_2(pc+0,0); }
  { INSTR_aload_1(pc+2,1); }
  { INSTR_arraylength(pc+4,2); }
  { INSTR_isub(pc+6,2); }
  { INSTR_iaload(pc+8,1); }
  { INSTR_i2l(pc+10,0); }
  { INSTR_lstore_3(pc+12,1); }
  { INSTR_aload_0(pc+14,-1); }
  { INSTR_end_sequence(pc+16,0); }
  puts("Overrun of super_iload_2_aload_1_arraylength_isub_iaload_i2l_lstore_3_aload_0 handler");
}
CASE(_super_aload_0_iload_1_bipush_iadd): { // super_aload_0_iload_1_bipush_iadd
  { INSTR_aload_0(pc+0,0); }
  { INSTR_iload_1(pc+2,1); }
  { INSTR_bipush(pc+4,2); }
  { INSTR_iadd(pc+7,3); }
  { INSTR_end_sequence(pc+9,2); }
  puts("Overrun of super_aload_0_iload_1_bipush_iadd handler");
}
CASE(_super_iastore_iinc): { // super_iastore_iinc
  { INSTR_iastore(pc+0,0); }
  { INSTR_iinc(pc+2,-3); }
  { INSTR_end_sequence(pc+6,-3); }
  puts("Overrun of super_iastore_iinc handler");
}
CASE(_super_aload_0_aload_0): { // super_aload_0_aload_0
  { INSTR_aload_0(pc+0,0); }
  { INSTR_aload_0(pc+2,1); }
  { INSTR_end_sequence(pc+4,2); }
  puts("Overrun of super_aload_0_aload_0 handler");
}
CASE(_super_aload_0_arraylength_aload_1_arraylength): { // super_aload_0_arraylength_aload_1_arraylength
  { INSTR_aload_0(pc+0,0); }
  { INSTR_arraylength(pc+2,1); }
  { INSTR_aload_1(pc+4,1); }
  { INSTR_arraylength(pc+6,2); }
  { INSTR_end_sequence(pc+8,2); }
  puts("Overrun of super_aload_0_arraylength_aload_1_arraylength handler");
}
CASE(_super_dup_istore): { // super_dup_istore
  { INSTR_dup(pc+0,0); }
  { INSTR_istore(pc+2,1); }
  { INSTR_end_sequence(pc+5,0); }
  puts("Overrun of super_dup_istore handler");
}
CASE(_super_iload_iload_3): { // super_iload_iload_3
  { INSTR_iload(pc+0,0); }
  { INSTR_iload_3(pc+3,1); }
  { INSTR_end_sequence(pc+5,2); }
  puts("Overrun of super_iload_iload_3 handler");
}
CASE(_super_iload_iload_iload_2_iadd_iload_irem_iastore_aload_0): { // super_iload_iload_iload_2_iadd_iload_irem_iastore_aload_0
  { INSTR_iload(pc+0,0); }
  { INSTR_iload(pc+3,1); }
  { INSTR_iload_2(pc+6,2); }
  { INSTR_iadd(pc+8,3); }
  { INSTR_iload(pc+10,2); }
  { INSTR_irem(pc+13,3); }
  { INSTR_iastore(pc+15,2); }
  { INSTR_aload_0(pc+17,-1); }
  { INSTR_end_sequence(pc+19,0); }
  puts("Overrun of super_iload_iload_iload_2_iadd_iload_irem_iastore_aload_0 handler");
}
CASE(_super_iload_1_iload_2): { // super_iload_1_iload_2
  { INSTR_iload_1(pc+0,0); }
  { INSTR_iload_2(pc+2,1); }
  { INSTR_end_sequence(pc+4,2); }
  puts("Overrun of super_iload_1_iload_2 handler");
}
CASE(_super_dup_x1_iconst_1_iadd): { // super_dup_x1_iconst_1_iadd
  { INSTR_dup_x1(pc+0,0); }
  { INSTR_iconst_1(pc+2,1); }
  { INSTR_iadd(pc+4,2); }
  { INSTR_end_sequence(pc+6,1); }
  puts("Overrun of super_dup_x1_iconst_1_iadd handler");
}
CASE(_super_iadd_istore_1_iinc): { // super_iadd_istore_1_iinc
  { INSTR_iadd(pc+0,0); }
  { INSTR_istore_1(pc+2,-1); }
  { INSTR_iinc(pc+4,-2); }
  { INSTR_end_sequence(pc+8,-2); }
  puts("Overrun of super_iadd_istore_1_iinc handler");
}
CASE(_super_iload_2_iinc): { // super_iload_2_iinc
  { INSTR_iload_2(pc+0,0); }
  { INSTR_iinc(pc+2,1); }
  { INSTR_end_sequence(pc+6,1); }
  puts("Overrun of super_iload_2_iinc handler");
}
CASE(_super_istore_3_iload_1_iload_2_ixor_iload_1_iload_3_ixor): { // super_istore_3_iload_1_iload_2_ixor_iload_1_iload_3_ixor
  { INSTR_istore_3(pc+0,0); }
  { INSTR_iload_1(pc+2,-1); }
  { INSTR_iload_2(pc+4,0); }
  { INSTR_ixor(pc+6,1); }
  { INSTR_iload_1(pc+8,0); }
  { INSTR_iload_3(pc+10,1); }
  { INSTR_ixor(pc+12,2); }
  { INSTR_end_sequence(pc+14,1); }
  puts("Overrun of super_istore_3_iload_1_iload_2_ixor_iload_1_iload_3_ixor handler");
}
CASE(_super_iload_1_iconst_1_ishl_istore_1_aload_0_iload_1_iinc_baload_sipush_iand): { // super_iload_1_iconst_1_ishl_istore_1_aload_0_iload_1_iinc_baload_sipush_iand
  { INSTR_iload_1(pc+0,0); }
  { INSTR_iconst_1(pc+2,1); }
  { INSTR_ishl(pc+4,2); }
  { INSTR_istore_1(pc+6,1); }
  { INSTR_aload_0(pc+8,0); }
  { INSTR_iload_1(pc+10,1); }
  { INSTR_iinc(pc+12,2); }
  { INSTR_baload(pc+16,2); }
  { INSTR_sipush(pc+18,1); }
  { INSTR_iand(pc+22,2); }
  { INSTR_end_sequence(pc+24,1); }
  puts("Overrun of super_iload_1_iconst_1_ishl_istore_1_aload_0_iload_1_iinc_baload_sipush_iand handler");
}
CASE(_super_aload_1_iload): { // super_aload_1_iload
  { INSTR_aload_1(pc+0,0); }
  { INSTR_iload(pc+2,1); }
  { INSTR_end_sequence(pc+5,2); }
  puts("Overrun of super_aload_1_iload handler");
}
CASE(_super_ishr_i2b_bastore): { // super_ishr_i2b_bastore
  { INSTR_ishr(pc+0,0); }
  { INSTR_i2b(pc+2,-1); }
  { INSTR_bastore(pc+4,-1); }
  { INSTR_end_sequence(pc+6,-4); }
  puts("Overrun of super_ishr_i2b_bastore handler");
}
CASE(_super_aload_0_iload_1_baload_sipush_iand_aload_0_iload_1_iconst_1_iadd_baload_sipush_iand_bipush_ishl_ior): { // super_aload_0_iload_1_baload_sipush_iand_aload_0_iload_1_iconst_1_iadd_baload_sipush_iand_bipush_ishl_ior
  { INSTR_aload_0(pc+0,0); }
  { INSTR_iload_1(pc+2,1); }
  { INSTR_baload(pc+4,2); }
  { INSTR_sipush(pc+6,1); }
  { INSTR_iand(pc+10,2); }
  { INSTR_aload_0(pc+12,1); }
  { INSTR_iload_1(pc+14,2); }
  { INSTR_iconst_1(pc+16,3); }
  { INSTR_iadd(pc+18,4); }
  { INSTR_baload(pc+20,3); }
  { INSTR_sipush(pc+22,2); }
  { INSTR_iand(pc+26,3); }
  { INSTR_bipush(pc+28,2); }
  { INSTR_ishl(pc+31,3); }
  { INSTR_ior(pc+33,2); }
  { INSTR_end_sequence(pc+35,1); }
  puts("Overrun of super_aload_0_iload_1_baload_sipush_iand_aload_0_iload_1_iconst_1_iadd_baload_sipush_iand_bipush_ishl_ior handler");
}
CASE(_super_aload_0_iload_1): { // super_aload_0_iload_1
  { INSTR_aload_0(pc+0,0); }
  { INSTR_iload_1(pc+2,1); }
  { INSTR_end_sequence(pc+4,2); }
  puts("Overrun of super_aload_0_iload_1 handler");
}
CASE(_super_aload_0_iload_1_iload_2_iadd_iconst_1_isub_baload_bipush): { // super_aload_0_iload_1_iload_2_iadd_iconst_1_isub_baload_bipush
  { INSTR_aload_0(pc+0,0); }
  { INSTR_iload_1(pc+2,1); }
  { INSTR_iload_2(pc+4,2); }
  { INSTR_iadd(pc+6,3); }
  { INSTR_iconst_1(pc+8,2); }
  { INSTR_isub(pc+10,3); }
  { INSTR_baload(pc+12,2); }
  { INSTR_bipush(pc+14,1); }
  { INSTR_end_sequence(pc+17,2); }
  puts("Overrun of super_aload_0_iload_1_iload_2_iadd_iconst_1_isub_baload_bipush handler");
}
CASE(_super_iconst_1_isub): { // super_iconst_1_isub
  { INSTR_iconst_1(pc+0,0); }
  { INSTR_isub(pc+2,1); }
  { INSTR_end_sequence(pc+4,0); }
  puts("Overrun of super_iconst_1_isub handler");
}
CASE(_super_ishr_i2b_bastore_aload_0_iload_1_iload_2): { // super_ishr_i2b_bastore_aload_0_iload_1_iload_2
  { INSTR_ishr(pc+0,0); }
  { INSTR_i2b(pc+2,-1); }
  { INSTR_bastore(pc+4,-1); }
  { INSTR_aload_0(pc+6,-4); }
  { INSTR_iload_1(pc+8,-3); }
  { INSTR_iload_2(pc+10,-2); }
  { INSTR_end_sequence(pc+12,-1); }
  puts("Overrun of super_ishr_i2b_bastore_aload_0_iload_1_iload_2 handler");
}
CASE(_super_imul_iload_ixor_istore_1): { // super_imul_iload_ixor_istore_1
  { INSTR_imul(pc+0,0); }
  { INSTR_iload(pc+2,-1); }
  { INSTR_ixor(pc+5,0); }
  { INSTR_istore_1(pc+7,-1); }
  { INSTR_end_sequence(pc+9,-2); }
  puts("Overrun of super_imul_iload_ixor_istore_1 handler");
}
CASE(_super_ishr_istore_2_iload_2_iconst_1_ishl_iconst_2_iadd_istore_3_iload_3_iload_1): { // super_ishr_istore_2_iload_2_iconst_1_ishl_iconst_2_iadd_istore_3_iload_3_iload_1
  { INSTR_ishr(pc+0,0); }
  { INSTR_istore_2(pc+2,-1); }
  { INSTR_iload_2(pc+4,-2); }
  { INSTR_iconst_1(pc+6,-1); }
  { INSTR_ishl(pc+8,0); }
  { INSTR_iconst_2(pc+10,-1); }
  { INSTR_iadd(pc+12,0); }
  { INSTR_istore_3(pc+14,-1); }
  { INSTR_iload_3(pc+16,-2); }
  { INSTR_iload_1(pc+18,-1); }
  { INSTR_end_sequence(pc+20,0); }
  puts("Overrun of super_ishr_istore_2_iload_2_iconst_1_ishl_iconst_2_iadd_istore_3_iload_3_iload_1 handler");
}
CASE(_super_20_istore_2_iload_2_iload_2_bipush_iushr_iload_2_bipush_iushr_ixor_ixor_istore_2_iload_2_iload_2_bipush_2606179701): { // super_20_istore_2_iload_2_iload_2_bipush_iushr_iload_2_bipush_iushr_ixor_ixor_istore_2_iload_2_iload_2_bipush_2606179701
  { INSTR_istore_2(pc+0,0); }
  { INSTR_iload_2(pc+2,-1); }
  { INSTR_iload_2(pc+4,0); }
  { INSTR_bipush(pc+6,1); }
  { INSTR_iushr(pc+9,2); }
  { INSTR_iload_2(pc+11,1); }
  { INSTR_bipush(pc+13,2); }
  { INSTR_iushr(pc+16,3); }
  { INSTR_ixor(pc+18,2); }
  { INSTR_ixor(pc+20,1); }
  { INSTR_istore_2(pc+22,0); }
  { INSTR_iload_2(pc+24,-1); }
  { INSTR_iload_2(pc+26,0); }
  { INSTR_bipush(pc+28,1); }
  { INSTR_iushr(pc+31,2); }
  { INSTR_ixor(pc+33,1); }
  { INSTR_iload_2(pc+35,0); }
  { INSTR_iconst_4(pc+37,1); }
  { INSTR_iushr(pc+39,2); }
  { INSTR_ixor(pc+41,1); }
  { INSTR_end_sequence(pc+43,0); }
  puts("Overrun of super_20_istore_2_iload_2_iload_2_bipush_iushr_iload_2_bipush_iushr_ixor_ixor_istore_2_iload_2_iload_2_bipush_2606179701 handler");
}
CASE(_super_bipush_iload_1_imul_aload_0_iload_3): { // super_bipush_iload_1_imul_aload_0_iload_3
  { INSTR_bipush(pc+0,0); }
  { INSTR_iload_1(pc+3,1); }
  { INSTR_imul(pc+5,2); }
  { INSTR_aload_0(pc+7,1); }
  { INSTR_iload_3(pc+9,2); }
  { INSTR_end_sequence(pc+11,3); }
  puts("Overrun of super_bipush_iload_1_imul_aload_0_iload_3 handler");
}
CASE(_super_iload_iload): { // super_iload_iload
  { INSTR_iload(pc+0,0); }
  { INSTR_iload(pc+3,1); }
  { INSTR_end_sequence(pc+6,2); }
  puts("Overrun of super_iload_iload handler");
}
CASE(_super_dload_dload_dmul_dadd_dstore): { // super_dload_dload_dmul_dadd_dstore
  { INSTR_dload(pc+0,0); }
  { INSTR_dload(pc+3,2); }
  { INSTR_dmul(pc+6,4); }
  { INSTR_dadd(pc+8,2); }
  { INSTR_dstore(pc+10,0); }
  { INSTR_end_sequence(pc+13,-2); }
  puts("Overrun of super_dload_dload_dmul_dadd_dstore handler");
}
CASE(_super_aload_iload_2_iinc_iload_i2b_bastore_iinc): { // super_aload_iload_2_iinc_iload_i2b_bastore_iinc
  { INSTR_aload(pc+0,0); }
  { INSTR_iload_2(pc+3,1); }
  { INSTR_iinc(pc+5,2); }
  { INSTR_iload(pc+9,2); }
  { INSTR_i2b(pc+12,3); }
  { INSTR_bastore(pc+14,3); }
  { INSTR_iinc(pc+16,0); }
  { INSTR_end_sequence(pc+20,0); }
  puts("Overrun of super_aload_iload_2_iinc_iload_i2b_bastore_iinc handler");
}
CASE(_super_aload_0_iload_2): { // super_aload_0_iload_2
  { INSTR_aload_0(pc+0,0); }
  { INSTR_iload_2(pc+2,1); }
  { INSTR_end_sequence(pc+4,2); }
  puts("Overrun of super_aload_0_iload_2 handler");
}
CASE(_super_aload_3_iload_iinc_aload_0_iload): { // super_aload_3_iload_iinc_aload_0_iload
  { INSTR_aload_3(pc+0,0); }
  { INSTR_iload(pc+2,1); }
  { INSTR_iinc(pc+5,2); }
  { INSTR_aload_0(pc+9,2); }
  { INSTR_iload(pc+11,3); }
  { INSTR_end_sequence(pc+14,4); }
  puts("Overrun of super_aload_3_iload_iinc_aload_0_iload handler");
}
CASE(_super_aload_0_aload_1): { // super_aload_0_aload_1
  { INSTR_aload_0(pc+0,0); }
  { INSTR_aload_1(pc+2,1); }
  { INSTR_end_sequence(pc+4,2); }
  puts("Overrun of super_aload_0_aload_1 handler");
}
CASE(_super_iload_1_iload_2_irem): { // super_iload_1_iload_2_irem
  { INSTR_iload_1(pc+0,0); }
  { INSTR_iload_2(pc+2,1); }
  { INSTR_irem(pc+4,2); }
  { INSTR_end_sequence(pc+6,1); }
  puts("Overrun of super_iload_1_iload_2_irem handler");
}
CASE(_super_iload_1_i2l_iconst_0_lshl_ladd): { // super_iload_1_i2l_iconst_0_lshl_ladd
  { INSTR_iload_1(pc+0,0); }
  { INSTR_i2l(pc+2,1); }
  { INSTR_iconst_0(pc+4,2); }
  { INSTR_lshl(pc+6,3); }
  { INSTR_ladd(pc+8,2); }
  { INSTR_end_sequence(pc+10,0); }
  puts("Overrun of super_iload_1_i2l_iconst_0_lshl_ladd handler");
}
CASE(_super_astore_2_aload_0): { // super_astore_2_aload_0
  { INSTR_astore_2(pc+0,0); }
  { INSTR_aload_0(pc+2,-1); }
  { INSTR_end_sequence(pc+4,0); }
  puts("Overrun of super_astore_2_aload_0 handler");
}
CASE(_super_iload_3_iload_2): { // super_iload_3_iload_2
  { INSTR_iload_3(pc+0,0); }
  { INSTR_iload_2(pc+2,1); }
  { INSTR_end_sequence(pc+4,2); }
  puts("Overrun of super_iload_3_iload_2 handler");
}
CASE(_super_new_dup): { // super_new_dup
  { INSTR_new(pc+0,0); }
  { INSTR_dup(pc+4,1); }
  { INSTR_end_sequence(pc+6,2); }
  puts("Overrun of super_new_dup handler");
}
CASE(_super_iload_3_bipush): { // super_iload_3_bipush
  { INSTR_iload_3(pc+0,0); }
  { INSTR_bipush(pc+2,1); }
  { INSTR_end_sequence(pc+5,2); }
  puts("Overrun of super_iload_3_bipush handler");
}
CASE(_super_iload_1_iinc_iload_2_iastore_aload_0): { // super_iload_1_iinc_iload_2_iastore_aload_0
  { INSTR_iload_1(pc+0,0); }
  { INSTR_iinc(pc+2,1); }
  { INSTR_iload_2(pc+6,1); }
  { INSTR_iastore(pc+8,2); }
  { INSTR_aload_0(pc+10,-1); }
  { INSTR_end_sequence(pc+12,0); }
  puts("Overrun of super_iload_1_iinc_iload_2_iastore_aload_0 handler");
}
CASE(_super_astore_2_aload_2): { // super_astore_2_aload_2
  { INSTR_astore_2(pc+0,0); }
  { INSTR_aload_2(pc+2,-1); }
  { INSTR_end_sequence(pc+4,0); }
  puts("Overrun of super_astore_2_aload_2 handler");
}
CASE(_super_aload_1_iload_3): { // super_aload_1_iload_3
  { INSTR_aload_1(pc+0,0); }
  { INSTR_iload_3(pc+2,1); }
  { INSTR_end_sequence(pc+4,2); }
  puts("Overrun of super_aload_1_iload_3 handler");
}
