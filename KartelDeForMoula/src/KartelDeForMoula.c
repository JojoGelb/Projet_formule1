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
    bool_t onPeutFinir;
    bool_t PATH_ESSENCE_GENERATED;
    bool_t CHECK_NERF;

    NODE ***mapNodes;
    NODE **path;
    NODE **pathNerf;
    NODE **pathEssence;
    NODE **lastP2Position;
    NODE **lastP3Position;
    NODE *nodeEnd;
    NODE *nodeStart;

    int mapWidth, mapHeight, gaslevel, i, conso, consoPathD, consoPathDUtilseEssence, consoPathNerf, y, x, round, valeurNerf, tailleE, tailleN, lastFart;

    char action[100];
    char line_buffer[MAX_LINE_LENGTH];

    cpu_time_used = -1;
    start = clock();

    valeurNerf = 0;
    acceleration.x = 0;
    acceleration.y = 0;
    speed.x = 0;
    speed.y = 0;
    round = 0;
    lastFart = 1;
    i = 0;
    repeat = FALSE;
    PATH_ESSENCE_GENERATED = TRUE;
    onPeutFinir = FALSE;
    CHECK_NERF = TRUE;
    

    /* PARTIE LECTURE DES INFOS DE LA COURSE*/
    fgets(line_buffer, MAX_LINE_LENGTH, stdin); /* Read gas level at Start */
    sscanf(line_buffer, "%d %d %d", &mapWidth, &mapHeight, &gaslevel);

    /*=========================PATHFINDING=============================*/

    findex = generateListIndex();
    mapNodes = createNodeMap(mapWidth, mapHeight);
    nodeEnd = NULL;
    nodeStart = NULL;

    for (y = 0; y < mapHeight; ++y) { /* Read map data, line per line */
        fgets(line_buffer, MAX_LINE_LENGTH, stdin);
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

    while (!feof(stdin)) {

        if (cpu_time_used != -1) {
            start = clock();
        }

        /*Le fgets reste coincé parfois à cause d'une bug du GDC*/
        /*fprintf(stderr,"BEFORE GETS\n");*/
        fgets(line_buffer, MAX_LINE_LENGTH, stdin); /* Read positions of pilots*/
        /*fprintf(stderr,"AFTER GETS\n");*/
        fflush(stderr);

        sscanf(line_buffer, "%d %d %d %d %d %d",
                &player1Position.x, &player1Position.y,
                &player2Position.x, &player2Position.y,
                &player3Position.x, &player3Position.y);

        resetMapPlayersPosition(mapNodes,mapWidth,&lastP2Position,&lastP3Position,player2Position,player3Position);
        
        /*Cas particulier: bloqué le trajet et potentiellement un mur*/
        if(repeat == TRUE){
            int speedx = player1Position.x - nodeStart->x;
            int speedy = player1Position.y - nodeStart->y;
            gaslevel -= gasConsumption(0,0,speedx,speedy,nodeStart->sable);
            nodeStart = mapNodes[player1Position.y * mapWidth + player1Position.x][findIndex(findex,speedx,speedy)];

        }

        if (round == 0) {
            if(repeat == FALSE){
                nodeStart = mapNodes[player1Position.y * mapWidth + player1Position.x][findIndex(findex, 0, 0)];
                generate_heat_map(mapNodes, mapWidth, mapHeight, nodeEnd);
            }else{
                repeat = FALSE;
            }

            consoPathD = 0;
            path = get_path(mapNodes, mapWidth, mapHeight, nodeStart, &consoPathD,valeurNerf);

            if(path[0] == NULL){
                round = 0;
                repeat = TRUE;
                sprintf(action, "%d %d", 0, 0);
                fprintf(stdout, "%s", action);
                fflush(stdout); /* CAUTION : This is necessary  */
                continue;
            }
            reversePath(path);
        }

        /*Une voiture passe devant*/
        if(hit_a_wall(mapNodes,mapWidth,path[round],path[round+1])){
                onPeutFinir = FALSE;
                nodeStart = path[round];
                path = get_path(mapNodes, mapWidth, mapHeight, path[round],&consoPathD,valeurNerf);
                round = 0;
                lastFart += 5-valeurNerf;
                pathEssence = NULL;

                if(path[0] == NULL){
                    repeat = TRUE;
                    sprintf(action, "%d %d", 0, 0);
                    fprintf(stdout, "%s", action);
                    fflush(stdout);
                    continue;
                }
                reversePath(path);
            }

        /*Calcule du path-essence*/
        if (round == 1 && pathEssence == NULL && onPeutFinir == FALSE) {
            i = 0;
            while (path[i] != NULL) {
                i++;
            }
            conso = 0;

            consoPathDUtilseEssence = calculConsommationEssenceSurTrajet(path,round,i);
            i-=2;
            
            while (gaslevel - consoPathDUtilseEssence - conso < lastFart && i > 0) {
                end = clock();
                cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
                if(cpu_time_used > 0.90){ /*pour éviter les timeout si le calcul prend trop de temps, 0.9 car la suite du code met en moyenne 0.065 sec à s'éxecuter*/
                    PATH_ESSENCE_GENERATED = FALSE;
                    break;
                }
                pathEssence = get_path_essence(mapNodes, mapWidth, mapHeight, path[i], &conso);
                consoPathDUtilseEssence = calculConsommationEssenceSurTrajet(path, round, i+1);
                i--;
                PATH_ESSENCE_GENERATED = TRUE;
            }

            if(pathEssence != NULL){
                reversePath(pathEssence);
            }else{
                CHECK_NERF = FALSE;
                onPeutFinir = TRUE;
            }
        }

        /*Quand on arrive au meme point sur la route on échange le A* avec le de path de pathEssence*/
        if (pathEssence != NULL && player1Position.x == pathEssence[0]->x && player1Position.y == pathEssence[0]->y) {
            round = 0;
            path = pathEssence;
        }

        /*On check si limiter la vitesse maximale est plus rentable*/
        if(round == 2 && CHECK_NERF == TRUE){
            if(PATH_ESSENCE_GENERATED == FALSE){ /*si le path-essence est pas généré, cela veut dire que l'on devrait freiner très tôt dans le circuit*/
                valeurNerf++;
                onPeutFinir = FALSE;
                nodeStart = path[round];
                path = get_path(mapNodes, mapWidth, mapHeight, path[round],&consoPathD,valeurNerf);
                round = 0;
                pathEssence = NULL;

                if(path[0] == NULL){
                    repeat = TRUE;
                    sprintf(action, "%d %d", 0, 0);
                    fprintf(stdout, "%s", action);
                    fflush(stdout);
                    continue;
                }
                reversePath(path);

            } else {
                consoPathNerf = 0;
                pathNerf = get_path(mapNodes, mapWidth, mapHeight, path[round], &consoPathNerf,valeurNerf+1);

                if(pathNerf[0] != NULL){
                    tailleE = 0;
                    tailleN = 0;
                    while (pathNerf[tailleN] != NULL) {
                        tailleN++;
                    }
                    while (pathEssence[tailleE] != NULL) {
                        tailleE++;
                    }
                    if(i + tailleE > tailleN && consoPathNerf-gaslevel < conso){
                        valeurNerf++;
                        onPeutFinir = FALSE;
                        nodeStart = path[round];
                        round = 0;
                        pathEssence = NULL;
                        reversePath(pathNerf);
                        path = pathNerf;
                    } else {
                        CHECK_NERF = FALSE;
                    }
                } else {
                    CHECK_NERF = FALSE;
                }
            }
        }

        acceleration = nextAcceleration(path, NULL, &speed, round + 1);
        round++;

        gaslevel -= gasConsumption(acceleration.x,acceleration.y,path[round-1]->speedX,path[round-1]->speedY,path[round-1]->sable);

        end = clock();
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

        sprintf(action, "%d %d\n", acceleration.x, acceleration.y);

        fprintf(stdout, "%s", action);
        fflush(stdout); /* CAUTION : This is necessary  */
        fflush(stderr);
        
    }

    return EXIT_SUCCESS;
}