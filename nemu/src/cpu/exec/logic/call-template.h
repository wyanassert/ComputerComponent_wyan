#include "cpu/exec/template-start.h"

#define instr call

static void do_execute()
{
    int len=concat(decode_i_, SUFFIX)(cpu.eip + 1);
	if(2 == DATA_BYTE){
		cpu.esp =cpu.esp - 2;
		MEM_W(cpu.esp, (uint16_t)((cpu.eip + len) & 0x0000ffff));

		cpu.eip = (cpu.eip + op_src->val)&0x0000ffff;
	}
	else if(4 == DATA_BYTE){
		cpu.esp = cpu.esp-4;
		MEM_W(cpu.esp,  cpu.eip + len);
		cpu.eip = cpu.eip+ op_src->val;
	}
	print_asm_template1();
   /* #if DATA_BYTE == 2
    cpu.esp -= 4;
    MEM_W(cpu.esp, cpu.eip);
    cpu.eip = (op_src->val + cpu.eip) & 0x0000ffff;
    #else
    cpu.esp-=4;
    MEM_W(cpu.esp, cpu.eip);
    cpu.eip+=op_src->val;
    #endif // DATA_BYTE
    print_asm_template1();*/
}

make_instr_helper(i)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
