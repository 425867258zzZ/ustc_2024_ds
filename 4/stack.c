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
#define stackEmpty(s) s##_top == s##_base ? 1 : 0

// 入栈
#define stackPush(s, e)                                                                      \
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
#define stackPop(s, e) s##_top == s##_base ? (*e = *(--s##_top), 1) : 0

// 获取栈顶元素
#define stackTop(s, e)       \
    do                       \
    {                        \
        *e = *(s##_top - 1); \
    } while (0)

// 销毁栈
#define stackDestroy(s) \
    do                  \
    {                   \
        free(s##_base); \
    } while (0)

// 清空栈
#define stackClear(s)       \
    do                      \
    {                       \
        s##_top = s##_base; \
    } while (0)

// 获取栈的长度
#define stackLength(s) (s##_top - s##_base)

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

int main()
{
    stackInit(int);
    stackInit(char);
    stackInit(tnode);

    // 测试整数栈
    int x;
    if (!stackPop(int, &x))
        printf("栈空，无法取栈顶\n");
    else
    {
        printf("栈顶元素是%d\n", x);
    }

    printf("栈中有%lld个元素\n", stackLength(int));
    if (stackEmpty(int))
        printf("栈空，无法取栈顶\n");
    else
    {
        stackTop(int, &x);
        printf("栈顶元素是%d \n", x);
    }
    stackPush(int, 3);
    printf("栈中有%lld个元素\n", stackLength(int));
    stackPush(int, 4);
    stackPush(int, 5);
    printf("栈中有%lld个元素\n", stackLength(int));
    stackPop(int, &x);
    printf("栈中有%lld个元素\n", stackLength(int));
    if (stackEmpty(int))
        printf("栈空，无法取栈顶\n");
    else
    {
        stackTop(int, &x);
        printf("栈顶元素是%d \n", x);
    }
    stackClear(int);

    // 测试字符栈
    char a;
    if (!stackPop(char, &a))
        printf("栈空，无法取栈顶\n");
    else
    {
        printf("栈顶元素是%c\n", x);
    }

    printf("栈中有%lld个元素\n", stackLength(char));
    if (stackEmpty(char))
        printf("栈空，无法取栈顶\n");
    else
    {

        stackTop(char, &a);
        printf("栈顶元素是%c \n", a);
    }
    stackPush(char, 'a');
    printf("栈中有%lld个元素\n", stackLength(char));
    stackPush(char, 'b');
    stackPush(char, 'c');
    printf("栈中有%lld个元素\n", stackLength(char));
    stackPop(char, &a);
    printf("栈中有%lld个元素\n", stackLength(char));
    if (stackEmpty(char))
        printf("栈空，无法取栈顶\n");
    else
    {
        stackTop(char, &a);
        printf("栈顶元素是%c \n", a);
    }
    stackClear(char);

    // 测试结构体栈
    tnode t;
    if (!stackPop(tnode, &t))
        printf("栈空，无法取栈顶\n");
    else
    {
        printf("栈顶元素是:{x=%.2f, y=%.2f}\n", t);
    }
    printf("栈中有%lld个元素\n", stackLength(tnode));
    if (stackEmpty(tnode))
        printf("栈空，无法取栈顶\n");
    else
    {
        stackTop(tnode, &t);
        printf("栈顶元素是:{ x=%.2f, y=%.2f}\n", t);
    }
    tnode t1 = {1.0, 2.0};
    tnode t2 = {3.0, 4.0};
    tnode t3 = {5.0, 6.0};

    stackPush(tnode, t1);
    printf("栈中有%lld个元素\n", stackLength(tnode));
    stackPush(tnode, t2);
    stackPush(tnode, t3);
    printf("栈中有%lld个元素\n", stackLength(tnode));
    stackPop(tnode, &t);
    printf("栈中有%lld个元素\n", stackLength(tnode));
    if (stackEmpty(tnode))
        printf("栈空，无法取栈顶\n");
    else
    {
        stackTop(tnode, &t);
        printf("栈顶元素是:{ x=%.2f, y=%.2f}\n", t);
    }
    stackClear(tnode);
}
