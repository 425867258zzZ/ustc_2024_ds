// 基于链表的多项式运算
#include <stdio.h>
#include <stdlib.h>

// 多项式链表，按x的指数升序排列
typedef struct LinkNode
{
    float coef; // 系数
    int expn;   // 指数
    struct LinkNode *next;
} LinkNode;

// 初始化链表
LinkNode *initList()
{
    LinkNode *q = (LinkNode *)malloc(sizeof(LinkNode));
    if (!q)
    {
        printf("头结点分配失败！");
        exit(0);
    }
    q->next = NULL;
    return q;
}

// 链表浅复制
LinkNode *copyList(LinkNode *p)
{
    LinkNode *res = (LinkNode *)malloc(sizeof(LinkNode));
    res->next = NULL;
    LinkNode *cur = p->next;

    while (cur != NULL)
    {
        LinkNode *tmp = (LinkNode *)malloc(sizeof(LinkNode));
        tmp->coef = cur->coef;
        tmp->expn = cur->expn;
        tmp->next = NULL;

        LinkNode *p = res;
        while (p->next != NULL)
        {
            p = p->next;
        }
        p->next = tmp;
        cur = cur->next;
    }
    return res;
}
// 尾插法建立多项式链表
void creatList(LinkNode *q)
{
    float coef = 0;
    int expn = 0;
    LinkNode *tail = q;
    for (int i = 1;; i++)
    {
        printf("请输入第%d项的系数：", i);
        scanf("%f", &coef);
        if (coef != 0)
        {
            printf("请输入第%d项的指数：", i);
            scanf("%d", &expn);
            LinkNode *q = (LinkNode *)malloc(sizeof(LinkNode));
            q->coef = coef;
            q->expn = expn;
            tail->next = q;
            q->next = NULL;
            tail = q;
        }
        else
            break;
    }
    printf("创建成功！\n");
}

// 格式化输出多项式
void printList(LinkNode *p)
{
    LinkNode *q = p->next;
    int flag = 1;
    if (q == NULL)
    {
        printf("0");
    }
    else
    {
        while (q)
        {
            if (q->coef == 0)
            {
                q = q->next;
                continue;
            }
            if (q->coef > 0 && flag != 1)
            {
                printf("+");
            }
            if (q->coef != 1 && q->coef != -1)
            {
                printf("%g", q->coef);
                if (q->expn == 1)
                    putchar('X');
                else if (q->expn != 0)
                    printf("X^%d", q->expn);
            }
            else
            {
                if (q->coef == 1)
                {
                    if (!q->expn)
                    {
                        putchar('1');
                    }
                    else if (q->expn == 1)
                    {
                        putchar('X');
                    }
                    else
                    {
                        printf("X^%d", q->expn);
                    }
                }
                if (q->coef == -1)
                {
                    if (!q->expn)
                    {
                        printf("-1");
                    }
                    else if (q->expn == 1)
                    {
                        printf("-X");
                    }
                    else
                    {
                        printf("-X^%d", q->expn);
                    }
                }
            }
            q = q->next;
            flag++;
        }
    }
    printf("\n");
}

// 将a和b两个多项式相加
LinkNode *addList(LinkNode *a, LinkNode *b)
{
    LinkNode *pa = a->next;
    LinkNode *pb = b->next;
    LinkNode *res = initList();
    LinkNode *tail = res;
    while (pa && pb)
    {
        LinkNode *q = (LinkNode *)malloc(sizeof(LinkNode));
        if (pa->expn > pb->expn)
        {
            q->coef = pb->coef;
            q->expn = pb->expn;
            pb = pb->next;
        }
        else if (pa->expn == pb->expn)
        {
            q->coef = pb->coef + pa->coef;
            q->expn = pb->expn;
            pb = pb->next;
            pa = pa->next;
        }
        else
        {
            q->coef = pa->coef;
            q->expn = pa->expn;
            pa = pa->next;
        }
        tail->next = q;
        q->next = NULL;
        tail = q;
    }
    if (pa == NULL)
        tail->next = pb;
    else
        tail->next = pa;
    return res;
}

// 多项式相减，a-b
LinkNode *mimuList(LinkNode *a, LinkNode *b)
{
    LinkNode *inverse_b = initList();
    LinkNode *tail = inverse_b;
    while (b->next)
    {
        LinkNode *q = (LinkNode *)malloc(sizeof(LinkNode));
        q->coef = -(b->next->coef);
        q->expn = b->next->expn;
        tail->next = q;
        q->next = NULL;
        tail = q;
        b = b->next;
    }
    return addList(a, inverse_b);
}

// 多项式相乘
LinkNode *mutiList(LinkNode *a, LinkNode *b)
{
    LinkNode *pa = a->next;
    LinkNode *res = initList();
    while (pa)
    {
        LinkNode *temp = initList();
        LinkNode *temp_tail = temp;
        LinkNode *pb = b->next;
        while (pb)
        {
            LinkNode *q = (LinkNode *)malloc(sizeof(LinkNode));
            q->coef = pa->coef * pb->coef;
            q->expn = pa->expn + pb->expn;
            temp_tail->next = q;
            q->next = NULL;
            temp_tail = q;
            pb = pb->next;
        }
        res = addList(res, temp);
        pa = pa->next;
    }
    return res;
}

// 反转多项式，使其按x降幂排列,返回链表浅拷贝的反转
LinkNode *reaverseList(LinkNode *p)
{
    LinkNode *p_copy = copyList(p);
    LinkNode *head = p_copy->next;
    LinkNode *pre = NULL;
    LinkNode *cur = NULL;
    while (head)
    {
        cur = head->next;
        head->next = pre;
        pre = head;
        head = cur;
    }
    LinkNode *res = (LinkNode *)malloc(sizeof(LinkNode));
    res->next = pre;
    return res;
}

// 多项式相除a÷b，返回形式如c*b+d，其中c，d为多项式
LinkNode *deviList(LinkNode *a, LinkNode *b)
{
    LinkNode *ra = reaverseList(a);
    LinkNode *rb = reaverseList(b);
    LinkNode *res = (LinkNode *)malloc(sizeof(LinkNode));
    LinkNode *tail = res;
    tail->next = NULL;
    while (ra->next && rb->next)
    {
        if (ra->next->expn >= rb->next->expn)
        {
            LinkNode *tmp = (LinkNode *)malloc(sizeof(LinkNode));
            tmp->coef = ra->coef / rb->coef;
            tmp->expn = ra->expn - rb->expn;
            tail->next = tmp;
            tmp->next = NULL;
            tail = tmp;
        }
        ra = mimuList(ra, mutiList(res, rb));
    }
    return NULL;
}

int main()
{
    LinkNode *a = initList();
    LinkNode *b = initList();
    creatList(a);
    printf("a:");
    printList(a);

    creatList(b);
    printf("b:");
    printList(b);

    printf("a+b:");
    printList(addList(a, b));

    printf("a*b:");
    printList(mutiList(a, b));

    printf("a-b:");
    printList(mimuList(a, b));
}