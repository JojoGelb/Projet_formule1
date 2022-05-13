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
    double cpu_time_used;

    VECT2D acceleration;
    VECT2D speed;
    VECT2D player1Position;
    VECT2D player2Position;
    VECT2D player3Position;
    VECT2D *findex;

    bool_t repeat;

    NODE ***mapNodes;
    NODE **path;
    NODE **pathEssence;
    NODE **lastP2Position;
    NODE **lastP3Position;
    NODE *nodeEnd;
    NODE *nodeStart;

    int mapWidth, mapHeight, gaslevel, i, conso, autreConso, y, x, round;

    char action[100];
    char line_buffer[MAX_LINE_LENGTH];
    char *ansiFegts; /*ne sert à rein à part respecter le ansi sur le retour des fonctions fgets*/

    cpu_time_used = -1;
    start = clock();

    acceleration.x = 0;
    acceleration.y = 0;
    speed.x = 0;
    speed.y = 0;
    round = 0;
    repeat = FALSE;
    

    /* PARTIE LECTURE DES INFOS DE LA COURSE*/
    ansiFegts = fgets(line_buffer, MAX_LINE_LENGTH, stdin); /* Read gas level at Start */
    sscanf(line_buffer, "%d %d %d", &mapWidth, &mapHeight, &gaslevel);

    /*=========================PATHFINDING=============================*/

    findex = generateListIndex();
    mapNodes = createNodeMap(mapWidth, mapHeight);
    nodeEnd = NULL;
    nodeStart = NULL;

    fprintf(stderr, "=== >Map loaded< ===\n");

    for (y = 0; y < mapHeight; ++y) { /* Read map data, line per line */
        ansiFegts = fgets(line_buffer, MAX_LINE_LENGTH, stdin);
        x = 0;
        while (line_buffer[x] != '\0') {
            if (line_buffer[x] == '.' || line_buffer[x] == '~') {
                if (line_buffer[x] == '~') {
                    allocationSpeedForSandNode(mapNodes, findex, mapWidth, x, y);
                } else {
                    mapNodes[y * mapWidth + x] = NULL;
                }
            } else if (line_buffer[x] == '=') {
                allocationSpeedForEndNode(mapNodes, findex, mapWidth, x, y);
                nodeEnd = mapNodes[y * mapWidth + x][findIndex(findex, 0, 0)];
            } else if (line_buffer[x] == '#') {
                allocationSpeedForNormalNode(mapNodes, findex, mapWidth, x, y);
            }
            x++;
        }
        fputs(line_buffer, stderr);
    }

    for (y = 0; y < mapHeight; y++) {
        for (x = 0; x < mapWidth; x++) {
            if (mapNodes[y * mapWidth + x] != NULL) {
                if (!mapNodes[y * mapWidth + x][0]->sable) {
                    findNeighbourForNormalNode(mapNodes, findex, mapWidth, mapHeight, x, y);
                } else {
                    findNeighbourForSandNode(mapNodes, findex, mapWidth, mapHeight, x, y);
                }
            }
        }
    }

    /*==================================================================================================================*/

    path = NULL;
    pathEssence = NULL;

    lastP2Position = NULL;
    lastP3Position = NULL;

    fflush(stderr);

    while (!feof(stdin)) {
        fprintf(stderr,"START WHILE %d %d\n",repeat, round);
        if (cpu_time_used != -1) {
            start = clock();
        }

        ansiFegts = fgets(line_buffer, MAX_LINE_LENGTH, stdin); /* Read positions of pilots*/

        sscanf(line_buffer, "%d %d %d %d %d %d",
                &player1Position.x, &player1Position.y,
                &player2Position.x, &player2Position.y,
                &player3Position.x, &player3Position.y);

        resetMapPlayersPosition(mapNodes,mapWidth,&lastP2Position,&lastP3Position,player2Position,player3Position);
        
        /*Cas particulier: bloqué le trajet et potentiellement un mur*/
        if(repeat == TRUE){
            int speedx = player1Position.x - nodeStart->x;
            int speedy = player1Position.y - nodeStart->y;
            /*Potential issue here*/
            gaslevel -= gasConsumption(0,0,speedx,speedy,nodeStart->sable);
            nodeStart = mapNodes[player1Position.y * mapWidth + player1Position.x][findIndex(findex,speedx,speedy)];

        }

        if (round == 0) {
            fprintf(stderr, "=======================START PATH FINDING==========================\n");
            if(repeat == FALSE){
                nodeStart = mapNodes[player1Position.y * mapWidth + player1Position.x][findIndex(findex, 0, 0)];
                generate_heat_map(mapNodes, mapWidth, mapHeight, nodeEnd);
            }else{
                repeat = FALSE;
            }

            path = get_path(mapNodes, mapWidth, mapHeight, nodeStart);

            if(path[0] == NULL){
                    fprintf(stderr,"------------------cant find da wai ON NORMAL-----------------\n");
                    display_node_map(mapNodes,mapWidth,mapHeight,NULL);
                    repeat = TRUE;
                    sprintf(action, "%d %d", 0, 0);
                    fprintf(stdout, "%s", action);
                    fflush(stdout); /* CAUTION : This is necessary  */
                    continue;
            }

            reversePath(path);
            fprintf(stderr, "==========================END PATH FINDING============================\n");
            display_node_map(mapNodes, mapWidth, mapHeight, path);
        }

        /*Une voiture passe devant*/
        if(hit_a_wall(mapNodes,mapWidth,path[round],path[round+1])){
                fprintf(stderr,"------------------CROSSROAD-----------------\n");
                nodeStart = path[round];
                path = get_path(mapNodes, mapWidth, mapHeight, path[round]);
                round = 0;
                pathEssence = NULL;
                /*NO path available ahead: skip a beat. TODO: Recalcul the position and speed et recalcule speed + essence path*/
                if(path[0] == NULL){
                    fprintf(stderr,"------------------cant find da wai: JUST PAUSE FOR A WHILE-----------------\n");
                    repeat = TRUE;
                    sprintf(action, "%d %d", 0, 0);
                    fprintf(stdout, "%s", action);
                    fflush(stdout);
                    continue;
                }
                reversePath(path);
                display_node_map(mapNodes, mapWidth, mapHeight, path);

                fprintf(stderr,"------------------END CROSSROAD-----------------\n");
            }

        /*Calculer le path essence en decalage pour meilleur perf
        Peut faire mieux
        Mettre dans une fonction
        Split to avoid timeout*/
        if (round != 0 && pathEssence == NULL) {
            fprintf(stderr, "===================START CHECK ESSESNCE===================\n");
            i = 0;
            while (path[i] != NULL) {
                i++;
            }
            conso = 0;

            i--;

            autreConso = calculConsommationEssenceSurTrajet(path, i) + gasConsumption(path[0]->x - nodeStart->x, path[0]->y - nodeStart->y, 0, 0, nodeStart->sable);

            
            /*On skipera si on a assez pour faire un tour complet*/
            while (gaslevel - autreConso - conso < 1 && i > 0) {
                pathEssence = get_path_essence(mapNodes, mapWidth, mapHeight, path[i], &conso);

                autreConso = calculConsommationEssenceSurTrajet(path, i + 1) + gasConsumption(path[0]->x - nodeStart->x, path[0]->y - nodeStart->y, 0, 0, nodeStart->sable);
                i--;
            }

            if(pathEssence != NULL){
                reversePath(pathEssence);
                display_node_map(mapNodes, mapWidth, mapHeight, pathEssence);
            }else{
                fprintf(stderr,"ON PEUT GAS JUSQU'AU BOUT \n");
            }
            fprintf(stderr, "===================FIN CHECK ESSESNCE===================\n");
        }

        /*Quand on arrive au meme point sur la route on swap de path vers pathEssence*/
        if (pathEssence != NULL && player1Position.x == pathEssence[0]->x && player1Position.y == pathEssence[0]->y) {
            round = 0;
            path = pathEssence;
        }

        acceleration = nextAcceleration(path, NULL, &speed, round + 1);
        round++;

        fflush(stderr);

        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
        fprintf(stderr, "\nTemps utilisé: %f \n\n", cpu_time_used);

        sprintf(action, "%d %d", acceleration.x, acceleration.y);
        gaslevel -= gasConsumption(acceleration.x,acceleration.y,path[round-1]->speedX,path[round-1]->speedY,path[round-1]->sable);
        fprintf(stdout, "%s", action);
        fflush(stdout); /* CAUTION : This is necessary  */
        fprintf(stderr, "    Action: %s   Gas remaining: %d\n\n", action, 0);
        fprintf(stderr, "\nSENT\n\n");
        fprintf(stderr, "%s", ansiFegts);
        fflush(stderr);
    }

    return EXIT_SUCCESS;
}