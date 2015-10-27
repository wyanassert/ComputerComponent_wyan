#include "cpu/exec/template-start.h"
#include "cpu/reg.h"
#define instr cmp

static void do_execute () {
	DATA_TYPE result = op_dest->val - op_src->val;


	int dest=(int)op_dest->val;
	int src=(int)op_src->val;
	int result_flag=dest-src;
	int PF_flag=0;


	if(result>0)
        cpu.SF=0;
    else
        if(result<=0)
            cpu.SF=1;

   if(result==0){
        cpu.ZF=1;
    }
    else cpu.ZF=0;

    if((dest>0&&src<0&&result_flag<0)||(dest<0&&src>0&&result_flag>0))
        cpu.OF=1;
    else cpu.OF=0;


    if((unsigned)op_dest->val<(unsigned)op_src->val)
        cpu.CF=1;
        else cpu.CF=0;

    if(result&0b1)
        PF_flag++;
    if(result&0b10)
        PF_flag++;
    if(result&0b100)
        PF_flag++;
    if(result&0b1000)
        PF_flag++;
    if(result&0b10000)
        PF_flag++;
    if(result&0b100000)
        PF_flag++;
    if(result&0b1000000)
        PF_flag++;
    if(result&0b10000000)
        PF_flag++;
    if(PF_flag%2==0)
       cpu.PF=1;
    else cpu.PF=0;

   if((unsigned)(op_dest->val&0xF)<(unsigned)(op_src->val&0xF))
        cpu.AF=1;
  else cpu.AF=0;




	/* TODO: Update cpuS. */

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
