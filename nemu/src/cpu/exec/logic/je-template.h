#include "cpu/exec/template-start.h"

#define instr je

static void do_execute(){
    uint32_t result = cpu.eip + (int32_t)op_src->val;
    if(DATA_BYTE == 2){
    	cpu.eip = result & 0x0000FFFF;
    }
    else
        cpu.eip = result;
	print_asm_template1();
}


make_instr_helper(i)
#include "cpu/exec/template-end.h"
