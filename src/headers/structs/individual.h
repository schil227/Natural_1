/*
 * individual.h
 *
 *  Created on: Apr 14, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_INDIVIDUAL_H_
#define SRC_HEADERS_STRUCTS_INDIVIDUAL_H_
//#include"../character_pub_methods.h"
#include"../item_pub_methods.h"

typedef struct{
	int inventorySize;
	item * inventoryArr[40];
} inventory;

typedef struct {
	character* playerCharacter;
	char name[32];
	int ID;

	inventory * backpack;

	short int totalHP;
	short int hp;
	short int totalMana;
	short int mana;
	short int totalActions;
	short int remainingActions;
	short int hasAttacked;

	short int AC;
	short int attack;
	short int maxDam;
	short int minDam;
	short int range;
	char critType[3];
	short int mvmt;

	//physical damage resistance
	short int bluntDR;
	short int chopDR;
	short int slashDR;
	short int pierceDR;

	//magic damage resistance
	short int earthDR;
	short int fireDR;
	short int waterDR;
	short int lightningDR;

	short int earthWeakness;
	short int fireWeakness;
	short int waterWeakness;
	short int lightiningWeakness;

	int jumpTarget;

} individual;



#endif /* SRC_HEADERS_STRUCTS_INDIVIDUAL_H_ */
