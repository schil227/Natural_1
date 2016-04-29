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
	if((thisField->grid[x][y])->currentIndividual != NULL){
		(thisField->grid[x][y])->currentIndividual = NULL;
		return 1;
	}

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
	//space * currentSpace = getSpaceFromField(thisField,inX,inY);
//	space * newSpace;
	int newX = thisIndividual->playerCharacter->x + xMoveChange(direction);
	int newY = thisIndividual->playerCharacter->y + yMoveChange(direction);

	//check if in bounds, and newSpace exists
	if(!(newX >= 0 && newX < thisField->totalX && newY >=0 && newY < thisField->totalY)){
		return 0;
	}

	//space exists, wont be null
//	newSpace = getSpaceFromField(thisField,newX,newY);

	//can the individual go to this space?
	if(getSpaceFromField(thisField,newX,newY)->isPassable && getSpaceFromField(thisField,newX,newY)->currentIndividual == NULL){
		getSpaceFromField(thisField,inX,inY)->currentIndividual = NULL;
		getSpaceFromField(thisField,newX,newY)->currentIndividual = thisIndividual;
		thisIndividual->playerCharacter->x = newX;
		thisIndividual->playerCharacter->y = newY;
		return 1;
	}else{
//		printf("is not passable");
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

int tryAttackEnemies(individualGroup * enemies, individual * player, field * thisField, int x, int y){
	int i, j, enemiesPassed = 0, numTimesToAttack = 1;

	numTimesToAttack += calcExtraTimesToAttack(player);

	for (i = 0; i < enemies->MAX_INDIVIDUALS; i++) {

		individual * tmpEnemy = enemies->individuals[i];

		if(tmpEnemy != NULL){
			enemiesPassed++;
			if (tmpEnemy->playerCharacter->x == x && tmpEnemy->playerCharacter->y == y && individualWithinRange(player, tmpEnemy)) {

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

void useAbilityOnIndividualsInAOERange(individual * thisIndividual, individualGroup * thisIndividualGroup, individual * player, individualGroup * npcs, individualGroup * enemies, field * thisField, int x, int y){
	int aoe = 0,  minX, maxX, minY, maxY;
	ability * tmpAbility = thisIndividual->activeAbilities->selectedAbility;

	decreaseMana(thisIndividual, tmpAbility->totalManaCost);

	if(processCasterOnlyEffects(thisIndividual, tmpAbility)){
		if(thisIndividualGroup != NULL){
			deleteIndividiaulFromGroup(thisIndividualGroup, thisIndividual);
		}

		removeIndividualFromField(thisField, thisIndividual->playerCharacter->x, thisIndividual->playerCharacter->y);
		triggerEventOnDeath(thisIndividual->ID);
		removeFromExistance(thisIndividual->ID);
	}

	if(tmpAbility->aoeEnabled){
		aoe = tmpAbility->aoe->effectAndManaArray[tmpAbility->aoe->selectedIndex]->effectMagnitude;
	}

	minX = x - aoe;
	maxX = x + aoe;
	minY = y - aoe;
	maxY = y + aoe;

	preprocessIndividalGroupsInAOE(thisIndividual, enemies, thisField, minX, maxX, minY, maxY);
	preprocessIndividalGroupsInAOE(thisIndividual, npcs, thisField, minX, maxX, minY, maxY);

	useAbilityOnIndividualGroupsInAOE(thisIndividual, enemies, thisField, minX, maxX, minY, maxY);
	useAbilityOnIndividualGroupsInAOE(thisIndividual, npcs, thisField, minX, maxX, minY, maxY);

	if(player->hp > 0 && player->playerCharacter->x >= minX && player->playerCharacter->x <= maxX &&
		player->playerCharacter->y >= minY && player->playerCharacter->y <= maxY ){

		if(tmpAbility->type == 't'){
			if(attackIndividualWithAbility(thisIndividual, player)){
				removeIndividualFromField(thisField, player->playerCharacter->x, player->playerCharacter->y);
			}
		}else if (tmpAbility->type == 'd'){
			//add duration ability logic here
			useDurationAbilityOnIndividual(player, tmpAbility);
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
					if(thisIndividual->activeAbilities->selectedAbility->type == 't'){
						triggerEventOnAttack(tmp->ID);

						if(!individualInGroup(tmp, thisGroup)){
							individualsPassed--;
						}
					}else if (thisIndividual->activeAbilities->selectedAbility->type == 'd'){
						if(abilityIsHarmful(thisIndividual->activeAbilities->selectedAbility)){
							triggerEventOnAttack(tmp->ID);

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
					if(attackIndividualWithAbility(thisIndividual, tmp)){
						deleteIndividiaulFromGroup(thisGroup, tmp);
						removeIndividualFromField(thisField, tmp->playerCharacter->x, tmp->playerCharacter->y);
						removeFromExistance(tmp->ID);
					}
				}else if (thisIndividual->activeAbilities->selectedAbility->type == 'd'){

					//add duration ability logic here
					if(useDurationAbilityOnIndividual(tmp, thisIndividual->activeAbilities->selectedAbility)){
						deleteIndividiaulFromGroup(thisGroup, tmp);
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
/*
 * take in a character, output the corrosponding background name (string)
*/
int generateBackground(char backgroundSymbol){
//	printf("backgroundChar: %c\n", backgroundSymbol);

	if(backgroundSymbol == 'c'){
//		printf("Found  a C!\n");
		return 2502;
	}else if (backgroundSymbol == 'g'){
		return 2501;
	}else if (backgroundSymbol == '-'){
		return 2511;
	}else if (backgroundSymbol == '='){
		return 2512;
	}else if (backgroundSymbol == 'r'){
		return 2513;
	}else if (backgroundSymbol == 'u'){
		return 2514;
	}else if (backgroundSymbol == 'o'){
		return 2515;
	}else if (backgroundSymbol == 'w'){
		return 2521;
	}else if (backgroundSymbol == 't'){
		return 2561;
	}else if (backgroundSymbol == 'p'){
		return 2541;
	}else if (backgroundSymbol == '\\'){
		return 2542;
	}else if (backgroundSymbol == 'f'){
		return 2551;
	}else if (backgroundSymbol == 'x'){
		return 2531;
	}else if (backgroundSymbol == 's'){
		return 2532;
	}else if (backgroundSymbol == 'd'){
		return 2533;
	}else{
		return 2501;
	}
}

field * loadMap(char * mapName, char* directory, individual * player, individualGroup* enemies, individualGroup * npcs){

	char transitMap[80], enemyMap[80], enemyItemMap[80], npcMap[80], npcItemMap[80], fieldItemMap[80], dialog[80];
	char * fullMapName = appendStrings(directory, mapName);
//	fullMapName[strlen(fullMapName)-1] = '\0'; //remove /n at end

	FILE * fp = fopen(fullMapName, "r");

	//transit map filename
	fgets(transitMap,80,fp);

	//enemy filename
	fgets(enemyMap,80,fp);

	//enemy item filename
	fgets(enemyItemMap,80,fp);

	//enemy filename
	fgets(npcMap,80,fp);

	//enemy item filename
	fgets(npcItemMap,80,fp);

	//field item filename
	fgets(fieldItemMap,80,fp);

	//dialog filename
	fgets(dialog, 80,fp);

	fclose(fp);

	loadGroup(enemies, enemyMap, directory);
	loadGroup(npcs, npcMap, directory);
	loadDialog(dialog, directory);

	field* thisField = initField(fullMapName);

	loadFieldItems(thisField, fieldItemMap, directory);

	makeTransitSpaces(transitMap, directory, thisField, player);

	moveIndividualSpace(thisField,player, player->playerCharacter->x, player->playerCharacter->y);
	setGroupToField(thisField, enemies);
	setGroupToField(thisField, npcs);

	free(fullMapName);

	return thisField;
}

destroyField(field * thisField, individual * player){
	int i,j, itemsPassed;

	if(thisField->thisFieldInventory != NULL){
		itemsPassed = 0;
//		for(i = 0; i < 1000; i++){
//			if(thisField->thisFieldInventory->inventoryArr[i] != NULL){
//				destroyItem(thisField->thisFieldInventory->inventoryArr[i]);
//				itemsPassed++;
//			}
//			if(itemsPassed >= thisField->thisFieldInventory->inventorySize){
//				break;
//			}
//		}
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

int addItemToField(fieldInventory * thisFieldInventory, item * thisItem){
	int i;
	if(thisFieldInventory->inventorySize >= 1000){
		return 0;
	}
	for(i = 0; i < 1000; i++){
		if(thisFieldInventory->inventoryArr[i] == NULL){
			thisFieldInventory->inventoryArr[i] = thisItem;
			thisFieldInventory->inventorySize++;
			return 1;
		}
	}

	return 0;
}

void removeItemFromField(fieldInventory * thisFieldInventory, item * thisItem){
	int i;

	for(i = 0; i < 1000; i++){
		if(thisFieldInventory->inventoryArr[i] == thisItem){
			removeFromExistance(thisFieldInventory->inventoryArr[i]->ID);
			thisFieldInventory->inventoryArr[i] = NULL;
			break;
		}
	}

}

int attemptGetItemFromField(field * thisField, individual * thisIndividual){
	int i, itemsProcessed = 0;
	int itemX = thisIndividual->playerCharacter->x;
	int itemY = thisIndividual->playerCharacter->y;
	item * theItem = NULL;

	for(i = 0; i < 1000; i++){
		if(thisField->thisFieldInventory->inventoryArr[i] != NULL){
			theItem = thisField->thisFieldInventory->inventoryArr[i];

			if(theItem->itemCharacter->x == itemX && theItem->itemCharacter->y == itemY){
				removeItemFromField(thisField->thisFieldInventory, theItem);
				addItemToIndividual(thisIndividual->backpack, theItem);
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

field* initField(char* fieldFileName){
	field* thisField = malloc(sizeof(field));
	FILE * fp = fopen(fieldFileName, "r");
	char line[80];
	int init_y = 0, init_x = 0, xIndex, i, j;

	//used to get rid of the first 7 lines which are file names
	fgets(line,80,fp);
	fgets(line,80,fp);
	fgets(line,80,fp);
	fgets(line,80,fp);
	fgets(line,80,fp);
	fgets(line,80,fp);
	fgets(line,80,fp);

	//init field to null
	for(i = 0; i < 100; i++){
		for(j = 0; j < 100; j++){
			thisField->grid[i][j] = NULL;
		}
	}

	while(fgets(line,80,fp) != NULL){
		init_x = 0;
		for(xIndex = 0; xIndex < strlen(line); xIndex+=2){

			space* newSpace = malloc(sizeof(space));
			newSpace->currentIndividual = NULL;//malloc(sizeof(individual));
			newSpace->thisTransitInfo = NULL;// malloc(sizeof(transitInfo));
			character* backgroundCharacter = malloc(sizeof(character));
			char currentChar = line[xIndex];

			//initialize background
			backgroundCharacter->nameLength = 0;
			backgroundCharacter->width = 40;
			backgroundCharacter->height = 40;
			backgroundCharacter->x = init_x;
			backgroundCharacter->y = init_y;
			backgroundCharacter->imageID = generateBackground(currentChar);
			if(currentChar == 'c'
				|| currentChar == '-'
				|| currentChar == '='
				|| currentChar == 'r'
				|| currentChar == 'u'
				|| currentChar == 'o'
				|| currentChar == 's'
				|| currentChar == 'x'
				|| currentChar == 'w'
				|| currentChar == 't'){
				newSpace->isPassable = 0;
			}else{
				newSpace->isPassable = 1;
			}

			currentChar = line[xIndex+1];
			if (currentChar == '>') {
				backgroundCharacter->direction = 3;
			}else if (currentChar == 'v'){
				backgroundCharacter->direction = 2;
			}else if (currentChar == '<'){
				backgroundCharacter->direction = 1;
			}else{
				backgroundCharacter->direction = 0;
			}

			newSpace->background = backgroundCharacter;
			thisField->grid[init_x][init_y] = newSpace;
			init_x++;
		}

		init_y++;

	}

	fclose(fp);


	thisField->thisFieldInventory = malloc(sizeof(fieldInventory));
	thisField->thisFieldInventory->inventorySize = 0;

	for(i = 0; i < 1000; i++){
		thisField->thisFieldInventory->inventoryArr[i] = NULL;
	}

	thisField->totalX = init_x;
	thisField->totalY = init_y;

	return thisField;
}

void drawField(HDC hdc, HDC hdcBuffer, field* this_field, shiftData * viewShift){

	HDC hdcMem = CreateCompatibleDC(hdc);
	int x, y, i;

	for (y = 0; y < this_field->totalY; y++) {
		for (x = 0; x < this_field->totalX; x++) {

			character * tmpBackground = this_field->grid[x][y]->background;

			if(tmpBackground->direction != 0){
				drawRotatedBackground(hdcMem, hdcBuffer, tmpBackground, viewShift);
			}else{

			SelectObject(hdcMem, tmpBackground->image);

			BitBlt(hdcBuffer,
				tmpBackground->x*40 - (viewShift->xShift)*40,
				tmpBackground->y*40 - (viewShift->yShift)*40,
				tmpBackground->width,
				tmpBackground->height, hdcMem, 0, 0,
				SRCCOPY);
			}

		}
	}

	DeleteDC(hdcMem);
}

void drawItemsFromField(HDC hdc, HDC hdcBuffer, fieldInventory * thisFieldInventory, shiftData * viewShift){
	int i, numDrawn = 0;
	for(i = 0; i < 1000; i++){

		if(thisFieldInventory->inventoryArr[i] != NULL){
			drawCharacter(hdc, hdcBuffer, thisFieldInventory->inventoryArr[i]->itemCharacter, viewShift);
			numDrawn++;
		}

		//all items drawn
		if(numDrawn == thisFieldInventory->inventorySize){
			break;
		}

	}
}

void drawRotatedBackground(HDC hdcMem, HDC hdcBuffer, character * backgroundCharacter, shiftData * viewShift){

	int direction = backgroundCharacter->direction;
	float cosine = (float) cos(direction*M_PI/2.0);
	float sine = (float) sin(direction*M_PI/2.0);

	// Compute dimensions of the resulting bitmap
	// First get the coordinates of the 3 corners other than origin
	int x1 = (int) (40 * sine);
	int y1 = (int) (40 * cosine);
	int x2 = (int) (40 * cosine + 40 * sine);
	int y2 = (int) (40 * cosine - 40 * sine);
	int x3 = (int) (40 * cosine);
	int y3 = (int) (-40 * sine);

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

	SelectObject(hdcMem, backgroundCharacter->image);

	int xMod = calcXMod(direction, backgroundCharacter, viewShift);
	int yMod = calcYMod(direction, backgroundCharacter, viewShift);

	BitBlt(hdcBuffer,
			xMod,//*(backgroundCharacter->x - (viewShift->xShift) * 40),
			yMod,//*(backgroundCharacter->y - (viewShift->yShift) * 40),
			backgroundCharacter->width, backgroundCharacter->height, hdcMem, 0,
			0,
			SRCCOPY);

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
		return -1*(backgroundCharacter->y*40 - viewShift->yShift*40);
	}else if(direction == 2){
		return -1*(backgroundCharacter->x*40-viewShift->xShift*40);
	}
	else{
		return (backgroundCharacter->y*40 -viewShift->yShift*40);
	}
}

int calcYMod(int direction, character * backgroundCharacter, shiftData * viewShift){
	if(direction == 1){
		return (backgroundCharacter->x*40-viewShift->xShift*40);
	}else if(direction == 2){
		return -1*(backgroundCharacter->y*40-viewShift->yShift*40);
	}else{
		return -1*(backgroundCharacter->x*40-viewShift->xShift*40);
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
