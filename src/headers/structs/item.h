/*
 * item.h
 *
 *  Created on: Oct 24, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_ITEM_H_
#define SRC_HEADERS_STRUCTS_ITEM_H_

#include"../character_pub_methods.h"

typedef struct {
	character * itemCharacter;
	int ID;
	short int npcID;
	char type; // (w)eapon/(a)rmor/(i)tem
	char name[32];
	char description[256];

	//type specific info
	char weaponDamageType;//(b)lunt, (c)hop, (s)lash, (p)ierce
	char armorClass;//(l)ight, (m)edium, (h)eavy
	char itemType;//(c)onsumable, (d)uration

	short int strMod;
	short int dexMod;
	short int conMod;
	short int willMod;
	short int intMod;
	short int wisMod;
	short int chrMod;
	short int luckMod;

	short int totalHealthMod;
	short int healthMod;

	short int totalManaMod;
	short int manaMod;

	short int minTurns;
	short int maxTurns;

	short int acMod;
	short int attackMod;
	short int damMod;
	short int maxDamMod;
	short int minDamMod;
	short int mvmtMod;
	short int rangeMod;

	short int bluntDRMod;
	short int chopDRMod;
	short int slashDRMod;
	short int pierceDRMod;

	//magic damage resistance
	short int earthDRMod;
	short int fireDRMod;
	short int waterDRMod;
	short int lightningDRMod;

	short int earthWeaknessMod;
	short int fireWeaknessMod;
	short int waterWeaknessMod;
	short int lightiningWeaknessMod;

	short int isEquipt;
	short int price;
} item;

#endif /* SRC_HEADERS_STRUCTS_ITEM_H_ */
