#include "Queue.h"

QUEUE * create_queue(){

    QUEUE* f = malloc(sizeof(QUEUE));
    f->head = NULL;
    f->tail = NULL;

    if(!f){
        perror("There is an error during the creation of the QUEUE");
        exit(EXIT_FAILURE);
    }

    return f;

}

int is_empty(const QUEUE * f){
    //1 = empty, 0 = not empty; -1: not initialised
    if( f == NULL ){
        perror("Uninitialized queue");
        exit(EXIT_FAILURE);
    }

    if(f->head != NULL){
        return 0;
    }

    return 1;

}

void enqueue(NODE* val, QUEUE * f){

    QNODE* n = malloc(sizeof(QNODE));

    if(!n){
        perror("Malloc error in enqueue");
        exit(EXIT_FAILURE);
    }

    n->next = NULL;
    n->value = val;

    if(is_empty(f)){
        f->head = n;
        f->tail = n;
        n->pred = NULL;
    }else{
        f->tail->next = n;
        n->pred = f->tail;
        f->tail = n;
    }
}

void trierElem(QNODE* elem, QUEUE * f, int lastSorted){
    QNODE* current = f->head;
    //QNODE* temp;
    int index = 0;

    if(elem == f->head){
        return;
    }

    //switch with head
    if(current->value->distanceToEnd > elem->value->distanceToEnd){
        //fprintf(stderr,"HEAD\n");
        elem->pred->next = elem->next;
        elem->next->pred = elem->pred;
        elem->pred = NULL;
        elem->next = current;
        f->head = elem;
        current->pred = elem;
        return;
    }

    while(current!= NULL && index <= lastSorted){
        index++;
        if(elem->value->distanceToEnd < current->value->distanceToEnd){
            
            elem->pred->next = elem->next;

            if(elem == f->tail){
                f->tail = elem->pred;
            }else{
                elem->next->pred = elem->pred;
            }

            //Appear before current
            elem->pred = current->pred;
            elem->next = current;

            //Change current
            current->pred->next = elem;
            current->pred = elem;
            return;
        }
        current = current->next;
    }
    fprintf(stderr,"NO CHANGE %d\n",elem->value->distanceToEnd);
}

void dequeue(QUEUE * f){

    if( f == NULL ){
        perror("Uninitialized queue");
        exit(EXIT_FAILURE);
    }

    if(f->head){
        f->head = f->head->next;
        f->head->pred = NULL;
        //free(n);
        //n = NULL;
    }else{
        perror("List is already empty.");
        exit(EXIT_FAILURE);
    }
}

QNODE* find(NODE* val, QUEUE* q){
    QNODE * n;

    n = q->head;

    if(is_empty(q)){
        fprintf(stderr,"NULL empty\n");
        return NULL;
    }
    while(n != NULL){
        if(n->value == val)
            return n;
        n = n->next;
    }
    fprintf(stderr,"Not found NULL\n");
    return NULL;
}

void display(const QUEUE * f){

    QNODE * n;

    n = f->head;

    if(is_empty(f)){
        //printf("List is already empty.");
        return;
    }

    //printf("Display of a list: \n");

    while(n != NULL){
        //fprintf(stderr,"%d %d - ", n->value->x, n->value->y);
        fprintf(stderr,"%d - ", n->value->distanceToEnd);
        n = n->next;
    }

    fprintf(stderr,"\n");

}

int memory_size(const QUEUE * f){

    if( f == NULL ){
        perror("Uninitialized queue");
        exit(EXIT_FAILURE);
    }

    int result;

    result = sizeof(*f);
    printf("%d\n",result);

    QNODE * n;

    n = f->head;

    while(n != NULL){
        n = n->next;
    }

    return result ;
}

void concat(QUEUE * q1, QUEUE * q2){

    q1->tail->next = q2->head;
    q1->tail = q2->tail;

}

QUEUE * copie(QUEUE * q){

    QUEUE * q1 = create_queue();

    QNODE * n;
    QNODE * pre = NULL;
    QNODE * next_node;

    n = q->head;

    if(n == NULL){
        q1->head = NULL;
        q1->tail = NULL;
        return q1;
    }

    //head
    pre= malloc(sizeof(QNODE));
    pre->next = NULL;
    pre->value = n->value;

    q1->head = pre;
    
    n = n->next;
    //body
    while(n != NULL){
        
        next_node= malloc(sizeof(QNODE));
        next_node->next = NULL;
        next_node->value = n->value;

        pre->next = next_node;

        pre = next_node;

        n = n->next;
    }
    //tail
    q1->tail = pre;

    return q1;

}

void delete_queue(QUEUE * q){
    
    while(q->head != NULL){
        dequeue(q);
    }
    free(q);


}