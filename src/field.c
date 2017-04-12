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

	if(getSpaceFromField(thisField,newX,newY)->isPassable && getSpaceFromField(thisField,newX,newY)->currentIndividual == NULL){
		getSpaceFromField(thisField,inX,inY)->currentIndividual = NULL;
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

	if(getSpaceFromField(thisField,x,y)->currentIndividual == NULL){
		getSpaceFromField(thisField, thisIndividual->playerCharacter->x, thisIndividual->playerCharacter->y)->currentIndividual = NULL;
		getSpaceFromField(thisField, x, y)->currentIndividual = thisIndividual;
		thisIndividual->playerCharacter->x = x;
		thisIndividual->playerCharacter->y = y;
		return 1;
	}

	//space occoupied
	return 0;

}

/*
 * FromJump: the players old space may not exist anymore - removing logic to remove currentIndividual from old space
 */
int setIndividualSpace(field *thisField, individual *thisIndividual, int x, int y){

	//check if in bounds
	if(!(x >= 0 && x < thisField->totalX && y >=0 && y < thisField->totalY)){
			return 0;
	}

	if(getSpaceFromField(thisField,x,y)->currentIndividual == NULL){
//		getSpaceFromField(thisField, thisIndividual->playerCharacter->x, thisIndividual->playerCharacter->y)->currentIndividual = NULL;
		getSpaceFromField(thisField, x, y)->currentIndividual = thisIndividual;
		thisIndividual->playerCharacter->x = x;
		thisIndividual->playerCharacter->y = y;
		return 1;
	}

	//space occoupied
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
	individual * tmpIndividual = getIndividualFromField(thisField, x, y);
	int i, numTimesToAttack = 1;
	numTimesToAttack += calcExtraTimesToAttack(player);

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

	return 0;
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
					|| useDurationAbilityOnIndividual(player, thisIndividual->activeAbilities->selectedAbility)){
				removeIndividualFromField(thisField, player->playerCharacter->x, player->playerCharacter->y);
			}
		}else if (tmpAbility->type == 'd'){
			//add duration ability logic here
			if(useDurationAbilityOnIndividual(player, tmpAbility)){
				removeIndividualFromField(thisField, player->playerCharacter->x, player->playerCharacter->y);
			}
		}
	}

}

void preprocessIndividalGroupsInAOE(individual * thisIndividual, individualGroup * thisGroup, field * thisField,int minX, int maxX, int minY, int maxY){
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
							|| useDurationAbilityOnIndividual(tmp, thisIndividual->activeAbilities->selectedAbility)){
						deleteIndividiaulFromGroup(thisGroup, tmp);
						removeIndividualFromField(thisField, tmp->playerCharacter->x, tmp->playerCharacter->y);
						removeFromExistance(tmp->ID);

						individualsPassed--;
					}
				}else if (thisIndividual->activeAbilities->selectedAbility->type == 'd'){

					if(useDurationAbilityOnIndividual(tmp, thisIndividual->activeAbilities->selectedAbility)){
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


animation * generateBackground(char backgroundSymbol){
	switch(backgroundSymbol){
		case 'c':
			return cloneAnimationFromRegistry(7502);
		case 'g':
			return cloneAnimationFromRegistry(7501);
		case '-':
			return cloneAnimationFromRegistry(7503);
		case '=':
			return cloneAnimationFromRegistry(7504);
		case 'r':
			return cloneAnimationFromRegistry(7505);
		case 'u':
			return cloneAnimationFromRegistry(7506);
		case 'o':
			return cloneAnimationFromRegistry(7507);
		case 't':
			return cloneAnimationFromRegistry(7514);
		case 'p':
			return cloneAnimationFromRegistry(7511);
		case '\\':
			return cloneAnimationFromRegistry(7512);
		case 'f':
			return cloneAnimationFromRegistry(7513);
		case 'x':
			return cloneAnimationFromRegistry(7508);
		case 's':
			return cloneAnimationFromRegistry(7509);
		case 'd':
			return cloneAnimationFromRegistry(7510);
		default:
			return cloneAnimationFromRegistry(7501);
	}

//	if(backgroundSymbol == 'c'){
//		return 7502;
//	}else if (backgroundSymbol == 'g'){
//		return 7501;
//	}else if (backgroundSymbol == '-'){
//		return 7503;
//	}else if (backgroundSymbol == '='){
//		return 7504;
//	}else if (backgroundSymbol == 'r'){
//		return 7505;
//	}else if (backgroundSymbol == 'u'){
//		return 7506;
//	}else if (backgroundSymbol == 'o'){
//		return 7507;
//	}else if (backgroundSymbol == 't'){
//		return 7514;
//	}else if (backgroundSymbol == 'p'){
//		return 7511;
//	}else if (backgroundSymbol == '\\'){
//		return 7512;
//	}else if (backgroundSymbol == 'f'){
//		return 7513;
//	}else if (backgroundSymbol == 'x'){
//		return 7508;
//	}else if (backgroundSymbol == 's'){
//		return 7509;
//	}else if (backgroundSymbol == 'd'){
//		return 7510;
//	}else{
//		return 7501;
//	}
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

	loadGroups(thisGroupContainer, thisMapInfo, thisField);

	addItemsToField(thisMapInfo, thisField);

	makeTransitSpaces(transitMap, directory, thisField, player);

	moveIndividualSpace(thisField,player, player->playerCharacter->x, player->playerCharacter->y);

	free(fullMapName);

	return thisField;
}

destroyField(field * thisField, individual * player){
	int i,j;

	if(thisField->thisFieldInventory != NULL){
		free(thisField->thisFieldInventory);
	}

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

int addItemToField(field * thisField, item * thisItem){
	int i;
	if(thisField->thisFieldInventory->inventorySize >= thisField->thisFieldInventory->MAX_ITEMS){
		return 0;
	}

	for(i = 0; i < thisField->thisFieldInventory->MAX_ITEMS; i++){
		if(thisField->thisFieldInventory->inventoryArr[i] == NULL){
			thisField->thisFieldInventory->inventoryArr[i] = thisItem;
			thisField->thisFieldInventory->inventorySize++;

			return 1;
		}
	}

	return 0;
}

void removeItemFromField(field * thisField, item * thisItem){
	int i;
	mapInfo * thisMap = getMapInfoFromRegistry(thisField->id);

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

void populateCurrentSpaceInventory(field * thisField, individual * thisIndividual){
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

				if(tmpItem->itemCharacter->x == thisIndividual->playerCharacter->x &&
						tmpItem->itemCharacter->y == thisIndividual->playerCharacter->y){
					addItemToInventory(thisField->currentSpaceInventory, tmpItem);
				}

				if(itemsPassed == thisField->thisFieldInventory->inventorySize){
					break;
				}
			}
		}
	}
}

field* initField(char* fieldFileName){
	field* thisField = malloc(sizeof(field));
	FILE * fp = fopen(fieldFileName, "r");
	char line[80];
	int init_y = 0, init_x = 0, xIndex, i, j;

	//used to get rid of the first 2 lines
	fgets(line,80,fp);

	//field ID
	fgets(line, 80, fp);
	thisField->id = atoi(line);

	//init field to null
	for(i = 0; i < 100; i++){
		for(j = 0; j < 100; j++){
			thisField->grid[i][j] = NULL;
		}
	}

	while(fgets(line,80,fp) != NULL){
		init_x = 0;
		for(xIndex = 0; xIndex < strlen(line); xIndex+=2){
			char spaceType = line[xIndex];
			char direction = line[xIndex+1];

			space* newSpace = malloc(sizeof(space));
			newSpace->currentIndividual = NULL;//malloc(sizeof(individual));
			newSpace->thisTransitInfo = NULL;// malloc(sizeof(transitInfo));

			character* backgroundCharacter = malloc(sizeof(character));
			backgroundCharacter->x = init_x;
			backgroundCharacter->y = init_y;
			backgroundCharacter->xOff = 0;
			backgroundCharacter->yOff = 0;
			backgroundCharacter->thisAnimationContainer = initAnimationContainer();
			backgroundCharacter->thisAnimationContainer->animationsEnabled = 1;
			backgroundCharacter->thisAnimationContainer->defaultAnimation = 0;

			if(spaceType == 'c'
				|| spaceType == '-'
				|| spaceType == '='
				|| spaceType == 'r'
				|| spaceType == 'u'
				|| spaceType == 'o'
				|| spaceType == 's'
				|| spaceType == 'x'
				|| spaceType == 'w'
				|| spaceType == 't'){
				newSpace->isPassable = 0;
			}else{
				newSpace->isPassable = 1;
			}

			if(spaceType == 't' || spaceType == 's' ||  spaceType == 'x'){
				newSpace->canSeeThrough = 0;
				newSpace->canAttackThrough = 0;
			}else{
				newSpace->canSeeThrough = 1;
				newSpace->canAttackThrough = 1;
			}

			if (direction == '>') {
				backgroundCharacter->direction = 3;
			}else if (direction == 'v'){
				backgroundCharacter->direction = 2;
			}else if (direction == '<'){
				backgroundCharacter->direction = 1;
			}else{
				backgroundCharacter->direction = 0;
			}

			animation * backgroundAnimation = generateBackground(spaceType);
			addAnimationToContainer(backgroundCharacter->thisAnimationContainer, backgroundAnimation);

			newSpace->background = backgroundCharacter;
			thisField->grid[init_x][init_y] = newSpace;
			init_x++;
		}

		init_y++;

	}

	fclose(fp);


	thisField->thisFieldInventory = malloc(sizeof(fieldInventory));
	thisField->thisFieldInventory->inventorySize = 0;
	thisField->thisFieldInventory->MAX_ITEMS = 1000;

	thisField->currentSpaceInventory = malloc(sizeof(inventory));
	thisField->currentSpaceInventory->inventorySize = 0;
	thisField->currentSpaceInventory->MAX_ITEMS = 40;

	for(i = 0; i < 1000; i++){
		thisField->thisFieldInventory->inventoryArr[i] = NULL;
		if(i < 40){
			thisField->currentSpaceInventory->inventoryArr[i] = NULL;
		}
	}

	thisField->totalX = init_x;
	thisField->totalY = init_y;

	return thisField;
}

void updateFieldGraphics(HDC hdc, HDC hdcBuffer, field* this_field){
	int x, y, i;

	for (y = 0; y < this_field->totalY; y++) {
		for (x = 0; x < this_field->totalX; x++) {
			character * tmpBackground = this_field->grid[x][y]->background;

			if(tmpBackground->direction > 0){
				rotateAnimationFrames(hdc, hdcBuffer, tmpBackground->thisAnimationContainer->animations[tmpBackground->thisAnimationContainer->currentAnimation], tmpBackground->direction);
			}
		}
	}
}

void drawField(HDC hdc, HDC hdcBuffer, field* this_field, shiftData * viewShift){
	int x, y, i;

	for (y = 0; y < this_field->totalY; y++) {
		for (x = 0; x < this_field->totalX; x++) {
			character * tmpBackground = this_field->grid[x][y]->background;
			updateAnimation(tmpBackground);

			drawCharacterAnimation(hdc, hdcBuffer, tmpBackground, viewShift, 0);
		}
	}
}

void drawItemsFromField(HDC hdc, HDC hdcBuffer, fieldInventory * thisFieldInventory, shiftData * viewShift){
	int i, numDrawn = 0;
	for(i = 0; i < 1000; i++){

		if(thisFieldInventory->inventoryArr[i] != NULL){
			drawCharacterAnimation(hdc, hdcBuffer, thisFieldInventory->inventoryArr[i]->itemCharacter, viewShift);
			numDrawn++;
		}

		//all items drawn
		if(numDrawn == thisFieldInventory->inventorySize){
			break;
		}

	}
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

void drawRotatedBackground(HDC hdc, HDC hdcBuffer, character * backgroundCharacter, shiftData * viewShift){

	int direction = backgroundCharacter->direction;
	float cosine = (float) cos(direction*M_PI/2.0);
	float sine = (float) sin(direction*M_PI/2.0);

	// Compute dimensions of the resulting bitmap
	// First get the coordinates of the 3 corners other than origin
	int x1 = (int) (100 * sine);
	int y1 = (int) (100 * cosine);
	int x2 = (int) (100 * cosine + 100 * sine);
	int y2 = (int) (100 * cosine - 100 * sine);
	int x3 = (int) (100 * cosine);
	int y3 = (int) (-100 * sine);

	int minx = min(0, min(x1, min(x2,x3)));
	int miny = min(0, min(y1, min(y2,y3)));
	int maxx = max(0, max(x1, max(x2,x3)));
	int maxy = max(0, max(y1, max(y2,y3)));

	SetGraphicsMode(hdcBuffer, GM_ADVANCED);
	XFORM xForm;
	xForm.eM11 = cosine;
	xForm.eM12 = -sine;
	xForm.eM21 = sine;
	xForm.eM22 = cosine;
	xForm.eDx = (float) -minx;
	xForm.eDy = (float) -miny;

	if (!SetWorldTransform(hdcBuffer, &xForm)) {
		printf("setworldtransform failed");
	}

//	SelectObject(hdcMem, backgroundCharacter->image);

	int xMod = calcXMod(direction, backgroundCharacter, viewShift);
	int yMod = calcYMod(direction, backgroundCharacter, viewShift);

	drawRotatedBackgroundByPixel(hdc, hdcBuffer, backgroundCharacter, viewShift, xMod, yMod, 0);
//drawUnboundAnimationByPixels(hdc, hdcBuffer, backgroundCharacter, viewShift, xMod, yMod, 0);
//	BitBlt(hdcBuffer,
//			xMod,//*(backgroundCharacter->x - (viewShift->xShift) * 40),
//			yMod,//*(backgroundCharacter->y - (viewShift->yShift) * 40),
//			backgroundCharacter->width, backgroundCharacter->height, hdcMem, 0,
//			0,
//			SRCCOPY);

	//set world transform back to normal
	xForm.eM11 = 1;
	xForm.eM12 = 0;
	xForm.eM21 = 0;
	xForm.eM22 = 1;
	xForm.eDx = 0;
	xForm.eDy = 0;
	if (!SetWorldTransform(hdcBuffer, &xForm)) {
		printf("setworldtransform failed");
	}

}

int calcXMod(int direction, character * backgroundCharacter, shiftData * viewShift){
	if(direction == 1){
		return -1*((backgroundCharacter->y*52 - viewShift->yShift*52) - 25);
	}else if(direction == 2){
		return -1*((backgroundCharacter->x*52-viewShift->xShift*52) - 25);
	}
	else{
		return (backgroundCharacter->y*52 -viewShift->yShift*52) - 25;
	}
}

int calcYMod(int direction, character * backgroundCharacter, shiftData * viewShift){
	if(direction == 1){
		return (backgroundCharacter->x*52-viewShift->xShift*52) - 25;
	}else if(direction == 2){
		return -1*((backgroundCharacter->y*52-viewShift->yShift*52) - 25);
	}else{
		return -1*((backgroundCharacter->x*52-viewShift->xShift*52) - 25);
	}
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

	return newMapInfo;
}

void parseMapIndividualsFromLine(mapInfo * thisMapInfo, char * line){
	char * value;

	if(line == NULL){
		return;
	}

	value = strtok(line, ",");

	if(value == NULL){
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

	if(value == NULL){
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

void createMapInfoFromLine(mapInfo * thisMapInfo, char * line){
	char * strtok_save_pointer;
	char * value = strtok_r(line,";",&strtok_save_pointer);
	thisMapInfo->id = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	strcpy(thisMapInfo->mapName, value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	parseMapIndividualsFromLine(thisMapInfo, value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	parseMapItemsFromLine(thisMapInfo, value);
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
