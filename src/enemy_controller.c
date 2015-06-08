/*
 * enemy_controller.c
 *
 *  Created on: May 4, 2015
 *      Author: Adrian
 */
#include"./headers/field_pub_methods.h"

typedef struct {
	int pathLength;
	int x;
	int y;
	space **currentSpace;
	struct node ** previousNode;
} node;

node * newNode(int pathLength, int x, int y, space**currentSpacePointer){
	node* thisNode = malloc(sizeof(node));
	thisNode->previousNode; // = malloc(sizeof(node));
	thisNode->x = x;
	thisNode->y = y;
	thisNode->currentSpace = currentSpacePointer;
	return thisNode;
}

void getSpaceClosestToPlayer(field * this_field, individual * thisIndividual, individual * targetIndividual){
	space ** startingSpace = getSpaceAddressFromField(field, thisIndividual->playerCharacter->x, thisIndividual->playerCharacter->y);
	space ** targetSpace = getSpaceAddressFromField(field, targetIndividual->playerCharacter->x, targetIndividual->playerCharacter->y);
	node * startingNode = newNode(0,thisIndividual->playerCharacter->x,thisIndividual->playerCharacter->y,startingSpace);

}

int containsNode(int x, int y, node** nodeList){
	int i = 0;
	while(nodeList[i] != NULL){
		if(nodeList[i]->x == x && nodeList[i]->y == y){
			return 1;
		}
		i++;
	}
	return 0;
}

//TODO make this less dumb, keep track of the size in an object
void addNodeToList(node * newNode, node ** nodeList){
	int i = 0;

	while(nodeList[i] != NULL){
		i++;
	}

	nodeList[i] = &newNode;
	nodeList[i+1] = NULL;

}

node ** getNewActiveNodes(node * parentNode, node ** allNodes, field * thisField){
	node** newActiveNodes[9];
	space** tmpSpace;
	int dx,dy,newX,newY, index=0;

	for(dx = -1; dx < 2; dx++){
		for(dy = -1; dy < 2; dy++){
			newX = parentNode->x+dx;
			newY = parentNode->y+dy;
			tmpSpace = getSpaceAddressFromField(thisField,newX,newY);
			if((*tmpSpace) != NULL && (*tmpSpace)->isPassable && containsNode(newX,newY) == 0){
				node * newNode = newNode(parentNode->pathLength+1, newX, newY,tmpSpace);
				newNode->previousNode = &parentNode;
				addNodeToList(newNode, allNodes);
				newActiveNodes[index] = &newNode;
				index++;
			}
		}
	}

	newActiveNodes[index] = NULL;
	return newActiveNodes;
}

node* pathFind(int targetX, int targetY, node ** allNodes, node ** activeNodes, field * thisField){
	node ** newActiveNodes;
	int numNewActiveNodes = 0;
	int i = 0;


	while(activeNodes[i] != NULL){
		int j = 0;
		node ** newNodes = getNewActiveNodes(*(activeNodes[i]), allNodes, thisField);
		while(newNodes[j] != NULL){
			if((*newNodes[j])->x == targetX && (*newNodes[j])->y){ //found target space
				//TODO: DESTROY OTHER NODES!!//
				return (*newNodes[j]);
			}


		}
	}
}

void enemyAction(individual * enemy, field * thisField, individual * player){
	wanderAround(thisField, enemy);
	attackIfInRamge(enemy,player);
}
