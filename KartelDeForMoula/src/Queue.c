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

void trierElemDistance(QNODE* elem, QUEUE * f, int lastSorted){
    QNODE* current = f->head;

    int index = 0;

    if(elem == f->head){
        return;
    }
    if(current->value->distanceToEnd > elem->value->distanceToEnd){
        elem->pred->next = elem->next;
        if(elem->next == NULL){
            f->tail = elem->pred;
            elem->pred->next = NULL;
        }else{
            elem->next->pred = elem->pred;
        }

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

            /*Appear before current*/
            elem->pred = current->pred;
            elem->next = current;

            /*Change current*/
            current->pred->next = elem;
            current->pred = elem;
            return;
        }
        current = current->next;
    }
}

/*
* Trie un element de la liste dans la liste (a utiliser apres enqueue
*/
void trierElemGlobal(QNODE* elem, QUEUE * f){
    QNODE* current = f->head;

    if(elem == f->head){
        return;
    }

    /*switch with head*/
    if(current->value->fGlobalGoal > elem->value->fGlobalGoal){
        elem->pred->next = elem->next;
        if(elem->next == NULL){
            f->tail = elem->pred;
            elem->pred->next = NULL;
        }else{
            elem->next->pred = elem->pred;
        }
        
        elem->pred = NULL;
        elem->next = current;
        f->head = elem;
        current->pred = elem;
        return;
    }


    while(current!= NULL){
        if(elem->value->fGlobalGoal < current->value->fGlobalGoal){
            elem->pred->next = elem->next;

            if(elem == f->tail){
                f->tail = elem->pred;
            }else{
                elem->next->pred = elem->pred;
            }

            /*Appear before current*/
            elem->pred = current->pred;
            elem->next = current;

            /*Change current*/
            current->pred->next = elem;
            current->pred = elem;
            return;
        }
        current = current->next;
    }
}

void dequeue(QUEUE * f){

    if( f == NULL ){
        perror("Uninitialized queue");
        exit(EXIT_FAILURE);
    }

    if(f->head){
        f->head = f->head->next;
        f->head->pred = NULL;
    }else{
        perror("List is already empty.");
        exit(EXIT_FAILURE);
    }
}

QNODE* find(NODE* val, QUEUE* q){
    QNODE * n;

    n = q->head;

    if(is_empty(q)){
        return NULL;
    }
    while(n != NULL){
        if(n->value == val)
            return n;
        n = n->next;
    }
    
    return NULL;
}

void display_distance(const QUEUE * f){

    QNODE * n;

    n = f->head;

    if(is_empty(f)){
        return;
    }

    while(n != NULL){
        fprintf(stderr,"%d - ", n->value->distanceToEnd);
        n = n->next;
    }

    fprintf(stderr,"\n");

}

void display_global(const QUEUE * f){

    QNODE * n;

    n = f->head;

    if(is_empty(f)){
        return;
    }

    while(n != NULL){
        fprintf(stderr,"%f - ", n->value->fGlobalGoal);
        n = n->next;
    }

    fprintf(stderr,"\n");

}

int memory_size(const QUEUE * f){
    int result;
    QNODE * n;

    if( f == NULL ){
        perror("Uninitialized queue");
        exit(EXIT_FAILURE);
    }

    result = sizeof(*f);
    printf("%d\n",result);

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

void delete_queue(QUEUE * q){
    
    while(q->head != NULL){
        dequeue(q);
    }
    free(q);

}