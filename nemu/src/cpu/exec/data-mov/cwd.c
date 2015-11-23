#include "cwd.h"
#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper_v(cwd)

make_helper(cwd_w)
{
    if (((int16_t)cpu.gpr[0]._16) < (int16_t)0)
        cpu.gpr[2]._16 = 0xffff;
    else
        cpu.gpr[2]._16 = 0;
    print_asm("cwd");
    return 1;
}

make_helper(cwd_l)
{
    if (((int32_t)cpu.eax) < (int32_t)0)
        cpu.edx = 0xffffffff;
    else
        cpu.edx = 0;
    print_asm("cdq");
    return 1;
}
