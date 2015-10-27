#include "cpu/exec/template-start.h"

#define instr call

static void do_execute()
{
    #if DATA_BYTE == 2
    cpu.esp -= 4;
    MEM_W(cpu.esp, cpu.eip);
    cpu.eip = (op_src->val + cpu.eip) & 0x0000ffff;
    #else
    cpu.esp-=4;
    MEM_W(cpu.esp, cpu.eip);
    cpu.eip+=op_src->val;
    #endif // DATA_BYTE
    print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
