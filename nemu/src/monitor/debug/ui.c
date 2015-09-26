#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

extern CPU_state cpu;
void cpu_exec(uint32_t);

/* We use the ``readline'' library to provide more flexibility to read from stdin. */
char* rl_gets()
{
    static char *line_read = NULL;

    if (line_read)
    {
        free(line_read);
        line_read = NULL;
    }

    line_read = readline("(nemu) ");

    if (line_read && *line_read)
    {
        add_history(line_read);
    }

    return line_read;
}

static int cmd_c(char *args)
{
    cpu_exec(-1);
    return 0;
}

static int cmd_q(char *args)
{
    return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args)
{
    int n = atoi(args);
    cpu_exec(n);
    return 0;
}

static int cmd_info(char *args)
{
    if(strlen(args) == 1 && args[0] == 'r')
    {
        printf("exa:%x  ax:%x  ah:%x  al:%x\n", cpu.eax, cpu.gpr[0]._16, cpu.gpr[0]._8[1], cpu.gpr[0]._8[0]);
        printf("ecx:%x  cx:%x  ch:%x  cl:%x\n", cpu.ecx, cpu.gpr[1]._16, cpu.gpr[1]._8[1], cpu.gpr[1]._8[0]);
        printf("edx:%x  dx:%x  dh:%x  dl:%x\n", cpu.edx, cpu.gpr[2]._16, cpu.gpr[2]._8[1], cpu.gpr[2]._8[0]);
        printf("ebx:%x  bx:%x  bh:%x  bl:%x\n", cpu.ebx, cpu.gpr[3]._16, cpu.gpr[3]._8[1], cpu.gpr[3]._8[0]);
        printf("esp:%x  sp:%x\n", cpu.esp, cpu.gpr[4]._16);
        printf("ebp:%x  bp:%x\n", cpu.ebp, cpu.gpr[5]._16);
        printf("esi:%x  si:%x\n", cpu.esi, cpu.gpr[6]._16);
        printf("edi:%x  di:%x\n", cpu.edi, cpu.gpr[7]._16);
        printf("eip:%x\n", cpu.eip);
        return 0;
    }
    else if(strlen(args) == 1 && args[0] == 'w')
    {
        return 0;
    }
    else
    {
        printf("ETRROR: unexpected input!!!");
        return 0;
    }
}

static int cmd_p(char *args)
{
    return 0;
}

static int cmd_x(char *args)
{
    printf("%s\n", args);
    int N = 0;
    int tmpAddr;
    char *tmpCharP = NULL;
    tmpCharP = strtok(args, " ");
    if(tmpCharP)
    {
        N = atoi(tmpCharP);
    }
    tmpCharP = strtok(NULL, " ");
    if(tmpCharP)
    {
        if(strlen(tmpCharP) > 1)
        {
            if(tmpCharP[0] == '0' && tmpCharP[1] == 'x')
            {
                tmpCharP += 2;
            }
        }
        tmpAddr = atoi(tmpCharP);
        printf("N:%d, addr:%d\n", N, tmpAddr);
    }
    else
    {
        printf("Error: input error!!\n");
    }
    return 0;
}

static struct
{
    char *name;
    char *description;
    int (*handler) (char *);
} cmd_table [] =
{
    { "help", "Display informations about all supported commands", cmd_help },
    { "c", "Continue the execution of the program", cmd_c },
    { "q", "Exit NEMU", cmd_q },
    { "si", "single step", cmd_si},
    { "info", "print state information of program", cmd_info},
    {"p", "work out the value of an experssion", cmd_p},
    {"x", "swap the memory", cmd_x},
    /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args)
{
    /* extract the first argument */
    char *arg = strtok(NULL, " ");
    int i;

    if(arg == NULL)
    {
        /* no argument given */
        for(i = 0; i < NR_CMD; i ++)
        {
            printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        }
    }
    else
    {
        for(i = 0; i < NR_CMD; i ++)
        {
            if(strcmp(arg, cmd_table[i].name) == 0)
            {
                printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
                return 0;
            }
        }
        printf("Unknown command '%s'\n", arg);
    }
    return 0;
}



void ui_mainloop()
{
    while(1)
    {
        char *str = rl_gets();
        char *str_end = str + strlen(str);

        /* extract the first token as the command */
        char *cmd = strtok(str, " ");
        if(cmd == NULL)
        {
            continue;
        }

        /* treat the remaining string as the arguments,
         * which may need further parsing
         */
        char *args = cmd + strlen(cmd) + 1;
        if(args >= str_end)
        {
            args = NULL;
        }

#ifdef HAS_DEVICE
        extern void sdl_clear_event_queue(void);
        sdl_clear_event_queue();
#endif


        int i;
        for(i = 0; i < NR_CMD; i ++)
        {
            if(strcmp(cmd, cmd_table[i].name) == 0)
            {
                if(cmd_table[i].handler(args) < 0)
                {
                    return;
                }
                break;
            }
        }

        if(i == NR_CMD)
        {
            printf("Unknown command '%s'\n", cmd);
        }
    }
}
