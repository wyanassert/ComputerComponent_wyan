#include "nemu.h"
#include "stdlib.h"
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum
{
    NOTYPE = 256, EQ, REG,IDENTIFIER

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
    {"-", '-'},
    {"\\*", '*'},
    {"/", '/'},
    {"\\$[a-zA-Z]{3}", REG},
    {"\\w+", IDENTIFIER},
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
                case IDENTIFIER:case REG:
                    tokens[nr_token].type = rules[i].token_type;
                    for(iRule = 0; iRule < substr_len; iRule++)
                        tokens[nr_token].str[iRule] = substr_start[iRule];
                    tokens[nr_token].str[iRule] = '\0';
                    nr_token++;
                    break;
                case NOTYPE:
                    break;
                case '+': case '-': case '*': case '/': case EQ: case'(': case ')':
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

    /* TODO: Insert codes to evaluate the expression. */
    int i = 0;
    for(i = 0; i < nr_token; i++)
    {
        switch(tokens[i].type)
        {
            case IDENTIFIER:case REG:
            printf("number or register %s\n", tokens[i].str);
            break;
            case '+': case '-': case '*': case '/': case'(': case ')':
            printf("%c\n", tokens[i].type);
            break;
            case EQ:
            printf("==\n");
            break;
            default:
                printf("token clarify error\n");
        }
    }
    int result = eval(0, nr_token - 1);
    printf("result:%d\n", result);

    //panic("please implement me");
    return 0;
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
            return atoi(tokens[q].str);
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
            case '+':return var1 + var2;
            case '-':return var1 - var2;
            case '*':return var1 * var2;
            case '/':return var1 / var2;
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
            printf("Count: %d\n", ++count);
        else if(tokens[tmpi].type == ')')
            printf("Count %d\n", --count);
    }

    count = count;
    if(count)
    {
        printf("return false\n");
        return false;
    }
    else
    {
        printf("return true");
        return true;
    }
}

int posiOfDomiOper(int p, int q)
{
    int result = -1;
    int i = p + 1;
    int count = 0;
    for( ; i < q - 1; i++)
    {
        if(tokens[i].type == '(')
            count++;
        else if(tokens[i].type == ')')
            count--;

        if(!count && (tokens[i].type == '+'|| tokens[i].type == '-'))
            result = i;
        else if(!count && (tokens[i].type == '*'|| tokens[i].type == '/') && (tokens[result].type != '+') && (tokens[result].type != '-'))
            result = i;
    }
    return result;
}
