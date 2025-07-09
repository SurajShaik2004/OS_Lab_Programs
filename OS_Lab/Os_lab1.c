//FCFS.

#include <stdio.h>

struct node
{
    int pid;
    int AT;
    int BT;
};

void Quicksort(struct node arr[], int low, int high);
void SortProcessId(struct node arr[], int size);
int Partition(struct node arr[], int low, int high);
void swap(struct node *a, struct node *b);
void print(struct node arr[], int size);
void printall(int avgtat, int avgwt, int C_T[], struct node arr[], int size);
void Completion_Time(struct node arr[], int C_T[], int size);
int avgTaT(struct node arr[], int C_T[], int size);
int avgWt(struct node arr[], int C_T[], int size);
int check(struct node arr[], int size);
void GantChart(struct node arr[], int C_T[], int size);

void main()
{

    int size;
    printf("\nEnter Number Of Processes :");
    scanf("%d", &size);

    int C_T[size];
    int avgtat, avgwt, ck;
    // printf("\nEnter the Number of processes :-");
    // scanf("%d",&size);
    struct node arr[size];
    for (int i = 0; i < size; i++)
    {
        printf("\nEnter Pid AT BT :");
        scanf("%d %d %d", &arr[i].pid, &arr[i].AT, &arr[i].BT);
    }
    ck = check(arr, size);
    if (ck == 0)
    {
        printf("You Entred Invalid Data.(time cant be negative)");
    }
    else
    {
        printf("\nYour Input Before Sorting:\n");
        print(arr, size);
        Quicksort(arr, 0, size - 1);
        SortProcessId(arr, size);
        printf("\nYour Input After Sorting :");
        print(arr, size);
        Completion_Time(arr, C_T, size);
        avgtat = avgTaT(arr, C_T, size);
        avgwt = avgWt(arr, C_T, size);
        GantChart(arr, C_T, size);
        printall(avgtat, avgwt, C_T, arr, size);
    }
}

void SortProcessId(struct node arr[], int size)
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
    printf("\n__________________");
    printf("\n| P_id | AT | BT |\n");
    printf("__________________\n");
    for (int i = 0; i < size; i++)
    {
        printf("|  %02d  | %02d | %02d |\n", arr[i].pid, arr[i].AT, arr[i].BT);
    }
    printf("__________________\n");
}

// To Print Actual Output.
void printall(int avgtat, int avgwt, int C_T[], struct node arr[], int size)
{
    printf("\n____________________________________");
    printf("\n| P_id  | AT | BT | CT | TAT | WT  |\n");
    printf("____________________________________\n");
    for (int i = 0; i < size; i++)
    {
        printf("|  %02d   | %02d | %02d | %02d | %02d  | %02d  |\n", arr[i].pid, arr[i].AT, arr[i].BT, C_T[i], (C_T[i] - arr[i].AT), ((C_T[i] - arr[i].AT) - arr[i].BT));
    }
    printf("___________________________________\n");
    printf("\nAverage T.A.T :: %d", avgtat);
    printf("\nAverage W.T :: %d ", avgwt);
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

void Completion_Time(struct node arr[], int C_T[], int size)
{
    int currentTime = 0;
    for (int i = 0; i < size; i++)
    {
        if (arr[i].AT <= currentTime)
        {
            C_T[i] = currentTime + arr[i].BT;
            currentTime = C_T[i];
        }
        else
        {
            C_T[i] = arr[i].AT + arr[i].BT;
            currentTime = C_T[i];
        }
    }
}

int avgTaT(struct node arr[], int C_T[], int size)
{
    int sum = 0, avg;
    for (int i = 0; i < size; i++)
    {
        sum += (C_T[i] - arr[i].AT);
    }
    avg = sum / size;
    return avg;
}

int avgWt(struct node arr[], int C_T[], int size)
{
    int sum = 0, avg;
    for (int i = 0; i < size; i++)
    {
        sum += ((C_T[i] - arr[i].AT) - arr[i].BT);
    }
    avg = sum / size;
    return avg;
}

void GantChart(struct node arr[], int C_T[], int size)
{
    int currenttime = 0;
    printf("\nGant Chart:");
    printf("\n_____________________________________________________________");
    printf("\n|");
    for (int i = 0; i < size; i++)
    {
        if (currenttime >= arr[i].AT)
        {
            printf("  P%d |", arr[i].pid);
            currenttime = C_T[i];
        }
        else
        {
            printf("/////|");
            currenttime = arr[i].AT;
            i--;
        }
    }
    currenttime = 0;
    printf("\n0");
    for (int i = 0; i < size; i++)
    {
        if (currenttime >= arr[i].AT)
        {
            printf("____%02d", C_T[i]);
            currenttime = C_T[i];
        }
        else
        {
            printf("____%02d", arr[i].AT);
            currenttime = arr[i].AT;
            i--;
        }
    }
}
