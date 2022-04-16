#include "AStarAlgorithm.h"

NODE * createNode(){
    NODE* node;
    node = (NODE *)malloc(sizeof(NODE));
    if (node == NULL){
        fprintf(stderr, "Crash during node creation\n");
        exit(-1);
    }
    node->vecNeighbours = (NODE**) malloc(NUMBER_OF_NEIBOURGH *sizeof(*node->vecNeighbours));
    return node;
}

NODE* createNodeMap(int width, int height){

    NODE* nodes = (NODE*) malloc(width * height *sizeof(*nodes));
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            nodes[y * width + x] = *createNode();
            nodes[y * width + x].x = x; 
            nodes[y * width + x].y = y; 
            nodes[y * width + x].bObstacle = FALSE; 
            nodes[y * width + x].parent = NULL; 
            nodes[y * width + x].bVisited = FALSE; 
        }
    }

    //FAIRE EN SORTE DE RENSEIGNER OBSTACLE EN AVANCE ICI

    int num_neigh = 0;

    for (int x = 0; x < width; x++){
			for (int y = 0; y < height; y++){
				if(y>0){
                    nodes[y*width + x].vecNeighbours[0] = &nodes[(y - 1) * width + (x + 0)];
                    num_neigh++;
                }
					
				if(y<height-1){
                    nodes[y*width + x].vecNeighbours[1] = &nodes[(y + 1) * width + (x + 0)];
                    num_neigh++;
                }
					
				if (x>0){
                    nodes[y*width + x].vecNeighbours[2] = &nodes[(y + 0) * width + (x - 1)];
                    num_neigh++;
                }
					
				if(x<width-1){
                    nodes[y*width + x].vecNeighbours[3] = &nodes[(y + 0) * width + (x + 1)];
                    num_neigh++;
                }
                nodes[y*width + x].numberOfNeighbours = num_neigh;
                num_neigh = 0;
					

				// We can also connect diagonally
				/*if (y>0 && x>0)
					nodes[y*width + x].vecNeighbours.push_back(&nodes[(y - 1) * width + (x - 1)]);
				if (y<height-1 && x>0)
					nodes[y*width + x].vecNeighbours.push_back(&nodes[(y + 1) * width + (x - 1)]);
				if (y>0 && x<width-1)
					nodes[y*width + x].vecNeighbours.push_back(&nodes[(y - 1) * width + (x + 1)]);
				if (y<height - 1 && x<width-1)
					nodes[y*width + x].vecNeighbours.push_back(&nodes[(y + 1) * width + (x + 1)]);
				*/
			}
    }

    return nodes;
}

void display_node_map(NODE * map, int width, int height,VECT2D ** path){

    char c = '*';

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            //fprintf(stderr, "%d",map[y * width + x].numberOfNeighbours);
            if(map[y * width + x].bObstacle == TRUE){
                fprintf(stderr, ".");
            }else{
                if(path == NULL){
                    fprintf(stderr, "*");
                }else{
                    for(int i = 0; i < 800; i++){
                        if(path[i] == NULL){
                            break;
                        }
                        if(path[i]->x == x && path[i]->y == y){
                            c = '$';
                            break;
                        }
                    }
                    fprintf(stderr, "%c",c);
                    c = '*';
                }
                
            }
        }
        fprintf(stderr, "\n");
    }
    if(path != NULL){
        display_vector(path);
    }
}

void display_vector(VECT2D ** path){
    for(int i = 0; i < 800; i++){
        if(path[i] == NULL){
            break;
        }
        fprintf(stderr, "%d %d - ",path[i]->x, path[i]->y);
    }
}


void Solve_AStar(NODE * nodes, int width, int height, NODE * nodeStart, NODE * nodeEnd){
    
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            nodes[y * width + x].parent = NULL; 
            nodes[y * width + x].bVisited = FALSE; 
            nodes[y * width + x].fLocalGoal = INT_MAX; 
            nodes[y * width + x].fGlobalGoal = INT_MAX; 
        }
    }

    fprintf(stderr, "INIT DONE\n");

    NODE* nodeCurrent = nodeStart;
    nodeStart->fLocalGoal = 0.0f;
    
    nodeStart->fGlobalGoal = heuristic(nodeStart, nodeEnd);

    int lastPushList = 0;
    NODE* not_tested[800];
    not_tested[lastPushList] = nodeStart; 
    lastPushList++;

    fprintf(stderr, "MORE INIT DONE\n");

    while (lastPushList != 0){
        //fprintf(stderr, "START LAST PUSH LIST\n");
        //fprintf(stderr, "BEFORE SORT\nSTART: %d %d\nEnd: %d %d\n", nodeStart->x, nodeStart->y, nodeEnd->x, nodeEnd->y);
        sort_nodes_list(not_tested,lastPushList);
        //fprintf(stderr, "ADTER SORT\nSTART: %d %d\nEnd: %d %d\n", nodeStart->x, nodeStart->y, nodeEnd->x, nodeEnd->y);
        //fprintf(stderr, "--SORTED--\n");
        while(lastPushList != 0 && not_tested[0]->bVisited == TRUE){
            shift_left(not_tested,lastPushList);
            lastPushList--;
        }

        if(lastPushList == 0){
            break;
        }

        //fprintf(stderr, "DID FEW THINGS THERE\n");

        nodeCurrent = not_tested[0];
        nodeCurrent->bVisited = TRUE;


        //fprintf(stderr, "START NEIBOURGH %d\n",nodeCurrent->numberOfNeighbours);
        for(int i = 0;i < nodeCurrent->numberOfNeighbours; i++){
            if(nodeCurrent->vecNeighbours[i]->bVisited == FALSE && nodeCurrent->vecNeighbours[i]->bObstacle == FALSE){
                not_tested[lastPushList] = nodeCurrent->vecNeighbours[i];
                lastPushList++;
            }

            float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeCurrent->vecNeighbours[i]); 


            if (fPossiblyLowerGoal < nodeCurrent->vecNeighbours[i]->fLocalGoal){
					nodeCurrent->vecNeighbours[i]->parent = nodeCurrent;
					nodeCurrent->vecNeighbours[i]->fLocalGoal = fPossiblyLowerGoal;

					// The best path length to the neighbour being tested has changed, so
					// update the neighbour's score. The heuristic is used to globally bias
					// the path algorithm, so it knows if its getting better or worse. At some
					// point the algo will realise this path is worse and abandon it, and then go
					// and search along the next best path.
					nodeCurrent->vecNeighbours[i]->fGlobalGoal = nodeCurrent->vecNeighbours[i]->fLocalGoal + heuristic(nodeCurrent->vecNeighbours[i], nodeEnd);
				}

        }

        

    }
    

}

float distance(NODE * nodeA, NODE * nodeB){
    return sqrtf((nodeA->x - nodeB->x)*(nodeA->x - nodeB->x) + ((nodeA->y - nodeB->y)*(nodeA->y - nodeB->y)));
}

float heuristic(NODE * nodeA, NODE * nodeB){
    return distance(nodeA,nodeB);
}

void sort_nodes_list(NODE ** list, int last_push){
    int i, j, min_idx;
    
    for (i = 0; i < last_push - 1; i++) {

        min_idx = i;
        for (j = i + 1; j < last_push; j++){
            if (list[j]->fGlobalGoal < list[min_idx]->fGlobalGoal){
                min_idx = j;
            }
        }
        
        NODE* temp = list[min_idx];
        list[min_idx] = list[i];
        list[i] = temp;

    }
}

void shift_left(NODE ** list, int last_push){
    for(int i = 1; i < last_push; i++){
        list[i-1] = list[i];
    }
}

VECT2D ** get_path(NODE * nodes, int width, int height, NODE * nodeStart, NODE * nodeEnd){
    VECT2D** path = malloc(800 * sizeof(VECT2D));
	NODE *p = nodeEnd;

    fprintf(stderr, "START SOLVE\n");
    fprintf(stderr, "START: %d %d\nEnd: %d %d\n", nodeStart->x, nodeStart->y, nodeEnd->x, nodeEnd->y);
    Solve_AStar(nodes,width,height,nodeStart,nodeEnd);
    fprintf(stderr, "Solve_AStarSTART: %d %d\nEnd: %d %d\n", nodeStart->x, nodeStart->y, nodeEnd->x, nodeEnd->y);
    fprintf(stderr, "end solve SOLVE\n");
    int i = 0;
	while (p->parent != NULL){
        fprintf(stderr, "PARENTS: %d %d\n", p->x, p->y);
		path[i] = create_vector(p->x,p->y);
		i++;

		p = p->parent;
	}

    

    path[i]= NULL;

    return path;
}

VECT2D * create_vector(int x, int y){
    VECT2D * vect = malloc(sizeof(VECT2D));
    vect->x = x;
    vect->y = y;
    return vect;
}

