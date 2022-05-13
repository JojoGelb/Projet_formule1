#include "AStarAlgorithm.h"
#include "../follow_line/follow_line.h"
#include "Queue.h"

NODE *createNode() {
    NODE *node;
    node = (NODE *)malloc(sizeof(NODE));
    if (node == NULL) {
        fprintf(stderr, "Crash during node creation\n");
        exit(-1);
    }
    node->vecNeighbours = (NODE **)malloc(NUMBER_OF_NEIBOURGH * sizeof(*node->vecNeighbours));
    return node;
}

/** Trouve pour une vitesse sa position dans le tableau tab*/
int findIndex(VECT2D *tab, char speedX, char speedY) {
    int i;
    for (i = 0; i < NUMBER_OF_SPEED; i++) {
        if (tab[i].x == speedX && tab[i].y == speedY) {
            return i;
        }
    }
    return -1;
}

/** Genrere une liste avec toutes les combinaisons de vitesses possible*/
VECT2D *generateListIndex() {
    int index, i, j;
    VECT2D *result;

    result = (VECT2D *)malloc(NUMBER_OF_SPEED * sizeof(*result));
    index = 0;

    for (i = -5; i < 6; i++) {
        for (j = -5; j < 6; j++) {
            if (sqrt(pow(i, 2) + pow(j, 2)) <= 5) {
                result[index].x = i;
                result[index].y = j;
                index++;
            }
        }
    }
    return result;
}

/** alloue la place pour toute la map */
NODE ***createNodeMap(VECT2D *tab, int width, int height) {
    NODE ***nodes;
    int x, y;

    nodes = (NODE ***)malloc(width * height * sizeof(NODE **));
    for (x = 0; x < width; x++) {
        for (y = 0; y < height; y++) {
            nodes[y * width + x] = (NODE **)malloc(NUMBER_OF_SPEED * sizeof(*nodes[y * width + x]));
        }
    }
    return nodes;
}

/**alloue les 81 vitesses possibles pour un Node normal*/
void allocationSpeedForNormalNode(NODE ***nodes, VECT2D *tab, int width, int x, int y) {
    int i;
    for (i = 0; i < NUMBER_OF_SPEED; i++) {
        nodes[y * width + x][i] = createNode();
        nodes[y * width + x][i]->speedX = tab[i].x;
        nodes[y * width + x][i]->speedY = tab[i].y;
        nodes[y * width + x][i]->x = x;
        nodes[y * width + x][i]->y = y;
        nodes[y * width + x][i]->parent = NULL;
        nodes[y * width + x][i]->bVisited = FALSE;
        nodes[y * width + x][i]->sable = FALSE;
        nodes[y * width + x][i]->end = FALSE;
    }
}

void allocationSpeedForSandNode(NODE ***nodes, VECT2D *tab, int width, int x, int y) {
    int i;
    for (i = 0; i < NUMBER_OF_SPEED; i++) {
        nodes[y * width + x][i] = createNode();
        nodes[y * width + x][i]->speedX = tab[i].x;
        nodes[y * width + x][i]->speedY = tab[i].y;
        nodes[y * width + x][i]->x = x;
        nodes[y * width + x][i]->y = y;
        nodes[y * width + x][i]->parent = NULL;
        nodes[y * width + x][i]->bVisited = FALSE;
        nodes[y * width + x][i]->sable = TRUE;
        nodes[y * width + x][i]->end = FALSE;
    }
}

void allocationSpeedForEndNode(NODE ***nodes, VECT2D *tab, int width, int x, int y) {
    int i;
    for (i = 0; i < NUMBER_OF_SPEED; i++) {
        nodes[y * width + x][i] = createNode();
        nodes[y * width + x][i]->speedX = tab[i].x;
        nodes[y * width + x][i]->speedY = tab[i].y;
        nodes[y * width + x][i]->x = x;
        nodes[y * width + x][i]->y = y;
        nodes[y * width + x][i]->parent = NULL;
        nodes[y * width + x][i]->bVisited = FALSE;
        nodes[y * width + x][i]->sable = FALSE;
        nodes[y * width + x][i]->end = TRUE;
    }
}

/**
 * @brief trouve tous les voisins d'un noeud ensablé
 * @TODO on peut facilement simplifier l'algo
 */
void findNeighbourForSandNode(NODE ***nodes, VECT2D *tab, int width, int height, int x, int y) {

    int findex;
    int num_neigh;
    int x_prime;
    int y_prime;
    int i;
    int norm;

    for (i = 0; i < NUMBER_OF_SPEED; i++) {

        x_prime = nodes[y * width + x][i]->x + nodes[y * width + x][i]->speedX;
        y_prime = nodes[y * width + x][i]->y + nodes[y * width + x][i]->speedY;

        num_neigh = 0;

        norm = abs(nodes[y * width + x][i]->speedX) + abs(nodes[y * width + x][i]->speedY);

        /* Speed == 0*/
        if (norm == 0) {
            /* UP*/
            if (y > 0 && nodes[(y_prime - 1) * width + (x_prime + 0)] != NULL) {
                findex = findIndex(tab, nodes[y * width + x][i]->speedX, nodes[y * width + x][i]->speedY - 1);
                if (findex != -1) {
                    nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime - 1) * width + (x_prime + 0)][findex];
                    num_neigh++;
                }
            }

            /* DOWN*/
            if (y < height - 1 && nodes[(y_prime + 1) * width + (x_prime)] != NULL) {
                findex = findIndex(tab, nodes[y * width + x][i]->speedX, nodes[y * width + x][i]->speedY + 1);
                if (findex != -1) {
                    nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime + 1) * width + (x_prime)][findex];
                    num_neigh++;
                }
            }

            /*LEFT*/
            if (x > 0 && nodes[(y_prime)*width + (x_prime - 1)] != NULL) {
                findex = findIndex(tab, nodes[y * width + x][i]->speedX - 1, nodes[y * width + x][i]->speedY);
                if (findex != -1) {
                    nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime)*width + (x_prime - 1)][findex];
                    num_neigh++;
                }
            }

            /* RIGHT*/
            if (x < width - 1 && nodes[(y_prime)*width + (x_prime + 1)] != NULL) {
                findex = findIndex(tab, nodes[y * width + x][i]->speedX + 1, nodes[y * width + x][i]->speedY);
                if (findex != -1) {
                    nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime)*width + (x_prime + 1)][findex];
                    num_neigh++;
                }
            }
        } else if (norm == 1) { /* Speed x = 1 or -1 || Speed y = -1 or 1*/
            /* Slow down on every sides: CENTER POSITION*/
            nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[y * width + x][findIndex(tab, 0, 0)];
            num_neigh++;

            if (nodes[y * width + x][i]->speedX == 1) {
                /* NO MOOVE : RIGHT*/
                if (x < width - 1 && nodes[y * width + (x + 1)] != NULL) {
                    nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[y * width + (x + 1)][i];
                    num_neigh++;
                }
                /* TOP*/
                if (y > 0 && nodes[(y - 1) * width + x] != NULL) {
                    findex = findIndex(tab, 0, -1);
                    nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y - 1) * width + x][findex];
                    num_neigh++;
                }

                /* Bottom*/
                if (y < height - 1 && nodes[(y + 1) * width + x] != NULL) {
                    findex = findIndex(tab, 0, 1);
                    nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y + 1) * width + x][findex];
                    num_neigh++;
                }

            } else if (nodes[y * width + x][i]->speedX == -1) {
                /* NO MOOVE : LEFT*/
                if (x > 0 && nodes[y * width + (x - 1)] != NULL) {
                    nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[y * width + (x - 1)][i];
                    num_neigh++;
                }
                /* TOP CRASH ICI TOUCHE PLAFOND*/
                if (y > 0 && nodes[(y - 1) * width + x] != NULL) {
                    findex = findIndex(tab, 0, -1);
                    nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y - 1) * width + x][findex];
                    num_neigh++;
                }

                /* Bottom*/
                if (y < height - 1 && nodes[(y + 1) * width + x] != NULL) {
                    findex = findIndex(tab, 0, 1);
                    nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y + 1) * width + x][findex];
                    num_neigh++;
                }
            } else if (nodes[y * width + x][i]->speedY == 1) {
                /* NO MOOVE : BOTTOM*/
                if (y <= height - 1 && nodes[(y + 1) * width + x] != NULL) {
                    nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y + 1) * width + x][i];
                    num_neigh++;
                }
                /* LEFT*/
                if (x > 0 && nodes[y * width + (x - 1)] != NULL) {
                    findex = findIndex(tab, -1, 0);
                    nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[y * width + (x - 1)][findex];
                    num_neigh++;
                }

                /*RIGHT*/
                if (x < width - 1 && nodes[y * width + (x + 1)] != NULL) {
                    findex = findIndex(tab, 1, 0);
                    nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[y * width + (x + 1)][findex];
                    num_neigh++;
                }
            } else if (nodes[y * width + x][i]->speedY == -1) {
                /* NO MOOVE : TOP*/
                if (y > 0 && nodes[(y - 1) * width + x] != NULL) {
                    nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y - 1) * width + x][i];
                    num_neigh++;
                }
                /* LEFT*/
                if (x > 0 && nodes[y * width + (x - 1)] != NULL) {
                    findex = findIndex(tab, -1, 0);
                    nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[y * width + (x - 1)][findex];
                    num_neigh++;
                }
                /* RIGHT*/
                if (x < width - 1 && nodes[y * width + (x + 1)] != NULL) {
                    findex = findIndex(tab, 1, 0);
                    nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[y * width + (x + 1)][findex];
                    num_neigh++;
                }
            }
        } else { /* Too fast to slow down: crash next round = speed 0,0*/
            nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[y * width + x][findIndex(tab, 0, 0)];
            num_neigh++;
        }

        nodes[y * width + x][i]->numberOfNeighbours = num_neigh;
    }
}

/**
 * @brief trouve tous les voisins d'un noeud
 * @TODO on peut facilement simplifier l'algo
 */
void findNeighbourForNormalNode(NODE ***nodes, VECT2D *tab, int width, int height, int x, int y) {

    
    bool_t x_in, y_in;
    int y_prime, x_prime, i, num_neigh, findex;

    num_neigh = 0;
    findex = 0;

    for (i = 0; i < NUMBER_OF_SPEED; i++) {
        y_prime = y + nodes[y * width + x][i]->speedY;
        x_prime = x + nodes[y * width + x][i]->speedX;
        x_in = (x_prime >= 0 && x_prime < width);
        y_in = (y_prime >= 0 && y_prime < height);

        /* 4*/
        if (x_in && y_in && nodes[(y_prime)*width + (x_prime)] != NULL) {
            nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime)*width + (x_prime)][i];
            num_neigh++;
        }

        /* 1*/
        if (y_prime > 0 && y_prime <= height && x_in && nodes[(y_prime - 1) * width + (x_prime + 0)] != NULL) {
            findex = findIndex(tab, nodes[y * width + x][i]->speedX, nodes[y * width + x][i]->speedY - 1);
            if (findex != -1) {
                nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime - 1) * width + (x_prime + 0)][findex];
                num_neigh++;
            }
        }

        /* 7*/
        if (y_prime < height - 1 && y_prime > -1 && x_in && nodes[(y_prime + 1) * width + (x_prime + 0)] != NULL) {
            findex = findIndex(tab, nodes[y * width + x][i]->speedX, nodes[y * width + x][i]->speedY + 1);
            if (findex != -1) {
                nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime + 1) * width + (x_prime + 0)][findex];
                num_neigh++;
            }
        }

        /* 3*/
        if (x_prime > 0 && x_prime <= width && y_in && nodes[(y_prime + 0) * width + (x_prime - 1)] != NULL) {
            findex = findIndex(tab, nodes[y * width + x][i]->speedX - 1, nodes[y * width + x][i]->speedY);
            if (findex != -1) {
                nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime + 0) * width + (x_prime - 1)][findex];
                num_neigh++;
            }
        }

        /* 5*/
        if (x_prime < width - 1 && x_prime >= -1 && y_in && nodes[(y_prime + 0) * width + (x_prime + 1)] != NULL) {
            findex = findIndex(tab, nodes[y * width + x][i]->speedX + 1, nodes[y * width + x][i]->speedY);
            if (findex != -1) {
                nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime + 0) * width + (x_prime + 1)][findex];
                num_neigh++;
            }
        }
        /* 0 carefull not to be out of the map*/
        if (y_prime > 0 && x_prime > 0 && x_prime <= width && y_prime <= height && nodes[(y_prime - 1) * width + (x_prime - 1)] != NULL) {
            findex = findIndex(tab, nodes[y * width + x][i]->speedX - 1, nodes[y * width + x][i]->speedY - 1);
            if (findex != -1) {
                nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime - 1) * width + (x_prime - 1)][findex];
                num_neigh++;
            }
        }

        /* 6*/
        if (y_prime < height - 1 && x_prime > 0 && y_prime >= -1 && x_prime <= width && nodes[(y_prime + 1) * width + (x_prime - 1)] != NULL) {
            findex = findIndex(tab, nodes[y * width + x][i]->speedX - 1, nodes[y * width + x][i]->speedY + 1);
            if (findex != -1) {
                nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime + 1) * width + (x_prime - 1)][findex];
                num_neigh++;
            }
        }

        /* 2*/
        if (y_prime > 0 && x_prime < width - 1 && y_prime <= height && x_prime >= -1 && nodes[(y_prime - 1) * width + (x_prime + 1)] != NULL) {
            findex = findIndex(tab, nodes[y * width + x][i]->speedX + 1, nodes[y * width + x][i]->speedY - 1);
            if (findex != -1) {
                nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime - 1) * width + (x_prime + 1)][findex];
                num_neigh++;
            }
        }

        /* 8*/
        if (y_prime < height - 1 && x_prime < width - 1 && y_prime >= -1 && x_prime >= -1 && nodes[(y_prime + 1) * width + (x_prime + 1)] != NULL) {
            findex = findIndex(tab, nodes[y * width + x][i]->speedX + 1, nodes[y * width + x][i]->speedY + 1);
            if (findex != -1) {
                nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime + 1) * width + (x_prime + 1)][findex];
                num_neigh++;
            }
        }

        nodes[y * width + x][i]->numberOfNeighbours = num_neigh;

        num_neigh = 0;
    }
}

/*
Calcule si la voiture tape un mur lors d'un deplacement
*/
int hit_a_wall(NODE ***nodeMap, int width, NODE *start, NODE *stop) {
    {
        InfoLine vline;
        Pos2Dint p;
        initLine(start->x, start->y, stop->x, stop->y, &vline);
        while (nextPoint(&vline, &p, +1) > 0) {
            if (p.x == start->x && p.y == start->y) {
                /* We suppose that the start position is not worth visiting! */
                continue;
            }
            if (nodeMap[p.y * width + p.x] == NULL) {
                return 1;
            }
        }
        return 0;
    }
}

/** Affiche la map.
 * si path est != de null: affiche la route emprunté représenté par des $ */
void display_node_map(NODE ***map, int width, int height, NODE **path) {

    int i, x, y;
    char c;

    c = '*';
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            if (map[y * width + x] == NULL) {
                fprintf(stderr, ".");
            } else if (map[y * width + x][0]->end == TRUE) {
                fprintf(stderr, "=");
            } else {
                if (path == NULL) {
                    fprintf(stderr, "*");
                } else {
                    for (i = 0; i < 800; i++) {
                        if (path[i] == NULL) {
                            break;
                        }
                        if (path[i]->x == x && path[i]->y == y) {
                            c = '$';
                            break;
                        }
                    }
                    fprintf(stderr, "%c", c);
                    c = '*';
                }
            }
        }
        fprintf(stderr, "\n");
    }
}

/** Affiche position x y stocké dans path*/
void display_vector(NODE **path) {
    int i;
    fprintf(stderr, "START PATH \n");
    for (i = 0; i < 800; i++) {
        if (path[i] == NULL) {
            break;
        }
        fprintf(stderr, "%d %d - ", path[i]->x, path[i]->y);
    }
    fprintf(stderr, "\nEND PATH \n");
}

/**
 * Detecte si un noeud est dans la list
 * smaller: si -1 ne regarde pas le cout, sinon return true si le coup est plus petit que celui de searched
 */
int contain(NODE **list, int lastIndex, NODE *searched, int smaller) {
    int i;
    for (i = 0; i < lastIndex; i++) {
        if (list[i] == searched) {
            if (smaller == -1 || searched->fLocalGoal < list[i]->fLocalGoal)
                return 1;
        }
    }
    return 0;
}

/**
 * @brief Solve pathfinding problem
 *
 * @param nodes List2D de pointeur :[position x,y][vitesse]
 * @param width largeur map
 * @param height longueur map
 * @param nodeStart
 */
NODE *Solve_AStar(NODE ***nodes, int width, int height, NODE *nodeStart) {
    int i, x, y;
    NODE *nodeCurrent;

    for (x = 0; x < width; x++) {
        for (y = 0; y < height; y++) {
            if (nodes[y * width + x] != NULL) {
                for (i = 0; i < NUMBER_OF_SPEED; i++) {
                    nodes[y * width + x][i]->parent = NULL;
                    nodes[y * width + x][i]->bVisited = FALSE;
                    nodes[y * width + x][i]->fLocalGoal = INT_MAX;
                    nodes[y * width + x][i]->fGlobalGoal = INT_MAX;
                }
            }
        }
    }

    nodeCurrent = nodeStart;
    nodeStart->fLocalGoal = 0.0f;

    nodeStart->fGlobalGoal = 0;

    int openLastPushIndex = 1;
    int openLastNotSortedIndex = 1;
    NODE *open_list[80000];

    open_list[0] = nodeStart;

    while (openLastPushIndex != 0) {

        sort_nodes_list(open_list, openLastPushIndex, &openLastNotSortedIndex);
        openLastNotSortedIndex = openLastPushIndex;

        nodeCurrent = open_list[0];

        /* Depilement*/
        shift_left(open_list, openLastPushIndex);
        openLastPushIndex--;
        openLastNotSortedIndex--;

        if (nodeCurrent->end == TRUE) {
            fprintf(stderr, "END: %d %d - %d %d\n", nodeCurrent->x, nodeCurrent->y, nodeCurrent->speedX, nodeCurrent->speedY);
            return nodeCurrent;
        }

        for (i = 0; i < nodeCurrent->numberOfNeighbours; i++) {
            if (!(nodeCurrent->vecNeighbours[i] == NULL  || hit_a_wall(nodes, width, nodeCurrent, nodeCurrent->vecNeighbours[i]))) {
                int cout = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeCurrent->vecNeighbours[i]);
                int heur = cout + heuristic(nodeCurrent->vecNeighbours[0]);

                if (nodeCurrent->vecNeighbours[i]->fGlobalGoal > heur && contain(open_list, openLastPushIndex, nodeCurrent->vecNeighbours[i], -1)) {
                    continue;
                }

                if (nodeCurrent->vecNeighbours[i]->fGlobalGoal > heur && nodeCurrent->vecNeighbours[i]->bVisited) {
                    continue;
                }

                if(nodeCurrent->vecNeighbours[i] == nodeCurrent){
                    continue;
                }

                nodeCurrent->vecNeighbours[i]->parent = nodeCurrent;

                open_list[openLastPushIndex] = nodeCurrent->vecNeighbours[i];
                openLastPushIndex++;
                openLastNotSortedIndex++;
            }
        }
        nodeCurrent->bVisited = TRUE;
    }
    return NULL;
}

/** Le cout d'un deplacement: pour le moment une distance*/
float distance(NODE *nodeA, NODE *nodeB) {
    /* distance euclidenne avec et sans vitesse
    sqrtf((nodeA->x - nodeB->x + nodeB->speedX)*(nodeA->x - nodeB->x + nodeB->speedX) + ((nodeA->y - nodeB->y + nodeB->speedY)*(nodeA->y - nodeB->y + nodeB->speedY)));
    return sqrtf((nodeA->x - nodeB->x)*(nodeA->x - nodeB->x) + ((nodeA->y - nodeB->y)*(nodeA->y - nodeB->y)));*/

    /* Distance de manhatan*/
    return abs(abs(nodeB->x - nodeA->x - nodeA->speedX) + abs(nodeB->y - nodeA->y - nodeA->speedY));
}

/** Sert à influer la progression de l'algorithme*/
float heuristic(NODE *nodeA) {
    return nodeA->distanceToEnd; /* abs(nodeB->x - nodeA->x) + abs(nodeB->x - nodeA->x);*/
}

/** Tri une liste de noeuds selon le global goal*/
void sort_nodes_list(NODE **list, int last_push, int *lastBigNumber) {
    int i, j;

    NODE *tempNode;

    for (i = *lastBigNumber; i < last_push; i++) {
        if (list[i]->fGlobalGoal > 100000) {
            continue;
        }

        for (j = 0; j < i; j++) {
            if (list[j]->fGlobalGoal > list[i]->fGlobalGoal) {
                tempNode = list[j];
                list[j] = list[i];
                list[i] = tempNode;
            }
        }
    }
}

/** Depile l'element le plus à gauche d'une liste*/
void shift_left(NODE **list, int last_push) {
    int i;
    for (i = 1; i < last_push; i++) {
        list[i - 1] = list[i];
    }
}

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
int gasConsumption(int accX, int accY, int speedX, int speedY, int inSand) {
    int gas = accX * accX + accY * accY;
    gas += (int)(sqrt(speedX * speedX + speedY * speedY) * 3.0 / 2.0);
    if (inSand) {
        gas += 1;
    }
    return gas;
}

/** Retourne le trajet à suivre par le pilote
 *  Résoud le A star puis stocke le résultat dans un tableau de pointeur de vecteur 2d (position à suivre)
 */ 
NODE **get_path(NODE ***nodes, int width, int height, NODE *nodeStart) {
    NODE **path = malloc(800 * sizeof(NODE));
    NODE* p;
    
    p = Solve_AStar(nodes, width, height, nodeStart);
    int i = 0;
    while (p != NULL) {
        fprintf(stderr, "%d %d - ", p->x, p->y);
        path[i] = p;
        i++;

        p = p->parent;
    }

    path[i] = NULL;

    return path;
}

VECT2D *create_vector(int x, int y) {
    VECT2D *vect = malloc(sizeof(VECT2D));
    vect->x = x;
    vect->y = y;
    return vect;
}

/** Inverse le tableau de vect2d donné en parametre
 *  dernier elem = premier ...
 */
void reversePath(NODE **path) {
    int n, low, high;
    for (n = 0; n < 800; n++) {
        if (path[n] == NULL) {
            break;
        }
    }
    for (low = 0, high = n - 1; low < high; low++, high--) {
        NODE *temp = path[low];
        path[low] = path[high];
        path[high] = temp;
    }
}

/** Calcule et retourne la prochaine acceleration à prendre selon la vitesse et la position*/
VECT2D nextAcceleration(NODE **path, VECT2D *position, VECT2D *lastSpeed, int index) {

    VECT2D newAcceleration;

    if (path[index] == NULL) {
        newAcceleration.x = 0;
        newAcceleration.y = 0;
        fprintf(stderr, "LAST Acceleration %d %d, Speed %d %d\n", newAcceleration.x, newAcceleration.y, lastSpeed->x, lastSpeed->y);
        return newAcceleration;
    }

    if (index == 0) {
        newAcceleration.x = (path[index]->x - position->x);
        newAcceleration.y = path[index]->y - position->y;
    } else {
        fprintf(stderr, "CALCUL: H%d %d - B%d %d - S%d %d", path[index]->x, path[index]->y, path[index - 1]->x, path[index - 1]->y, lastSpeed->x, lastSpeed->y);
        newAcceleration.x = (path[index]->x - path[index - 1]->x) - path[index - 1]->speedX;
        newAcceleration.y = (path[index]->y - path[index - 1]->y) - path[index - 1]->speedY;
    }

    lastSpeed->x += newAcceleration.x;
    lastSpeed->y += newAcceleration.y;

    fprintf(stderr, "Acceleration %d %d, Speed %d %d\n", newAcceleration.x, newAcceleration.y, lastSpeed->x, lastSpeed->y);

    return newAcceleration;
}

void generate_heat_map(NODE ***nodes, int width, int height, NODE *start) {
    int y, x;
    int lastChange = 1;
    int poids_sable = 5;
    QUEUE *q = create_queue();

    /* Init la QUEUE trié par distance au start*/
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            if (nodes[y * width + x] != NULL) {
                nodes[y * width + x][0]->distanceToEnd = INT_MAX;
                enqueue(nodes[y * width + x][0], q);

                nodes[y * width + x][0]->bVisited = FALSE;
                nodes[y * width + x][0]->fLocalGoal = INT_MAX;
                nodes[y * width + x][0]->fGlobalGoal = INT_MAX;

                /* start first in the list*/
                if (x == start->x && y == start->y) {
                    nodes[y * width + x][0]->distanceToEnd = 0;
                    q->tail->next = q->head->next;
                    q->tail->pred->next = q->head;

                    q->head->pred = q->tail->pred;
                    q->head->next->pred = q->tail;
                    q->tail->pred = NULL;
                    q->head->next = NULL;

                    QNODE *temp = q->head;
                    q->head = q->tail;
                    q->tail = temp;
                }
            }
        }
    }

    QNODE *current = NULL;

    while (current != q->tail) {

        current = q->head;

        /* Depile*/
        if (current != q->tail) {
            dequeue(q);
        }

        if (current->value->distanceToEnd == INT_MAX) {
            fprintf(stderr, "WEIRD came out too soon of the heat map algorithm\n");
            break;
        }

        lastChange--;

        /* check 8 neighbour
        0 1 2
        3 4 5
        6 7 8
        */
        if (current->value->y > 0 && current->value->x > 0 && current->value->sable == FALSE && nodes[(current->value->y - 1) * width + current->value->x - 1] != NULL &&
            nodes[(current->value->y - 1) * width + current->value->x - 1][0]->distanceToEnd > current->value->distanceToEnd) {
            nodes[(current->value->y - 1) * width + current->value->x - 1][0]->distanceToEnd = current->value->distanceToEnd + 1;

            if (nodes[(current->value->y - 1) * width + current->value->x - 1][0]->sable == TRUE) {
                nodes[(current->value->y - 1) * width + current->value->x - 1][0]->distanceToEnd = current->value->distanceToEnd + poids_sable;
            }

            nodes[(current->value->y - 1) * width + current->value->x - 1][0]->parent = current->value;
            trierElemDistance(find(nodes[(current->value->y - 1) * width + current->value->x - 1][0], q), q, lastChange);

            lastChange++;
        }

        if (current->value->y > 0 && nodes[(current->value->y - 1) * width + current->value->x] != NULL &&
            nodes[(current->value->y - 1) * width + current->value->x][0]->distanceToEnd > current->value->distanceToEnd) {
            nodes[(current->value->y - 1) * width + current->value->x][0]->distanceToEnd = current->value->distanceToEnd + 1;

            if (nodes[(current->value->y - 1) * width + current->value->x][0]->sable == TRUE) {
                nodes[(current->value->y - 1) * width + current->value->x][0]->distanceToEnd = current->value->distanceToEnd + poids_sable;
            }

            nodes[(current->value->y - 1) * width + current->value->x][0]->parent = current->value;
            trierElemDistance(find(nodes[(current->value->y - 1) * width + current->value->x][0], q), q, lastChange);
            lastChange++;
        }

        if (current->value->y > 0 && current->value->x < width - 1 && current->value->sable == FALSE &&
            nodes[(current->value->y - 1) * width + current->value->x + 1] != NULL &&
            nodes[(current->value->y - 1) * width + current->value->x + 1][0]->distanceToEnd > current->value->distanceToEnd) {
            nodes[(current->value->y - 1) * width + current->value->x + 1][0]->distanceToEnd = current->value->distanceToEnd + 1;

            if (nodes[(current->value->y - 1) * width + current->value->x + 1][0]->sable == TRUE) {
                nodes[(current->value->y - 1) * width + current->value->x + 1][0]->distanceToEnd = current->value->distanceToEnd + poids_sable;
            }

            nodes[(current->value->y - 1) * width + current->value->x + 1][0]->parent = current->value;
            trierElemDistance(find(nodes[(current->value->y - 1) * width + current->value->x + 1][0], q), q, lastChange);
            lastChange++;
        }

        if (current->value->x > 0 && nodes[current->value->y * width + current->value->x - 1] != NULL &&
            nodes[current->value->y * width + current->value->x - 1][0]->distanceToEnd > current->value->distanceToEnd) {
            nodes[current->value->y * width + current->value->x - 1][0]->distanceToEnd = current->value->distanceToEnd + 1;

            if (nodes[(current->value->y) * width + current->value->x - 1][0]->sable == TRUE) {
                nodes[(current->value->y) * width + current->value->x - 1][0]->distanceToEnd = current->value->distanceToEnd + poids_sable;
            }

            nodes[current->value->y * width + current->value->x - 1][0]->parent = current->value;
            trierElemDistance(find(nodes[(current->value->y) * width + current->value->x - 1][0], q), q, lastChange);
            lastChange++;
        }
        if (current->value->x < width - 1 && nodes[current->value->y * width + current->value->x + 1] != NULL &&
            nodes[current->value->y * width + current->value->x + 1][0]->distanceToEnd > current->value->distanceToEnd) {
            nodes[current->value->y * width + current->value->x + 1][0]->distanceToEnd = current->value->distanceToEnd + 1;

            if (nodes[(current->value->y) * width + current->value->x + 1][0]->sable == TRUE) {
                nodes[(current->value->y) * width + current->value->x + 1][0]->distanceToEnd = current->value->distanceToEnd + poids_sable;
            }

            nodes[current->value->y * width + current->value->x + 1][0]->parent = current->value;
            trierElemDistance(find(nodes[(current->value->y) * width + current->value->x + 1][0], q), q, lastChange);
            lastChange++;
        }
        if (current->value->y < height - 1 && current->value->x > 0 && current->value->sable == FALSE &&
            nodes[(current->value->y + 1) * width + current->value->x - 1] != NULL &&
            nodes[(current->value->y + 1) * width + current->value->x - 1][0]->distanceToEnd > current->value->distanceToEnd) {
            nodes[(current->value->y + 1) * width + current->value->x - 1][0]->distanceToEnd = current->value->distanceToEnd + 1;

            if (nodes[(current->value->y + 1) * width + current->value->x - 1][0]->sable == TRUE) {
                nodes[(current->value->y + 1) * width + current->value->x - 1][0]->distanceToEnd = current->value->distanceToEnd + poids_sable;
            }

            nodes[(current->value->y + 1) * width + current->value->x - 1][0]->parent = current->value;
            QNODE *test = find(nodes[(current->value->y + 1) * width + current->value->x - 1][0], q);

            trierElemDistance(test, q, lastChange);
            lastChange++;
        }
        if (current->value->y < height - 1 && nodes[(current->value->y + 1) * width + current->value->x] != NULL &&
            nodes[(current->value->y + 1) * width + current->value->x][0]->distanceToEnd > current->value->distanceToEnd) {
            nodes[(current->value->y + 1) * width + current->value->x][0]->distanceToEnd = current->value->distanceToEnd + 1;

            if (nodes[(current->value->y + 1) * width + current->value->x][0]->sable == TRUE) {
                nodes[(current->value->y + 1) * width + current->value->x][0]->distanceToEnd = current->value->distanceToEnd + poids_sable;
            }

            nodes[(current->value->y + 1) * width + current->value->x][0]->parent = current->value;
            trierElemDistance(find(nodes[(current->value->y + 1) * width + current->value->x][0], q), q, lastChange);
            lastChange++;
        }
        if (current->value->y < height - 1 && current->value->x < width - 1 && current->value->sable == FALSE &&
            nodes[(current->value->y + 1) * width + current->value->x + 1] != NULL &&
            nodes[(current->value->y + 1) * width + current->value->x + 1][0]->distanceToEnd > current->value->distanceToEnd) {
            nodes[(current->value->y + 1) * width + current->value->x + 1][0]->distanceToEnd = current->value->distanceToEnd + 1;

            if (nodes[(current->value->y + 1) * width + current->value->x + 1][0]->sable == TRUE) {
                nodes[(current->value->y + 1) * width + current->value->x + 1][0]->distanceToEnd = current->value->distanceToEnd + poids_sable;
            }

            nodes[(current->value->y + 1) * width + current->value->x + 1][0]->parent = current->value;
            trierElemDistance(find(nodes[(current->value->y + 1) * width + current->value->x + 1][0], q), q, lastChange);
            lastChange++;
        }
        free(current);
    }

    free(q);
}

void sort_heat_list(NODE **list, int last_push) {
    int i, j;
    NODE *tempNode;

    for (i = 0; i < last_push; i++) {
        for (j = 0; j < i; j++) {
            if (list[j]->distanceToEnd > list[i]->distanceToEnd) {
                tempNode = list[j];
                list[j] = list[i];
                list[i] = tempNode;
            }
        }
    }
}

void display_heat_map(NODE ***map, int width, int height) {
    int y, x;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            if (map[y * width + x] == NULL) {
                fprintf(stderr, " . ");
            } else {
                fprintf(stderr, "%3d", map[y * width + x][0]->distanceToEnd);
            }
        }
        fprintf(stderr, "\n");
    }
}

/**
 * @brief Solve avec carburant en principal
 *
 * @param nodes List2D de pointeur :[position x,y][vitesse]
 * @param width largeur map
 * @param height longueur map
 * @param nodeStart
 */
NODE *Solve_AStar_Essence(NODE ***nodes, int width, int height, NODE *nodeStart, VECT2D *tab) {
    int i, x, y;

    for (x = 0; x < width; x++) {
        for (y = 0; y < height; y++) {
            if (nodes[y * width + x] != NULL) {
                for (i = 0; i < NUMBER_OF_SPEED; i++) {
                    nodes[y * width + x][i]->parent = NULL;
                    nodes[y * width + x][i]->bVisited = FALSE;
                    nodes[y * width + x][i]->fLocalGoal = INT_MAX;
                    nodes[y * width + x][i]->fGlobalGoal = INT_MAX;
                }
            }
        }
    }

    QUEUE *q = create_queue();

    nodeStart->fLocalGoal = 0.0f;
    nodeStart->fGlobalGoal = 0;

    int openLastPushIndex = 1;

    fprintf(stderr,"START: %d %d\n", nodeStart->x, nodeStart->y);
    enqueue(nodeStart, q);


    QNODE *current = NULL;

    while (!is_empty(q)) {
        /*if(current!= NULL){
            free(current);
            current = NULL;
        }*/
        current = q->head;

        /* Depile*/
        if (current != q->tail) {
            dequeue(q);
            openLastPushIndex--;
        }

        if (current->value->end == TRUE) {
            fprintf(stderr, "END: %d %d - %d %d\n", current->value->x, current->value->y, current->value->speedX, current->value->speedY);
            return current->value;
        }

        for (i = 0; i < current->value->numberOfNeighbours; i++) {
            if (!(hit_a_wall(nodes, width, current->value, current->value->vecNeighbours[i]) ||
                    current->value->vecNeighbours[i]->bVisited ||
                    (find(current->value->vecNeighbours[i], q) != NULL && current->value->vecNeighbours[i]->fLocalGoal < current->value->fLocalGoal))) {

                float cout = current->value->fLocalGoal + gasConsumption(current->value->vecNeighbours[i]->x - current->value->x - current->value->speedX, current->value->vecNeighbours[i]->y - current->value->y - current->value->speedY, current->value->speedX, current->value->speedY,current->value->sable) /*distance(current->value, current->value->vecNeighbours[i])*/;
                float heur = cout + heuristic(nodes[current->value->vecNeighbours[i]->y * width + current->value->vecNeighbours[i]->x][0]);

                current->value->vecNeighbours[i]->fLocalGoal = cout;
                current->value->vecNeighbours[i]->fGlobalGoal = heur;

                if(current->value->vecNeighbours[i] == current->value){
                    continue;
                }

                current->value->vecNeighbours[i]->parent = current->value;


                enqueue(current->value->vecNeighbours[i], q);
                trierElemGlobal(find(current->value->vecNeighbours[i], q), q);

                openLastPushIndex++;
                
            }
        }
        current->value->bVisited = TRUE;
    }
    fprintf(stderr,"CANT FIND DA WEI\n");
    return NULL;
}

/** Retourne le trajet à suivre par le pilote
 *  Résoud le A star puis stocke le résultat dans un tableau de pointeur de vecteur 2d (position à suivre)
 */
NODE **get_path_essence(NODE ***nodes, int width, int height, NODE *nodeStart, VECT2D *tab, int * carburant) {
    NODE **path = malloc(800 * sizeof(NODE));
    NODE *p;
    int consomation_essence = 0;

    p = Solve_AStar_Essence(nodes, width, height, nodeStart, tab);



    if (p == NULL) {
        fprintf(stderr, "NO PATH FOUND\n");
    }

    int i = 0;
    while (p != NULL) {
        fprintf(stderr, "%d %d - ", p->x, p->y);
        path[i] = p;
        if(p->parent != NULL){
            consomation_essence += gasConsumption(p->x - p->parent->x - p->parent->speedX, p->y - p->parent->y - p->parent->speedY, p->parent->speedX, p->parent->speedY, p->parent->sable);  
        }
        i++;
        p = p->parent;
    }

    path[i] = NULL;

    *carburant = consomation_essence;

    return path;
}

int calculConsommationEssenceSurTrajet(NODE ** path,int indexFin){
    int i;
    int consommation = 0;
    for(i = 0; i < indexFin-1; i++){
        consommation += gasConsumption(path[i+1]->x - path[i]->x - path[i]->speedX, path[i+1]->y - path[i]->y - path[i]->speedY, path[i]->speedX,  path[i]->speedY,  path[i]->sable);  
    }
    return consommation;
}

/** Set a node to null if another player on it and keep trace of it to restore it next round*/
void resetMapPlayersPosition(NODE *** mapNodes, int mapWidth, NODE ***lastP2Position, NODE ***lastP3Position, VECT2D player2Position, VECT2D player3Position){
    if (*lastP2Position != NULL) {
            mapNodes[(*lastP2Position)[0]->y * mapWidth + (*lastP2Position)[0]->x] = *lastP2Position;
        }

        if (*lastP3Position != NULL) {
            mapNodes[(*lastP3Position)[0]->y * mapWidth + (*lastP3Position)[0]->x] = *lastP3Position;
        }

        *lastP2Position = mapNodes[player2Position.y * mapWidth + player2Position.x];
        *lastP3Position = mapNodes[player3Position.y * mapWidth + player3Position.x];


        if(*lastP2Position!= NULL){
            mapNodes[player2Position.y * mapWidth + player2Position.x] = NULL;
        }
        if(*lastP3Position != NULL){
            mapNodes[player3Position.y * mapWidth + player3Position.x] = NULL;
        }
}
