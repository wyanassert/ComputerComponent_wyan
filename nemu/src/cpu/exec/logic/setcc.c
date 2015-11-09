#include "setcc.h"
#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_setcc(seta, !cpu.CF && !cpu.ZF)
make_setcc(setae, !cpu.CF)
make_setcc(setb, cpu.CF)
make_setcc(setbe, cpu.CF || cpu.ZF)
make_setcc(sete, cpu.ZF)
make_setcc(setg, !cpu.ZF && (cpu.SF == cpu.OF))
make_setcc(setge, cpu.SF == cpu.OF)
make_setcc(setl, cpu.SF != cpu.OF)
make_setcc(setle, cpu.ZF || (cpu.SF != cpu.OF))
make_setcc(setne, !cpu.ZF)
make_setcc(setno, !cpu.OF)
make_setcc(setnp, !cpu.PF)
make_setcc(setns, !cpu.SF)
make_setcc(seto, cpu.OF)
make_setcc(setp, cpu.PF)
make_setcc(sets, cpu.SF)

