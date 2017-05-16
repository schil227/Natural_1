/*
 * character_info_view.h
 *
 *  Created on: May 11, 2017
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_CHARACTER_INFO_VIEW_H_
#define SRC_HEADERS_STRUCTS_CHARACTER_INFO_VIEW_H_

#include "../individual_pub_methods.h"

typedef enum {ACTIVE_EFFECT_ABILITY, ACTIVE_EFFECT_STATUS, ACTIVE_EFFECT_ITEM, ACTIVE_EFFECT_FOOD, ACTIVE_EFFECT_EQUIPT_ITEM} activeEffectType;

typedef struct{
	int inCharacterInfoView;
	int inInfoView;
	int canLevelUp;
	activeEffectType selectedEffectMode;
	int selectedEffectIndex;
	int effectDrawSkipCount;
	int MAX_EFFECTS;

	individual * thisIndividual;
	item * equippedItems[5];
	int numEquiptItems;
	int MAX_EQUIPPED_ITEMS;

	fixedCharacter * infoWindow;
	fixedCharacter * activeEffectsWindow;
	fixedCharacter * activeEffectsArrow;
	fixedCharacter * characterInfoArrow;
	fixedCharacter * levelUpArrow;
	fixedCharacter * selectArrow;
	fixedCharacter * scrollUpArrow;
	fixedCharacter * scrollDownArrow;
	fixedCharacter * drChart;
	fixedCharacter * frame;
} characterInfoView;

#endif /* SRC_HEADERS_STRUCTS_CHARACTER_INFO_VIEW_H_ */
