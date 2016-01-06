#include "cbw.h"
#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper_v(cbw)

make_helper(cbw_w)
{
    if (((int8_t)cpu.gpr[0]._8[0]) < (int8_t)0)
        cpu.gpr[0]._8[1] = 0xff;
    else
        cpu.gpr[0]._8[1] = 0;
    print_asm("cbw");
    return 1;
}

make_helper(cbw_l)
{
    int32_t tmp = ((int16_t)cpu.gpr[0]._16);
    cpu.eax = tmp;
    print_asm("cwde");
    return 1;
}
