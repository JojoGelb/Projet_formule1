#include "AStarAlgorithm.h"
#include "../follow_line/follow_line.h"

NODE * createNode(){
    NODE* node;
    node = (NODE *)malloc(sizeof(NODE));
    if (node == NULL){
        fprintf(stderr, "Crash during node creation\n");
        exit(-1);
    }
    node->speedY = 4; 
    node->speedX = 2; 
    node->vecNeighbours = (NODE**) malloc(NUMBER_OF_NEIBOURGH *sizeof(*node->vecNeighbours));
    return node;
}

int findIndex(VECT2D* tab,char speedX, char speedY){
    for(int i = 0; i<NUMBER_OF_SPEED; i++){
        if(tab[i].x == speedX && tab[i].y == speedY){
            return i;
        }
    }
    return -1;
}

VECT2D * generateListIndex(){
  VECT2D * result = (VECT2D*) malloc(NUMBER_OF_SPEED*sizeof(*result));

  int index = 0;
    for(int i=-5; i< 6; i++){
        for(int j=-5; j< 6; j++){
            if(sqrt(pow(i,2)+pow(j,2)) <= 5){
                result[index].x = i;
                result[index].y = j;
                index++;
            }
        }
    }
  return result;
}

NODE*** createNodeMap(VECT2D* tab,int width, int height){

    NODE*** nodes = (NODE***) malloc(width * height *sizeof(NODE**));
    //fprintf(stderr,"MALLOAC DONE\n");
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            nodes[y * width + x] = (NODE**) malloc(width * height *sizeof(*nodes[y * width + x]));
        }
    }
    //fprintf(stderr,"TAB DONE\n");


    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){

            
            /* 26 possible speed: norm < 5 && (x,y) < 5
                0,0 | 0,1 | 0,2 | 0,3 | 0,4 | 0,5
                1,0 | 1,1 | 1,2 | 1,3 | 1,4
                2,0 | 2,1 | 2,2 | 2,3 | 2,4
                3,0 | 3,1 | 3,2 | 3,3 | 3,4
                4,0 | 4,1 | 4,2 | 4,3
                5,0 | 
            */
           int index = 0;

            for(int i=-5; i< 6; i++){
                for(int j=-5; j< 6; j++){
                    if(sqrt(pow(i,2)+pow(j,2)) <= 5){
                        nodes[y * width + x][index] = createNode();
                        nodes[y * width + x][index]->speedX = i; 
                        nodes[y * width + x][index]->speedY = j; 
                        nodes[y * width + x][index]->x = x; 
                        nodes[y * width + x][index]->y = y; 
                        nodes[y * width + x][index]->bObstacle = FALSE; 
                        nodes[y * width + x][index]->parent = NULL; 
                        nodes[y * width + x][index]->bVisited = FALSE; 
                        nodes[y * width + x][index]->border = FALSE; 
                        nodes[y * width + x][index]->end = FALSE; 
                        index++;
                    }
                }
            }
        }
    }

    //FAIRE EN SORTE DE RENSEIGNER OBSTACLE EN AVANCE ICI

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
    for (int x = 0; x < width; x++){
			for (int y = 0; y < height; y++){
                for(int i =0; i < NUMBER_OF_SPEED; i++){
                    int y_prime = y + nodes[y*width + x][i]->speedY;
                    int x_prime = x + nodes[y*width + x][i]->speedX;

                    //fprintf(stderr,"\n==============\ni = %d\n Position : %d %d\nSpeed: %d %d\nPrime = %d %d\n\n",i,x,y, nodes[y*width + x][i]->speedX, nodes[y*width + x][i]->speedY, x_prime, y_prime);

                    bool_t x_in, y_in;

                    x_in = (x_prime >= 0 && x_prime < width);
                    y_in = (y_prime >= 0 && y_prime < height);


                    // 4
                    if(x_in && y_in){
                        //fprintf(stderr,"in 4\n");
                        
                            nodes[y*width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime) * width + (x_prime)][i];
                            num_neigh++;
                        
                        
                    }

                    // 1
                    if(y_prime>0 && y_prime <= height && x_in){
                        findex = findIndex(tab,nodes[y*width + x][i]->speedX,nodes[y*width + x][i]->speedY-1);
                        //fprintf(stderr,"in 1: %d\n", findex);
                        if(findex != -1){
                            nodes[y*width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime - 1) * width + (x_prime + 0)][findex];
                            num_neigh++;
                        }
                    }

                    // 7
				    if(y_prime<height-1 && y_prime > -1 && x_in){
                        findex = findIndex(tab,nodes[y*width + x][i]->speedX,nodes[y*width + x][i]->speedY+1);
                        //fprintf(stderr,"in 7 : index = %d\n",findex);
                        if(findex != -1){
                            nodes[y*width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime + 1) * width + (x_prime + 0)][findex];
                            num_neigh++;
                        }
                    }

                    // 3
				    if (x_prime>0 && x_prime <= width && y_in){
                        findex = findIndex(tab,nodes[y*width + x][i]->speedX-1,nodes[y*width + x][i]->speedY);
                        //fprintf(stderr,"in 3\n");
                        if(findex != -1){
                            nodes[y*width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime + 0) * width + (x_prime - 1)][findex];
                            num_neigh++;
                        }
                    }

                    // 5
				    if(x_prime<width-1 && x_prime>=-1 && y_in){
                        findex = findIndex(tab,nodes[y*width + x][i]->speedX+1,nodes[y*width + x][i]->speedY);
                        //fprintf(stderr,"in 5\n");
                        if(findex != -1){
                            nodes[y*width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime + 0) * width + (x_prime + 1)][findex];
                            num_neigh++;
                        }
                    }
                     // 0 carefull not to be out of the map 
				    if (y_prime>0 && x_prime>0 && x_prime <= width && y_prime <= height){
                        findex = findIndex(tab,nodes[y*width + x][i]->speedX-1,nodes[y*width + x][i]->speedY-1);
                        //fprintf(stderr,"in 0\n");
                        if(findex != -1){
				    	    nodes[y*width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime - 1) * width + (x_prime - 1)][findex];
                            num_neigh++;
                        }
                    }

                    // 6
                    if (y_prime<height-1 && x_prime>0 && y_prime >= -1 && x_prime <= width){
                        findex = findIndex(tab,nodes[y*width + x][i]->speedX-1,nodes[y*width + x][i]->speedY+1);
                        //fprintf(stderr,"in 6\n");
                        if(findex != -1){
				    	    nodes[y*width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime + 1) * width + (x_prime - 1)][findex];
                            num_neigh++;
                        }
                    }

                    // 2
                    if (y_prime>0 && x_prime<width-1  && y_prime <= height && x_prime >= -1){
                        findex = findIndex(tab,nodes[y*width + x][i]->speedX+1,nodes[y*width + x][i]->speedY-1);
                        //fprintf(stderr,"in 2\n");
                        if(findex != -1){
				    	    nodes[y*width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime - 1) * width + (x_prime + 1)][findex];
                            num_neigh++;
                        }
                    }

                    // 8
                    if (y_prime<height - 1 && x_prime<width-1 && y_prime >= -1 && x_prime >= -1){
                        findex = findIndex(tab,nodes[y*width + x][i]->speedX+1,nodes[y*width + x][i]->speedY+1);
                        //fprintf(stderr,"in 8\n");
                        if(findex != -1){
				    	    nodes[y*width + x][i]->vecNeighbours[num_neigh] = nodes[(y_prime + 1) * width + (x_prime + 1)][findex];
                            num_neigh++;
                        }
                    }

                    nodes[y*width + x][i]->numberOfNeighbours = num_neigh;


                    num_neigh = 0;
                }
			}
			
    }

    return nodes;
}

int hit_a_wall(NODE*** nodeMap,int width, NODE * start, NODE * stop){
{
  InfoLine vline;
  Pos2Dint p;
  initLine(start->x, start->y, stop->x, stop->y, &vline);
  while (nextPoint(&vline, &p, +1) > 0) {
    if (p.x == start->x && p.y == start->y) {
      /* We suppose that the start position is not worth visiting! */
      continue;
    }

    if(nodeMap[p.y * width + p.x][0]->bObstacle){
        return 1;
    }
  }
  return 0;
}

}

void display_node_map(NODE *** map, int width, int height,VECT2D ** path){

    char c = '*';

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(map[y * width + x][0]->bObstacle == TRUE){
                fprintf(stderr, ".");
            }
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

void Solve_AStar(NODE *** nodes, int width, int height, NODE * nodeStart, NODE * nodeEnd){
    
    //fprintf(stderr, "START SOLVE ASTAR\nFIN: %d %d\nDebut: %d %d\n", nodeEnd->x, nodeEnd->y,nodeStart->x, nodeStart->y);


    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            
            /*
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
            */
           for(int i = 0; i < NUMBER_OF_SPEED; i++){
                nodes[y * width + x][i]->parent = NULL; 
                nodes[y * width + x][i]->bVisited = FALSE; 
                nodes[y * width + x][i]->fLocalGoal = INT_MAX; 
                nodes[y * width + x][i]->fGlobalGoal = INT_MAX;
           }
 
        }
    }
    //display_node_map(nodes,width,height,NULL);

    //fprintf(stderr, "INIT DONE\n");

    NODE* nodeCurrent = nodeStart;
    nodeStart->fLocalGoal = 0.0f;
    
    nodeStart->fGlobalGoal = heuristic(nodeStart, nodeEnd);

    int lastPushList = 0;
    int lastNotSorted = 1;
    NODE* not_tested[40000];
    not_tested[lastPushList] = nodeStart; 
    lastPushList++;


    while (lastPushList != 0){
        //fprintf(stderr,"lastpush: %d\n",lastPushList);

        sort_nodes_list(not_tested,lastPushList,&lastNotSorted);
        lastNotSorted = lastPushList;

        while(lastPushList != 0 && not_tested[0]->bVisited == TRUE){
            shift_left(not_tested,lastPushList);
            lastPushList--;
            lastNotSorted--;
        }

        if(lastPushList == 0){
            break;
        }

        nodeCurrent = not_tested[0];
        nodeCurrent->bVisited = TRUE;

        for(int i = 0;i < nodeCurrent->numberOfNeighbours; i++){
            if(nodeCurrent->vecNeighbours[i]->bVisited == FALSE && nodeCurrent->vecNeighbours[i]->bObstacle == FALSE && !hit_a_wall(nodes,width,nodeCurrent,nodeCurrent->vecNeighbours[i])){ // &&nodeCurrent->vecNeighbours[i]->border == FALSE 
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
    //fprintf(stderr, "END ASTAR");

}

float distance(NODE * nodeA, NODE * nodeB){
    //return sqrtf((nodeA->x - nodeB->x + nodeB->speedX)*(nodeA->x - nodeB->x + nodeB->speedX) + ((nodeA->y - nodeB->y + nodeB->speedY)*(nodeA->y - nodeB->y + nodeB->speedY)));
    return sqrtf((nodeA->x - nodeB->x)*(nodeA->x - nodeB->x) + ((nodeA->y - nodeB->y)*(nodeA->y - nodeB->y)));
}

float heuristic(NODE * nodeA, NODE * nodeB){
    return distance(nodeA,nodeB);
}

float heuristicEnd(NODE * nodeA, NODE * nodeB){
    return sqrtf((nodeA->x - nodeB->x + nodeA->speedX)*(nodeA->x - nodeB->x + nodeA->speedX) + ((nodeA->y - nodeB->y + nodeA->speedY)*(nodeA->y - nodeB->y + nodeA->speedY)));;
}

void sort_nodes_list(NODE ** list, int last_push, int *lastBigNumber){
    int i, j;

    NODE* tempNode;

    for(i = *lastBigNumber; i < last_push; i++){
        if(list[i]->fGlobalGoal > 100000){
            continue;
        }

            for (j = 0; j < i; j++) {
                if(list[j]->fGlobalGoal > list[i]->fGlobalGoal){
                    tempNode = list[j];
                    list[j] = list[i];
                    list[i] = tempNode;
                }
            }

    }
}

void shift_left(NODE ** list, int last_push){
    for(int i = 1; i < last_push; i++){
        list[i-1] = list[i];
    }
}

VECT2D ** get_path(NODE *** nodes, int width, int height, NODE * nodeStart, NODE * nodeEnd){
    VECT2D** path = malloc(800 * sizeof(VECT2D));
	NODE *p = nodeEnd;

    Solve_AStar(nodes,width,height,nodeStart,nodeEnd);

    int i = 0;
	while (p->parent != NULL){

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

void reversePath(VECT2D ** path)
{
    int n;
    for(n = 0; n < 800; n++){
        if(path[n] == NULL){
            break;
        }
    }
    for (int low = 0, high = n - 1; low < high; low++, high--)
    {
        VECT2D* temp = path[low];
        path[low] = path[high];
        path[high] = temp;
    }
}


VECT2D nextAcceleration(VECT2D ** path, VECT2D* position, VECT2D* lastSpeed, int index){

    VECT2D newAcceleration;

    if(path[index +1] == NULL){
        newAcceleration.x = 0;
        newAcceleration.y = 0;
        return newAcceleration;
    }

    if(index == 0){
        newAcceleration.x = (path[index]->x - position->x);
        newAcceleration.y =  path[index]->y - position->y ;
    }else{
        //fprintf(stderr, "CALCUL: H%d %d - B%d %d - S%d %d", path[index]->x, path[index]->y, path[index-1]->x, path[index -1]->y, lastSpeed->x, lastSpeed->y);
        newAcceleration.x = (path[index]->x - path[index-1]->x)  - lastSpeed->x;
        newAcceleration.y =  (path[index]->y- path[index-1]->y) - lastSpeed->y;
    }


    

    lastSpeed->x += newAcceleration.x;
    lastSpeed->y += newAcceleration.y;

    fprintf(stderr,"Acceleration %d %d, Speed %d %d\n", newAcceleration.x, newAcceleration.y, lastSpeed->x, lastSpeed->y);


    return newAcceleration;

}
