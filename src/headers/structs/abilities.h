/*
 * abilities.h
 *
 *  Created on: Feb 22, 2016
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_ABILITIES_H_
#define SRC_HEADERS_STRUCTS_ABILITIES_H_

typedef struct{
	short int effectMagnitude;
	short int manaCost;
} effectAndMana;

typedef struct{
	int size;
	int selectedIndex;
	int defaultStartingIndex; //if things can go positive/negitive, start with 0 cost index
	effectAndMana effectAndManaArray[32];

} effectAndManaMapList;

typedef struct{
	int damageTypeEnabled;
	char damageType;
	int rangeEnabled;
	effectAndManaMapList * range;
	int targetedEnabled;
	effectAndManaMapList * targeted;
	int diceDamageEnabled;
	effectAndManaMapList * diceDamage;
	int diceDamageDurationEnabled;
	effectAndManaMapList * diceDamageDuration;

	int acEnabled;
	effectAndManaMapList * ac;
	int damageModEnabled;
	effectAndManaMapList * damageMod;

	//Note: for DR, ignore mana cost per effect, calculate by total DR
	// roundUp((bluntDR + chop + pierce + slash)/2)
	int bluntDREnabled;
	effectAndManaMapList * bluntDR;
	int chopDREnabled;
	effectAndManaMapList * chopDR;
	int pierceDREnabled;
	effectAndManaMapList * pierceDR;
	int slashDREnabled;
	effectAndManaMapList * slashDR;

} property;

typedef struct{
	char name[32];
	char description[128];
	int totalManaCost;

	property properties[16];
} effect;

typedef struct{
	char type;
	char name[32];
	int totalManaCost;

	effect effects[16];
}ability;

#endif /* SRC_HEADERS_STRUCTS_ABILITIES_H_ */
