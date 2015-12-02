/*
 * enemy_controller_pub_methods.h
 *
 *  Created on: Jul 14, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_ENEMY_CONTROLLER_PUB_METHODS_H_
#define SRC_HEADERS_ENEMY_CONTROLLER_PUB_METHODS_H_
#include"./field_pub_methods.h"

nodeArr * getSpaceClosestToPlayer(field * thisField, individual * thisIndividual, individual * targetIndividual);
void destroyNodeArr(nodeArr * thisNodeArr);

#endif /* SRC_HEADERS_ENEMY_CONTROLLER_PUB_METHODS_H_ */
