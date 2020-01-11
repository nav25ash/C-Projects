
#include "config.h"


void robot_arm (robotNode* node)
{
    while(1)
    {
    node->pkt=processListFront;  //need to be edited
    printf("robot  Number = %d\n",node->robotNo+1);
    node->status=BUSY;
    sleep(5);
    node->status=IDLE;
    if(node->pkt != NULL)
    {
        pthread_mutex_lock(&m1);
        node->pkt->Status=COMPLETE;
        pthread_mutex_unlock(&m1);
    }
    }
}
