/*
 * individual_pub_methods.h
 *
 *  Created on: Apr 14, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_INDIVIDUAL_PUB_METHODS_H_
#define SRC_HEADERS_INDIVIDUAL_PUB_METHODS_H_
#include"./structs/individual.h"


individual *initIndividual();
void destroyIndividual(individual* thisIndividual);
void drawIndividual(HDC hdc, HDC hdcBuffer, individual* player, shiftData * viewShift);
int attackIndividual(individual *thisIndividual, individual *targetIndividual);
int individualWithinRange(individual * thisIndividual, individual * target);
int calcDR(individual * targetIndividual, char attackType);
int addItemToIndividual(inventory * backpack, item * newItem);

int defineIndividual(individual * thisIndividual, int imageID, int ID, COLORREF rgb, char * name, int direction, int x,
		int y, int totalHP, int totalActions, int totalMana, int AC, int attack, int maxDam, int minDam,  char critType[3],
		int range, int mvmt, int bluntDR, int chopDR, int slashDR, int pierceDR, int earthDR, int fireDR,
		int waterDR, int lightningDR, int earthWeakness, int fireWeakness, int waterWeakness,
		int lightiningWeakness, int dialogID, int gold);

#endif /* SRC_HEADERS_INDIVIDUAL_PUB_METHODS_H_ */
