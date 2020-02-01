
#include<stdio.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include <errno.h>

#define NUMBER_OF_ROBOTS 2
#define WAITING     0
#define IN_PROCESS  1
#define COMPLETE    2


#define IDLE        0
#define BUSY        1
#define STOP        2

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))


typedef struct PackageNode
{
    int PackageNumber;
    int TotalItems;
    int Status;
    int ListofItems [10];
    int ItemsPending;
    int NoItemUnavailable;
    struct PackageNode *ptr;
}node;

typedef struct __attribute__((__packed__)) robotNode
{
    int robotNo;
    int status;
    int count;
    node* pkt;
    
}robotNode;



void robot_arm (robotNode* node);



node *front,*rear,*processListFront,*processListRear,*completedFront,*completedRear;
pthread_mutex_t m1,m2;
//sem_t signal[NUMBER_OF_ROBOTS];
