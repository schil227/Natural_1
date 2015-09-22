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
//	toReturn->name = malloc(sizeof(char)*32);
	toReturn->playerCharacter = malloc(sizeof(character));
	toReturn->playerCharacter->image = malloc(sizeof(HBITMAP));
	toReturn->playerCharacter->imageMask = malloc(sizeof(HBITMAP));
	return toReturn;
}

int defineIndividual(individual * thisIndividual, int imageID, COLORREF rgb, char * name, int direction, int x,
		int y, int totalHP, int totalActions, int AC, int attack, int maxDam, int minDam,  char critType[3],
		int range, int mvmt){
	BITMAP bm;

	thisIndividual->playerCharacter->imageID = imageID;
	thisIndividual->playerCharacter->image = LoadBitmap(GetModuleHandle(NULL),
			MAKEINTRESOURCE(imageID));

	if(thisIndividual->playerCharacter->image == NULL) {
		return 1;
	}

	thisIndividual->playerCharacter->imageMask = CreateBitmapMask(
			thisIndividual->playerCharacter->image, rgb);

	GetObjectA(thisIndividual->playerCharacter->image, sizeof(bm), &bm);

	thisIndividual->playerCharacter->height = bm.bmHeight;
	thisIndividual->playerCharacter->width = bm.bmWidth;

	thisIndividual->playerCharacter->rgb = rgb;

	strcpy(thisIndividual->name, name);
	thisIndividual->playerCharacter->direction = direction;
	thisIndividual->playerCharacter->x = x;
	thisIndividual->playerCharacter->y = y;
	thisIndividual->totalHP = totalHP;
	thisIndividual->hp = totalHP;
	thisIndividual->totalActions = totalActions;
	thisIndividual->remainingActions = totalActions;
	thisIndividual->AC = AC;
	thisIndividual->attack = attack;
	thisIndividual->maxDam = maxDam;
	thisIndividual->minDam = minDam;
	strcpy(thisIndividual->critType,critType);
	thisIndividual->range = range;
	thisIndividual->mvmt = mvmt;

	thisIndividual->jumpTarget = 0;

	return 0;
}

void destroyIndividual(individual* thisIndividual){
	if(thisIndividual->playerCharacter){ //Null check
		destroyCharacter(thisIndividual->playerCharacter);
	}
//	if(thisIndividual->name){ //Null check
//		free(thisIndividual->name);
//	}

	free(thisIndividual);

}

int attackIndividual(individual *thisIndividual, individual *targetIndividual){
	int d20 = rand() % 20 + 1;

	if(d20 == 20){
		return damageIndividual(thisIndividual, targetIndividual, 1);

	} else if(d20 == 1){ //THE natural one.
		cwrite("Where'd you learn to fight?\n");
		return 0;

	}else if(d20+thisIndividual->attack >= targetIndividual->AC){ //Tie goes to attacker, of course.
		return damageIndividual(thisIndividual, targetIndividual, 0);

	}else{ //miss
		sendMissedDialog(thisIndividual->name,targetIndividual->name,d20,targetIndividual->AC);
		return 0;
	}
}

int damageIndividual(individual *thisIndividual, individual *targetIndividual, int isCrit){
	int attackDamage;
	thisIndividual->hasAttacked = 1;
	if(isCrit){
		cwrite("CRITICAL HIT!!!\n");
		attackDamage = calcCrit(thisIndividual);
	}else{
		attackDamage = rand() % (thisIndividual->maxDam - thisIndividual->minDam);
		attackDamage = attackDamage + thisIndividual->minDam;
	}

	sendHitDialog(thisIndividual->name, targetIndividual->name, thisIndividual->maxDam, attackDamage);
	targetIndividual->hp = targetIndividual->hp - attackDamage;

	if(targetIndividual->hp <= 0){ //target is dead
		sendDeathDialog(targetIndividual->name, thisIndividual->name);
		return 1;
	}else{ //non-fatal blow
		return 0;
	}
}

int calcCrit(individual * thisIndividual){
	if(strcmp(thisIndividual->critType, "MAX") == 0){
		return thisIndividual->maxDam;
	} else if(strcmp(thisIndividual->critType, "DUB") == 0){
		int attackDamage = rand() % (thisIndividual->maxDam - thisIndividual->minDam);
		attackDamage = (attackDamage + thisIndividual->minDam) * 2;
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

int attackIfInRange(individual *thisIndividual, individual *targetIndividual){
	if(individualWithinRange(thisIndividual, targetIndividual)){
		attackIndividual(thisIndividual, targetIndividual);
		return 1;
	}else{
		return 0;
	}
}

void drawIndividual(HDC hdc, HDC hdcBuffer, individual* thisIndividual, shiftData * viewShift){
		HDC hdcMem = CreateCompatibleDC(hdc);
		SelectObject(hdcMem, thisIndividual->playerCharacter->imageMask);

		BitBlt(hdcBuffer, thisIndividual->playerCharacter->x*40 - (viewShift->xShift)*40, thisIndividual->playerCharacter->y*40 - (viewShift->yShift)*40, thisIndividual->playerCharacter->width, thisIndividual->playerCharacter->height, hdcMem, 0, 0, SRCAND);

		SelectObject(hdcMem, thisIndividual->playerCharacter->image);

		BitBlt(hdcBuffer, thisIndividual->playerCharacter->x*40 - (viewShift->xShift)*40, thisIndividual->playerCharacter->y*40 - (viewShift->yShift)*40, thisIndividual->playerCharacter->width, thisIndividual->playerCharacter->height, hdcMem, 0, 0, SRCPAINT);
		DeleteDC(hdcMem);

}

void rotateImage(int angle){


}

