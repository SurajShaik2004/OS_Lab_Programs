// first fit :: out of all the avilable partitions, first sufficient partition is used.
//  are allocated for that process.

#include <stdio.h>

#define Num 4
#define Blocks 4

struct node1
{
    int pid;
    int size;  // each job size.
    int block; // the index of partition in which the job is allocated.
};

struct node2
{
    int RamSiz;
    int state;
};

int main()
{
    struct node1 jobs[Num];
    struct node2 RAM[Blocks] = {{150, 0}, {100, 0}, {300, 0}, {400, 0}}; // are else take the inputs from the user.
}