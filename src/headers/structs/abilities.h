/*
 * abilities.h
 *
 *  Created on: Feb 22, 2016
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_ABILITIES_H_
#define SRC_HEADERS_STRUCTS_ABILITIES_H_

#include<stdlib.h>
#include"../character_pub_methods.h"

typedef enum {ABILITY_DAMAGE_TYPE, ABILITY_RANGE, ABILITY_TARGETED, ABILITY_EXTRA_ATTACK, ABILITY_DICE_DAMAGE, ABILITY_DAMAGE, ABILITY_STATUS, ABILITY_STATUS_DAMAGE, ABILITY_STATUS_DICE_DAMAGE,
	ABILITY_STATUS_DICE_DURATION, ABILITY_STATUS_DURATION, ABILITY_AOE, ABILITY_DURATION, ABILITY_DURATION_MOD, ABILITY_ACTIONS,
	ABILITY_STR, ABILITY_DEX, ABILITY_CON, ABILITY_WILL, ABILITY_INT, ABILITY_WIS, ABILITY_CHR, ABILITY_LUCK, ABILITY_AC, ABILITY_ATTACK,
	ABILITY_DAMAGE_MOD, ABILITY_MVMT, ABILITY_DICE_HP, ABILITY_HP, ABILITY_TOTAL_HP, ABILITY_TOTAL_MANA, ABILITY_BLUNT_DR,
	ABILITY_CHOP_DR, ABILITY_PIERCE_DR, ABILITY_SLASH_DR, ABILITY_EARTH_DR,	ABILITY_FIRE_DR, ABILITY_WATER_DR, ABILITY_LIGHTNING_DR
} effect_types;

typedef enum {DEFAULT_ABILITY, LEVELUP_ABILITY} creation_modes;

typedef struct{
	char type[16];
	int manaCost;
}typeAndMana;

typedef struct{
	short int effectMagnitude;
	short int manaCost;
} effectAndMana;

typedef struct{
	int size;
	int MAX_SIZE;
	int selectedIndex;
	int defaultStartingIndex;
	typeAndMana * typeAndManaArray[16];
}typeAndManaMapList;

typedef struct{
	int size;
	int MAX_SIZE;
	int selectedIndex;
	int defaultStartingIndex; //if things can go positive/negitive, start with 0 cost index
	effectAndMana * effectAndManaArray[32];

} effectAndManaMapList;

typedef struct{
	int ID;
	char type;
	char name[16];
	char typeName[16];
	char description[128];
	int totalManaCost;
	int level;
	int numEnabledEffects;

	short int damageTypeEnabled;
	typeAndManaMapList * damageType;
	short int rangeEnabled;
	effectAndManaMapList * range;
	short int targetedEnabled;
	effectAndManaMapList * targeted;
	short int extraAttackEnabled;
	effectAndManaMapList * extraAttack;
	short int diceDamageEnabled;
	effectAndManaMapList * diceDamage;
	short int damageEnabled;
	effectAndManaMapList * damage;

	short int statusEnabled;
	typeAndManaMapList * status;
	short int statusDiceDamageEnabled;
	effectAndManaMapList * statusDiceDamage;
	short int statusDamageEnabled;
	effectAndManaMapList * statusDamage;
	short int diceStatusDurationEnabled;
	effectAndManaMapList * diceStatusDuration;
	short int statusDurationEnabled;
	effectAndManaMapList * statusDuration;

	short int aoeEnabled;
	effectAndManaMapList * aoe;
	short int durationEnabled;
	effectAndManaMapList * duration;
	short int durationModEnabled;
	effectAndManaMapList * durationMod;
	short int actionsEnabled;
	effectAndManaMapList * actions;

	short int STREnabled;
	effectAndManaMapList * STR;
	short int DEXEnabled;
	effectAndManaMapList * DEX;
	short int CONEnabled;
	effectAndManaMapList * CON;
	short int WILLEnabled;
	effectAndManaMapList * WILL;
	short int INTEnabled;
	effectAndManaMapList * INT;
	short int WISEnabled;
	effectAndManaMapList * WIS;
	short int CHREnabled;
	effectAndManaMapList * CHR;
	short int LUCKEnabled;
	effectAndManaMapList * LUCK;

	short int acEnabled;
	effectAndManaMapList * ac;
	short int attackEnabled;
	effectAndManaMapList * attack;
	short int damageModEnabled;
	effectAndManaMapList * damageMod;
	short int mvmtEnabled;
	effectAndManaMapList * mvmt;
	short int diceHPEnabled;
	effectAndManaMapList * diceHP;
	short int hpEnabled;
	effectAndManaMapList * hp;
	short int totalHPEnabled;
	effectAndManaMapList * totalHP;
	short int totalManaEnabled;
	effectAndManaMapList * totalMana;

	short int bluntDREnabled;
	effectAndManaMapList * bluntDR;
	short int chopDREnabled;
	effectAndManaMapList * chopDR;
	short int pierceDREnabled;
	effectAndManaMapList * pierceDR;
	short int slashDREnabled;
	effectAndManaMapList * slashDR;

	short int earthDREnabled;
	effectAndManaMapList * earthDR;
	short int fireDREnabled;
	effectAndManaMapList * fireDR;
	short int waterDREnabled;
	effectAndManaMapList * waterDR;
	short int lightningDREnabled;
	effectAndManaMapList * lightningDR;
} ability;

typedef struct{
	ability * thisAbility;
	int turnsRemaining;
} activeAbility;

typedef struct {
	int inCreateMode;
	int inNameMode;
	int templateIndex;

	int numAbilityTemplates;
	int MAX_ABILITY_TEMPLATES;
	int currentTemplateIndex;

	int effectStartingIndex;
	int effectCurrentIndex;
	int effectEndingIndex;
	int MAX_FIELDS_ON_WINDOW;

	int waitingForName;

	creation_modes mode;
	effect_types selectedType;

	character * creationWindow;
	character * selector;
	character * leftRightArrow;
	character * scrollUpArrow;
	character * scrollDownArrow;
	ability * abilityTemplates[3];

	ability * abilityInsance;
} abilityCreationInstance;

typedef struct{
	int inAbiltyViewMode;
	int numAbilites;
	int MAX_ABILITIES;

	int currentAbilityIndex;
	int startingAbilityIndex;
	int endingAbilityIndex;
	int MAX_ABILITIES_ON_WINDOW;

	ability * abilitiesList[64];

	character * abilityViewWindow;
	character * selector;
} abilityView;

#endif /* SRC_HEADERS_STRUCTS_ABILITIES_H_ */
