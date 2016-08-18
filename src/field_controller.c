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
	thisGroup->MAX_INDIVIDUALS = 50;
	for(index = 0; index < 50; index++){
		thisGroup->individuals[index] = NULL;
	}
	return thisGroup;
}

int addIndividualToGroup(individualGroup * thisGroup, individual * thisIndividual){
	int i;
	for(i = 0; i < thisGroup->MAX_INDIVIDUALS; i++){
		if(thisGroup->individuals[i] == NULL){
			thisGroup->individuals[i] = thisIndividual;
			thisGroup->numIndividuals++;
			return 1;
		}
	}

	return 0;
}

void nextAvailableIndividualIndex(individualGroup * thisGroup){
	int i, startingIndex;

	startingIndex = thisGroup->currentIndividualIndex;

	if(startingIndex < 0){
		i = 0;
	}else{
		i = startingIndex + 1;
	}

	for(i; i < thisGroup->MAX_INDIVIDUALS; i++){
		if(thisGroup->individuals[i] != NULL){
			thisGroup->currentIndividualIndex = i;
			return;
		}
	}

	thisGroup->currentIndividualIndex = -1;
}

void createIndividualFromLine(individual * newIndividual, char * line){
	int imageID,ID,r,g,b,direction,x,y,baseHP,totalActions,baseMana,ac,attack,maxDam,minDam,range,mvmt,los,isSneaking,
	bluntDR,chopDR,slashDR,pierceDR,earthDR,fireDR,waterDR,lightningDR,earthWeakness,
	fireWeakness,waterWeakness,lightiningWeakness,dialogID,gold,STR,DEX,CON,WILL,INT,WIS,CHR,LUCK,baseDam;
	char * name = malloc(sizeof(char) * 32);
	char * strtok_save_pointer;
	char critType[4];
	animationContainer * thisAnimationContainer = initAnimationContainer();
	animationContainer * secondaryAnimationContainer = NULL;

	char * value = strtok_r(line,";",&strtok_save_pointer);
	imageID = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	ID = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	r = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	g = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	b = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	strcpy(name, value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	direction = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	x = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	y = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	STR = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	DEX = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	CON = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	WILL = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	INT = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	WIS = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	CHR = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	LUCK = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	baseHP = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	totalActions = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	baseMana = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	ac= atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	attack= atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	maxDam = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	minDam= atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	baseDam = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	strcpy(critType,value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	range = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	mvmt = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	los = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	isSneaking = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	bluntDR = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	chopDR = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	slashDR = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	pierceDR = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	earthDR = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	fireDR = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	waterDR = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	lightningDR = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	earthWeakness = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	fireWeakness = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	waterWeakness = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	lightiningWeakness = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	dialogID = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	gold = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	thisAnimationContainer->animationsEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	thisAnimationContainer->defaultAnimation = atoi(value);

	//load each applicable animation and data
	value = strtok_r(NULL,";",&strtok_save_pointer);
	loadAnimationFromLine(thisAnimationContainer, ANIMATION_IDLE, value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	loadAnimationFromLine(thisAnimationContainer, ANIMATION_ATTACK_SLASH, value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	loadAnimationFromLine(thisAnimationContainer, ANIMATION_ATTACK_CHOP, value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	loadAnimationFromLine(thisAnimationContainer, ANIMATION_ATTACK_BLUNT, value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	loadAnimationFromLine(thisAnimationContainer, ANIMATION_ATTACK_PIERCE, value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	loadAnimationFromLine(thisAnimationContainer, ANIMATION_ATTACK_BOW, value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	loadAnimationFromLine(thisAnimationContainer, ANIMATION_HARM, value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	loadAnimationFromLine(thisAnimationContainer, ANIMATION_DEATH, value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	loadAnimationFromLine(thisAnimationContainer, ANIMATION_CAST, value);

	secondaryAnimationContainer = cloneAnimationContainer(thisAnimationContainer);

	dialogID = loadOrAddIndividualDialog(ID,dialogID);
	if(defineIndividual(newIndividual,imageID,ID,RGB(r,g,b),name,direction,x,y,STR,DEX,CON,WILL,INT,WIS,CHR,LUCK,baseHP,totalActions,baseMana,ac,attack,maxDam,minDam,baseDam,critType,range,mvmt,los,isSneaking,
			bluntDR,chopDR,slashDR,pierceDR,earthDR,fireDR,waterDR,lightningDR,earthWeakness,fireWeakness,waterWeakness,lightiningWeakness, dialogID, gold, thisAnimationContainer, secondaryAnimationContainer)){
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

void drawIndividualGroup(HDC hdc, HDC hdcBuffer, individualGroup * thisGroup, shiftData * viewShift){
	int index;

	for(index = 0; index < thisGroup->MAX_INDIVIDUALS; index++){
		int individualsPassed = 0;
		individual * tmp = thisGroup->individuals[index];

		if(tmp != NULL){
			individualsPassed++;
			drawIndividual(hdc, hdcBuffer, thisGroup->individuals[index], viewShift);

			if(individualsPassed == thisGroup->numIndividuals){
				break;
			}
		}
	}
}

void clearGroup(individualGroup * thisGroup){
	int i;
	for(i = 0; i < thisGroup->MAX_INDIVIDUALS; i++){
		thisGroup->individuals[i] = NULL;
	}
	thisGroup->numIndividuals = 0;
}

void setGroupToField(field * thisField, individualGroup * thisGroup){
	individual * tmpEnemy;
	int i, individualsPassed = 0;
	for(i = 0; i < thisGroup->MAX_INDIVIDUALS; i++){
		tmpEnemy = thisGroup->individuals[i];
		if(tmpEnemy != NULL){
			individualsPassed++;
			moveIndividualSpace(thisField,tmpEnemy,tmpEnemy->playerCharacter->x, tmpEnemy->playerCharacter->y);

			if(individualsPassed == thisGroup->numIndividuals){
				break;
			}
		}

	}
}

item * createFieldItemFromFile(char line[1024]){
	item * newItem;
	char name[32], description[256];
	char type, weaponDamType, armorClass, itemType;
	char * strtok_save_pointer;
	int npcID, imageID, ID, price, r, g, b, x, y, totalHealthMod, healthMod, totalManaMod, manaMod, acMod, attackMod, damMod,
	maxDamMod, minDamMod, minTurns, maxTurns, mvmtMod, rangeMod, bluntDRMod, chopDRMod, slashDRMod,
	pierceDRMod, earthDRMod, fireDRMod, waterDRMod, lightningDRMod, earthWeaknessMod,
	fireWeaknessMod, waterWeaknessMod, lightiningWeaknessMod, isEquipt, strMod, dexMod, conMod, willMod, intMod, wisMod, chrMod, luckMod;
	double weaponStrMod;
	animationContainer * thisAnimationContainer = initAnimationContainer();
	animationContainer * secondaryAnimationContainer = NULL;

	char * value = strtok_r(line,";",&strtok_save_pointer);
	npcID = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	imageID = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	ID = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	type = value[0];

	value = strtok_r(NULL,";",&strtok_save_pointer);
	weaponDamType = value[0];

	value = strtok_r(NULL,";",&strtok_save_pointer);
	armorClass = value[0];

	value = strtok_r(NULL,";",&strtok_save_pointer);
	itemType = value[0];

	value = strtok_r(NULL,";",&strtok_save_pointer);
	price = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	r = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	g = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	b = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	strcpy(name, value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	x = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	y = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	weaponStrMod = atof(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	strMod = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	dexMod = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	conMod = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	willMod = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	intMod = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	wisMod = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	chrMod= atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	luckMod = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	totalHealthMod = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	healthMod = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	totalManaMod = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	manaMod = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	acMod = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	attackMod = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	damMod = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	maxDamMod = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	minDamMod = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	minTurns = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	maxTurns = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	mvmtMod = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	rangeMod = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	bluntDRMod = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	chopDRMod = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	slashDRMod = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	pierceDRMod = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	earthDRMod = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	fireDRMod = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	waterDRMod = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	lightningDRMod = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	earthWeaknessMod = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	fireWeaknessMod = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	waterWeaknessMod = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	lightiningWeaknessMod = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	isEquipt = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	thisAnimationContainer->animationsEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	thisAnimationContainer->defaultAnimation = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	loadAnimationFromLine(thisAnimationContainer, ANIMATION_IDLE, value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	loadAnimationFromLine(thisAnimationContainer, ANIMATION_IDLE_EQUIPT, value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	loadAnimationFromLine(thisAnimationContainer, ANIMATION_ATTACK_SLASH, value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	loadAnimationFromLine(thisAnimationContainer, ANIMATION_ATTACK_CHOP, value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	loadAnimationFromLine(thisAnimationContainer, ANIMATION_ATTACK_BLUNT, value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	loadAnimationFromLine(thisAnimationContainer, ANIMATION_ATTACK_PIERCE, value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	loadAnimationFromLine(thisAnimationContainer, ANIMATION_ATTACK_BOW, value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	loadAnimationFromLine(thisAnimationContainer, ANIMATION_HARM, value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	loadAnimationFromLine(thisAnimationContainer, ANIMATION_DEATH, value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	loadAnimationFromLine(thisAnimationContainer, ANIMATION_CAST, value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	loadAnimationFromLine(thisAnimationContainer, ANIMATION_CONSUME, value);

	secondaryAnimationContainer = cloneAnimationContainer(thisAnimationContainer);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	strcpy(description, value);

	newItem = createItem(npcID, imageID, RGB(r,g,b),x,y, ID, type, name, description,weaponStrMod,
			strMod, dexMod, conMod, willMod, intMod, wisMod, chrMod, luckMod,
			weaponDamType, armorClass, itemType, price,
			totalHealthMod,healthMod,totalManaMod,manaMod,acMod,attackMod,damMod,maxDamMod,minDamMod, minTurns, maxTurns,
			mvmtMod,rangeMod,bluntDRMod,chopDRMod,slashDRMod,pierceDRMod,earthDRMod,
			fireDRMod,waterDRMod,lightningDRMod,earthWeaknessMod,fireWeaknessMod,
			waterWeaknessMod, lightiningWeaknessMod, isEquipt, thisAnimationContainer, secondaryAnimationContainer);

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

int individualInGroup(individual * thisIndividual, individualGroup * thisIndividualGroup){
	int i, individualsPassed = 0;
	for(i = 0; i < thisIndividualGroup->MAX_INDIVIDUALS; i++){
		if(thisIndividualGroup->individuals[i] != NULL){
			individualsPassed++;
			if(thisIndividualGroup->individuals[i] == thisIndividual){
				return 1;
			}

			if(individualsPassed == thisIndividualGroup->numIndividuals){
				break;
			}
		}
	}

	return 0;
}

individual *  deleteIndividiaulFromGroup(individualGroup * thisGroup, individual * thisIndividual){
	int index, individualsPassed = 0;

	for( index = 0; index < thisGroup->MAX_INDIVIDUALS; index++){

		if(thisGroup->individuals[index] != NULL){
			individualsPassed++;

			if (thisGroup->individuals[index] == thisIndividual) {
				individual * toReturn = thisGroup->individuals[index];
				thisGroup->individuals[index] = NULL;
				thisGroup->numIndividuals--;
				return toReturn;
			}

			if (individualsPassed == thisGroup->numIndividuals) {
				break;
			}
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

void tryTalkGroups(individualGroup * enemies, individualGroup * npcs, individual * player, int cursorX, int cursorY){
	if(!tryTalk(npcs,player,getCursorX(),getCursorY()) && !tryTalk(enemies,player,getCursorX(),getCursorY())){
		cwrite("There's nobody there.");
	}
}

int tryTalk(individualGroup * thisGroup, individual * thisIndividual, int cursorX, int cursorY){
	int index, individualsPassed = 0;
	for (index = 0; index < thisGroup->MAX_INDIVIDUALS; index++) {

		individual * tmpIndividual = thisGroup->individuals[index];

		if (tmpIndividual != NULL) {
			individualsPassed++;

			if (tmpIndividual->playerCharacter->x == cursorX && tmpIndividual->playerCharacter->y == cursorY && individualWithinRange(thisIndividual, tmpIndividual)) {
				if (setCurrentMessageByIndividualID(tmpIndividual->ID)) {
					setSpeakingIndividualID(tmpIndividual->ID);
					toggleDrawDialogBox();
				}
				return 1;
			}

			if (individualsPassed == thisGroup->numIndividuals){
				break;
			}
		}
	}
	return 0;
}
