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
 * @brief  Retourne la liste avec tout les vecteurs de vitesse possibles que peut prendre le pilote
 * 
 * @return VECT2D* 
 */
VECT2D * generateListIndex();

/**
 * @brief Crée un noeud
 * 
 * @return NODE* 
 */
NODE * createNode();

/**
 * @brief Alloue la mémoire de toutes les cases de la carte
 * 
 * @param width la largeur de la carte
 * @param height la hauteur de la carte
 * @return NODE*** la carte du circuit
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
 * @return VECT2D* le vecteur à deux dimensions crée
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

/**
 * @brief Trie une liste de noeuds à partir d'un index de cette liste
 * 
 * @param list la liste à trier
 * @param last_push la taille de la liste
 * @param lastBigNumber l'index où la liste n'est plus triée
 */
void sort_nodes_list(NODE ** list, int last_push, int* lastBigNumber);

/**
 * @brief dépile la première valeur d'une liste et décale toute les valeurs à gauche
 * 
 * @param list la liste 
 * @param last_push la taille de la liste
 */
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

/**
 * @brief Retourne l'heuristique d'un noeud
 * 
 * @deprecated
 * @param nodeA le neud dont on veut l'heuristique
 * @return float 
 */
float heuristic(NODE * nodeA);

/**
 * @brief Inverse un liste de noeuds
 * 
 * @param path la liste à inverser
 */
void reversePath(NODE ** path);

/**
 * @brief Calcule la trajet que le pilote doit prendre pour finir le circuit le plus rapidement possible, via l'algorithme de pathfinding A*
 * 
 * @param nodes la carte du circuit
 * @param width la largeur de la carte
 * @param height la hauteur de la carte
 * @param nodeStart le noeud de départ du pilote
 * @param slowDown indique de combien on veut réduire la vitesse maximale du pilote
 * @return NODE* le noeud où devra arriver le pilote, prend la valeur null si on ne trouve pas de trajet
 */
NODE* Solve_AStar(NODE *** nodes, int width, int height, NODE * nodeStart, char slowDown);

/**
 * @brief  Retourne le trajet à suivre par le pilote pour se déplacer le plus rapidement possible
 * Résoud le A* puis stocke le résultat dans un tableau de noeuds (position à suivre)
 * 
 * @param nodes la carte du circuit
 * @param width la largeur de la carte
 * @param height la hauteur de la carte
 * @param nodeStart le noeud de départ du pilote
 * @param autreConso la variable qui va contenir le coût en essence total du trajet
 * @param slowDown indique de combien on veut réduire la vitesse maximale du pilote
 * @return NODE** le tableau contenant les noeuds par lesquels doit passer le pilote
 */
NODE ** get_path(NODE *** nodes, int width, int height, NODE * nodeStart, int* autreConso, char slowDown);

/**
 * @brief Retourne le trajet à suivre par le pilote pour se déplacer le plus rapidement possible
 * Résoud le A* puis stocke le résultat dans un tableau de noeuds (position à suivre)
 * 
 * @param nodes la carte du circuit
 * @param width la largeur de la carte
 * @param height la hauteur de la carte
 * @param nodeStart le noeud de départ du pilote
 * @param carburant la variable qui va contenir le coût en essence total du trajet
 * @return NODE** le tableau contenant les noeuds par lesquels doit passer le pilote
 */
NODE **get_path_essence(NODE ***nodes, int width, int height, NODE *nodeStart, int * carburant);

/**
 * @brief Calcule la trajet que le pilote doit prendre pour finir le circuit en consommant le moins d'essence possible, via l'algorithme de pathfinding A*
 * 
 * @param nodes la carte du circuit
 * @param width la largeur de la carte
 * @param height la hauteur de la carte
 * @param nodeStart le noeud de départ du pilote
 * @return NODE* le noeud où devra arriver le pilote, prend la valeur null si on ne trouve pas de trajet
 */
NODE *Solve_AStar_Essence(NODE ***nodes, int width, int height, NODE *nodeStart);

/**
 * @brief Retourne la prochaine accélération que le pilote doit prendre pour suivre son trajet
 * 
 * @param path la liste de points où le pilote doit passer pour aller jusqu'à l'arrivée
 * @param lastPosition la derniere position x,y du pilote
 * @param lastSpeed la derniere vitesse x,y du pilote
 * @param index le nombre de point où le joueur est déjà passé sur le trajet en cours
 * @return VECT2D le vecteur à deux dimension de la prochaine accélération
 */
VECT2D nextAcceleration(NODE ** path, VECT2D* lastPosition, VECT2D* lastSpeed, int index);

/**
 * @brief Génère la heat map : met à jour la distance par rapport à la fin de la course de chaque noeuds.
 * Utilise l'algorithme de pathfinding Dijkstra
 * 
 * @param nodesMap la carte du circuit
 * @param width la largeur de la carte
 * @param height la hauteur de la carte
 * @param start noeuds où doit arriver le pilote en fin de course
 */
void generate_heat_map(NODE *** nodesMap, int width, int height, NODE * start);

/**
 * @brief Compute the gas consumption of a requested acceleration
 *
 * CAUTION: Even an illegal move will result in gas consumption. Producing
 * illegal moves should be prevented as much as possible!
 *
 * @param accX Acceleration x component
 * @param accY Acceleration y component
 * @param speedX Speed x component
 * @param speedY Speed y component
 * @param inSand (boolean)
 * @return Number of gas units consumed
 */
int gasConsumption(int accX, int accY, int speedX, int speedY, int inSand);

/**
 * @brief Affiche la carte du circuit
 * 
 * @param map la carte du circuit
 * @param width la largeur du circuit
 * @param height la hauteur du circuit
 */
void display_heat_map(NODE *** map, int width, int height);

/**
 * @brief Calcule la consommation du pilote entre deux points de son trajet
 * 
 * @param path le trajet de noeuds que doit suivre le pilote
 * @param indexDebut l'indice du point dans le trajet où l'on souhaite commencer à calculer la consommation
 * @param indexFin l'indice du point dans le trajet où l'on souhaite arrêter le calcul de la consommation
 * @return int la consommation totale rentre les deux intervals
 */
int calculConsommationEssenceSurTrajet(NODE ** path, int indexDebut,int indexFin);

/**
 * @brief Recherche si un obstacle se trouve entre deux noeuds
 * 
 * @param nodeMap la carte du circuit
 * @param width la largeur de la carte
 * @param start le noeud de départ
 * @param stop le noeud d'arrivée
 * @return int 1 si un obstacle est sur la route, 0 sinon
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