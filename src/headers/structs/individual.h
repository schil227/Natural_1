/*
 * individual.h
 *
 *  Created on: Apr 14, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_INDIVIDUAL_H_
#define SRC_HEADERS_STRUCTS_INDIVIDUAL_H_
//#include"../character_pub_methods.h"
#include"../item_pub_methods.h"
#include"../abilities_pub_methods.h"

#include<math.h>

typedef enum { STATUS_NONE, STATUS_POISONED, STATUS_PARALYZED, STATUS_CONFUSED, STATUS_BURNING, STATUS_BLEEDING, STATUS_BERZERK, STATUS_SILENCED} statusEffect;

typedef enum { GROUP_ALLIES, GROUP_ENEMIES, GROUP_BEASTS, GROUP_NPCS, GROUP_GUARDS} groupType;

typedef struct{
	int x;
	int y;
} cord;

typedef struct{
	cord * cords[50];
	int numCords;
	int MAX_CORDS;
}cordArr;

typedef struct{
	int inventorySize;
	item * inventoryArr[40];
} inventory;

typedef struct{
	int remaningTurns;
	item * thisItem;
} activeItem;

typedef struct{
	int activeItemsTotal;
	int MAX_ABILITIES;
	activeItem * activeItemArr[40];
} activeItemList;

typedef struct{
	int numAbilities;
	int MAX_ABILITIES;
	ability * abilitiesList[64];
}abilityList;

typedef struct{
	int numAbilities;
	int MAX_ABILITIES;
	ability * selectedAbility;
	activeAbility * abilitiesList[64];
}activeAbilityList;

typedef struct{
	statusEffect effect;
	int turnsRemaining;
	int damage;
	int diceDamage;
}status;

typedef struct{
	int numStatuses;
	int MAX_STATUSES;
	status * statuses[16];
}statusList;

struct individual{
	character* playerCharacter;
	char name[32];
	int ID;

	inventory * backpack;
	activeItemList * activeItems;

	abilityList * abilities;
	activeAbilityList * activeAbilities;
	statusList * activeStatuses;

	//for animating
	character * weaponItem;
	character * armorItem;
	character * shieldItem;

	short int STR;
	short int DEX;
	short int CON;
	short int WILL;
	short int INT;
	short int WIS;
	short int CHR;
	short int LUCK;

	short int baseHP;
	short int hp;
	short int baseMana;
	short int mana;
	short int totalActions;
	short int remainingActions;
	short int hasAttacked;

	short int AC;
	short int attack;
	short int maxDam;
	short int minDam;
	short int baseDam;
	short int range;
	char critType[4];
	short int mvmt;
	short int LoS;
	short int isSneaking;

	//physical damage resistance
	short int bluntDR;
	short int chopDR;
	short int slashDR;
	short int pierceDR;

	//magic damage resistance
	short int earthDR;
	short int fireDR;
	short int waterDR;
	short int lightningDR;

	short int earthWeakness;
	short int fireWeakness;
	short int waterWeakness;
	short int lightiningWeakness;

	int dialogID;

	int gold;

	int jumpTarget;

	int faction;
	int targetedDuration;
	struct individual * targetedIndividual;

	groupType currentGroupType;
};

struct individual;
typedef struct individual individual;


#endif /* SRC_HEADERS_STRUCTS_INDIVIDUAL_H_ */
