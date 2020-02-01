
#include "config.h"
#include <sched.h>

#define handle_error_en(en, msg) \
do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

void deleteProcessQ()
{
    node *temp,*new;
   // pthread_mutex_lock(&m1);
    //temp=processListFront;
    new=processListFront;
   // new->ptr=NULL;
    
    
    
    
    //Delete from available package
    if(processListFront->ptr==NULL)
    {
        processListRear=NULL;
        processListFront=NULL;
    }
    else
    {
                    
        processListFront=processListFront->ptr;
    }
    
    new->ptr = NULL;
    //displaystatus();
}

void updateCompletedQ(node *pkt)
{
    node *new;
    new=pkt;
    
    // Add to completed package
    if(completedFront==NULL && completedRear== NULL)
    {
        completedFront=new;
    }
    else
    {
        completedRear->ptr=new;
    }
    completedRear=new;
    
}

void InsertToProcessList(node *package)
{
    node *new,*curnode,*prevnode;
    int pendingitems;
    new=package;
    new->ptr=NULL;
    pendingitems = package->ItemsPending;
    
    if(processListFront==NULL && processListRear== NULL)
    {
        processListFront=new;
        processListRear=new;
    }
    else
    {
        if(processListFront->ItemsPending > pendingitems)
        {
            new->ptr = processListFront;
            processListFront=new;
        }
        else if(processListRear->ItemsPending <= pendingitems)
        {
            processListRear->ptr=new;
            processListRear=new;               //the new element becomes the last element
        }
        else
        {
            prevnode=processListFront;
            curnode=processListFront->ptr;
            while(curnode!=NULL)
            {
                if(curnode->ItemsPending > pendingitems)
                {
                    new->ptr=curnode;
                    prevnode->ptr=new;
                    break;                      //break out of the loop once the node is inserted in the right slot
                }
                else
                {
                    prevnode=curnode;
                    curnode=curnode->ptr;
                }
            }
        }
    }
}

void packItems()
{
    sleep(5);
}

void robot_arm (robotNode* RobotNode)
{
    robotNode *r_node = RobotNode;
    struct sched_param param;
    int policy,max_priority,error,i;
    
    /*error = pthread_getschedparam (pthread_self(), (int *) &policy, &param);
    if (error != 0)
        handle_error_en(error, "pthread_getschedparam");
    max_priority=sched_get_priority_max(policy);*/
    
    while(1)
    {
        //Check status of robot
        pthread_mutex_lock(&m1);
        if (r_node->status != STOP)
            //if the robot is stopped,the package is processed by the next free robot
        {
            
            if(processListFront != NULL)
            {
                r_node->status=BUSY;
                r_node->pkt=processListFront;  //first come first serve
                deleteProcessQ();
                pthread_mutex_unlock(&m1);
                printf("robot  Number = %d package number=%d\n",r_node->robotNo,r_node->pkt->PackageNumber);
                for(i=0;i< r_node->pkt->TotalItems;i++)
                {
                    
                    pthread_mutex_lock(&m1);
                    if (r_node->status == STOP)
                    {
                        InsertToProcessList(r_node->pkt);  // Adds package back to ready list
                        pthread_mutex_unlock(&m1);
                        printf("robot %d stopped while processing Package number %d\n",r_node->robotNo,r_node->pkt->PackageNumber);
                        break;
                    }
                    else
                    {
                        pthread_mutex_unlock(&m1);
                        if(r_node->pkt->ItemsPending > 0)
                        {
                            packItems();
                            r_node->pkt->ItemsPending--;
                        }
                    }
                   /* param.sched_priority = max_priority - (r_node->pkt->TotalItems - i);
                    error = pthread_setschedparam(pthread_self(), policy, &param);
                    if (error != 0)
                        handle_error_en(error, "pthread_setschedparam");*/
                    
                }
                if( r_node->pkt->ItemsPending == 0 ) //if all the items are processed then move the package into completed list.
                {
                    r_node->count=r_node->count + 1 ;
                    r_node->pkt->Status=COMPLETE;
                    pthread_mutex_lock(&m1);
                    updateCompletedQ(r_node->pkt);
                    r_node->status=IDLE;
                    pthread_mutex_unlock(&m1);
                    
                }
                sleep(10 - r_node->robotNo);
            }
            else
            {
                pthread_mutex_unlock(&m1);
                sleep(5); // wait for process list to be filled.
            }
        }
        else
        {
            pthread_mutex_unlock(&m1);
            sleep(5);
        }
    }
}


