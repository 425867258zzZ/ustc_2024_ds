### 2.1

BABAD BCABD CDDBC

### 2.2.10

```c
#include <stdio.h>

#define ElemType int
#define N 9

void deleteItem(ElemType A[], int n, ElemType item)
{
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        if (A[i] == item)
        {
            count++;
        }
        else
        {
            A[i - count] = A[i];
        }
    }
    for (int k = n - count; k < n; k++)
    {
        A[k] = 0;
    }
}

void printArray(ElemType *A, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", A[i]);
    }
    printf("\n");
}

int main()
{
    ElemType A[N] = {2,1,2,3,4,2,2,5,6};
    ElemType item = 2;
    deleteItem(A, N, item);
    printArray(A, N);
}

```

输出结果:{1 3 4 5 6 0 0 0 0}
