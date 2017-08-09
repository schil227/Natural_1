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

typedef enum {ABILITY_TYPE, ABILITY_DAMAGE_TYPE, ABILITY_RANGE, ABILITY_TARGETED, ABILITY_EXTRA_ATTACK, ABILITY_DICE_DAMAGE, ABILITY_DICE_DAMAGE_MULTIPLIER,
	ABILITY_DAMAGE, ABILITY_STATUS, ABILITY_STATUS_DAMAGE, ABILITY_STATUS_DICE_DAMAGE, ABILITY_STATUS_DICE_DURATION,
	ABILITY_STATUS_DURATION, ABILITY_AOE_NOVA, ABILITY_AOE_LINE, ABILITY_DURATION, ABILITY_DURATION_MOD, ABILITY_ACTIONS, ABILITY_AC, ABILITY_ATTACK,
	ABILITY_DAMAGE_MOD, ABILITY_MVMT, ABILITY_DICE_HP, ABILITY_HP, ABILITY_BASE_HP, ABILITY_BASE_MANA, ABILITY_FOOD, ABILITY_BASE_FOOD, ABILITY_LOS,
	ABILITY_BLUNT_DR, ABILITY_CHOP_DR, ABILITY_PIERCE_DR, ABILITY_SLASH_DR, ABILITY_EARTH_DR,	ABILITY_FIRE_DR, ABILITY_WATER_DR, ABILITY_LIGHTNING_DR
} effect_types;

typedef enum {ABILITY_CREATE_DEFAULT, ABILITY_CREATE_PERMENANT_ONLY, ABILITY_CREATE_EXCEPT_PERMENANT} abilityCreationMode;

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
	short int diceDamageMultiplierEnabled;
	effectAndManaMapList * diceDamageMultiplier;

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

	short int aoeNovaEnabled;
	effectAndManaMapList * aoeNova;
	short int aoeLineEnabled;
	effectAndManaMapList * aoeLine;
	short int durationEnabled;
	effectAndManaMapList * duration;
	short int durationModEnabled;
	effectAndManaMapList * durationMod;
	short int actionsEnabled;
	effectAndManaMapList * actions;

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
	short int baseHPEnabled;
	effectAndManaMapList * baseHP;
	short int baseManaEnabled;
	effectAndManaMapList * baseMana;

	short int foodEnabled;
	effectAndManaMapList * food;
	short int baseFoodEnabled;
	effectAndManaMapList * baseFood;
	short int LoSEnabled;
	effectAndManaMapList * LoS;

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

	int effectIndex;
	int effectIndexOffset;

	int effectStartingIndex;
	int effectCurrentIndex;
	int effectEndingIndex;
	int MAX_FIELDS_ON_WINDOW;
	int EFFECT_FIELD_DRAW_HEIGHT;
	int idCounter;
	int waitingForName;

	int bonusMana;
	int totalBonusMana;

	creation_modes mode;
	abilityCreationMode createMode;
	effect_types selectedType;

	fixedCharacter * creationWindow;
	fixedCharacter * selector;
	fixedCharacter * leftRightArrow;
	fixedCharacter * scrollUpArrow;
	fixedCharacter * scrollDownArrow;

	fixedCharacter * abilityEntrySelect;
	fixedCharacter * abilityEntryEdit;
	fixedCharacter * abilityTypeBox;
	fixedCharacter * abilityTypeBoxSelected;
	fixedCharacter * abilitySlider2Un;
	fixedCharacter * abilitySlider4Un;
	fixedCharacter * abilitySlider5B;
	fixedCharacter * abilitySlider6B2R;
	fixedCharacter * abilitySlider6UnB;
	fixedCharacter * abilitySlider6UnR;
	fixedCharacter * abilitySlider7B;
	fixedCharacter * abilitySlider9B3R;
	fixedCharacter * abilitySlider10B4R;
	fixedCharacter * abilitySlider11B;
	fixedCharacter * abilitySlider11Un;

	ability * abilityTemplates[4];

	char descriptionPermanent[256];
	char descriptionPermanentRange[256];
	char descriptionPermanentSTR[256];
	char descriptionPermanentDEX[256];
	char descriptionPermanentCON[256];
	char descriptionPermanentWIS[256];
	char descriptionPermanentINT[256];
	char descriptionPermanentWILL[256];
	char descriptionPermanentCHR[256];
	char descriptionPermanentLUCK[256];
	char descriptionPermanentAC[256];
	char descriptionPermanentAttack[256];
	char descriptionPermanentDamageMod[256];
	char descriptionPermanentMvmt[256];
	char descriptionPermanentBaseHp[256];
	char descriptionPermanentBaseMana[256];
	char descriptionPermanentBluntDR[256];
	char descriptionPermanentChopDR[256];
	char descriptionPermanentPierceDR[256];
	char descriptionPermanentSlashDR[256];
	char descriptionPermanentEarthDR[256];
	char descriptionPermanentFireDR[256];
	char descriptionPermanentWaterDR[256];
	char descriptionPermanentLightningDR[256];

	char descriptionDuration[256];
	char descriptionDurationRange[256];
	char descriptionDurationDiceDamage[256];
	char descriptionDurationDiceDamageMult[256];
	char descriptionDurationDamage[256];
	char descriptionDurationStatus[256];
	char descriptionDurationStatusDiceDamage[256];
	char descriptionDurationStatusDamage[256];
	char descriptionDurationStatusDiceDuration[256];
	char descriptionDurationStatusDuration[256];
	char descriptionDurationAOENova[256];
	char descriptionDurationAOELine[256];
	char descriptionDurationDiceDuration[256];
	char descriptionDurationDuration[256];
	char descriptionDurationActions[256];
	char descriptionDurationSTR[256];
	char descriptionDurationDEX[256];
	char descriptionDurationCON[256];
	char descriptionDurationWIS[256];
	char descriptionDurationINT[256];
	char descriptionDurationWILL[256];
	char descriptionDurationCHR[256];
	char descriptionDurationLUCK[256];
	char descriptionDurationAC[256];
	char descriptionDurationAttack[256];
	char descriptionDurationDamageMod[256];
	char descriptionDurationMvmt[256];
	char descriptionDurationDiceHp[256];
	char descriptionDurationHp[256];
	char descriptionDurationBaseHp[256];
	char descriptionDurationBaseMana[256];
	char descriptionDurationBluntDR[256];
	char descriptionDurationChopDR[256];
	char descriptionDurationPierceDR[256];
	char descriptionDurationSlashDR[256];
	char descriptionDurationEarthDR[256];
	char descriptionDurationFireDR[256];
	char descriptionDurationWaterDR[256];
	char descriptionDurationLightningDR[256];

	char descriptionTargeted[256];
	char descriptionTargetedDamageType[256];
	char descriptionTargetedRange[256];
	char descriptionTargetedTargeted[256];
	char descriptionTargetedDiceDamage[256];
	char descriptionTargetedDiceDamageMult[256];
	char descriptionTargetedDamage[256];
	char descriptionTargetedStatus[256];
	char descriptionTargetedStatusDiceDamage[256];
	char descriptionTargetedStatusDamage[256];
	char descriptionTargetedStatusDiceDuration[256];
	char descriptionTargetedStatusDuration[256];
	char descriptionTargetedAOENova[256];
	char descriptionTargetedAOELine[256];
	char descriptionTargetedDiceDuration[256];
	char descriptionTargetedDuration[256];
	char descriptionTargetedActions[256];
	char descriptionTargetedSTR[256];
	char descriptionTargetedDEX[256];
	char descriptionTargetedCON[256];
	char descriptionTargetedWIS[256];
	char descriptionTargetedINT[256];
	char descriptionTargetedWILL[256];
	char descriptionTargetedCHR[256];
	char descriptionTargetedLUCK[256];
	char descriptionTargetedAC[256];
	char descriptionTargetedAttack[256];
	char descriptionTargetedDamageMod[256];
	char descriptionTargetedMvmt[256];
	char descriptionTargetedDiceHp[256];
	char descriptionTargetedHp[256];
	char descriptionTargetedBaseHp[256];
	char descriptionTargetedBaseMana[256];
	char descriptionTargetedBluntDR[256];
	char descriptionTargetedChopDR[256];
	char descriptionTargetedPierceDR[256];
	char descriptionTargetedSlashDR[256];
	char descriptionTargetedEarthDR[256];
	char descriptionTargetedFireDR[256];
	char descriptionTargetedWaterDR[256];
	char descriptionTargetedLightningDR[256];

	char descriptionInstant[256];
	char descriptionInstantDamageType[256];
	char descriptionInstantRange[256];
	char descriptionInstantExtraAttack[256];
	char descriptionInstantDiceDamage[256];
	char descriptionInstantDiceDamageMult[256];
	char descriptionInstantDamage[256];
	char descriptionInstantStatus[256];
	char descriptionInstantStatusDiceDamage[256];
	char descriptionInstantStatusDamage[256];
	char descriptionInstantStatusDiceDuration[256];
	char descriptionInstantStatusDuration[256];
	char descriptionInstantActions[256];
	char descriptionInstantAttack[256];
	char descriptionInstantDamageMod[256];
	char descriptionInstantMvmt[256];
	char descriptionInstantDiceHp[256];
	char descriptionInstantHp[256];
	char descriptionInstantBaseHp[256];

	ability * abilityInsance;
	ability * abilityLevelUpOriginal;
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

	fixedCharacter * abilityViewWindow;
	fixedCharacter * selector;
} abilityView;

#endif /* SRC_HEADERS_STRUCTS_ABILITIES_H_ */
