
#include "config.h"


void robot_arm (robotNode* node)
{
    while(1)
    {
        //Check status of robot
        if (node->status != STOP)
            //if the robot is stopped,the package is processed by the next free robot
        {
            pthread_mutex_lock(&m1);
            node->pkt=processListFront;  //first come first serve
            pthread_mutex_unlock(&m1);
            //printf("robot  Number = %d\n",node->robotNo+1);
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
}
