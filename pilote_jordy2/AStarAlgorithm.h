#ifndef __STAR__
#define __STAR__

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define NUMBER_OF_NEIBOURGH 9
#define NUMBER_OF_SPEED 81


typedef enum {
    FALSE,
    TRUE
} bool_t;

typedef struct vect2D{
    int x,y;
} VECT2D;

typedef struct node{
    char bObstacle;
    char bVisited;
    float fGlobalGoal;
    float fLocalGoal;
    int x;
    int y;
    int speedX;
    int speedY;
    struct node** vecNeighbours; //Array
    int numberOfNeighbours;
    struct node* parent;        //One element

    //add
    char border;
    char end;
} NODE;

VECT2D * generateListIndex();

NODE * createNode();

NODE*** createNodeMap(VECT2D* tab,int width, int height);

VECT2D * create_vector(int x, int y);

void display_vector(VECT2D ** path);

void display_node_map(NODE *** map, int width, int height, VECT2D** path);

void sort_nodes_list(NODE ** list, int last_push, int* lastBigNumber);

void shift_left(NODE ** list, int last_push);

float distance(NODE * nodeA, NODE * nodeB);

int findIndex(VECT2D* tab,char speedX, char speedY);

float heuristic(NODE * nodeA, NODE * nodeB);
float heuristicEnd(NODE * nodeA, NODE * nodeB);
void reversePath(VECT2D ** path);

void Solve_AStar(NODE *** nodes, int width, int height, NODE * nodeStart, NODE * nodeEnd);

VECT2D ** get_path(NODE *** nodes, int width, int height, NODE * nodeStart, NODE * nodeEnd);

VECT2D nextAcceleration(VECT2D ** path, VECT2D* position,VECT2D* lastSpeed, int index);

#endif


