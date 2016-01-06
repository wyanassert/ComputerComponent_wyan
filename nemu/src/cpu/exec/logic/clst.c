#include "clst.h"
#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(cld)
{
    cpu.DF = 0;
    print_asm("cld");
    return 1;
}
