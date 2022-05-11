#ifndef __QUEUE__
#define __QUEUE__

#include <stdio.h>
#include <stdlib.h>
#include "AStarAlgorithm.h"

typedef struct qNode QNODE;
typedef struct queue QUEUE;

struct qNode {
    NODE* value;
    QNODE *next;
    QNODE *pred;
};

struct queue {
    QNODE *head;
    QNODE *tail;
};

/**
 * Create and return a pointeur on a QUEUE.
 */
QUEUE * create_queue();

/**
 * Determine if a QUEUE is emptyy.
 * @param f a pointer on a QUEUE.
 * @return int: 1 = empty, 0 = not empty; -1: not initialised
 */
int is_empty(const QUEUE * f);

/**
 * Add a number at the end of the QUEUE.
 * @param val the number to add to the QUEUE.
 * @param f the targeted QUEUE.
 */
void enqueue(NODE* val, QUEUE * f);

/**
 * Remove the last number from a QUEUE.
 * @param f the targeted QUEUE.
 */
void dequeue(QUEUE * f);

/**
 * Display every elements of a QUEUE.
 * @param f the targeted QUEUE.
 */
void display_distance(const QUEUE * f);

/**
 * Return the memory size of every element in a QUEUE
 * @param f the targeted QUEUE.
 */
int memory_size(const QUEUE * f);

/**
 * Concatenate two QUEUE
 * @param q1 the fist QUEUE to be modified.
 * @param q2 the second QUEUE.
 * @note easily produce bugs when we aren't carefull during free process because q2 become part of q1
 */
void concat(QUEUE * q1, QUEUE * q2);

/**
 * Free the memory space occupied by a QUEUE
 * @param q1 the targeted QUEUE.
*/
void delete_queue(QUEUE * q);

void trierElemDistance(QNODE* elem, QUEUE * f, int lastSorted);

void trierElemGlobal(QNODE* elem, QUEUE * f);

QNODE* find(NODE* val, QUEUE* q);

void display_global(const QUEUE * f);

#endif