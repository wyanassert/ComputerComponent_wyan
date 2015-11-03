#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute(){
     #if DATA_BYTE==1
        OPERAND_W(op_dest,(int16_t)op_src->val);
    #else
        OPERAND_W(op_dest,(int32_t)op_src->val);
    #endif

    //flags affected none

    print_asm_template2();


}

make_instr_helper(rm2r);
#include "cpu/exec/template-end.h"
