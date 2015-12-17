/*
 * field_controller.h
 *
 *  Created on: Jul 23, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_FIELD_CONTROLLER_H_
#define SRC_HEADERS_STRUCTS_FIELD_CONTROLLER_H_
#include "../field_pub_methods.h"

typedef struct {
	size_t size;
	int numIndividuals;
	int currentIndividualIndex;
	individual * individuals[50];
} individualGroup;

#endif /* SRC_HEADERS_STRUCTS_FIELD_CONTROLLER_H_ */
