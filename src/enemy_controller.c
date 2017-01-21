/*
 * enemy_controller.c
 *
 *  Created on: May 4, 2015
 *      Author: Adrian
 */
#include"./headers/enemy_controller_pub_methods.h"
#include<stdio.h>

node * createNewNode(int pathLength, int x, int y){
	node* thisNode = malloc(sizeof(node));
	thisNode->pathLength = pathLength;
	thisNode->x = x;
	thisNode->y = y;
	thisNode->isFinalPathNode = 0;
	thisNode->previousNode = NULL;
	return thisNode;
}

int containsNode(int x, int y, node ** nodeList){
	int i = 0;
	while(nodeList[i] != NULL){
		node * tmpNode = nodeList[i];
//		printf("containsNode: x:%d y:%d, %d,%d \n",tmpNode->x,  tmpNode->y, x, y);
		if(tmpNode->x == x && tmpNode->y == y){
//			printf("done, in \n");
			return 1;
		}
		i++;
	}
//	printf("done, not in\n");
	return 0;
}

//TODO make this less dumb, keep track of the size in an object
void addNodeToList(node * newNode, node ** nodeList){
	int i = 0;
	while(nodeList[i] != NULL){
		i++;
	}
//	printf("\n");
	nodeList[i] = newNode;

}

node ** getNewActiveNodes(node * parentNode, node ** allNodes, int targetX, int targetY, individual * thisIndividual, field * thisField){
	node** newActiveNodes = malloc(sizeof(node*)*9);
	int x,dx,dy,newX,newY, index=0;
	for(x = 0; x < 9; x++){
		newActiveNodes[x] = NULL;
	}

	if (parentNode->pathLength < 10) {
		for (dx = -1; dx < 2; dx++) {
			for (dy = -1; dy < 2; dy++) {
				newX = parentNode->x + dx;
				newY = parentNode->y + dy;

				space * tmpSpace = getSpaceFromField(thisField, newX, newY);

				if(tmpSpace == NULL || containsNode(newX, newY, allNodes)){
					continue;
				}

				if (tmpSpace->isPassable && (thisIndividual == NULL || tmpSpace->currentIndividual == NULL || isAlly(thisIndividual, tmpSpace->currentIndividual) || (newX == targetX && newY == targetY))){
					node * newNode = createNewNode(parentNode->pathLength + 1, newX, newY);
					newNode->previousNode = parentNode;
					addNodeToList(newNode, allNodes);
					newActiveNodes[index] = newNode;
					index++;
				}
			}
		}
	}

	newActiveNodes[index] = NULL;
	return newActiveNodes;
}

cordArr * getUniquePassableCordsSurroundingCord(individual * thisIndividual, field * thisField, cord * thisCord, cordArr * totalCords, int nonAlliesPassable){
	int dx,dy, newX, newY;
	cordArr * toReturn = initCordArr();

	for (dx = -1; dx < 2; dx++) {
		for (dy = -1; dy < 2; dy++) {
			newX = thisCord->x + dx;
			newY = thisCord->y + dy;

			if(cordArrContainsCoordinates(totalCords, newX, newY) || (thisCord->x == newX && thisCord->y == newY)){
				continue;
			}

			space * tmpSpace = getSpaceFromField(thisField, newX, newY);

			if(tmpSpace != NULL && tmpSpace->isPassable && (nonAlliesPassable || (thisIndividual == NULL || tmpSpace->currentIndividual == NULL || isAlly(thisIndividual, tmpSpace->currentIndividual)))){
				cord * newCord = malloc(sizeof(cord));
				newCord->x = newX;
				newCord->y = newY;

				addCordIfUnique(toReturn, newCord);
			}
		}
	}

	return toReturn;
}

node* pathFind(int targetX, int targetY, node ** allNodes, node ** activeNodes, individual * thisIndividual, field * thisField){
	node * newActiveNodes[100];
	int x, i=0, numNewActiveNodes = 0;
	for(x = 0; x < 100; x++){
		newActiveNodes[x] = NULL;
	}

	while(activeNodes[i] != NULL){
		int j = 0;
		node ** newNodes = getNewActiveNodes(activeNodes[i], allNodes, targetX, targetY, thisIndividual, thisField);

		while(newNodes[j] != NULL){
			if(newNodes[j]->x == targetX && newNodes[j]->y == targetY){ //found target space
				node * foundTargetSpace = newNodes[j];
				free(newNodes);

				return (foundTargetSpace);
			}

			numNewActiveNodes++;
			addNodeToList(newNodes[j], newActiveNodes);
			j++;
		}

		free(newNodes);
		i++;
	}

	if(numNewActiveNodes == 0){
		return createNewNode(-1,-1,-1);
	}

	return pathFind(targetX, targetY, allNodes, newActiveNodes, thisIndividual, thisField);
}

nodeArr * getFullNodePath(field * thisField, individual * thisIndividual, int thisX, int thisY, int targetX, int targetY){
	int i, size = 0;
	node * startingNode = createNewNode(0,thisX,thisY);
	startingNode->previousNode = NULL;
	node * allNodes[300];

	node * activeNodes[100];
	for(i = 0; i < 300; i++){
		if(i < 100){
			activeNodes[i] = NULL;
		}
		allNodes[i] = NULL;
	}
	allNodes[0] = startingNode;
	activeNodes[0] = startingNode;

	node * endNode = pathFind(targetX, targetY, allNodes, activeNodes, thisIndividual, thisField);

	nodeArr * resultArr = malloc(sizeof(nodeArr));
	resultArr->size = 0;

	for(i = 0; i < 20; i++){
		resultArr->nodeArray[i] = NULL;
	}

	if(endNode->pathLength != -1){

		node * tmpNode = endNode;
		while(tmpNode->previousNode != NULL){
			tmpNode->isFinalPathNode = 1;
			tmpNode = tmpNode->previousNode;
			size++;
		}

		resultArr->size = size;

		tmpNode = endNode;

		for(size=resultArr->size-1; size >=0; size--){
			resultArr->nodeArray[size] = tmpNode;
			tmpNode = (node*) tmpNode->previousNode;
		}

	}else{
		free(endNode);
	}

	i = 0;
	while(allNodes[i] != NULL){
		if(allNodes[i]->isFinalPathNode != 1){
			free(allNodes[i]);
		}
		i++;
	}

	return resultArr;
}

void initializeArr(node ** nodeArr, int size){
	int i;
	for(i = 0; i < size; i++){
		nodeArr[size] = NULL;
	}
}

int spaceIsTaken(node * thisNode, field * thisField){
	int a = spaceIsAvailable(thisField, thisNode->x, thisNode->y);
//	printf("a:%d\n",a);
	if(a == 0){
		return 0;
	}else{
		return 1;
	}
}

node * shallowCloneNode(node * thisNode){
	node * newNode = malloc(sizeof(node));
	newNode->pathLength = thisNode->pathLength;
	newNode->x = thisNode->x;
	newNode->y = thisNode->y;
	return newNode;
}

node * findOpenNode(node * endNode, node ** activeNodes, individual * thisIndividual, int moveRange, int distanceFromLastNode, field * thisField, node ** allNodes, nodeArr * nodePath, int debugDepth){
	int i;
	node * newActiveNodes[300];

	if(debugDepth > 100){
		cwrite("!! FIND OPEN NODE FAILURE !!");
	}

	for(i = 0; i < 300; i++){
		newActiveNodes[i] = NULL;
	}

	i = 0;

	while(activeNodes[i] != NULL){
		if( !spaceIsTaken(activeNodes[i], thisField)){
			return activeNodes[i];
		}

		if(moveRange > 0){
			node ** tmpNodes = getNewActiveNodes(activeNodes[i], allNodes, -1, -1, thisIndividual, thisField); // filters out blocked nodes and nodes in allNodes
			int j = 0;

			while(tmpNodes[j] != NULL){
				if(!spaceIsTaken(tmpNodes[j], thisField)){
					addNodeToList(tmpNodes[j], newActiveNodes);
				}
				j++;
			}

			free(tmpNodes);
		}

		i++;
	}


	if(newActiveNodes[0] == NULL){
		if( endNode->previousNode == NULL){
			return NULL;
		}else{
			node * tmpNode = shallowCloneNode(endNode->previousNode);

			if(containsNode(tmpNode->x, tmpNode->y, allNodes)){
				return NULL;

			}else{
				addNodeToList(tmpNode, newActiveNodes);

				addNodeToList(tmpNode,allNodes);
				return findOpenNode( endNode->previousNode, newActiveNodes, thisIndividual, distanceFromLastNode, distanceFromLastNode +1, thisField, allNodes, nodePath, debugDepth++);
			}
		}
	}else{
		return findOpenNode(endNode, newActiveNodes, thisIndividual, moveRange-1, distanceFromLastNode, thisField, allNodes,nodePath, debugDepth++);
	}

}

nodeArr * processPath(field * thisField, nodeArr * nodePath, individual * thisIndividual){
	int i;
	int nodeIndex = max(min(nodePath->size, getAttributeSum(thisIndividual,"mvmt")), 0);


	if(nodeIndex > 0){ //going somewhere
		node * endNode = nodePath->nodeArray[nodeIndex-1];
		node * allNodes[300];
		node * activeNodes[300];
		int i;
		for(i = 0; i < 300; i++){
			activeNodes[i] = NULL;
			allNodes[i] = NULL;
		}

		node * endNodeCopy = shallowCloneNode(endNode);
		addNodeToList(endNodeCopy, activeNodes);
		addNodeToList(endNodeCopy, allNodes);

		node * targetNode = findOpenNode(endNode, activeNodes, thisIndividual, 0, 1, thisField, allNodes, nodePath, 0);

		if(targetNode != NULL){
			int targetx, targety;
			targetx = targetNode->x;
			targety = targetNode->y;

			for (i = 0; i < 300; i++) {
//				if(activeNodes[i] != NULL){
//					free(activeNodes[i]);
//				}
				if(allNodes[i] != NULL){
					free(allNodes[i]);
				}
			}

			return getFullNodePath(thisField, thisIndividual, thisIndividual->playerCharacter->x, thisIndividual->playerCharacter->y, targetx, targety);
		} else {
			printf("returning null\n");

			for (i = 0; i < 300; i++) {
//				if(activeNodes[i] != NULL){
//					free(activeNodes[i]);
//				}
				if(allNodes[i] != NULL){
					free(allNodes[i]);
				}
			}

			nodeArr * nullNode = malloc(sizeof(nodeArr));
			nullNode->size = 0;
			return nullNode;
		}
	}

	nodeArr * nullNode = malloc(sizeof(nodeArr));
	nullNode->size = 0;

	return nullNode;
}

nodeArr * getSpaceClosestToSpace(field * thisField, individual * thisIndividual, int x, int y){
	nodeArr * resultArr = getFullNodePath(thisField, thisIndividual, thisIndividual->playerCharacter->x, thisIndividual->playerCharacter->y, x, y);

	nodeArr * actualPath = processPath(thisField, resultArr, thisIndividual);
	destroyNodeArr(resultArr);

	return actualPath;
}

nodeArr * getSpaceClosestToPlayer(field * thisField, individual * thisIndividual, individual * targetIndividual){
	nodeArr * resultArr = getFullNodePath(thisField, thisIndividual, thisIndividual->playerCharacter->x, thisIndividual->playerCharacter->y, targetIndividual->playerCharacter->x, targetIndividual->playerCharacter->y);

	nodeArr * actualPath = processPath(thisField, resultArr, thisIndividual);
	destroyNodeArr(resultArr);

	return actualPath;
}

//Only used in legacy Testing - goes toward the player, attacks if in range.
void testEnemyAction( individual * enemy, field * thisField, individual * player){
	printf("starting: %s\n", enemy->name);
	nodeArr * resultArr = getSpaceClosestToPlayer(thisField, enemy, player);
	int i;
	int size = resultArr->size;
	printf("moving enemy. size: %d\n",size);
	if(size > 0){
		printf(enemy->name);
		printf(" starting space:[%d,%d] \n", enemy->playerCharacter->x, enemy->playerCharacter->y);
		removeIndividualFromField(thisField,enemy->playerCharacter->x, enemy->playerCharacter->y);
	}
	for(i = 0; i < size; i++){

		if(i+1 == size){
			printf(enemy->name);
			printf(" at target space:[%d,%d] \n", resultArr->nodeArray[i]->x, resultArr->nodeArray[i]->y);
			printf("space is taken:%d\n",spaceIsAvailable(thisField,resultArr->nodeArray[i]->x, resultArr->nodeArray[i]->y));
			if(moveIndividualSpace(thisField,enemy, resultArr->nodeArray[i]->x, resultArr->nodeArray[i]->y) == 0){
				printf("Individiual at space!");
			}else{
				printf(enemy->name);
				printf(": %d,%d is at space: %p\n", enemy->playerCharacter->x,enemy->playerCharacter->y, getIndividualAddressFromField(thisField,resultArr->nodeArray[i]->x, resultArr->nodeArray[i]->y));
				printf("space is taken:%d\n",spaceIsAvailable(thisField,resultArr->nodeArray[i]->x, resultArr->nodeArray[i]->y));
			}
		}else{
			printf("moving to: [%d,%d]\n",resultArr->nodeArray[i]->x,resultArr->nodeArray[i]->y);
//			setIndividualTmpSpace(thisField,enemy, resultArr->nodeArray[i]->x, resultArr->nodeArray[i]->y);
		}

	}

	destroyNodeArr(resultArr);

	printf("\n");

	attackIfInRange(enemy,player);
}

void populateMoveNodeMeta(moveNodeMeta * thisMoveNodeMeta, nodeArr * thisNodeArr){
	int i;
	moveNode * ptrToCurrentNode[1];
	ptrToCurrentNode[0] = NULL;

	thisMoveNodeMeta->pathLength = thisNodeArr->size;

	for (i = 0; i < thisNodeArr->size; i++) {
		moveNode * tmpMoveNode = malloc(sizeof(moveNode));
		tmpMoveNode->hasTraversed = 0;
		tmpMoveNode->x = thisNodeArr->nodeArray[i]->x;
		tmpMoveNode->y = thisNodeArr->nodeArray[i]->y;
		tmpMoveNode->nextMoveNode = NULL;
		if (*ptrToCurrentNode == NULL) {
			*ptrToCurrentNode = tmpMoveNode;
			thisMoveNodeMeta->rootMoveNode = tmpMoveNode;
		} else {
			(*ptrToCurrentNode)->nextMoveNode = tmpMoveNode;
			*ptrToCurrentNode = tmpMoveNode;
		}
	}
}

int isGreaterThanPercentage(int numerator, int devisor, int percentToBeat){
	if(devisor == 0){
		return 1;
	}else{
		return ((int)(((numerator*1.0/devisor))*100)) > percentToBeat;
	}
}

int isLessThanPercentage(int numerator, int devisor, int percentToBeat){
	if(devisor == 0){
		return 1;
	}else{
		return ((int)(((numerator*1.0/devisor))*100)) < percentToBeat;
	}
}

int isLowOnMana(individual * thisIndividual){
	int baseMana = getAttributeSum(thisIndividual, "baseMana");

	if(isLessThanPercentage(thisIndividual->mana, baseMana, 35)){
		int weightedRand = (rand() % 100) + getAttributeSum(thisIndividual, "INT") * 5 + getAttributeSum(thisIndividual, "WIS") * 5;

		if(isGreaterThanPercentage( weightedRand, 100, 50) ){
			return 1;
		}
	}

	return 0;
}

item * findManaRestoringItem(inventory * backpack){
	int i, randIndex;
	inventory manaItems;
	item * tmpItem;

	manaItems.inventorySize = 0;
	for(i = 0; i < 40; i++){
		manaItems.inventoryArr[i] = NULL;
	}

	for(i = 0; i < 40; i++){
		tmpItem = backpack->inventoryArr[i];

		if(tmpItem != NULL && tmpItem->type == 'i' && tmpItem->manaMod > 0){
			addItemToInventory(&manaItems, tmpItem);
		}
	}

	if(manaItems.inventorySize == 0){
		return NULL;
	}else{
		randIndex = rand() % manaItems.inventorySize;
		return manaItems.inventoryArr[randIndex];
	}
}

int isLowOnHP(individual * thisIndividual){
	int baseHP = getAttributeSum(thisIndividual, "baseHP");

	if(isLessThanPercentage(thisIndividual->hp, baseHP, 35)){
		int weightedRand = (rand() % 100) + getAttributeSum(thisIndividual, "CON") * 5 + getAttributeSum(thisIndividual, "WIS") * 5;

		if(isGreaterThanPercentage( weightedRand, 100, 50) ){
			return 1;
		}
	}

	return 0;
}

item * getRandomHPRestoringItem(inventory * backpack){
	int i, currentIndex = 0, randIndex;
	item * healingItems[40];
	item * tmpItem;

	for(i = 0; i < 40; i++){
		tmpItem = backpack->inventoryArr[i];

		if(tmpItem != NULL && tmpItem->type == 'i' && tmpItem->healthMod > 0){
			healingItems[currentIndex] = tmpItem;
			currentIndex++;
		}
	}

	if(currentIndex == 0){
		return NULL;
	}else{
		randIndex = rand() % currentIndex;
		return healingItems[randIndex];
	}
}


ability * getRandomHPRestoringAbility(int availableMana, abilityList * thisAbilityList){
	int i, numAbilities = 0, randIndex;
	abilityList healingAbilities;
	ability * tmpAbility;

	for(i = 0; i < thisAbilityList->numAbilities; i++){
		tmpAbility = thisAbilityList->abilitiesList[i];
		if(tmpAbility != NULL && tmpAbility->totalManaCost <= availableMana ){
			if(tmpAbility->diceHPEnabled && tmpAbility->diceHP->effectAndManaArray[tmpAbility->diceHP->selectedIndex]->effectMagnitude > 0){
				healingAbilities.abilitiesList[numAbilities] = tmpAbility;
				numAbilities++;
			}else if(tmpAbility->hpEnabled && tmpAbility->hp->effectAndManaArray[tmpAbility->hp->selectedIndex]->effectMagnitude > 0){
				healingAbilities.abilitiesList[numAbilities] = tmpAbility;
				numAbilities++;
			}
		}
	}

	if(numAbilities == 0){
		return NULL;
	}else{
		randIndex = rand() % numAbilities;
		return healingAbilities.abilitiesList[randIndex];
	}
}

int isInLineOfSight(individual * thisIndividual, individual * targetIndividual, field * thisField){
	int i, toReturn = 1;
	int weightedRand = (rand() % 100);

	if( (targetIndividual->isSneaking  && isGreaterThanPercentage( weightedRand  + getAttributeSum(targetIndividual, "DEX") * 10, 100, 60)) || isGreaterThanPercentage( weightedRand, 100, 90) ){
		return 0;
	}
	cordArr * thisCordArr = cordsBetweenTwoIndividuals(thisIndividual, targetIndividual, thisIndividual->LoS);

	if(thisCordArr == NULL){
		return 0;
	}

	for(i = 0; i < thisCordArr->numCords; i++){
		space * tmpSpace = getSpaceFromField(thisField,thisCordArr->cords[i]->x + thisIndividual->playerCharacter->x, thisCordArr->cords[i]->y + thisIndividual->playerCharacter->y);
		if( tmpSpace == NULL || !tmpSpace->canSeeThrough ){
			toReturn = 0;
			break;
		}
	}

	destroyCordArr(thisCordArr);

	return toReturn;
}

//TODO: dont go in order for choosing targets, get closest (or something)
individual * acquireTarget(individual * enemy, individual * player, groupContainer * thisGroupContainer, field * thisField){
	int i, individualsPassed = 0, playerInLineOfSight = isInLineOfSight(enemy, player, thisField);

	if(player->hp > 0 && playerInLineOfSight && (isGreaterThanPercentage(rand() % 100, 100, 50) || enemy->thisBehavior->isFocusedOnPlayer)){
		return player;
	}

	// for each npc, guards, enemy-npcs etc.
	if(thisGroupContainer->enemies->numIndividuals > 0){
		for(i = 0; i < thisGroupContainer->enemies->MAX_INDIVIDUALS; i++){
			if(thisGroupContainer->enemies->individuals[i] != NULL){
				if((thisGroupContainer->enemies->individuals[i]->faction == -1 || thisGroupContainer->enemies->individuals[i]->faction != enemy->faction) && isInLineOfSight(enemy, thisGroupContainer->enemies->individuals[i], thisField)){
					return thisGroupContainer->enemies->individuals[i];
				}
				individualsPassed++;
			}

			if(individualsPassed == thisGroupContainer->enemies->numIndividuals){
				break;
			}
		}
	}

	if(playerInLineOfSight){
		return player;
	}

	return NULL;
}

individual * getClosestEnemyInLoS(individual * thisIndividual, field * thisField){
	int i,j,k;
	cord * startingCord = malloc(sizeof(cord));
	startingCord->x = thisIndividual->playerCharacter->x;
	startingCord->y = thisIndividual->playerCharacter->y;

	cordArr * activeCords = initCordArr();
	cordArr * totalCords = initCordArr();

	addCordIfUnique(activeCords, startingCord);
	addCordIfUnique(totalCords, startingCord);

	individualGroup * foundEnemies = initGroup();

	for(i = 0; i < 10; i++){
		cordArr * newActiveCords = initCordArr();
		cordArr * newCords;

		for(j = 0; j < activeCords->numCords; j++){
			int foundIndividual = 0;
			newCords = getUniquePassableCordsSurroundingCord(thisIndividual, thisField, activeCords->cords[j], totalCords, 1);

			for(k = 0; k < newCords->numCords; k++){
				addCordIfUnique(totalCords, newCords->cords[k]);
				addCordIfUnique(newActiveCords, newCords->cords[k]);
				individual * tmpIndividual = getIndividualFromField(thisField, newCords->cords[k]->x, newCords->cords[k]->y);

				if(tmpIndividual != NULL && !isAlly(thisIndividual, tmpIndividual) && isInLineOfSight(thisIndividual,tmpIndividual, thisField)){
					addIndividualToGroup(foundEnemies, tmpIndividual);
					foundIndividual = 1;
				}
			}

			free(newCords);

			if(foundIndividual){
				int index = rand() % foundEnemies->numIndividuals;
				individual * toReturn = foundEnemies->individuals[index];

				free(activeCords);
				free(newActiveCords);
				free(foundEnemies);
				destroyCordArr(totalCords);

				return toReturn;
			}
		}

		activeCords->numCords = 0;

		for(j = 0; j < newActiveCords->numCords; j++){
			addCordIfUnique(activeCords, newActiveCords->cords[j]);
		}

		free(newActiveCords);
	}

	free(foundEnemies);
	free(activeCords);
	destroyCordArr(totalCords);

	return NULL;
}

int checkForTargets(individual * enemy, individual * player, groupContainer * thisGroupContainer, field * thisField){
	if(enemy->targetedIndividual != NULL){
		if(enemy->targetedIndividual->hp <= 0 || getIndividualFromField(thisField, enemy->targetedIndividual->playerCharacter->x, enemy->targetedIndividual->playerCharacter->y) == NULL){
			enemy->targetedIndividual = NULL;
		}else{
			int targetIsInSight = isInLineOfSight(enemy, enemy->targetedIndividual, thisField);

			if(!targetIsInSight){
				enemy->targetedDuration -= 2;
			}

			enemy->targetedDuration--;

			if(enemy->targetedDuration <= 0){
				int targetResubscription = rand() % 100;

				if(targetIsInSight && isGreaterThanPercentage(targetResubscription, 100, 50)){
					enemy->targetedDuration = (rand() % 4) + 6;
				}else{
					enemy->targetedIndividual = NULL;
				}
			}
		}
	}

	if(enemy->targetedIndividual == NULL){
		enemy->targetedIndividual = getClosestEnemyInLoS(enemy, thisField);//acquireTarget(enemy, player, thisGroupContainer, thisField);
	}

	if(enemy->targetedIndividual == NULL){
		return 0;
	}else{
		return 1;
	}
}

int hasOffensiveAbilityInRange(individual * thisIndividual){
	int i, randIndex, abilityRange, range = 0;
	abilityList inRangeOffensiveAbilities;
	inRangeOffensiveAbilities.numAbilities = 0;
	inRangeOffensiveAbilities.MAX_ABILITIES = 64;

	if(thisIndividual->targetedIndividual == NULL || thisIndividual->abilities->numAbilities == 0){
		return 0;
	}

	range = max(abs(thisIndividual->playerCharacter->x - thisIndividual->targetedIndividual->playerCharacter->x) , abs(thisIndividual->playerCharacter->y - thisIndividual->targetedIndividual->playerCharacter->y));

	for(i = 0; i < thisIndividual->abilities->numAbilities; i++){
		if(!thisIndividual->abilities->abilitiesList[i]->rangeEnabled){
			continue;
		}

		abilityRange = thisIndividual->abilities->abilitiesList[i]->range->effectAndManaArray[thisIndividual->abilities->abilitiesList[i]->range->selectedIndex]->effectMagnitude;

		if( thisIndividual->abilities->abilitiesList[i]->totalManaCost <= thisIndividual->mana
				&& abilityRange >= range
				&& abilityIsOffensive(thisIndividual->abilities->abilitiesList[i])
				&& inRangeOffensiveAbilities.numAbilities < inRangeOffensiveAbilities.MAX_ABILITIES){
			inRangeOffensiveAbilities.abilitiesList[inRangeOffensiveAbilities.numAbilities] = thisIndividual->abilities->abilitiesList[i];
			inRangeOffensiveAbilities.numAbilities++;
		}
	}

	if(inRangeOffensiveAbilities.numAbilities == 0){
		return 0;
	}else{
		randIndex = rand() % inRangeOffensiveAbilities.numAbilities;
		thisIndividual->activeAbilities->selectedAbility = inRangeOffensiveAbilities.abilitiesList[randIndex];
		return 1;
	}
}

int abilityInRangeOfIndividual(ability * thisAbility, individual * thisIndividual, individual * targetIndividual){
	int maxDistance;

	if(!thisAbility->rangeEnabled){
		return 0;
	}

	maxDistance = max(abs(thisIndividual->playerCharacter->x - targetIndividual->playerCharacter->x), abs(thisIndividual->playerCharacter->y - targetIndividual->playerCharacter->y));

	if(maxDistance <= thisAbility->range->effectAndManaArray[thisAbility->range->selectedIndex]->effectMagnitude){
		return 1;
	}else{
		return 0;
	}
}

void rerollBehavior(individual * thisIndividual){
	if(isGreaterThanPercentage(thisIndividual->thisBehavior->offensiveness, 100, rand() % 100)){
		thisIndividual->thisBehavior->isOffensive = 1;
	}else{
		thisIndividual->thisBehavior->isOffensive = 0;
	}

	if(isGreaterThanPercentage(thisIndividual->thisBehavior->tacticalness, 100, rand() % 100)){
		thisIndividual->thisBehavior->isTactical = 1;
	}else{
		thisIndividual->thisBehavior->isTactical = 0;
	}

	if(thisIndividual->thisBehavior->isCowardly){
		thisIndividual->thisBehavior->cowardlyTurnsRemaining--;
	}

	if(thisIndividual->thisBehavior->cowardlyTurnsRemaining < 0){
		if(isGreaterThanPercentage(thisIndividual->thisBehavior->cowardness, 100, rand() % 100)){
			thisIndividual->thisBehavior->isCowardly = 1;
			thisIndividual->thisBehavior->cowardlyTurnsRemaining = (rand() % 4)+1;
		}else{
			thisIndividual->thisBehavior->isCowardly = 0;
		}
	}
}

int isAlly(individual * thisIndividual, individual * possibleAlly){
	if(thisIndividual->faction == -1 || possibleAlly->faction == -1){
		return 0;
	}

	if(thisIndividual->currentGroupType == possibleAlly->currentGroupType && thisIndividual->faction == possibleAlly->faction){
		return 1;
	}

	if((thisIndividual->isPlayer || thisIndividual->currentGroupType == GROUP_ALLIES)
			&& (possibleAlly->currentGroupType == GROUP_NPCS || possibleAlly->currentGroupType == GROUP_GUARDS)){
		if(!possibleAlly->thisBehavior->isHostileToPlayer){
			return 1;
		}else{
			return 0;
		}
	}

	if((possibleAlly->isPlayer || possibleAlly->currentGroupType == GROUP_ALLIES)
			&& (thisIndividual->currentGroupType == GROUP_NPCS || thisIndividual->currentGroupType == GROUP_GUARDS)){
		if(!thisIndividual->thisBehavior->isHostileToPlayer){
			return 1;
		}else{
			return 0;
		}
	}

	//guards dont attack NPCs, NPCs dont attack each other
	if((thisIndividual->currentGroupType == GROUP_GUARDS && possibleAlly->currentGroupType == GROUP_NPCS)
			|| (thisIndividual->currentGroupType == GROUP_NPCS && possibleAlly->currentGroupType == GROUP_GUARDS)
			|| (thisIndividual->currentGroupType == GROUP_NPCS && possibleAlly->currentGroupType == GROUP_NPCS)){
		return 1;
	}

	return 0;
}

int cordArrContainsCoordinates(cordArr * thisCordArr, int x, int y){
	int i;

	for(i = 0; i < thisCordArr->numCords; i++){
		if(thisCordArr->cords[i]->x == x && thisCordArr->cords[i]->y == y){
			return 1;
		}
	}

	return 0;
}

cord * findRetreatSpace(individual * thisIndividual, groupContainer * thisGroupContainer, field * thisField, int x, int y){
	int i,j,hasChanged=0;
	cord * retreatSpot = malloc(sizeof(cord));
	retreatSpot->x = thisIndividual->playerCharacter->x;
	retreatSpot->y = thisIndividual->playerCharacter->y;

	cordArr * oldSpaces = malloc(sizeof(cordArr));
	oldSpaces->numCords = 0;

	for(i = 0; i < getAttributeSum(thisIndividual, "mvmt"); i++){
		int * direcitonArray = getOrderedRetreatDirections(getRetreatDirection(retreatSpot->x, retreatSpot->y,x,y));

		if(direcitonArray == NULL){
			break;
		}

		for(j = 0; j < 5; j++){
			int newX = retreatSpot->x + xMoveChange(direcitonArray[j]);
			int newY = retreatSpot->y + yMoveChange(direcitonArray[j]);

			if(cordArrContainsCoordinates(oldSpaces, newX, newY)){
				continue;
			}

			cord * newCord = malloc(sizeof(cord));
			newCord->x = newX;
			newCord->y = newY;

			addCordIfUnique(oldSpaces, newCord);

			space * nextSpace = getSpaceFromField(thisField, newX, newY);

			if(nextSpace != NULL && nextSpace->isPassable && (nextSpace->currentIndividual == NULL || isAlly(thisIndividual, nextSpace->currentIndividual)) ){
				retreatSpot->x = newX;
				retreatSpot->y = newY;

				hasChanged = 1;
				break;
			}
		}

		free(direcitonArray);

		if(!hasChanged){ //cannot move further
			destroyCordArr(oldSpaces);
			return retreatSpot;
		}

		hasChanged = 0;
	}

	destroyCordArr(oldSpaces);
	return retreatSpot;
}

int cordHasRangeAdvantage(individual* thisIndividual, int x, int y){
	int rangeGreaterThanAttacker = getAttributeSum(thisIndividual, "range") > getAttributeSum(thisIndividual->targetedIndividual, "range");
	int targetInRange = targetInRangeOfCord(thisIndividual->targetedIndividual, getAttributeSum(thisIndividual, "range"), x, y);
	int attackerInRange = cordWithinRange(thisIndividual->targetedIndividual, x, y);

	if(rangeGreaterThanAttacker){
		if( targetInRange && !attackerInRange){
			return 2;
		}else if(targetInRange && attackerInRange){
			return 1;
		}else{
			return 0;
		}
	}else{
		if(targetInRange && attackerInRange){
			return 2;
		}else{
			return 0;
		}
	}

}

cord * getCordOutsideTargetRange(individual * thisIndividual, field * thisField){
	cord * startingCord = malloc(sizeof(cord));
	startingCord->x = thisIndividual->playerCharacter->x;
	startingCord->y = thisIndividual->playerCharacter->y;

	if(cordHasRangeAdvantage(thisIndividual, startingCord->x, startingCord->y)){
		return startingCord;
	}

	int i,j,k;
	cordArr * activeCords = initCordArr();
	cordArr * totalCords = initCordArr();

	cord * toReturn = malloc(sizeof(cord));
	toReturn->x = startingCord->x;
	toReturn->y = startingCord->y;

	addCordIfUnique(activeCords, startingCord);
	addCordIfUnique(totalCords, startingCord);

	for(i = getAttributeSum(thisIndividual, "mvmt"); i > 0; i--){
		cordArr * newActiveCords = initCordArr();
		cordArr * newCords;

		for(j = 0; j < activeCords->numCords; j++){
			int cordFound = 0;
			newCords = getUniquePassableCordsSurroundingCord(thisIndividual, thisField, activeCords->cords[j], totalCords, 0);

			for(k = 0; k < newCords->numCords; k++){
				addCordIfUnique(totalCords, newCords->cords[k]);
				addCordIfUnique(newActiveCords, newCords->cords[k]);
				individual * tmpIndividual = getIndividualFromField(thisField, newCords->cords[k]->x, newCords->cords[k]->y);

				if(tmpIndividual != NULL){
					continue;
				}

				if(!cordWithinRange(thisIndividual->targetedIndividual, newCords->cords[k]->x, newCords->cords[k]->y)){
					toReturn->x = newCords->cords[k]->x;
					toReturn->y = newCords->cords[k]->y;

					cordFound = 1;
				}

			}

			free(newCords);

			if(cordFound){
				free(activeCords);
				free(newActiveCords);
				destroyCordArr(totalCords);

				return toReturn;
			}
		}

		activeCords->numCords = 0;

		for(j = 0; j < newActiveCords; j++){
			addCordIfUnique(activeCords, newActiveCords->cords[k]);
		}

		free(newActiveCords);
	}

	free(activeCords);
	destroyCordArr(totalCords);

	return toReturn;
}

cord * getCordWithTargetInRange(individual * thisIndividual, field * thisField){
	cord * startingCord = malloc(sizeof(cord));
	startingCord->x = thisIndividual->playerCharacter->x;
	startingCord->y = thisIndividual->playerCharacter->y;

	if(cordHasRangeAdvantage(thisIndividual, startingCord->x, startingCord->y) == 2){
		return startingCord;
	}

	int i,j,k;
	cordArr * activeCords = initCordArr();
	cordArr * totalCords = initCordArr();

	cord * toReturn = malloc(sizeof(cord));
	toReturn->x = startingCord->x;
	toReturn->y = startingCord->y;

	addCordIfUnique(activeCords, startingCord);
	addCordIfUnique(totalCords, startingCord);

	for(i = getAttributeSum(thisIndividual, "mvmt"); i > 0; i--){
		cordArr * newActiveCords = initCordArr();
		cordArr * newCords;

		for(j = 0; j < activeCords->numCords; j++){
			int cordFound = 0;
			newCords = getUniquePassableCordsSurroundingCord(thisIndividual, thisField, activeCords->cords[j], totalCords, 0);

			for(k = 0; k < newCords->numCords; k++){
				addCordIfUnique(totalCords, newCords->cords[k]);
				addCordIfUnique(newActiveCords, newCords->cords[k]);
				individual * tmpIndividual = getIndividualFromField(thisField, newCords->cords[k]->x, newCords->cords[k]->y);

				if(tmpIndividual != NULL || cordFound){
					continue;
				}

				int cordAdvantage = cordHasRangeAdvantage(thisIndividual, newCords->cords[k]->x, newCords->cords[k]->y);

				if(cordAdvantage == 2){
					toReturn->x = newCords->cords[k]->x;
					toReturn->y = newCords->cords[k]->y;

					char outStr[128];
					sprintf(outStr,"Found ideal spot: %s->%s [%d,%d] -> [%d,%d]",thisIndividual->name, thisIndividual->targetedIndividual->name, toReturn->x, toReturn->y,  thisIndividual->targetedIndividual->playerCharacter->x, thisIndividual->targetedIndividual->playerCharacter->y);

					cwrite(outStr);
					cordFound = 1;
				}

				if(cordAdvantage == 1){
					toReturn->x = newCords->cords[k]->x;
					toReturn->y = newCords->cords[k]->y;
				}
			}

			free(newCords);

			if(cordFound){
				free(activeCords);
				free(newActiveCords);
				destroyCordArr(totalCords);

				return toReturn;
			}
		}

		activeCords->numCords = 0;

		for(j = 0; j < newActiveCords->numCords; j++){
			addCordIfUnique(activeCords, newActiveCords->cords[j]);
		}

		free(newActiveCords);
	}

	free(activeCords);
	destroyCordArr(totalCords);

	return toReturn;
}

int moveToSelectedLocation(individual * thisIndividual, field * thisField,  moveNodeMeta ** thisMoveNodeMeta, int x, int y){
	nodeArr * enemyNodeArr = getSpaceClosestToSpace(thisField, thisIndividual, x, y);

	thisIndividual->remainingActions--;

	//nowhere to go, nothing to animate
	if (enemyNodeArr->size == 0) {
		destroyNodeArr(enemyNodeArr);
		thisIndividual->activeAbilities->selectedAbility = NULL;
		return 0;
	}

	//Gonna move, remove them from the field and update the moveNodeMeta
	getSpaceFromField(thisField, thisIndividual->playerCharacter->x,
			thisIndividual->playerCharacter->y)->currentIndividual = NULL;

	(*thisMoveNodeMeta) = malloc(sizeof(moveNodeMeta));
	(*thisMoveNodeMeta)->sum = 0;

	populateMoveNodeMeta(*thisMoveNodeMeta, enemyNodeArr);

	destroyNodeArr(enemyNodeArr);


	return 1;
}

int moveToGetOutOfTargetsRange(individual * thisIndividual, field * thisField,  moveNodeMeta ** thisMoveNodeMeta){
	int x, y;
	cord * targetSpace = getCordOutsideTargetRange(thisIndividual, thisField);

	x = targetSpace->x;
	y = targetSpace->y;

	free(targetSpace);

	return moveToSelectedLocation(thisIndividual, thisField, thisMoveNodeMeta, x, y);
}

int moveToGetTargetInRange(individual * thisIndividual, field * thisField,  moveNodeMeta ** thisMoveNodeMeta){
	int x, y;
	cord * targetSpace = getCordWithTargetInRange(thisIndividual, thisField);
	x = targetSpace->x;
	y = targetSpace->y;

	free(targetSpace);

	return moveToSelectedLocation(thisIndividual, thisField, thisMoveNodeMeta, x, y);
}

int retreatFromTarget(individual * thisIndividual, groupContainer * thisGroupContainer, field * thisField,  moveNodeMeta ** thisMoveNodeMeta){
	int x, y;
	cord * targetSpace = findRetreatSpace(thisIndividual, thisGroupContainer, thisField, thisIndividual->targetedIndividual->playerCharacter->x, thisIndividual->targetedIndividual->playerCharacter->y);
	x = targetSpace->x;
	y = targetSpace->y;

	free(targetSpace);

	return moveToSelectedLocation(thisIndividual, thisField, thisMoveNodeMeta, x, y);
}

int noEnemiesInRange(individual * enemy, groupContainer * thisGroupContainer, field * thisField, int range){
	int i, j, startingX, startingY;

	startingX = max(enemy->playerCharacter->x - range, 0);
	startingY = max(enemy->playerCharacter->y - range, 0);

	for(i = startingX; i < startingX + (range*2 + 1); i++){
		for(j = startingY; j < startingY + (range*2 + 1); j++){
			space * tmpSpace = getSpaceFromField(thisField, i, j);

			if(tmpSpace != NULL && tmpSpace->currentIndividual != NULL && !isAlly(enemy, tmpSpace->currentIndividual) ){
				return 0;
			}
		}
	}

	return 1;
}

item * getBuffItem(inventory * thisInventory){
	int i;

	for(i = 0; i < thisInventory->inventorySize; i++){
		if(thisInventory->inventoryArr[i] != NULL && thisInventory->inventoryArr[i]->type == 'i' && itemIsBuffing(thisInventory->inventoryArr[i])){
			return thisInventory->inventoryArr[i];
		}
	}

	return NULL;
}

ability * getRandomBuffAbility(individual * thisIndividual){
	abilityList * buffAbilities = malloc(sizeof(abilityList));
	buffAbilities->numAbilities = 0;

	ability * selectedAbility;
	int i;

	for(i = 0; i < thisIndividual->abilities->numAbilities; i++){
		if(thisIndividual->abilities->abilitiesList[i] != NULL && thisIndividual->abilities->abilitiesList[i]->totalManaCost <= thisIndividual->mana && abilityIsBuffing(thisIndividual->abilities->abilitiesList[i])){
			buffAbilities->abilitiesList[buffAbilities->numAbilities] = thisIndividual->abilities->abilitiesList[i];
			buffAbilities->numAbilities++;
		}
	}

	if(!buffAbilities->numAbilities){
		free(buffAbilities);
		return NULL;
	}

	selectedAbility = buffAbilities->abilitiesList[rand() % buffAbilities->numAbilities];
	free(buffAbilities);

	return selectedAbility;
}

ability * getRandomOffensiveAbility(individual * thisIndividual){
	ability * selectedAbility;
	abilityList * offensiveAbilities = malloc(sizeof(abilityList));
	offensiveAbilities->numAbilities = 0;
	int i;

	for(i = 0; i < thisIndividual->abilities->numAbilities; i++){
		if(thisIndividual->abilities->abilitiesList[i] != NULL && thisIndividual->abilities->abilitiesList[i]->type == 't' &&
				thisIndividual->abilities->abilitiesList[i]->totalManaCost <= thisIndividual->mana && abilityIsOffensive(thisIndividual->abilities->abilitiesList[i])){
			offensiveAbilities->abilitiesList[offensiveAbilities->numAbilities] = thisIndividual->abilities->abilitiesList[i];
			offensiveAbilities->numAbilities++;
		}
	}

	if(!offensiveAbilities->numAbilities){
		free(offensiveAbilities);
		return NULL;
	}

	selectedAbility = offensiveAbilities->abilitiesList[rand() % offensiveAbilities->numAbilities];
	free(offensiveAbilities);

	return selectedAbility;
}


int selectHealingAbility(individual * thisIndividual){
	int i;

	abilityList * healingAbilities = malloc(sizeof(abilityList));
	healingAbilities->numAbilities = 0;
	healingAbilities->MAX_ABILITIES = 64;

	for(i = 0; i < thisIndividual->abilities->numAbilities; i++){
		if(healingAbilities->numAbilities < healingAbilities->MAX_ABILITIES && thisIndividual->abilities->abilitiesList[i] != NULL
				&& (thisIndividual->abilities->abilitiesList[i]->hpEnabled
				&& thisIndividual->abilities->abilitiesList[i]->hp->effectAndManaArray[thisIndividual->abilities->abilitiesList[i]->hp->selectedIndex]->effectMagnitude > 0)
				||  (thisIndividual->abilities->abilitiesList[i]->diceHPEnabled
						&& thisIndividual->abilities->abilitiesList[i]->diceHP->effectAndManaArray[thisIndividual->abilities->abilitiesList[i]->diceHP->selectedIndex]->effectMagnitude > 0)){
			healingAbilities->abilitiesList[healingAbilities->numAbilities] = thisIndividual->abilities->abilitiesList[i];
			healingAbilities->numAbilities++;
		}
	}

	if(healingAbilities->numAbilities == 0){
		free(healingAbilities);
		return 0;
	}

	thisIndividual->activeAbilities->selectedAbility = healingAbilities->abilitiesList[(rand() % healingAbilities->numAbilities)];
	return 1;
}

individualGroup * getAlliesInRange(individual * thisIndividual, field * thisField, int radius){
	int startingX = max(thisIndividual->playerCharacter->x - radius,0);
	int startingY = max(thisIndividual->playerCharacter->y - radius,0);
	int i, j;

	individualGroup * alliesInRange = initGroup();

	for(i = startingX; i < startingX + radius*2 +1; i++){
		for(j = startingY; j < startingY + radius*2 + 1; j++){
			individual * tmpIndiviudal = getIndividualFromField(thisField, i, j);

			if(tmpIndiviudal != NULL && isAlly(thisIndividual, tmpIndiviudal)){
				addIndividualToGroup(alliesInRange, tmpIndiviudal);
			}
		}
	}

	return alliesInRange;
}

individual * allyRequiringHealing(individual * thisIndividual, individualGroup * alliesInRange){
	int i, j;

	individualGroup * healableIndividuals = initGroup();

	for(i = 0; i < alliesInRange->numIndividuals; i++){
		if(!isGreaterThanPercentage(alliesInRange->individuals[i]->hp, getTotalHP(alliesInRange->individuals[i]), 80)){
			addIndividualToGroup(healableIndividuals, alliesInRange->individuals[i]);
		}
	}

	if(healableIndividuals->numIndividuals == 0){
		free(healableIndividuals);
		return NULL;
	}

	j = 0;
	for(i = 0; i < healableIndividuals->numIndividuals; i++){
		if(healableIndividuals->individuals[i]->hp < healableIndividuals->individuals[j]->hp){
			j = i;
		}
	}

	individual * toReturn = healableIndividuals->individuals[j];
	free(healableIndividuals);
	return toReturn;
}

int moveCloserToTarget(individual * enemy, individual * targetIndividual, field * thisField, moveNodeMeta ** thisMoveNodeMeta){
	nodeArr * enemyNodeArr = getSpaceClosestToPlayer(thisField, enemy, targetIndividual);

	//nowhere to go, nothing to animate
	if (enemyNodeArr->size == 0) {
		destroyNodeArr(enemyNodeArr);
		enemy->remainingActions--;
		return 0;
	}

	//Gonna move, remove them from the field and update the moveNodeMeta
	getSpaceFromField(thisField, enemy->playerCharacter->x,
			enemy->playerCharacter->y)->currentIndividual = NULL;

	(*thisMoveNodeMeta) = malloc(sizeof(moveNodeMeta));
	(*thisMoveNodeMeta)->sum = 0;

	populateMoveNodeMeta(*thisMoveNodeMeta, enemyNodeArr);

	destroyNodeArr(enemyNodeArr);

	enemy->remainingActions--;
	return 1;
}

int attackModule(individual * enemy, individual * player, groupContainer * thisGroupContainer, field * thisField, moveNodeMeta ** thisMoveNodeMeta){
	if(enemy->thisBehavior->isTactical && getAttributeSum(enemy, "range") > getAttributeSum(enemy->targetedIndividual,"range")  && individualWithinRange(enemy->targetedIndividual, enemy)){
		 //&& isGreaterThanPercentage(rand() % 100, 100, 50)
		int x, y;
		cord * rangedCord = getCordWithTargetInRange(enemy, thisField);
		x = rangedCord->x;
		y = rangedCord->y;

		free(rangedCord);

		if(cordHasRangeAdvantage(enemy, x, y) == 2){
			return moveToSelectedLocation(enemy, thisField, thisMoveNodeMeta, x, y);
		}
	}

	if (individualWithinRange(enemy, enemy->targetedIndividual)) {
		if (attackIndividual(enemy, enemy->targetedIndividual)) {
			deleteIndividiaulFromGroup(getGroupFromIndividual(thisGroupContainer, enemy->targetedIndividual), enemy->targetedIndividual);
			removeIndividualFromField(thisField,
					enemy->targetedIndividual->playerCharacter->x,
					enemy->targetedIndividual->playerCharacter->y);
		}
		enemy->remainingActions--;
		return 0;
	} else {
		if(enemy->thisBehavior->isTactical){
			return tacticalModule(enemy, player, thisGroupContainer, thisField, thisMoveNodeMeta);
		}else{
			int toReturn = moveCloserToTarget(enemy, enemy->targetedIndividual, thisField, thisMoveNodeMeta);

			//cannot reach target and target out of range, get new closer target
			if(!toReturn){
				enemy->targetedIndividual = NULL;
				enemy->targetedDuration = 0;
			}

			return toReturn;
		}

	}
}

int tacticalModule(individual * enemy, individual * player, groupContainer * thisGroupContainer, field * thisField, moveNodeMeta ** thisMoveNodeMeta){
	if(enemy->thisBehavior->isOffensive){

		if(!individualWithinRange(enemy, enemy->targetedIndividual)){
			//move in attack range
			return moveToGetTargetInRange(enemy, thisField, thisMoveNodeMeta);
		}
	}else{
		individualGroup * allies = getAlliesInRange(enemy, thisField,4);

		//Move closer to an ally nearby
		if(allies->numIndividuals > 0){
			int toReturn = moveCloserToTarget(enemy, allies->individuals[rand() % allies->numIndividuals], thisField, thisMoveNodeMeta);

			free(allies);
			return toReturn;
		}

		free(allies);

		//Move out of attack range
		if(individualWithinRange(enemy->targetedIndividual, enemy)){
			return moveToGetOutOfTargetsRange(enemy, thisField, thisMoveNodeMeta);
		}
	}

	return 0;
}

int tryRestoreMana(individual * thisIndividual){
	item * manaRestoringItem = findManaRestoringItem(thisIndividual->backpack);

	if(manaRestoringItem != NULL){
		if(manaRestoringItem->type == 'd' && thisIndividual->activeItems->activeItemsTotal < 40){
			consumeItem(thisIndividual, manaRestoringItem);
			addItemToActiveItemList(thisIndividual, manaRestoringItem);
			free(removeItemFromInventory(thisIndividual->backpack, manaRestoringItem));
			thisIndividual->remainingActions--;
			return 1;
		}

		if(manaRestoringItem->type == 'i'){
			consumeItem(thisIndividual, manaRestoringItem);
			free(removeItemFromInventory(thisIndividual->backpack, manaRestoringItem));
			thisIndividual->remainingActions--;
			return 1;
		}
	}

	return 0;
}

int tryHeal(individual * thisIndividual, individual * player, groupContainer * thisGroupContainer, field * thisField){
	ability * hpRestoringAbility = getRandomHPRestoringAbility(thisIndividual->mana, thisIndividual->abilities);
	item * hpRestoringItem = getRandomHPRestoringItem(thisIndividual->backpack);
	int selectedHealingMethod = 0;

	if (hpRestoringItem != NULL && hpRestoringAbility != NULL) {
		selectedHealingMethod = (rand() % 2) + 1;
	} else if (hpRestoringItem != NULL) {
		selectedHealingMethod = 1;
	} else if (hpRestoringAbility != NULL) {
		selectedHealingMethod = 2;
	}

	if (selectedHealingMethod == 1) {
		if(hpRestoringItem->itemType == 'd' && thisIndividual->activeItems->activeItemsTotal < 40){
			char msg[128];
			sprintf(msg, "%s used %s.\n", thisIndividual->name, hpRestoringItem->name);
			cwrite(msg);
			consumeItem(thisIndividual, hpRestoringItem);
			addItemToActiveItemList(thisIndividual, hpRestoringItem);
			removeItemFromInventory(thisIndividual->backpack, hpRestoringItem);

			thisIndividual->remainingActions--;
			return 1;
		}

		if(hpRestoringItem->itemType == 'c'){
			char msg[128];
			sprintf(msg, "%s used %s.\n", thisIndividual->name, hpRestoringItem->name);
			cwrite(msg);
			consumeItem(thisIndividual, hpRestoringItem);
			free(removeItemFromInventory(thisIndividual->backpack, hpRestoringItem));
			thisIndividual->remainingActions--;
			return 1;
		}

		if(hpRestoringAbility != NULL){
			selectedHealingMethod = 2;
		}
	}

	if (selectedHealingMethod == 2) {
		if(hpRestoringAbility->type == 'd' || hpRestoringAbility->type == 't'){
			thisIndividual->activeAbilities->selectedAbility = hpRestoringAbility;
			useAbilityOnIndividualsInAOERange(thisIndividual, player, thisGroupContainer, thisField, thisIndividual->playerCharacter->x, thisIndividual->playerCharacter->y);
			thisIndividual->activeAbilities->selectedAbility = NULL;
			thisIndividual->remainingActions--;
			return 1;
		}else if(hpRestoringAbility->type == "i"){
			useAbility(thisIndividual, hpRestoringAbility);
			thisIndividual->remainingActions--;
			return 1;
		}
	}

	return 0;
}

int useAbilityOnTargetedSpace(individual * enemy, individual * player, groupContainer * thisGroupContainer, field * thisField, int x, int y){
	int numActions = 1;

	useAbilityOnIndividualsInAOERange(enemy, player, thisGroupContainer, thisField, x, y);

	if(enemy->activeAbilities->selectedAbility->actionsEnabled){
		numActions += enemy->activeAbilities->selectedAbility->actions->effectAndManaArray[enemy->activeAbilities->selectedAbility->actions->selectedIndex]->effectMagnitude;
	}

	enemy->activeAbilities->selectedAbility = NULL;

	enemy->remainingActions -= numActions;
	return 0;
}

int performAction(individual * thisIndividual, individual * player, groupContainer * thisGroupContainer, field * thisField, moveNodeMeta ** thisMoveNodeMeta){
	switch(thisIndividual->currentGroupType){
		case GROUP_ENEMIES:
			return enemyAction(thisIndividual, player, thisGroupContainer, thisField, thisMoveNodeMeta);
		case GROUP_NPCS:
			return npcAction(thisIndividual, player, thisGroupContainer, thisField, thisMoveNodeMeta);
		case GROUP_BEASTS:
			thisIndividual->remainingActions = 0;
			return 0;
		case GROUP_GUARDS:
			return guardAction(thisIndividual, player, thisGroupContainer, thisField, thisMoveNodeMeta);
		default:
			return 0;
	}
}

int enemyAction(individual * enemy, individual * player, groupContainer * thisGroupContainer, field * thisField, moveNodeMeta ** thisMoveNodeMeta){

	//Restore Mana
	if(isLowOnMana(enemy)){
		if(tryRestoreMana(enemy)){
			return 0;
		}
	}

	//Restore HP
	if(isLowOnHP(enemy)){
		if(tryHeal(enemy, player, thisGroupContainer, thisField)){
			return 0;
		}
	}

	//Do checkForTargets, if null walk about, else go to target.
	if(!checkForTargets(enemy, player, thisGroupContainer, thisField)){
		enemy->remainingActions = 0;
		if(isGreaterThanPercentage(rand(), 100, 50)){//wander a bit.
			return 0;
		}else{//do nothing
			return 0;
		}//Add a 'speak' option
	}

	//On Deck Ability
	if(enemy->activeAbilities->selectedAbility != NULL){
		if(enemy->mana >= enemy->activeAbilities->selectedAbility->totalManaCost){
			if(abilityIsOffensive(enemy->activeAbilities->selectedAbility)){
				if(enemy->targetedIndividual != NULL){
					if(abilityInRangeOfIndividual(enemy->activeAbilities->selectedAbility, enemy, enemy->targetedIndividual)){
						cwrite("CASE 1\n");
						return useAbilityOnTargetedSpace(enemy, player, thisGroupContainer, thisField, enemy->targetedIndividual->playerCharacter->x, enemy->targetedIndividual->playerCharacter->y);
					}else{
						int moving = moveCloserToTarget(enemy, enemy->targetedIndividual, thisField, thisMoveNodeMeta);

						if(moving){
							return moving;
						}else{
							//cannot reach target and target out of range, get new closer target
							enemy->targetedIndividual = NULL;
							enemy->targetedDuration = 0;
							enemy->activeAbilities->selectedAbility = NULL;
						}
					}
				}else{
					enemy->activeAbilities->selectedAbility = NULL;
				}
			}else{//is buff ability
				if(enemy->allyIndividual != NULL){
					if(abilityInRangeOfIndividual(enemy->activeAbilities->selectedAbility, enemy, enemy->allyIndividual)){
						cwrite("CASE 2\n");
						return useAbilityOnTargetedSpace(enemy, player, thisGroupContainer, thisField, enemy->allyIndividual->playerCharacter->x, enemy->allyIndividual->playerCharacter->y);
					}else{
						int moving = moveCloserToTarget(enemy, enemy->allyIndividual, thisField, thisMoveNodeMeta);

						if(moving){
							return moving;
						}else{
							//cannot reach target and target out of range, get new closer target
							enemy->allyIndividual = NULL;
							enemy->targetedDuration = 0;
							enemy->activeAbilities->selectedAbility = NULL;
						}
					}
				}else{
					enemy->activeAbilities->selectedAbility = NULL;
				}
			}
		}else{
			enemy->activeAbilities->selectedAbility = NULL;
		}
	}

	//Update behavior
	rerollBehavior(enemy);

	//Cowardly Actions
	if(enemy->thisBehavior->isCowardly){
		if(noEnemiesInRange(enemy, thisGroupContainer, thisField, 4)){
			char fleeText[128];

			if(isGreaterThanPercentage((rand() %100) + getAttributeSum(enemy, "DEX")*10 + getAttributeSum(enemy, "WIS")*5 ,100, 50)){
				sprintf(fleeText, "%s fled the fight!", enemy->name);
				cwrite(fleeText);

				deleteIndividiaulFromGroup(getGroupFromIndividual(thisGroupContainer, enemy), enemy);
				removeIndividualFromField(thisField, enemy->playerCharacter->x, enemy->playerCharacter->y);
				enemy->remainingActions--;
				return 0;
			}else{
				sprintf(fleeText, "%s tried to flee, but couldn't escape!", enemy->name);
				cwrite(fleeText);

				enemy->remainingActions--;
				return 0;
			}
		} else {
			int isMoving = retreatFromTarget(enemy, thisGroupContainer, thisField, thisMoveNodeMeta);

			enemy->remainingActions--;
			return isMoving;
		}
	}

	//Offensive
	if (enemy->thisBehavior->isOffensive) {

		if(enemy->thisBehavior->isTactical && enemy->backpack->inventorySize > 0 && isGreaterThanPercentage(rand()%100,100,67)){
			item * randomBuffItem = getBuffItem(enemy->backpack);

			if(randomBuffItem != NULL){
				modifyItem(randomBuffItem, enemy);
				enemy->remainingActions--;
				return 0;
			}
		}

		//has attack ability to use?
		if (isGreaterThanPercentage(enemy->thisBehavior->abilityAffinity, 100, rand() % 100) && hasOffensiveAbilityInRange(enemy)) {

			ability * buffAbility = getRandomBuffAbility(enemy);
			ability * offensiveAbility = getRandomOffensiveAbility(enemy);

			if(offensiveAbility == NULL && buffAbility == NULL){
				return attackModule(enemy, player, thisGroupContainer, thisField, thisMoveNodeMeta);
			}else if(offensiveAbility != NULL && buffAbility != NULL){
				if(rand() % 2){
					offensiveAbility = NULL;
				} else{
					buffAbility = NULL;
				}
			}

			if(offensiveAbility != NULL){
				enemy->activeAbilities->selectedAbility = offensiveAbility;

				int range = max(abs(enemy->playerCharacter->x - enemy->targetedIndividual->playerCharacter->x) , abs(enemy->playerCharacter->y - enemy->targetedIndividual->playerCharacter->y));
				int abilityRange = offensiveAbility->range->effectAndManaArray[offensiveAbility->range->selectedIndex]->effectMagnitude;

				if(abilityRange < range){//dont remove selected ability - it's now on deck
					int toReturn = moveCloserToTarget(enemy, enemy->targetedIndividual, thisField, thisMoveNodeMeta);

					if(!toReturn){
						//cannot reach target and target out of range, get new closer target
						enemy->targetedIndividual = NULL;
						enemy->targetedDuration = 0;
						enemy->activeAbilities->selectedAbility = NULL;
					}

					return toReturn;
				}else{
					cwrite("CASE 3\n");
					return useAbilityOnTargetedSpace(enemy, player, thisGroupContainer, thisField, enemy->targetedIndividual->playerCharacter->x, enemy->targetedIndividual->playerCharacter->y);
				}
			}else{
				enemy->activeAbilities->selectedAbility = buffAbility;
				cwrite("CASE 4\n");
				return useAbilityOnTargetedSpace(enemy, player, thisGroupContainer, thisField, enemy->playerCharacter->x, enemy->playerCharacter->y);
			}
		}else{
			return attackModule(enemy, player, thisGroupContainer, thisField, thisMoveNodeMeta);
		}
	}

	//Supportive
	else{
		cwrite("Being supportive~~ :3");

		if(enemy->abilities->numAbilities == 0){
			return attackModule(enemy, player, thisGroupContainer, thisField, thisMoveNodeMeta);
		}

		if(enemy->thisBehavior->isTactical && isGreaterThanPercentage(rand() % 100, 100, 30)){
			return tacticalModule(enemy, player, thisGroupContainer, thisField, thisMoveNodeMeta);
		}

		if(((enemy->mana < (getTotalMana(enemy)/2)) && isGreaterThanPercentage(rand() % 100, 100, 30)) || !canUseAnyAbilities(enemy)){
			channelMana(enemy);
			return 0;
		}

		individualGroup * alliesInRange = getAlliesInRange(enemy, thisField, 8);

		if(selectHealingAbility(enemy)){
			individual * ally  = allyRequiringHealing(enemy, alliesInRange);

			if(ally != NULL){
				free(alliesInRange);
				enemy->allyIndividual = ally;

				if(abilityInRangeOfIndividual(enemy->activeAbilities->selectedAbility, enemy, enemy->allyIndividual)){
					cwrite("CASE 5\n");
					return useAbilityOnTargetedSpace(enemy, player, thisGroupContainer, thisField,  enemy->allyIndividual->playerCharacter->x, enemy->allyIndividual->playerCharacter->y);
				}else{
					int moving = moveCloserToTarget(enemy, enemy->allyIndividual, thisField, thisMoveNodeMeta);

					if(moving){
						return moving;
					}else{
						//cannot reach target and target out of range, get new closer target
						enemy->allyIndividual = NULL;
						enemy->targetedDuration = 0;
						enemy->activeAbilities->selectedAbility = NULL;
					}
				}
			}
		}

		ability * randomBuffAbility = getRandomBuffAbility(enemy);

		if(randomBuffAbility == NULL){
			channelMana(enemy);
			return 0;
		}

		enemy->activeAbilities->selectedAbility = randomBuffAbility;

		if(randomBuffAbility->type == 'd' || randomBuffAbility->type == 'i'){
			free(alliesInRange);
			cwrite("CASE 6\n");
			return useAbilityOnTargetedSpace(enemy, player, thisGroupContainer, thisField,  enemy->playerCharacter->x, enemy->playerCharacter->y);
		}

		enemy->allyIndividual = alliesInRange->individuals[rand() % alliesInRange->numIndividuals];

		free(alliesInRange);

		if(abilityInRangeOfIndividual(enemy->activeAbilities->selectedAbility, enemy, enemy->allyIndividual)){
			cwrite("CASE 7\n");
			return useAbilityOnTargetedSpace(enemy, player, thisGroupContainer, thisField,  enemy->allyIndividual->playerCharacter->x, enemy->allyIndividual->playerCharacter->y);
		}else{
			int moving = moveCloserToTarget(enemy, enemy->allyIndividual, thisField, thisMoveNodeMeta);

			if(moving){
				return moving;
			}else{
				//cannot reach target and target out of range, get new closer target
				enemy->allyIndividual = NULL;
				enemy->targetedDuration = 0;
				enemy->activeAbilities->selectedAbility = NULL;
			}
		}

		return 0;
	}
}

int guardAction(individual * guard, individual * player, groupContainer * thisGroupContainer, field * thisField, moveNodeMeta ** thisMoveNodeMeta){

	//Restore Mana
	if(isLowOnMana(guard)){
		if(tryRestoreMana(guard)){
			return 0;
		}
	}

	//Restore HP
	if(isLowOnHP(guard)){
		if(tryHeal(guard, player, thisGroupContainer, thisField)){
			return 0;
		}
	}

	//Do checkForTargets, if null walk about, else go to target.

	findDangerousIndividualNearBy(guard, player, thisGroupContainer, thisField);

	if(guard->targetedIndividual == NULL){

		//talk to player if they have a bounty
		if(getCurrentBounty(player) > 0){
			if(individualWithinTalkingRange(guard, player, 2)){
				setCurrentMessageByIndividualID(guard->ID, (guard->thisBehavior->isHostileToPlayer && (guard->currentGroupType == GROUP_NPCS || guard->currentGroupType == GROUP_GUARDS)), guard->thisBehavior->hasAlreadyYieldedToPlayer);

				setSpeakingIndividualID(guard->ID);
				toggleDrawDialogBox();
				guard->remainingActions--;
				return 0;

			}else{
				return moveCloserToTarget(guard, player, thisField, thisMoveNodeMeta);
			}
		}

		if(!atDesiredLocation(guard)){
			return moveToSelectedLocation(guard, thisField, thisMoveNodeMeta, guard->desiredLocation->x, guard->desiredLocation->y);
		}

		guard->remainingActions = 0;
		if(isGreaterThanPercentage(rand(), 100, 50)){//wander a bit.
			return 0;
		}else{//do nothing
			return 0;
		}//Add a 'speak' option
	}

	//On Deck Ability
	if(guard->activeAbilities->selectedAbility != NULL){
		if(guard->mana >= guard->activeAbilities->selectedAbility->totalManaCost){
			if(abilityIsOffensive(guard->activeAbilities->selectedAbility)){
				if(guard->targetedIndividual != NULL){
					if(abilityInRangeOfIndividual(guard->activeAbilities->selectedAbility, guard, guard->targetedIndividual)){
						cwrite("CASE 1\n");
						return useAbilityOnTargetedSpace(guard, player, thisGroupContainer, thisField, guard->targetedIndividual->playerCharacter->x, guard->targetedIndividual->playerCharacter->y);
					}else{
						int moving = moveCloserToTarget(guard, guard->targetedIndividual, thisField, thisMoveNodeMeta);

						if(moving){
							return moving;
						}else{
							//cannot reach target and target out of range, get new closer target
							guard->targetedIndividual = NULL;
							guard->targetedDuration = 0;
							guard->activeAbilities->selectedAbility = NULL;
						}
					}
				}else{
					guard->activeAbilities->selectedAbility = NULL;
				}
			}else{//is buff ability
				if(guard->allyIndividual != NULL){
					if(abilityInRangeOfIndividual(guard->activeAbilities->selectedAbility, guard, guard->allyIndividual)){
						cwrite("CASE 2\n");
						return useAbilityOnTargetedSpace(guard, player, thisGroupContainer, thisField, guard->allyIndividual->playerCharacter->x, guard->allyIndividual->playerCharacter->y);
					}else{
						int moving = moveCloserToTarget(guard, guard->allyIndividual, thisField, thisMoveNodeMeta);

						if(moving){
							return moving;
						}else{
							//cannot reach target and target out of range, get new closer target
							guard->allyIndividual = NULL;
							guard->targetedDuration = 0;
							guard->activeAbilities->selectedAbility = NULL;
						}
					}
				}else{
					guard->activeAbilities->selectedAbility = NULL;
				}
			}
		}else{
			guard->activeAbilities->selectedAbility = NULL;
		}
	}

	//Update behavior
	rerollBehavior(guard);

	//Cowardly Actions
	if(guard->thisBehavior->isCowardly){
		if(noEnemiesInRange(guard, thisGroupContainer, thisField, 4)){
			char fleeText[128];

			if(isGreaterThanPercentage((rand() %100) + getAttributeSum(guard, "DEX")*10 + getAttributeSum(guard, "WIS")*5 ,100, 50)){
				sprintf(fleeText, "%s fled the fight!", guard->name);
				cwrite(fleeText);

				deleteIndividiaulFromGroup(getGroupFromIndividual(thisGroupContainer, guard), guard);
				removeIndividualFromField(thisField, guard->playerCharacter->x, guard->playerCharacter->y);
				guard->remainingActions--;
				return 0;
			}else{
				sprintf(fleeText, "%s tried to flee, but couldn't escape!", guard->name);
				cwrite(fleeText);

				guard->remainingActions--;
				return 0;
			}
		} else {
			int isMoving = retreatFromTarget(guard, thisGroupContainer, thisField, thisMoveNodeMeta);

			guard->remainingActions--;
			return isMoving;
		}
	}

	//Offensive
	if (guard->thisBehavior->isOffensive) {

		if(guard->thisBehavior->isTactical && guard->backpack->inventorySize > 0 && isGreaterThanPercentage(rand()%100,100,67)){
			item * randomBuffItem = getBuffItem(guard->backpack);

			if(randomBuffItem != NULL){
				modifyItem(randomBuffItem, guard);
				guard->remainingActions--;
				return 0;
			}
		}

		//has attack ability to use?
		if (isGreaterThanPercentage(guard->thisBehavior->abilityAffinity, 100, rand() % 100) && hasOffensiveAbilityInRange(guard)) {

			ability * buffAbility = getRandomBuffAbility(guard);
			ability * offensiveAbility = getRandomOffensiveAbility(guard);

			if(offensiveAbility == NULL && buffAbility == NULL){
				return attackModule(guard, player, thisGroupContainer, thisField, thisMoveNodeMeta);
			}else if(offensiveAbility != NULL && buffAbility != NULL){
				if(rand() % 2){
					offensiveAbility = NULL;
				} else{
					buffAbility = NULL;
				}
			}

			if(offensiveAbility != NULL){
				guard->activeAbilities->selectedAbility = offensiveAbility;

				int range = max(abs(guard->playerCharacter->x - guard->targetedIndividual->playerCharacter->x) , abs(guard->playerCharacter->y - guard->targetedIndividual->playerCharacter->y));
				int abilityRange = offensiveAbility->range->effectAndManaArray[offensiveAbility->range->selectedIndex]->effectMagnitude;

				if(abilityRange < range){//dont remove selected ability - it's now on deck
					int toReturn = moveCloserToTarget(guard, guard->targetedIndividual, thisField, thisMoveNodeMeta);

					if(!toReturn){
						//cannot reach target and target out of range, get new closer target
						guard->targetedIndividual = NULL;
						guard->targetedDuration = 0;
						guard->activeAbilities->selectedAbility = NULL;
					}

					return toReturn;
				}else{
					cwrite("CASE 3\n");
					return useAbilityOnTargetedSpace(guard, player, thisGroupContainer, thisField, guard->targetedIndividual->playerCharacter->x, guard->targetedIndividual->playerCharacter->y);
				}
			}else{
				guard->activeAbilities->selectedAbility = buffAbility;
				cwrite("CASE 4\n");
				return useAbilityOnTargetedSpace(guard, player, thisGroupContainer, thisField, guard->playerCharacter->x, guard->playerCharacter->y);
			}
		}else{
			return attackModule(guard, player, thisGroupContainer, thisField, thisMoveNodeMeta);
		}
	}

	//Supportive
	else{
		cwrite("Being supportive~~ :3");

		if(guard->abilities->numAbilities == 0){
			return attackModule(guard, player, thisGroupContainer, thisField, thisMoveNodeMeta);
		}

		if(guard->thisBehavior->isTactical && isGreaterThanPercentage(rand() % 100, 100, 30)){
			return tacticalModule(guard, player, thisGroupContainer, thisField, thisMoveNodeMeta);
		}

		if(((guard->mana < (getTotalMana(guard)/2)) && isGreaterThanPercentage(rand() % 100, 100, 30)) || !canUseAnyAbilities(guard)){
			channelMana(guard);
			return 0;
		}

		individualGroup * alliesInRange = getAlliesInRange(guard, thisField, 8);

		if(selectHealingAbility(guard)){
			individual * ally  = allyRequiringHealing(guard, alliesInRange);

			if(ally != NULL){
				free(alliesInRange);
				guard->allyIndividual = ally;

				if(abilityInRangeOfIndividual(guard->activeAbilities->selectedAbility, guard, guard->allyIndividual)){
					cwrite("CASE 5\n");
					return useAbilityOnTargetedSpace(guard, player, thisGroupContainer, thisField,  guard->allyIndividual->playerCharacter->x, guard->allyIndividual->playerCharacter->y);
				}else{
					int moving = moveCloserToTarget(guard, guard->allyIndividual, thisField, thisMoveNodeMeta);

					if(moving){
						return moving;
					}else{
						//cannot reach target and target out of range, get new closer target
						guard->allyIndividual = NULL;
						guard->targetedDuration = 0;
						guard->activeAbilities->selectedAbility = NULL;
					}
				}
			}
		}

		ability * randomBuffAbility = getRandomBuffAbility(guard);

		if(randomBuffAbility == NULL){
			channelMana(guard);
			return 0;
		}

		guard->activeAbilities->selectedAbility = randomBuffAbility;

		if(randomBuffAbility->type == 'd' || randomBuffAbility->type == 'i'){
			free(alliesInRange);
			cwrite("CASE 6\n");
			return useAbilityOnTargetedSpace(guard, player, thisGroupContainer, thisField,  guard->playerCharacter->x, guard->playerCharacter->y);
		}

		guard->allyIndividual = alliesInRange->individuals[rand() % alliesInRange->numIndividuals];

		free(alliesInRange);

		if(abilityInRangeOfIndividual(guard->activeAbilities->selectedAbility, guard, guard->allyIndividual)){
			cwrite("CASE 7\n");
			return useAbilityOnTargetedSpace(guard, player, thisGroupContainer, thisField,  guard->allyIndividual->playerCharacter->x, guard->allyIndividual->playerCharacter->y);
		}else{
			int moving = moveCloserToTarget(guard, guard->allyIndividual, thisField, thisMoveNodeMeta);

			if(moving){
				return moving;
			}else{
				//cannot reach target and target out of range, get new closer target
				guard->allyIndividual = NULL;
				guard->targetedDuration = 0;
				guard->activeAbilities->selectedAbility = NULL;
			}
		}

		return 0;
	}
}

int withinXSpacesOfTarget(individual * thisIndividual, individual * targetIndividual, int numSpaces){
	return (numSpaces >= max(abs(targetIndividual->playerCharacter->x - thisIndividual->playerCharacter->x),abs(targetIndividual->playerCharacter->y - thisIndividual->playerCharacter->y)));
}

int atDesiredLocation(individual * thisIndividual){
	if(thisIndividual->playerCharacter->x == thisIndividual->desiredLocation->x && thisIndividual->playerCharacter->y == thisIndividual->desiredLocation->y){
		return 1;
	}

	return 0;
}

void addIndividualsInLineOfSight(individual * thisIndividual, individualGroup * thisGroup, individualGroup * targetGroup, field * thisField){
	int i, numIndividualsPassed = 0;

	for(i = 0; i < thisGroup->MAX_INDIVIDUALS; i++){
		if(thisGroup->individuals[i] != NULL){
			if(isInLineOfSight(thisIndividual, thisGroup->individuals[i], thisField)){
				addIndividualToGroup(targetGroup, thisGroup->individuals[i]);
			}

			numIndividualsPassed++;
			if(numIndividualsPassed == thisGroup->numIndividuals){
				numIndividualsPassed = 0;
				break;
			}
		}
	}
}

individual * getClosestIndividual(individual * thisIndividual, individualGroup * thisGroup){
	int i, individualsPassed = 0, distance = 99;
	individual * closestIndividual = NULL;

	for(i = 0; i < thisGroup->MAX_INDIVIDUALS; i++){
		if(thisGroup->individuals[i] != NULL){
			int thisDistance = max(abs(thisIndividual->playerCharacter->x - thisGroup->individuals[i]->playerCharacter->x) , abs(thisIndividual->playerCharacter->y - thisGroup->individuals[i]->playerCharacter->y));

			if(thisDistance < distance){
				closestIndividual = thisGroup->individuals[i];
				distance = thisDistance;
			}

			individualsPassed++;

			if(individualsPassed == thisGroup->numIndividuals){
				break;
			}
		}
	}

	return closestIndividual;
}

//also deal with setting targeted individual
individual * getDangerousIndividualNearByInLoS(individual * friendlyIndividual, individual *  player, groupContainer * thisGroupContainer, field * thisField){
	individualGroup * dangerousIndividuals = initGroup();

	if(friendlyIndividual->thisBehavior->isHostileToPlayer && isInLineOfSight(friendlyIndividual, player, thisField)){
		addIndividualToGroup(dangerousIndividuals, player);
		addIndividualsInLineOfSight(friendlyIndividual, thisGroupContainer->allies, dangerousIndividuals, thisField);
	}

	addIndividualsInLineOfSight(friendlyIndividual, thisGroupContainer->enemies, dangerousIndividuals, thisField);
	addIndividualsInLineOfSight(friendlyIndividual, thisGroupContainer->beasts, dangerousIndividuals, thisField);

	if(friendlyIndividual->thisBehavior->isHostileToPlayer){
		addIndividualsInLineOfSight(friendlyIndividual, thisGroupContainer->allies, dangerousIndividuals, thisField);
	}

	individual * toReturn = getClosestIndividual(friendlyIndividual, dangerousIndividuals);

	free(dangerousIndividuals);

	return toReturn;
}

void findDangerousIndividualNearBy(individual * friendlyIndividual, individual * player, groupContainer * thisGroupContainer, field * thisField, int maxDistance){
	if(friendlyIndividual->targetedIndividual != NULL){
		if(friendlyIndividual->targetedIndividual->hp <= 0
				|| isAlly(friendlyIndividual, friendlyIndividual->targetedIndividual)
				|| (friendlyIndividual->targetedIndividual->isPlayer && !friendlyIndividual->thisBehavior->isHostileToPlayer)
				|| maxDistance < max(abs(friendlyIndividual->playerCharacter->x - friendlyIndividual->targetedIndividual->playerCharacter->x) , abs(friendlyIndividual->playerCharacter->y - friendlyIndividual->targetedIndividual->playerCharacter->y))){
			friendlyIndividual->targetedIndividual = getClosestEnemyInLoS(friendlyIndividual, thisField);// getDangerousIndividualNearByInLoS(friendlyIndividual, player, thisGroupContainer, thisField);

			if(friendlyIndividual->targetedIndividual != NULL){
				friendlyIndividual->targetedDuration = (rand() % 4) + 6;
			}else{
				friendlyIndividual->thisBehavior->isSurrounded = 0;
			}

			return;
		}

		friendlyIndividual->targetedDuration--;

		if(friendlyIndividual->targetedDuration == 0){
			friendlyIndividual->targetedIndividual = getClosestEnemyInLoS(friendlyIndividual, thisField); //getDangerousIndividualNearByInLoS(friendlyIndividual, player, thisGroupContainer, thisField);

			if(friendlyIndividual->targetedIndividual != NULL){
				friendlyIndividual->targetedDuration = (rand() % 4) + 6;
			}else{
				friendlyIndividual->thisBehavior->isSurrounded = 0;
			}

			return;
		}

	}else{
		if(friendlyIndividual->targetedDuration > 0){
			friendlyIndividual->targetedDuration = 0;
		}

		friendlyIndividual->targetedIndividual = getClosestEnemyInLoS(friendlyIndividual,thisField);//getDangerousIndividualNearByInLoS(friendlyIndividual, player, thisGroupContainer, thisField);

		if(friendlyIndividual->targetedIndividual != NULL){
			friendlyIndividual->targetedDuration = (rand() % 4) + 6;
		}else{
			friendlyIndividual->thisBehavior->isSurrounded = 0;
		}

		return;
	}
}

int npcAction(individual * npc, individual * player, groupContainer * thisGroupContainer, field * thisField, moveNodeMeta ** thisMoveNodeMeta){
	//Restore Mana
	if(isLowOnMana(npc)){
		if(tryRestoreMana(npc)){
			return 0;
		}
	}

	//Restore HP
	if(isLowOnHP(npc)){
		if(tryHeal(npc, player, thisGroupContainer, thisField)){
			return 0;
		}
	}

	//set target to nearby enemy in line of sight, if not disable isSurrounded
	findDangerousIndividualNearBy(npc, player, thisGroupContainer, thisField, 8);

	if(npc->targetedIndividual != NULL){
		cord * targetSpace = findRetreatSpace(npc, thisGroupContainer, thisField, npc->targetedIndividual->playerCharacter->x, npc->targetedIndividual->playerCharacter->y);
		int x = targetSpace->x;
		int y = targetSpace->y;

		free(targetSpace);

		if(!npc->thisBehavior->isSurrounded && !(abs(targetSpace->x - npc->playerCharacter->x) <= 1 && abs(targetSpace->y - npc->playerCharacter->y) <= 1)){
			int toReturn = moveToSelectedLocation(npc, thisField, thisMoveNodeMeta, x, y);

			return toReturn;
		}else{
			//TODO: needs to be more precise: should only attack individual if in range, not to seek them out.
			if(npc->thisBehavior->isSurrounded || withinXSpacesOfTarget(npc, npc->targetedIndividual, 5)){
				npc->thisBehavior->isSurrounded = 1;
				int toReturn = attackModule(npc, player, thisGroupContainer,thisField, thisMoveNodeMeta);

				if(npc->targetedIndividual == NULL){
					npc->thisBehavior->isSurrounded = 0;
				}

				return toReturn;
			}

			npc->remainingActions--;
			return 0;
		}

	}else{
		if(!atDesiredLocation(npc)){
			return moveToSelectedLocation(npc, thisField, thisMoveNodeMeta, npc->desiredLocation->x, npc->desiredLocation->y);
		}

		npc->remainingActions--;
		return 0;
	}

}

int initializeEnemyTurn(individualGroup * enemies, individual * player, field * thisField, moveNodeMeta ** thisMoveNodeMeta){

	if (enemies->numIndividuals == 0) {
		return 1;
	}

	//initial currentIndividualIndex check
	if (enemies->currentIndividualIndex == -1) {
		nextAvailableIndividualIndex(enemies);
	}

	individual * tmpIndividual = enemies->individuals[enemies->currentIndividualIndex];

	if(startTurn(tmpIndividual)){
		deleteIndividiaulFromGroup(enemies, tmpIndividual);
		removeIndividualFromField(thisField, tmpIndividual->playerCharacter->x, tmpIndividual->playerCharacter->y);
		return 1;
	}

	if(tmpIndividual->remainingActions < 0){
		return 0;
	}

	return 0;
}

void enemyAttackAction(individual * enemy, individual * player){
	attackIfInRange(enemy,player);
}

void destroyNodeArr(nodeArr * thisNodeArr){
	int i;

	for(i = 0; i < thisNodeArr->size; i++){
		if(thisNodeArr->nodeArray[i] != NULL){
			free(thisNodeArr->nodeArray[i]);
		}
	}

	free(thisNodeArr);
}
