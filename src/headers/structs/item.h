/*
 * item.h
 *
 *  Created on: Oct 24, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_ITEM_H_
#define SRC_HEADERS_STRUCTS_ITEM_H_

#include"../character_pub_methods.h";

typedef struct {
	character * itemCharacter;
	int ID;
	int type; //equippable, consumeable

	short int totalHealthMod;
	short int healthMod;

	short int totalManaMod;
	short int manaMod;

	short int attackMod;
	short int damMod;
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

} item;

typedef struct{
	item * theItem;
	char damType;
} weapon;

typedef struct{
	item * theItem;
	char weightClass;
} armor;

#endif /* SRC_HEADERS_STRUCTS_ITEM_H_ */
