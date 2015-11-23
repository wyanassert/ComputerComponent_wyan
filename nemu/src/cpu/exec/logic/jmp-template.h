#include "cpu/exec/template-start.h"
#include "cpu/reg.h"
#define instr jmp


static void do_execute(){

   if (op_src->type == OP_TYPE_IMM)
    {
        int32_t tsrc = (DATA_TYPE_S)op_src->val;
        cpu.eip += tsrc;
#if DATA_BYTE == 2
        cpu.eip &= 0x0000ffff;
#endif // DATA_BYTE
    }
    else
    {
        cpu.eip = op_src->val;
#if DATA_BYTE == 2
        cpu.eip &= 0xffff;
#endif // DATA_BYTE

    }
    print_asm_template1();

}

make_instr_helper(i)
make_instr_helper(rm);
#include "cpu/exec/template-end.h"
