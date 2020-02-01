Steps to execute the program:

1) Clone the repository
2) Open terminal ,execute gcc main.c robot_arm.c -o main.o
3) open a second termainl,execute gcc process_management.c -o mnt.o
4) Execute the mnt.o binary followed by main.o binary
The program can be started,suspended and terminated using the process_management.  

Note: if the process_management was terminated abruptly,ensure to execute rm /tmp/myfifo in command window.

Once the Start command is issued.
The user needs to input the value of the package attribute.

The are 3 link list:process list,wait list and completed list
Once the user inputs the package details the item required is crossed checked with a buffer of items
"isIteamsAvailable"  function .If all the items are available then add the package to the process list else it is place in a waiting list.
This ensures the robot are able to fill another box still waiting for the items in the previous package.Each robot picks the first available package to be processed.
You should see them on the screen under the  'Packages in Assembly'.Once package is processed it is moved to the complete list.The robots wait for packages in the process list.

If the robot is stopped,the package that was currently being processed is added back to the processList.The current package is inserted such that the pending items of the next ready-package in the list is greater than the current package.

example: Package number    Total no.of items       List                 Status      Items Pending 
                    6                               4                        2 2 2 2                1                   4
                    7                               2                           3 3                   1                   2
                    8                               5                     1 1 1 1 1                1                   5
                    
                    the robot 3 starts processing package no 6
                    we halt thr robot after it processes 1 item
                    then the pacake number 6 is added back to the process list after package 7
                    
 This is done so that the package with lesser number of items to be processed is given priority.
 
 Note :The the list is not  sorted.We insert the current package before another package that has more number of items to be procesed. 
                    

main.c 
Consists of the 
- main function
- Input for the package list 
 We have a link list used to add all packages that have items available.
 the items are valiated using  isIteamsAvailable()
 
 robot_arm.c 
 The robot  function.

Process_management.c
The program to manage the process
1.To start the process,enter 'S' 
2.To stop the process,enter 'E'
3.to suspend the process,enter 'H'
4.To stop a robot,enter 'R'
5.To re-start a robot,enter 'N'
6.To terminate the program,enter 'X'"

We can provide the name to be displayed as input to the program.

expected output:
Welcome name

If input is not provided,
expected output:
Welcome Manager

use NUMBER_OF_ROBOTS  (config.h) to increase the number of robots.
Each robot is created as a seperate thread.


