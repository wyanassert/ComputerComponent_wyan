#include "cpu/exec/template-start.h"
#include "cpu/reg.h"
#define instr pop

static void do_execute(){

  if(DATA_BYTE==1){
    OPERAND_W(op_dest,MEM_R(cpu.esp));
    cpu.esp+=2;
}

if(DATA_BYTE==2){
    OPERAND_W(op_dest,MEM_R(cpu.esp));
    cpu.esp+=2;
}
if(DATA_BYTE==4){
OPERAND_W(op_dest,MEM_R(cpu.esp));
cpu.esp+=4;
}
print_asm_template1();


}

make_instr_helper(r)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
