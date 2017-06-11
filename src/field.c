/*
 * field.c
 *
 *  Created on: Mar 29, 2015
 *      Author: Adrian
 */

#include"./headers/field_pub_methods.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>


individual* getIndividualFromField(field* thisField, int x, int y){
	if(thisField == NULL){
		return NULL;
	}

	if(x < thisField->totalX && x >= 0 && y < thisField->totalY && y >= 0){
		return thisField->grid[x][y]->currentIndividual;
	}

	return NULL;
}

individual* getIndividualAddressFromField(field* thisField, int x, int y){
	if(x < thisField->totalX && x >= 0 && y < thisField->totalY && y >= 0){

		return &(thisField->grid[x][y]->currentIndividual);
	}

	return NULL;
}

character* getBackgroundFromField(field* thisField, int x, int y){
	if(x < thisField->totalX && x >= 0 && y < thisField->totalY && y >= 0){
		return (thisField->grid[x][y])->background;
	}

	return NULL;
}

space* getSpaceFromField(field* thisField, int x, int y){
	if(x < thisField->totalX && x >= 0 && y < thisField->totalY && y >= 0){
		return thisField->grid[x][y];
	}

	return NULL;
}

int spaceIsAvailable(field* thisField, int x, int y){
	if(x < thisField->totalX && x >= 0 && y < thisField->totalY && y >= 0){
		if(thisField->grid[x][y]->currentIndividual != NULL){
			return 1;
		}
	}

	return 0;
}

int isSpacePassable(field* thisField, int x, int y){
	if(x < thisField->totalX && x >= 0 && y < thisField->totalY && y >= 0){
			if(thisField->grid[x][y]->isPassable){
				return 1;
			}
	}

	return 0;
}

space* getSpaceAddressFromField(field* thisField, int x, int y){
	printf("trying to get space \n");
	if(x < thisField->totalX && x >= 0 && y < thisField->totalY && y >= 0){
		return &(thisField->grid[x][y]);
	}
	printf("was a null space! \n");
	return NULL;
}

int removeIndividualFromField(field * thisField, int x, int y){
	space * tmpSpace = thisField->grid[x][y];

	if(tmpSpace->currentIndividual != NULL){
		tmpSpace->currentIndividual = NULL;
		return 1;
	}

	char logOut[128];
	sprintf(logOut, "!! COULD NOT REMOVE INDIVIDUAL FROM FIELD AT [%d, %d] !!", x, y);
	cwrite(logOut);

	return 0;
}

/*
 * getAdjacentSpaces: returns (up to) 9 spaces surrounding a space as
 * indicated by the x and y values
 */
space** getAdjacentSpaces(field *thisField, int x, int y){
	space** adjacentSpaces = malloc(sizeof(space*)*9);
	space* tmpSpace = malloc(sizeof(tmpSpace));
	int dx,dy,index=0;
	for(dx = -1; dx < 2; dx++){
		for(dy = -1; dy < 2; dy++){
			tmpSpace = getSpaceFromField(thisField,x+dx, y+dy);
			if(tmpSpace != NULL){
				adjacentSpaces[index] = tmpSpace;
				index++;
			}
		}
	}
	adjacentSpaces[index] = '\0';
	return adjacentSpaces;
}

/*
 * xMoveChange and yMoveChange
 * calculates the grid move for x and y depending on the keystroke.
 * for example, to move left the player would hit 4 (dir), decreasing the x, y would stay the same
 */
int xMoveChange(int dir){
	switch(dir){
	case 1:
	case 4:
	case 7:
		return -1;
	case 3:
	case 6:
	case 9:
		return 1;
	}

	//if 2,5,8, no x change
	return 0;
}

int yMoveChange(int dir){
	switch(dir){
	case 7:
	case 8:
	case 9:
		return -1;
	case 1:
	case 2:
	case 3:
		return 1;
	}

	//if 4,5,6 no y change
	return 0;
}

int moveIndividual(field *thisField, individual *thisIndividual, int direction){
	int inX = thisIndividual->playerCharacter->x;
	int inY = thisIndividual->playerCharacter->y;
	int newX = thisIndividual->playerCharacter->x + xMoveChange(direction);
	int newY = thisIndividual->playerCharacter->y + yMoveChange(direction);

	//check if in bounds, and newSpace exists
	if(!(newX >= 0 && newX < thisField->totalX && newY >=0 && newY < thisField->totalY)){
		return 0;
	}

	space * tmpSpace = getSpaceFromField(thisField,newX,newY);

	if(tmpSpace->isPassable && canPassThroughInteractableObject(tmpSpace->interactableObject) && tmpSpace->currentIndividual == NULL){
		getSpaceFromField(thisField,inX,inY)->currentIndividual = NULL;
		tmpSpace->spaceIsReserved = 0;
		getSpaceFromField(thisField,newX,newY)->currentIndividual = thisIndividual;
		thisIndividual->playerCharacter->x = newX;
		thisIndividual->playerCharacter->y = newY;
		return 1;
	}else{
		return 0;
	}
}

int moveIndividualSpace(field *thisField, individual *thisIndividual, int x, int y){

	//check if in bounds
	if(!(x >= 0 && x < thisField->totalX && y >=0 && y < thisField->totalY)){
			return 0;
	}

	space * tmpSpace = getSpaceFromField(thisField,x,y);

	if(tmpSpace->currentIndividual == NULL){
		tmpSpace->spaceIsReserved = 0;
		getSpaceFromField(thisField, thisIndividual->playerCharacter->x, thisIndividual->playerCharacter->y)->currentIndividual = NULL;
		getSpaceFromField(thisField, x, y)->currentIndividual = thisIndividual;
		thisIndividual->playerCharacter->x = x;
		thisIndividual->playerCharacter->y = y;
		return 1;
	}

	//space occoupied
	return 0;
}

int setIndividualSpace(field *thisField, individual *thisIndividual, int x, int y){
	//check if in bounds
	if(!(x >= 0 && x < thisField->totalX && y >=0 && y < thisField->totalY)){
			return 0;
	}

	space * tmpSpace = getSpaceFromField(thisField,x,y);

	if(tmpSpace->currentIndividual == NULL){
		tmpSpace->spaceIsReserved = 0;
		tmpSpace->currentIndividual = thisIndividual;
		thisIndividual->playerCharacter->x = x;
		thisIndividual->playerCharacter->y = y;
		return 1;
	}

	return 0;
}

int attackIfInRange(individual *thisIndividual, individual *targetIndividual, field * thisField){
	if(isInAttackRange(thisIndividual, targetIndividual, thisField)){
		attackIndividual(thisIndividual, targetIndividual);
		return 1;
	}else{
		return 0;
	}
}

int tryAttackIndividual(groupContainer * thisGroupContainer, individual * player, field * thisField, int x, int y){
	int i, numTimesToAttack = 1, toReturn = 0;
	space * tmpSpace = getSpaceFromField(thisField, x, y);

	if(tmpSpace == NULL){
		return 0;
	}

	individual * tmpIndividual = tmpSpace->currentIndividual;

	numTimesToAttack += calcExtraTimesToAttack(player);

	//Only player may trigger attack events for interactables
	if(player->isPlayer){
		interactable * tmpInteractable = tmpSpace->interactableObject;

		if(tmpInteractable != NULL && tmpInteractable->isEnabled){
			int attackEventID = tmpInteractable->onAttackEventID;
			if(attackEventID != -1){
				player->currentInteractableObject = tmpInteractable;
				triggerEvent(attackEventID);
				toReturn = 1;
			}
		}
	}

	if(tmpIndividual != NULL && (tmpIndividual != player || player->thisBehavior->gotConfused) && isInAttackRange(player, tmpIndividual, thisField)){
		for(i = 0; i < numTimesToAttack; i++){
			if(tmpIndividual->hp > 0){
				if (attackIndividual(player, tmpIndividual)) {
					deleteIndividiaulFromGroup(getGroupFromIndividual(thisGroupContainer, tmpIndividual), tmpIndividual);
					removeIndividualFromField(thisField, tmpIndividual->playerCharacter->x, tmpIndividual->playerCharacter->y);
				}
			}
		}

		return 1;
	}

	return toReturn;
}

int tryAttackEnemies(individualGroup * enemies, individual * player, field * thisField, int x, int y){
	int i, j, enemiesPassed = 0, numTimesToAttack = 1;

	numTimesToAttack += calcExtraTimesToAttack(player);

	for (i = 0; i < enemies->MAX_INDIVIDUALS; i++) {

		individual * tmpEnemy = enemies->individuals[i];

		if(tmpEnemy != NULL){
			enemiesPassed++;
			if (tmpEnemy->playerCharacter->x == x && tmpEnemy->playerCharacter->y == y && isInAttackRange(player, tmpEnemy, thisField)) {

				for(j = 0; j < numTimesToAttack; j++){

					if(tmpEnemy->hp > 0){

						if (attackIndividual(player, tmpEnemy)) {
							deleteIndividiaulFromGroup(enemies, tmpEnemy);
							removeIndividualFromField(thisField, tmpEnemy->playerCharacter->x, tmpEnemy->playerCharacter->y);
						}
					}

				}
				return 1;
			}

			if (enemiesPassed == enemies->numIndividuals){
				break;
			}
		}
	}

	return 0;
}

int getSelectedAbilityRange(individual * thisIndividual){
	int range = 0;

	ability * targetAbility = thisIndividual->activeAbilities->selectedAbility;

	if(targetAbility->rangeEnabled){
		range += targetAbility->range->effectAndManaArray[targetAbility->range->selectedIndex]->effectMagnitude;
	}

	return range;
}

int cursorWithinAbilityRange(individual * player, int x, int y){
	int range = 0; //NOT attributeSum, should be relative to the ability

	ability * targetAbility = player->activeAbilities->selectedAbility;

	if(targetAbility->rangeEnabled){
		range += targetAbility->range->effectAndManaArray[targetAbility->range->selectedIndex]->effectMagnitude;
	}

	if(abs(player->playerCharacter->x - x) <= range && abs(player->playerCharacter->y - y) <= range){
		return 1;
	}else{
		return 0;
	}
}

void useAbilityOnIndividualsInAOERange(individual * thisIndividual, individual * player, groupContainer * thisGroupContainer, field * thisField, int x, int y){
	int aoe = 0,  minX, maxX, minY, maxY;
	ability * tmpAbility = thisIndividual->activeAbilities->selectedAbility;

	decreaseMana(thisIndividual, tmpAbility->totalManaCost);

	if(processCasterOnlyEffects(thisIndividual, tmpAbility)){
		if(getGroupFromIndividual(thisGroupContainer, thisIndividual) != NULL){
			deleteIndividiaulFromGroup(getGroupFromIndividual(thisGroupContainer, thisIndividual), thisIndividual);
		}

		removeIndividualFromField(thisField, thisIndividual->playerCharacter->x, thisIndividual->playerCharacter->y);
		triggerEventOnDeath(thisIndividual->ID, thisIndividual->isPlayer);

		removeFromExistance(thisIndividual->ID);
	}

	if(tmpAbility->aoeEnabled){
		aoe = tmpAbility->aoe->effectAndManaArray[tmpAbility->aoe->selectedIndex]->effectMagnitude;
	}

	minX = x - aoe;
	maxX = x + aoe;
	minY = y - aoe;
	maxY = y + aoe;

	//Only Player may trigger attack on interactables
	if(thisIndividual->isPlayer){
		processInteractablesInAOE(thisIndividual, thisField, minX, maxX, minY, maxY);
	}

	preprocessIndividalGroupsInAOE(thisIndividual, thisGroupContainer->enemies, thisField, minX, maxX, minY, maxY);
	preprocessIndividalGroupsInAOE(thisIndividual, thisGroupContainer->npcs, thisField, minX, maxX, minY, maxY);
	preprocessIndividalGroupsInAOE(thisIndividual, thisGroupContainer->guards, thisField, minX, maxX, minY, maxY);
	preprocessIndividalGroupsInAOE(thisIndividual, thisGroupContainer->beasts, thisField, minX, maxX, minY, maxY);

	useAbilityOnIndividualGroupsInAOE(thisIndividual, thisGroupContainer->enemies, thisField, minX, maxX, minY, maxY);
	useAbilityOnIndividualGroupsInAOE(thisIndividual, thisGroupContainer->npcs, thisField, minX, maxX, minY, maxY);
	useAbilityOnIndividualGroupsInAOE(thisIndividual, thisGroupContainer->guards, thisField, minX, maxX, minY, maxY);
	useAbilityOnIndividualGroupsInAOE(thisIndividual, thisGroupContainer->beasts, thisField, minX, maxX, minY, maxY);

	if(player->hp > 0 && player->playerCharacter->x >= minX && player->playerCharacter->x <= maxX &&
		player->playerCharacter->y >= minY && player->playerCharacter->y <= maxY ){

		if(tmpAbility->type == 't'){
			if((abilityIsOffensive(thisIndividual->activeAbilities->selectedAbility) && attackIndividualWithAbility(thisIndividual, player))
					|| useDurationAbilityOnIndividual(player, thisIndividual->activeAbilities->selectedAbility, thisIndividual->name)){
				removeIndividualFromField(thisField, player->playerCharacter->x, player->playerCharacter->y);
			}
		}else if (tmpAbility->type == 'd'){
			//add duration ability logic here
			if(useDurationAbilityOnIndividual(player, tmpAbility, thisIndividual->name)){
				removeIndividualFromField(thisField, player->playerCharacter->x, player->playerCharacter->y);
			}
		}
	}
}

void processInteractablesInAOE(individual * player, field * thisField, int minX, int maxX, int minY, int maxY){
	int i, j;
	space * tmpSpace;

	for(i = minX; i < maxX; i++){
		for(j = minY; j < maxY; j++){

			tmpSpace = getSpaceFromField(thisField, i, j);

			if(tmpSpace->interactableObject != NULL){
				if(player->activeAbilities->selectedAbility->type == 't' && abilityIsOffensive(player->activeAbilities->selectedAbility)){
					int attackEventID = tmpSpace->interactableObject->onAttackEventID;
					if(attackEventID  != -1){
						player->currentInteractableObject = tmpSpace->interactableObject;
						triggerEvent(attackEventID);
					}
				}
			}
		}
	}
}

void preprocessIndividalGroupsInAOE(individual * thisIndividual, individualGroup * thisGroup, field * thisField, int minX, int maxX, int minY, int maxY){
	int i, individualsPassed = 0;

		for(i = 0; i < thisGroup->MAX_INDIVIDUALS; i++){
			individual * tmp = thisGroup->individuals[i];

			if(tmp != NULL && tmp->hp > 0){
				individualsPassed++;

				if(tmp->playerCharacter->x >= minX &&
						tmp->playerCharacter->x <= maxX &&
						tmp->playerCharacter->y >= minY &&
						tmp->playerCharacter->y <= maxY ){
					if(thisIndividual->activeAbilities->selectedAbility->type == 't' && abilityIsOffensive(thisIndividual->activeAbilities->selectedAbility)){
						triggerEventOnAttack(tmp->ID, thisIndividual->isPlayer);
						onAttackedChecks(thisIndividual, tmp);

						if(!individualInGroup(tmp, thisGroup)){
							individualsPassed--;
						}
					}else if (thisIndividual->activeAbilities->selectedAbility->type == 'd'){
						if(abilityIsHarmful(thisIndividual->activeAbilities->selectedAbility)){
							triggerEventOnAttack(tmp->ID, thisIndividual->isPlayer);
							onAttackedChecks(thisIndividual, tmp);

							if(!individualInGroup(tmp, thisGroup)){
								individualsPassed--;
							}
						}
					}
				}

				if(individualsPassed >= thisGroup->numIndividuals){
					break;
				}
			}
		}
}

void useAbilityOnIndividualGroupsInAOE(individual * thisIndividual, individualGroup * thisGroup, field * thisField,int minX, int maxX, int minY, int maxY){
	int i, individualsPassed = 0;

	for(i = 0; i < thisGroup->MAX_INDIVIDUALS; i++){
		individual * tmp = thisGroup->individuals[i];

		if(tmp != NULL && tmp->hp > 0){
			individualsPassed++;

			if(tmp->playerCharacter->x >= minX &&
					tmp->playerCharacter->x <= maxX &&
					tmp->playerCharacter->y >= minY &&
					tmp->playerCharacter->y <= maxY ){
				if(thisIndividual->activeAbilities->selectedAbility->type == 't'){
					if((abilityIsOffensive(thisIndividual->activeAbilities->selectedAbility) && attackIndividualWithAbility(thisIndividual, tmp))
							|| useDurationAbilityOnIndividual(tmp, thisIndividual->activeAbilities->selectedAbility, thisIndividual->name)){
						deleteIndividiaulFromGroup(thisGroup, tmp);
						removeIndividualFromField(thisField, tmp->playerCharacter->x, tmp->playerCharacter->y);
						removeFromExistance(tmp->ID);

						individualsPassed--;
					}
				}else if (thisIndividual->activeAbilities->selectedAbility->type == 'd'){

					if(useDurationAbilityOnIndividual(tmp, thisIndividual->activeAbilities->selectedAbility, thisIndividual->name)){
						deleteIndividiaulFromGroup(thisGroup, tmp);

						triggerEventOnDeath(thisIndividual->ID, thisIndividual->isPlayer);

						if(thisIndividual->isPlayer && (tmp->currentGroupType == GROUP_NPCS || tmp->currentGroupType == GROUP_GUARDS)){
							processCrimeEvent(CRIME_MURDER, 300, tmp->ID, 0);
						}

						removeIndividualFromField(thisField, tmp->playerCharacter->x, tmp->playerCharacter->y);
						removeFromExistance(tmp->ID);

						individualsPassed--;
					}
				}

			}

			if(individualsPassed >= thisGroup->numIndividuals){
				break;
			}
		}
	}
}

void wanderAround(field * thisField, individual * thisIndividual){
 int direction = rand() % 10+1;
 moveIndividual(thisField, thisIndividual, direction);
}

field * loadMap(char * mapName, char* directory, individual * player, groupContainer * thisGroupContainer){
	int mapID;
	char transitMap[80], mapIDStr[80];
	char * fullMapName = appendStrings(directory, mapName);

	FILE * fp = fopen(fullMapName, "r");

	//transit map filename
	fgets(transitMap,80,fp);

	fgets(mapIDStr,80,fp);
	fclose(fp);

	mapID = atoi(mapIDStr);

	field* thisField = initField(fullMapName);

	mapInfo * thisMapInfo = getMapInfoFromRegistry(mapID);

	thisMapInfo->isCurrentMap = 1;
	loadGroups(thisGroupContainer, thisMapInfo, thisField);
	addItemsToField(thisMapInfo, thisField);
	addInteractableObjectsToField(thisMapInfo, thisField);
	thisField->isDark = thisMapInfo->isDark;

	makeTransitSpaces(transitMap, directory, thisField, player);

	moveIndividualSpace(thisField,player, player->playerCharacter->x, player->playerCharacter->y);

	free(fullMapName);

	printf("done loading map!");
	return thisField;
}

destroyField(field * thisField, individual * player){
	int i,j;

	while(!tryGetFieldInventoryReadLock()){}
	while(!tryGetFieldInventoryWriteLock()){}

	if(thisField->thisFieldInventory != NULL){
		free(thisField->thisFieldInventory);
		thisField->thisFieldInventory = NULL;
	}

	releaseFieldInventoryWriteLock();
	releaseFieldInventoryReadLock();

	for(i = 0; i < 100; i++){
		for(j = 0; j < 100; j++){
			if(thisField->grid[i][j] != NULL){
				destroyCharacter(thisField->grid[i][j]->background);
				if(thisField->grid[i][j]->thisTransitInfo != NULL){
					free(thisField->grid[i][j]->thisTransitInfo);
				}
				free(thisField->grid[i][j]);
			}
		}
	}
	free(thisField->playerCords);

	free(thisField);
}

void makeTransitSpaces(char * transitMap, char* directory, field * thisField, individual * player){
	char * fullTransitFile = appendStrings(directory, transitMap);
	fullTransitFile[strlen(fullTransitFile)-1] = '\0'; //remove '\n' at end of line
	FILE * enemyFP = fopen(fullTransitFile, "r");
	char line[80];

	printf("%s\n",fullTransitFile);

	while (fgets(line, 80, enemyFP) != NULL) {
		if (line[0] != '#') { //ignore commented-out lines
			space * tmpSpace;
			int id, x, y, targetID;
			char targetTransitMap[32]; // = malloc(sizeof(char) * 32);

			char * transitInstance = strtok(line, ";");
			id = atoi(transitInstance);

			transitInstance = strtok(NULL, ";");
			x = atoi(transitInstance);

			transitInstance = strtok(NULL, ";");
			y = atoi(transitInstance);

			transitInstance = strtok(NULL, ";");
			strcpy(targetTransitMap, transitInstance);

			transitInstance = strtok(NULL, ";");
			targetID = atoi(transitInstance);

			tmpSpace = getSpaceFromField(thisField, x, y);

			if (thisField->grid[x][y]->thisTransitInfo != NULL) {
				free(thisField->grid[x][y]->thisTransitInfo);
				thisField->grid[x][y]->thisTransitInfo = NULL;
			}

			thisField->grid[x][y]->thisTransitInfo = malloc(sizeof(transitInfo));
			thisField->grid[x][y]->thisTransitInfo->transitID = id;
			strcpy(thisField->grid[x][y]->thisTransitInfo->transitMap,targetTransitMap);
			thisField->grid[x][y]->thisTransitInfo->targetMapTransitID = targetID;


			//spawn player at this location
			if(player->jumpTarget == id){
				printf("Jumping player:[%d,%d]\n", x, y);
				setIndividualSpace(thisField,player,x,y);
				player->jumpTarget = 0;
			}

		}
	}

	free(fullTransitFile);
	fclose(enemyFP);

}

void addInteractableObjectToField(interactable * thisInteractableObject, field * thisField){
	int x = thisInteractableObject->thisCharacter->x;
	int y = thisInteractableObject->thisCharacter->y;

//	if(!thisInteractableObject->shouldDraw){
//		return;
//	}

	space * tmpSpace = getSpaceFromField(thisField, x, y);

	if(tmpSpace != NULL){
		tmpSpace->interactableObject = thisInteractableObject;
	}
}

int addItemToField(field * thisField, item * thisItem){
	int i;

	while(!tryGetFieldInventoryReadLock()){}
	while(!tryGetFieldInventoryWriteLock()){}

	if(thisField->thisFieldInventory->inventorySize >= thisField->thisFieldInventory->MAX_ITEMS){
		releaseFieldInventoryWriteLock();
		releaseFieldInventoryReadLock();
		return 0;
	}

	for(i = 0; i < thisField->thisFieldInventory->MAX_ITEMS; i++){
		if(thisField->thisFieldInventory->inventoryArr[i] == NULL){
			thisField->thisFieldInventory->inventoryArr[i] = thisItem;
			thisField->thisFieldInventory->inventorySize++;

			releaseFieldInventoryWriteLock();
			releaseFieldInventoryReadLock();
			return 1;
		}
	}

	releaseFieldInventoryWriteLock();
	releaseFieldInventoryReadLock();

	return 0;
}

void removeItemFromField(field * thisField, item * thisItem){
	int i;
	mapInfo * thisMap = getMapInfoFromRegistry(thisField->id);

	while(!tryGetFieldInventoryReadLock()){}
	while(!tryGetFieldInventoryWriteLock()){}

	for(i = 0; i < thisField->thisFieldInventory->MAX_ITEMS; i++){
		if(thisField->thisFieldInventory->inventoryArr[i] == thisItem){
//			removeFromExistance(thisField->thisFieldInventory->inventoryArr[i]->ID);
			thisField->thisFieldInventory->inventoryArr[i] = NULL;

			if(thisMap != NULL){
				removeItemIdFromMapInfo(thisMap, thisItem->ID);
			}

			break;
		}
	}

	releaseFieldInventoryWriteLock();
	releaseFieldInventoryReadLock();
}

int attemptGetItemFromField(field * thisField, individual * thisIndividual){
	int i, itemsProcessed = 0;
	int itemX = thisIndividual->playerCharacter->x;
	int itemY = thisIndividual->playerCharacter->y;
	item * theItem = NULL;

	if(thisField->thisFieldInventory->inventorySize == 0){
		return 0;
	}

	for(i = 0; i < thisField->thisFieldInventory->MAX_ITEMS; i++){
		if(thisField->thisFieldInventory->inventoryArr[i] != NULL){
			theItem = thisField->thisFieldInventory->inventoryArr[i];

			if(theItem->itemCharacter->x == itemX && theItem->itemCharacter->y == itemY){
				removeItemFromField(thisField, theItem);
				addItemToInventory(thisIndividual->backpack, theItem);
				theItem->npcID = thisIndividual->ID;

				triggerEventOnPickup(theItem->ID, thisIndividual->isPlayer);

				if(theItem->owner != 0 && theItem->isStolen == 0){
					theItem->isStolen = 1;

					processCrimeEvent(CRIME_STEALING, theItem->price, theItem->owner, theItem->ID);
				}

				return 1;
			}

			itemsProcessed++;
		}

		if(itemsProcessed == thisField->thisFieldInventory->inventorySize){
			return 0;
		}
	}
	return 0;
}

void populateCurrentSpaceInventory(field * thisField, int x, int y){
	int i, itemsPassed = 0;
	item * tmpItem;

	for(i = 0; i < thisField->currentSpaceInventory->MAX_ITEMS; i++){
		thisField->currentSpaceInventory->inventoryArr[i] = NULL;
	}
	thisField->currentSpaceInventory->inventorySize = 0;

	if(thisField->thisFieldInventory->inventorySize != 0){
		for(i = 0; i < thisField->thisFieldInventory->MAX_ITEMS; i++){
			tmpItem = thisField->thisFieldInventory->inventoryArr[i];

			if(tmpItem != NULL){
				itemsPassed++;

				if(tmpItem->itemCharacter->x == x && tmpItem->itemCharacter->y == y){
					addItemToInventory(thisField->currentSpaceInventory, tmpItem);
				}

				if(itemsPassed == thisField->thisFieldInventory->inventorySize){
					break;
				}
			}
		}
	}
}

int canPassThroughImage(int imageID){
	if(imageID == 7502 ||
		imageID == 7503 ||
		imageID == 7504 ||
		imageID == 7505 ||
		imageID == 7506 ||
		imageID == 7507 ||
		imageID == 7509 ||
		imageID == 7508 ||
		imageID == 7514){
		return 0;
	}else{
		return 1;
	}
}

int canSeeThroughImage(int imageID){
	if(imageID == 7514 ||
		imageID == 7509 ||
		imageID == 7508){
		return 0;
	}else{
		return 1;
	}
}

int canAttackThroughImage(int imageID){
	if(imageID == 7514 ||
		imageID == 7509 ||
		imageID == 7508){
		return 0;
	}else{
		return 1;
	}
}

field * initField(char * fieldFileName){
	field* thisField = malloc(sizeof(field));
	FILE * fp = fopen(fieldFileName, "r");
	char line[1024];
	char * value;
	char * strtok_save_pointer;

	//used to get rid of the first 2 lines
	fgets(line,1024,fp);

	//field ID
	fgets(line, 1024, fp);
	thisField->id = atoi(line);

	int i, j;
	int fieldWidth = 0;
	int fieldHeight = 0;

	//init field to null
	for(i = 0; i < 100; i++){
		for(j = 0; j < 100; j++){
			thisField->grid[i][j] = NULL;
		}
	}

	i = 0;
	j = 0;

	while(fgets(line, 1024, fp) != NULL){
		int imageId, direction;
		i = 0;

		value = strtok_r(line, ";", &strtok_save_pointer);

		while(value != NULL && *value != '\n'){
			imageId = atoi(strtok(value,","));
			direction = atoi(strtok(NULL,","));

			space * newSpace = malloc(sizeof(space));
			newSpace->currentIndividual = NULL;
			newSpace->thisTransitInfo = NULL;
			newSpace->spaceIsReserved = 0;
			newSpace->isPassable = canPassThroughImage(imageId);
			newSpace->canSeeThrough = canSeeThroughImage(imageId);
			newSpace->canAttackThrough = canAttackThroughImage(imageId);
			newSpace->interactableObject = NULL;

			character* backgroundCharacter = malloc(sizeof(character));
			backgroundCharacter->x = i;
			backgroundCharacter->y = j;
			backgroundCharacter->xOff = 0;
			backgroundCharacter->yOff = 0;
			backgroundCharacter->thisAnimationContainer = initAnimationContainer();
			backgroundCharacter->thisAnimationContainer->animationsEnabled = 1;
			backgroundCharacter->thisAnimationContainer->defaultAnimation = 0;
			backgroundCharacter->darkAnimationContainer = initAnimationContainer();
			backgroundCharacter->darkAnimationContainer->animationsEnabled = 1;
			backgroundCharacter->darkAnimationContainer->defaultAnimation = 0;
			backgroundCharacter->secondaryAnimationContainer = NULL;
			backgroundCharacter->direction = direction;

			addAnimationToContainer(backgroundCharacter->thisAnimationContainer, cloneAnimationFromRegistry(imageId));

			newSpace->background = backgroundCharacter;
			thisField->grid[i][j] = newSpace;
			i++;

			value = strtok_r(NULL, ";", &strtok_save_pointer);
		}

		j++;
	}

	fclose(fp);

	thisField->totalX = i;
	thisField->totalY = j;

	thisField->thisFieldInventory = malloc(sizeof(fieldInventory));
	thisField->thisFieldInventory->inventorySize = 0;
	thisField->thisFieldInventory->MAX_ITEMS = 1000;

	thisField->currentSpaceInventory = malloc(sizeof(inventory));
	thisField->currentSpaceInventory->inventorySize = 0;
	thisField->currentSpaceInventory->MAX_ITEMS = 40;

	thisField->playerCords = malloc(sizeof(cord));
	thisField->playerCords->x = 0;
	thisField->playerCords->y = 0;

	for(i = 0; i < 1000; i++){
		thisField->thisFieldInventory->inventoryArr[i] = NULL;
		if(i < 40){
			thisField->currentSpaceInventory->inventoryArr[i] = NULL;
		}
	}

	return thisField;
}

void updateFieldGraphics(HDC hdc, HDC hdcBuffer, field* thisField){
	int x, y, i;

	for (y = 0; y < thisField->totalY; y++) {
		for (x = 0; x < thisField->totalX; x++) {
			character * tmpBackground = thisField->grid[x][y]->background;

			animation * defaultAnimation = tmpBackground->thisAnimationContainer->animations[tmpBackground->thisAnimationContainer->currentAnimation];

			if(thisField->isDark){
				animation * greyScaleAnimation = cloneAnimation(defaultAnimation);
				addAnimationToContainer(tmpBackground->darkAnimationContainer, greyScaleAnimation);
			}

			if(tmpBackground->direction > 0){
				rotateAnimationFrames(hdc, hdcBuffer, defaultAnimation, tmpBackground->direction);

				if(thisField->isDark){
					printf("dark, rotate\n");fflush(stdout);
					rotateAnimationFrames(hdc, hdcBuffer, tmpBackground->darkAnimationContainer->animations[tmpBackground->darkAnimationContainer->currentAnimation], tmpBackground->direction);
				}
			}

			if(thisField->isDark){
				printf("dark, greyscale\n");fflush(stdout);
				animation * tmpAnimation = tmpBackground->darkAnimationContainer->animations[tmpBackground->darkAnimationContainer->defaultAnimation];
				printf("default: %d \n", tmpBackground->darkAnimationContainer->defaultAnimation);fflush(stdout);
				convertToGreyScale(hdc, hdcBuffer, tmpAnimation);
			}
		}
	}
}

void drawField(HDC hdc, HDC hdcBuffer, field* thisField, shiftData * viewShift){
	int x, y, i;

	for (y = 0; y < thisField->totalY; y++) {
		for (x = 0; x < thisField->totalX; x++) {
			character * tmpBackground = thisField->grid[x][y]->background;
			updateAnimation(tmpBackground);

			if(thisField->isDark && thisField->playerLoS < max(abs(thisField->playerCords->x - x), abs(thisField->playerCords->y - y))){
				drawCharacterAnimationGreyscale(hdc, hdcBuffer, tmpBackground, viewShift, 0);
			}else{
				drawCharacterAnimation(hdc, hdcBuffer, tmpBackground, viewShift, 0);
			}

			if(thisField->grid[x][y]->interactableObject != NULL && thisField->grid[x][y]->interactableObject->shouldDraw && (!thisField->isDark || thisField->playerLoS >= max(abs(thisField->playerCords->x - x), abs(thisField->playerCords->y - y)))){
				drawCharacterAnimation(hdc, hdcBuffer, thisField->grid[x][y]->interactableObject->thisCharacter, viewShift, 0);
				updateAnimation(thisField->grid[x][y]->interactableObject->thisCharacter);
			}

		}
	}
}

void drawItemsFromField(HDC hdc, HDC hdcBuffer, field * thisField, shiftData * viewShift){
	int i, numDrawn = 0;

	printf("WANT: DRAW ITEMS\n");fflush(stdout);
	while(!tryGetFieldInventoryReadLock()){}
	printf("GOT: DRAW ITEMS\n");fflush(stdout);
	if(thisField->thisFieldInventory == NULL){
		releaseFieldInventoryReadLock();
		printf("RELEASED: DRAW ITEMS\n");fflush(stdout);
		return;
	}

	for(i = 0; i < 1000; i++){

		if(thisField->thisFieldInventory->inventoryArr[i] != NULL){

			if(!thisField->isDark || thisField->playerLoS >= max(
					abs(thisField->playerCords->x - thisField->thisFieldInventory->inventoryArr[i]->itemCharacter->x),
					abs(thisField->playerCords->y - thisField->thisFieldInventory->inventoryArr[i]->itemCharacter->y))){
				drawCharacterAnimation(hdc, hdcBuffer, thisField->thisFieldInventory->inventoryArr[i]->itemCharacter, viewShift);
			}

			numDrawn++;
		}

		//all items drawn
		if(numDrawn == thisField->thisFieldInventory->inventorySize){
			break;
		}

	}

	releaseFieldInventoryReadLock();
	printf("RELEASED: DRAW ITEMS\n");fflush(stdout);
}

void rotateAndDrawImage(HDC hdc, HDC hdcBuffer, character * backgroundCharacter, shiftData * viewShift){
	animation * currentAnimation = backgroundCharacter->thisAnimationContainer->animations[backgroundCharacter->thisAnimationContainer->currentAnimation];
	BITMAPINFO bitMapInfo = {0};
	bitMapInfo.bmiHeader.biSize = sizeof(bitMapInfo.bmiHeader);

	HBITMAP thisImage = LoadImage(GetModuleHandle(NULL), currentAnimation->imageID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION); //GetModuleHandle(NULL)

	//Fill out bitMapInfo
	if(!GetDIBits(hdcBuffer, thisImage, 0, 0, NULL, &bitMapInfo, DIB_RGB_COLORS)){
		printf("Couldint get the bitMapInfo.\n");
	}

	//Create pixel buffer
	char * lpPixels = malloc(sizeof(char)*(bitMapInfo.bmiHeader.biSizeImage));

//	bitMapInfo.bmiHeader.biBitCount = 32;
	bitMapInfo.bmiHeader.biCompression = BI_RGB;
	bitMapInfo.bmiHeader.biHeight = abs(bitMapInfo.bmiHeader.biHeight);

	int result = GetDIBits(hdc, thisImage, 0, bitMapInfo.bmiHeader.biHeight, lpPixels, &bitMapInfo, DIB_RGB_COLORS);

	if(result == 0){
		printf("Couldint get the bitmap.\n");
	}

	SetDIBits(hdc, thisImage, 0, bitMapInfo.bmiHeader.biHeight, lpPixels, &bitMapInfo, DIB_RGB_COLORS);

//	HBITMAP image = CreateBitmapIndirect(&srcImg);
//
	HDC hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, thisImage);
//
	BitBlt(hdcBuffer, backgroundCharacter->x*52 + (int)(backgroundCharacter->xOff*52) - (viewShift->xShift)*52 - 25, backgroundCharacter->y*52 + (int)(backgroundCharacter->yOff*52) - (viewShift->yShift)*52 - 25,
				100, 100,
				hdcMem,
				0,
				0,
				SRCPAINT);
	DeleteDC(hdcMem);

	//delete buffer
	free(lpPixels);
}

moveNode * nodeAlreadyTraversed(moveNode ** nodes, int x, int y){
	int i = 0;
	while(nodes[i] != NULL){
		if(nodes[i]->x == x && nodes[i]->y == y){
			return nodes[i];
		}
	}

	return NULL;
}

moveNode * alreadyContainsNode(moveNode * rootNode, int x, int y) {
	moveNode * currentNode = rootNode;

	while (currentNode->nextMoveNode != NULL) {
		if (currentNode->x == x && currentNode->y == y) {
			return &currentNode;
		}

		currentNode = (moveNode *)currentNode->nextMoveNode;
	}

	//check last node
	if (currentNode->x == x && currentNode->y == y) {
		return &currentNode;
	}

	return NULL;
}

int freeUpMovePath(moveNode * currentNode){
	int sum = 1;
	if(currentNode->nextMoveNode !=NULL){
		sum += freeUpMovePath((moveNode *)currentNode->nextMoveNode);
	}

	free(currentNode);
	return sum;
}


void printField(field * thisField){
	int i,j;

	for(i = 0; i < thisField->totalX; i++){
		for(j = 0; j < thisField->totalY; j++){
			printf("[%d,%d] : %d\n",i,j, getIndividualFromField(thisField,i,j));//spaceIsAvailable(thisField,i,j));
		}
	}
}

mapInfo * initMapInfo(){
	mapInfo * newMapInfo = malloc(sizeof(mapInfo));
	newMapInfo->MAX_INDIVIDUALS = 250;
	newMapInfo->numIndividuals = 0;
	newMapInfo->MAX_ITEMS = 500;
	newMapInfo->numItems = 0;
	newMapInfo->MAX_INTERACTABLES = 500;
	newMapInfo->numInteractables = 0;
	newMapInfo->isDark = 0;
	newMapInfo->isCurrentMap = 0;

	return newMapInfo;
}

void parseMapIndividualsFromLine(mapInfo * thisMapInfo, char * line){
	char * value;

	if(line == NULL){
		return;
	}

	value = strtok(line, ",");

	if(value == NULL || *value == '\n' || atoi(value) == -1){
		return;
	}

	thisMapInfo->individuals[thisMapInfo->numIndividuals] = atoi(value);
	thisMapInfo->numIndividuals++;

	value = strtok(NULL, ",");

	while(value != NULL){
		if(thisMapInfo->numIndividuals >= thisMapInfo->MAX_INDIVIDUALS){
			cwrite("!! MAX INDIVIDUALS ON FIELD !!");
			return;
		}

		thisMapInfo->individuals[thisMapInfo->numIndividuals] = atoi(value);
		thisMapInfo->numIndividuals++;

		value = strtok(NULL, ",");
	}
}

void parseMapItemsFromLine(mapInfo * thisMapInfo, char * line){
	char * value;

	if(line == NULL){
		return;
	}

	value = strtok(line, ",");

	if(value == NULL || *value == '\n' || atoi(value) == -1){
		return;
	}

	thisMapInfo->items[thisMapInfo->numItems] = atoi(value);
	thisMapInfo->numItems++;

	value = strtok(NULL, ",");

	while(value != NULL){
		if(thisMapInfo->numItems >= thisMapInfo->MAX_ITEMS){
			cwrite("!! MAX ITEMS ON FIELD !!");
			return;
		}

		thisMapInfo->items[thisMapInfo->numItems] = atoi(value);
		thisMapInfo->numItems++;

		value = strtok(NULL, ",");
	}
}

void parseMapInteractablesFromLine(mapInfo * thisMapInfo, char * line){
	char * value;

	if(line == NULL){
		return;
	}

	value = strtok(line, ",");

	if(value == NULL || *value == '\n' || atoi(value) == -1){
		return;
	}

	thisMapInfo->interactableObjects[thisMapInfo->numInteractables] = atoi(value);
	thisMapInfo->numInteractables++;

	value = strtok(NULL, ",");

	while(value != NULL){
		if(thisMapInfo->numInteractables >= thisMapInfo->MAX_INTERACTABLES){
			cwrite("!! MAX INTERACTABLES ON FIELD !!");
			return;
		}

		thisMapInfo->interactableObjects[thisMapInfo->numInteractables] = atoi(value);
		thisMapInfo->numInteractables++;

		value = strtok(NULL, ",");
	}
}

void createMapInfoFromLine(mapInfo * thisMapInfo, char * line){
	char * strtok_save_pointer;
	char * value = strtok_r(line,";",&strtok_save_pointer);
	thisMapInfo->id = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	strcpy(thisMapInfo->mapName, value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	thisMapInfo->isCurrentMap = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	thisMapInfo->isDark = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	parseMapIndividualsFromLine(thisMapInfo, value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	parseMapItemsFromLine(thisMapInfo, value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	parseMapInteractablesFromLine(thisMapInfo, value);
}

char * getMapInfoAsLine(mapInfo * thisMapInfo){
	int i = 0, j;
	char * line = malloc(sizeof(char) * 2048);

	i += sprintf(line + i, "%d;", thisMapInfo->id);
	i += sprintf(line + i, "%s;", thisMapInfo->mapName);
	i += sprintf(line + i, "%d;", thisMapInfo->isCurrentMap);
	i += sprintf(line + i, "%d;", thisMapInfo->isDark);

	if(thisMapInfo->numIndividuals == 0){
		i += sprintf(line + i, "-1;");
	}else{
		for(j = 0; j < thisMapInfo->numIndividuals; j++){
			if(j + 1 == thisMapInfo->numIndividuals){
				i += sprintf(line + i, "%d", thisMapInfo->individuals[j]);
			}else{
				i += sprintf(line + i, "%d,", thisMapInfo->individuals[j]);
			}
		}

		i += sprintf(line + i, ";");
	}

	if(thisMapInfo->numItems == 0){
			i += sprintf(line + i, "-1;");
	}else{
		for(j = 0; j < thisMapInfo->numItems; j++){
			if(j + 1 == thisMapInfo->numItems){
				i += sprintf(line + i, "%d", thisMapInfo->items[j]);
			}else{
				i += sprintf(line + i, "%d,", thisMapInfo->items[j]);
			}
		}

		i += sprintf(line + i, ";");
	}

	if(thisMapInfo->numInteractables == 0){
			i += sprintf(line + i, "-1;");
	}else{
		for(j = 0; j < thisMapInfo->numInteractables; j++){
			if(j + 1 == thisMapInfo->numInteractables){
				i += sprintf(line + i, "%d", thisMapInfo->interactableObjects[j]);
			}else{
				i += sprintf(line + i, "%d,", thisMapInfo->interactableObjects[j]);
			}
		}

		i += sprintf(line + i, ";");
	}

	return line;
}

void removeIndividualIdFromMapInfo(mapInfo * thisMap, int individualID){
	int i;

	if(thisMap->numIndividuals > 0){
		for(i = 0; i < thisMap->numIndividuals; i++){
			if(thisMap->individuals[i] == individualID){
				thisMap->numIndividuals--;
				thisMap->individuals[i] = thisMap->individuals[thisMap->numIndividuals];
				thisMap->individuals[thisMap->numIndividuals] = NULL;

				return;
			}
		}
	}
}

void addIndividualIdToMapInfo(mapInfo * thisMap, int individualID){
	if(thisMap->numIndividuals < thisMap->MAX_INDIVIDUALS){
		thisMap->individuals[thisMap->numIndividuals] = individualID;
		thisMap->numIndividuals++;
	}
}


void removeItemIdFromMapInfo(mapInfo * thisMap, int itemID){
	int i;

	if(thisMap->numItems > 0){
		for(i = 0; i < thisMap->numItems; i++){
			if(thisMap->items[i] == itemID){
				thisMap->numItems--;
				thisMap->items[i] = thisMap->items[thisMap->numItems];
				thisMap->items[thisMap->numItems] = NULL;

				return;
			}
		}
	}
}

void addIndividualsToMapInfo(mapInfo * thisMap, int individualID){
	if(thisMap->numIndividuals < thisMap->MAX_INDIVIDUALS){
		thisMap->individuals[thisMap->numIndividuals] = individualID;
		thisMap->numIndividuals++;
		return;
	}

	char logOut[128];
	sprintf(logOut, "!! COULD NOT ADD INDIVIDUAL TO MAP INFO, FULL: %d!!", individualID);
	cwrite(logOut);
}

void addItemToMapInfo(mapInfo * thisMap, int itemID){
	if(thisMap->numItems < thisMap->MAX_ITEMS){
		thisMap->items[thisMap->numItems] = itemID;
		thisMap->numItems++;
		return;
	}

	char logOut[128];
	sprintf(logOut, "!! COULD NOT ADD ITEM TO MAP INFO, FULL: %d!!", itemID);
	cwrite(logOut);
}
