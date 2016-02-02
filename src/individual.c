/*
 * individual.c
 *
 *  Created on: Apr 14, 2015
 *      Author: Adrian
 */

#include"./headers/individual_pub_methods.h"

int isRandomized = 0;

individual *initIndividual(){
	int i;

	individual* toReturn = malloc(sizeof(individual));
//	toReturn->name = malloc(sizeof(char)*32);
	toReturn->playerCharacter = malloc(sizeof(character));
//	toReturn->playerCharacter->image = malloc(sizeof(HBITMAP));
//	toReturn->playerCharacter->imageMask = malloc(sizeof(HBITMAP));

	toReturn->backpack = malloc(sizeof(inventory));
	toReturn->backpack->inventorySize = 0;

	toReturn->activeItems = malloc(sizeof(activeItemList));
	toReturn->activeItems->activeItemsTotal = 0;

	for (i = 0; i < 40; i++) {
		toReturn->backpack->inventoryArr[i] = NULL;
		toReturn->activeItems->activeItemArr[i] = NULL;
	}
	return toReturn;
}

int defineIndividual(individual * thisIndividual, int imageID, int ID, COLORREF rgb, char * name, int direction, int x,
		int y, int totalHP, int totalActions, int totalMana, int AC, int attack, int maxDam, int minDam,  char critType[3],
		int range, int mvmt, int bluntDR, int chopDR, int slashDR, int pierceDR, int earthDR, int fireDR,
		int waterDR, int lightningDR, int earthWeakness, int fireWeakness, int waterWeakness,
		int lightiningWeakness, int dialogID, int gold){

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

	thisIndividual->totalHP = totalHP;
	thisIndividual->hp = totalHP;
	thisIndividual->totalActions = totalActions;
	thisIndividual->remainingActions = totalActions;
	thisIndividual->totalMana = totalMana;
	thisIndividual->mana = totalMana;
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

	thisIndividual->dialogID = dialogID;

	thisIndividual->gold = gold;

	thisIndividual->jumpTarget = 0;

	return 0;
}

void destroyIndividual(individual* thisIndividual){
	int i, itemsPassed;
	if(thisIndividual->playerCharacter){ //Null check
		destroyCharacter(thisIndividual->playerCharacter);
	}

//	if(thisIndividual->activeItems != NULL){
//		itemsPassed = 0;
//		for(i = 0; i < 40; i++){
//			if(thisIndividual->activeItems->activeItemArr[i] != NULL){
////				destroyItem(thisIndividual->activeItems->activeItemArr[i]->thisItem);
////				free(thisIndividual->activeItems->activeItemArr[i]);
//				itemsPassed++;
//			}
//			if(itemsPassed >= thisIndividual->activeItems->activeItemsTotal){
//				break;
//			}
//
//		}
//	}

	free(thisIndividual->activeItems);

//	if(thisIndividual->backpack != NULL){
//		itemsPassed = 0;
//		for(i = 0; i < 40; i++){
//			if(thisIndividual->backpack->inventoryArr[i] != NULL){
//				destroyItem(thisIndividual->backpack->inventoryArr[i]);
//				itemsPassed++;
//			}
//			if(itemsPassed >= thisIndividual->backpack->inventorySize){
//				break;
//			}
//
//		}
//	}

	free(thisIndividual->backpack);

	free(thisIndividual);

}

int attackIndividual(individual *thisIndividual, individual *targetIndividual){
	int d20 = rand() % 20 + 1;
	int totalAttack = d20 + getAttributeSum(thisIndividual, "attack");
	int totalAC = getAttributeSum(targetIndividual,"ac");
	int i;
	item * tmpItem;

	triggerEventOnAttack(targetIndividual->ID);

	if(d20 == 20){
		return damageIndividual(thisIndividual, targetIndividual, 1);

	} else if(d20 == 1){ //THE natural one.
		cwrite("Where'd you learn to fight?\n");
		return 0;
	}

	if(totalAttack >= totalAC){ //Tie goes to attacker, of course.
		return damageIndividual(thisIndividual, targetIndividual, 0);
	}else{ //miss
		sendMissedDialog(thisIndividual->name,targetIndividual->name,d20,totalAC);
		return 0;
	}
}

int damageIndividual(individual *thisIndividual, individual *targetIndividual, int isCrit){
	int totalDamage = 0, i, totalDR, maxDamTotal, minDamTotal;
	char attackType = 'b'; //for now, default is blunt (punching)
	item * tmpItem;
	maxDamTotal = getAttributeSum(thisIndividual, "maxDam");
	minDamTotal = getAttributeSum(thisIndividual, "minDam");

	thisIndividual->hasAttacked = 1;

	for(i = 0; i < 40; i++){
		tmpItem = thisIndividual->backpack->inventoryArr[i];

		if(tmpItem != NULL && tmpItem->isEquipt && tmpItem->type == 'w'){
			attackType = tmpItem->weaponDamageType;
			break;
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

	if(totalDamage > 0){
		triggerEventOnHarm(targetIndividual->ID);
	}

	sendHitDialog(thisIndividual->name, targetIndividual->name, thisIndividual->maxDam, totalDamage);
	targetIndividual->hp = targetIndividual->hp - totalDamage;

	if(targetIndividual->hp <= 0){ //target is dead
		sendDeathDialog(targetIndividual->name, thisIndividual->name);
		triggerEventOnDeath(targetIndividual->ID);
		removeFromExistance(targetIndividual->ID);
		return 1;
	}else{ //non-fatal blow
		return 0;
	}
}

int calcDR(individual * targetIndividual, char attackType){
	int i, totalDR = 0;
	item * tmpItem;
	switch (attackType) {
	case 'b':
		totalDR += getAttributeSum(targetIndividual, "bluntDR");
		break;
	case 'c':
		totalDR += getAttributeSum(targetIndividual, "chopDR");
		break;
	case 's':
		totalDR += getAttributeSum(targetIndividual, "slashDR");
		break;
	case 'p':
		totalDR += getAttributeSum(targetIndividual, "pierceDR");
		break;
	}

	return totalDR;
}

int calcCrit(individual * thisIndividual, int maxDamTotal, int minDamTotal){
	if(strcmp(thisIndividual->critType, "MAX") == 0){
		return getAttributeSum(thisIndividual, "maxDam");
	} else if(strcmp(thisIndividual->critType, "DUB") == 0){
		int attackDamage = rand() % (maxDamTotal - minDamTotal);
		attackDamage = (attackDamage + minDamTotal) * 2;
	}
}

void startTurn(individual * thisIndividual){
	int i, itemsPassed = 0;
	activeItem * tmpActiveItem;

	for(i = 0; i < 40; i++){
		tmpActiveItem = thisIndividual->activeItems->activeItemArr[i];

		if(tmpActiveItem != NULL){
			tmpActiveItem->remaningTurns--;

			if(tmpActiveItem->remaningTurns <= 0){
				//free(tmpActiveItem->thisItem);
				//free(tmpActiveItem);
				thisIndividual->activeItems->activeItemArr[i] = NULL;
				thisIndividual->activeItems->activeItemsTotal--;
			}else{
				consumeItem(thisIndividual,tmpActiveItem->thisItem);
				itemsPassed++;
			}

		}

		if(itemsPassed >= thisIndividual->activeItems->activeItemsTotal){
			break;
		}
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

item * removeItemFromInventory(inventory * backpack, item * thisItem){
	int i;
	item * removedItem;

	for(i = 0; i < 40; i++){
		if(backpack->inventoryArr[i] == thisItem){
			removedItem = backpack->inventoryArr[i];
			removedItem->isEquipt = 0;

			backpack->inventoryArr[i] = NULL;
			backpack->inventorySize--;

			return removedItem;
		}
	}
}

void modifyItem(item * theItem, individual * thisIndividual) {
	char type = theItem->type;
	switch (type) {
		case 'w': {
			if(theItem->isEquipt){
				theItem->isEquipt = 0;
			}else{
				tryEquipItem(thisIndividual->backpack, theItem);
			}
		}
		break;
		case 'a': {
			if(theItem->isEquipt){
				theItem->isEquipt = 0;
			}else{
				tryEquipItem(thisIndividual, theItem);
			}
		}
		break;
		case 'i': {
			if(theItem->itemType == 'c'){
				consumeItem(thisIndividual, theItem);
				free(removeItemFromInventory(thisIndividual->backpack, theItem));
			}else if(theItem->itemType == 'd'){
				if(thisIndividual->activeItems->activeItemsTotal < 40){
					consumeItem(thisIndividual, theItem);
					addItemToActiveItemList(thisIndividual, theItem);
					removeItemFromInventory(thisIndividual->backpack, theItem);
				}
			}
		}
	}
}

void tryEquipItem(inventory * backpack, item * thisItem){
	int i;
	item * tmpItem;

	for(i = 0; i < 40; i++){
		tmpItem = backpack->inventoryArr[i];
		if(tmpItem != NULL && tmpItem != thisItem && tmpItem->type == thisItem->type && tmpItem->isEquipt){
			return;
		}
	}

	thisItem->isEquipt = 1;
}

void addItemToActiveItemList(individual * thisIndividual, item * theItem){
	int i, numTurns = 0;

		for(i = 0; i < 40; i++){

			if(thisIndividual->activeItems->activeItemArr[i] == NULL){
				activeItem * newActiveItem = malloc(sizeof(activeItem));
				newActiveItem->thisItem = theItem;

				if(theItem->maxTurns <= theItem->minTurns){
					numTurns = theItem->minTurns;
				}else{
					numTurns = (rand() % (theItem->maxTurns - theItem->minTurns)) + theItem->minTurns;
				}

				newActiveItem->remaningTurns = numTurns;

				thisIndividual->activeItems->activeItemArr[i] = newActiveItem;
				thisIndividual->activeItems->activeItemsTotal++;
				break;
			}
		}

}

int addItemToIndividual(inventory * backpack, item * newItem){
	int i, availableSpot;
	if(backpack->inventorySize < 40){
		for(i = 0; i < 40; i++){
			if(backpack->inventoryArr[i] == NULL){
				backpack->inventoryArr[i] = newItem;
				backpack->inventorySize++;
				return 1;
			}
		}
	}

	return 0;
}

/*
 * this will affect attributes instantaniously for no duration of time,
 * meaning atrributes like chopDR are not checked, because they have no meaningful affect.
 */
void consumeItem(individual * thisIndividual, item * theItem){

	//note, +/- healthMod
	if(theItem->healthMod != 0){
		if(theItem->healthMod > 0){
			healIndividual(thisIndividual, theItem->healthMod);

		}
	}

	if(theItem->manaMod != 0){
		if(theItem->manaMod > 0){
			restoreMana(thisIndividual, theItem->manaMod);
		}
	}
}

void activateDurationItem(individual * individual, item * thisItem){

}

void healIndividual(individual * thisIndividual, int hp){
	if(thisIndividual->totalHP - thisIndividual->hp < hp){
		thisIndividual->hp = thisIndividual->totalHP;
	}else{
		thisIndividual->hp += hp;
	}
}

void restoreMana(individual * thisIndividual, int mana){
	if(thisIndividual->totalMana - thisIndividual->mana < mana){
		thisIndividual->mana = thisIndividual->totalMana;
	}else{
		thisIndividual->mana += mana;
	}
}

int attemptToBuyItem(item * thisItem, individual * thisIndividual){

	if(thisItem->price <= thisIndividual->gold){
		item * newItem = cloneItem(thisItem);

		if(newItem == NULL){
			char * errLog[128];
			sprintf(errLog, "!! ITEM %d COULD NOT BE CLONED!!", thisItem->ID);
			return 0;
		}

		addItemToRegistry(newItem);
		addItemToIndividual(thisIndividual->backpack, newItem);

		thisIndividual->gold -= thisItem->price;
		return 1;
	}
	return 0;

}

int getAttributeFromIndividual(individual * thisIndividual, char * attribute){
	int toReturn = 0;

	if(strcmp("totalHealth",attribute) == 0 ){
		return thisIndividual->totalHP;
	} else if(strcmp("health",attribute) == 0 ){
		return thisIndividual->hp;
	} else if(strcmp("totalMana",attribute) == 0 ){
		return thisIndividual->totalMana;
	} else if(strcmp("mana",attribute) == 0 ){
		return thisIndividual->mana;
	} else if(strcmp("minTurns",attribute) == 0 ){
		return 0;
	} else if(strcmp("maxTurns",attribute) == 0 ){
		return 0;
	} else if(strcmp("ac",attribute) == 0 ){
		return thisIndividual->AC;
	} else if(strcmp("attack",attribute) == 0 ){
		return thisIndividual->attack;
	} else if(strcmp("dam",attribute) == 0 ){
		return 0;
	} else if(strcmp("maxDam",attribute) == 0 ){
		return thisIndividual->maxDam;
	} else if(strcmp("minDam",attribute) == 0 ){
		return thisIndividual->minDam;
	} else if(strcmp("mvmt",attribute) == 0 ){
		return thisIndividual->mvmt;
	} else if(strcmp("range",attribute) == 0 ){
		return thisIndividual->range;
	} else if(strcmp("bluntDR",attribute) == 0 ){
		return thisIndividual->bluntDR;
	} else if(strcmp("chopDR",attribute) == 0 ){
		return thisIndividual->chopDR;
	} else if(strcmp("slashDR",attribute) == 0 ){
		return thisIndividual->slashDR;
	} else if(strcmp("pierceDR",attribute) == 0 ){
		return thisIndividual->pierceDR;
	} else if(strcmp("earthDR",attribute) == 0 ){
		return thisIndividual->earthDR;
	} else if(strcmp("fireDR",attribute) == 0 ){
		return thisIndividual->fireDR;
	} else if(strcmp("waterDR",attribute) == 0 ){
		return thisIndividual->waterDR;
	} else if(strcmp("lightningDR",attribute) == 0 ){
		return thisIndividual->lightningDR;
	} else if(strcmp("earthWeakness",attribute) == 0 ){
		return thisIndividual->earthWeakness;
	} else if(strcmp("fireWeakness",attribute) == 0 ){
		return thisIndividual->fireWeakness;
	} else if(strcmp("waterWeakness",attribute) == 0 ){
		return thisIndividual->waterWeakness;
	} else if(strcmp("lightiningWeakness",attribute) == 0 ){
		return thisIndividual->lightiningWeakness;
	}

	return toReturn;
}

int getAttributeFromItem(item * thisItem, item * activeItem, char * attribute){
	int toReturn = 0;
	if(strcmp("totalHealth",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->totalHealthMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->totalHealthMod;
		 }
	} else if(strcmp("health",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->healthMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->healthMod;
		 }
	} else if(strcmp("totalMana",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->totalManaMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->totalManaMod;
		 }
	} else if(strcmp("mana",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->manaMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->manaMod;
		 }
	} else if(strcmp("minTurns",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->minTurns;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->minTurns;
		 }
	} else if(strcmp("maxTurns",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->maxTurns;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->maxTurns;
		 }
	} else if(strcmp("ac",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->acMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->acMod;
		 }
	} else if(strcmp("attack",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->attackMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->attackMod;
		 }
	} else if(strcmp("dam",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->damMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->damMod;
		 }
	} else if(strcmp("maxDam",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->maxDamMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->maxDamMod;
		 }
	} else if(strcmp("minDam",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->minDamMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->minDamMod;
		 }
	} else if(strcmp("mvmt",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->mvmtMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->mvmtMod;
		 }
	} else if(strcmp("range",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->rangeMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->rangeMod;
		 }
	} else if(strcmp("bluntDR",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->bluntDRMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->bluntDRMod;
		 }
	} else if(strcmp("chopDR",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->chopDRMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->chopDRMod;
		 }
	} else if(strcmp("slashDR",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->slashDRMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->slashDRMod;
		 }
	} else if(strcmp("pierceDR",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->pierceDRMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->pierceDRMod;
		 }
	} else if(strcmp("earthDR",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->earthDRMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->earthDRMod;
		 }
	} else if(strcmp("fireDR",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->fireDRMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->fireDRMod;
		 }
	} else if(strcmp("waterDR",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->waterDRMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->waterDRMod;
		 }
	} else if(strcmp("lightningDR",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->lightningDRMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->lightningDRMod;
		 }
	} else if(strcmp("earthWeakness",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->earthWeaknessMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->earthWeaknessMod;
		 }
	} else if(strcmp("fireWeakness",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->fireWeaknessMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->fireWeaknessMod;
		 }
	} else if(strcmp("waterWeakness",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->waterWeaknessMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->waterWeaknessMod;
		 }
	} else if(strcmp("lightiningWeakness",attribute) == 0 ){
		 if(thisItem != NULL && thisItem->isEquipt){
			 toReturn += thisItem->lightiningWeaknessMod;
		 }
		 if(activeItem != NULL){
			 toReturn += activeItem->lightiningWeaknessMod;
		 }
	}
	return toReturn;
}

int getAttributeSum(individual * thisIndividual, char * attribute){
	int toReturn = 0, i, itemTotal = 0, activeItemTotal = 0;

	toReturn += getAttributeFromIndividual(thisIndividual, attribute);

	for(i = 0; i < 40; i++){
		activeItem * tmpActiveItem;
		// have all items been used?
		if(itemTotal == thisIndividual->backpack->inventorySize &&
			activeItemTotal == thisIndividual->activeItems->activeItemsTotal){
			break;
		}
		if(thisIndividual->activeItems->activeItemArr[i] != NULL){
			toReturn += getAttributeFromItem(thisIndividual->backpack->inventoryArr[i],
								thisIndividual->activeItems->activeItemArr[i]->thisItem, attribute);
			activeItemTotal++;
		}else{
			toReturn += getAttributeFromItem(thisIndividual->backpack->inventoryArr[i],
								NULL, attribute);
		}

		if(thisIndividual->backpack->inventoryArr[i] != NULL){
			itemTotal++;
		}

	}

	return toReturn;
}
