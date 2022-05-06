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

// Trouve pour une vitesse sa position dans le tableau tab
int findIndex(VECT2D *tab, char speedX, char speedY) {
    for (int i = 0; i < NUMBER_OF_SPEED; i++) {
        if (tab[i].x == speedX && tab[i].y == speedY) {
            return i;
        }
    }
    return -1;
}

// Genrere une liste avec toutes les combinaisons de vitesses possible
VECT2D *generateListIndex() {
    VECT2D *result = (VECT2D *)malloc(NUMBER_OF_SPEED * sizeof(*result));

    int index = 0;
    for (int i = -5; i < 6; i++) {
        for (int j = -5; j < 6; j++) {
            if (sqrt(pow(i, 2) + pow(j, 2)) <= 5) {
                result[index].x = i;
                result[index].y = j;
                index++;
            }
        }
    }
    return result;
}

NODE ***createNodeMap(VECT2D *tab, int width, int height) {
    //on alloue toute la table
    NODE ***nodes = (NODE ***)malloc(width * height * sizeof(NODE **));
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            nodes[y * width + x] = (NODE **)malloc(NUMBER_OF_SPEED * sizeof(*nodes[y * width + x]));
        }
    }

    //on alloue toutes les vitesses (81) associées à chaque noeud
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            // UTILISER TAB PLUTOT
            for (int i = 0; i < NUMBER_OF_SPEED; i++) {
                nodes[y * width + x][i] = createNode();
                nodes[y * width + x][i]->speedX = tab[i].x;
                nodes[y * width + x][i]->speedY = tab[i].y;
                nodes[y * width + x][i]->x = x;
                nodes[y * width + x][i]->y = y;
                nodes[y * width + x][i]->bObstacle = FALSE;
                nodes[y * width + x][i]->parent = NULL;
                nodes[y * width + x][i]->bVisited = FALSE;
                nodes[y * width + x][i]->sable = FALSE;
                nodes[y * width + x][i]->end = FALSE;
            }
        }
    }

    // FAIRE EN SORTE DE RENSEIGNER OBSTACLE EN AVANCE ICI

    int num_neigh = 0;

    /*
    Rensignement voisins selon la vitesse sur la case

    (x_prime, y_prime) = position 4
    On init le tableau de haut en bas
    0,0 0,1 0,2 0,3 ...
    1,0 1,1 1,2 ...

    0 1 2
    3 4 5
    6 7 8
    */
    int findex = 0;

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            for (int i = 0; i < NUMBER_OF_SPEED; i++) {
                int y_prime = y + nodes[y * width + x][i]->speedY;
                int x_prime = x + nodes[y * width + x][i]->speedX;

                // fprintf(stderr,"\n==============\ni = %d\n Position : %d %d\nSpeed: %d %d\nPrime = %d %d\n\n",i,x,y, nodes[y*width + x][i]->speedX, nodes[y*width + x][i]->speedY, x_prime, y_prime);

                bool_t x_in, y_in;

                x_in = (x_prime >= 0 && x_prime < width);
                y_in = (y_prime >= 0 && y_prime < height);

                // 4
                if (x_in && y_in) {
                    // fprintf(stderr,"in 4\n");

                    nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime)*width + (x_prime)][i];
                    num_neigh++;
                }

                // 1
                if (y_prime > 0 && y_prime <= height && x_in) {
                    findex = findIndex(tab, nodes[y * width + x][i]->speedX, nodes[y * width + x][i]->speedY - 1);
                    // fprintf(stderr,"in 1: %d\n", findex);
                    if (findex != -1) {
                        nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime - 1) * width + (x_prime + 0)][findex];
                        num_neigh++;
                    }
                }

                // 7
                if (y_prime < height - 1 && y_prime > -1 && x_in) {
                    findex = findIndex(tab, nodes[y * width + x][i]->speedX, nodes[y * width + x][i]->speedY + 1);
                    // fprintf(stderr,"in 7 : index = %d\n",findex);
                    if (findex != -1) {
                        nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime + 1) * width + (x_prime + 0)][findex];
                        num_neigh++;
                    }
                }

                // 3
                if (x_prime > 0 && x_prime <= width && y_in) {
                    findex = findIndex(tab, nodes[y * width + x][i]->speedX - 1, nodes[y * width + x][i]->speedY);
                    // fprintf(stderr,"in 3\n");
                    if (findex != -1) {
                        nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime + 0) * width + (x_prime - 1)][findex];
                        num_neigh++;
                    }
                }

                // 5
                if (x_prime < width - 1 && x_prime >= -1 && y_in) {
                    findex = findIndex(tab, nodes[y * width + x][i]->speedX + 1, nodes[y * width + x][i]->speedY);
                    // fprintf(stderr,"in 5\n");
                    if (findex != -1) {
                        nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime + 0) * width + (x_prime + 1)][findex];
                        num_neigh++;
                    }
                }
                // 0 carefull not to be out of the map
                if (y_prime > 0 && x_prime > 0 && x_prime <= width && y_prime <= height) {
                    findex = findIndex(tab, nodes[y * width + x][i]->speedX - 1, nodes[y * width + x][i]->speedY - 1);
                    // fprintf(stderr,"in 0\n");
                    if (findex != -1) {
                        nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime - 1) * width + (x_prime - 1)][findex];
                        num_neigh++;
                    }
                }

                // 6
                if (y_prime < height - 1 && x_prime > 0 && y_prime >= -1 && x_prime <= width) {
                    findex = findIndex(tab, nodes[y * width + x][i]->speedX - 1, nodes[y * width + x][i]->speedY + 1);
                    // fprintf(stderr,"in 6\n");
                    if (findex != -1) {
                        nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime + 1) * width + (x_prime - 1)][findex];
                        num_neigh++;
                    }
                }

                // 2
                if (y_prime > 0 && x_prime < width - 1 && y_prime <= height && x_prime >= -1) {
                    findex = findIndex(tab, nodes[y * width + x][i]->speedX + 1, nodes[y * width + x][i]->speedY - 1);
                    // fprintf(stderr,"in 2\n");
                    if (findex != -1) {
                        nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime - 1) * width + (x_prime + 1)][findex];
                        num_neigh++;
                    }
                }

                // 8
                if (y_prime < height - 1 && x_prime < width - 1 && y_prime >= -1 && x_prime >= -1) {
                    findex = findIndex(tab, nodes[y * width + x][i]->speedX + 1, nodes[y * width + x][i]->speedY + 1);
                    // fprintf(stderr,"in 8\n");
                    if (findex != -1) {
                        nodes[y * width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime + 1) * width + (x_prime + 1)][findex];
                        num_neigh++;
                    }
                }

                nodes[y * width + x][i]->numberOfNeighbours = num_neigh;

                num_neigh = 0;
            }
        }
    }

    return nodes;
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

            /*TODO : sable = pas mur mais pas fin*/
            if (nodeMap[p.y * width + p.x][0]->bObstacle && !nodeMap[p.y * width + p.x][0]->sable ) {
                return 1;
            }
        }
        return 0;
    }
}

// Affiche la map.
//  si path est != de null: affiche la route emprunté représenté par des $
void display_node_map(NODE ***map, int width, int height, VECT2D **path) {

    char c = '*';

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (map[y * width + x][0]->bObstacle == TRUE) {
                fprintf(stderr, ".");
            } else if (map[y * width + x][0]->end == TRUE) {
                fprintf(stderr, "=");
            } else {
                if (path == NULL) {
                    fprintf(stderr, "*");
                } else {
                    for (int i = 0; i < 800; i++) {
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
    if (path != NULL) {
        display_vector(path);
    }
}

// Affiche position x y stocké dans path
void display_vector(VECT2D **path) {
    fprintf(stderr, "START PATH \n");
    for (int i = 0; i < 800; i++) {
        if (path[i] == NULL) {
            break;
        }
        fprintf(stderr, "%d %d - ", path[i]->x, path[i]->y);
    }
    fprintf(stderr, "\nEND PATH \n");
}

/*
 * Detecte si un noeud est dans la list
 * smaller: si -1 ne regarde pas le cout, sinon return true si le coup est plus petit que celui de searched
 */
int contain(NODE **list, int lastIndex, NODE *searched, int smaller) {
    for (int i = 0; i < lastIndex; i++) {
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
 * @param nodeEnd
 * @return carburant
 */
NODE *Solve_AStar(NODE ***nodes, int width, int height, NODE *nodeStart, NODE *nodeEnd, int carburant) {

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {

            for (int i = 0; i < NUMBER_OF_SPEED; i++) {
                nodes[y * width + x][i]->parent = NULL;
                nodes[y * width + x][i]->bVisited = FALSE;
                nodes[y * width + x][i]->fLocalGoal = INT_MAX;
                nodes[y * width + x][i]->fGlobalGoal = INT_MAX;
            }
        }
    }

    NODE *nodeCurrent = nodeStart;
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

        // Depilement
        shift_left(open_list, openLastPushIndex);
        openLastPushIndex--;
        openLastNotSortedIndex--;

        if (nodeCurrent->end == TRUE) {
            fprintf(stderr, "END: %d %d - %d %d\n", nodeCurrent->x, nodeCurrent->y, nodeCurrent->speedX, nodeCurrent->speedY);
            return nodeCurrent;
        }

        for (int i = 0; i < nodeCurrent->numberOfNeighbours; i++) {
            if (!(nodeCurrent->vecNeighbours[i]->bObstacle == TRUE ||
                    hit_a_wall(nodes, width, nodeCurrent, nodeCurrent->vecNeighbours[i]))) {
                int cout = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeCurrent->vecNeighbours[i]);
                int heur = cout + heuristic(nodeCurrent->vecNeighbours[i], nodeEnd);

                if (nodeCurrent->vecNeighbours[i]->fGlobalGoal > heur && contain(open_list, openLastPushIndex, nodeCurrent->vecNeighbours[i], -1)) {
                    continue;
                }

                if (nodeCurrent->vecNeighbours[i]->fGlobalGoal > heur && nodeCurrent->vecNeighbours[i]->bVisited) {
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
    return nodeEnd;
}

// Le cout d'un deplacement: pour le moment une distance
float distance(NODE *nodeA, NODE *nodeB) {
    // distance euclidenne avec et sans vitesse
    //  sqrtf((nodeA->x - nodeB->x + nodeB->speedX)*(nodeA->x - nodeB->x + nodeB->speedX) + ((nodeA->y - nodeB->y + nodeB->speedY)*(nodeA->y - nodeB->y + nodeB->speedY)));
    // return sqrtf((nodeA->x - nodeB->x)*(nodeA->x - nodeB->x) + ((nodeA->y - nodeB->y)*(nodeA->y - nodeB->y)));

    // Distance de manhatan
    return abs(2 - abs(nodeB->x - nodeA->x - nodeA->speedX) + abs(nodeB->y - nodeA->y - nodeA->speedY));
}

// Sert à influer la progression de l'algorithme
float heuristic(NODE *nodeA, NODE *nodeB) {
    return nodeB->distanceToEnd;//abs(nodeB->x - nodeA->x) + abs(nodeB->x - nodeA->x);
}

// Tri une liste de noeuds selon le global goal
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

// Depile l'element le plus à gauche d'une liste
void shift_left(NODE **list, int last_push) {
    for (int i = 1; i < last_push; i++) {
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

// Retourne le trajet à suivre par le pilote
//  Résoud le A star puis stocke le résultat dans un tableau de pointeur de vecteur 2d (position à suivre)
VECT2D **get_path(NODE ***nodes, int width, int height, NODE *nodeStart, NODE *nodeEnd, int carburant) {
    VECT2D **path = malloc(800 * sizeof(VECT2D));
    NODE *p = nodeEnd;

    p = Solve_AStar(nodes, width, height, nodeStart, nodeEnd, carburant);

    int i = 0;
    while (p->parent != NULL && p->parent != p) {
        fprintf(stderr, "%d %d - ", p->x, p->y);
        path[i] = create_vector(p->x, p->y);
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

// Inverse le tableau de vect2d donné en parametre
//  dernier elem = premier ...
void reversePath(VECT2D **path) {
    int n;
    for (n = 0; n < 800; n++) {
        if (path[n] == NULL) {
            break;
        }
    }
    for (int low = 0, high = n - 1; low < high; low++, high--) {
        VECT2D *temp = path[low];
        path[low] = path[high];
        path[high] = temp;
    }
}

// Calcule et retourne la prochaine acceleration à prendre selon la vitesse et la position
VECT2D nextAcceleration(VECT2D **path, VECT2D *position, VECT2D *lastSpeed, int index) {

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
        newAcceleration.x = (path[index]->x - path[index - 1]->x) - lastSpeed->x;
        newAcceleration.y = (path[index]->y - path[index - 1]->y) - lastSpeed->y;
    }

    lastSpeed->x += newAcceleration.x;
    lastSpeed->y += newAcceleration.y;

    fprintf(stderr, "Acceleration %d %d, Speed %d %d\n", newAcceleration.x, newAcceleration.y, lastSpeed->x, lastSpeed->y);

    return newAcceleration;
}

void generate_heat_map(NODE ***nodes, int width, int height, NODE *start, NODE *end) {

    int lastChange = 1;
    QUEUE *q = create_queue();

    //Init la QUEUE trié par distance au start
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (nodes[y * width + x][0]->bObstacle == FALSE) {
                nodes[y * width + x][0]->distanceToEnd = INT_MAX;
                enqueue(nodes[y * width + x][0], q);

                nodes[y * width + x][0]->bVisited = FALSE;
                nodes[y * width + x][0]->fLocalGoal = INT_MAX;
                nodes[y * width + x][0]->fGlobalGoal = INT_MAX;

                //start first in the list
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

        // Depile
        if (current != q->tail) {
            dequeue(q);
        }

        if (current->value->distanceToEnd == INT_MAX) {
            fprintf(stderr,"WEIRD came out too soon of the heat map algorithm");
            break;
        }

        lastChange--;

        
        /* check 8 neighbour
        0 1 2
        3 4 5
        6 7 8
        */
        //display(q);
        fprintf(stderr, "START NEIGH\n0\n");
        fprintf(stderr, "%d %d\n", current->value->y, current->value->x);
        if (current->value->y > 0 && current->value->x > 0 && nodes[(current->value->y - 1) * width + current->value->x - 1][0]->bObstacle == FALSE &&
            nodes[(current->value->y - 1) * width + current->value->x - 1][0]->distanceToEnd > current->value->distanceToEnd) {
            nodes[(current->value->y - 1) * width + current->value->x - 1][0]->distanceToEnd = current->value->distanceToEnd + 1;
            nodes[(current->value->y - 1) * width + current->value->x - 1][0]->parent = current->value;
            trierElem(find(nodes[(current->value->y - 1) * width + current->value->x - 1][0], q), q, lastChange);

            lastChange++;
        }
        fprintf(stderr, "1\n");
        if (current->value->y > 0 && nodes[(current->value->y - 1) * width + current->value->x][0]->bObstacle == FALSE &&
            nodes[(current->value->y - 1) * width + current->value->x][0]->distanceToEnd > current->value->distanceToEnd) {
            nodes[(current->value->y - 1) * width + current->value->x][0]->distanceToEnd = current->value->distanceToEnd + 1;
            nodes[(current->value->y - 1) * width + current->value->x][0]->parent = current->value;
            trierElem(find(nodes[(current->value->y - 1) * width + current->value->x][0], q), q, lastChange);
            lastChange++;
        }
        fprintf(stderr, "2\n");
        if (current->value->y > 0 && current->value->x < width - 1 && nodes[(current->value->y - 1) * width + current->value->x + 1][0]->bObstacle == FALSE &&
            nodes[(current->value->y - 1) * width + current->value->x + 1][0]->distanceToEnd > current->value->distanceToEnd) {
            nodes[(current->value->y - 1) * width + current->value->x + 1][0]->distanceToEnd = current->value->distanceToEnd + 1;
            nodes[(current->value->y - 1) * width + current->value->x + 1][0]->parent = current->value;
            trierElem(find(nodes[(current->value->y - 1) * width + current->value->x + 1][0], q), q, lastChange);
            lastChange++;
        }

        fprintf(stderr, "3\n");
        if (current->value->x > 0 && nodes[current->value->y * width + current->value->x - 1][0]->bObstacle == FALSE &&
            nodes[current->value->y * width + current->value->x - 1][0]->distanceToEnd > current->value->distanceToEnd) {
            nodes[current->value->y * width + current->value->x - 1][0]->distanceToEnd = current->value->distanceToEnd + 1;
            nodes[current->value->y * width + current->value->x - 1][0]->parent = current->value;
            trierElem(find(nodes[(current->value->y) * width + current->value->x - 1][0], q), q, lastChange);
            lastChange++;
        }
        fprintf(stderr, "5\n");
        if (current->value->x < width - 1 && nodes[current->value->y * width + current->value->x + 1][0]->bObstacle == FALSE &&
            nodes[current->value->y * width + current->value->x + 1][0]->distanceToEnd > current->value->distanceToEnd) {
            nodes[current->value->y * width + current->value->x + 1][0]->distanceToEnd = current->value->distanceToEnd + 1;
            nodes[current->value->y * width + current->value->x + 1][0]->parent = current->value;
            trierElem(find(nodes[(current->value->y) * width + current->value->x + 1][0], q), q, lastChange);
            lastChange++;
        }

        fprintf(stderr, "6\n");
        if (current->value->y < height - 1 && current->value->x > 0 && nodes[(current->value->y + 1) * width + current->value->x - 1][0]->bObstacle == FALSE &&
            nodes[(current->value->y + 1) * width + current->value->x - 1][0]->distanceToEnd > current->value->distanceToEnd) {
            nodes[(current->value->y + 1) * width + current->value->x - 1][0]->distanceToEnd = current->value->distanceToEnd + 1;
            nodes[(current->value->y + 1) * width + current->value->x - 1][0]->parent = current->value;
            QNODE* test = find(nodes[(current->value->y + 1) * width + current->value->x - 1][0], q);
            //fprintf(stderr, "CRASH %d %d %d %d %d %d\n", test->value->x, test->value->y, q->head->value->x,q->head->value->y,q->tail->value->x,q->tail->value->y);

            trierElem(test, q, lastChange);
            lastChange++;
        }
        fprintf(stderr, "7\n");
        if (current->value->y < height - 1 && nodes[(current->value->y + 1) * width + current->value->x][0]->bObstacle == FALSE &&
            nodes[(current->value->y + 1) * width + current->value->x][0]->distanceToEnd > current->value->distanceToEnd) {
            nodes[(current->value->y + 1) * width + current->value->x][0]->distanceToEnd = current->value->distanceToEnd + 1;
            nodes[(current->value->y + 1) * width + current->value->x][0]->parent = current->value;
            trierElem(find(nodes[(current->value->y + 1) * width + current->value->x][0], q), q, lastChange);
            lastChange++;
        }
        fprintf(stderr, "8\n");
        if (current->value->y < height - 1 && current->value->x < width - 1 && nodes[(current->value->y + 1) * width + current->value->x + 1][0]->bObstacle == FALSE &&
            nodes[(current->value->y + 1) * width + current->value->x + 1][0]->distanceToEnd > current->value->distanceToEnd) {
            nodes[(current->value->y + 1) * width + current->value->x + 1][0]->distanceToEnd = current->value->distanceToEnd + 1;
            nodes[(current->value->y + 1) * width + current->value->x + 1][0]->parent = current->value;
            trierElem(find(nodes[(current->value->y + 1) * width + current->value->x + 1][0], q), q, lastChange);
            lastChange++;
        }
        free(current);
    }

    free(q);

    display_heat_map(nodes, width, height);
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
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (map[y * width + x][0]->bObstacle == TRUE) {
                fprintf(stderr, ".");
            } else {
                fprintf(stderr, "%d", map[y * width + x][0]->distanceToEnd);
            }
        }
        fprintf(stderr, "\n");
    }
}
