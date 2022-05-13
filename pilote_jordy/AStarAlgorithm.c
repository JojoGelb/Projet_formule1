#include "AStarAlgorithm_pilote_jordy.h"

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
            nodes[y * width + x].border = FALSE; 
            nodes[y * width + x].end = FALSE; 
        }
    }

    //FAIRE EN SORTE DE RENSEIGNER OBSTACLE EN AVANCE ICI

    int num_neigh = 0;

    for (int x = 0; x < width; x++){
			for (int y = 0; y < height; y++){
				if(y>0){
                    nodes[y*width + x].vecNeighbours[num_neigh] = &nodes[(y - 1) * width + (x + 0)];
                    num_neigh++;
                }
					
				if(y<height-1){
                    nodes[y*width + x].vecNeighbours[num_neigh] = &nodes[(y + 1) * width + (x + 0)];
                    num_neigh++;
                }
					
				if (x>0){
                    nodes[y*width + x].vecNeighbours[num_neigh] = &nodes[(y + 0) * width + (x - 1)];
                    num_neigh++;
                }
					
				if(x<width-1){
                    nodes[y*width + x].vecNeighbours[num_neigh] = &nodes[(y + 0) * width + (x + 1)];
                    num_neigh++;
                }
					
				if (y>0 && x>0){
					nodes[y*width + x].vecNeighbours[num_neigh] = &nodes[(y - 1) * width + (x - 1)];
                    num_neigh++;
                }
                if (y<height-1 && x>0){
					nodes[y*width + x].vecNeighbours[num_neigh] = &nodes[(y + 1) * width + (x - 1)];
                    num_neigh++;
                }
                if (y>0 && x<width-1){
					nodes[y*width + x].vecNeighbours[num_neigh] = &nodes[(y - 1) * width + (x + 1)];
                    num_neigh++;
                }
                if (y<height - 1 && x<width-1){
					nodes[y*width + x].vecNeighbours[num_neigh] = &nodes[(y + 1) * width + (x + 1)];
                    num_neigh++;
                }

                nodes[y*width + x].numberOfNeighbours = num_neigh;
                num_neigh = 0;

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
            }/*else if(map[y * width + x].border == TRUE){
                fprintf(stderr, "\\");
            }*/
            else{
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
    fprintf(stderr, "START PATH \n");
    for(int i = 0; i < 800; i++){
        if(path[i] == NULL){
            break;
        }
        fprintf(stderr, "%d %d - ",path[i]->x, path[i]->y);
    }
    fprintf(stderr, "\nEND PATH \n");
}

void Solve_AStar(NODE * nodes, int width, int height, NODE * nodeStart, NODE * nodeEnd){
    
    fprintf(stderr, "FIN: %d %d\n", nodeEnd->x, nodeEnd->y);

    //Init graph
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            
            //ADDON
            if(nodes[y * width + x].end == FALSE && (
                (x > 0 &&  nodes[y * width + x-1].bObstacle) ||
                (x < width-1 && nodes[y * width + x+1].bObstacle ) ||
                (y > 0 && nodes[(y-1) * width + x].bObstacle) ||
                (y < height -1 && nodes[(y+1) * width + x].bObstacle)
                )){
                    nodes[y * width + x].border = TRUE;
                }
            //ADDON

            nodes[y * width + x].parent = NULL; 
            nodes[y * width + x].bVisited = FALSE; 
            nodes[y * width + x].fLocalGoal = INT_MAX; 
            nodes[y * width + x].fGlobalGoal = INT_MAX; 
        }
    }
    display_node_map(nodes,width,height,NULL);

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
            if(nodeCurrent->vecNeighbours[i]->bVisited == FALSE && nodeCurrent->vecNeighbours[i]->bObstacle == FALSE ){ // &&nodeCurrent->vecNeighbours[i]->border == FALSE 
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

    //fprintf(stderr, "START SOLVE\n");
    //fprintf(stderr, "START: %d %d\nEnd: %d %d\n", nodeStart->x, nodeStart->y, nodeEnd->x, nodeEnd->y);
    Solve_AStar(nodes,width,height,nodeStart,nodeEnd);
    //fprintf(stderr, "Solve_AStarSTART: %d %d\nEnd: %d %d\n", nodeStart->x, nodeStart->y, nodeEnd->x, nodeEnd->y);
    //fprintf(stderr, "end solve SOLVE\n");
    int i = 0;
	while (p->parent != NULL){
        //fprintf(stderr, "PARENTS: %d %d\n", p->x, p->y);
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

/*
0: TOO fast sum speed > 5
1: alright you can go
2: x >5
3: y > 5
*/
int canAccelerate(const VECT2D * currentSpeed, const VECT2D * acceleration){

    if(currentSpeed->x + acceleration->x > 5){
        fprintf(stderr,"X TOO FAST"); 
        return 2;
    } 
    
    if(currentSpeed->y + acceleration->y > 5){
       fprintf(stderr,"Y TOO FAST"); 
        return 3;
    }

    return sqrt(pow(currentSpeed->x+acceleration->x,2)+pow(currentSpeed->y+acceleration->y,2)) <= 5;

}

void nerf(VECT2D * acc, VECT2D * speed){
    if((speed->x > 0 && acc->x > 0) ||(speed->x < -0 && acc->x < 0)){
        acc->x = 0;
    }
    if((speed->y > 0 && acc->y > 0) ||(speed->y < -0 && acc->y < 0)){
        acc->y = 0;
    }
    fprintf(stderr,"NERF\n");
}


int findClosestPointOnTrack(VECT2D ** path, VECT2D * currentPosition){
    int indexResult = -1;
    int difference = abs(path[0]->x - currentPosition->x) + abs(path[0]->y - currentPosition->y);
    int calcul;

    for(int i = 0; i < 800; i++){
        if(path[i] == NULL){
            break;
        }
        calcul = abs(path[i]->x - currentPosition->x) + abs(path[i]->y - currentPosition->y);
        if(calcul < difference){
            indexResult = i;
            difference = calcul;
        }

    }
    return indexResult;
}

VECT2D nextAcceleration(VECT2D ** path, VECT2D * currentSpeed,VECT2D * currentPosition){

    VECT2D newAcceleration;

    VECT2D nextPosition;

    fprintf(stderr,"speed: %d %d \n",currentSpeed->x, currentSpeed->y);

    nextPosition.x = currentPosition->x + currentSpeed->x;
    nextPosition.y = currentPosition->y + currentSpeed->y;

    for(int i = 0; i < 800; i++){
        if(path[i] == NULL){
            break;
        }

        newAcceleration.x = path[i]->x - nextPosition.x;
        newAcceleration.y = path[i]->y - nextPosition.y;


        //Straight to the good place or lower the speed
        if( newAcceleration.x == 0 && newAcceleration.y == 0) {
                fprintf(stderr,"ACCELERATION 0: %d %d \n",newAcceleration.x, newAcceleration.y);
                //free ici surement
                path[i] = NULL;
                return newAcceleration;
        }


        //Need to accelerate
        if( (abs(newAcceleration.y) < 2) && (abs(newAcceleration.x) < 2)){
            
            fprintf(stderr,"NEED TO GO FASTER %d %d \n",newAcceleration.x, newAcceleration.y);
            int canSpeed = canAccelerate(currentSpeed,&newAcceleration);

            while(canSpeed != 1){
                if(canSpeed == 0){
                    if(abs(currentSpeed->x)>abs(currentSpeed->y)){
                        if(currentSpeed->x > 0){
                            newAcceleration.x -=1;
                        }else{
                            newAcceleration.x +=1;
                        }
                    }else{
                        if(currentSpeed->y > 0){
                            newAcceleration.y -=1;
                        }else{
                            newAcceleration.y +=1;
                        }
                    }
                }else if(canSpeed == 2){
                    newAcceleration.x = 0;
                }else if(canSpeed == 3){
                    newAcceleration.y = 0;
                }
            canSpeed = canAccelerate(currentSpeed,&newAcceleration);
            }

            nerf(&newAcceleration,currentSpeed);
            fprintf(stderr,"BOOOOOOOOST: %d %d \n",newAcceleration.x, newAcceleration.y);
            return newAcceleration;
        }
        
    } //ICI PRENDRE EN COMPTE LES BORDS

    //IF WE END UP HERE IT MEANS WE COULDN'T REACH NEXT STEP WITH NORMAL ACCELERATION

    // USE BOOST THEN

    fprintf(stderr, "Can't decide the next moove\n");

    int index = findClosestPointOnTrack(path,&nextPosition);

    fprintf(stderr, "CRASH ICI %d \n", index);

    index -= (int)sqrt(pow(currentSpeed->x,2)+pow(currentSpeed->y,2));

    fprintf(stderr, "CRASH APRES\n");


    //fprintf(stderr, "Position: %d %d\nVitesse: %d %d\nNextWantedPosition: %d %d\n",currentPosition->x,currentPosition->y,currentSpeed->x,currentSpeed->y,path[index]->x,path[index]->y);
    

    //end track
    if(index < 0){
        newAcceleration.x = 0;
        newAcceleration.y = 0;
    }else{
        if(path[index]->x < nextPosition.x){
            newAcceleration.x = -1;
        }else if(path[index-1]->x > nextPosition.x){
            newAcceleration.x = 1;
        }else{
            newAcceleration.x = 0;
        }

        if(path[index-1]->y < nextPosition.y){
            newAcceleration.y = -1;
        }else if(path[index-1]->y > nextPosition.y){
            newAcceleration.y = 1;
        }else{
            newAcceleration.y = 0;
        }

    fprintf(stderr, "Acceleration choisit: %d %d\n",newAcceleration.x,newAcceleration.y);

        int canSpeed = canAccelerate(currentSpeed,&newAcceleration);

            while(canSpeed != 1){
                if(canSpeed == 0){
                    if(abs(currentSpeed->x)>abs(currentSpeed->y)){
                        if(currentSpeed->x > 0){
                            newAcceleration.x -=1;
                        }else{
                            newAcceleration.x +=1;
                        }
                    }else{
                        if(currentSpeed->y > 0){
                            newAcceleration.y -=1;
                        }else{
                            newAcceleration.y +=1;
                        }
                    }
                }else if(canSpeed == 2){
                    newAcceleration.x = 0;
                }else if(canSpeed == 3){
                    newAcceleration.y = 0;
                }
            canSpeed = canAccelerate(currentSpeed,&newAcceleration);
            }

    }
    nerf(&newAcceleration,currentSpeed);
    return newAcceleration;

}
