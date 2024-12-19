// 链表ADT实现
#include <stdio.h>
#include <stdlib.h>

#define ElemType int
#define _DEBUG_ 1

typedef struct Lnode
{
    ElemType data;
    struct Lnode *next;
} LinkNode;


LinkNode *first;

LinkNode *initList()
{
    LinkNode *p = (LinkNode *)malloc(sizeof(LinkNode));
    if (!p)
    {
        printf("头结点分配失败！");
        exit(0);
    }
    p->next = NULL;
    return p;
}

#ifdef _DEBUG_
LinkNode head;

void initList1(LinkNode *p)
{
    p = (LinkNode *)malloc(sizeof(LinkNode));
    if (!p)
    {
        printf("头结点分配失败！");
        exit(0);
    }
    p->next = NULL;
}

void initList2()
{
    first = &head;
    first->next = NULL;
}
#endif

int listLength(LinkNode *p)
{
    /*
    返回链表的长度
    */
    int count = 0;
    while (p->next != NULL)
    {
        count++;
        p = p->next;
    }
    return count;
}

int listEmpty(LinkNode *p)
{
    /*
    返回链表是否为空，1为空，0非空
    */
    if (p->next == NULL)
        return 1;
    return 0;
}

void clearList(LinkNode *p)
{
    /*
    清除链表
    */
    LinkNode *tmp;
    while (p->next != NULL)
    {
        tmp = p->next;
        p->next = tmp->next;
        free(tmp);
    }
}

void destoryList(LinkNode *p)
{
    /*
    连同哨兵节点一起销毁链表
    */
    clearList(p);
    free(p);
}

int getElem(LinkNode *p, int i, ElemType *e)
{
    /*
    获取链表的第i个元素，并储存在*e中,若i不合法则返回0
    */
    if (i >= 1 && i < listLength(p))
    {
        int index = 0;
        while (p->next)
        {
            p = p->next;
            index++;
            if (index == i)
            {
                *e = p->data;
                return index;
            }
        }
    }
    return 0;
}

LinkNode *locateElem(LinkNode *p, ElemType e)
{
    /*
    返回指向链表中等于e的节点，若没有则返回NULL
    */
    while (p->next != NULL)
    {
        p = p->next;
        if (p->data == e)
            return p;
    }
    return NULL;
}

// 返回结点cur_e的前驱结点
LinkNode *priorElem(LinkNode *p, LinkNode *cur_e)
{
    while (p->next != NULL)
    {
        p = p->next;
        if (p->next == cur_e)
        {
            return p;
        }
    }
    return NULL;
}

// 返回节点cur_e的后继结点
LinkNode *nextElem(LinkNode *p, LinkNode *cur_e)
{
    while (p->next != NULL)
    {
        p = p->next;
        if (p == cur_e)
            return p->next;
    }
    return NULL;
}

// 在链表的第i个节点前插入元素e
LinkNode *listInsert(LinkNode *p, int i, ElemType e)
{
    if (i >= 1 && i <= listLength(p) + 1)
    {
        for (int k = 0; k < i - 1; k++)
        {
            p = p->next;
        }

        LinkNode *q = (LinkNode *)malloc(sizeof(LinkNode));
        if (!q)
        {
            printf("结点分配失败！");
            exit(0);
        }
        q->data = e;
        q->next = p->next;
        p->next = q;
        return q;
    }
    return NULL;
}

// 删除链表的第i个节点，并将删除的节点的值储存在*e中
int listDelete(LinkNode *p, int i, ElemType *e)
{
    if (i >= 1 && i <= listLength(p))
    {
        for (int k = 0; k < i - 1; k++)
        {
            p = p->next;
        }
        LinkNode *tmp = p->next;
        p->next = tmp->next;
        *e = tmp->data;
        free(tmp);
        return 1;
    }
    return 0;
}

//  遍历链表，对每个节点执行函数ptrFunc
void listTraverse(LinkNode *p, void (*ptrFunc)(LinkNode *))
{
    while (p->next)
    {
        p = p->next;
        ptrFunc(p);
    }
}

void debugListNode(LinkNode *p)
{
    if (p->next == NULL)
        printf("%d->NULL\n", p->data);
    else
        printf("%d->", p->data);
}

void add2(LinkNode *p)
{
    p->data += 2;
}
int main()
{
    void (*ptrFunc)(LinkNode *) = debugListNode;

    first = initList();
    listInsert(first, 1, 2);
    listInsert(first, 1, 3);
    listInsert(first, 1, 4);
    listInsert(first, 1, 5);

    listTraverse(first, ptrFunc);

    listInsert(first, 1, 6);
    listInsert(first, 1, 7);
    listInsert(first, 1, 8);
    listInsert(first, 1, 9);

    listTraverse(first, ptrFunc);

    listInsert(first, 3, 10);
    listInsert(first, 5, 11);
    listInsert(first, 7, 12);
    listInsert(first, 9, 13);

    listTraverse(first, ptrFunc);

    ElemType ei;
    printf("取之前数据%d\n", ei);
    getElem(first, 10, &ei);
    printf("取之后数据%d\n", ei);

    LinkNode *p = locateElem(first, 10);
    if (!p)
    {
        printf("未找到\n");
    }
    else
    {
        p = priorElem(first, p);
        printf("前驱为%d\n", p->data);
        printf("节点为%d\n", p->next->data);
        if (p->next->next)
        {
            printf("后继为%d\n", p->next->next->data);
        }
    }

    printf("删除前的值为%d\n", ei);
    if (listDelete(first, 10, &ei))
    {
        printf("删除后的值为%d\n", ei);
    }
    else
    {
        printf("删除失败\n");
    }
    listTraverse(first, ptrFunc);

    ptrFunc = add2;
    listTraverse(first, ptrFunc);
    printf("完成+2后的链表：\n");
    listTraverse(first, debugListNode);
    return 0;
}