/*
 * test_main.c
 *
 *  Created on: Mar 21, 2015
 *      Author: Adrian
 */

#include<assert.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"../src/headers/general.h"
//#include"./test_headers/test_general.h"
//#include"./test_headers/test_character.h"
#include"../src/headers/individual_pub_methods.h"
#include"../src/headers/field_pub_methods.h"
#include"../src/headers/console_pub_methods.h"
#include"../src/headers/enemy_controller_pub_methods.h"
#include"../src/headers/dialog_pub_methods.h"

static char * mapTestDirectory = "C:\\Users\\Adrian\\C\\Natural_1_new_repo\\unit_tests\\testMaps\\";//".\\unit_tests\\testMaps\\";//

//individual* testPlayer;
//individualGroup* testGroupContainer->enemies;
//individualGroup* testGroupContainer->npcs;
//field* main_test_field;
//shiftData* testShiftData;

actionAITest(individual* testPlayer, groupContainer * testGroupContainer, field* testField){
	int i;
	individualGroup * testEnemies = testGroupContainer->enemies;
	individualGroup * testGuards = testGroupContainer->guards;

	//Only individual remaining after tests
	individual * testEnemy = testEnemies->individuals[4];
	int tmpX = testEnemy->playerCharacter->x;
	int tmpY = testEnemy->playerCharacter->y;

	assert(!isLowOnMana(testEnemy));

	testEnemy->mana = 0;

	assert(isLowOnMana(testEnemy));

	channelMana(testEnemy);
	assert(testEnemy->mana == 2);

	assert(!isLowOnHP(testEnemy));

	//raise base by significant magnitute, hp looks low
	testEnemy->baseHP = 10;
	assert(isLowOnHP(testEnemy));

	ability * randHealAbility = getRandomHPRestoringAbility(testEnemy);
	assert(strcmp(randHealAbility->name, "Heal Target") == 0);

	ability * randOffensiveAbility = getRandomOffensiveAbility(testEnemy);
	assert(strcmp(randOffensiveAbility->name, "Fireball") == 0);

	ability * randBuffAbility = getRandomBuffAbility(testEnemy);
	assert(strcmp(randBuffAbility->name, "Focus") == 0);

//	item * randHealingItem = getRandomHPRestoringItem(testEnemy->backpack);
//	assert(strcmp(randHealingItem->name, "Herb") == 0);

	//uses healing ability, not item
	rand();
	rand();
	tryHeal(testEnemy,testPlayer, testGroupContainer, testField);
	assert(testEnemy->hp == 4);

	assert(isGreaterThanPercentage(5,10,2));
	assert(!isGreaterThanPercentage(2,10,25));

	assert(!abilityIsOffensive(randHealAbility));
	assert(abilityIsOffensive(randOffensiveAbility));

	assert(atDesiredLocation(testEnemy));

	testEnemy->playerCharacter->x = 25;
	testEnemy->playerCharacter->y = 25;
	assert(!atDesiredLocation(testEnemy));

	testEnemy->playerCharacter->x = tmpX;
	testEnemy->playerCharacter->y = tmpY;

	//move to 4,1 4,3 (between trees) and test LoS
	assert(removeIndividualFromField(testField, testPlayer->playerCharacter->x, testPlayer->playerCharacter->y));
	assert(removeIndividualFromField(testField, testEnemy->playerCharacter->x, testEnemy->playerCharacter->y));

	assert(moveIndividualSpace(testField, testPlayer, 4,1));
	assert(!moveIndividualSpace(testField, testEnemy, 4,1));
	assert(moveIndividualSpace(testField, testEnemy, 4,3));

	/*  P
	 * TTT
	 *  E
	 */

	//RAND: 1/10 chance they wont notice (greater if sneaking)
	assert(!isInLineOfSight(testEnemy, testPlayer, testField));

	assert(removeIndividualFromField(testField, testEnemy->playerCharacter->x, testEnemy->playerCharacter->y));
	assert(moveIndividualSpace(testField, testEnemy, 0,1));

	/* E   P
	 *    TTT
	 */
	assert(isInLineOfSight(testEnemy, testPlayer, testField));

	assert(removeIndividualFromField(testField, testEnemy->playerCharacter->x, testEnemy->playerCharacter->y));
	assert(moveIndividualSpace(testField, testEnemy, 2,2));

	/*     P
	 *   ETTT
	 */
	assert(!isInLineOfSight(testEnemy, testPlayer, testField));

	assert(removeIndividualFromField(testField, testEnemy->playerCharacter->x, testEnemy->playerCharacter->y));
	assert(moveIndividualSpace(testField, testEnemy, 1,2));

	/*     P		Edge case, passes through {[1,2],[2,2],[3,2]!,[3,1],[4,1]}
	 *  E TTT
	 */
	assert(!isInLineOfSight(testEnemy, testPlayer, testField));

	assert(removeIndividualFromField(testField, testEnemy->playerCharacter->x, testEnemy->playerCharacter->y));
	assert(moveIndividualSpace(testField, testEnemy, 0,2));

	/*     P
	 * E  TTT
	 */
	assert(isInLineOfSight(testEnemy, testPlayer, testField));

	assert(strcmp(((individual *)getClosestEnemyInLoS(testEnemy, testField))->name, testPlayer->name) == 0);
	checkForTargets(testEnemy, testPlayer, testGroupContainer, testField);

	assert(strcmp(testEnemy->targetedIndividual->name, testPlayer->name) == 0);

	//Used for cowardly actions, range is radius + 1
	assert(!noEnemiesInRange(testEnemy, testGroupContainer, testField, 2));
	assert(noEnemiesInRange(testEnemy, testGroupContainer, testField, 1));

	assert(!abilityInRangeOfIndividual(randOffensiveAbility, testEnemy, testPlayer));

	assert(removeIndividualFromField(testField, testEnemy->playerCharacter->x, testEnemy->playerCharacter->y));
	assert(moveIndividualSpace(testField, testEnemy, 1,2));

	assert(abilityInRangeOfIndividual(randOffensiveAbility, testEnemy, testPlayer));

	testEnemy->activeAbilities->selectedAbility = randOffensiveAbility;

	assert(testPlayer->hp == 15);

	//Fireball player on their space
	useAbilityOnTargetedSpace(testEnemy, testPlayer, testGroupContainer, testField, 4,1);

	assert(testPlayer->hp == 12);

	testEnemy->targetedIndividual = testPlayer;
	testEnemy->targetedDuration = 3;
	assert(moveCloserToTarget(testEnemy, testPlayer, testField));

	//As a side effect of moveCloserToTarget, the individuals is removed from the field
	//returning individual to field:
	assert(moveIndividualSpace(testField, testEnemy, 1,2));

	assert(testEnemy->thisMoveNodeMeta->pathLength == 2);

	moveNode * tmpMoveNode = testEnemy->thisMoveNodeMeta->rootMoveNode;
	assert(tmpMoveNode->x == 2 && tmpMoveNode->y == 1);

	tmpMoveNode = tmpMoveNode->nextMoveNode;
	assert(tmpMoveNode->x == 3 && tmpMoveNode->y == 0);

	freeUpMovePath(testEnemy->thisMoveNodeMeta->rootMoveNode);
	testEnemy->thisMoveNodeMeta->pathLength = 0;

	// For some odd reason, running normally and running with debug causes the retreat space to differ;
	// reinitializing the random seed causes consistant results; either form are correct since this method
	// has a large set of acceptable outcomes.
	srand(0);
	cord * targetSpace = findRetreatSpace(testEnemy, testGroupContainer, testField, testEnemy->targetedIndividual->playerCharacter->x, testEnemy->targetedIndividual->playerCharacter->y);
	printf("[%d,%d], %s\n", targetSpace->x, targetSpace->y, testEnemy->targetedIndividual->name);

	assert(targetSpace->x == 0 && targetSpace->y == 0);

	assert(selectHealingAbility(testEnemy));

	assert(strcmp(testEnemy->activeAbilities->selectedAbility->name,randHealAbility->name) == 0);

	individual * testGuard = testGuards->individuals[0];

	assert(removeIndividualFromField(testField, testGuard->playerCharacter->x, testGuard->playerCharacter->y));
	assert(moveIndividualSpace(testField, testGuard, 0,0));

	findDangerousIndividualNearBy(testGuard, testPlayer,testGroupContainer, testField, 10);

	assert(testGuard->targetedIndividual->ID == testEnemy->ID);

	individualGroup * guardAllies = getAlliesInRange(testGuard, testField, 10);

	assert(guardAllies->numIndividuals == 2);

	testPlayer->hp = 1;

	assert(((individual*)allyRequiringHealing(testGuard, guardAllies))->ID == testPlayer->ID);

	testPlayer->hp = 20;

	assert(allyRequiringHealing(testGuard, guardAllies) == NULL);

	free(guardAllies);

	assert(testEnemy->thisBehavior->isOffensive == 0);
	assert(testEnemy->thisBehavior->isTactical == 0);
	assert(testEnemy->thisBehavior->isCowardly == 0);
	assert(testEnemy->thisBehavior->hasAbilityAffinity == 0);

		rerollBehavior(testEnemy);

	assert(testEnemy->thisBehavior->isOffensive == 1);
	assert(testEnemy->thisBehavior->isTactical == 1);
	assert(testEnemy->thisBehavior->isCowardly == 0);
	assert(testEnemy->thisBehavior->hasAbilityAffinity == 0);

	assert(getCurrentBounty(testPlayer) == 0);

	addActiveCrime(testPlayer, CRIME_ASSULT, 40, 0, 0, 0);

	assert(getCurrentBounty(testPlayer) == 0);

	reportActiveCrimes(testPlayer);

	assert(getCurrentBounty(testPlayer) == 40);
}

int mainTest(individual* testPlayer, groupContainer * testGroupContainer, field* main_test_field, shiftData * testShiftData) {
	//setup

	BITMAP bm;
	UINT ret;

	int x, y;
	int imageID;
	int dummyActionMode = 0;

	assert(testGroupContainer->enemies->numIndividuals == 6);

	assert(testGroupContainer->npcs->numIndividuals == 1);

	//player is on field at their coordinates, name check
	individual * tmpIndividual;// = malloc(sizeof(tmpIndividual));
	tmpIndividual = getIndividualFromField( main_test_field,testPlayer->playerCharacter->x, testPlayer->playerCharacter->y);

	assert(!strcmp(tmpIndividual->name,	testPlayer->name));

	//enemies are on field at their coordinates, name check
	tmpIndividual = getIndividualFromField( main_test_field,testGroupContainer->enemies->individuals[0]->playerCharacter->x, testGroupContainer->enemies->individuals[0]->playerCharacter->y);
	assert(!strcmp(tmpIndividual->name, testGroupContainer->enemies->individuals[0]->name));

	tmpIndividual = getIndividualFromField( main_test_field,testGroupContainer->enemies->individuals[1]->playerCharacter->x, testGroupContainer->enemies->individuals[1]->playerCharacter->y);
	assert(!strcmp(tmpIndividual->name, testGroupContainer->enemies->individuals[1]->name));

	tmpIndividual = getIndividualFromField( main_test_field,testGroupContainer->enemies->individuals[2]->playerCharacter->x, testGroupContainer->enemies->individuals[2]->playerCharacter->y);
	assert(!strcmp(tmpIndividual->name, testGroupContainer->enemies->individuals[2]->name));

	tmpIndividual = getIndividualFromField( main_test_field,testGroupContainer->enemies->individuals[3]->playerCharacter->x, testGroupContainer->enemies->individuals[3]->playerCharacter->y);
	assert(!strcmp(tmpIndividual->name, testGroupContainer->enemies->individuals[3]->name));

	tmpIndividual = getIndividualFromField( main_test_field,testGroupContainer->enemies->individuals[4]->playerCharacter->x, testGroupContainer->enemies->individuals[4]->playerCharacter->y);
	assert(!strcmp(tmpIndividual->name, testGroupContainer->enemies->individuals[4]->name));

	tmpIndividual = getIndividualFromField( main_test_field,testGroupContainer->enemies->individuals[5]->playerCharacter->x, testGroupContainer->enemies->individuals[5]->playerCharacter->y);
	assert(!strcmp(tmpIndividual->name, testGroupContainer->enemies->individuals[5]->name));

	int i, individualsPassed = 0;
	for(i = 0; i < testGroupContainer->enemies->MAX_INDIVIDUALS; i++){

		if(testGroupContainer->enemies->individuals[i] != NULL){
			individualsPassed++;
			testEnemyAction((testGroupContainer->enemies->individuals[i]), main_test_field, testPlayer);

			if(individualsPassed == testGroupContainer->enemies->numIndividuals){
				break;
			}
		}

	}

	//skeleton 1&5 too far away, same starting position
	tmpIndividual =testGroupContainer->enemies->individuals[0];
	assert(tmpIndividual->playerCharacter->x == 10 &&
			tmpIndividual->playerCharacter->y == 0 );
	tmpIndividual =testGroupContainer->enemies->individuals[5];
	assert(tmpIndividual->playerCharacter->x == 9 &&
			tmpIndividual->playerCharacter->y == 15 );

//	//skeletons 2,3,4,5 move towards the player
	tmpIndividual =testGroupContainer->enemies->individuals[1];
	assert(tmpIndividual->playerCharacter->x == 8 &&
			tmpIndividual->playerCharacter->y == 2 );
	tmpIndividual =testGroupContainer->enemies->individuals[2];
	assert(tmpIndividual->playerCharacter->x == 7 &&
			tmpIndividual->playerCharacter->y == 1 );
	tmpIndividual =testGroupContainer->enemies->individuals[3];
	assert(tmpIndividual->playerCharacter->x == 8 &&
			tmpIndividual->playerCharacter->y == 1 );
	tmpIndividual =testGroupContainer->enemies->individuals[4];
	assert(tmpIndividual->playerCharacter->x == 6 &&
			tmpIndividual->playerCharacter->y == 0 );


	//get full node path of closest skeleton to player
	nodeArr * resultArr = getFullNodePath(main_test_field, testPlayer, testPlayer->playerCharacter->x, testPlayer->playerCharacter->y,
			testGroupContainer->enemies->individuals[4]->playerCharacter->x, testGroupContainer->enemies->individuals[4]->playerCharacter->y);

	assert(resultArr->nodeArray[0]->x == 2 && resultArr->nodeArray[0]->y == 0);
	assert(resultArr->nodeArray[1]->x == 3 && resultArr->nodeArray[1]->y == 0);
	assert(resultArr->nodeArray[2]->x == 4 && resultArr->nodeArray[2]->y == 0);
	assert(resultArr->nodeArray[3]->x == 5 && resultArr->nodeArray[3]->y == 0);
	assert(resultArr->nodeArray[4]->x == 6 && resultArr->nodeArray[4]->y == 0);

	//get the node path to the first open space within the movement range
	resultArr = getSpaceClosestToPlayer(main_test_field, testGroupContainer->enemies->individuals[4], testPlayer);

	assert(resultArr->nodeArray[0]->x == 5 && resultArr->nodeArray[0]->y == 0);
	assert(resultArr->nodeArray[1]->x == 4 && resultArr->nodeArray[0]->y == 0);
	assert(resultArr->nodeArray[2]->x == 3 && resultArr->nodeArray[0]->y == 0);

	free(resultArr);

	//warping player to space with herb
	moveIndividualSpace(main_test_field,testPlayer,3,3);
	attemptGetItemFromField(main_test_field,testPlayer);

	//warping player to space with dagger
	moveIndividualSpace(main_test_field,testPlayer,3,4);
	attemptGetItemFromField(main_test_field,testPlayer);

	assert(testPlayer->backpack->inventorySize == 2);

	//pre-dagger equip:
	assert(getAttributeSum(testPlayer,"attack") == 3);

	//equip the dagger
	modifyItem(testPlayer->backpack->inventoryArr[1], testPlayer);

	//check attack for increase after equip
	assert(getAttributeSum(testPlayer,"attack") == 4);

	//stunt hp to test herb healing over duration
	testPlayer->hp -= 8;
	assert(testPlayer->hp ==  14);

	srand(1);

	//consume herb as active duration item
	modifyItem(testPlayer->backpack->inventoryArr[0], testPlayer);

	//herb heals the player for two
	assert(testPlayer->hp == 16);

	startTurn(testPlayer);
	assert(testPlayer->hp == 18);
	startTurn(testPlayer);
	assert(testPlayer->hp == 20);
	startTurn(testPlayer);
	assert(testPlayer->hp ==  22);
	startTurn(testPlayer);
	assert(testPlayer->hp ==  22);

	//warp player back to original spot
	moveIndividualSpace(main_test_field,testPlayer,1,1);

	//move the enemies two more times, attack the player
	individualsPassed = 0;
	for(i = 0; i < testGroupContainer->enemies->MAX_INDIVIDUALS; i++){

		if(testGroupContainer->enemies->individuals[i] != NULL){
			individualsPassed++;
			testEnemyAction((testGroupContainer->enemies->individuals[i]), main_test_field, testPlayer);

			if(individualsPassed == testGroupContainer->enemies->numIndividuals){
				break;
			}
		}

	}

	// eat some rands to make the skellys attack the player
	for(i = 0; i < 24; i++){
		printf("%d, ", rand() % 20);
	}

	fflush(stdout);

	individualsPassed = 0;
	for(i = 0; i < testGroupContainer->enemies->MAX_INDIVIDUALS; i++){

		if(testGroupContainer->enemies->individuals[i] != NULL){
			individualsPassed++;
			testEnemyAction((testGroupContainer->enemies->individuals[i]), main_test_field, testPlayer);

			if(individualsPassed == testGroupContainer->enemies->numIndividuals){
				break;
			}
		}

	}

	//while skeletons 2, 3, and 4 all moved without being blocked, 5 reacts to the blocked path
	//and moves to [3,1]
	tmpIndividual = testGroupContainer->enemies->individuals[4];
	assert(tmpIndividual->playerCharacter->x == 3 && tmpIndividual->playerCharacter->y == 1 );

	//after being attacked 2 (missed once), player health is 17
	assert(testPlayer->hp == 19);

	srand(2);

	//skeleton0 cannot attack the player, vis versa
	assert(!isInAttackRange(testGroupContainer->enemies->individuals[0],testPlayer, main_test_field));
	assert(!isInAttackRange(testPlayer,testGroupContainer->enemies->individuals[0], main_test_field));

	//skeleton5 cannot reach the player, but the player can attack
	assert(!isInAttackRange(testGroupContainer->enemies->individuals[4],testPlayer, main_test_field));
	assert(isInAttackRange(testPlayer,testGroupContainer->enemies->individuals[4], main_test_field));

	rand();

	//player attacks skeleton5, skeleton5 dies
	assert(attackIndividual(testPlayer,testGroupContainer->enemies->individuals[4]));

	srand(3);

	//warping player next to doorway
	moveIndividualSpace(main_test_field,testPlayer,6,9);

	//fails, not a transitional space
	assert(!attemptToTransit(&main_test_field, testPlayer, testGroupContainer, testShiftData, mapTestDirectory, 0));

	//warping player to doorway
	moveIndividualSpace(main_test_field,testPlayer,6,8);

	//works, player is in a transitional space - new field is loaded
	assert(attemptToTransit(&main_test_field, testPlayer, testGroupContainer, testShiftData,mapTestDirectory, 0));

	printf("Player location %d,%d\n", testPlayer->playerCharacter->x,testPlayer->playerCharacter->y);

	//player was transported to new map, new position is [2,3]
	assert(testPlayer->playerCharacter->x == 2 && testPlayer->playerCharacter->y == 3);

	//transport back
	assert(attemptToTransit(&main_test_field, testPlayer, testGroupContainer, testShiftData,mapTestDirectory, 0));

	//player is back at map1 doorway
	assert(testPlayer->playerCharacter->x == 6 && testPlayer->playerCharacter->y == 8);

	srand(4);

	//try talking to npc, too far away
	assert(!tryTalkIndividualFromField(testPlayer,main_test_field,0,2));

	//warp player closer to npc
	moveIndividualSpace(main_test_field,testPlayer,1,1);

	//talking will be successful now:
	assert(tryTalkIndividualFromField(testPlayer,main_test_field,0,2));

	//advance dialog (enter) "Hello"
	advanceDialog();

	//select dialog "Do you like my robes?
	nextDialogDecision(); //"no"

	//select no - npc will be mad
	advanceDialog();

	//select dialog "are you trying to anger me?
	nextDialogDecision(); //"no - CHR Check"

	//select no - do CHR Check
	advanceDialog();

	//
	advanceDialog();

	//event attached to dialog, npc becomes hostile
	assert(getEventFromCurrentMessage());

	individual * tmpNPC = getIndividualFromRegistry(testGroupContainer->npcs->individuals[0]->ID);

	//process the CHR Check, successful
	processEvent(getEventFromCurrentMessage(), testPlayer, testGroupContainer, main_test_field, &dummyActionMode);

	createPermanentAbiltyTest(testPlayer);
	createSelfDurationAbility(testPlayer);
	createTargetedAbility(testPlayer);
	createInstanceAbility(testPlayer);

	/*use self duraiton ability,
	 * poison testPlayer and npc,
	 *  -2 AC/-1 attack,
	 *   npc becomes enemy
	 */

	srand(5);

	//update ability view
	refreshAbilityView(testPlayer->abilities->numAbilities, testPlayer->abilities->abilitiesList);

	//chose duration ability from view
	selectNextAbility();

	//verify ability can be used
	assert(canUseAbility(testPlayer, getAbilityToActivate()));

	//player didn't die by using this ability (impossible, it's not an instant ability), abiltiy is set to selectedAbility
	assert(!useAbility(testPlayer, getAbilityToActivate()));

	//user is within range of self ability
	assert(cursorWithinAbilityRange(testPlayer, testPlayer->playerCharacter->x, testPlayer->playerCharacter->y));

	//user's hp before poison affects it
	assert(testPlayer->hp == 19);

	// eat rand to make poison last 1 turn
	rand();
	rand();
	rand();

	//use duration ability on everyone within range
	useAbilityOnIndividualsInAOERange(testPlayer, testPlayer, testGroupContainer, main_test_field, testPlayer->playerCharacter->x, testPlayer->playerCharacter->y, testPlayer->playerCharacter->x, testPlayer->playerCharacter->y);

	//clear selectedAbility
	testPlayer->activeAbilities->selectedAbility = NULL;

	//since duration ability was harmful, npc is now an enemy
	assert(!individualInGroup(tmpNPC, testGroupContainer->npcs));
	assert(individualInGroup(tmpNPC, testGroupContainer->enemies));

	//After spell cast hp check
	assert(testPlayer->hp == 16);

	//player and the NPC are poisoned
	assert(testPlayer->activeStatuses->statuses[0]->effect == STATUS_POISONED);
	assert(tmpNPC->activeStatuses->statuses[0]->effect == STATUS_POISONED);

	//status magnitude
	assert(testPlayer->activeStatuses->statuses[0]->turnsRemaining == 1);
	assert(testPlayer->activeStatuses->statuses[0]->damage == 1);
	assert(testPlayer->activeStatuses->statuses[0]->diceDamage == 4);

	//after new turn, poison has fewer turns, damages individual
	startTurn(testPlayer);
	assert(testPlayer->activeStatuses->statuses[0]->turnsRemaining == 0);
	assert(testPlayer->hp == 14);

	srand(6);
	/*
	 * use targeted ability on enemy
	 */
	refreshAbilityView(testPlayer->abilities->numAbilities, testPlayer->abilities->abilitiesList);

	//go to targeted ability from view
	selectNextAbility();//targeted

	//Player doesn't have enough mana!
	assert(!canUseAbility(testPlayer, getAbilityToActivate()));

	//give the player some mana
	restoreMana(testPlayer, 10);

	//now the ability can be used
	assert(canUseAbility(testPlayer, getAbilityToActivate()));

	//player didn't die by using this ability (impossible, it's not an instant ability), abiltiy is set to selectedAbility
	assert(!useAbility(testPlayer, getAbilityToActivate()));

	//ability is NOT within range beyond the selected enemy (skelly)
	assert(!cursorWithinAbilityRange(testPlayer, 0, 11));

	//ability is in range of the enemy
	assert(cursorWithinAbilityRange(testPlayer, 0, 10));

	//get skelly, the targeted enemy
	tmpIndividual = testGroupContainer->enemies->individuals[0];

	//skelly is in good health
	assert(tmpIndividual->hp == 8);

	srand(7);

	//use ability on selected space,  harming skelly4
	useAbilityOnIndividualsInAOERange(testPlayer, testPlayer, testGroupContainer, main_test_field, tmpIndividual->playerCharacter->x, tmpIndividual->playerCharacter->y, tmpIndividual->playerCharacter->x, tmpIndividual->playerCharacter->y);

	//skelly was killed by a 11 damage roll (2d8)
	assert(tmpIndividual->hp == -7);

	/*
 	 * use instant ability to strike NPC several times, pay with several turns
 	 */

	//point to the instant ability
	selectNextAbility();

	//clear old selected ability
	testPlayer->activeAbilities->selectedAbility = NULL;

	//verify the ability can be used
	assert(canUseAbility(testPlayer, getAbilityToActivate()));

	//verify the number of turns the player has before using the ability
	assert(testPlayer->remainingActions == 2);

	//verify player health before dice heal
	assert(testPlayer->hp == 14);

	//assert the player is not killed by the instant action
	assert(!useAbility(testPlayer, getAbilityToActivate()));

	//player healed from diceHP
	assert(testPlayer->hp == 15);

	srand(8);
	//consume more rand() to get better throws

	//give enemy an HP boost, they're gonna need it
	healIndividual(tmpNPC, 10);

	//verify tmpNPC was killed by the multiple attacks
	assert(tryAttackEnemies(testGroupContainer->enemies, testPlayer, main_test_field, tmpNPC->playerCharacter->x, tmpNPC->playerCharacter->y));

	//tmpNPC has -6 hp after barrage of attacks
	assert(tmpNPC->hp == -6);

	//after decreasing the turns, the ability is no longer selected, player has action debt
	testGroupContainer->groupActionMode = 0;
	testGroupContainer->initGroupActionMode = 0;
	decreaseTurns(testPlayer, testGroupContainer, 1, 1);

	//player loses 4 actions, gets 2 replenished on endTurn() call (net: 0 actions for next turn)
	assert(testPlayer->remainingActions == 0);

	//the selected instant ability was automatially cleared on decreaseTurns call
	assert(testPlayer->activeAbilities->selectedAbility == NULL);

	//it is now the enemies turn
	assert(testGroupContainer->groupActionMode);
	assert(testGroupContainer->initGroupActionMode);

	actionAITest(testPlayer,testGroupContainer, main_test_field);

	//reduce food, lose well-fed bonus
	decreaseFood(testPlayer, 100.0);
	assert(getAttributeSum(testPlayer, "STR") == 1);

	//reduce food more, gain hunger negative
	decreaseFood(testPlayer, 225.0);
	assert(getAttributeSum(testPlayer, "STR") == 0);

	//restore back to normal
	testPlayer->food = 300;

	return 0;
}

void createPermanentAbiltyTest(individual * testPlayer){
	//try create ability, fail (mana = -2)
	assert(!canCreateAbility());

	//down to AC
	setAbilityCreationSelectedType(ABILITY_AC);
//	selectNextEffect();

	//try create ability, fail (mana = -1)
	assert(!canCreateAbility());

	//increase AC by 1
	interpretRightAbilityCreation();

	//try create ability, succeed (mana = 0)
	assert(canCreateAbility());

	//ability name is empty - need to fill out
	assert(nameEmpty());

	// Give abiltiy name ABCBA
	selectCharacter();
	selectLetterRight();
	selectCharacter();
	selectLetterRight();
	selectCharacter();
	selectLetterLeft();
	selectCharacter();
	selectLetterLeft();
	selectCharacter();

	assert(!nameEmpty());

	setAbilityName(getNameFromInstance());
	addAbilityToIndividual(testPlayer, getNewAbility());
	changeAbilityTemplate(0);
	resetNameBoxInstance();

	//the name's been reset
	assert(nameEmpty());

	//player now has a perminant ability granting +2 AC
	assert(testPlayer->activeAbilities->numAbilities == 1);
	assert(testPlayer->abilities->numAbilities == 1);
	assert(strcmp(testPlayer->abilities->abilitiesList[0]->name,"ABCBA") == 0);
	assert(getAttributeSum(testPlayer, "ac") == 15);

	//return to abilityType
	selectPreviousEffect();
	selectPreviousEffect();
	selectPreviousEffect();
}

void createSelfDurationAbility(individual * testPlayer){
	int i;

	//select Self Duration ability
	interpretRightAbilityCreation();

	//Down to status
	for(i = 0; i < 6; i++){
		selectNextEffect();
	}

	//status: poison
	setAbilityCreationSelectedType(ABILITY_STATUS);
	interpretRightAbilityCreation();

	//d4 dice damage
	setAbilityCreationSelectedType(ABILITY_STATUS_DICE_DAMAGE);
	interpretRightAbilityCreation();

	//+1 damage
	setAbilityCreationSelectedType(ABILITY_STATUS_DAMAGE);
	interpretRightAbilityCreation();

	//d4 dice duration
	setAbilityCreationSelectedType(ABILITY_STATUS_DICE_DURATION);
	interpretRightAbilityCreation();

	//+1 duration
	setAbilityCreationSelectedType(ABILITY_STATUS_DURATION);
	interpretRightAbilityCreation();

	//+1 AOE
	setAbilityCreationSelectedType(ABILITY_AOE_NOVA);
	interpretRightAbilityCreation();

	//down to AC
	for(i = 0; i < 12; i++){
		selectNextEffect();
	}

	//-2 AC
	setAbilityCreationSelectedType(ABILITY_AC);
	interpretLeftAbilityCreation();
	interpretLeftAbilityCreation();

	//-1 Attack
	selectNextEffect();
	setAbilityCreationSelectedType(ABILITY_ATTACK);
	interpretLeftAbilityCreation();

	//ability is +4 mana, able to create
	assert(canCreateAbility());

	//ability name is empty - need to fill out
	assert(nameEmpty());

	// Give abiltiy name Jill
	selectLetterUp();//at backspace
	selectLetterUp();//at 9
	selectCharacter(); //select 9 accidently
	selectLetterDown();//at backspace
	selectCharacter(); //delete 9
	selectLetterDown();//at J
	selectCharacter(); //J
	selectLetterLeft();//at I
	selectLetterDown();//at V
	selectLetterDown();//at i
	selectCharacter(); //i
	selectLetterRight();//at j
	selectLetterRight();//at k
	selectLetterRight();//at l
	selectCharacter();//l
	selectCharacter();//l

	assert(!nameEmpty());

	setAbilityName(getNameFromInstance());
	addAbilityToIndividual(testPlayer, getNewAbility());
	changeAbilityTemplate(0);
	resetNameBoxInstance();

	//the name's been reset
	assert(nameEmpty());

	//player now has a duration ability
	assert(testPlayer->activeAbilities->numAbilities == 1);
	assert(testPlayer->abilities->numAbilities == 2);
	assert(strcmp(testPlayer->abilities->abilitiesList[1]->name,"Jill") == 0);

	//return to ability select
	for(i = 0; i < 19; i++){
		selectPreviousEffect();
	}
}

void createTargetedAbility(individual * testPlayer){
	setAbilityCreationSelectedType(ABILITY_TYPE);

	//go to targeted ability
	interpretRightAbilityCreation();

	//go down to range
	selectNextEffect();
	selectNextEffect();

	//range: 9
	setAbilityCreationSelectedType(ABILITY_RANGE);
	interpretRightAbilityCreation();
	interpretRightAbilityCreation();
	interpretRightAbilityCreation();

	//go down to dice damage
	selectNextEffect();
	selectNextEffect();

	//Dice Damage: d8
	setAbilityCreationSelectedType(ABILITY_DICE_DAMAGE);
	interpretRightAbilityCreation();
	interpretRightAbilityCreation();

	//go down to dice damage multiplier
	selectNextEffect();

	//multiplier: 2d8
	setAbilityCreationSelectedType(ABILITY_DICE_DAMAGE_MULTIPLIER);
	interpretRightAbilityCreation();

	//ability is +5 mana, able to create
	assert(canCreateAbility());

	//ability name is empty - need to fill out
	assert(nameEmpty());

	//Give ability name: A0
	selectCharacter(); //A
	selectLetterDown();//at N
	selectLetterDown();//at a
	selectLetterDown();//at n
	selectLetterDown();//at 0
	selectCharacter(); //0


	assert(!nameEmpty());

	setAbilityName(getNameFromInstance());
	addAbilityToIndividual(testPlayer, getNewAbility());
	changeAbilityTemplate(0);
	resetNameBoxInstance();

	//the name's been reset
	assert(nameEmpty());

	//player now has a duration ability
	assert(testPlayer->activeAbilities->numAbilities == 1);
	assert(testPlayer->abilities->numAbilities == 3);
	assert(strcmp(testPlayer->abilities->abilitiesList[2]->name,"A0") == 0);

	//go back to ability selection
	selectPreviousEffect();
	selectPreviousEffect();
	selectPreviousEffect();
	selectPreviousEffect();
	selectPreviousEffect();

}

void createInstanceAbility(individual * testPlayer){
	int i;

	//go to instant ability
	interpretRightAbilityCreation();

	//go down to extraAttack
	selectNextEffect();
	selectNextEffect();

	//+2 extraAttacks
	setAbilityCreationSelectedType(ABILITY_EXTRA_ATTACK);
	interpretRightAbilityCreation();
	interpretRightAbilityCreation();

	//go down to actions
	for(i = 0; i < 9; i++){
		selectNextEffect();
	}

	//-3 actions
	setAbilityCreationSelectedType(ABILITY_ACTIONS);
	interpretRightAbilityCreation();
	interpretRightAbilityCreation();
	interpretRightAbilityCreation();

	//go down to diceHP
	for(i = 0; i < 4; i++){
		selectNextEffect();
	}

	//d4 diceHP
	setAbilityCreationSelectedType(ABILITY_DICE_HP);
	interpretRightAbilityCreation();

	//ability is 0 mana, able to create
	assert(canCreateAbility());

	//ability name is empty - need to fill out
	assert(nameEmpty());

	//Give ability a classic name: AAA
	selectCharacter(); //A
	selectCharacter(); //A
	selectCharacter(); //A


	assert(!nameEmpty());

	setAbilityName(getNameFromInstance());
	addAbilityToIndividual(testPlayer, getNewAbility());
	changeAbilityTemplate(0);
	resetNameBoxInstance();

	//the name's been reset
	assert(nameEmpty());

	//player now has a duration ability
	assert(testPlayer->activeAbilities->numAbilities == 1);
	assert(testPlayer->abilities->numAbilities == 4);
	assert(strcmp(testPlayer->abilities->abilitiesList[3]->name,"AAA") == 0);

	//go back to ability selection
	for(i = 0; i < 15; i++){
		selectPreviousEffect();
	}

}

int test_main(individual * testPlayer, groupContainer * testGroupContainer, field* main_test_field, shiftData * testShiftData) {
//	printf("testing general\n");
//	test_general_all();
//	printf("testing character\n");
//	test_character_all();
//	mock_field_test();

	mainTest(testPlayer, testGroupContainer, main_test_field, testShiftData);

	return 0;
}
