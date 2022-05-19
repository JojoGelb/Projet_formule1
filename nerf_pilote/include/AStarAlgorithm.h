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
    struct node** vecNeighbours; /*Array*/
    int numberOfNeighbours;
    struct node* parent;        /*One element*/

    int distanceToEnd;
    char end;
} NODE;

/**
 * @brief Retourne la liste avec tout les vecteurs de vitesse possibles que peut prendre le pilote
 */
VECT2D * generateListIndex();

/**
 * @brief alloue un noeud 
 */
NODE * createNode();

/**
 * @brief Alloue la mémoire de toutes les cases de la carte
 * 
 * @param width la largeur de la carte
 * @param height la hauteur de la carte
 * @return NODE*** 
 */
NODE*** createNodeMap(int width, int height);

/**
 * @brief Alloue la place pour toutes les vitesses associées à un noeud normal
 * 
 * @param nodes la carte du circuit
 * @param tab la liste avec tout les vecteurs de vitesse possibles que peut prendre le pilote
 * @param width la largeur de la carte
 * @param x la position en x du noeud sur la carte 
 * @param y la position en y du noeud sur la carte
 */
void allocationSpeedForNormalNode(NODE ***nodes, VECT2D *tab, int width, int x, int y);

/**
 * @brief Alloue la place pour toutes les vitesses associées à un noeud de sable
 * 
 * @param nodes la carte du circuit
 * @param tab la liste avec tout les vecteurs de vitesse possibles que peut prendre le pilote
 * @param width la largeur de la carte
 * @param x la position en x du noeud sur la carte 
 * @param y la position en y du noeud sur la carte
 */
void allocationSpeedForSandNode(NODE ***nodes, VECT2D *tab, int width, int x, int y);

/**
 * @brief Alloue la place pour toutes les vitesses associées à un noeud d'arrivée
 * 
 * @param nodes la carte du circuit
 * @param tab la liste avec tout les vecteurs de vitesse possibles que peut prendre le pilote
 * @param width la largeur de la carte
 * @param x la position en x du noeud sur la carte 
 * @param y la position en y du noeud sur la carte
 */
void allocationSpeedForEndNode(NODE ***nodes, VECT2D *tab, int width, int x, int y);

/**
 * @brief Associe chacun des voisins à un noeud normal pour chaque possibilité de vitessse que peut avoir le pilote en partant de ce noeud
 * 
 * @param nodes la carte du circuit
 * @param tab la liste avec tout les vecteurs de vitesse possibles que peut prendre le pilote
 * @param width la largeur de la carte
 * @param height la hauteur de la carte
 * @param x la position en x du noeud sur la carte 
 * @param y la position en y du noeud sur la carte
 */
void findNeighbourForNormalNode(NODE ***nodes, VECT2D *tab, int width, int height, int x, int y);

/**
 * @brief Associe chacun des voisins à un noeud de sable pour chaque possibilité de vitessse que peut avoir le pilote en en partant de ce noeud
 * 
 * @param nodes la carte du circuit
 * @param tab la liste avec tout les vecteurs de vitesse possibles que peut prendre le pilote
 * @param width la largeur de la carte
 * @param height la hauteur de la carte
 * @param x la position en x du noeud sur la carte 
 * @param y la position en y du noeud sur la carte
 */
void findNeighbourForSandNode(NODE ***nodes, VECT2D *tab, int width, int height, int x, int y);

/**
 * @brief Créer un vecteur à deux dimensions
 * 
 * @param x la valeur x associée au vecteur
 * @param y la valeur x associée au vecteur
 * @return VECT2D* 
 */
VECT2D * create_vector(int x, int y);

/**
 * @brief affiche un tous les vecteurs d'accélération que doit prendre notre piloe pour arriver à l'arrivée 
 * 
 * @param path la liste des vecteurs d'accélération
 */
void display_vector(NODE ** path);

/**
 * @brief Affiche la carte du circuit
 * 
 * @param map la carte du circuit
 * @param width la taille de la carte
 * @param height la hauteur de la carte
 * @param path la liste des vecteurs d'accélération de notre pilote si l'on souhaite afficher son trajet
 */
void display_node_map(NODE *** map, int width, int height, NODE** path);

void sort_nodes_list(NODE ** list, int last_push, int* lastBigNumber);

void shift_left(NODE ** list, int last_push);

/**
 * @brief calcul la distance de Manhattan entre deux noeuds
 * 
 * @param nodeA le noeud de départ
 * @param nodeB le noeud d'arrivée
 * @return float 
 */
float distance(NODE * nodeA, NODE * nodeB);

/**
 * @brief Trouve l'indice d'un noeud dans une case du circuit grâce à sa vitesse
 * 
 * @param tab la liste des vecteurs de vitesses possibles d'un pilote
 * @param speedX la vitesse en x associée au noeud que l'on cherche
 * @param speedY la vitesse en y associée au noeud que l'on cherche
 * @return int 
 */
int findIndex(VECT2D* tab,char speedX, char speedY);

float heuristic(NODE * nodeA);

void reversePath(NODE ** path);

NODE* Solve_AStar(NODE *** nodes, int width, int height, NODE * nodeStart, char slowDown);

NODE ** get_path(NODE *** nodes, int width, int height, NODE * nodeStart, int* autreConso, char slowDown);

NODE **get_path_essence(NODE ***nodes, int width, int height, NODE *nodeStart, int * carburant);

NODE *Solve_AStar_Essence(NODE ***nodes, int width, int height, NODE *nodeStart);

VECT2D nextAcceleration(NODE ** path, VECT2D* lastPosition, VECT2D* lastSpeed, int index);

void generate_heat_map(NODE *** nodesMap, int width, int height, NODE * start);

int gasConsumption(int accX, int accY, int speedX, int speedY, int inSand);

void display_heat_map(NODE *** map, int width, int height);

void sort_heat_list(NODE ** list, int last_push);

/**
 * @brief Calcule la consommation du pilote entre deux points de son trajet
 * 
 * @param path le trajet de noeuds que doit suivre le pilote
 * @param indexDebut l'indice du point dans le trajet où l'on souhaite commencer à calculer la consommation
 * @param indexFin l'indice du point dans le trajet où l'on souhaite arrêter le calcul de la consommation
 * @return int 
 */
int calculConsommationEssenceSurTrajet(NODE ** path, int indexDebut,int indexFin);

/**
 * @brief Recherche si un obstacle se trouve entre deux noeuds
 * 
 * @param nodeMap la carte du circuit
 * @param width la largeur de la carte
 * @param start le noeud de départ
 * @param stop le noeud d'arrivée
 * @return int 
 */
int hit_a_wall(NODE ***nodeMap, int width, NODE *start, NODE *stop);

/**
 * @brief Actualise les cases de la carte où se trouvent les pilotes concurrents
 * 
 * @param mapNodes la carte du circuit
 * @param mapWidth la largeur de la carte
 * @param lastP2Position la case de la carte où le premier pilote concurrent se trouvait au tour précédent
 * @param lastP3Position la case de la carte où le deuxième pilote concurrent se trouvait au tour précédent
 * @param player2Position la position actuelle du premier pilote concurrent
 * @param player3Position la position actuelle du deuxième pilote concurrent
 */
void resetMapPlayersPosition(NODE *** mapNodes, int mapWidth, NODE ***lastP2Position, NODE ***lastP3Position, VECT2D player2Position, VECT2D player3Position);

#endif