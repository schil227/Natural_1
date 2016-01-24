/*
 * field_controller.c
 *
 *  Created on: Jul 23, 2015
 *      Author: Adrian
 */

#include "./headers/field_controller_pub_methods.h"
#include<stdio.h>

individualGroup * initGroup(){
	int index;
	individualGroup * thisGroup = malloc(sizeof(individualGroup));
	thisGroup->numIndividuals = 0;
	thisGroup->currentIndividualIndex = -1;
	for(index = 0; index < 50; index++){
		thisGroup->individuals[index] = NULL;
	}
	return thisGroup;
}

int addIndividualToGroup(individualGroup * thisGroup, individual * thisIndividual){
	if(thisGroup->numIndividuals < 50){
		int index = thisGroup->numIndividuals;
		thisGroup->individuals[index] = thisIndividual;
		thisGroup->numIndividuals = thisGroup->numIndividuals + 1;
		return 1;
	}

	return 0;
}

void createIndividualFromLine(individual * newIndividual, char * line){
	int imageID,ID,r,g,b,direction,x,y,totalHP,totalActions,totalMana,ac,attack,maxDam,minDam,range,mvmt,
	bluntDR,chopDR,slashDR,pierceDR,earthDR,fireDR,waterDR,lightningDR,earthWeakness,
	fireWeakness,waterWeakness,lightiningWeakness,dialogID;
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

	value = strtok(NULL,";");
	dialogID = atoi(value);

	dialogID = loadOrAddIndividualDialog(ID,dialogID);
	if(defineIndividual(newIndividual,imageID,ID,RGB(r,g,b),name,direction,x,y,totalHP,totalActions,totalMana,ac,attack,maxDam,minDam,critType,range,mvmt,
			bluntDR,chopDR,slashDR,pierceDR,earthDR,fireDR,waterDR,lightningDR,earthWeakness,fireWeakness,waterWeakness,lightiningWeakness, dialogID)){
		printf("failed making new individual\n");
	}

	free(name);
}



void loadGroup(individualGroup * group, char * fileName, char* directory){
	char * fullFileName = appendStrings(directory, fileName);
	fullFileName[strlen(fullFileName)-1] = '\0'; //remove '\n' at end of line
	FILE * FP = fopen(fullFileName, "r");
	char line[160];

	while(fgets(line,160,FP) != NULL){
		if (line[0] != '#') {
			char * value = strtok(line,";");
			int id = atoi(value);

			individual * newIndividual = getIndividualFromRegistry(id);
			if(newIndividual != NULL){
				if (doesExist(newIndividual->ID)) {
					addIndividualToGroup(group, newIndividual);
				}
			}else{
				char * errLog[128];
				sprintf(errLog, "!!INDIVIDUAL NOT FOUND : %d!!", id);
				cwrite(errLog);
			}

		}
	}

	fclose(FP);
	free(fullFileName);
}

void clearGroup(individualGroup * thisGroup){
	int i;
	for(i = 0; i < thisGroup->numIndividuals; i++){
		thisGroup->individuals[i] = NULL;
	}
	thisGroup->numIndividuals = 0;
}

void setGroupToField(field * thisField, individualGroup * thisGroup){
	individual * tmpEnemy;
	int i;
	for(i = 0; i < thisGroup->numIndividuals; i++){
		tmpEnemy = thisGroup->individuals[i];
		moveIndividualSpace(thisField,tmpEnemy,tmpEnemy->playerCharacter->x, tmpEnemy->playerCharacter->y);
	}
}

void loadGroupItems(individualGroup * group, char * itemFile, char* directory){
	char * fullItemFile = appendStrings(directory, itemFile);
	fullItemFile[strlen(fullItemFile) - 1] = '\0'; //remove '\n' at end of line
	FILE * itemFP = fopen(fullItemFile, "r");
	char line[512];
	item * newItem;

//	while (fgets(line, 512, itemFP) != NULL) {
//		if (line[0] != '#') {
//			createEquipItemFromFile(line, group);
//		}
//	}
	fclose(itemFP);
	free(fullItemFile);
}

void createEquipItemFromFile(char line[512], individualGroup * group){
	item * newItem;
	char name[32], description[256];
	char type, weaponDamType, armorClass, itemType;
	int i, itemAdded=0, npcID, imageID, ID, r, g, b, x, y, totalHealthMod, healthMod, totalManaMod, manaMod, acMod, attackMod, damMod,
	maxDamMod, minDamMod, minTurns,  maxTurns, mvmtMod, rangeMod, bluntDRMod, chopDRMod, slashDRMod,
	pierceDRMod, earthDRMod, fireDRMod, waterDRMod, lightningDRMod, earthWeaknessMod,
	fireWeaknessMod, waterWeaknessMod, lightiningWeaknessMod, isEquipt;

	char * value = strtok(line,";");
		npcID = atoi(value);

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

		newItem = createItem(npcID, imageID, RGB(r,g,b),x,y, ID, type, name, description, weaponDamType, armorClass, itemType,
				totalHealthMod,healthMod,totalManaMod,manaMod,acMod,attackMod,damMod,maxDamMod,minDamMod, minTurns, maxTurns,
				mvmtMod,rangeMod,bluntDRMod,chopDRMod,slashDRMod,pierceDRMod,earthDRMod,
				fireDRMod,waterDRMod,lightningDRMod,earthWeaknessMod,fireWeaknessMod,
				waterWeaknessMod, lightiningWeaknessMod, isEquipt);

	for(i = 0; i < group->numIndividuals; i++){
		if(group->individuals[i]->ID == npcID){
			addItemToIndividual(group->individuals[i]->backpack, newItem);
			itemAdded = 1;
			break;
		}
	}

	if(!itemAdded){
		destroyItem(newItem);
	}

}

item * createFieldItemFromFile(char line[512]){
	item * newItem;
	char name[32], description[256];
	char type, weaponDamType, armorClass, itemType;
	int i, npcID, imageID, ID, r, g, b, x, y, totalHealthMod, healthMod, totalManaMod, manaMod, acMod, attackMod, damMod,
	maxDamMod, minDamMod, minTurns, maxTurns, mvmtMod, rangeMod, bluntDRMod, chopDRMod, slashDRMod,
	pierceDRMod, earthDRMod, fireDRMod, waterDRMod, lightningDRMod, earthWeaknessMod,
	fireWeaknessMod, waterWeaknessMod, lightiningWeaknessMod;

	char * value = strtok(line,";");
	npcID = atoi(value);

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
		strcpy(description, value);

		newItem = createItem(npcID, imageID, RGB(r,g,b),x,y, ID, type, name, description, weaponDamType, armorClass, itemType,
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

	while (fgets(line, 512, itemFP) != NULL) {
		if (line[0] != '#') {
			char * value = strtok(line,";");
			int id = atoi(value);

			item * newItem = getItemFromRegistry(id);//createFieldItemFromFile(line);
			if(newItem != NULL){
				if (doesExist(newItem->ID)) {
					addItemToField(thisField->thisFieldInventory, newItem);
				}
			}else{
				char * errLog[128];
				sprintf(errLog, "!! ITEM NOT FOUND : %d !!", id);
			}
		}

	}
	fclose(itemFP);
	free(fullEnemyFile);
}

individual *  deleteIndividiaulFromGroup(individualGroup * thisGroup, individual * thisIndividual){
	int index;
	int numIndividuals = thisGroup->numIndividuals;
	for( index = 0; index < numIndividuals; index++){
		if(thisGroup->individuals[index] == thisIndividual){
			individual * toReturn = thisGroup->individuals[index];
			if(index == numIndividuals - 1){ //last index
				thisGroup->individuals[index] = NULL;
			}else{
				thisGroup->individuals[index] = thisGroup->individuals[numIndividuals-1];
				thisGroup->individuals[numIndividuals-1] = NULL;
			}

			thisGroup->numIndividuals = thisGroup->numIndividuals - 1;
			return toReturn;

		}
	}

	return NULL;

}

int attemptToTransit(field ** thisField, individual * player, individualGroup * enemies, individualGroup * npcs, shiftData * viewShift, char * mapDirectory){
	space * tmpSpace = (*thisField)->grid[player->playerCharacter->x][player->playerCharacter->y];

		if(tmpSpace->thisTransitInfo != NULL && tmpSpace->thisTransitInfo->targetMapTransitID != 0){
			int x, y, imageID;
			char mapName[256];
			strcpy(mapName, tmpSpace->thisTransitInfo->transitMap);
			player->jumpTarget = tmpSpace->thisTransitInfo->targetMapTransitID;

			destroyField(*thisField, player);
			clearGroup(enemies);
			clearGroup(npcs);
			*thisField = loadMap(mapName, mapDirectory, player, enemies, npcs);

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

int tryTalk(individualGroup * thisGroup, individual * thisIndividual, int cursorX, int cursorY){
	int index;
	for (index = 0; index < thisGroup->numIndividuals; index++) {

		individual * tmpIndividual = thisGroup->individuals[index];

		if (tmpIndividual->playerCharacter->x == cursorX && tmpIndividual->playerCharacter->y == cursorY && individualWithinRange(thisIndividual, tmpIndividual)) {
			if(setCurrentMessageByIndividualID(tmpIndividual->ID)){
				setSpeakingIndividualID(tmpIndividual->ID);
				toggleDrawDialogBox();
			}
			return 1;
		}
	}
	return 0;
}
