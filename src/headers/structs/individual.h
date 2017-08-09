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
#include"../interactable_pub_methods.h"

#include<math.h>

typedef enum { STATUS_NONE, STATUS_POISONED, STATUS_PARALYZED, STATUS_CONFUSED, STATUS_BURNING, STATUS_BLEEDING, STATUS_BERZERK, STATUS_SILENCED, STATUS_SLEEPING} statusEffect;

typedef enum { GROUP_ALLIES, GROUP_ENEMIES, GROUP_BEASTS, GROUP_NPCS, GROUP_GUARDS, GROUP_NULL} groupType;

//Ordered from least to most severe
typedef enum { CRIME_NONE, CRIME_STEALING, CRIME_PICKPOCKETING, CRIME_ASSULT, CRIME_MURDER, CRIME_TREASON} crimeType;

typedef enum {DIALOG_DEFAULT, DIALOG_CRIME_WITNESS, DIALOG_HOSTILE_TO_PLAYER, DIALOG_ATTACKED_BY_PLAYER, DIALOG_STOLEN_FROM_BY_PLAYER, DIALOG_AFRAID_OF_PLAYER} dialogType;

typedef enum {SPREAD_12_4, SPREAD_10_6, SPREAD_8_8, SPREAD_6_10, SPREAD_4_12} diceSpread;

typedef struct{
	cord * cords[300];
	int numCords;
	int MAX_CORDS;
}cordArr;

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
	int respawns;
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

typedef struct {
	int x;
	int y;
	int hasTraversed;
	struct moveNode * nextMoveNode;
} moveNode;

typedef struct{
	int sum;
	int pathLength;
	int useDummyCords;
	character * shadowCharacter;
	moveNode * rootMoveNode;
	cord * dummyCord;
} moveNodeMeta;

struct individual{
	character* playerCharacter;
	char name[32];
	int ID;
	int isPlayer;
	int level;
	int exp;

	diceSpread spread;

	inventory * backpack;
	activeItemList * activeItems;

	abilityList * abilities;
	activeAbilityList * activeAbilities;
	statusList * activeStatuses;
	interactable * currentInteractableObject;
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
	short int baseFood;
	double food;
	short int foodBonusActive;
	short int foodNegativeActive;

	short int AC;
	short int attack;
	short int maxDam;
	short int minDam;
	short int baseDam;
	short int range;
	char critType[4];
	short int mvmt;
	short int LoS;
	short int darkLoS;
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

	int dialogID;
	int dialogPortraitID;

	int fateTokens;
	int gold;

	int jumpTarget;
	int numTurnsInOptionalAction;

	int faction;
	int targetedDuration;
	int bonusMana;

	behavior * thisBehavior;
	specialDialogs * specialDialog;
	struct individual * targetedIndividual;
	struct individual * allyIndividual;
	cord * desiredLocation;
	reportedCrimes * thisReportedCrimes;
	activeCrimes * thisActiveCrimes;
	groupType currentGroupType;
	groupType defaultGroupType;
	moveNodeMeta * thisMoveNodeMeta;
};

struct individual;
typedef struct individual individual;


#endif /* SRC_HEADERS_STRUCTS_INDIVIDUAL_H_ */
