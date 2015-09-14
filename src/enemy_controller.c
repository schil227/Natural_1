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
	int x;
	for(x = 0; x < 9; x++){
		newActiveNodes[x] = NULL;
	}
//	space** tmpSpace = NULL;
	int dx,dy,newX,newY, index=0;

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
	node ** newActiveNodes = malloc(sizeof(node*)*100);
	int x;
	for(x = 0; x < 100; x++){
		newActiveNodes[x] = NULL;
	}
	int numNewActiveNodes = 0;
	int i = 0;
//	printf("New pathFind iteration \n");

	while(activeNodes[i] != NULL){
	//	printf("active node %d \n", i);
		int j = 0;
		node ** newNodes = getNewActiveNodes(activeNodes[i], allNodes, thisField);

		while(newNodes[j] != NULL){
		//	printf("checking new node: [%d,%d] == [%d,%d] \n", newNodes[j]->x, newNodes[j]->y, targetX, targetY);

			if(newNodes[j]->x == targetX && newNodes[j]->y == targetY){ //found target space
		//		printf("found the target space!!! \n");
				return (newNodes[j]);
			}

			numNewActiveNodes++;
			addNodeToList(newNodes[j], newActiveNodes);
			j++;

		}
		free(newNodes);
		i++;
	}

	free(activeNodes);

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
 * getSpaceClosestToPlayer:
 * This is the pathfinding algorithm, which finds the shortest path between two individuals.
 * This function returns a struct nodeArr, which is a list of node pointers (node closest to
 *  thisIndividual first) and the size of the nodePointers
 *
 *  Note that this algorithm does not include 'weighted' spaces, meaning that each space takes
 *  the same movement-cost to traverse (
 */
nodeArr * getFullNodePath(field * thisField, int thisX,int thisY,int  targetX, int targetY){
//	printf("starting!\n");
	node * startingNode = createNewNode(0,thisX,thisY); //startingSpace);
	startingNode->previousNode = NULL;
	node ** allNodes = malloc(sizeof(node*)*300);
	node ** activeNodes = malloc(sizeof(node*)*100);
	allNodes[0] = NULL;
	activeNodes[0] = NULL;

	int i;
	for(i = 0; i < 300; i++){
		if(i < 100){
			activeNodes[i] = NULL;
		}
		allNodes[i] = NULL;
	}
	allNodes[0] = startingNode; //this works, the other doesn't
	activeNodes[0] = startingNode;
//	printf("starting:3\n");
	node * endNode = pathFind(targetX, targetY, allNodes, activeNodes, thisField);

	nodeArr * resultArr = malloc(sizeof(nodeArr));

	resultArr->size = 0;

	if(endNode->pathLength != -1){
		int size = 0;

		node * tmpNode = endNode;
		while(tmpNode->previousNode != NULL){
			tmpNode->isFinalPathNode = 1;
			tmpNode = tmpNode->previousNode;
			size++;
		}

		resultArr->size = size;
		resultArr->nodeArray = malloc(sizeof(node*)*size);

		tmpNode = endNode;

		for(size=resultArr->size-1; size >=0; size--){
			resultArr->nodeArray[size] = tmpNode;
			tmpNode = (node*) tmpNode->previousNode;
		}

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
	int i = 0;
	for(i; i < size; i++){
		nodeArr[size] = NULL;
	}
}

int spaceIsTaken(node * thisNode, field * thisField){
	int a = spaceIsAvailable(thisField, thisNode->x, thisNode->y);
	printf("a:%d\n",a);
	if(a == 0){
		return 0;
	}else{
		return 1;
	}
}

node * findOpenNode(node * endNode, node ** activeNodes, individual * thisIndividual, int moveRange, int distanceFromLastNode, field * thisField, node ** allNodes){
	int i = 0;
	node ** newActiveNodes = malloc(sizeof(node*)*300);

	for(i; i < 300; i++){
		newActiveNodes[i] = NULL;
	}

	i = 0;

	//check for free spaces in active nodes
	while(activeNodes[i] != NULL){
		printf("currentNode:[%d,%d]\n",activeNodes[i]->x, activeNodes[i]->y);
		if( !spaceIsTaken(activeNodes[i], thisField)){
			return activeNodes[i];
		}else{
			printf("space was taken:[%d,%d]\n",activeNodes[i]->x, activeNodes[i]->y);
		}

		addNodeToList(activeNodes[i], allNodes);

		if(moveRange > 0){
			printf("before 1st call\n");
			fflush(stdout);
			node ** tmpNodes = getNewActiveNodes(activeNodes[i], allNodes, thisField); // filters out blocked nodes and nodes in allNodes
			printf("1st getNewActiveNodes Call\n");
			fflush(stdout);
			int j = 0;

			while(tmpNodes[j] != NULL){

				if(!spaceIsTaken(tmpNodes[j], thisField)){
					addNodeToList(tmpNodes[j], newActiveNodes);
				}

				j++;
			}
		}

		i++;
	}

	if(newActiveNodes[0] == NULL){
		if( endNode->previousNode == NULL){
			return NULL;
		}else{
			addNodeToList(endNode->previousNode, newActiveNodes);

			return findOpenNode( endNode->previousNode, newActiveNodes, thisIndividual, distanceFromLastNode, distanceFromLastNode +1, thisField, allNodes);
		}
	}else{
		return findOpenNode(endNode, newActiveNodes, thisIndividual, moveRange-1, distanceFromLastNode, thisField, allNodes);
	}

}

nodeArr * processPath(field * thisField, nodeArr * nodePath, individual * thisIndividaul){
	int nodeIndex = max(min(nodePath->size, thisIndividaul->mvmt)-1, 0);
	nodeArr * nullNode = malloc(sizeof(nodeArr*));
	nullNode->size = 0;

	if(nodeIndex > 0){ //going somewhere
		node * endNode = nodePath->nodeArray[nodeIndex];
		node ** allNodes = malloc(sizeof(node*)*300);
		node ** activeNodes= malloc(sizeof(node*)*300);
		int i;
		for(i = 0; i < 300; i++){
			activeNodes[i] = NULL;
			allNodes[i] = NULL;
		}

		addNodeToList(endNode, activeNodes);
		node * targetNode = findOpenNode(endNode, activeNodes, thisIndividaul, 0, 1, thisField, allNodes);

		if(targetNode != NULL){
			return getFullNodePath(thisField, thisIndividaul->playerCharacter->x, thisIndividaul->playerCharacter->y, targetNode->x, targetNode->y);
		} else {
			return nullNode;
		}
	}

	return nullNode;
}

nodeArr * getSpaceClosestToPlayer(field * thisField, individual * thisIndividual, individual * targetIndividual){
	nodeArr * resultArr = getFullNodePath(thisField, thisIndividual->playerCharacter->x, thisIndividual->playerCharacter->y, targetIndividual->playerCharacter->x, targetIndividual->playerCharacter->y);

	return processPath(thisField, resultArr, thisIndividual);
}


//HWND hwnd, MSG msg, WPARAM wParam, LPARAM lParam,
void enemyAction( individual * enemy, field * thisField, individual * player){
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
			if(setIndividualSpace(thisField,enemy, resultArr->nodeArray[i]->x, resultArr->nodeArray[i]->y) == 0){
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
	printf("\n");



	attackIfInRange(enemy,player);
}
