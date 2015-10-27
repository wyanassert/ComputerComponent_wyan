#include "cpu/exec/template-start.h"

#define instr push

static void do_execute()
{
    #if DATA_BYTE == 1
    cpu.esp -= 2;
    MEM_W(cpu.esp, op_src->val);
    #else
    cpu.esp -= 4;
    MEM_W(cpu.esp, op_src->val);
    #endif // DATA_BYTE

    print_asm_template1();
}

make_instr_helper(r)


#include "cpu/exec/template-end.h"