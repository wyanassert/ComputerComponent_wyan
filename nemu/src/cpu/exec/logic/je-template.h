#include "cpu/exec/template-start.h"

#define instr je

static void do_execute(){
    if(cpu.ZF==1){
        #if DATA_BYTE==1
            cpu.eip+=(DATA_TYPE_S)op_src->val;
        #elif DATA_BYTE==2
            cpu.eip=cpu.eip+(DATA_TYPE_S)op_src->val;
            cpu.eip=cpu.eip&0xFFFF;
        #else
            cpu.eip+=(int32_t)op_src->val;
        #endif // DATA_BYTE
    }

	print_asm_template1();
}


make_instr_helper(i)
#include "cpu/exec/template-end.h"
