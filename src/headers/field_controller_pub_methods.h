/*
 * field_controller_pub_methods.h
 *
 *  Created on: Jul 23, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_FIELD_CONTROLLER_PUB_METHODS_H_
#define SRC_HEADERS_FIELD_CONTROLLER_PUB_METHODS_H_
#include "./structs/field_controller.h"

int addEnemyToEnemies(enemies * thisEnemies, individual * enemey);
int attemptToTransit(field ** thisField, individual * player, enemies * thisEnemies, shiftData * viewShift, char * mapDirectory);
void clearEnemies(enemies * thisEnemies);

#endif /* SRC_HEADERS_FIELD_CONTROLLER_PUB_METHODS_H_ */
