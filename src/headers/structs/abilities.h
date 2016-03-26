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

typedef enum {RANGE, TARGETED, EXTRA_ATTACK, DICE_DAMAGE, DAMAGE, DICE_DAMAGE_DURATION,
	DICE_DAMAGE_DURATION_MOD, AOE, DURATION, DURATION_MOD, STR, DEX, CON, WILL, INTEL, WIS, CHR, LUCK,
	AC, DAMAGE_MOD, MVMT, HP, TOTAL_HP, TOTAL_MANA, BLUNT_DR, CHOP_DR, PIERCE_DR, SLASH_DR, EARTH_DR,
	FIRE_DR, WATER_DR, LIGHTNING_DR } effect_types;

typedef enum {DEFAULT_ABILITY, LEVELUP_ABILITY} creation_modes;

typedef struct{
	short int effectMagnitude;
	short int manaCost;
} effectAndMana;

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
	char damageType;
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
	short int diceDamageDurationEnabled;
	effectAndManaMapList * diceDamageDuration;
	short int diceDamageDurationModEnabled;
	effectAndManaMapList * diceDamageDurationMod;
	short int aoeEnabled;
	effectAndManaMapList * aoe;
	short int durationEnabled;
	effectAndManaMapList * duration;
	short int durationModEnabled;
	effectAndManaMapList * durationMod;

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
	short int damageModEnabled;
	effectAndManaMapList * damageMod;
	short int mvmtEnabled;
	effectAndManaMapList * mvmt;
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

	char currentCharIndex;
	int activeNameIndex;
	char newAbilityName[16];

	creation_modes mode;
	effect_types selectedType;

	character * creationWindow;
	character * nameWindow;
	character * selector;
	character * leftRightArrow;
	character * scrollUpArrow;
	character * scrollDownArrow;
	ability * abilityTemplates[3];

	ability * abilityInsance;
} abilityCreationInstance;

#endif /* SRC_HEADERS_STRUCTS_ABILITIES_H_ */
