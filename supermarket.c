#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "queue.h"


typedef struct cashier cashierT;

int waiting = 0;    // global variable for counting the total waiting time of each customer

int status[4], counter[4];
/* global variable for showing the number of customer in each cashier, including the customer serving (status[4]) and
time counter for each cashier to count the remaining serve time (counter[4])*/

struct customer{
    int checkoutTime;
    int nItems;
    float payment;
};

struct cashier{
    int numberOfCustomersServed;
    int totalCustomerServiceTime;
    float totalAmountReceived;
    queueADT customerQ;
};

//Display report
void showResult(cashierT cash[]){
    int totalSC;    //total number of served customer of all cashiers
    printf("***Report***\n");
    for (int i = 0; i < 4; i++){
        printf("-Cashier %d-\n", i);
        printf("Number Of Customers Served: %d\n", cash[i].numberOfCustomersServed);
        totalSC += cash[i].numberOfCustomersServed;     // add up number of customers served of each cashier
        printf("Total Customer Service Time: %d\n", cash[i].totalCustomerServiceTime);
        printf("Total Amount Received: $%.1f\n\n", cash[i].totalAmountReceived);
    }
    float average = (float) waiting / (float) totalSC;
    printf("Average waiting time: %.2f min(s)\n", average);
}

//Display cashier information and perform dequeuing
void checkCashier(cashierT *cash, int num){
    customerT current;
    current = (customerT)malloc(sizeof(*current));    //variable for holding the element after dequeuing
    printf("=Cashier %d=\n", num);
    printf("No. of customer in the cashier: %d\n", status[num]);
    printf("Length of queue: %d\n", QueueLength(cash->customerQ));

    if (QueueLength(cash->customerQ) == 1 && counter[num] <= 0){    //if the cashier is available
        current = Dequeue(cash->customerQ);
        cash->numberOfCustomersServed++;
        cash->totalCustomerServiceTime += current->checkoutTime;
        cash->totalAmountReceived += current->payment;
        counter[num] = current->checkoutTime;        //start counting the service time
    }

    counter[num] --;    //a minute of service time passed
    if (counter[num] > 0) printf("Remaining serving time: %d\n", counter[num]);

    if (counter[num] == 0){     //if service completed
        status[num] --;     //number of customer in the cashier -1
        printf("Previous customer in the cashier left.\n");
        printf("No. of customer in cashier : %d\n", status[num]);
        if (status[num] > 0){   //if there is/are customer in the queue
            printf("Next customer start to be served.\n");
            current = Dequeue(cash->customerQ);
            counter[num] = current->checkoutTime;   //start counting the service time
            cash->numberOfCustomersServed++;
            cash->totalCustomerServiceTime += current->checkoutTime;
            cash->totalAmountReceived += current->payment;
        }
        else{
            printf("No customer in queue now.\n");
        }
    }
}


int main(){
    int end = 0;    //trigger to end the program
    FILE *datafile, *renewfile;
    datafile = fopen("supermarket.dat","r");

    if (datafile == NULL) {
        printf("File not found.");
        return 0;
    }     //if no supermarket.dat found, end the program

    char line[100];     //for reading file
    cashierT cashiers[4];
    int mins = 0;   //for recording the time passed

    for (int i = 0; i < 4; i++){
        cashiers[i].numberOfCustomersServed = 0;
        cashiers[i].totalCustomerServiceTime = 0;
        cashiers[i].totalAmountReceived = 0;
        cashiers[i].customerQ = EmptyQueue();
        status[i] = 0;
        counter[i] = 0;
    }   //initialize each cashier

    customerT cust;
    cust = (customerT)malloc(sizeof(*cust));

    int temp = -1;  //temporary variable, arbitrary value initialized

    do{
        printf("--Cashiers Information Start--\n\n");
        for (int j = 0; j < 4; j++){    //display each cashiers' information
            checkCashier(&(cashiers[j]), j);
            if (status[j] == 0) printf("Cashier %d: Available\n", j); else printf("Cashier %d: Serving...\n", j);
            printf("\n");
        }

        for (int j = 0; j < 4; j++){    //if there is customer in the queue that waiting for service, add up the time
            if (QueueLength(cashiers[j].customerQ) > 0) waiting += 1;
        }

        printf("--Cashiers Information End--\n\n");

        if (!feof(datafile)){       //if not end of the file
            fgets(line, 100, datafile);
            if (strcmp(line, "\n") != 0){   //if not empty line
                sscanf(line, "%d %d %f", &(cust->checkoutTime), &(cust->nItems), &(cust->payment));     //pass information from datafile
                printf("A customer come. Customer information: %d minute(s) checkout time, %d item(s), $%.1f in total.\n", cust->checkoutTime, cust->nItems, cust->payment);

                int i;
                if (cust->nItems <= 5) i = 0; else i = 1;   //if less than 5 items, cashier 0 could be chosen
                temp = i;
                for (i; i < 4; i++){
                    if (status[i] == 0){    //if there is cashier available right now
                        temp = i;
                        break;
                    }
                    if(status[temp] > status[i]){   //find the shortest queue
                        temp = i;
                    }
                }
                Enqueue(cashiers[temp].customerQ, cust);       //Enqueue the customer into the queue of chosen cashier
                status[temp]++;     //Number of customer in the chosen cashier +1
                printf("Customer went to cashier %d. In place %d.\n\n", temp, QueueLength(cashiers[temp].customerQ));
            }
            else{       //if empty line
                printf("No customer come..\n");
            }
        }
        else{   //if all data is read
            printf("No customer come.\n");
            printf("\n");
            end = 1;
        }
        mins++;     //a minutes passed
        printf("\n****%d minute(s) passed...****\n\n\n", mins);
        for (int i = 0; i < 4; i++){
            if (status[i] > 0) end = 0;
        }   //if all data read and all cashier are available (Job finished)
    }while (end == 0);  //when end == 1, escape the do-while loop
    printf("####No more customer.####\n\n\n");
    showResult(cashiers);   //display the report
    printf("\n======Program End======");
    fclose(datafile);
    return 0;
}

