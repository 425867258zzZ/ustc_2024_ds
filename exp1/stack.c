// 栈ADT实现
#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 100
#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10

// 初始化栈
#define stackInit(s)                                         \
    do                                                       \
    {                                                        \
        s##_base = (s *)malloc(STACK_INIT_SIZE * sizeof(s)); \
        if (!s##_base)                                       \
        {                                                    \
            printf("malloc error\n");                        \
            exit(0);                                         \
        }                                                    \
        s##_top = s##_base;                                  \
        s##_stacksize = STACK_INIT_SIZE;                     \
    } while (0)

// 判断栈是否为空
#define isEmpty(s) s##_top == s##_base ? 1 : 0

// 入栈
#define Push(s, e)                                                                      \
    do                                                                                       \
    {                                                                                        \
        if (s##_top - s##_base >= s##_stacksize)                                             \
        {                                                                                    \
            s##_base = (s *)realloc(s##_base, (s##_stacksize + STACKINCREMENT) * sizeof(s)); \
            if (!s##_base)                                                                   \
                exit(0);                                                                     \
            s##_top = s##_base + s##_stacksize;                                              \
            s##_stacksize += STACKINCREMENT;                                                 \
        }                                                                                    \
        *(s##_top++) = e;                                                                    \
    } while (0)

// 出栈
#define Pop(s, e) s##_top == s##_base ? (*e = *(--s##_top), 1) : 0

// 获取栈顶元素
#define getTop(s, e)       \
    do                       \
    {                        \
        *e = *(s##_top - 1); \
    } while (0)

// 销毁栈
#define Destroy(s) \
    do                  \
    {                   \
        free(s##_base); \
    } while (0)

// 清空栈
#define Clear(s)       \
    do                      \
    {                       \
        s##_top = s##_base; \
    } while (0)

// 获取栈的长度
#define Length(s) (s##_top - s##_base)

unsigned char_stacksize;
char *char_top;
char *char_base;

unsigned int_stacksize;
int *int_top;
int *int_base;

typedef struct node
{
    float x, y;
} tnode;
unsigned tnode_stacksize;
tnode *tnode_top;
tnode *tnode_base;
