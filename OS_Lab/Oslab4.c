// Round Robin Scheduling.

#include <stdio.h>
#define Max 10
#define TQ 2

struct node
{
    int pid;
    int AT;
    int BT;
    int CT;
    int visit;
};

struct node2
{
    int pid;
    int BT;
};

struct node *CQueue[Max];
int front = -1, rear = -1;

void CompletionTime(struct node arr[], int size);
void enqueue(struct node *val);
struct node *dequeue();
int isEmpty();
void updateBT(struct node arr[], struct node2 BT[], int size);
void Quicksort(struct node arr[], int low, int high);
void swap(struct node *a, struct node *b);
int Partition(struct node arr[], int low, int high);
void SortProcessId_AT(struct node arr[], int size);
void print(struct node arr[], int size);
void printall(struct node arr[], int size);
int check(struct node arr[], int size);

void main()
{
    int size;
    printf("\nEnter Number Of Processes :");
    scanf("%d", &size);

    int avgtat, avgwt, ck;
    struct node2 BT[size];

    struct node arr[size];
    for (int i = 0; i < size; i++)
    {
        arr[i].CT = 0;
        arr[i].visit = 0;
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
        // Sort the given input based on AT and processid.
        Quicksort(arr, 0, size - 1);
        SortProcessId_AT(arr, size);
        printf("\nThe Provided input in the order of arrival time:");
        print(arr, size);
        // NOW we are ready to calculate Completion time o f processes.
        CompletionTime(arr, size);
        updateBT(arr, BT, size);
        printall(arr, size);
    }
}

void CompletionTime(struct node arr[], int size)
{
    int completed = 0;       // To keep track of number of processes that are completed.
    int i = 0;               // To Enqueue the processes that are arrived while the execution of current process.
    int vCount = 0;          // To know all the processes are visited are not.
    int nextAt = 0;          // To handle ideal times.when queue is empty and curtime < AT of next process.
    struct node *p;          // To point the current executing process.
    int curtime = arr[0].AT; // Since ,Upto first process arrives its an ideal time.
    enqueue(&arr[0]);
    arr[0].visit = 1;
    vCount++;
    printf("\nGant Chart :");
    printf("\n_____________________________________________________________________________________________");
    if (curtime != 0)
    {
        printf("\n|%02d|///|%02d|", 0, curtime);
    }
    else
    {
        printf("\n|%02d|", curtime);
    }

    while (completed != size)
    {
        p = dequeue();
        printf(" P%d |", p->pid);
        if (p->BT <= TQ)
        {
            p->CT = curtime + p->BT;
            curtime = p->CT;
            printf("%02d|", curtime);
            if (vCount < size)
            {
                i = 0;
                while (i < size && arr[i].AT <= curtime)
                {
                    if (arr[i].visit == 0)
                    {
                        enqueue(&arr[i]);
                        arr[i].visit = 1;
                        vCount++;
                    }
                    i++;
                }
                if (i != size && arr[i].AT > curtime)
                {
                    nextAt = arr[i].AT;
                }
                else
                {
                    nextAt = 0; // mandatory not optional.
                }
            }
            completed++;
        }
        else if (p->BT > TQ)
        {
            p->BT -= TQ;
            curtime += TQ;
            printf("%02d|", curtime);
            if (vCount < size)
            {
                i = 0;
                while (i < size && arr[i].AT <= curtime)
                {
                    if (arr[i].visit == 0)
                    {
                        enqueue(&arr[i]);
                        arr[i].visit = 1;
                        vCount++;
                    }
                    i++;
                }
            }
            enqueue(p);
            // here no need to calculate next arrival time ,since Queue never become empty.
        }
        // To handle cpu ideal time when curtime is less then next arrival time of process and Queue is empty.
        if (nextAt && completed != size)
        {
            if (isEmpty())
            {
                curtime = nextAt;
                enqueue(&arr[i]);
                nextAt = 0;
                printf("///|%02d|", curtime);
            }
        }
    }
    printf("\n_____________________________________________________________________________________________");
}

void print(struct node arr[], int size)
{
    printf("\n_____________________");
    printf("\n| P_id | AT | BT |\n");
    printf("_____________________\n");
    for (int i = 0; i < size; i++)
    {
        printf("|  %02d  | %02d | %02d |\n", arr[i].pid, arr[i].AT, arr[i].BT);
    }
    printf("_____________________\n");
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

// Sorting based on AT
void Quicksort(struct node arr[], int low, int high)
{
    if (low < high)
    {
        int pos;
        pos = Partition(arr, low, high);
        Quicksort(arr, low, pos - 1);
        Quicksort(arr, pos + 1, high);
    }
}

void swap(struct node *a, struct node *b)
{
    struct node t;
    t = *a;
    *a = *b;
    *b = t;
}

int Partition(struct node arr[], int low, int high)
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

void updateBT(struct node arr[], struct node2 BT[], int size)
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

void enqueue(struct node *val)
{
    if (front == -1 && rear == -1)
    {
        front = rear = 0;
        CQueue[rear] = val;
    }
    else if ((rear + 1) % Max == front)
    {
        printf("\nQUEUE IS FILLED.");
    }
    else
    {
        rear = (rear + 1) % Max;
        CQueue[rear] = val;
    }
}

struct node *dequeue()
{
    struct node *val;
    if (front == -1 || rear == -1)
    {
        return NULL;
    }
    else if (front == rear)
    {
        val = CQueue[front];
        front = rear = -1;
        return val;
    }
    else
    {
        val = CQueue[front];
        front = (front + 1) % Max;
        return val;
    }
}

int isEmpty()
{
    if (front == -1 && rear == -1)
    {
        return 1;
    }
    return 0;
}