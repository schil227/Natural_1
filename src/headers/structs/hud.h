/*
 * hud.h
 *
 *  Created on: Jan 26, 2017
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_HUD_H_
#define SRC_HEADERS_STRUCTS_HUD_H_

#include "../individual_pub_methods.h";

typedef struct{
	fixedCharacter * actionModeCharacter;
	fixedCharacter * poisonedCharacter;
	fixedCharacter * burningCharacter;
	fixedCharacter * berzerkCharacter;
	fixedCharacter * paralyzedCharacter;
	fixedCharacter * confusedCharacter;
	fixedCharacter * bleedingCharacter;
	fixedCharacter * silencedCharacter;
	fixedCharacter * sleepCharacter;
	fixedCharacter * sneakingCharacter;

	int isPoisoned;
	int isBurning;
	int isBerzerking;
	int isParalyzed;
	int isConfused;
	int isBleeding;
	int isSilenced;
	int isSleeping;

}hudInstance;


#endif /* SRC_HEADERS_STRUCTS_HUD_H_ */
