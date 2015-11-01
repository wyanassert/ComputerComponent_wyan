#include "cpu/exec/template-start.h"
#include "cpu/reg.h"
#define instr jbe

static void do_execute(){
    if(cpu.CF==1||cpu.ZF==1){
        cpu.eip+=op_src->val;
        cpu.eip-=0x100;}
    print_asm_template1();



}


make_instr_helper(i)
#include "cpu/exec/template-end.h"
