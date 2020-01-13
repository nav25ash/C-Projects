
#include<stdio.h>
#include <unistd.h>
#include<time.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>

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
    int ListofItems [10];
    int TotalItems;
    int Status;
    struct PackageNode *ptr;
}node;

typedef struct robot_arm
{
    int robotNo;
    node* pkt;
    int status;
    struct robot_arm * ptr;
}robotNode;



void robot_arm (robotNode* node);

node *front,*rear,*processListFront,*processListRear;
pthread_mutex_t m1;
//sem_t signal[NUMBER_OF_ROBOTS];
