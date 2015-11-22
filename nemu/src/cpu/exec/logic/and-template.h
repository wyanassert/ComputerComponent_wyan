#include "cpu/exec/template-start.h"

#define instr and

static void do_execute () {
	DATA_TYPE result = op_dest->val & op_src->val;
	printf("%d,\n",op_src->size);
    OPERAND_W(op_dest, result);

    cpu.CF = 0;
    cpu.OF = 0;

#if DATA_BYTE == 1
    cpu.SF = (result >> 7) & 0x1;
#endif // DATA_BYTE
#if DATA_BYTE == 2
    cpu.SF = (result >> 15) & 0x1;
#endif // DATA_BYTE
#if DATA_BYTE == 4
    cpu.SF = (result >> 31) & 0x1;
#endif // DATA_BYTE
    cpu.ZF = (result == 0);
    cpu.PF = (result ^ (result >> 1) ^ (result >> 2) ^ (result >> 3) ^ (result >> 4) ^ (result >> 5) ^ (result >> 6) ^ (result >> 7)) & 0x01;

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
