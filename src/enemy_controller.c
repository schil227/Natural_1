/*
 * enemy_controller.c
 *
 *  Created on: May 4, 2015
 *      Author: Adrian
 */
#include"./headers/enemy_controller_pub_methods.h"

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
	printf("\n");
	nodeList[i] = newNode;

}

void printNodeList(node ** nodeList){
	int i = 0;
	while(nodeList[i] != NULL){
		printf("[%d,%d] ", nodeList[i]->x, nodeList[i]->y);
		i++;
	}
	printf("\n");
}

node ** getNewActiveNodes(node * parentNode, node ** allNodes, field * thisField){
	node** newActiveNodes = malloc(sizeof(node*)*9);
	int x;
	for(x = 0; x < 9; x++){
		newActiveNodes[x] = NULL;
	}
//	space** tmpSpace = NULL;
	int dx,dy,newX,newY, index=0;

	printf("getting new active nodes \n");
	if (parentNode->pathLength < 100) {
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
	printf("index: %d\n", index);
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
	printf("New pathFind iteration \n");

	while(activeNodes[i] != NULL){
		printf("active node %d \n", i);
		int j = 0;
		node ** newNodes = getNewActiveNodes(activeNodes[i], allNodes, thisField);

		while(newNodes[j] != NULL){
			printf("checking new node: [%d,%d] == [%d,%d] \n", newNodes[j]->x, newNodes[j]->y, targetX, targetY);

			if(newNodes[j]->x == targetX && newNodes[j]->y == targetY){ //found target space
				printf("found the target space!!! \n");
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
		printf("Path not found.");
		return createNewNode(-1,-1,-1);
	}

	printf("recurring on the following spaces:");
	printNodeList(newActiveNodes);
	printf("all nodes:");
	printNodeList(allNodes);
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
nodeArr * getSpaceClosestToPlayer(field * thisField, individual * thisIndividual, individual * targetIndividual){
	printf("starting!\n");
	node * startingNode = createNewNode(0,thisIndividual->playerCharacter->x,thisIndividual->playerCharacter->y); //startingSpace);
	startingNode->previousNode = NULL;
	node ** allNodes = malloc(sizeof(node**)*300);
	node ** activeNodes = malloc(sizeof(node**)*100);
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
	printf("starting:3\n");
	node * endNode = pathFind(targetIndividual->playerCharacter->x, targetIndividual->playerCharacter->y, allNodes, activeNodes, thisField);

	nodeArr * resultArr = malloc(sizeof(nodeArr));

	if(endNode->pathLength != -1){
		int size = 0;
		printf("path Found:\n");
		node * tmpNode = endNode;
		while(tmpNode->previousNode != NULL){
			printf("[%d,%d]\n", tmpNode->x, tmpNode->y);
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

//HWND hwnd, MSG msg, WPARAM wParam, LPARAM lParam,
void enemyAction( individual * enemy, field * thisField, individual * player){
	nodeArr * resultArr = getSpaceClosestToPlayer(thisField, enemy, player);
	int i;
	int animateloop = 1;
	int size = min(resultArr->size, enemy->mvmt);

	moveNode * rootMoveNode = malloc(sizeof(rootMoveNode));
	moveNode ** tmpMoveNode = &rootMoveNode;

	for(i = 0; i < size; i++){

		printf("moving to: [%d,%d]\n",resultArr->nodeArray[i]->x,resultArr->nodeArray[i]->y);
		if(setIndividualSpace(thisField,enemy, resultArr->nodeArray[i]->x, resultArr->nodeArray[i]->y)==0){
			printf("no go!\n");
			break; // path is blocked by individual
		}
	}

	attackIfInRange(enemy,player);
}
