#include "cpu/exec/template-start.h"
#include "cpu/reg.h"
#define instr je

static void do_execute(){
    if(cpu.ZF==1)
        cpu.eip+=op_src->val;
    print_asm_template1();



}


make_instr_helper(i)
#include "cpu/exec/template-end.h"