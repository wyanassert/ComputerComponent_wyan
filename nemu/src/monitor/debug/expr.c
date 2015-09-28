#include "nemu.h"
#include "stdlib.h"
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

extern CPU_state cpu;

enum
{
    NOTYPE = 256, EQ, UEQ, AND, OR,NOT, REG,IDENTIFIER

    /* TODO: Add more token types */

};

static struct rule
{
    char *regex;
    int token_type;
} rules[] =
{

    /* TODO: Add more rules.
     * Pay attention to the precedence level of different rules.
     */

    {" +",	NOTYPE},				// spaces
    {"\\+", '+'},					// plus
    {"==", EQ},						// equal
   // {"!=", UEQ},
   // {"&&", AND},
   // {"||", OR},
   // {"!", NOT},
    {"-", '-'},
    {"\\*", '*'},
    {"/", '/'},
    {"\\$[a-zA-Z]{2,3}", REG},
    {"\\w+|0x[a-fA-F0-9]+", IDENTIFIER},
    {"\\(",'('},
    {"\\)",')'}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
    int i;
    char error_msg[128];
    int ret;

    for(i = 0; i < NR_REGEX; i ++)
    {
        ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
        if(ret != 0)
        {
            regerror(ret, &re[i], error_msg, 128);
            Assert(ret != 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
        }
    }
}

typedef struct token
{
    int type;
    char str[32];
} Token;

Token tokens[32];
int nr_token;


bool check_parentheses(int p, int q);
int eval(int p, int q);
int posiOfDomiOper(int p, int q);
int intFromReg(char *reg);
int rankOfOper(int oper);

static bool make_token(char *e)
{
    int position = 0;
    int i = 0;
    regmatch_t pmatch;

    nr_token = 0;
    while(e[position] != '\0')
    {
        /* Try all rules one by one. */
        for(i = 0; i < NR_REGEX; i ++)
        {
            if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0)
            {
                char *substr_start = e + position;
                int substr_len = pmatch.rm_eo;
                //Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
                position += substr_len;

                /* TODO: Now a new token is recognized with rules[i]. Add codes
                 * to record the token in the array ``tokens''. For certain
                 * types of tokens, some extra actions should be performed.
                 */
                int iRule = 0;
                switch(rules[i].token_type)
                {
                case IDENTIFIER:
                case REG:
                    tokens[nr_token].type = rules[i].token_type;
                    for(iRule = 0; iRule < substr_len; iRule++)
                        tokens[nr_token].str[iRule] = substr_start[iRule];
                    tokens[nr_token].str[iRule] = '\0';
                    nr_token++;
                    break;
                case NOTYPE:
                    break;
                case '+':
                case '-':
                case '*':
                case '/':
                case EQ:
                case UEQ:
                case AND:
                case OR:
                case NOT:
                case'(':
                case ')':
                    tokens[nr_token].type = rules[i].token_type;
                    nr_token++;
                    break;
                default:
                    panic("please implement me, make_token()  switch{}");
                }

                break;
            }
        }

        if(i == NR_REGEX)
        {
            printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
            return false;
        }
    }
    return true;
}

uint32_t expr(char *e, bool *success)
{
    if(!make_token(e))
    {
        *success = false;
        printf("Error:make_token() func run error\n");
        return 0;
    }
    *success = true;
    /* TODO: Insert codes to evaluate the expression. */
    /*int i = 0;
    for(i = 0; i < nr_token; i++)
    {
        switch(tokens[i].type)
        {
        case IDENTIFIER:
        case REG:
            printf("number or register %s\n", tokens[i].str);
            break;
        case '+':
        case '-':
        case '*':
        case '/':
        case'(':
        case ')':
            printf("%c\n", tokens[i].type);
            break;
        case EQ:
            printf("==\n");
            break;
        default:
            printf("token clarify error\n");
        }
    }*/
    int result = eval(0, nr_token - 1);

    //panic("please implement me");
    return result;
}


int eval(int p, int q)
{
    if(p > q)
    {
        printf("Error:bad expersion!\n");
        return 0;
    }
    else if(p == q)
    {
        if(tokens[q].type == IDENTIFIER || tokens[q].type == REG)
        {
            if(tokens[q].type == IDENTIFIER)
            {
                if(tokens[q].str[0] == '0' && tokens[q].str[1] == 'x')
                {
                    int i = strlen(tokens[q].str) - 1;
                    int tmp16 = 1;
                    int tmpResult = 0;
                    for( ; i > 1; i--)
                    {
                        char c = tokens[q].str[i];
                        if(c >= '0' && c <= '9')
                            c = c- '0';
                        if(c >= 'a' && c <= 'f')
                            c = c - 'a' + 10;
                        if(c >= 'A' && c <= 'F')
                            c = c -'A' + 10;
                        tmpResult = tmpResult + c * tmp16;
                        tmp16 *= 16;
                    }
                    return tmpResult;
                }
                return atoi(tokens[q].str);
            }
            else
            {
                return intFromReg(tokens[q].str + 1);
            }
        }
        else
        {
            printf("Error:while p == q, result should be a number\n");
            return 0;
        }
    }
    else if(check_parentheses(p, q))
    {
        return eval(p + 1, q - 1);
    }
    else
    {
        //we should do more here
        int opPosi = posiOfDomiOper(p, q);
        if(opPosi == -1)
        {
            printf("Error : dominant operator find error\n");
        }
        int var1 = eval(p, opPosi - 1);
        int var2 = eval(opPosi + 1, q);
        switch(tokens[opPosi].type)
        {
        case '+':
            return var1 + var2;
        case '-':
            return var1 - var2;
        case '*':
            return var1 * var2;
        case '/':
            return var1 / var2;
        }
        return 0;
    }
}

bool check_parentheses(int p, int q)
{
    if(tokens[p].type != '(' || tokens[q].type != ')')
        return false;
    int count = 0;
    int tmpi = p;
    for(; tmpi <= q; tmpi++)
    {
        printf("%d %s\n", tmpi, tokens[tmpi].str);
        if(tokens[tmpi].type == '(')
            ++count;
        else if(tokens[tmpi].type == ')')
            --count;
    }

    count = count;
    if(count)
    {
        return false;
    }
    else
    {
        return true;
    }
}

int posiOfDomiOper(int p, int q)
{
    int result = -1;
    int i = p;
    int count = 0;
    int oldRank = -1;
    for( ; i <= q; i++)
    {
        if(tokens[i].type == '(')
            count++;
        else if(tokens[i].type == ')')
            count--;

        if(rankOfOper(tokens[i].type) >= oldRank)
        {
            result = i;
            oldRank = rankOfOper(tokens[i].type);
        }
    }
    return result;
}

int intFromReg(char *reg)
{
    char *tmpC;
    int i = 0;
    if(strlen(reg) == 2)
    {
        i = 1;
        tmpC = reg;
    }
    else
    {
        i = 0;
        tmpC = reg + 1;
    }

    if(*(tmpC) == 'a' || *(tmpC) == 'A')
    {
        if(*(tmpC + 1) == 'x' || *(tmpC + 1) == 'X')
        {
            if(i)//ax
                return cpu.gpr[0]._16;
            else//eax
                return cpu.eax;
        }
        else if(*(tmpC + 1) == 'h' || *(tmpC + 1) == 'H')//ah
        {
            return cpu.gpr[0]._8[1];
        }
        else if(*(tmpC + 1) == 'l' || *(tmpC + 1) == 'L')//al
        {
            return cpu.gpr[0]._8[0];
        }
        else
        {
            printf("Error:input register error!\n");
            return 0;
        }
    }
    else if(*(tmpC) == 'c' || *(tmpC) == 'C')
    {
        if(*(tmpC + 1) == 'x'|| *(tmpC + 1) == 'X')
        {
            if(i)//cx
                return cpu.gpr[1]._16;
            else//ecx
                return cpu.ecx;
        }
        else if(*(tmpC + 1) == 'h' || *(tmpC + 1) == 'H')//ch
        {
            return cpu.gpr[1]._8[1];
        }
        else if(*(tmpC + 1) == 'l' || *(tmpC + 1) == 'L')//cl
        {
            return cpu.gpr[1]._8[0];
        }
        else
        {
            printf("Error:input register error!\n");
            return 0;
        }
    }
    else if(*(tmpC) == 'd' || *(tmpC) == 'D')
    {
        if(*(tmpC + 1) == 'x'||*(tmpC + 1) == 'X')
        {
            if(i)//dx
                return cpu.gpr[2]._16;
            else//edx
                return cpu.edx;
        }
        else if(*(tmpC + 1) == 'h' || *(tmpC + 1) == 'H')//dh
        {
            return cpu.gpr[2]._8[1];
        }
        else if(*(tmpC + 1) == 'l' || *(tmpC + 1) == 'L')//dl
        {
            return cpu.gpr[2]._8[0];
        }
        else if(*(tmpC + 1) == 'i' || *(tmpC + 1) == 'I')
        {
            if(i)//di
                return cpu.gpr[7]._16;
            else//edi
                return cpu.edi;
        }
        else
        {
            printf("Error:input register error!\n");
            return 0;
        }
    }
    else if(*(tmpC) == 'b' || *(tmpC) == 'B')
    {
        if(*(tmpC + 1) == 'x'||*(tmpC + 1) == 'X')
        {
            if(i)//bx
                return cpu.gpr[3]._16;
            else//ebx
                return cpu.ebx;
        }
        else if(*(tmpC + 1) == 'h' || *(tmpC + 1) == 'H')//bh
        {
            return cpu.gpr[3]._8[1];
        }
        else if(*(tmpC + 1) == 'l' || *(tmpC + 1) == 'L')//bl
        {
            return cpu.gpr[3]._8[0];
        }
        else if(*(tmpC + 1) == 'p' || *(tmpC + 1) == 'P')
        {
            if(i)
                return cpu.gpr[5]._16;
            else
                return cpu.ebp;
        }
        else
        {
            printf("Error:input register error!\n");
            return 0;
        }
    }
    else if(*(tmpC) == 's' || *(tmpC) == 'S')
    {
        if(*(tmpC + 1) == 'p'||*(tmpC + 1) == 'P')
        {
            if(i)//sp
                return cpu.gpr[4]._16;
            else//esp
                return cpu.esp;
        }
        else if(*(tmpC + 1) == 'i'||*(tmpC + 1) == 'I')
        {
            if(i)//si
                return cpu.gpr[6]._16;
            else//esi
                return cpu.esi;
        }
        else
        {
            printf("Error:input register error!\n");
            return 0;
        }
    }
    else if(*(tmpC) == 'i' || *(tmpC) == 'I')
    {
            return cpu.eip;
    }
    else
    {
        printf("Error:input register error!\n");
        return 0;
    }

}

int rankOfOper(int oper)
{
    switch(oper)
    {
        case OR: return 0;
        case AND: return 1;
        case UEQ: return 2;
        case EQ: return 3;
        case '+': case '-': return 4;
        case '*': case '/': return 5;
        case NOT: return 6;
        default:
            return -2;
    }
}
