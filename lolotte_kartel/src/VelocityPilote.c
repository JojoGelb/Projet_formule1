#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "AStarAlgorithm.h"

#define MAX_LINE_LENGTH 1024
#define BOOSTS_AT_START 5

int main() {

    clock_t start, end;
    double cpu_time_used = -1;
    start = clock();

    VECT2D acceleration;
    VECT2D speed;

    VECT2D player1Position;
    VECT2D player2Position;
    VECT2D player3Position;

    // VECT2D LastPosition;

    int round = 0;
    int mapWidth, mapHeight, gaslevel, y, x;

    // int boost = BOOSTS_AT_START;

    char action[100];
    char line_buffer[MAX_LINE_LENGTH];

    acceleration.x = 0;
    acceleration.y = 0;
    speed.x = 0;
    speed.y = 0;

    // PARTIE LECTURE DES INFOS DE LA COURSE
    fgets(line_buffer, MAX_LINE_LENGTH, stdin); // Read gas level at Start
    // fprintf(stderr, "=== >Map< ===\n");
    sscanf(line_buffer, "%d %d %d", &mapWidth, &mapHeight, &gaslevel);

    //=========================PATHFINDING=============================//
    VECT2D *findex = generateListIndex();

    // opti ce truc
    NODE ***mapNodes = createNodeMap(findex, mapWidth, mapHeight);
    NODE *nodeEnd = NULL;
    NODE *nodeStart = NULL;

    fprintf(stderr, "=== >Map loaded< ===\n");

    for (y = 0; y < mapHeight; ++y) { /* Read map data, line per line */
        fgets(line_buffer, MAX_LINE_LENGTH, stdin);
        x = 0;
        while (line_buffer[x] != '\0') {
            // IL faut allouer les noeud et leur mettre leur vitesse
            if (line_buffer[x] == '.' || line_buffer[x] == '~') {
                if (line_buffer[x] == '~') {
                    allocationSpeedForSandNode(mapNodes, findex, mapWidth, x, y);
                } else {
                    mapNodes[y * mapWidth + x] = NULL;
                }
            }
            else if (line_buffer[x] == '=') {
                allocationSpeedForEndNode(mapNodes, findex, mapWidth, x, y);
                nodeEnd = mapNodes[y * mapWidth + x][findIndex(findex, 0, 0)];
            }
            else if (line_buffer[x] == '#'){ //si il n'y a pas ce test, le programme crache à la dernière case ???
                allocationSpeedForNormalNode(mapNodes, findex, mapWidth, x, y);
            }
            x++;
        }
        fputs(line_buffer, stderr);
    }

    for(y=0; y<mapHeight; y++){
        for(x=0; x<mapWidth; x++){
            if(mapNodes[y * mapWidth + x] != NULL){
                if(!mapNodes[y * mapWidth + x][0]->sable){
                    findNeighbourForNormalNode(mapNodes, findex, mapWidth, mapHeight, x, y);
                } else {
                    findNeighbourForSandNode(mapNodes, findex, mapWidth, mapHeight, x, y);
                }
            }
        }
    }

    //==================================================================================================================//

    NODE **path;
    NODE **pathEssence = NULL;

    fflush(stderr);

    while (!feof(stdin)) {

        if (cpu_time_used != -1) {
            start = clock();
        }

        fgets(line_buffer, MAX_LINE_LENGTH, stdin); // Read positions of pilots


        sscanf(line_buffer, "%d %d %d %d %d %d",
                &player1Position.x, &player1Position.y,
                &player2Position.x, &player2Position.y,
                &player3Position.x, &player3Position.y);

        if (round == 0) {
            fprintf(stderr, "START PATH FINDING\n");
            nodeStart = mapNodes[player1Position.y * mapWidth + player1Position.x][findIndex(findex, 0, 0)];
            generate_heat_map(mapNodes, mapWidth, mapHeight, nodeEnd);

            path = get_path(mapNodes, mapWidth, mapHeight, nodeStart, nodeEnd);

            reversePath(path);
            fprintf(stderr, "END PATH FINDING\n");
            display_node_map(mapNodes, mapWidth, mapHeight, path);
        }
        fprintf(stderr, "\n3\n");

        //Split to avoid timeout
        if(round != 0 && pathEssence == NULL){
            int i = 0;
            while (path[i] != NULL) {
                i++;
            }
            int conso = 150000;

            i--;
            int autreConso = calculConsommationEssenceSurTrajet(path,i) + gasConsumption(path[0]->x - nodeStart->x,path[0]->y - nodeStart->y,0,0,nodeStart->sable);
            while(gaslevel - autreConso - conso < 1){
                pathEssence = get_path_essence( mapNodes, mapWidth, mapHeight, 
                                                path[i], 
                                                nodeEnd,findex, &conso);

                autreConso = calculConsommationEssenceSurTrajet(path,i+1) + gasConsumption(path[0]->x - nodeStart->x,path[0]->y - nodeStart->y,0,0,nodeStart->sable);
                i--;
            }
            reversePath(pathEssence);
            display_node_map(mapNodes,mapWidth,mapHeight,pathEssence);
        }

        //Swap path
        if(pathEssence != NULL && player1Position.x == pathEssence[0]->x && player1Position.y == pathEssence[0]->y){
            round = 0;
            path = pathEssence;
        }

        acceleration = nextAcceleration(path, NULL, &speed, round+1);

        round++;

        fflush(stderr);

        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        fprintf(stderr, "\nTemps utilisé: %f \n\n", cpu_time_used);

        sprintf(action, "%d %d", acceleration.x, acceleration.y);
        fprintf(stdout, "%s", action);
        fflush(stdout); /* CAUTION : This is necessary  */
        fprintf(stderr, "    Action: %s   Gas remaining: %d\n\n", action, 0);
        fprintf(stderr, "\nSENT\n\n");
        fflush(stderr);
    }

    return EXIT_SUCCESS;
}
