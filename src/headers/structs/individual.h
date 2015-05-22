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

	short int maxDam;
	short int minDam;


} individual;

#endif /* SRC_HEADERS_STRUCTS_INDIVIDUAL_H_ */
