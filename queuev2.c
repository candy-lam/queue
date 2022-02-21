#include <stdlib.h>
#include <stdio.h>
#include "queue.h"
#define nElements 100

struct queueCDT{
    int element[nElements];/* max number of elements is 99*/
    int front;
    int rear;
};

queueADT EmptyQueue(void){
    queueADT q;
    q = (queueADT)malloc(sizeof(*q));
    q->front = 1; // initialize front to 1
    q->rear = 0; //initialize rear to 0
    return(q);
}

void Enqueue(queueADT q, queueElementT element){
    queueElementT value = element;
    if ((q->rear + 2) % nElements == q->front){
        printf("Queue is full.\n");
        return;
    } //when the queue if full
    else{
        q->rear = (q->rear + 1) % nElements;
        q->element[q->rear] = value;
    }
}

queueElementT Dequeue(queueADT q){
    int result;
    if (q->front == (q->rear + 1) % nElements){
        printf("Queue is empty.\n");
        exit(EXIT_FAILURE);
    } // when the queue is empty
    result = q->element[q->front];
    q->element[q->front] = NULL; // Keep the element[front - 1] unused
    q->front = (q->front + 1) % nElements;
    return(result);
}

int QueueLength(queueADT q){
    int n;
    if (q->rear > q->front)
        n = q->rear - q->front + 1;
    else if (q->rear == q->front)
        n = 1;
    else if (QueueIsEmpty(q))
        n = 0;
    else
        n = q->rear + (99 - q->front) + 1;
    return n;
}

int QueueIsEmpty(queueADT q){
    return(q->front == (q->rear + 1) % nElements);
}


