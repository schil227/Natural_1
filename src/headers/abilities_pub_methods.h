/*
 * abilities_pub_methods.h
 *
 *  Created on: Mar 2, 2016
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_ABILITIES_PUB_METHODS_H_
#define SRC_HEADERS_ABILITIES_PUB_METHODS_H_
#include "./structs/abilities.h"

effectAndManaMapList * cloneEffectAndManaMapList(effectAndManaMapList * thisMap);
ability * createAbilityFromLine(char line[2048]);
ability * cloneAbility(ability * thisAbility);
char * getEffectAndManaString(char * propertyName, effectAndManaMapList * map);
void processEffectMapListRendering(int * effectIndex, int isEnabled, HDC hdc, HDC hdcBuffer, effect_types type, int xOff, int yOff, char * fieldName, int isDR, effectAndManaMapList * mapList, int * startingPixelHeight);
effectAndManaMapList * getMapListFromEffectType();
effectAndManaMapList * getMapListFromEffectTypeUpgradeClone();
void clearAblitiesView();

#endif /* SRC_HEADERS_ABILITIES_PUB_METHODS_H_ */
