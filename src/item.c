/*
 * item.c
 *
 *  Created on: Oct 26, 2015
 *      Author: Adrian
 */
#include"./headers/item_pub_methods.h"

item * createItem(int npcID, int imageID, COLORREF rgb, int x, int y, int ID, char type, char *name, char *description,double weaponStrMod,
		int strMod, int dexMod, int conMod, int willMod, int intMod, int wisMod, int chrMod, int luckMod,
		char weaponDamageType, char armorClass, char itemType, int price, int totalHealthMod, int healthMod, int totalManaMod,
		int manaMod, int acMod, int attackMod, int damMod, int maxDamMod, int minDamMod, int minTurns, int maxTurns,
		int mvmtMod, int rangeMod, int bluntDRMod, int chopDRMod, int slashDRMod, int pierceDRMod, int earthDRMod,
		int fireDRMod, int waterDRMod, int lightningDRMod, int earthWeaknessMod, int fireWeaknessMod,
		int waterWeaknessMod, int lightiningWeaknessMod, int isEquipt, animationContainer * thisAnimationContainer){

	item * thisItem = malloc(sizeof(item));
	thisItem->itemCharacter = createCharacter(imageID,rgb,x,y);

	if (thisItem->itemCharacter->image == NULL) {
		return NULL;
	}

	thisItem->itemCharacter->thisAnimationContainer = thisAnimationContainer;

	thisItem->npcID = npcID;
	thisItem->ID = ID;
	thisItem->type = type;
	strcpy(thisItem->name, name);
	strcpy(thisItem->description, description);

	thisItem->weaponDamageType= weaponDamageType;
	thisItem->armorClass = armorClass;
	thisItem->itemType = itemType;
	thisItem->price = price;
	thisItem->weaponStrMod = weaponStrMod;

	thisItem->strMod = strMod;
	thisItem->dexMod = dexMod;
	thisItem->conMod = conMod;
	thisItem->willMod = willMod;
	thisItem->intMod = intMod;
	thisItem->wisMod = wisMod;
	thisItem->chrMod = chrMod;
	thisItem->luckMod = luckMod;

	thisItem->totalHealthMod = totalHealthMod;
	thisItem->healthMod = healthMod;
	thisItem->totalManaMod = totalManaMod;
	thisItem->manaMod = manaMod;
	thisItem->acMod = acMod;
	thisItem->attackMod = attackMod;
	thisItem->damMod = damMod;
	thisItem->maxDamMod = maxDamMod;
	thisItem->minDamMod = minDamMod;
	thisItem->minTurns = minTurns;
	thisItem->maxTurns = maxTurns;
	thisItem->mvmtMod = mvmtMod;
	thisItem->rangeMod = rangeMod;
	thisItem->bluntDRMod = bluntDRMod;
	thisItem->chopDRMod = chopDRMod;
	thisItem->slashDRMod = slashDRMod;
	thisItem->pierceDRMod = pierceDRMod;
	thisItem->earthDRMod = earthDRMod;
	thisItem->fireDRMod = fireDRMod;
	thisItem->waterDRMod = waterDRMod;
	thisItem->lightningDRMod = lightningDRMod;
	thisItem->earthWeaknessMod = earthWeaknessMod;
	thisItem->fireWeaknessMod = fireWeaknessMod;
	thisItem->waterWeaknessMod = waterWeaknessMod;
	thisItem->lightiningWeaknessMod = lightiningWeaknessMod;
	thisItem->isEquipt = isEquipt;

	return thisItem;
}

void destroyItem(item * thisItem){
	if(thisItem->itemCharacter != NULL){
		destroyCharacter(thisItem->itemCharacter);
	}

	free(thisItem);
}

item * cloneItem(item * thisItem){
	item * newItem = malloc(sizeof(item));
	newItem->itemCharacter = createCharacter(thisItem->itemCharacter->imageID, thisItem->itemCharacter->rgb,
								thisItem->itemCharacter->x, thisItem->itemCharacter->y);

	if (thisItem->itemCharacter->image == NULL) {
			return NULL;
	}

	newItem->npcID = thisItem->npcID;
	newItem->ID = thisItem->ID;
	newItem->type = thisItem->type;
	strcpy(newItem->name, thisItem->name);
	strcpy(newItem->description, thisItem->description);

	newItem->weaponDamageType= thisItem->weaponDamageType;
	newItem->armorClass = thisItem->armorClass;
	newItem->itemType = thisItem->itemType;
	newItem->price = thisItem->price;

	newItem->totalHealthMod = thisItem->totalHealthMod;
	newItem->healthMod = thisItem->healthMod;
	newItem->totalManaMod = thisItem->totalManaMod;
	newItem->manaMod = thisItem->manaMod;
	newItem->acMod = thisItem->acMod;
	newItem->attackMod = thisItem->attackMod;
	newItem->damMod = thisItem->damMod;
	newItem->maxDamMod = thisItem->maxDamMod;
	newItem->minDamMod = thisItem->minDamMod;
	newItem->minTurns = thisItem->minTurns;
	newItem->maxTurns = thisItem->maxTurns;
	newItem->mvmtMod = thisItem->mvmtMod;
	newItem->rangeMod = thisItem->rangeMod;
	newItem->bluntDRMod = thisItem->bluntDRMod;
	newItem->chopDRMod = thisItem->chopDRMod;
	newItem->slashDRMod = thisItem->slashDRMod;
	newItem->pierceDRMod = thisItem->pierceDRMod;
	newItem->earthDRMod = thisItem->earthDRMod;
	newItem->fireDRMod = thisItem->fireDRMod;
	newItem->waterDRMod = thisItem->waterDRMod;
	newItem->lightningDRMod = thisItem->lightningDRMod;
	newItem->earthWeaknessMod = thisItem->earthWeaknessMod;
	newItem->fireWeaknessMod = thisItem->fireWeaknessMod;
	newItem->waterWeaknessMod = thisItem->waterWeaknessMod;
	newItem->lightiningWeaknessMod = thisItem->lightiningWeaknessMod;
	newItem->isEquipt = thisItem->isEquipt;

	return newItem;
}
