#include "cpu/exec/template-start.h"
#define instr sub

static void do_execute () {
    int val1 = op_dest->val;
    int val2 = op_src->val;
    int res = val1 - val2;
	OPERAND_W(op_dest, res);

	if((((int) val1) > 0 && ((int) val2) < 0 && ((int) res) < 0) || (((int) val1) < 0 && ((int) val2) > 0 && ((int) res) > 0))
	{
        cpu.OF = 1;
	}
	else
        cpu.OF = 0;
    if(res <= 0)
        cpu.SF = 1;
    else
        cpu.SF = 0;
    if(res == 0)
        cpu.ZF = 1;
    else
        cpu.ZF = 0;

    if((val1 & 0xf) - (val2 & 0xf) < 0)
        cpu.AF = 0;
    else
        cpu.AF = 1;

    if(((res & 0x80) >> 7 ^ (res & 0x40) >> 6 ^ (res & 0x20) >> 5^ (res & 0x10) >> 4 ^ (res & 0x8) >> 3 ^ (res & 0x4) >> 2 ^ (res & 0x2) >> 1 ^ (res & 0x1)) == 0)
    {
        cpu.PF = 1;
    }
    else
        cpu.PF = 0;
    if(((unsigned) val1) >= ((unsigned) val2))
        cpu.PF = 1;
    else
        cpu.PF = 0;

    if(((unsigned) val1) >= ((unsigned) val2))
        cpu.CF = 0;
    else
        cpu.CF = 1;
    print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
