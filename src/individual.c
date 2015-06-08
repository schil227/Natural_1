/*
 * individual.c
 *
 *  Created on: Apr 14, 2015
 *      Author: Adrian
 */

#include"./headers/individual_pub_methods.h"


int isRandomized = 0;

individual *initIndividual(){
	individual* toReturn = malloc(sizeof(individual));
	toReturn->name = malloc(sizeof(char)*32);
	toReturn->playerCharacter = malloc(sizeof(character));
	toReturn->playerCharacter->image = malloc(sizeof(HBITMAP));
	return toReturn;
}

void destroyIndividual(individual* thisIndividual){
	if(thisIndividual->playerCharacter){ //Null check
		destroyCharacter(thisIndividual->playerCharacter);
	}
	if(thisIndividual->name){ //Null check
		free(thisIndividual->name);
	}

	free(thisIndividual);

}

int attackIndividual(individual *thisIndividual, individual *targetIndividual){
	int attackDamage;
	thisIndividual->hasAttacked = 1;

	attackDamage = rand() % (thisIndividual->maxDam - thisIndividual->minDam);
	attackDamage = attackDamage + thisIndividual->minDam;
	printf("dam:%d\n", attackDamage);
	targetIndividual->hp = targetIndividual->hp - attackDamage;

	if(targetIndividual->hp <= 0){ //target is dead
		return 1;
	}else{ //non-fatal blow
		return 0;
	}
}

void endTurn(individual *thisIndividual){
	printf("player turn ended\n");
	thisIndividual->hasAttacked = 0;
	thisIndividual->remainingActions = thisIndividual->totalActions;
}

int individualWithinRange(individual * thisIndividual, individual * target){
	if(abs(thisIndividual->playerCharacter->x - target->playerCharacter->x) <= thisIndividual->range
	&& abs(thisIndividual->playerCharacter->y - target->playerCharacter->y) <= thisIndividual->range){
		return 1;
	}
	return 0;
}

int attackIfInRamge(individual *thisIndividual, individual *targetIndividual){
	if(individualWithinRange(thisIndividual, targetIndividual)){
		attackIndividual(thisIndividual, targetIndividual);
		return 1;
	}else{
		return 0;
	}
}

void drawPlayer(HDC hdc, HDC hdcBuffer, individual* player){
		HDC hdcMem = CreateCompatibleDC(hdc);
		SelectObject(hdcMem, player->playerCharacter->imageMask);

		BitBlt(hdcBuffer, player->playerCharacter->x*40, player->playerCharacter->y*40, player->playerCharacter->width, player->playerCharacter->height, hdcMem, 0, 0, SRCAND);

		SelectObject(hdcMem, player->playerCharacter->image);// player->image);

		BitBlt(hdcBuffer, player->playerCharacter->x*40, player->playerCharacter->y*40, player->playerCharacter->width, player->playerCharacter->height, hdcMem, 0, 0, SRCPAINT); //was SRCPAINT
		DeleteDC(hdcMem);
}
