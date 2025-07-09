// SRTF

#include <stdio.h>

struct node1
{
    int pid;
    int AT;
    int BT;
    int CT;
};

// while calculating completion time BT values will be modified.so to retrive the BT storing a copy of BT.
struct node2
{
    int pid;
    int BT;
};

int check(struct node1 arr[], int size);
int checkCT(struct node1 arr[], int size);
void print(struct node1 arr[], int size);
void Quicksort(struct node1 arr[], int low, int high, int i);
int PartitionAT(struct node1 arr[], int low, int high);
int PartitionBT(struct node1 arr[], int low, int high);
void swap(struct node1 *a, struct node1 *b);
void SortProcessId_BT(struct node1 arr[], int size);
void SortProcessId_AT(struct node1 arr[], int size);
void CompletionTime(struct node1 arr[], int size);
void printall(struct node1 arr[], int size);
void updateBT(struct node1 arr[], struct node2 BT[], int size);

void main()
{
    int size;
    printf("\nEnter Number Of Processes :");
    scanf("%d", &size);

    int avgtat, avgwt, ck;
    struct node2 BT[size];

    struct node1 arr[size];
    for (int i = 0; i < size; i++)
    {
        arr[i].CT = 0;
        printf("\nEnter Pid AT BT :");
        scanf("%d %d %d", &arr[i].pid, &arr[i].AT, &arr[i].BT);
        // while calculating completion time we are changing BT values from "arr"
        // so store it in some temp array BT to update arr table again;
        BT[i].BT = arr[i].BT;
        BT[i].pid = arr[i].pid;
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
        // BT values are changed in the above function while calculating Completion time of process.

        updateBT(arr, BT, size);

        // Table with updated Completion time.
        printf("\nTable with updated CT :");
        printall(arr, size);
    }
}

void CompletionTime(struct node1 arr[], int size)
{
    int curtime = 0, j = 0, maxAT, i;
    struct node1 *p;
    struct node1 temp = {0, 0, 100000, 0}; // just for some refrence the node1 with max BT.
    maxAT = arr[size].AT;

    int slot; // max amount of time we can execute the process is clalculated here;

    // Among the current available processes selecting the one with least B.T;
    // var j is used to iterate over increasing order of AT.
    // var i is used to find the process with min BT out of all available processes.

    printf("\nGrant Chart :");
    printf("\n_");
    printf("\n|00|");
    while (curtime <= maxAT && j < size)
    {
        i = 0;
        if (arr[j].AT <= curtime)
        {
            p = &temp;

            // while loop breakes when it encounters a process which is not currently available
            // But it is the next available process. since AT are in sorted order.
            // when loop terminates i is pointing at this process.
            while (i < size && arr[i].AT <= curtime)
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
                int nextAT = arr[i].AT;
                slot = nextAT - curtime; // This is the max time for which we can execute a process that is selected.
                if (p->BT < slot)
                {
                    p->CT = curtime + p->BT;
                    printf(" P%d |%02d|", p->pid, p->CT);
                    curtime = p->CT;
                }
                else
                {
                    p->BT = p->BT - slot;
                    curtime = curtime + slot;
                    printf(" P%d |%02d|", p->pid, curtime);
                }
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
    printf("\n_");
}

void updateBT(struct node1 arr[], struct node2 BT[], int size)
{
    int x, j;
    for (int i = 0; i < size; i++)
    {
        x = arr[i].pid;
        j = 0;
        while (BT[j].pid != x)
        {
            j++;
        }
        arr[i].BT = BT[j].BT;
    }
}

int checkCT(struct node1 arr[], int size)
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

int check(struct node1 arr[], int size)
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

void print(struct node1 arr[], int size)
{
    printf("\n_");
    printf("\n| P_id | AT | BT | CT |\n");
    printf("_\n");
    for (int i = 0; i < size; i++)
    {
        printf("|  %02d  | %02d | %02d | %02d |\n", arr[i].pid, arr[i].AT, arr[i].BT, arr[i].CT);
    }
    printf("_\n");
}

void printall(struct node1 arr[], int size)
{
    int avgtat = 0, avgwt = 0;
    int tat, wt;
    printf("\n_");
    printf("\n| P_id | AT | BT | CT | TAT | WT |\n");
    printf("_\n");
    for (int i = 0; i < size; i++)
    {
        tat = arr[i].CT - arr[i].AT;
        wt = tat - arr[i].BT;
        avgtat += tat;
        avgwt += wt;
        printf("|  %02d  | %02d | %02d | %02d | %02d | %02d |\n", arr[i].pid, arr[i].AT, arr[i].BT, arr[i].CT, tat, wt);
    }
    printf("_\n");
    printf("\nAverage TAT = %d", avgtat / size);
    printf("\nAverage  WT = %d", avgwt / size);
}

void swap(struct node1 *a, struct node1 *b)
{
    struct node1 temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void Quicksort(struct node1 arr[], int low, int high, int i)
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

int PartitionAT(struct node1 arr[], int low, int high)
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

int PartitionBT(struct node1 arr[], int low, int high)
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

void SortProcessId_BT(struct node1 arr[], int size)
{
    struct node1 temp;
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

void SortProcessId_AT(struct node1 arr[], int size)
{
    struct node1 temp;
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