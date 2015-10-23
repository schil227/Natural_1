/*
 * field_controller.c
 *
 *  Created on: Jul 23, 2015
 *      Author: Adrian
 */

#include "./headers/field_controller_pub_methods.h"
#include<stdio.h>

enemies * initEnemies(){
	int index;
	enemies * thisEnemies = malloc(sizeof(enemies));
	thisEnemies->numEnemies = 0;
	for(index = 0; index < 50; index++){
		thisEnemies->enemies[index] = NULL;
	}
	return thisEnemies;
}

int addEnemyToEnemies(enemies * thisEnemies, individual * enemey){
	if(thisEnemies->numEnemies < 50){
		int index = thisEnemies->numEnemies;
		thisEnemies->enemies[index] = enemey;
		thisEnemies->numEnemies = thisEnemies->numEnemies + 1;
		return 1;
	}

	return 0;
}

void createEnemyFromLine(individual * newEnemy, char * line){
	int imageID,ID,r,g,b,direction,x,y,totalHP,totalActions,ac,attack,maxDam,minDam,range,mvmt;
	char * name;
	char critType[3];

	char * value = strtok(line,",");
	imageID = atoi(value);

	value = strtok(NULL,",");
	ID = atoi(value);

	value = strtok(NULL,",");
	r = atoi(value);
	value = strtok(NULL,",");
	g = atoi(value);
	value = strtok(NULL,",");
	b = atoi(value);

	value = strtok(NULL,",");
	name = value;
	value = strtok(NULL,",");
	direction = atoi(value);
	value = strtok(NULL,",");
	x = atoi(value);
	value = strtok(NULL,",");
	y = atoi(value);
	value = strtok(NULL,",");
	totalHP = atoi(value);
	value = strtok(NULL,",");
	totalActions = atoi(value);
	value = strtok(NULL,",");
	ac= atoi(value);
	value = strtok(NULL,",");
	attack= atoi(value);
	value = strtok(NULL,",");
	maxDam = atoi(value);
	value = strtok(NULL,",");
	minDam= atoi(value);
	value = strtok(NULL,",");
	strcpy(critType,value);
	value = strtok(NULL,",");
	range = atoi(value);
	value = strtok(NULL,",");
	mvmt = atoi(value);

	if(defineIndividual(newEnemy,imageID,ID,RGB(r,g,b),name,direction,x,y,totalHP,totalActions,ac,attack,maxDam,minDam,critType,range,mvmt)){
		printf("failed making new enemy\n");
	}
}

void loadEnemies(enemies * enemiesList, char * enemyFile, char* directory){
	char * fullEnemyFile = appendStrings(directory, enemyFile);
	fullEnemyFile[strlen(fullEnemyFile)-1] = '\0'; //remove '\n' at end of line
	FILE * enemyFP = fopen(fullEnemyFile, "r");
	char line[80];

	while(fgets(line,80,enemyFP) != NULL){
		individual * newEnemy = initIndividual();
		createEnemyFromLine(newEnemy, line);
		if(doesExist(newEnemy->ID)){
			addEnemyToEnemies(enemiesList,newEnemy);
		}
	}

	fclose(enemyFP);
}

void clearEnemies(enemies * thisEnemies){
	int i;
	for(i = 0; i < thisEnemies->numEnemies; i++){
		destroyIndividual(thisEnemies->enemies[i]);
	}
	thisEnemies->numEnemies = 0;
}

void setEnemiesToField(field * thisField, enemies * enemiesList){
	individual * tmpEnemy;
	int i;
	for(i = 0; i < enemiesList->numEnemies; i++){
		tmpEnemy = enemiesList->enemies[i];
		setIndividualSpace(thisField,tmpEnemy,tmpEnemy->playerCharacter->x, tmpEnemy->playerCharacter->y);
	}
}

individual *  deleteEnemyFromEnemies(enemies * thisEnemies, individual * enemey){
	int index;
	int numEnemies = thisEnemies->numEnemies;
	for( index = 0; index < numEnemies; index++){
		if(thisEnemies->enemies[index] == enemey){
			individual * toReturn = thisEnemies->enemies[index];
			if(index == numEnemies - 1){ //last index
				thisEnemies->enemies[index] = NULL;
			}else{
				thisEnemies->enemies[index] = thisEnemies->enemies[numEnemies-1];
				thisEnemies->enemies[numEnemies-1] = NULL;
			}

			thisEnemies->numEnemies = thisEnemies->numEnemies - 1;
			return toReturn;

		}
	}

	return NULL;

}

int attemptToTransit(field ** thisField, individual * player, enemies * thisEnemies, shiftData * viewShift, char * mapDirectory){
	space * tmpSpace = (*thisField)->grid[player->playerCharacter->x][player->playerCharacter->y];

		if(tmpSpace->thisTransitInfo != NULL && tmpSpace->thisTransitInfo->targetMapTransitID != 0){
			int x, y, imageID;
			player->jumpTarget = tmpSpace->thisTransitInfo->targetMapTransitID;
			free(*thisField);
			*thisField = loadMap(tmpSpace->thisTransitInfo->transitMap,  mapDirectory, player, thisEnemies);
			viewShift->xShift = 0;
			viewShift->yShift = 0;
			for (y = 0; y < (*thisField)->totalY; y++) {
				for (x = 0; x < (*thisField)->totalX; x++) {
					imageID = ((*thisField)->grid[x][y]->background)->imageID;
					(*thisField)->grid[x][y]->background->image = malloc(
							sizeof(HBITMAP));
					(*thisField)->grid[x][y]->background->image = LoadBitmap(GetModuleHandle(NULL), imageID);
					if ((*thisField)->grid[x][y]->background->image == NULL) {
						printf("failed\n");
					}
				}
			}

			clearMessages();
			return 1;
		}
	return 0;
}
