/*
 * individual.c
 *
 *  Created on: Apr 14, 2015
 *      Author: Adrian
 */

#include"./headers/individual_pub_methods.h"
#include<stdlib.h>

int isRandomized = 0;

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

	if(!isRandomized){
		srand(time(NULL));
		isRandomized = 1;
	}

	attackDamage = rand() % (thisIndividual->maxDam - thisIndividual->minDam);
	attackDamage = attackDamage + thisIndividual->minDam;

	targetIndividual->hp = targetIndividual->hp - attackDamage;

	if(targetIndividual->hp <= 0){ //target is dead
		return 1;
	}else{ //non-fatal blow
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
