/*
 * individual.h
 *
 *  Created on: Apr 14, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_INDIVIDUAL_H_
#define SRC_HEADERS_STRUCTS_INDIVIDUAL_H_
#include"../character_pub_methods.h"

typedef struct {
	character* playerCharacter;
	char* name;

	short int totalHP;
	short int hp;
	short int totalActions;
	short int remainingActions;
	short int hasAttacked;

	short int AC;

	short int range;
	short int attack;
	short int maxDam;
	short int minDam;
	char critType[3];

	short int mvmt;

} individual;

#endif /* SRC_HEADERS_STRUCTS_INDIVIDUAL_H_ */
