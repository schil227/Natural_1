/*
 * field_controller_pub_methods.h
 *
 *  Created on: Jul 23, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_FIELD_CONTROLLER_PUB_METHODS_H_
#define SRC_HEADERS_FIELD_CONTROLLER_PUB_METHODS_H_
#include "./structs/field_controller.h"

int addIndividualToGroup(individualGroup * thisGroup, individual * thisIndividual);
int attemptToTransit(field ** thisField, individual * player, individualGroup * enemies, individualGroup * npcs, shiftData * viewShift, char * mapDirectory);
void clearGroup(individualGroup * thisGroup);

#endif /* SRC_HEADERS_FIELD_CONTROLLER_PUB_METHODS_H_ */
