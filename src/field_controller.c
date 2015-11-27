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
	int imageID,ID,r,g,b,direction,x,y,totalHP,totalActions,totalMana,ac,attack,maxDam,minDam,range,mvmt,
	bluntDR,chopDR,slashDR,pierceDR,earthDR,fireDR,waterDR,lightningDR,earthWeakness,
	fireWeakness,waterWeakness,lightiningWeakness;
	char * name = malloc(sizeof(char) * 32);
	char critType[3];

	char * value = strtok(line,";");
	imageID = atoi(value);

	value = strtok(NULL,";");
	ID = atoi(value);

	value = strtok(NULL,";");
	r = atoi(value);
	value = strtok(NULL,";");
	g = atoi(value);
	value = strtok(NULL,";");
	b = atoi(value);

	value = strtok(NULL,";");
	strcpy(name, value);

	value = strtok(NULL,";");
	direction = atoi(value);
	value = strtok(NULL,";");
	x = atoi(value);
	value = strtok(NULL,";");
	y = atoi(value);

	value = strtok(NULL,";");
	totalHP = atoi(value);
	value = strtok(NULL,";");
	totalActions = atoi(value);
	value = strtok(NULL,";");
	totalMana = atoi(value);

	value = strtok(NULL,";");
	ac= atoi(value);
	value = strtok(NULL,";");
	attack= atoi(value);
	value = strtok(NULL,";");
	maxDam = atoi(value);
	value = strtok(NULL,";");
	minDam= atoi(value);
	value = strtok(NULL,";");
	strcpy(critType,value);
	value = strtok(NULL,";");
	range = atoi(value);
	value = strtok(NULL,";");
	mvmt = atoi(value);

	value = strtok(NULL,";");
	bluntDR = atoi(value);
	value = strtok(NULL,";");
	chopDR = atoi(value);
	value = strtok(NULL,";");
	slashDR = atoi(value);
	value = strtok(NULL,";");
	pierceDR = atoi(value);

	value = strtok(NULL,";");
	earthDR = atoi(value);
	value = strtok(NULL,";");
	fireDR = atoi(value);
	value = strtok(NULL,";");
	waterDR = atoi(value);
	value = strtok(NULL,";");
	lightningDR = atoi(value);

	value = strtok(NULL,";");
	earthWeakness = atoi(value);
	value = strtok(NULL,";");
	fireWeakness = atoi(value);
	value = strtok(NULL,";");
	waterWeakness = atoi(value);
	value = strtok(NULL,";");
	lightiningWeakness = atoi(value);


	if(defineIndividual(newEnemy,imageID,ID,RGB(r,g,b),name,direction,x,y,totalHP,totalActions,totalMana,ac,attack,maxDam,minDam,critType,range,mvmt,
			bluntDR,chopDR,slashDR,pierceDR,earthDR,fireDR,waterDR,lightningDR,earthWeakness,fireWeakness,waterWeakness,lightiningWeakness)){
		printf("failed making new enemy\n");
	}

	free(name);
}

void loadEnemies(enemies * enemiesList, char * enemyFile, char* directory){
	char * fullEnemyFile = appendStrings(directory, enemyFile);
	fullEnemyFile[strlen(fullEnemyFile)-1] = '\0'; //remove '\n' at end of line
	FILE * enemyFP = fopen(fullEnemyFile, "r");
	char line[160];

	while(fgets(line,160,enemyFP) != NULL){
		if (line[0] != '#') {
			individual * newEnemy = initIndividual();
			createEnemyFromLine(newEnemy, line);
			if (doesExist(newEnemy->ID)) {
				addEnemyToEnemies(enemiesList, newEnemy);
			}else{
				destroyIndividual(newEnemy);
			}
		}
	}

	fclose(enemyFP);
	free(fullEnemyFile);
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

void loadEnemyItems(enemies * enemiesList, char * itemFile, char* directory){
	char * fullEnemyFile = appendStrings(directory, itemFile);
	fullEnemyFile[strlen(fullEnemyFile) - 1] = '\0'; //remove '\n' at end of line
	FILE * itemFP = fopen(fullEnemyFile, "r");
	char line[512];
	item * newItem;

	while (fgets(line, 512, itemFP) != NULL) {
		if (line[0] != '#') {
			createEquipItemFromFile(line, enemiesList);
		}
	}
	fclose(itemFP);
	free(fullEnemyFile);
}

void createEquipItemFromFile(char line[512], enemies * enemiesList){
	item * newItem;
	char name[32], description[256];
	char type, weaponDamType, armorClass, itemType;
	int i, enemyId, imageID, ID, r, g, b, x, y, totalHealthMod, healthMod, totalManaMod, manaMod, acMod, attackMod, damMod,
	maxDamMod, minDamMod, minTurns,  maxTurns, mvmtMod, rangeMod, bluntDRMod, chopDRMod, slashDRMod,
	pierceDRMod, earthDRMod, fireDRMod, waterDRMod, lightningDRMod, earthWeaknessMod,
	fireWeaknessMod, waterWeaknessMod, lightiningWeaknessMod, isEquipt;

	char * value = strtok(line,";");
		enemyId = atoi(value);

		value = strtok(NULL,";");
		imageID = atoi(value);

		value = strtok(NULL,";");
		ID = atoi(value);

		value = strtok(NULL,";");
		type = value[0];

		value = strtok(NULL,";");
		weaponDamType = value[0];

		value = strtok(NULL,";");
		armorClass = value[0];

		value = strtok(NULL,";");
		itemType = value[0];

		value = strtok(NULL,";");
		r = atoi(value);
		value = strtok(NULL,";");
		g = atoi(value);
		value = strtok(NULL,";");
		b = atoi(value);

		value = strtok(NULL,";");
		strcpy(name, value);
		value = strtok(NULL,";");
		x = atoi(value);
		value = strtok(NULL,";");
		y = atoi(value);

		value = strtok(NULL,";");
		totalHealthMod = atoi(value);
		value = strtok(NULL,";");
		healthMod = atoi(value);
		value = strtok(NULL,";");
		totalManaMod = atoi(value);
		value = strtok(NULL,";");
		manaMod = atoi(value);

		value = strtok(NULL,";");
		acMod = atoi(value);
		value = strtok(NULL,";");
		attackMod = atoi(value);
		value = strtok(NULL,";");
		damMod = atoi(value);
		value = strtok(NULL,";");
		maxDamMod = atoi(value);
		value = strtok(NULL,";");
		minDamMod = atoi(value);

		value = strtok(NULL,";");
		minTurns = atoi(value);
		value = strtok(NULL,";");
		maxTurns = atoi(value);

		value = strtok(NULL,";");
		mvmtMod = atoi(value);
		value = strtok(NULL,";");
		rangeMod = atoi(value);

		value = strtok(NULL,";");
		bluntDRMod = atoi(value);
		value = strtok(NULL,";");
		chopDRMod = atoi(value);
		value = strtok(NULL,";");
		slashDRMod = atoi(value);
		value = strtok(NULL,";");
		pierceDRMod = atoi(value);

		value = strtok(NULL,";");
		earthDRMod = atoi(value);
		value = strtok(NULL,";");
		fireDRMod = atoi(value);
		value = strtok(NULL,";");
		waterDRMod = atoi(value);
		value = strtok(NULL,";");
		lightningDRMod = atoi(value);

		value = strtok(NULL,";");
		earthWeaknessMod = atoi(value);
		value = strtok(NULL,";");
		fireWeaknessMod = atoi(value);
		value = strtok(NULL,";");
		waterWeaknessMod = atoi(value);
		value = strtok(NULL,";");
		lightiningWeaknessMod = atoi(value);

		value = strtok(NULL,";");
		isEquipt = atoi(value);

		value = strtok(NULL,";");
		strcpy(description, value);

		newItem = createItem(imageID, RGB(r,g,b),x,y, ID, type, name, description, weaponDamType, armorClass, itemType,
				totalHealthMod,healthMod,totalManaMod,manaMod,acMod,attackMod,damMod,maxDamMod,minDamMod, minTurns, maxTurns,
				mvmtMod,rangeMod,bluntDRMod,chopDRMod,slashDRMod,pierceDRMod,earthDRMod,
				fireDRMod,waterDRMod,lightningDRMod,earthWeaknessMod,fireWeaknessMod,
				waterWeaknessMod, lightiningWeaknessMod, isEquipt);

	for(i = 0; i < enemiesList->numEnemies; i++){
		if(enemiesList->enemies[i]->ID == enemyId){
			addItemToIndividual(enemiesList->enemies[i]->backpack, newItem);
			break;
		}
	}

}

item * createFieldItemFromFile(char line[512]){
	item * newItem;
	char name[32], description[256];
	char type, weaponDamType, armorClass, itemType;
	int i, imageID, ID, r, g, b, x, y, totalHealthMod, healthMod, totalManaMod, manaMod, acMod, attackMod, damMod,
	maxDamMod, minDamMod, minTurns, maxTurns, mvmtMod, rangeMod, bluntDRMod, chopDRMod, slashDRMod,
	pierceDRMod, earthDRMod, fireDRMod, waterDRMod, lightningDRMod, earthWeaknessMod,
	fireWeaknessMod, waterWeaknessMod, lightiningWeaknessMod;

	char * value = strtok(line,";");
		imageID = atoi(value);

		value = strtok(NULL,";");
		ID = atoi(value);

		value = strtok(NULL,";");
		type = value[0];

		value = strtok(NULL,";");
		weaponDamType = value[0];

		value = strtok(NULL,";");
		armorClass = value[0];

		value = strtok(NULL,";");
		itemType = value[0];

		value = strtok(NULL,";");
		r = atoi(value);
		value = strtok(NULL,";");
		g = atoi(value);
		value = strtok(NULL,";");
		b = atoi(value);

		value = strtok(NULL,";");
		strcpy(name, value);
		value = strtok(NULL,";");
		x = atoi(value);
		value = strtok(NULL,";");
		y = atoi(value);

		value = strtok(NULL,";");
		totalHealthMod = atoi(value);
		value = strtok(NULL,";");
		healthMod = atoi(value);
		value = strtok(NULL,";");
		totalManaMod = atoi(value);
		value = strtok(NULL,";");
		manaMod = atoi(value);

		value = strtok(NULL,";");
		acMod = atoi(value);
		value = strtok(NULL,";");
		attackMod = atoi(value);
		value = strtok(NULL,";");
		damMod = atoi(value);
		value = strtok(NULL,";");
		maxDamMod = atoi(value);
		value = strtok(NULL,";");
		minDamMod = atoi(value);

		value = strtok(NULL,";");
		minTurns = atoi(value);
		value = strtok(NULL,";");
		maxTurns = atoi(value);

		value = strtok(NULL,";");
		mvmtMod = atoi(value);
		value = strtok(NULL,";");
		rangeMod = atoi(value);

		value = strtok(NULL,";");
		bluntDRMod = atoi(value);
		value = strtok(NULL,";");
		chopDRMod = atoi(value);
		value = strtok(NULL,";");
		slashDRMod = atoi(value);
		value = strtok(NULL,";");
		pierceDRMod = atoi(value);

		value = strtok(NULL,";");
		earthDRMod = atoi(value);
		value = strtok(NULL,";");
		fireDRMod = atoi(value);
		value = strtok(NULL,";");
		waterDRMod = atoi(value);
		value = strtok(NULL,";");
		lightningDRMod = atoi(value);

		value = strtok(NULL,";");
		earthWeaknessMod = atoi(value);
		value = strtok(NULL,";");
		fireWeaknessMod = atoi(value);
		value = strtok(NULL,";");
		waterWeaknessMod = atoi(value);
		value = strtok(NULL,";");
		lightiningWeaknessMod = atoi(value);

		value = strtok(NULL,";");
		strcpy(description, value);

		newItem = createItem(imageID, RGB(r,g,b),x,y, ID, type, name, description, weaponDamType, armorClass, itemType,
				totalHealthMod,healthMod,totalManaMod,manaMod,acMod, attackMod,damMod,maxDamMod,minDamMod, minTurns, maxTurns,
				mvmtMod,rangeMod,bluntDRMod,chopDRMod,slashDRMod,pierceDRMod,earthDRMod,
				fireDRMod,waterDRMod,lightningDRMod,earthWeaknessMod,fireWeaknessMod,
				waterWeaknessMod, lightiningWeaknessMod, 0);

		return newItem;
}

void loadFieldItems(field * thisField, char * itemFile, char* directory){
	char * fullEnemyFile = appendStrings(directory, itemFile);
	fullEnemyFile[strlen(fullEnemyFile) - 1] = '\0'; //remove '\n' at end of line
	FILE * itemFP = fopen(fullEnemyFile, "r");
	char line[512];
	item * newItem;

	while (fgets(line, 512, itemFP) != NULL) {
		if (line[0] != '#') {
			newItem = createFieldItemFromFile(line);
			if (doesExist(newItem->ID)) {
				addItemToField(thisField->thisFieldInventory, newItem);
			} else {
				free(newItem);
			}
		}

	}
	fclose(itemFP);
	free(fullEnemyFile);
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
			char mapName[256];
			strcpy(mapName, tmpSpace->thisTransitInfo->transitMap);
			player->jumpTarget = tmpSpace->thisTransitInfo->targetMapTransitID;

			destroyField(*thisField, player);
			clearEnemies(thisEnemies);
			*thisField = loadMap(mapName, mapDirectory, player, thisEnemies);

			viewShift->xShift = 0;
			viewShift->yShift = 0;
			//load images for new map
			for (y = 0; y < (*thisField)->totalY; y++) {
				for (x = 0; x < (*thisField)->totalX; x++) {
					imageID = ((*thisField)->grid[x][y]->background)->imageID;
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
