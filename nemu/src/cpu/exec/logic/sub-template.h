#include "cpu/exec/template-start.h"
#if DATA_BYTE == 1
#define RET_DATA_TYPE int16_t
#if DATA_BYTE == 2
#define RET_DATA_TYPE int32_t
#if DATA_BYTE == 4
#define RET_DATA_TYPE int64_t
#endif // DATA_BYTE

#define instr sub
extern CPU_state cpu;

static void do_execute () {
    RET_DATA_TYPE val1 = op_dest->val;
    RET_DATA_TYPE val2 = op_src->val;
    RET_DATA_TYPE res = val1 - val2;
	OPERAND_W(op_dest, res);

	if((((int) val1) > 0 && ((int) val2) < 0 && ((int) res) < 0) || (((int) val1) < 0 && ((int) val2) > 0 && ((int) res) > 0))
	{
        cpu.OF = 1;
	}

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
