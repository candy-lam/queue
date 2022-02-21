#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "queue.h"


typedef struct cashier cashierT;
int waiting = 0;
int status[4], counter[4];

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


void showResult(cashierT cash[]){


    int totalSC;
    printf("***Report***\n");
    for (int i = 0; i < 4; i++){
        printf("-Cashier %d-\n", i);
        printf("Number Of Customers Served: %d\n", cash[i].numberOfCustomersServed);
        totalSC += cash[i].numberOfCustomersServed;
        printf("Total Customer Service Time: %d\n", cash[i].totalCustomerServiceTime);
        printf("Total Amount Received: $%.1f\n\n", cash[i].totalAmountReceived);
    }
    float average = (float) waiting / (float) totalSC;
    printf("Average waiting time: %.2f min(s)\n", average);
}

void checkCashier(cashierT *cash, int num){
    customerT current;
    current = (customerT)malloc(sizeof(*current));
    printf("=Cashier %d=\n", num);
    //printf("Queue Length: %d\n", QueueLength((cash->customerQ)));
    printf("No. of customer in the cashier: %d\n", status[num]);
    //if (status[num] == 0) printf("Cashier %d: Available\n", num); else printf("Cashier %d: Serving...\n", num);
    if (QueueLength(cash->customerQ) == 1 && counter[num] <= 0){
        //printf("counter[%d]: %d\n", num, counter[num]);
        current = Dequeue(cash->customerQ);
        cash->numberOfCustomersServed++;
        cash->totalCustomerServiceTime += current->checkoutTime;
        cash->totalAmountReceived += current->payment;
        //printf("current->checkoutTime (after): %d\n", current->checkoutTime);
        counter[num] = current->checkoutTime;
        //printf("counter[%d] (after): %d\n", num, counter[num]);
    }

    counter[num] --;
    if (counter[num] > 0) printf("Remaining serving time: %d\n", counter[num]);

    if (counter[num] == 0){
        status[num] --;
        printf("Previous customer in cashier %d left.\n", num);
        current = NULL;
        printf("No. of customer in cashier : %d\n", status[num]);
        if (status[num] > 0){
            printf("Next customer start to be served.\n");
            current = Dequeue(cash->customerQ);
            //printf("Queue Length: %d\n", QueueLength((cash->customerQ)));
            counter[num] = current->checkoutTime;
            //printf("counter[%d] (after): %d\n", num, current->checkoutTime);
            cash->numberOfCustomersServed++;
            cash->totalCustomerServiceTime += current->checkoutTime;
            cash->totalAmountReceived += current->payment;
        }
        else{
            printf("No customer in queue %d now.\n", num);
        }
    }
}


int main(){
    FILE *datafile, *renewfile;
    datafile = fopen("supermarket.dat","r");

    char line[100];
    cashierT cashiers[4];
    int end = 0;
    int mins = 0;

    for (int i = 0; i < 4; i++){
        cashiers[i].numberOfCustomersServed = 0;
        cashiers[i].totalCustomerServiceTime = 0;
        cashiers[i].totalAmountReceived = 0;
        cashiers[i].customerQ = EmptyQueue();
        status[i] = 0;
        counter[i] = 0;
    }

    customerT cust;
    int temp = -1, wait;
    cust = (customerT)malloc(sizeof(*cust));
    customerT tempo;
    tempo = (customerT)malloc(sizeof(*tempo));

    do{
        printf("--Cashiers Information Start--\n\n");
        for (int j = 0; j < 4; j++){
            //printf("1st check Queue Length: %d\n", QueueLength((cashiers[j].customerQ)));
            checkCashier(&(cashiers[j]), j);
            if (status[j] == 0) printf("Cashier %d: Available\n", j); else printf("Cashier %d: Serving...\n", j);
            printf("\n");
        }

        for (int j = 0; j < 4; j++){
            if (QueueLength(cashiers[j].customerQ) > 0) waiting += 1;
        }

        printf("--Cashiers Information End--\n\n");

        if (!feof(datafile)){
            fgets(line, 100, datafile);
            if (strcmp(line, "\n") != 0){
                sscanf(line, "%d %d %f", &(cust->checkoutTime), &(cust->nItems), &(cust->payment));
                printf("A customer come. Customer information: %d minute(s) checkout time, %d item(s), $%.1f in total.\n", cust->checkoutTime, cust->nItems, cust->payment);
                int i;
                if (cust->nItems <= 5) i = 0; else i = 1;
                temp = i;
                for (i; i < 4; i++){
                    if (status[i] == 0){
                        temp = i;
                        break;
                    }
                    if(status[temp] > status[i]){
                        temp = i;
                    }
                }
                //for (int i = 0; i < 4; i++)
                //printf("Check Queue Length: %d\n", QueueLength((cashiers[i].customerQ)));
                Enqueue(cashiers[temp].customerQ, cust);
                status[temp]++;

                printf("Customer went to cashier %d. In place %d.\n\n", temp, QueueLength(cashiers[temp].customerQ));
            }
            else{
                //for (int i = 0; i < 4; i++)
                //printf("Check Queue Length: %d\n", QueueLength((cashiers[i].customerQ)));

                printf("No customer come..\n");
            }
        }
        else{
            printf("No customer come.\n");
            printf("\n");
        }
        mins++;
        printf("\n****%d minute(s) passed...****\n\n\n", mins);
        end = 1;
        for (int i = 0; i < 4; i++){
            if (status[i] > 0) end = 0;
        }
    }while (end == 0);
    printf("####No more customer.####\n\n\n");
    showResult(cashiers);
    printf("\n======Program End======");
    fclose(datafile);
    return 0;
}

