#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "AStarAlgorithm.h"

#define MAX_LINE_LENGTH 1024
#define BOOSTS_AT_START 5

int main()
{

    clock_t start, end;
    double cpu_time_used = -1;
    start = clock();
    
  VECT2D acceleration;
  VECT2D speed;

  VECT2D player1Position;
  VECT2D player2Position;
  VECT2D player3Position;

  //VECT2D LastPosition;

  int round = 0;
  int mapWidth, mapHeight, gaslevel, y;

  //int boost = BOOSTS_AT_START;

  char action[100];
  char line_buffer[MAX_LINE_LENGTH];
  
  acceleration.x = 0;
  acceleration.y = 0;
  speed.x = 0;
  speed.y = 0;
  
  //PARTIE LECTURE DES INFOS DE LA COURSE
  fgets(line_buffer, MAX_LINE_LENGTH, stdin);      //Read gas level at Start
  //fprintf(stderr, "=== >Map< ===\n");
  sscanf(line_buffer, "%d %d %d", &mapWidth, &mapHeight, &gaslevel);

  //=========================PATHFINDING=============================//
  VECT2D *findex = generateListIndex();


  //opti ce truc
  NODE *** mapNodes = createNodeMap(findex,mapWidth,mapHeight);
  NODE * nodeEnd = NULL;
  NODE * nodeStart = NULL;

  //fprintf(stderr, "=== >Map loaded< ===\n");

  for (y = 0; y < mapHeight; ++y) {        /* Read map data, line per line */
    fgets(line_buffer, MAX_LINE_LENGTH, stdin);
    int x = 0;

    while(line_buffer[x] !=  '\0'){
      //fprintf(stderr, "%d = %c\n",y,line_buffer[y]);
      if(line_buffer[x] == '.' || line_buffer[x] == '~' ){
        for(int i = 0; i <NUMBER_OF_SPEED; i++){
          mapNodes[y * mapWidth + x][i]->bObstacle = TRUE;
        }
        
      }

      if(line_buffer[x] == '='){
        nodeEnd = mapNodes[y * mapWidth + x][findIndex(findex,0,0)];
        for(int i = 0; i <NUMBER_OF_SPEED; i++){
          mapNodes[y * mapWidth + x][i]->end = TRUE;
        }
      }
      x++;
    }

    fputs(line_buffer, stderr);
  }

  //fprintf(stderr, "NODE MAP %d %d\n", mapWidth, mapHeight);
  display_node_map(mapNodes, mapWidth, mapHeight,NULL);

  //=================================================================//

  VECT2D ** path;

  fflush(stderr);

  while (!feof(stdin)) {

     //fprintf(stderr, "\nSTART ROUND %d \n",round);

    if(cpu_time_used != -1){
      start = clock();
    }

      fgets(line_buffer, MAX_LINE_LENGTH, stdin);   // Read positions of pilots

      //fprintf(stderr, "\nAFTER GETS\n");

      sscanf(line_buffer, "%d %d %d %d %d %d",
            &player1Position.x, &player1Position.y, 
            &player2Position.x, &player2Position.y, 
            &player3Position.x, &player3Position.y);

    if(round == 0){
        fprintf(stderr, "START PATH FINDING\n");
        nodeStart = mapNodes[player1Position.y * mapWidth + player1Position.x][findIndex(findex,0,0)];


        path = get_path(mapNodes,mapWidth,mapHeight, nodeStart, nodeEnd, gaslevel);
      
      fprintf(stderr, "END PATH FINDING\n");
      display_node_map(mapNodes,mapWidth,mapHeight,path);
      //display_node_map(mapNodes,mapWidth,mapHeight,NULL);

      reversePath(path);
      fprintf(stderr, "END PATH FINDING\n");
      display_node_map(mapNodes,mapWidth,mapHeight,path);

      /*LastPosition.x = 0;
      LastPosition.y = 0;*/
    }

    //Error on the game we took a full stop and speed reset
    /*if(LastPosition.x == player1Position.x && LastPosition.y == player1Position.y){
      speed.x = 0;
      speed.y = 0;
    }
  */
    //fprintf(stderr, "acceleration: Position joueur: %d %d\n", player1Position.x, player1Position.y);
    if(round == 0){
      acceleration = nextAcceleration(path,&player1Position,&speed,round);
    }else{
      acceleration = nextAcceleration(path,NULL,&speed,round);
    }
    round++;
    

    /*
    fprintf(stderr, "acceleration: %d %d\n", acceleration.x, acceleration.y);

    speed.x += acceleration.x;
    speed.y += acceleration.y;

    LastPosition = player1Position;*/

    //fprintf(stderr, "=== ROUND %d\n", round);
    fflush(stderr);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    fprintf(stderr,"\nTemps utilisé: %f \n\n", cpu_time_used);


    sprintf(action, "%d %d",acceleration.x, acceleration.y);
    fprintf(stdout, "%s", action);
    fflush(stdout);                           /* CAUTION : This is necessary  */
    fprintf(stderr, "    Action: %s   Gas remaining: %d\n\n", action, 0);
    fprintf(stderr, "\nSENT\n\n");
    fflush(stderr);

  }

  return EXIT_SUCCESS;
}
