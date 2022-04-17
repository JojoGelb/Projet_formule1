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
  VECT2D acceleration;
  VECT2D speed;

  VECT2D player1Position;
  VECT2D player2Position;
  VECT2D player3Position;

  VECT2D LastPosition;

  int round = 0;
  int mapWidth, mapHeight, gaslevel, y;

  int boost = BOOSTS_AT_START;

  char action[100];
  char line_buffer[MAX_LINE_LENGTH];
  
  acceleration.x = 0;
  acceleration.y = 0;
  speed.x = 0;
  speed.y = 0;
  
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
      if(line_buffer[x] == '.' || line_buffer[x] == '~' ){
        mapNodes[y * mapWidth + x].bObstacle = TRUE;
      }

      if(line_buffer[x] == '='){
        nodeEnd = &mapNodes[y * mapWidth + x];
        mapNodes[y * mapWidth + x].end = TRUE;
      }
      x++;
    }

    fputs(line_buffer, stderr);
  }

  fprintf(stderr, "NODE MAP %d %d\n", mapWidth, mapHeight);
  display_node_map(mapNodes, mapWidth, mapHeight,NULL);

  //=================================================================//

  VECT2D ** path;

  fflush(stderr);

  while (!feof(stdin)) {

    fgets(line_buffer, MAX_LINE_LENGTH, stdin);   // Read positions of pilots
    sscanf(line_buffer, "%d %d %d %d %d %d",
            &player1Position.x, &player1Position.y, 
            &player2Position.x, &player2Position.y, 
            &player3Position.x, &player3Position.y);

    if(round == 0){
        fprintf(stderr, "START PATH FINDING\n");
      nodeStart = &mapNodes[player1Position.y * mapWidth + player1Position.x];

      fprintf(stderr, "PATHSTART: %d %d\nEnd: %d %d\n", nodeStart->x, nodeStart->y, nodeEnd->x, nodeEnd->y);
      path = get_path(mapNodes,mapWidth,mapHeight, nodeStart, nodeEnd);
      fprintf(stderr, "PATHSTART: %d %d\nEnd: %d %d\n", nodeStart->x, nodeStart->y, nodeEnd->x, nodeEnd->y);
        fprintf(stderr, "END PATH FINDING\n");
      display_node_map(mapNodes,mapWidth,mapHeight,path);
      //display_node_map(mapNodes,mapWidth,mapHeight,NULL);

      LastPosition.x = 0;
      LastPosition.y = 0;
    }

    //Error on the game we took a full stop and speed reset
    if(LastPosition.x == player1Position.x && LastPosition.y == player1Position.y){
      speed.x = 0;
      speed.y = 0;
    }

    round++;

    acceleration = nextAcceleration(path,&speed,&player1Position);
    
    fprintf(stderr, "acceleration: %d %d\n", acceleration.x, acceleration.y);

    speed.x += acceleration.x;
    speed.y += acceleration.y;

    LastPosition = player1Position;

    fprintf(stderr, "=== ROUND %d\n", round);
    fflush(stderr);



    sprintf(action, "%d %d", acceleration.x, acceleration.y);
    fprintf(stdout, "%s", action);
    fflush(stdout);                           /* CAUTION : This is necessary  */
    fprintf(stderr, "    Action: %s   Gas remaining: %d\n\n", action, 0);
    fflush(stderr);

    

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
