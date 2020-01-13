#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    char cmd;
    int num;
    int fd,status,flag=0;
    char * commandQ = "/tmp/myfifo";
    
    /* create the FIFO (named pipe) */
    if ((status = mkfifo(commandQ, 0666)) < 0) {
        printf("\n %d \n", status);
        return 0;
    }
    
    if (argc < 2)
    {
        printf("Welcome Manager\n");
        
    }
    else
    {
        printf("Welcome %s\n",argv[1]);
    }
    printf("1.To start the process,enter 'S' \n2.To stop the process,enter 'E'\n"
           "3.to suspend the process,enter 'H'\n4.To stop a robot,enter 'R'\n5.To terminate the program,enter 'X'\n");
   
    
    while(1)
    {
        scanf(" %c",&cmd);
        //printf("%c\n",cmd);
        
        if(cmd != 'X' && cmd != 'S' && cmd !='E' && cmd !='H' && cmd!='R')
            printf("INVAILD INPUT\n");
        else
        {
            if(cmd == 'X')
                break;
            //terminate program
            else
            {
                /* write command to the FIFO */
                fd = open(commandQ,O_WRONLY);
                //printf("fd=%d\n",fd);
                write(fd, &cmd , 1);
                
                if (cmd == 'R')
                {
                    printf("Enter the Robot number\n");
                    scanf("%d",&num);
                    write(fd, &cmd , sizeof(cmd));
                    
                }
            }
            close(fd);
            
        }
        
    }
    unlink(commandQ);
    printf("\nProcess terminated\n");
}
