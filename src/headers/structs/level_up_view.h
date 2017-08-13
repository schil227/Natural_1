/*
 * level_up_view.h
 *
 *  Created on: Aug 4, 2017
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_LEVEL_UP_VIEW_H_
#define SRC_HEADERS_STRUCTS_LEVEL_UP_VIEW_H_

#include "../individual_pub_methods.h"

typedef enum {LEVELUP_MODE_DEFAULT, LEVELUP_MODE_ROLL, LEVELUP_MODE_ABLITIES} levelUpMode;
typedef enum {LEVELUP_HP, LEVELUP_MANA, LEVELUP_NONE, LEVELUP_STR, LEVELUP_DEX, LEVELUP_CON, LEVELUP_INT, LEVELUP_WIS, LEVELUP_WILL, LEVELUP_CHR, LEVELUP_LUCK, LEVELUP_NEXT} levelUpField;
typedef enum {ROLLMODE_ROLL, ROLLMODE_ACCEPT} rollMode;
typedef enum {UPGRADE_ABILITY_ABILITIES, UPGRADE_ABILITY_POINTS, UPGRADE_ABILITY_BONUS, UPGRADE_ABILITY_DONE} upgradeAbilityMode;
typedef struct{
	int upgradePoints;
	int bonusMana;
	int selectedAbilityIndex;
	int abilitiesOffset;
	int numAbilities;
	int abilitiesPerScreen;
	int inCreateAbilityMode;
	int upgradedAbilities[256];
	int numUpgradedAbilities;
	upgradeAbilityMode currentMode;

	fixedCharacter * upgradeAbilitiesView;
	fixedCharacter * entryAddAbility;

	char descriptionPermenantAbilityUpgrade[256];
	char descriptionDurationAbilityUpgrade[256];
	char descriptionTargetedAbilityUpgrade[256];
	char descriptionInstantAbilityUpgrade[256];
	char descriptionAbilityAlreadyUpgraded[256];
	char descriptionNewAbility[256];
	char descriptionUpgradePoints[256];
	char descriptionBonusPoints[256];
	char descriptionFinshed[256];
} levelUpAbilitiesView;

typedef struct{
	int inLevelUpMode;
	int bonusStatPoints;
	int bonusMana;
	int bonusHP;
	int rollsRemaining;
	levelUpMode currentMode;
	levelUpField currentField;
	levelUpField selectedStatField;
	rollMode rollChoice;
	individual * player;

	fixedCharacter * levelUpView;
	fixedCharacter * spreadRollView;
	fixedCharacter * fieldSelect;
	fixedCharacter * fieldEnter;
	fixedCharacter * rollSelect;
	fixedCharacter * rollEnter;
	fixedCharacter * statFieldEdit;
	fixedCharacter * selectArrowLeft;
	fixedCharacter * selectArrowRight;

	levelUpAbilitiesView * abilitiesView;
} levelUpView;

#endif /* SRC_HEADERS_STRUCTS_LEVEL_UP_VIEW_H_ */
