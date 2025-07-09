// SJF

#include <stdio.h>

struct node
{
    int pid;
    int AT;
    int BT;
    int CT;
};

int check(struct node arr[], int size);
int checkCT(struct node arr[], int size);
void print(struct node arr[], int size);
void Quicksort(struct node arr[], int low, int high, int i);
int PartitionAT(struct node arr[], int low, int high);
int PartitionBT(struct node arr[], int low, int high);
void swap(struct node *a, struct node *b);
void SortProcessId_BT(struct node arr[], int size);
void SortProcessId_AT(struct node arr[], int size);
void CompletionTime(struct node arr[], int size);
void printall(struct node arr[], int size);

void main()
{
    int size;
    printf("\nEnter Number Of Processes :");
    scanf("%d", &size);

    int avgtat, avgwt, ck;

    struct node arr[size];
    for (int i = 0; i < size; i++)
    {
        arr[i].CT = 0;
        printf("\nEnter Pid AT BT :");
        scanf("%d %d %d", &arr[i].pid, &arr[i].AT, &arr[i].BT);
    }
    ck = check(arr, size);
    if (ck == 0)
    {
        printf("\nInvalid input AT or BT cannot be negative");
    }
    else
    {
        printf("\nThe Table before sorting :");
        print(arr, size);
        // sorting based on AT and if AT is same sort based on Pid.
        Quicksort(arr, 0, size - 1, 0);
        SortProcessId_AT(arr, size);

        printf("\nThe Table after sorting Based on AT:");
        print(arr, size);
        // calculating completion time.
        CompletionTime(arr, size - 1);
        // Table with updated Completion time.
        printf("\nTable with updated CT :");
        printall(arr, size);
    }
}

void CompletionTime(struct node arr[], int size)
{
    int curtime = 0, j = 0, maxAT, i;
    struct node *p;
    struct node temp = {0, 0, 100000, 0};
    maxAT = arr[size].AT;

    printf("\nGant Chart :");
    printf("\n_________________________________________________________________________________________");
    printf("\n|00|");
    // Among the current available processes selecting the one with least B.T;
    // var j is used to iterate over increasing order of AT.
    // var i is used to find the process with min BT out of all available processes.
    while (curtime <= maxAT && j <= size)
    {

        i = 0;
        if (arr[j].AT <= curtime)
        {
            p = &temp;
            // iterating through all the available processes and finding the min BT process out of them.
            while (i <= size && arr[i].AT <= curtime)
            {
                if ((arr[i].BT < p->BT) && arr[i].CT == 0)
                {
                    p = &arr[i];
                    // Their is a reason to increment i outside.
                }
                i++;
            }
            if (p->BT != 100000)
            {
                p->CT = curtime + p->BT;
                curtime = p->CT;
                printf(" P%d |%02d|", p->pid, p->CT);
            }
            j++;
        }
        else
        {
            curtime = arr[j].AT;
            printf("///|%02d|", curtime);
        }
    }
    if (checkCT(arr, size))
    {
        Quicksort(arr, 0, size, 1);
        SortProcessId_BT(arr, size + 1);
        for (i = 0; i <= size; i++)
        {
            if (arr[i].CT == 0)
            {
                arr[i].CT = curtime + arr[i].BT;
                curtime = arr[i].CT;
                printf(" P%d |%02d|", arr[i].pid, curtime);
            }
        }
    }
    printf("\n_________________________________________________________________________________________");
}

int checkCT(struct node arr[], int size)
{
    for (int i = 0; i <= size; i++)
    {
        if (arr[i].CT == 0)
        {
            return 1;
        }
    }
    return 0;
}

int check(struct node arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i].AT < 0 || arr[i].BT < 0)
        {
            return 0;
        }
    }
    return 1;
}

void print(struct node arr[], int size)
{
    printf("\n_________________________");
    printf("\n| P_id | AT | BT | CT |\n");
    printf("_________________________\n");
    for (int i = 0; i < size; i++)
    {
        printf("|  %02d  | %02d | %02d | %02d |\n", arr[i].pid, arr[i].AT, arr[i].BT, arr[i].CT);
    }
    printf("_________________________\n");
}

void printall(struct node arr[], int size)
{
    int avgtat = 0, avgwt = 0;
    int tat, wt;
    printf("\n___________________________________");
    printf("\n| P_id | AT | BT | CT | TAT | WT |\n");
    printf("___________________________________\n");
    for (int i = 0; i < size; i++)
    {
        tat = arr[i].CT - arr[i].AT;
        wt = tat - arr[i].BT;
        avgtat += tat;
        avgwt += wt;
        printf("|  %02d  | %02d | %02d | %02d | %02d | %02d |\n", arr[i].pid, arr[i].AT, arr[i].BT, arr[i].CT, tat, wt);
    }
    printf("___________________________________\n");
    printf("\nAverage TAT = %d", avgtat / size);
    printf("\nAverage  WT = %d", avgwt / size);
}

void swap(struct node *a, struct node *b)
{
    struct node temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void Quicksort(struct node arr[], int low, int high, int i)
{
    if (low < high)
    {
        int pos;
        if (i == 0)
        {
            pos = PartitionAT(arr, low, high);
            Quicksort(arr, low, pos - 1, 0);
            Quicksort(arr, pos + 1, high, 0);
        }
        else
        {
            pos = PartitionBT(arr, low, high);
            Quicksort(arr, low, pos - 1, 1);
            Quicksort(arr, pos + 1, high, 1);
        }
    }
}

int PartitionAT(struct node arr[], int low, int high)
{
    int j = low - 1;
    int pivot = arr[high].AT;
    for (int i = low; i < high; i++)
    {
        if (arr[i].AT <= pivot)
        {
            j++;
            swap(&arr[j], &arr[i]);
        }
    }
    swap(&arr[j + 1], &arr[high]);
    return j + 1;
}

int PartitionBT(struct node arr[], int low, int high)
{
    int j = low - 1;
    int pivot = arr[high].BT;
    for (int i = low; i < high; i++)
    {
        if (arr[i].BT <= pivot)
        {
            j++;
            swap(&arr[j], &arr[i]);
        }
    }
    swap(&arr[j + 1], &arr[high]);
    return j + 1;
}

void SortProcessId_BT(struct node arr[], int size)
{
    struct node temp;
    int j;
    for (int i = 1; i < size; i++)
    {
        temp = arr[i];
        j = i - 1;
        while (j >= 0 && (arr[j].BT == temp.BT) && (arr[j].pid > temp.pid))
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;
    }
}

void SortProcessId_AT(struct node arr[], int size)
{
    struct node temp;
    int j;
    for (int i = 1; i < size; i++)
    {
        temp = arr[i];
        j = i - 1;
        while (j >= 0 && (arr[j].AT == temp.AT) && (arr[j].pid > temp.pid))
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;
    }
}