#include <stdio.h>
#include <stdlib.h>
#include "stdint.h"

typedef uint32_t swaddr_t;

typedef struct
{
    union
    {
        union
        {
            uint32_t _32;
            uint16_t _16;
            uint8_t _8[2];
        } gpr[8];

        //Do NOT change the order of the GPRs' definitions.

        struct
        {
            uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
        };
    };

    swaddr_t eip;
} CPU_state;

int main()
{
    CPU_state cpu;

    cpu.eax = 0x76543210;
    printf("%x, %x, %x, %x", cpu.gpr[0]._8[0], cpu.gpr[0]._8[1], cpu.gpr[0]._16, cpu.gpr[0]._32);

    return 0;
}
