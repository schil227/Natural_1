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
	int ID,r,g,b,direction,x,y,baseHP,totalActions,baseMana,ac,attack,maxDam,minDam,range,mvmt,los,isSneaking,
	bluntDR,chopDR,slashDR,pierceDR,earthDR,fireDR,waterDR,lightningDR,earthWeakness,
	fireWeakness,waterWeakness,lightiningWeakness,dialogID,gold,STR,DEX,CON,WILL,INT,WIS,CHR,LUCK,baseDam,faction;
	int offensiveness, abilityAffinity, tacticalness, cowardness;
	char * name = malloc(sizeof(char) * 32);
	char * strtok_save_pointer;
	char critType[4];
	animationContainer * thisAnimationContainer = initAnimationContainer();
	animationContainer * secondaryAnimationContainer = NULL;
	abilityList loadedAbilities;
	loadedAbilities.MAX_ABILITIES = 64;
	loadedAbilities.numAbilities = 0;

	char * value = strtok_r(line,";",&strtok_save_pointer);
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
	faction = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	offensiveness = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	abilityAffinity = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	tacticalness = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	cowardness = atoi(value);

	//Load abilities from given type
	value = strtok_r(NULL,";",&strtok_save_pointer);
	loadAbilitiesToList(&loadedAbilities, value, 0);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	loadAbilitiesToList(&loadedAbilities, value, 1);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	loadAbilitiesToList(&loadedAbilities, value, 2);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	loadAbilitiesToList(&loadedAbilities, value, 3);

	//load animation metadata
	value = strtok_r(NULL,";",&strtok_save_pointer);
	thisAnimationContainer->animationsEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	thisAnimationContainer->defaultAnimation = atoi(value);

	//load each applicable animation and data
	value = strtok_r(NULL,";",&strtok_save_pointer);
	addAnimationToContainer(thisAnimationContainer, cloneAnimationFromRegistry(atoi(value)));
	value = strtok_r(NULL,";",&strtok_save_pointer);
	addAnimationToContainer(thisAnimationContainer, cloneAnimationFromRegistry(atoi(value)));
	value = strtok_r(NULL,";",&strtok_save_pointer);
	addAnimationToContainer(thisAnimationContainer, cloneAnimationFromRegistry(atoi(value)));
	value = strtok_r(NULL,";",&strtok_save_pointer);
	addAnimationToContainer(thisAnimationContainer, cloneAnimationFromRegistry(atoi(value)));
	value = strtok_r(NULL,";",&strtok_save_pointer);
	addAnimationToContainer(thisAnimationContainer, cloneAnimationFromRegistry(atoi(value)));
	value = strtok_r(NULL,";",&strtok_save_pointer);
	addAnimationToContainer(thisAnimationContainer, cloneAnimationFromRegistry(atoi(value)));
	value = strtok_r(NULL,";",&strtok_save_pointer);
	addAnimationToContainer(thisAnimationContainer, cloneAnimationFromRegistry(atoi(value)));
	value = strtok_r(NULL,";",&strtok_save_pointer);
	addAnimationToContainer(thisAnimationContainer, cloneAnimationFromRegistry(atoi(value)));
	value = strtok_r(NULL,";",&strtok_save_pointer);
	addAnimationToContainer(thisAnimationContainer, cloneAnimationFromRegistry(atoi(value)));

	secondaryAnimationContainer = cloneAnimationContainer(thisAnimationContainer);

	dialogID = loadOrAddIndividualDialog(ID,dialogID);
	if(defineIndividual(newIndividual,ID,0,RGB(r,g,b),name,direction,x,y,STR,DEX,CON,WILL,INT,WIS,CHR,LUCK,baseHP,totalActions,baseMana,ac,attack,maxDam,minDam,baseDam,critType,range,mvmt,los,isSneaking,
			bluntDR,chopDR,slashDR,pierceDR,earthDR,fireDR,waterDR,lightningDR,earthWeakness,fireWeakness,waterWeakness,lightiningWeakness, dialogID, gold, faction, offensiveness, abilityAffinity, tacticalness, cowardness,
			&loadedAbilities, thisAnimationContainer, secondaryAnimationContainer)){
		printf("failed making new individual\n");
	}

	free(name);
}

void loadAbilitiesToList(abilityList * abilities, char * line, int abilityTemplateType){
	int i, intVal;
	char * value = NULL;

	value = strtok(line, ",");
	intVal = atoi(value);

	if(intVal == -1){
		return;
	}

	for(i = 0; i < intVal; i++){
		value = strtok(NULL, ",");
		ability * tmpAbility = getAbilityFromRegistryFromType(atoi(value), abilityTemplateType);

		if(tmpAbility != NULL){
			if(abilities->numAbilities < abilities->MAX_ABILITIES){
				abilities->abilitiesList[abilities->numAbilities] = tmpAbility;
				abilities->numAbilities++;
			}else{
				cwrite("!! CANNOT ADD MORE ABILITIES TO INDIVIDUAL !!");
			}
		}
	}
}

void loadGroup(individualGroup * group, groupType thisGroupType, char * fileName, char* directory){
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
					newIndividual->currentGroupType = thisGroupType;
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

groupContainer * initGroupContainer(individualGroup * enemies, individualGroup* npcs, individualGroup* allies, individualGroup* beasts, individualGroup* guards){
	groupContainer * thisGroupContainer = malloc(sizeof(groupContainer));
	if(enemies == NULL){
		thisGroupContainer->enemies = initGroup();
	}else{
		thisGroupContainer->enemies = enemies;
	}

	if(npcs == NULL){
		thisGroupContainer->npcs = initGroup();
	}else{
		thisGroupContainer->npcs = npcs;
	}

	if(allies == NULL){
		thisGroupContainer->allies = initGroup();
	}else{
		thisGroupContainer->allies = allies;
	}

	if(beasts == NULL){
		thisGroupContainer->beasts = initGroup();
	}else{
		thisGroupContainer->beasts = beasts;
	}

	if(guards == NULL){
		thisGroupContainer->guards = initGroup();
	}else{
		thisGroupContainer->guards = guards;
	}

	thisGroupContainer->groupActionMode = 0;
	thisGroupContainer->initGroupActionMode = 0;
	thisGroupContainer->groupMoveMode = 0;
	thisGroupContainer->postGroupActionMode = 0;

	thisGroupContainer->selectedGroup = NULL;
	thisGroupContainer->activeGroup = GROUP_NULL;

	return thisGroupContainer;
}

individualGroup * getGroupFromIndividual(groupContainer * thisGroupContainer, individual * thisIndividual){

	if(thisIndividual == NULL){
		cwrite("!! NULL INDIVIDUAL: CANNOT OBTAIN GROUP FROM INDIVIDUAL !!");
		return NULL;
	}

	switch(thisIndividual->currentGroupType){
	case GROUP_ALLIES:
		return thisGroupContainer->allies;
	case GROUP_BEASTS:
		return thisGroupContainer->beasts;
	case GROUP_ENEMIES:
		return thisGroupContainer->enemies;
	case GROUP_GUARDS:
		return thisGroupContainer->guards;
	case GROUP_NPCS:
		return thisGroupContainer->npcs;
	default:
		cwrite("!! INDIVIDUAL GROUP TYPE NOT FOUND !! ");
		return NULL;
	}

}

int setNextActiveGroup(groupContainer * thisGroupContainer){
	switch(thisGroupContainer->activeGroup){
	case GROUP_ALLIES:
		thisGroupContainer->activeGroup = GROUP_ENEMIES;
		thisGroupContainer->selectedGroup = thisGroupContainer->enemies;
		return 1;
	case GROUP_BEASTS:
		thisGroupContainer->activeGroup = GROUP_GUARDS;
		thisGroupContainer->selectedGroup = thisGroupContainer->guards;
		return 1;
	case GROUP_ENEMIES:
		thisGroupContainer->activeGroup = GROUP_BEASTS;
		thisGroupContainer->selectedGroup = thisGroupContainer->beasts;
		return 1;
	case GROUP_GUARDS:
		thisGroupContainer->activeGroup = GROUP_NPCS;
		thisGroupContainer->selectedGroup = thisGroupContainer->npcs;
		return 1;
	case GROUP_NPCS:
		thisGroupContainer->selectedGroup = NULL;
		thisGroupContainer->activeGroup = GROUP_NULL;
		return 0;
	default: //NULL, player just ended their turn
		thisGroupContainer->activeGroup = GROUP_ALLIES;
		thisGroupContainer->selectedGroup = thisGroupContainer->allies;
		return 1;
	}
}


void decreaseTurns(individual * thisIndividual, groupContainer * thisGroupContainer, int numTurns){

	if(thisIndividual->activeAbilities->selectedAbility != NULL && thisIndividual->activeAbilities->selectedAbility->type == 'i'){

		if(thisIndividual->activeAbilities->selectedAbility->actionsEnabled){
			numTurns += thisIndividual->activeAbilities->selectedAbility->actions->effectAndManaArray[thisIndividual->activeAbilities->selectedAbility->actions->selectedIndex]->effectMagnitude;
		 }

		removeActiveAbility(thisIndividual, thisIndividual->activeAbilities->selectedAbility);

		thisIndividual->activeAbilities->selectedAbility = NULL;
	}

	thisIndividual->remainingActions -= numTurns;

	if (thisIndividual->remainingActions <= 0) {
		endTurn(thisIndividual);
		thisGroupContainer->groupActionMode = 1;
		thisGroupContainer->initGroupActionMode = 1;
		setNextActiveGroup(thisGroupContainer);
	}
}

void drawGroups(HDC hdc, HDC hdcBuffer, groupContainer * thisGroupContainer, shiftData * viewShift){
	drawIndividualGroup(hdc, hdcBuffer, thisGroupContainer->npcs, viewShift);
	drawIndividualGroup(hdc, hdcBuffer, thisGroupContainer->enemies, viewShift);
	drawIndividualGroup(hdc, hdcBuffer, thisGroupContainer->guards, viewShift);
	drawIndividualGroup(hdc, hdcBuffer, thisGroupContainer->beasts, viewShift);

	//draw animated enemy/npc over others
	if(thisGroupContainer->enemies->currentIndividualIndex != -1){
		drawIndividual(hdc, hdcBuffer, thisGroupContainer->enemies->individuals[thisGroupContainer->enemies->currentIndividualIndex], viewShift);
	}

	if(thisGroupContainer->npcs->currentIndividualIndex != -1){
		drawIndividual(hdc, hdcBuffer, thisGroupContainer->npcs->individuals[thisGroupContainer->npcs->currentIndividualIndex], viewShift);
	}

	if(thisGroupContainer->beasts->currentIndividualIndex != -1){
		drawIndividual(hdc, hdcBuffer, thisGroupContainer->beasts->individuals[thisGroupContainer->beasts->currentIndividualIndex], viewShift);
	}

	if(thisGroupContainer->guards->currentIndividualIndex != -1){
		drawIndividual(hdc, hdcBuffer, thisGroupContainer->guards->individuals[thisGroupContainer->guards->currentIndividualIndex], viewShift);
	}
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
	int npcID, ID, price, r, g, b, x, y, totalHealthMod, healthMod, totalManaMod, manaMod, acMod, attackMod, damMod,
	maxDamMod, minDamMod, minTurns, maxTurns, mvmtMod, rangeMod, bluntDRMod, chopDRMod, slashDRMod,
	pierceDRMod, earthDRMod, fireDRMod, waterDRMod, lightningDRMod, earthWeaknessMod,
	fireWeaknessMod, waterWeaknessMod, lightiningWeaknessMod, isEquipt, strMod, dexMod, conMod, willMod, intMod, wisMod, chrMod, luckMod;
	double weaponStrMod;
	animationContainer * thisAnimationContainer = initAnimationContainer();
	animationContainer * secondaryAnimationContainer = NULL;

	char * value = strtok_r(line,";",&strtok_save_pointer);
	npcID = atoi(value);

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
	addAnimationToContainer(thisAnimationContainer, cloneAnimationFromRegistry(atoi(value)));
	value = strtok_r(NULL,";",&strtok_save_pointer);
	addAnimationToContainer(thisAnimationContainer, cloneAnimationFromRegistry(atoi(value)));
	value = strtok_r(NULL,";",&strtok_save_pointer);
	addAnimationToContainer(thisAnimationContainer, cloneAnimationFromRegistry(atoi(value)));
	value = strtok_r(NULL,";",&strtok_save_pointer);
	addAnimationToContainer(thisAnimationContainer, cloneAnimationFromRegistry(atoi(value)));
	value = strtok_r(NULL,";",&strtok_save_pointer);
	addAnimationToContainer(thisAnimationContainer, cloneAnimationFromRegistry(atoi(value)));
	value = strtok_r(NULL,";",&strtok_save_pointer);
	addAnimationToContainer(thisAnimationContainer, cloneAnimationFromRegistry(atoi(value)));
	value = strtok_r(NULL,";",&strtok_save_pointer);
	addAnimationToContainer(thisAnimationContainer, cloneAnimationFromRegistry(atoi(value)));
	value = strtok_r(NULL,";",&strtok_save_pointer);
	addAnimationToContainer(thisAnimationContainer, cloneAnimationFromRegistry(atoi(value)));
	value = strtok_r(NULL,";",&strtok_save_pointer);
	addAnimationToContainer(thisAnimationContainer, cloneAnimationFromRegistry(atoi(value)));
	value = strtok_r(NULL,";",&strtok_save_pointer);
	addAnimationToContainer(thisAnimationContainer, cloneAnimationFromRegistry(atoi(value)));
	value = strtok_r(NULL,";",&strtok_save_pointer);
	addAnimationToContainer(thisAnimationContainer, cloneAnimationFromRegistry(atoi(value)));

	thisAnimationContainer->currentAnimation = thisAnimationContainer->defaultAnimation;

	secondaryAnimationContainer = cloneAnimationContainer(thisAnimationContainer);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	strcpy(description, value);

	newItem = createItem(npcID, RGB(r,g,b),x,y, ID, type, name, description,weaponStrMod,
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

	if(thisGroup == NULL){
		return NULL;
	}

	for(index = 0; index < thisGroup->MAX_INDIVIDUALS; index++){

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

int attemptToTransit(field ** thisField, individual * player, groupContainer * thisGroupContainer, shiftData * viewShift, char * mapDirectory){
	space * tmpSpace = (*thisField)->grid[player->playerCharacter->x][player->playerCharacter->y];

		if(tmpSpace->thisTransitInfo != NULL && tmpSpace->thisTransitInfo->targetMapTransitID != 0){
//			int x, y, imageID;
			char mapName[256];
			strcpy(mapName, tmpSpace->thisTransitInfo->transitMap);
			player->jumpTarget = tmpSpace->thisTransitInfo->targetMapTransitID;

			destroyField(*thisField, player);
			clearGroup(thisGroupContainer->enemies);
			clearGroup(thisGroupContainer->npcs);
			clearGroup(thisGroupContainer->beasts);
			clearGroup(thisGroupContainer->guards);

			*thisField = loadMap(mapName, mapDirectory, player, thisGroupContainer);

//			viewShift->xShift = 0;
//			viewShift->yShift = 0;
//			//load images for new map
//			for (y = 0; y < (*thisField)->totalY; y++) {
//				for (x = 0; x < (*thisField)->totalX; x++) {
//					imageID = ((*thisField)->grid[x][y]->background)->imageID;
//					(*thisField)->grid[x][y]->background->image = LoadBitmap(GetModuleHandle(NULL), imageID);
//					if ((*thisField)->grid[x][y]->background->image == NULL) {
//						printf("failed\n");
//					}
//				}
//			}

			clearMessages();
			if(reportActiveCrimes(player)){
				cwrite("Your crimes have been reported by witnesses!");

			}
			return 1;
		}
	return 0;
}

void tryTalkGroups(groupContainer * thisGroupContainer, individual * player, int cursorX, int cursorY){
	if(!tryTalk(thisGroupContainer->npcs,player,getCursorX(),getCursorY()) &&
			!tryTalk(thisGroupContainer->beasts,player,getCursorX(),getCursorY()) &&
			!tryTalk(thisGroupContainer->guards,player,getCursorX(),getCursorY()) &&
			!tryTalk(thisGroupContainer->allies,player,getCursorX(),getCursorY()) &&
			!tryTalk(thisGroupContainer->enemies,player,getCursorX(),getCursorY())){
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
