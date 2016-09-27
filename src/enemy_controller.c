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

node ** getNewActiveNodes(node * parentNode, node ** allNodes, field * thisField){
	node** newActiveNodes = malloc(sizeof(node*)*9);
	int x,dx,dy,newX,newY, index=0;
	for(x = 0; x < 9; x++){
		newActiveNodes[x] = NULL;
	}

//	printf("getting new active nodes \n");
	if (parentNode->pathLength < 10) {
		for (dx = -1; dx < 2; dx++) {
			for (dy = -1; dy < 2; dy++) {
				newX = parentNode->x + dx;
				newY = parentNode->y + dy;
//				tmpSpace = getSpaceAddressFromField(thisField, newX, newY);
//				printf("got space \n");

				if (isSpacePassable(thisField, newX, newY) == 1 && containsNode(newX, newY, allNodes) == 0) {
//					printf("found new node. \n");
					node * newNode = createNewNode(parentNode->pathLength + 1, newX, newY);
					newNode->previousNode = parentNode;
					addNodeToList(newNode, allNodes);
					newActiveNodes[index] = newNode;
					index++;
				}
//				printf("done with if \n");
			}
		}
	}
//	printf("index: %d\n", index);
//	printf("done with getNewActiveNodes call \n");
	newActiveNodes[index] = NULL;
	return newActiveNodes;
}

node* pathFind(int targetX, int targetY, node ** allNodes, node ** activeNodes, field * thisField){
	node * newActiveNodes[100];
	int x, i=0, numNewActiveNodes = 0;
	for(x = 0; x < 100; x++){
		newActiveNodes[x] = NULL;
	}

	//	printf("New pathFind iteration \n");

	while(activeNodes[i] != NULL){
	//	printf("active node %d \n", i);
		int j = 0;
		node ** newNodes = getNewActiveNodes(activeNodes[i], allNodes, thisField);

		while(newNodes[j] != NULL){
		//	printf("checking new node: [%d,%d] == [%d,%d] \n", newNodes[j]->x, newNodes[j]->y, targetX, targetY);

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

//	free(activeNodes);

	if(numNewActiveNodes == 0){
//		printf("Path not found.");
		return createNewNode(-1,-1,-1);
	}

//	printf("recurring on the following spaces:");
//	printNodeList(newActiveNodes);
//	printf("all nodes:");
//	printNodeList(allNodes);
	return pathFind(targetX, targetY, allNodes, newActiveNodes, thisField);
}

/*
 * getFullNodePath:
 * This is the pathfinding algorithm, which finds the shortest path between two individuals.
 * This function returns a struct nodeArr, which is a list of node pointers (node closest to
 *  thisIndividual first) and the size of the nodePointers
 *
 *  Note that this algorithm does not include 'weighted' spaces, meaning that each space takes
 *  the same movement-cost to traverse (
 */
nodeArr * getFullNodePath(field * thisField, int thisX,int thisY,int  targetX, int targetY){
	int i, size = 0;
	node * startingNode = createNewNode(0,thisX,thisY); //startingSpace);
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
//	printf("starting:3\n");
	node * endNode = pathFind(targetX, targetY, allNodes, activeNodes, thisField);

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
//		resultArr->nodeArray = malloc(sizeof(node*)*size);

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

node * findOpenNode(node * endNode, node ** activeNodes, individual * thisIndividual, int moveRange, int distanceFromLastNode, field * thisField, node ** allNodes, nodeArr * nodePath){
	int i;
	node * newActiveNodes[300];

	for(i = 0; i < 300; i++){
		newActiveNodes[i] = NULL;
	}

	i = 0;
	printf("endnode: [%d,%d]\n", endNode->x,endNode->y);
	//check for free spaces in active nodes
	while(activeNodes[i] != NULL){
//		addNodeToList(activeNodes[i], allNodes);

		if( !spaceIsTaken(activeNodes[i], thisField)){
			return activeNodes[i];
		}

		if(moveRange > 0){
			node ** tmpNodes = getNewActiveNodes(activeNodes[i], allNodes, thisField); // filters out blocked nodes and nodes in allNodes
			int j = 0;

			while(tmpNodes[j] != NULL){

				if(!spaceIsTaken(tmpNodes[j], thisField)){
					addNodeToList(tmpNodes[j], newActiveNodes);
				}
//				printf("j: %d\n",j);
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
//				printf("called from a\n");
				return findOpenNode( endNode->previousNode, newActiveNodes, thisIndividual, distanceFromLastNode, distanceFromLastNode +1, thisField, allNodes, nodePath);
			}
		}
	}else{
//		printf("called from b\n");
		return findOpenNode(endNode, newActiveNodes, thisIndividual, moveRange-1, distanceFromLastNode, thisField, allNodes,nodePath);
	}

}

nodeArr * processPath(field * thisField, nodeArr * nodePath, individual * thisIndividaul){
	int i;
	int nodeIndex = max(min(nodePath->size, getAttributeSum(thisIndividaul,"mvmt")), 0);


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

		node * targetNode = findOpenNode(endNode, activeNodes, thisIndividaul, 0, 1, thisField, allNodes, nodePath);

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

			return getFullNodePath(thisField, thisIndividaul->playerCharacter->x, thisIndividaul->playerCharacter->y, targetx, targety);
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
	nodeArr * resultArr = getFullNodePath(thisField, thisIndividual->playerCharacter->x, thisIndividual->playerCharacter->y, x, y);

	nodeArr * actualPath = processPath(thisField, resultArr, thisIndividual);
	destroyNodeArr(resultArr);

	return actualPath;
}

nodeArr * getSpaceClosestToPlayer(field * thisField, individual * thisIndividual, individual * targetIndividual){
	nodeArr * resultArr = getFullNodePath(thisField, thisIndividual->playerCharacter->x, thisIndividual->playerCharacter->y, targetIndividual->playerCharacter->x, targetIndividual->playerCharacter->y);

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

individual * acquireTarget(individual * enemy, individual * player, groupContainer * thisGroupContainer, field * thisField){
	int i, individualsPassed = 0, playerInLineOfSight = isInLineOfSight(enemy, player, thisField);

	if(player->hp > 0 && playerInLineOfSight && isGreaterThanPercentage(rand() % 100, 100, 50)){
		return player;
	}

	// for each npc, guards, enemy-npcs etc.
	if(thisGroupContainer->enemies->numIndividuals > 0){
		for(i = 0; i < thisGroupContainer->enemies->MAX_INDIVIDUALS; i++){
			if(thisGroupContainer->enemies->individuals[i] != NULL){
				if(thisGroupContainer->enemies->individuals[i]->faction != enemy->faction && isInLineOfSight(enemy, thisGroupContainer->enemies->individuals[i], thisField)){
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

int checkForTargets(individual * enemy, individual * player, groupContainer * thisGroupContainer, field * thisField){
	if(enemy->targetedIndividual != NULL){
		if(enemy->targetedIndividual->hp <= 0){
			enemy->targetedIndividual = NULL;
		}else{
			int targetIsInSight = isInLineOfSight(enemy, enemy->targetedIndividual, thisField);

			if(!targetIsInSight){
				enemy->targetedDuration -= 2;
			}
			enemy->targetedDuration--;

			if(enemy->targetedDuration == 0){
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
		enemy->targetedIndividual = acquireTarget(enemy, player, thisGroupContainer, thisField);
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

	if(isGreaterThanPercentage(thisIndividual->thisBehavior->abilityAffinity, 100, rand() % 100)){
		thisIndividual->thisBehavior->hasAbilityAffinity = 1;
	}else{
		thisIndividual->thisBehavior->hasAbilityAffinity = 0;
	}

	if(isGreaterThanPercentage(thisIndividual->thisBehavior->tacticalness, 100, rand() % 100)){
		thisIndividual->thisBehavior->istactical = 1;
	}else{
		thisIndividual->thisBehavior->istactical = 0;
	}

	if(isGreaterThanPercentage(thisIndividual->thisBehavior->cowardness, 100, rand() % 50)){ //TODO: make 100 instead of 50
		thisIndividual->thisBehavior->isCowardly = 1;
	}else{
		thisIndividual->thisBehavior->isCowardly = 0;
	}

	thisIndividual->thisBehavior->turnsRemaining = (rand() % 6) + 2;
}

int isAlly(individual * thisIndividual, individual * possibleAlly, groupContainer * thisGroupContainer){
	int i, individualsPassed = 0;

	if(thisIndividual->faction != possibleAlly->faction){
		return 0;
	}

	individualGroup * thisGroup =  getGroupFromIndividual(thisGroupContainer, thisIndividual);

	for(i = 0; i < thisGroup->MAX_INDIVIDUALS; i++){
		if(thisGroup->individuals[i] != NULL){
			individualsPassed++;
		}

		if(thisGroup->individuals[i] == possibleAlly){
			return 1;
		}

		if(individualsPassed == thisGroup->numIndividuals){
			break;
		}
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

			if(nextSpace != NULL && nextSpace->isPassable && (nextSpace->currentIndividual == NULL || isAlly(thisIndividual, nextSpace->currentIndividual, thisGroupContainer)) ){
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

int retreatFromTarget(individual * thisIndividual, groupContainer * thisGroupContainer, field * thisField,  moveNodeMeta ** thisMoveNodeMeta){

	cord * targetSpace = findRetreatSpace(thisIndividual, thisGroupContainer, thisField, thisIndividual->targetedIndividual->playerCharacter->x, thisIndividual->targetedIndividual->playerCharacter->y);
	nodeArr * enemyNodeArr = getSpaceClosestToSpace(thisField, thisIndividual, targetSpace->x, targetSpace->y);

	//nowhere to go, nothing to animate
	if (enemyNodeArr->size == 0) {
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

	thisIndividual->remainingActions--;
	return 1;
}

int noEnemiesInRange(individual * enemy, groupContainer * thisGroupContainer, field * thisField, int range){
	int i, j, startingX, startingY;

	startingX = max(enemy->playerCharacter->x - range, 0);
	startingY = max(enemy->playerCharacter->y - range, 0);

	for(i = startingX; i < startingX + (range*2 + 1); i++){
		for(j = startingY; j < startingY + (range*2 + 1); j++){
			space * tmpSpace = getSpaceFromField(thisField, i, j);

			if(tmpSpace != NULL && tmpSpace->currentIndividual != NULL && !isAlly(enemy, tmpSpace->currentIndividual, thisGroupContainer) ){
				return 0;
			}
		}
	}

	return 1;
}

int enemyAction(individual * enemy, individual * player, groupContainer * thisGroupContainer, field * thisField, moveNodeMeta ** thisMoveNodeMeta){

	//Restore Mana
	if(isLowOnMana(enemy)){
		item * manaRestoringItem = findManaRestoringItem(enemy->backpack);
		if(manaRestoringItem != NULL){
			if(manaRestoringItem->type == 'd' && enemy->activeItems->activeItemsTotal < 40){
				consumeItem(enemy, manaRestoringItem);
				addItemToActiveItemList(enemy, manaRestoringItem);
				free(removeItemFromInventory(enemy->backpack, manaRestoringItem));
				return 0;
			}

			if(manaRestoringItem->type == 'i'){
				consumeItem(enemy, manaRestoringItem);
				free(removeItemFromInventory(enemy->backpack, manaRestoringItem));
				return 0;
			}
		}
	}

	//Restore HP
	if(isLowOnHP(enemy)){
		ability * hpRestoringAbility = getRandomHPRestoringAbility(enemy->mana, enemy->abilities);
		item * hpRestoringItem = getRandomHPRestoringItem(enemy->backpack);
		int selectedHealingMethod = 0;

		if (hpRestoringItem != NULL && hpRestoringAbility != NULL) {
			selectedHealingMethod = (rand() % 2) + 1;
		} else if (hpRestoringItem != NULL) {
			selectedHealingMethod = 1;
		} else if (hpRestoringAbility != NULL) {
			selectedHealingMethod = 2;
		}

		if (selectedHealingMethod == 1) {
			if(hpRestoringItem->itemType == 'd' && enemy->activeItems->activeItemsTotal < 40){
				char msg[128];
				sprintf(msg, "%s used %s.\n", enemy->name, hpRestoringItem->name);
				cwrite(msg);
				consumeItem(enemy, hpRestoringItem);
				addItemToActiveItemList(enemy, hpRestoringItem);
				removeItemFromInventory(enemy->backpack, hpRestoringItem);

				enemy->remainingActions--;
				return 0;
			}

			if(hpRestoringItem->itemType == 'c'){
				char msg[128];
				sprintf(msg, "%s used %s.\n", enemy->name, hpRestoringItem->name);
				cwrite(msg);
				consumeItem(enemy, hpRestoringItem);
				free(removeItemFromInventory(enemy->backpack, hpRestoringItem));
				enemy->remainingActions--;
				return 0;
			}

			if(hpRestoringAbility != NULL){
				selectedHealingMethod = 2;
			}
		}

		if (selectedHealingMethod == 2) {
			if(hpRestoringAbility->type == 'd' || hpRestoringAbility->type == 't'){
				enemy->activeAbilities->selectedAbility = hpRestoringAbility;
				useAbilityOnIndividualsInAOERange(enemy, player, thisGroupContainer, thisField, enemy->playerCharacter->x, enemy->playerCharacter->y);
				enemy->activeAbilities->selectedAbility = NULL;
				enemy->remainingActions--;
				return 0;
			}else if(hpRestoringAbility->type == "i"){
				useAbility(enemy, hpRestoringAbility);
				enemy->remainingActions--;
				return 0;
			}
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
						useAbilityOnIndividualsInAOERange(enemy, player, thisGroupContainer, thisField, enemy->targetedIndividual->playerCharacter->x, enemy->targetedIndividual->playerCharacter->y);
					}else{
						nodeArr * enemyNodeArr = getSpaceClosestToPlayer(thisField, enemy,
								enemy->targetedIndividual);

						//nowhere to go, nothing to animate
						if (enemyNodeArr->size == 0) {
							enemy->activeAbilities->selectedAbility = NULL;
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
				}else{
					enemy->activeAbilities->selectedAbility = NULL;
				}
			}else{//is buff ability
				if(enemy->allyIndividual != NULL){
					if(abilityInRangeOfIndividual(enemy->activeAbilities->selectedAbility, enemy, enemy->allyIndividual)){
						useAbilityOnIndividualsInAOERange(enemy, player, thisGroupContainer, thisField, enemy->allyIndividual->playerCharacter->x, enemy->allyIndividual->playerCharacter->y);
					}else{
						nodeArr * enemyNodeArr = getSpaceClosestToPlayer(thisField, enemy,
								enemy->allyIndividual);

						//nowhere to go, nothing to animate
						if (enemyNodeArr->size == 0) {
							enemy->activeAbilities->selectedAbility = NULL;
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
				}else{
					enemy->activeAbilities->selectedAbility = NULL;
				}
			}
		}else{
			enemy->activeAbilities->selectedAbility = NULL;
		}
	}

	if(enemy->thisBehavior->turnsRemaining == 0){
		rerollBehavior(enemy);
	}else{
		enemy->thisBehavior->turnsRemaining--;
	}

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

	//If pre-disposition to attacking (vs. support)
	if (1) {
		//has attack ability to use?
		if (isGreaterThanPercentage(rand() % 100, 100, 50) && hasOffensiveAbilityInRange(enemy)) {
			int numActions = 1;

			useAbilityOnIndividualsInAOERange(enemy, player, thisGroupContainer, thisField, enemy->targetedIndividual->playerCharacter->x, enemy->targetedIndividual->playerCharacter->y);

			if(enemy->activeAbilities->selectedAbility->actionsEnabled){
				numActions += enemy->activeAbilities->selectedAbility->actions->effectAndManaArray[enemy->activeAbilities->selectedAbility->actions->selectedIndex]->effectMagnitude;
			}

			enemy->activeAbilities->selectedAbility = NULL;

			enemy->remainingActions -= numActions;
			return 0;
		}

		//try physically attacking
		if (individualWithinRange(enemy, enemy->targetedIndividual)) {
			if (attackIndividual(enemy, enemy->targetedIndividual)) {
				deleteIndividiaulFromGroup(getGroupFromIndividual(thisGroupContainer, enemy->targetedIndividual), enemy->targetedIndividual);
				removeIndividualFromField(thisField,
						enemy->targetedIndividual->playerCharacter->x,
						enemy->targetedIndividual->playerCharacter->y);
			}
			enemy->remainingActions--;
			return 0;
		} else { //move closer to target
			nodeArr * enemyNodeArr = getSpaceClosestToPlayer(thisField, enemy,
					enemy->targetedIndividual);

			//nowhere to go, nothing to animate
			if (enemyNodeArr->size == 0) {
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

	int i;
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

