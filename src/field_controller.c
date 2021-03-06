/*
 * field_controller.c
 *
 *  Created on: Jul 23, 2015
 *      Author: Adrian
 */

#include "./headers/field_controller_pub_methods.h"
#include<stdio.h>

static windowTransition * thisWindowTransition;

void initWindowTransition(){
	thisWindowTransition = malloc(sizeof(windowTransition));

	thisWindowTransition->inWindowTransition = 0;
	thisWindowTransition->readyToTransitField = 0;
	thisWindowTransition->selectedTransition = TRANSITION_MEDIUM;
	thisWindowTransition->fadingOut = 1;
	thisWindowTransition->greyscaleLevel = 0;
	thisWindowTransition->fieldUnloaded = 0;
	thisWindowTransition->blackCharacter = createCharacter(7500, RGB(255,0,255), 0, 0);
}

void enableWindowTransition(int fadingOut, int fieldUnloaded, windowTransitionSpeed speed, int areaNodeID, char * mapDirectory, char * mapName){
	thisWindowTransition->greyscaleLevel = 0;
	thisWindowTransition->fadingOut = fadingOut;
	thisWindowTransition->fieldUnloaded = fieldUnloaded;
	thisWindowTransition->inWindowTransition = 1;
	thisWindowTransition->areaNodeId = areaNodeID;

	strcpy(thisWindowTransition->mapDirectory, mapDirectory);
	strcpy(thisWindowTransition->mapName, mapName);

	if(fadingOut){
		thisWindowTransition->selectedTransition = speed;
	}
}

void disableWindowTransition(windowTransitionSpeed speed){
	thisWindowTransition->inWindowTransition = 0;
}

int inWindowTransitionMode(){
	if(thisWindowTransition != NULL){
		return thisWindowTransition->inWindowTransition;
	}

	return 0;
}

int readyToTransit(){
	if(thisWindowTransition != NULL){
		return thisWindowTransition->readyToTransitField;
	}

	return 0;
}

int getWindowTransitionGreyscaleIncrement(){
	switch(thisWindowTransition->selectedTransition){
		case TRANSITION_SLOW:
			return 6;
		case TRANSITION_MEDIUM:
			return 12;
		case TRANSITION_FAST:
			return 18;
	}

	return 7;
}

void drawWindowTransition(HDC hdc, HDC hdcBuffer, RECT * rect){
	if(thisWindowTransition->fadingOut){
		thisWindowTransition->greyscaleLevel += getWindowTransitionGreyscaleIncrement();
	}else{
		thisWindowTransition->greyscaleLevel -= getWindowTransitionGreyscaleIncrement();
	}

	if(thisWindowTransition->greyscaleLevel < 0){
		thisWindowTransition->greyscaleLevel = 0;
	}else if(thisWindowTransition->greyscaleLevel > 255){
		thisWindowTransition->greyscaleLevel = 255;
	}

	if(thisWindowTransition->greyscaleLevel == 0){
		printf("disabling window transition");fflush(stdin);
		thisWindowTransition->inWindowTransition = 0;
		return;
	}else if(thisWindowTransition->greyscaleLevel == 255 && thisWindowTransition->fadingOut){
		printf("ready to draw");fflush(stdin);
		thisWindowTransition->readyToTransitField = 1;
		thisWindowTransition->fadingOut = 0;
	}

	drawFadeOutInAnimation(hdc, hdcBuffer, rect, thisWindowTransition->blackCharacter, thisWindowTransition->greyscaleLevel);
}







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
	while(!tryGetIndividualGroupReadLock()){}
	while(!tryGetIndividualGroupWriteLock()){}

	for(i = 0; i < thisGroup->MAX_INDIVIDUALS; i++){
		if(thisGroup->individuals[i] == NULL){
			thisGroup->individuals[i] = thisIndividual;
			thisGroup->numIndividuals++;

			releaseIndividualGroupWriteLock();
			releaseIndividualGroupReadLock();
			return 1;
		}
	}

	releaseIndividualGroupWriteLock();
	releaseIndividualGroupReadLock();
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
	int ID,isPlayer,r,g,b,direction,level,exp,spread,x,y,baseHP,totalActions,baseMana,ac,attack,maxDam,minDam,range,mvmt,los,darkLos,isSneaking,
	bluntDR,chopDR,slashDR,pierceDR,earthDR,fireDR,waterDR,lightningDR,dialogID,dialogPortraitID,fateTokens,bonusMana,gold,
	STR,DEX,CON,WILL,INT,WIS,CHR,LUCK,baseDam,faction,hp,mana,food;
	int offensiveness, abilityAffinity, tacticalness, cowardness, isHostileToPlayer, isFocusedOnPlayer, isSurrounded, respawns, desiredLocationX, desiredLocationY;
	char * name = malloc(sizeof(char) * 32);
	char * strtok_save_pointer;
	char critType[4];
	specialDialogs * thisDialog = malloc(sizeof(specialDialogs));
	thisDialog->activeDialog = DIALOG_DEFAULT;
	animationContainer * thisAnimationContainer = initAnimationContainer();
	animationContainer * secondaryAnimationContainer = NULL;
	abilityList loadedAbilities;
	loadedAbilities.MAX_ABILITIES = 64;
	loadedAbilities.numAbilities = 0;
	groupType defaultType = 0;
	groupType currentType = 0;

	char * value = strtok_r(line,";",&strtok_save_pointer);
	ID = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	isPlayer = atoi(value);

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
	level = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	exp = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	spread = atoi(value);

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
	hp = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	mana = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	food = atoi(value);

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
	darkLos = atoi(value);
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
	dialogID = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	thisDialog->sawPlayerCrime = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	thisDialog->attackedByPlayer = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	thisDialog->stolenFromByPlayer = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	thisDialog->afraidOfPlayer = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	thisDialog->playerIsMarkedForDeath = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	dialogPortraitID = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	fateTokens = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	bonusMana = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	gold = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	faction = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	defaultType = (groupType)(atoi(value));

	value = strtok_r(NULL,";",&strtok_save_pointer);
	currentType = (groupType)(atoi(value));

	value = strtok_r(NULL,";",&strtok_save_pointer);
	offensiveness = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	abilityAffinity = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	tacticalness = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	cowardness = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	isHostileToPlayer = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	isFocusedOnPlayer = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	isSurrounded = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	respawns = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	desiredLocationX = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	desiredLocationY = atoi(value);

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

	dialogID = loadOrAddIndividualDialog(ID, dialogID, 0);
	if(defineIndividual(newIndividual,ID,isPlayer,RGB(r,g,b),name,direction,x,y,level,exp,spread,STR,DEX,CON,WILL,INT,WIS,CHR,LUCK,hp, mana, food, baseHP,totalActions,baseMana,ac,attack,maxDam,minDam,baseDam,critType,range,mvmt,los,darkLos,isSneaking,
			bluntDR,chopDR,slashDR,pierceDR,earthDR,fireDR,waterDR,lightningDR, dialogID, dialogPortraitID, fateTokens, bonusMana, gold, faction, defaultType, currentType, offensiveness, abilityAffinity, tacticalness, cowardness,
			isHostileToPlayer, isFocusedOnPlayer, isSurrounded, respawns, desiredLocationX, desiredLocationY, thisDialog, &loadedAbilities, thisAnimationContainer, secondaryAnimationContainer)){
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

void loadGroups(groupContainer * thisGroupContainer, mapInfo * thisMap, field * thisField){
	int i;

	individual * tmpIndividual;

	//thisMap->individuals is self balancing
	for(i = 0; i < thisMap->numIndividuals; i++){
		tmpIndividual = getIndividualFromRegistry(thisMap->individuals[i]);

		if(tmpIndividual == NULL || !doesExist(tmpIndividual->ID) || tmpIndividual->hp <= 0){
			continue;
		}

		//in the case two individuals occoupy the same space due to respawning, yield to the non-respawning individual
		if(getIndividualFromField(thisField, tmpIndividual->playerCharacter->x, tmpIndividual->playerCharacter->y) != NULL){
			if(tmpIndividual->thisBehavior->respawns){
				//don't add tmpIndividual
				continue;
			}else{
				//remove existing individual
				removeIndividualFromField(thisField, tmpIndividual->playerCharacter->x, tmpIndividual->playerCharacter->y);
			}
		}

		moveIndividualSpace(thisField,tmpIndividual,tmpIndividual->playerCharacter->x, tmpIndividual->playerCharacter->y);

		switch(tmpIndividual->currentGroupType){
		case GROUP_ENEMIES:
			addIndividualToGroup(thisGroupContainer->enemies, tmpIndividual);
			break;
		case GROUP_NPCS:
			addIndividualToGroup(thisGroupContainer->npcs, tmpIndividual);
					break;
		case GROUP_GUARDS:
			addIndividualToGroup(thisGroupContainer->guards, tmpIndividual);
					break;
		case GROUP_BEASTS:
			addIndividualToGroup(thisGroupContainer->beasts, tmpIndividual);
					break;
		case GROUP_ALLIES:
			addIndividualToGroup(thisGroupContainer->allies, tmpIndividual);
					break;
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
				if (doesExist(newIndividual->ID) && newIndividual->currentGroupType == thisGroupType){
					newIndividual->currentGroupType = thisGroupType;
					newIndividual->defaultGroupType = thisGroupType;
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

	thisGroupContainer->movingIndividuals = initGroup();

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

individualGroup * getDefaultGroupFromIndividual(groupContainer * thisGroupContainer, individual * thisIndividual){
	if(thisIndividual == NULL){
		cwrite("!! NULL INDIVIDUAL: CANNOT OBTAIN GROUP FROM INDIVIDUAL !!");
		return NULL;
	}

	switch(thisIndividual->defaultGroupType){
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

void decreaseTurns(individual * thisIndividual, groupContainer * thisGroupContainer, int numTurns, int inActionMode){

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

		if(inActionMode && thisIndividual->isPlayer){
			thisGroupContainer->groupActionMode = 1;
			thisGroupContainer->initGroupActionMode = 1;
			setNextActiveGroup(thisGroupContainer);
		}
	}
}

void drawGroups(HDC hdc, HDC hdcBuffer, groupContainer * thisGroupContainer, field * thisField, shiftData * viewShift){
	while(!tryGetIndividualGroupReadLock()){}
	drawIndividualGroup(hdc, hdcBuffer, thisGroupContainer->npcs, thisField, viewShift);
	drawIndividualGroup(hdc, hdcBuffer, thisGroupContainer->enemies, thisField, viewShift);
	drawIndividualGroup(hdc, hdcBuffer, thisGroupContainer->guards, thisField, viewShift);
	drawIndividualGroup(hdc, hdcBuffer, thisGroupContainer->beasts, thisField, viewShift);
	drawIndividualGroup(hdc, hdcBuffer, thisGroupContainer->allies, thisField, viewShift);

	//draw animated individual over others
	individual * tmp;

	if(thisGroupContainer->enemies->currentIndividualIndex != -1){
		tmp = thisGroupContainer->enemies->individuals[thisGroupContainer->enemies->currentIndividualIndex];

		if(!thisField->isDark || thisField->playerLoS >= max(abs(thisField->playerCords->x - tmp->playerCharacter->x), abs(thisField->playerCords->y - tmp->playerCharacter->y))){
			drawIndividual(hdc, hdcBuffer, tmp, viewShift);
		}
	}

	if(thisGroupContainer->npcs->currentIndividualIndex != -1){
		tmp = thisGroupContainer->npcs->individuals[thisGroupContainer->npcs->currentIndividualIndex];

		if(!thisField->isDark || thisField->playerLoS >= max(abs(thisField->playerCords->x - tmp->playerCharacter->x), abs(thisField->playerCords->y - tmp->playerCharacter->y))){
			drawIndividual(hdc, hdcBuffer, tmp, viewShift);
		}
	}

	if(thisGroupContainer->beasts->currentIndividualIndex != -1){
		tmp = thisGroupContainer->beasts->individuals[thisGroupContainer->beasts->currentIndividualIndex];

		if(!thisField->isDark || thisField->playerLoS >= max(abs(thisField->playerCords->x - tmp->playerCharacter->x), abs(thisField->playerCords->y - tmp->playerCharacter->y))){
			drawIndividual(hdc, hdcBuffer, tmp, viewShift);
		}
	}

	if(thisGroupContainer->guards->currentIndividualIndex != -1){
		tmp = thisGroupContainer->guards->individuals[thisGroupContainer->guards->currentIndividualIndex];

		if(!thisField->isDark || thisField->playerLoS >= max(abs(thisField->playerCords->x - tmp->playerCharacter->x), abs(thisField->playerCords->y - tmp->playerCharacter->y))){
			drawIndividual(hdc, hdcBuffer, tmp, viewShift);
		}
	}

	if(thisGroupContainer->allies->currentIndividualIndex != -1){
		tmp = thisGroupContainer->allies->individuals[thisGroupContainer->allies->currentIndividualIndex];

		if(!thisField->isDark || thisField->playerLoS >= max(abs(thisField->playerCords->x - tmp->playerCharacter->x), abs(thisField->playerCords->y - tmp->playerCharacter->y))){
			drawIndividual(hdc, hdcBuffer, tmp, viewShift);
		}
	}

	releaseIndividualGroupReadLock();
}

void drawIndividualGroup(HDC hdc, HDC hdcBuffer, individualGroup * thisGroup, field * thisField, shiftData * viewShift){
	int index;

	if(thisGroup->numIndividuals > 0){
		for(index = 0; index < thisGroup->MAX_INDIVIDUALS; index++){
			int individualsPassed = 0;
			individual * tmp = thisGroup->individuals[index];

			if(tmp != NULL){
				individualsPassed++;

				//dont draw the current individual yet, do it after
				if(index != thisGroup->currentIndividualIndex){
					if(!thisField->isDark || thisField->playerLoS >= max(abs(thisField->playerCords->x - tmp->playerCharacter->x), abs(thisField->playerCords->y - tmp->playerCharacter->y))){
						drawIndividual(hdc, hdcBuffer, tmp, viewShift);
					}
				}

				if(individualsPassed == thisGroup->numIndividuals){
					break;
				}
			}
		}
	}
}

void clearGroup(individualGroup * thisGroup){
	int i;

	while(!tryGetIndividualGroupReadLock()){}
	while(!tryGetIndividualGroupWriteLock()){}

	for(i = 0; i < thisGroup->MAX_INDIVIDUALS; i++){
		thisGroup->individuals[i] = NULL;
	}

	thisGroup->numIndividuals = 0;

	releaseIndividualGroupWriteLock();
	releaseIndividualGroupReadLock();
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

void respawnFieldEntities(int fieldID){
	int i;

	mapInfo * thisMapInfo = getMapInfoFromRegistry(fieldID);

	for(i = 0; i < thisMapInfo->numIndividuals; i++){
		individual * tmpIndividual = getIndividualFromRegistry(thisMapInfo->individuals[i]);

		if(tmpIndividual != NULL){
			if(tmpIndividual->thisBehavior->respawns){
				tmpIndividual->hp = getTotalHP(tmpIndividual);
				addIndividualToExistance(tmpIndividual->ID);
			}else if(tmpIndividual->hp > 0){
				tmpIndividual->hp = getTotalHP(tmpIndividual);
			}

			tmpIndividual->playerCharacter->x = tmpIndividual->desiredLocation->x;
			tmpIndividual->playerCharacter->y = tmpIndividual->desiredLocation->y;
		}
	}

	for(i = 0; i < thisMapInfo->numInteractables; i++){
		interactable * tmpInteractable = getInteractableObjectFromRegistry(thisMapInfo->interactableObjects[i]);

		if(tmpInteractable->isRespawning){
			tmpInteractable->isEnabled = 1;
			tmpInteractable->inFinalMode = 0;
			tmpInteractable->shouldDraw = 1;//?
		}
	}
}

item * createFieldItemFromFile(char line[1024]){
	item * newItem;
	char name[32], description[256];
	char type, weaponDamType, armorClass, itemType;
	char * strtok_save_pointer;
	int npcID, ID, interactableObjectID, price, owner, isStolen, r, g, b, x, y, totalHealthMod, healthMod, totalManaMod, manaMod, food, acMod, attackMod, damMod,
	maxDamMod, minDamMod, minTurns, maxTurns, mvmtMod, rangeMod, darkLoSMod, secondaryDefaultAnimation, bluntDRMod, chopDRMod, slashDRMod,
	pierceDRMod, earthDRMod, fireDRMod, waterDRMod, lightningDRMod,isEquipt, strMod, dexMod, conMod, willMod, intMod, wisMod, chrMod, luckMod;
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
	owner = atoi(value);
	value = strtok_r(NULL,";",&strtok_save_pointer);
	isStolen = atoi(value);

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
	interactableObjectID = (int) getInteractType(value);

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
	food = atoi(value);

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
	darkLoSMod = atoi(value);

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
	isEquipt = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	thisAnimationContainer->animationsEnabled = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	thisAnimationContainer->defaultAnimation = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	secondaryDefaultAnimation = atoi(value);

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

	setDefaultAnimation(secondaryAnimationContainer, (animationState) secondaryDefaultAnimation);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	strcpy(description, value);

	newItem = createItem(npcID, RGB(r,g,b),x,y, ID, type, name, description, interactableObjectID, weaponStrMod,
			strMod, dexMod, conMod, willMod, intMod, wisMod, chrMod, luckMod,
			weaponDamType, armorClass, itemType, price, owner, isStolen,
			totalHealthMod,healthMod,totalManaMod,manaMod,food, acMod,attackMod,damMod,maxDamMod,minDamMod, minTurns, maxTurns,
			mvmtMod,rangeMod,darkLoSMod,bluntDRMod,chopDRMod,slashDRMod,pierceDRMod,earthDRMod,
			fireDRMod,waterDRMod,lightningDRMod, isEquipt, thisAnimationContainer, secondaryAnimationContainer);

	return newItem;
}

void addItemsToField(mapInfo * thisMapInfo, field * thisField){
	int i;

	item * tmpItem;

	//thisMapInfo->numItems is self balancing
	for(i = 0; i < thisMapInfo->numItems; i++){
		tmpItem = getItemFromRegistry(thisMapInfo->items[i]);

		if(tmpItem != NULL){
//			if (doesExist(tmpItem->ID)) {
				addItemToField(thisField, tmpItem);
//			}
		}else{
			char * errLog[128];
			sprintf(errLog, "!! ITEM NOT FOUND : %d !!", thisMapInfo->items[i]);
		}
	}
}

void addInteractableObjectsToField(mapInfo * thisMapInfo, field * thisField){
	int i;

	interactable * tmpInteractable;

	//thisMapInfo->numItems is self balancing
	for(i = 0; i < thisMapInfo->numInteractables; i++){
		tmpInteractable = getInteractableObjectFromRegistry(thisMapInfo->interactableObjects[i]);

		if(tmpInteractable != NULL){
//			if (doesExist(tmpInteractable->ID)) {
				addInteractableObjectToField(tmpInteractable, thisField);
//			}
		}else{
			char * errLog[128];
			sprintf(errLog, "!! INTERACTABLE OBJECT NOT FOUND NOT FOUND : %d !!", thisMapInfo->interactableObjects[i]);
		}
	}
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

	while(!tryGetIndividualGroupReadLock()){}
	while(!tryGetIndividualGroupWriteLock()){}

	for(index = 0; index < thisGroup->MAX_INDIVIDUALS; index++){

		if(thisGroup->individuals[index] != NULL){
			individualsPassed++;

			if (thisGroup->individuals[index] == thisIndividual) {
				individual * toReturn = thisGroup->individuals[index];
				thisGroup->individuals[index] = NULL;
				thisGroup->numIndividuals--;

				releaseIndividualGroupWriteLock();
				releaseIndividualGroupReadLock();

				return toReturn;
			}

			if (individualsPassed == thisGroup->numIndividuals) {
				break;
			}
		}
	}

	releaseIndividualGroupWriteLock();
	releaseIndividualGroupReadLock();

	char logOut[128];
	sprintf(logOut, "!! COULD NOT FIND INDIVIDUAL TO REMOVE FROM GROUP: %s !!", thisIndividual->name);
	cwrite(logOut);

	return NULL;
}

void removeAlliesFromField(individualGroup * allies, field * thisField){
	int i, individualsPassed = 0;

	if(allies->numIndividuals == 0){
		return;
	}

	mapInfo * thisMap = getMapInfoFromRegistry(thisField->id);

	for(i = 0; i < allies->MAX_INDIVIDUALS; i++){
		individual * tmpAlly = allies->individuals[i];
		if(tmpAlly != NULL){
			individualsPassed++;

			removeIndividualFromField(thisField, tmpAlly->playerCharacter->x, tmpAlly->playerCharacter->y);
			removeIndividualIdFromMapInfo(thisMap, tmpAlly->ID);

			if(individualsPassed == allies->numIndividuals){
				break;
			}
		}
	}
}

void setAlliesToField(individual * player, individualGroup * allies, field * thisField){
	int i, individualsPassed = 0;
	individual * tmpAlly;

	if(allies->numIndividuals == 0){
		return;
	}

	mapInfo * thisMap = getMapInfoFromRegistry(thisField->id);

	for(i = 0; i < allies->MAX_INDIVIDUALS; i++){
		tmpAlly = allies->individuals[i];

		tmpAlly->playerCharacter->x = -1;
		tmpAlly->playerCharacter->y = -1;

		if(tmpAlly != NULL){
			individualsPassed++;

			cord * freeSpace = selectRandomSpaceWeightedToStart(player, thisField, 5);

			setIndividualSpace(thisField, tmpAlly, freeSpace->x, freeSpace->y);

			if(tmpAlly->playerCharacter->x != -1 && tmpAlly->playerCharacter->y != -1){
				getSpaceFromField(thisField, freeSpace->x, freeSpace->y)->currentIndividual = tmpAlly;
				tmpAlly->playerCharacter->x = freeSpace->x;
				tmpAlly->playerCharacter->y = freeSpace->y;
				addIndividualsToMapInfo(thisMap, tmpAlly->ID);
			}

			if(individualsPassed == allies->numIndividuals){
				break;
			}
		}
	}
}

int transitDuringFade(field ** thisField, individual * player, groupContainer * thisGroupContainer, shiftData * viewShift){
	thisWindowTransition->readyToTransitField = 0;

	if(thisWindowTransition->fieldUnloaded){
		areaNode * thisAreaNode = getAreaNodeFromRegistry(thisWindowTransition->areaNodeId);

		mapInfo * thisMapInfo = getMapInfoFromRegistry(thisAreaNode->mapID);
		player->jumpTarget = thisAreaNode->mapTransitID;

		*thisField = loadMap(thisMapInfo->mapName, thisWindowTransition->mapDirectory, player, thisGroupContainer);

		setAlliesToField(player, thisGroupContainer->allies, *thisField);

		disableWorldMapMode();
		return 1;
	}else{
		return transit(thisField, thisWindowTransition->areaNodeId, player, thisGroupContainer, viewShift, thisWindowTransition->mapDirectory, thisWindowTransition->mapName);
	}
}

int attemptToTransit(field ** thisField, individual * player, groupContainer * thisGroupContainer, shiftData * viewShift, char * mapDirectory, int fadeOut){
	space * tmpSpace = (*thisField)->grid[player->playerCharacter->x][player->playerCharacter->y];

		if(tmpSpace->thisTransitInfo != NULL && (tmpSpace->thisTransitInfo->targetMapTransitID != 0 || tmpSpace->thisTransitInfo->areaNodeID != -1)){
			int i;
			int areaNodeID = tmpSpace->thisTransitInfo->areaNodeID;
			char mapName[256];

			strcpy(mapName, tmpSpace->thisTransitInfo->transitMap);
			player->jumpTarget = tmpSpace->thisTransitInfo->targetMapTransitID;

			if(tmpSpace->thisTransitInfo->transitEventID != -1){
				triggerEvent(tmpSpace->thisTransitInfo->transitEventID);
			}

			if(!fadeOut){
				return transit(thisField, areaNodeID, player, thisGroupContainer, viewShift, mapDirectory, mapName);
			}else{
				enableWindowTransition(fadeOut, 0, TRANSITION_MEDIUM, areaNodeID, mapDirectory, mapName);
				return 0;
			}
		}
	return 0;
}

int attemptToForceTransit(field ** thisField, individual * player, groupContainer * thisGroupContainer, shiftData * viewShift, char * mapDirectory, int targetMapID, int transitID, int fadeOut){
	int i;
	char mapName[64];

	mapInfo * targetMap = getMapInfoFromRegistry(targetMapID);

	strcpy(mapName, targetMap->mapName);
	player->jumpTarget = transitID;

	if(!fadeOut){
		return transit(thisField, -1, player, thisGroupContainer, viewShift, mapDirectory, mapName);
	}else{
		enableWindowTransition(fadeOut, 0, TRANSITION_MEDIUM, -1, mapDirectory, mapName);
	}

	return 1;
}

int transitFromAreaNode(areaNode * thisAreaNode, field ** thisField, individual * player, groupContainer * thisGroupContainer, shiftData * viewShift, char * mapDirectory, int fadeOut){
	if(fadeOut){
		enableWindowTransition(fadeOut, 1, TRANSITION_MEDIUM, thisAreaNode->id, mapDirectory, "");
		return 0;
	}else{
		mapInfo * thisMapInfo = getMapInfoFromRegistry(thisAreaNode->mapID);
		player->jumpTarget = thisAreaNode->mapTransitID;

		printf("WANT: world transit\n");fflush(stdout);
		while(!tryGetFieldWriteLock()){}
		printf("GOT: world transit\n");fflush(stdout);

		*thisField = loadMap(thisMapInfo->mapName, mapDirectory, player, thisGroupContainer);

		releaseFieldWriteLock();
		printf("RELEASED: world transit\n");fflush(stdout);

		setAlliesToField(player, thisGroupContainer->allies, *thisField);

		return 1;
	}
}

int transit(field ** thisField, int areaNodeID, individual * player, groupContainer * thisGroupContainer, shiftData * viewShift, char * mapDirectory, char * mapName){
	int i;

	while(!tryGetConsoleReadLock()){}
	while(!tryGetConsoleWriteLock()){}

	clearMessages();

	releaseConsoleWriteLock();
	releaseConsoleReadLock();

	if(reportActiveCrimes(player)){
		cwrite("Your crimes have been reported by witnesses!");
		clearCrimeSpecialDialogForGroup(thisGroupContainer->npcs);
	}

	groupTransitUpdate(thisGroupContainer);
	removeAlliesFromField(thisGroupContainer->allies, *thisField);

	printf("WANT: destroy\n");fflush(stdout);
	while(!tryGetFieldWriteLock()){}
	printf("GOT: destroy\n");fflush(stdout);

	mapInfo * thisMapInfo = getMapInfoFromRegistry((*thisField)->id);
	thisMapInfo->isCurrentMap = 0;

	addFieldToTraversedMaps(thisMapInfo->id);

	destroyField(*thisField, player);
	clearGroup(thisGroupContainer->enemies);
	clearGroup(thisGroupContainer->npcs);
	clearGroup(thisGroupContainer->beasts);
	clearGroup(thisGroupContainer->guards);

	for(i = 0; i < thisGroupContainer->movingIndividuals->numIndividuals; i++){
		thisGroupContainer->movingIndividuals->individuals[i]->playerCharacter->xOff = 0;
		thisGroupContainer->movingIndividuals->individuals[i]->playerCharacter->yOff = 0;
		freeUpMovePath(thisGroupContainer->movingIndividuals->individuals[i]->thisMoveNodeMeta->rootMoveNode);
	}

	clearGroup(thisGroupContainer->movingIndividuals);

	if(areaNodeID != -1){
		if(player->thisReportedCrimes->numReportedCrimes > 0){
			setGroupSpecialDialog(thisGroupContainer->guards, DIALOG_CRIME_WITNESS);
		}

		enableWorldMapMode(areaNodeID);

		releaseFieldWriteLock();
		printf("RELEASED: destroy\n");fflush(stdout);
		return 0;
	}

	*thisField = loadMap(mapName, mapDirectory, player, thisGroupContainer);

	releaseFieldWriteLock();
	printf("RELEASED: destroy\n");fflush(stdout);

	if(player->thisReportedCrimes->numReportedCrimes > 0){
		setGroupSpecialDialog(thisGroupContainer->guards, DIALOG_CRIME_WITNESS);
	}

	setAlliesToField(player, thisGroupContainer->allies, *thisField);

	return 1;
}

int tryPickPocketIndividualFromField(individual * player, field * thisField, int cursorX, int cursorY){
	individual * targetIndividual = getIndividualFromField(thisField, cursorX, cursorY);
	int isCrime = 0;

	if(targetIndividual != NULL){
		if(targetIndividual->currentGroupType == GROUP_NPCS || targetIndividual->currentGroupType == GROUP_GUARDS){
			isCrime = 1;
		}

		player->targetedIndividual = targetIndividual;
		return tryPickPocketIndividualShowView(player, targetIndividual, isCrime);
	}

	return -1; //no individual found
}

int tryPickPocketGroup(individualGroup * thisGroup, individual * player, int cursorX, int cursorY, int isCrime){
	int i, individualsPassed = 0, toReturn = 0;

	if(thisGroup->numIndividuals == 0){
		return 0;
	}

	for(i = 0; i < thisGroup->MAX_INDIVIDUALS; i++){
		if(thisGroup->individuals[i] != NULL){
			individual * tmpIndividual = thisGroup->individuals[i];

			if(tmpIndividual->playerCharacter->x == cursorX && tmpIndividual->playerCharacter->y == cursorY){
				toReturn = tryPickPocketIndividualShowView(player, tmpIndividual, isCrime);
				player->targetedIndividual = tmpIndividual;
				return toReturn;
			}


			individualsPassed++;

			if(individualsPassed == thisGroup->numIndividuals){
				break;
			}
		}
	}

	return toReturn;
}

int tryTalkIndividualFromField(individual * player, field * thisField, int cursorX, int cursorY){
	individual * tmpIndividual = getIndividualFromField(thisField, cursorX, cursorY);

	if(tmpIndividual == NULL){
		cwrite("There's nobody there.");
		return 0;
	}

	if(individualWithinTalkingRange(player, tmpIndividual, 2)){
		// Speaking individual is used for processing messages. Needs to be done here before the setCurrentMessage call.
		setSpeakingIndividualID(tmpIndividual->ID);

		if (setCurrentMessageByIndividualID(tmpIndividual->ID, (tmpIndividual->thisBehavior->isHostileToPlayer && (tmpIndividual->currentGroupType == GROUP_NPCS || tmpIndividual->currentGroupType == GROUP_GUARDS)), tmpIndividual->thisBehavior->hasAlreadyYieldedToPlayer, tmpIndividual->dialogPortraitID)){
			toggleDrawDialogBox();
			return 1;
		}
	}

	setSpeakingIndividualID(0);
	return 0;
}

void setGroupSpecialDialog(individualGroup * thisGroup, dialogType thisType){
	int i, individualsPassed = 0;

	for(i = 0; i < thisGroup->MAX_INDIVIDUALS; i++){
		if(thisGroup->individuals[i] != NULL){

			if(thisGroup->individuals[i]->specialDialog->activeDialog == DIALOG_DEFAULT){
				setSpecialDialogId(thisGroup->individuals[i]->ID, thisGroup->individuals[i]->specialDialog->sawPlayerCrime);
				thisGroup->individuals[i]->specialDialog->activeDialog = thisType;
			}

			individualsPassed++;

			if(individualsPassed == thisGroup->numIndividuals){
				break;
			}

		}
	}
}

void clearSpecialDialogForGroup(individualGroup * thisGroup, dialogType thisDialogType){
	int i, individualsPassed = 0;

	for(i = 0; i < thisGroup->MAX_INDIVIDUALS; i++){
		if(thisGroup->individuals[i] != NULL){

			if(thisGroup->individuals[i]->specialDialog->activeDialog == thisDialogType){
				removeSpecialDialog(thisGroup->individuals[i]->ID);
				thisGroup->individuals[i]->specialDialog->activeDialog = DIALOG_DEFAULT;
			}

			individualsPassed++;

			if(individualsPassed == thisGroup->numIndividuals){
				break;
			}
		}
	}
}

void clearCrimeSpecialDialogForGroup(individualGroup * thisGroup){
	int i, individualsPassed = 0;

	for(i = 0; i < thisGroup->MAX_INDIVIDUALS; i++){
		if(thisGroup->individuals[i] != NULL){

			if(thisGroup->individuals[i]->specialDialog->activeDialog == DIALOG_CRIME_WITNESS ||
					thisGroup->individuals[i]->specialDialog->activeDialog == DIALOG_ATTACKED_BY_PLAYER ||
					thisGroup->individuals[i]->specialDialog->activeDialog == DIALOG_STOLEN_FROM_BY_PLAYER ||
					thisGroup->individuals[i]->specialDialog->activeDialog == DIALOG_AFRAID_OF_PLAYER){
				removeSpecialDialog(thisGroup->individuals[i]->ID);
				thisGroup->individuals[i]->specialDialog->activeDialog = DIALOG_DEFAULT;
			}

			individualsPassed++;

			if(individualsPassed == thisGroup->numIndividuals){
				break;
			}
		}
	}
}

void groupTransitUpdate(groupContainer * thisGroupContainer){
	int i, individualsPassed = 0;

	if(thisGroupContainer->npcs->numIndividuals > 0){

		for(i = 0; i < thisGroupContainer->npcs->MAX_INDIVIDUALS; i++){
			if(thisGroupContainer->npcs->individuals[i] != NULL){

				individual * tmpIndividual = thisGroupContainer->npcs->individuals[i];

				tmpIndividual->thisBehavior->hasAlreadyYieldedToPlayer = 0;

				individualsPassed++;

				if(individualsPassed == thisGroupContainer->npcs->numIndividuals){
					individualsPassed = 0;
					break;
				}
			}
		}
	}

	if(thisGroupContainer->guards->numIndividuals > 0){

		for(i = 0; i < thisGroupContainer->guards->MAX_INDIVIDUALS; i++){
			if(thisGroupContainer->guards->individuals[i] != NULL){

				individual * tmpIndividual = thisGroupContainer->guards->individuals[i];

				tmpIndividual->thisBehavior->hasAlreadyYieldedToPlayer = 0;

				individualsPassed++;

				if(individualsPassed == thisGroupContainer->guards->numIndividuals){
					individualsPassed = 0;
					break;
				}
			}
		}
	}
}

void spawnIndividualToField(groupContainer * thisGroupContainer, field * thisField, int individualID, int x, int y, int desiredLocationX, int desiredLocationY){
	individual * tmpIndividual = getIndividualFromRegistry(individualID);
	space * tmpSpace = getSpaceFromField(thisField, x, y);
	if(tmpIndividual == NULL || tmpSpace == NULL || tmpSpace->currentIndividual != NULL){
		return;
	}

	tmpSpace->currentIndividual = tmpIndividual;

	tmpIndividual->playerCharacter->x = x;
	tmpIndividual->playerCharacter->y = y;
	tmpIndividual->desiredLocation->x = x;
	tmpIndividual->desiredLocation->y = y;

	switch(tmpIndividual->currentGroupType){
	case GROUP_ENEMIES:
		addIndividualToGroup(thisGroupContainer->enemies, tmpIndividual);
		break;
	case GROUP_NPCS:
		addIndividualToGroup(thisGroupContainer->npcs, tmpIndividual);
		break;
	case GROUP_GUARDS:
		addIndividualToGroup(thisGroupContainer->guards, tmpIndividual);
		break;
	case GROUP_BEASTS:
		addIndividualToGroup(thisGroupContainer->beasts, tmpIndividual);
		break;
	case GROUP_ALLIES:
		addIndividualToGroup(thisGroupContainer->allies, tmpIndividual);
		break;
	}

	mapInfo * thisMapInfo = getMapInfoFromRegistry(thisField->id);
	addIndividualIdToMapInfo(thisMapInfo, individualID);
}
