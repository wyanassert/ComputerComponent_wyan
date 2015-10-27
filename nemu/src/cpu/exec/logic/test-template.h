#include "cpu/exec/template-start.h"

#define instr test

static void do_execute()
{
    DATA_TYPE res = op_dest->val&op_src->val;
    cpu.CF = 0;
    cpu.OF = 0;

    if(res <= 0)
        cpu.SF = 1;
    else
        cpu.SF = 0;

    if(((res & 0x80) >> 7 ^ (res & 0x40) >> 6 ^ (res & 0x20) >> 5^ (res & 0x10) >> 4 ^ (res & 0x8) >> 3 ^ (res & 0x4) >> 2 ^ (res & 0x2) >> 1 ^ (res & 0x1)) == 0)
    {
        cpu.PF = 1;
    }
    else
        cpu.PF = 0;
    print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)


#include "cpu/exec/template-end.h"
