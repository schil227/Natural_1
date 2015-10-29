/*
 * item.c
 *
 *  Created on: Oct 26, 2015
 *      Author: Adrian
 */
#include"./headers/item_pub_methods.h"

item * createItem(int imageID, COLORREF rgb, int x, int y, int ID, char type, char name[32],
		char weaponDamageType, char armorClass, int totalHealthMod, int healthMod, int totalManaMod,
		int manaMod, int acMod, int attackMod, int damMod, int maxDamMod, int minDamMod, int mvmtMod, int rangeMod, int bluntDRMod,
		int chopDRMod, int slashDRMod, int pierceDRMod, int earthDRMod, int fireDRMod,
		int waterDRMod, int lightningDRMod, int earthWeaknessMod, int fireWeaknessMod,
		int waterWeaknessMod, int lightiningWeaknessMod, int isEquipt){

	item * thisItem = malloc(sizeof(item));
	thisItem->itemCharacter = malloc(sizeof(character));

	BITMAP bm;

	thisItem->itemCharacter->imageID = imageID;
	thisItem->itemCharacter->image = LoadBitmap(GetModuleHandle(NULL),
	MAKEINTRESOURCE(imageID));

	if (thisItem->itemCharacter->image == NULL) {
		return 1;
	}

	thisItem->itemCharacter->imageMask = CreateBitmapMask(
			thisItem->itemCharacter->image, rgb);

	GetObjectA(thisItem->itemCharacter->image, sizeof(bm), &bm);

	thisItem->itemCharacter->height = bm.bmHeight;
	thisItem->itemCharacter->width = bm.bmWidth;

	thisItem->itemCharacter->rgb = rgb;

	thisItem->ID = ID;
	thisItem->type = type;
	strcpy(thisItem->name, name);

	thisItem->weponDamageType= weaponDamageType;
	thisItem->armorClass = armorClass;

	thisItem->totalHealthMod = totalHealthMod;
	thisItem->healthMod = healthMod;
	thisItem->totalManaMod = totalManaMod;
	thisItem->manaMod = manaMod;
	thisItem->acMod = acMod;
	thisItem->attackMod = attackMod;
	thisItem->damMod = damMod;
	thisItem->maxDamMod = maxDamMod;
	thisItem->minDamMod = minDamMod;
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

