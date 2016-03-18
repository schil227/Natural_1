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

#endif /* SRC_HEADERS_ABILITIES_PUB_METHODS_H_ */
