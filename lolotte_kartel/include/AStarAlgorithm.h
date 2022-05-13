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
    char sable;
    char bVisited;
    double fGlobalGoal;
    double fLocalGoal;
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

void allocationSpeedForNormalNode(NODE ***nodes, VECT2D *tab, int width, int x, int y);

void allocationSpeedForSandNode(NODE ***nodes, VECT2D *tab, int width, int x, int y);

void allocationSpeedForEndNode(NODE ***nodes, VECT2D *tab, int width, int x, int y);

void findNeighbourForNormalNode(NODE ***nodes, VECT2D *tab, int width, int height, int x, int y);

void findNeighbourForSandNode(NODE ***nodes, VECT2D *tab, int width, int height, int x, int y);

VECT2D * create_vector(int x, int y);

void display_vector(NODE ** path);

void display_node_map(NODE *** map, int width, int height, NODE** path);

void sort_nodes_list(NODE ** list, int last_push, int* lastBigNumber);

void shift_left(NODE ** list, int last_push);

float distance(NODE * nodeA, NODE * nodeB);

int findIndex(VECT2D* tab,char speedX, char speedY);

float heuristic(NODE * nodeA);

void reversePath(NODE ** path);

NODE* Solve_AStar(NODE *** nodes, int width, int height, NODE * nodeStart);

NODE ** get_path(NODE *** nodes, int width, int height, NODE * nodeStart);

NODE **get_path_essence(NODE ***nodes, int width, int height, NODE *nodeStart, VECT2D *tab, int * carburant);

NODE *Solve_AStar_Essence(NODE ***nodes, int width, int height, NODE *nodeStart, VECT2D *tab);

VECT2D nextAcceleration(NODE ** path, VECT2D* lastPosition, VECT2D* lastSpeed, int index);

void generate_heat_map(NODE *** nodesMap, int width, int height, NODE * start);

int gasConsumption(int accX, int accY, int speedX, int speedY, int inSand);

void display_heat_map(NODE *** map, int width, int height);

void sort_heat_list(NODE ** list, int last_push);

int calculConsommationEssenceSurTrajet(NODE ** path,int indexFin);

int hit_a_wall(NODE ***nodeMap, int width, NODE *start, NODE *stop);

void resetMapPlayersPosition(NODE *** mapNodes, int mapWidth, NODE ***lastP2Position, NODE ***lastP3Position, VECT2D player2Position, VECT2D player3Position);

#endif