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
	toReturn->backpack = malloc(sizeof(inventory));
	return toReturn;
}

int defineIndividual(individual * thisIndividual, int imageID, int ID, COLORREF rgb, char * name, int direction, int x,
		int y, int totalHP, int totalActions, int AC, int attack, int maxDam, int minDam,  char critType[3],
		int range, int mvmt, int bluntDR, int chopDR, int slashDR, int pierceDR, int earthDR, int fireDR,
		int waterDR, int lightningDR, int earthWeakness, int fireWeakness, int waterWeakness,
		int lightiningWeakness){

	int i;
	BITMAP bm;

	thisIndividual->playerCharacter->imageID = imageID;
	thisIndividual->ID = ID;
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

	thisIndividual->backpack = malloc(sizeof(inventory));
	thisIndividual->backpack->inventorySize = 0;

	for(i = 0; i < 40; i++){
		thisIndividual->backpack->inventoryArr[i] = NULL;
	}

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

	thisIndividual->bluntDR = bluntDR;
	thisIndividual->chopDR = chopDR;
	thisIndividual->slashDR = slashDR;
	thisIndividual->pierceDR = pierceDR;

	thisIndividual->earthDR = earthDR;
	thisIndividual->fireDR = fireDR;
	thisIndividual->waterDR = waterDR;
	thisIndividual->lightningDR = lightningDR;

	thisIndividual->earthWeakness = earthWeakness;
	thisIndividual->fireWeakness = fireWeakness;
	thisIndividual->waterWeakness = waterWeakness;
	thisIndividual->lightiningWeakness = lightiningWeakness;

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
	int totalAttack = d20 + thisIndividual->attack;
	int totalDef = targetIndividual->AC;
	int i;

	if(d20 == 20){
		return damageIndividual(thisIndividual, targetIndividual, 1);

	} else if(d20 == 1){ //THE natural one.
		cwrite("Where'd you learn to fight?\n");
		return 0;
	}

	for(i = 0; i < thisIndividual->backpack->inventorySize; i++){
		if(thisIndividual->backpack->inventoryArr[i]->isEquipt){
			 totalAttack += thisIndividual->backpack->inventoryArr[i]->attackMod;
		}
	}


	for(i = 0; i < targetIndividual->backpack->inventorySize; i++){
		if(targetIndividual->backpack->inventoryArr[i]->isEquipt){
			totalDef += targetIndividual->backpack->inventoryArr[i]->acMod;
		}
	}

	if(totalAttack >= totalDef){ //Tie goes to attacker, of course.
		return damageIndividual(thisIndividual, targetIndividual, 0);
	}else{ //miss
		sendMissedDialog(thisIndividual->name,targetIndividual->name,d20,totalDef);
		return 0;
	}
}

int damageIndividual(individual *thisIndividual, individual *targetIndividual, int isCrit){
	int totalDamage = 0, i, totalDR, maxDamTotal, minDamTotal;
	char attackType = 'b'; //for now, default is blunt (punching)
	item * tmpItem;
	maxDamTotal = thisIndividual->maxDam;
	minDamTotal = thisIndividual->minDam;

	thisIndividual->hasAttacked = 1;

	for(i = 0; i < thisIndividual->backpack->inventorySize; i++){
		tmpItem = thisIndividual->backpack->inventoryArr[i];

		if(tmpItem->isEquipt){

			if(tmpItem->type == 'w'){
				attackType = tmpItem->weponDamageType;
				maxDamTotal += tmpItem->maxDamMod;
				minDamTotal += tmpItem->minDamMod;
			}

			totalDamage += tmpItem->damMod;
		}
	}

	if(minDamTotal < 0){
		minDamTotal = 0;
	}

	if(minDamTotal > maxDamTotal){
		maxDamTotal = minDamTotal;
	}

	if(isCrit){
		cwrite("CRITICAL HIT!!!\n");
		totalDamage = calcCrit(thisIndividual, maxDamTotal, minDamTotal);
	}else{
		totalDamage = rand() % (maxDamTotal - minDamTotal);
		totalDamage = totalDamage + minDamTotal;
	}

	totalDamage = totalDamage - calcDR(targetIndividual, attackType);

	if(totalDamage < 1){
		totalDamage = 0;
	}

	sendHitDialog(thisIndividual->name, targetIndividual->name, thisIndividual->maxDam, totalDamage);
	targetIndividual->hp = targetIndividual->hp - totalDamage;

	if(targetIndividual->hp <= 0){ //target is dead
		sendDeathDialog(targetIndividual->name, thisIndividual->name);
		removeFromExistance(targetIndividual->ID);
		return 1;
	}else{ //non-fatal blow
		return 0;
	}
}

int calcDR(individual * targetIndividual, char attackType){
	int i, totalDR = 0;
	switch (attackType) {
	case 'b':
		for (i = 0; i < targetIndividual->backpack->inventorySize; i++) {
			if (targetIndividual->backpack->inventoryArr[i]->isEquipt) {
				totalDR += targetIndividual->backpack->inventoryArr[i]->bluntDRMod;
			}
		}
		break;
	case 'c':
		for (i = 0; i < targetIndividual->backpack->inventorySize; i++) {
			if (targetIndividual->backpack->inventoryArr[i]->isEquipt) {
				totalDR += targetIndividual->backpack->inventoryArr[i]->chopDRMod;
			}
		}
		break;
	case 's':
		for (i = 0; i < targetIndividual->backpack->inventorySize; i++) {
			if (targetIndividual->backpack->inventoryArr[i]->isEquipt) {
				totalDR += targetIndividual->backpack->inventoryArr[i]->slashDRMod;
			}
		}
		break;
	case 'p':
		for (i = 0; i < targetIndividual->backpack->inventorySize; i++) {
			if (targetIndividual->backpack->inventoryArr[i]->isEquipt) {
				totalDR += targetIndividual->backpack->inventoryArr[i]->pierceDRMod;
			}
		}
		break;
	}

	return totalDR;
}

int calcCrit(individual * thisIndividual, int maxDamTotal, int minDamTotal){
	if(strcmp(thisIndividual->critType, "MAX") == 0){
		return thisIndividual->maxDam;
	} else if(strcmp(thisIndividual->critType, "DUB") == 0){
		int attackDamage = rand() % (maxDamTotal - minDamTotal);
		attackDamage = (attackDamage + minDamTotal) * 2;
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

int addItemToIndividual(inventory * backpack, item * newItem){
	int i, availableSpot;
	if(backpack->inventorySize < 40){
		for(i = 0; i < 40; i++){
			if(backpack->inventoryArr[i] == NULL){
				backpack->inventoryArr[backpack->inventorySize] = newItem;
				backpack->inventorySize++;
				return 1;
			}
		}

		return 0;
	}else{
		return 0;
	}
}

/*
 * HEY!
 * When you actually implement this, check that thisItem->type isnt
 * a normal item before hand, e.g. type != 'i'
 */
void equipItem(inventory * backpack, item * thisItem){
	int i;

	for(i = 0; i < backpack->inventorySize; i++){
		if(backpack->inventoryArr[i]->type == thisItem->type){
			backpack->inventoryArr[i]->isEquipt = 0;
			break;
		}
	}

	thisItem->isEquipt = 1;
}

item * removeItemFromInventory(inventory * backpack, item * thisItem){
	int i;
	item * removedItem;

	for(i = 0; i < backpack->inventorySize; i++){
		if(backpack->inventoryArr[i] == thisItem){
			removedItem = backpack->inventoryArr[i];
			removedItem->isEquipt = 0;

			backpack->inventoryArr[i] = NULL;
			backpack->inventorySize--;

			return removedItem;
		}
	}
}


