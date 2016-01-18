/*
 * global_register.h
 *
 *  Created on: Oct 22, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_GLOBAL_REGISTER_H_
#define SRC_HEADERS_STRUCTS_GLOBAL_REGISTER_H_

#include"../individual_pub_methods.h"

typedef struct {
	unsigned int existanceArray[1000];
	int sizeOfInt;

	individual * individualRegistry[1000];
	item * itemRegistry[5000];

	int MAX_INDIVIDUALS;
	int numIndividuals;
	int MAX_ITEMS;
	int numItems;
} globalRegister;

#endif /* SRC_HEADERS_STRUCTS_GLOBAL_REGISTER_H_ */
