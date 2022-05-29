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

int findIndex(VECT2D *tab, char speedX, char speedY) {
    int i;
    for (i = 0; i < NUMBER_OF_SPEED; i++) {
        if (tab[i].x == speedX && tab[i].y == speedY) {
            return i;
        }
    }
    return -1;
}

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

NODE ***createNodeMap(int width, int height) {
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
        } else if (norm == 1) {
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

int hit_a_wall(NODE ***nodeMap, int width, NODE *start, NODE *stop) {
    {
        InfoLine vline;
        Pos2Dint p;
        initLine(start->x, start->y, stop->x, stop->y, &vline);
        while (nextPoint(&vline, &p, +1) > 0) {
            if (nodeMap[p.y * width + p.x] == NULL) {
                return 1;
            }
        }
        return 0;
    }
}

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

void display_vector(NODE **path) {
    int i;
    fprintf(stderr, "START PATH \n");
    i = 0;
    while (path[i] != NULL) {
        fprintf(stderr, "%d %d - ", path[i]->x, path[i]->y);
        i++;
    }
    fprintf(stderr, "\nEND PATH \n");
}

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

NODE *Solve_AStar(NODE ***nodes, int width, int height, NODE *nodeStart, char slowDown) {
    int i, x, y, openLastPushIndex, heur,cout;
    QUEUE *q;
    QNODE *current;

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

    q = create_queue();
    nodeStart->fLocalGoal = 0.0f;
    nodeStart->fGlobalGoal = 0;
    openLastPushIndex = 1;
    current = NULL;

    enqueue(nodeStart, q);

    while (!is_empty(q)) {

        current = q->head;

        /* Depile*/
        if (current != q->tail) {
            dequeue(q);
            openLastPushIndex--;
        } else{
            q->head = NULL;
            q->tail = NULL;
        }

        for (i = 0; i < current->value->numberOfNeighbours; i++) {
            if (current->value->vecNeighbours[i]->speedX == current->value->speedX && current->value->vecNeighbours[i]->speedY == current->value->speedY) {
                NODE *temp = current->value->vecNeighbours[i];
                current->value->vecNeighbours[i] = current->value->vecNeighbours[0];
                current->value->vecNeighbours[0] = temp;
            }
        }

        for (i = 0; i < current->value->numberOfNeighbours; i++) {
            if (!(hit_a_wall(nodes, width, current->value, current->value->vecNeighbours[i]))) {
                
                if(!(abs(current->value->vecNeighbours[i]->speedX) < 6-slowDown && abs(current->value->vecNeighbours[i]->speedY < 6-slowDown))){
                    continue;
                }

                if (current->value->vecNeighbours[i]->end == TRUE) {
                    current->value->vecNeighbours[i]->parent = current->value;
                    return current->value->vecNeighbours[i];
                }

                cout = current->value->fLocalGoal + distance(current->value, current->value->vecNeighbours[i]);
                heur = cout + heuristic(current->value->vecNeighbours[0]);

                if (current->value->vecNeighbours[i]->fGlobalGoal > heur && find(current->value->vecNeighbours[i], q) != NULL) {
                    continue;
                }

                if (current->value->vecNeighbours[i]->fGlobalGoal > heur && current->value->vecNeighbours[i]->bVisited) {
                    continue;
                }

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
        free(current);
    }
    return NULL;
}

float distance(NODE *nodeA, NODE *nodeB) {
    /* Distance de manhatan*/
    return abs(abs(nodeB->x - nodeA->x - nodeA->speedX) + abs(nodeB->y - nodeA->y - nodeA->speedY));
}

float heuristic(NODE *nodeA) {
    return nodeA->distanceToEnd;
}

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

void shift_left(NODE **list, int last_push) {
    int i;
    for (i = 1; i < last_push; i++) {
        list[i - 1] = list[i];
    }
}

int gasConsumption(int accX, int accY, int speedX, int speedY, int inSand) {
    int gas = accX * accX + accY * accY;
    gas += (int)(sqrt(speedX * speedX + speedY * speedY) * 3.0 / 2.0);
    if (inSand) {
        gas += 1;
    }
    return gas;
}

NODE **get_path(NODE ***nodes, int width, int height, NODE *nodeStart, int* autreConso, char slowDown) {
    int i, consoEssence = 0;
    NODE **path = malloc(800 * sizeof(NODE));
    NODE *p;

    p = Solve_AStar(nodes, width, height, nodeStart, slowDown);
    i = 0;
    while (p != NULL) {
        path[i] = p;
        if (p->parent != NULL) {
            consoEssence += gasConsumption(p->x - p->parent->x - p->parent->speedX, p->y - p->parent->y - p->parent->speedY, p->parent->speedX, p->parent->speedY, p->parent->sable); 
        }
        i++;

        p = p->parent;
    }
    *autreConso = consoEssence;
    path[i] = NULL;

    return path;
}

VECT2D *create_vector(int x, int y) {
    VECT2D *vect = malloc(sizeof(VECT2D));
    vect->x = x;
    vect->y = y;
    return vect;
}

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

VECT2D nextAcceleration(NODE **path, VECT2D *position, VECT2D *lastSpeed, int index) {

    VECT2D newAcceleration;

    if (path[index] == NULL) {
        newAcceleration.x = 0;
        newAcceleration.y = 0;
        return newAcceleration;
    }

    if (index == 0) {
        newAcceleration.x = (path[index]->x - position->x);
        newAcceleration.y = path[index]->y - position->y;
    } else {
        newAcceleration.x = (path[index]->x - path[index - 1]->x) - path[index - 1]->speedX;
        newAcceleration.y = (path[index]->y - path[index - 1]->y) - path[index - 1]->speedY;
    }

    lastSpeed->x += newAcceleration.x;
    lastSpeed->y += newAcceleration.y;

    return newAcceleration;
}

void generate_heat_map(NODE ***nodes, int width, int height, NODE *start) {
    int y, x, lastChange, poids_sable;
    QNODE *temp;
    QUEUE *q;
    QNODE *current;

    /* Init la QUEUE trié par distance au start*/
    q = create_queue();
    lastChange = 1;
    poids_sable = 5;

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

                    temp = q->head;
                    q->head = q->tail;
                    q->tail = temp;
                }
            }
        }
    }

    current = NULL;

    while (current != q->tail) {

        current = q->head;

        /* Depile*/
        if (current != q->tail) {
            dequeue(q);
        }

        if (current->value->distanceToEnd == INT_MAX) {
            break;
        }

        lastChange--;

        /* check 4 neighbour */
        if (current->value->y > 0 && nodes[(current->value->y - 1) * width + current->value->x] != NULL &&
            nodes[(current->value->y - 1) * width + current->value->x][0]->distanceToEnd > current->value->distanceToEnd) {
            nodes[(current->value->y - 1) * width + current->value->x][0]->distanceToEnd = current->value->distanceToEnd + 1;

            if (nodes[(current->value->y - 1) * width + current->value->x][0]->sable == TRUE) {
                nodes[(current->value->y - 1) * width + current->value->x][0]->distanceToEnd = current->value->distanceToEnd + poids_sable;
            }

            nodes[(current->value->y - 1) * width + current->value->x][0]->parent = current->value;
            temp= find(nodes[(current->value->y - 1) * width + current->value->x][0],q);
            if(temp){
                trierElemDistance(temp, q, lastChange);
                lastChange++;
            }
        }

        if (current->value->x > 0 && nodes[current->value->y * width + current->value->x - 1] != NULL &&
            nodes[current->value->y * width + current->value->x - 1][0]->distanceToEnd > current->value->distanceToEnd) {
            nodes[current->value->y * width + current->value->x - 1][0]->distanceToEnd = current->value->distanceToEnd + 1;
            if (nodes[(current->value->y) * width + current->value->x - 1][0]->sable == TRUE) {
                nodes[(current->value->y) * width + current->value->x - 1][0]->distanceToEnd = current->value->distanceToEnd + poids_sable;
            }
            nodes[current->value->y * width + current->value->x - 1][0]->parent = current->value;
            temp= find(nodes[current->value->y * width + current->value->x - 1][0],q);
            if(temp){
                trierElemDistance(temp, q, lastChange);
                lastChange++;
            }
        }

        if (current->value->x < width - 1 && nodes[current->value->y * width + current->value->x + 1] != NULL &&
            nodes[current->value->y * width + current->value->x + 1][0]->distanceToEnd > current->value->distanceToEnd) {
            nodes[current->value->y * width + current->value->x + 1][0]->distanceToEnd = current->value->distanceToEnd + 1;

            if (nodes[(current->value->y) * width + current->value->x + 1][0]->sable == TRUE) {
                nodes[(current->value->y) * width + current->value->x + 1][0]->distanceToEnd = current->value->distanceToEnd + poids_sable;
            }

            nodes[current->value->y * width + current->value->x + 1][0]->parent = current->value;
            temp= find(nodes[current->value->y * width + current->value->x + 1][0],q);
            if(temp){
                trierElemDistance(temp, q, lastChange);
                lastChange++;
            }
        }

        if (current->value->y < height - 1 && nodes[(current->value->y + 1) * width + current->value->x] != NULL &&
            nodes[(current->value->y + 1) * width + current->value->x][0]->distanceToEnd > current->value->distanceToEnd) {
            nodes[(current->value->y + 1) * width + current->value->x][0]->distanceToEnd = current->value->distanceToEnd + 1;

            if (nodes[(current->value->y + 1) * width + current->value->x][0]->sable == TRUE) {
                nodes[(current->value->y + 1) * width + current->value->x][0]->distanceToEnd = current->value->distanceToEnd + poids_sable;
            }

            nodes[(current->value->y + 1) * width + current->value->x][0]->parent = current->value;
            temp= find(nodes[(current->value->y + 1) * width + current->value->x][0],q);
            if(temp){
                trierElemDistance(temp, q, lastChange);
                lastChange++;
            }
        }
        free(current);
    }

    free(q);
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

NODE *Solve_AStar_Essence(NODE ***nodes, int width, int height, NODE *nodeStart) {
    int i, x, y, openLastPushIndex;
    QUEUE *q;
    QNODE *current;

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

    q = create_queue();
    nodeStart->fLocalGoal = 0.0f;
    nodeStart->fGlobalGoal = 0;
    openLastPushIndex = 1;
    current = NULL;
    
    enqueue(nodeStart, q);

    while (!is_empty(q)) {
        current = q->head;

        /* Depile*/
        if (current != q->tail) {
            dequeue(q);
            openLastPushIndex--;
        }else{
            q->head = NULL;
            q->tail = NULL;
        }

        if (current->value->end == TRUE) {
            return current->value;
        }

        for (i = 0; i < current->value->numberOfNeighbours; i++) {
            if (!(hit_a_wall(nodes, width, current->value, current->value->vecNeighbours[i]) ||
                    current->value->vecNeighbours[i]->bVisited ||
                    (find(current->value->vecNeighbours[i], q) != NULL && current->value->vecNeighbours[i]->fLocalGoal < current->value->fLocalGoal))) {

                float cout = current->value->fLocalGoal + gasConsumption(current->value->vecNeighbours[i]->x - current->value->x - current->value->speedX, current->value->vecNeighbours[i]->y - current->value->y - current->value->speedY, current->value->speedX, current->value->speedY, current->value->sable);
                float heur = cout + heuristic(nodes[current->value->vecNeighbours[i]->y * width + current->value->vecNeighbours[i]->x][0]);

                current->value->vecNeighbours[i]->fLocalGoal = cout;
                current->value->vecNeighbours[i]->fGlobalGoal = heur;

                if (current->value->vecNeighbours[i] == current->value) {
                    continue;
                }

                current->value->vecNeighbours[i]->parent = current->value;

                enqueue(current->value->vecNeighbours[i], q);
                trierElemGlobal(find(current->value->vecNeighbours[i], q), q);

                openLastPushIndex++;
            }
        }
        current->value->bVisited = TRUE;
        free(current);
    }
    return NULL;
}

NODE **get_path_essence(NODE ***nodes, int width, int height, NODE *nodeStart, int *carburant) {
    NODE **path = malloc(800 * sizeof(NODE));
    NODE *p;
    int i, consomation_essence;

    consomation_essence = 0;
    i = 0;

    p = Solve_AStar_Essence(nodes, width, height, nodeStart);
    if (p == NULL) {
        fprintf(stderr, "NO PATH FOUND\n");
    }

    while (p != NULL) {
        path[i] = p;
        if (p->parent != NULL) {
            consomation_essence += gasConsumption(p->x - p->parent->x - p->parent->speedX, p->y - p->parent->y - p->parent->speedY, p->parent->speedX, p->parent->speedY, p->parent->sable);
        }
        i++;
        p = p->parent;
    }

    path[i] = NULL;

    *carburant = consomation_essence;

    return path;
}

int calculConsommationEssenceSurTrajet(NODE **path, int indexDebut, int indexFin) {
    int i;
    int consommation = 0;
    for (i = indexDebut; i < indexFin - 1; i++) {
        consommation += gasConsumption(path[i + 1]->x - path[i]->x - path[i]->speedX, path[i + 1]->y - path[i]->y - path[i]->speedY, path[i]->speedX, path[i]->speedY, path[i]->sable);
    }
    return consommation;
}

void resetMapPlayersPosition(NODE ***mapNodes, int mapWidth, NODE ***lastP2Position, NODE ***lastP3Position, VECT2D player2Position, VECT2D player3Position) {
    if (*lastP2Position != NULL) {
        mapNodes[(*lastP2Position)[0]->y * mapWidth + (*lastP2Position)[0]->x] = *lastP2Position;
    }

    if (*lastP3Position != NULL) {
        mapNodes[(*lastP3Position)[0]->y * mapWidth + (*lastP3Position)[0]->x] = *lastP3Position;
    }

    *lastP2Position = mapNodes[player2Position.y * mapWidth + player2Position.x];
    *lastP3Position = mapNodes[player3Position.y * mapWidth + player3Position.x];

    if (*lastP2Position != NULL) {
        mapNodes[player2Position.y * mapWidth + player2Position.x] = NULL;
    }
    if (*lastP3Position != NULL) {
        mapNodes[player3Position.y * mapWidth + player3Position.x] = NULL;
    }
}
