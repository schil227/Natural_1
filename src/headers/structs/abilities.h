/*
 * abilities.h
 *
 *  Created on: Feb 22, 2016
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_ABILITIES_H_
#define SRC_HEADERS_STRUCTS_ABILITIES_H_

#include<stdlib.h>

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
	char name[32];
	char description[128];
	int totalManaCost;

	short int damageTypeEnabled;
	char damageType;
	short int rangeEnabled;
	effectAndManaMapList * range;
	short int targetedEnabled;
	effectAndManaMapList * targeted;
	short int diceDamageEnabled;
	effectAndManaMapList * diceDamage;
	short int diceDamageDurationEnabled;
	effectAndManaMapList * diceDamageDuration;

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

	//Note: for DR, ignore mana cost per effect, calculate by total DR
	// roundUp((bluntDR + chop + pierce + slash)/2)
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
} effect;

typedef struct{
	char type;
	char name[32];
	int totalManaCost;

	effect effects[16];
}ability;

#endif /* SRC_HEADERS_STRUCTS_ABILITIES_H_ */
