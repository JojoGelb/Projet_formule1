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

typedef struct heatPoint{
    int x,y,intensity;
} heatPoint;

typedef struct node{
    char bObstacle;
    char sable;
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
    int distanceToEnd;
    char end;
} NODE;

/**
 * @brief Retourne la liste avec toutes les vitesses possible 
 */
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

void reversePath(VECT2D ** path);

NODE* Solve_AStar(NODE *** nodes, int width, int height, NODE * nodeStart, NODE * nodeEnd, int carburant);

VECT2D ** get_path(NODE *** nodes, int width, int height, NODE * nodeStart, NODE * nodeEnd, int carburant);

VECT2D nextAcceleration(VECT2D ** path, VECT2D* position,VECT2D* lastSpeed, int index);

void generate_heat_map(NODE *** nodesMap, int width, int height, NODE * start, NODE* end);

int gasConsumption(int accX, int accY, int speedX, int speedY, int inSand);

void display_heat_map(NODE *** map, int width, int height);

void sort_heat_list(NODE ** list, int last_push);

#endif