/*fcfs scheduler algorith*/

#include<stdio.h>
#include <unistd.h>
#include<time.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "config.h"



//node *front,*rear,*processListFront,*processListRear;
//pthread_mutex_t m1;

int ItemList[10] = { 1,2,5,6,7,3,4,5,6,7 };
robotNode *robot[NUMBER_OF_ROBOTS];

void addProcessQ();
void deleteProcessQ();
void addq(node *);  // Adds list of packages ready to be processed
void displaystatus();      // Display the status of all the packages
void display(node* list_pointer_front);
int isIteamsAvailable(int list[],int num);
void addToWaitList(node* package);

int main()
{
    int temp1,temp2,i,count,flag=0,err;
    
    pthread_t tid[2];  //Total number of treads created
    pthread_t Robotarm[NUMBER_OF_ROBOTS];
    
    int fd;
    char * myfifo = "/tmp/myfifo";
    char buf;
    
    front=NULL;
    rear =NULL;
    processListFront=NULL;
    processListRear=NULL;
    completedFront=NULL;
    completedRear=NULL;
    
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_mutex_init(&m1,NULL);  //Mutex for synchronization
    pthread_mutex_init(&m2,NULL);  //Mutex for synchronization OF Robot arms
    
    
    
    
    while(1)
    {
        fd = open(myfifo, O_RDONLY);    /* open, read, and display the message from the FIFO */
        count=read(fd, &buf, 1);
        close(fd);
        //printf("count Received: %d\n", count);
        if(  count == 0 )
            sleep(5);
        
       else{
          // printf("Received: %c\n", buf);
     
           if(buf == 'S')
           {
               if(!flag)
                {
                    temp1=pthread_create(&tid[0],&attr,(void*)addProcessQ,NULL);
                    for(i=0;i<NUMBER_OF_ROBOTS;i++)
                    {
                        robot[i]=(robotNode*)malloc(sizeof(robotNode));
                        robot[i]->robotNo=i;
                        robot[i]->count=0;
                        robot[i]->pkt = NULL;
                        temp1=pthread_create(&Robotarm[i],&attr,(void*) robot_arm,robot[i]);
                        
                    }
                     temp2=pthread_create(&tid[1],&attr,(void*)displaystatus,NULL);
                    flag=1;   //if Start cmd is given the second time,give a msg to the user
                }
                else
                    printf("The Process is in execution\n");
            
           }
        
            else if(buf == 'R')
            {
                if(flag)
                {
                    fd = open(myfifo, O_RDONLY);
                    //printf("trying to read\n");
                    count=read(fd,&i,sizeof(i));
                    if(i< NUMBER_OF_ROBOTS)
                    {
                        printf("Robot to be turned off: %d\n",i);
                        pthread_mutex_lock(&m1);
                        if(robot[i]->status != STOP)
                            robot[i]->status=STOP;
                        else
                            printf("Robot %d is already inactive\n",i);
                        pthread_mutex_unlock(&m1);
                    }
                    else
                        printf("%d is an invalid Robot number",i);
                    close(fd);
                }
                else
                    printf("Process is not active.\nStart the process\n");
            }
           
            else if(buf == 'N')
            {
                if(flag)
                {
                    fd = open(myfifo, O_RDONLY);
                    count=read(fd,&i,sizeof(i));
                    if(i< NUMBER_OF_ROBOTS)
                    {
                        printf("Robot to be turned on: %d\n",i);
                        pthread_mutex_lock(&m1);
                        if(robot[i]->status == STOP)
                            robot[i]->status=IDLE;
                        else
                            printf("Robot %d is already active\n",i);
                        pthread_mutex_unlock(&m1);
                    }
                    else
                        printf("%d is an invalid Robot number",i);
                    close(fd);
                }
                else
                    printf("Process is not active.\nStart the process\n");
            }
           
            else if(buf == 'H' )
            {
                if(flag)
                {
                for(i=0;i<NUMBER_OF_ROBOTS;i++)
                    
                    {
                        robot[i]->status=STOP;
                      //  printf("Robot to be turned off: %d\n", i);
                    }
                     printf("Process is suspended.\nAll robots are stopped\n");
                    
                }
                else
                   printf("Process is not active.\nStart the process\n");
                
            }

            else if(buf == 'E' || buf == 'X' )
            {
                if(flag)
                {
                    for(i=0;i<NUMBER_OF_ROBOTS;i++)
                    {
                        err=pthread_kill(Robotarm[i],0);
                        if(err)
                            printf("Error in terminating the robot %d,Error = %d",i+1,err);
                    }
                    pthread_kill(tid[0],0);
                    pthread_kill(tid[1],0);
                    printf("All threads are killed\nThe process is stopped\n");
                    flag=0;
                    
                }
                if(buf == 'X')
                {
                    printf("The process is terminated");
                    close(fd);
                    unlink(myfifo);
                    break;
                    
                }
            }
           //else
               //printf("Process is not active.\nStart the process\nVerify the command provided\n");
       }
        //printf("IN MAIN COUNT : %d\n", j++);
        
    }
    //pthread_join(tid[0],NULL);
    //pthread_join(tid[1],NULL);
}

void addProcessQ()
{
    
    while(1)
    {
        node *new;
        int i;
        new=(node*)malloc(sizeof(node));
        printf("ENTER THE PACKAGE DETAILS\n");
        printf("Package Number\tTotal number of items\t List of Items\n");
        scanf("%d%d",&new->PackageNumber,&new->TotalItems);
        //printf("total=%d\n",new->TotalItems);
        // fixed max limit of items to be 10;
        for(i=0; i < new->TotalItems && i < 9 ;i++)
        {
            scanf("%d",&new->ListofItems[i]);
            //printf("list input=%d\t",new->ListofItems[i]);
            
        }
        new->Status=WAITING;
        new->ItemsPending=new->TotalItems;
        new->NoItemUnavailable = isIteamsAvailable(new->ListofItems,new->TotalItems);  //searching in items Q
        new->ptr=NULL;
        
        
        if(new->NoItemUnavailable == 0)
        {
            addq( new );
        }
        else
        {
            addToWaitList(new);
        }
         //display(front);
        
    }
    
}

void addq(node * ProcessPackage)
{
    //int option=1;
    //time_t end;
    node *new;
    new=ProcessPackage;
    new->Status=IN_PROCESS;
    new->ptr=NULL;
    pthread_mutex_lock(&m1);
    if(processListFront==NULL && processListRear== NULL)
    {
        processListFront=new;
    }
    else
    {
        processListRear->ptr=new;
    }
    processListRear=new;
    //display(processListFront);
    pthread_mutex_unlock(&m1);
    // printf("\nPackage in Assembly line\n");
    //display(processListFront);
    
}


 void displaystatus()
{
    int i;
    sleep(20);
    while(1)
    {
        
        printf("Packages in Assembly\n");
        display(processListFront);
        
        printf("Packages in waiting\n");
        display(front);
        
        
        printf("Packages completed \n");
        display(completedFront);
        
        for(i=0;i<NUMBER_OF_ROBOTS;i++)
        {
            printf("No of packages processed by Robot %d = %d\n",robot[i]->robotNo,robot[i]->count);
            
        }
        sleep(10);
    }
}


void display(node* list)
{
    int i;
    node *curnode,*list_pointer_front;
    pthread_mutex_lock(&m1);
    list_pointer_front = list;
    if(list_pointer_front==NULL)
    {
        printf("No packages in the queue\n");
    }
    else
    {
        curnode=list_pointer_front;
       // printf("\nPackage in Assembly line\n");
        printf("Package Number\tTotal no.of items\tList of Items\tStatus \tItemsPending\n");
        while(curnode!=NULL)
        {
            printf("%d\t\t%d\t\t\t",curnode->PackageNumber,curnode->TotalItems);
            for(i=0;i<curnode->TotalItems;i++)
                printf(" %d",curnode->ListofItems[i]);
            
            printf("\t\t%d\t%d\n",curnode->Status,curnode->ItemsPending);
            curnode=curnode->ptr;
        }
    }
    pthread_mutex_unlock(&m1);
}

//Verify if the iteams required of the packages is vailable in the itemsList
int isIteamsAvailable(int list[],int num)
{
    int i,j,count=0;
    int flag=0;
    
    for(i=0;i<num;i++)
    {
        flag=0;
        for(j=0;j<(int)NELEMS(ItemList);j++)
        {
            if (list[i] == ItemList[j])
            {
                flag=1;
                break;
            }
        }
        if(flag == 0)
        {
            count=count+1;
        }
    }
    return count;
}

void addToWaitList(node* package)
{
    node *new,*curnode,*prevnode;
    new=package;
    
    if(front==NULL && rear== NULL)
    {
        front=new;
        rear=new;
    }
    else
    {
        if(front->NoItemUnavailable > new->NoItemUnavailable)
        {
            new->ptr = front;
            front=new;
        }
        else if(rear->NoItemUnavailable <= new->NoItemUnavailable)
        {
            rear->ptr=new;
            rear=new;               //the new element becomes the last element
        }
        else
        {
            prevnode=front;
            curnode=front->ptr;
            while(curnode!=NULL)
            {
                if(curnode->NoItemUnavailable > new->NoItemUnavailable)
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
