/*
 * individual_pub_methods.h
 *
 *  Created on: Apr 14, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_INDIVIDUAL_PUB_METHODS_H_
#define SRC_HEADERS_INDIVIDUAL_PUB_METHODS_H_
#include"./structs/individual.h"

void distroyIndividual(individual* thisIndividual);
void drawPlayer(HDC hdc, HDC hdcBuffer, individual* player);

#endif /* SRC_HEADERS_INDIVIDUAL_PUB_METHODS_H_ */
