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

typedef enum { STATUS_NONE, STATUS_POISONED, STATUS_PARALYZED, STATUS_CONFUSED, STATUS_BURNING, STATUS_BLEEDING, STATUS_BERZERK, STATUS_SILENCED, STATUS_SLEEPING} statusEffect;

typedef enum { GROUP_ALLIES, GROUP_ENEMIES, GROUP_BEASTS, GROUP_NPCS, GROUP_GUARDS, GROUP_NULL} groupType;

//Ordered from least to most severe
typedef enum { CRIME_NONE, CRIME_STEALING, CRIME_PICKPOCKETING, CRIME_ASSULT, CRIME_MURDER, CRIME_TREASON} crimeType;

typedef enum {DIALOG_DEFAULT, DIALOG_CRIME_WITNESS, DIALOG_HOSTILE_TO_PLAYER, DIALOG_ATTACKED_BY_PLAYER, DIALOG_STOLEN_FROM_BY_PLAYER, DIALOG_AFRAID_OF_PLAYER} dialogType;

typedef struct{
	int x;
	int y;
} cord;

typedef struct{
	cord * cords[300];
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

typedef struct{
	dialogType activeDialog;
	int sawPlayerCrime;
	int hostileToPlayer;
	int attackedByPlayer;
	int stolenFromByPlayer;
	int afraidOfPlayer;
	int playerIsMarkedForDeath;
} specialDialogs;

typedef struct{
	int offensiveness;
	int abilityAffinity;
	int tacticalness;
	int cowardness;

	int isOffensive;
	int hasAbilityAffinity;
	int isTactical;
	int isCowardly;
	int cowardlyTurnsRemaining;

	int isHostileToPlayer;
	int hasAlreadyYieldedToPlayer;
	int isFocusedOnPlayer;
	int isSurrounded;
	int isThreatened;
	int wasRecentlyAttacked;
	int alertDuration;
	int gotConfused;
}behavior;

typedef struct{
	crimeType crime;
	int crimeBounty;
	int witnessID;
	int victimID;
	int stolenItemID;
} activeCrimeEntry;

typedef struct{
	activeCrimeEntry * activeCrimeList[200];
	int numActiveCrimes;
	int MAX_ACTIVE_CRIMES;
} activeCrimes;

typedef struct{
	activeCrimeEntry * reportedCrimeList[200];
	int numReportedCrimes;
	int MAX_REPORTED_CRIMES;
} reportedCrimes;

struct individual{
	character* playerCharacter;
	char name[32];
	int ID;
	int isPlayer;

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

	behavior * thisBehavior;
	specialDialogs * specialDialog;
	struct individual * targetedIndividual;
	struct individual * allyIndividual;
	cord * desiredLocation;
	reportedCrimes * thisReportedCrimes;
	activeCrimes * thisActiveCrimes;
	groupType currentGroupType;
	groupType defaultGroupType;
};

struct individual;
typedef struct individual individual;


#endif /* SRC_HEADERS_STRUCTS_INDIVIDUAL_H_ */
