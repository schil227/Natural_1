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
int addItemToInventory(inventory * backpack, item * newItem);
char * lookUpStatusEffectName(statusEffect effect);
statusEffect lookUpStatusType(char * statusType[16]);
status * createStatusFromAbility(ability * thisAbility);
char getIndividualAttackType(individual * thisIndividual, item * weapon);
double calcSlope(int x1, int y1, int x2, int y2);

int defineIndividual(individual * thisIndividual, int ID, int isPlayer, COLORREF rgb, char * name, int direction, int x,
		int y, int STR, int DEX, int CON, int WILL, int INT, int WIS, int CHR, int LUCK, int baseHP, int totalActions, int baseMana, int baseAC, int attack, int maxDam, int minDam, int baseDam, char critType[3],
		int range, int mvmt, int LoS, int isSneaking, int bluntDR, int chopDR, int slashDR, int pierceDR, int earthDR, int fireDR,
		int waterDR, int lightningDR, int earthWeakness, int fireWeakness, int waterWeakness,
		int lightiningWeakness, int dialogID, int gold, int faction, int offensiveness, int abilityAffinity, int tacticalness, int cowardness,
		specialDialogs * thisDialog, abilityList * loadedAbilities, animationContainer * thisAnimationContainer,  animationContainer * secondaryAnimationContainer);

#endif /* SRC_HEADERS_INDIVIDUAL_PUB_METHODS_H_ */
