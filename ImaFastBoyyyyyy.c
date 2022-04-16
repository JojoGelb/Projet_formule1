#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AStarAlgorithm.h"

#define MAX_LINE_LENGTH 1024
#define BOOSTS_AT_START 5

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
int gasConsumption(int accX, int accY, int speedX, int speedY, int inSand)
{
  int gas = accX * accX + accY * accY;
  gas += (int)(sqrt(speedX * speedX + speedY * speedY) * 3.0 / 2.0);
  if (inSand) {
    gas += 1;
  }
  return -gas;
}

int main()
{
  int accelerationX = 0, accelerationY = 0;
  int round = 0;
  int speedX = 0, speedY = 0;
  int mapWidth, mapHeight, gaslevel, y;
  int p2X,p2Y,p3X,p3Y;


  int boost = BOOSTS_AT_START;
  int myX, myY;
  char action[100];
  char line_buffer[MAX_LINE_LENGTH];
  
  
  //PARTIE LECTURE DES INFOS DE LA COURSE
  fgets(line_buffer, MAX_LINE_LENGTH, stdin);      //Read gas level at Start
  fprintf(stderr, "=== >Map< ===\n");
  sscanf(line_buffer, "%d %d %d", &mapWidth, &mapHeight, &gaslevel);

  //=========================PATHFINDING=============================//
  NODE * mapNodes = createNodeMap(mapWidth,mapHeight);
  NODE * nodeEnd;
  NODE * nodeStart;

  for (y = 0; y < mapHeight; ++y) {        /* Read map data, line per line */
    fgets(line_buffer, MAX_LINE_LENGTH, stdin);
    int x = 0;

    while(line_buffer[x] !=  '\0'){
      //fprintf(stderr, "%d = %c\n",y,line_buffer[y]);
      if(line_buffer[x] == '.'){
        mapNodes[y * mapWidth + x].bObstacle = TRUE;
      }
      if(line_buffer[x] == '='){
        nodeEnd = &mapNodes[y * mapWidth + x];
      }
      x++;
    }

    fputs(line_buffer, stderr);
  }

  fprintf(stderr, "NODE MAP %d %d\n", mapWidth, mapHeight);
  display_node_map(mapNodes, mapWidth, mapHeight,NULL);

  //=================================================================//

  accelerationX = 0;
  accelerationY = 0;

  VECT2D ** path;

  fflush(stderr);

  while (!feof(stdin)) {
    fgets(line_buffer, MAX_LINE_LENGTH, stdin);   // Read positions of pilots
    fprintf(stderr, "plant\n");
    sscanf(line_buffer, "%d %d %d %d %d %d",
           &myX, &myY, &p2X, &p2Y, &p3X, &p3Y);

    if(round == 0){
        fprintf(stderr, "START PATH FINDING\n");
      nodeStart = &mapNodes[myY * mapWidth + myX];

     
       fprintf(stderr, "PATHSTART: %d %d\nEnd: %d %d\n", nodeStart->x, nodeStart->y, nodeEnd->x, nodeEnd->y);
      path = get_path(mapNodes,mapWidth,mapHeight, nodeStart, nodeEnd);
      fprintf(stderr, "PATHSTART: %d %d\nEnd: %d %d\n", nodeStart->x, nodeStart->y, nodeEnd->x, nodeEnd->y);
        fprintf(stderr, "END PATH FINDING\n");
      display_node_map(mapNodes,mapWidth,mapHeight,path);
      //display_node_map(mapNodes,mapWidth,mapHeight,NULL);
    }


    round++;
    /**/

    if(round == 1 || round == 2){
      accelerationX = 2;
      accelerationY = 0;
    }else if(round == 1){
      accelerationX = 1;
      accelerationY = 0;
    }else{
      accelerationX = 0;
      accelerationY = 0;
    }
      
    speedX += accelerationX;
    speedY += accelerationY;

    fprintf(stderr, "=== ROUND %d\n", round);
    fflush(stderr);



    sprintf(action, "%d %d", accelerationX, accelerationY);
    fprintf(stdout, "%s", action);
    fflush(stdout);                           /* CAUTION : This is necessary  */
    fprintf(stderr, "    Action: %s   Gas remaining: %d\n", action, 0);
    fflush(stderr);

    boost -=1;

  }

  /*
  while (!feof(stdin)) {
    int myX, myY, secondX, secondY, thirdX, thirdY;
    round++;
    fprintf(stderr, "=== ROUND %d\n", round);
    fflush(stderr);
    fgets(line_buffer, MAX_LINE_LENGTH, stdin);   // Read positions of pilots
    sscanf(line_buffer, "%d %d %d %d %d %d",
           &myX, &myY, &secondX, &secondY, &thirdX, &thirdY);
    fprintf(stderr, "    Positions: Me(%d,%d)  A(%d,%d), B(%d,%d)\n",
            myX, myY, secondX, secondY, thirdX, thirdY);
    fflush(stderr);
    // Gas consumption cannot be accurate here. 
    gasLevel += gasConsumption(accelerationX, accelerationY, speedX, speedY, 0);
    speedX += accelerationX;
    speedY += accelerationY;
    // Write the acceleration request to the race manager (stdout).
    sprintf(action, "%d %d", accelerationX, accelerationY);
    fprintf(stdout, "%s", action);
    fflush(stdout);                           // CAUTION : This is necessary 
    fprintf(stderr, "    Action: %s   Gas remaining: %d\n", action, gasLevel);
    fflush(stderr);
    if (0 && round > 4) { // (DISABLED) Force a segfault for testing purpose
      int * p = NULL;
      fprintf(stderr, "Good Bye!\n");
      fflush(stderr);
      *p = 0;
    }
  }
  */

  return EXIT_SUCCESS;
}
