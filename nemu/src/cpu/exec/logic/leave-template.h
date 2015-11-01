#include "cpu/exec/template-start.h"

#define instr leave

static void do_execute()
{
    #if DATA_BYTE == 2
        cpu.sp = cpu.bp;
    #else if DATA_BYTE == 4
        cpu.esp = cpu.ebp;
    #endif // DATA_BYTE

     #if DATA_BYTE == 2
        cpu.bp = MEM_R(cpu.sp);
        cpu.bp -=4;
    #else if DATA_BYTE == 4
        cpu.ebp = MEM_R(cpu.esp);
        cpu.esp -= 4;
    #endif // DATA_BYTE

    print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
