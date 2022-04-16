#ifndef __STAR__
#define __STAR__

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define NUMBER_OF_NEIBOURGH 4


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
    struct node** vecNeighbours; //Array
    int numberOfNeighbours;
    struct node* parent;        //One element
} NODE;

NODE * createNode();

NODE* createNodeMap(int width, int height);

void display_node_map(NODE * map, int width, int height, VECT2D** path);

void Solve_AStar(NODE * nodes, int width, int height, NODE * nodeStart, NODE * nodeEnd);

float distance(NODE * nodeA, NODE * nodeB);

float heuristic(NODE * nodeA, NODE * nodeB);

void sort_nodes_list(NODE ** list, int last_push);

void shift_left(NODE ** list, int last_push);

VECT2D ** get_path(NODE * nodes, int width, int height, NODE * nodeStart, NODE * nodeEnd);

VECT2D * create_vector(int x, int y);

void display_vector(VECT2D ** path);

#endif


