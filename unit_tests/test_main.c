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
#include"./test_headers/test_general.h"
//#include"./test_headers/test_character.h"
#include"../src/headers/individual_pub_methods.h"
#include"../src/headers/field_pub_methods.h"
#include"../src/headers/console_pub_methods.h"
#include"../src/headers/enemy_controller_pub_methods.h"
#include"../src/headers/dialog_pub_methods.h"

static char * mapTestDirectory = "C:\\Users\\Adrian\\C\\Natural_1_new_repo\\unit_tests\\testMaps\\";//".\\unit_tests\\testMaps\\";//

//individual* testPlayer;
//individualGroup* testEnemies;
//individualGroup* testNPCs;
//field* main_test_field;
//shiftData* testShiftData;

int mainTest(individual* testPlayer, individualGroup* testEnemies, individualGroup* testNPCs, field* main_test_field, shiftData * testShiftData) {
	//setup

	BITMAP bm;
	UINT ret;

//	testPlayer = initIndividual();
//	testEnemies = initGroup();
//	testNPCs = initGroup();
//	testShiftData = initShiftData();
//	initThisCursor(2004,RGB(224, 64, 192),0,0);
//	initThisConsole(2010,0,0,300,200);
//	initThisDialogBox(2012,10,10,RGB(255, 70, 255));
//	initalizeTheGlobalRegister();
//	initEventHandlers();
//	initAbilityCreationInstance(3500,RGB(255, 0, 255), 10, 10, mapTestDirectory, "test_effects_template.txt");
//	initThisAbilityView(3504, RGB(255, 0, 255), 10, 10);
//	initNameBoxInstance(3503, RGB(255,0,255), 20, 20);
//	loadTriggerMaps(mapTestDirectory, "test_onAttackTriggerMap.txt","test_onHarmTriggerMap.txt","test_onDeathTriggerMap.txt");
//	loadIndividualsToRegistry(mapTestDirectory,"test_individuals.txt");
//	loadItemsToRegistry(mapTestDirectory, "test_items.txt");
//	loadEventsToRegistry(mapTestDirectory, "test_events.txt");

//	if (defineIndividual(testPlayer, 2001, 0, RGB(255, 70, 255), "adr\0", 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 20, 2, 4, 13, 3, 10, 1, 1, "MAX\0", 2, 4,0,0,0,0,0,0,0,0,0,0,0,0,0,50)) {
//		}

	int x, y;
//	main_test_field = loadMap("test_map1.txt", mapTestDirectory, testPlayer, testEnemies, testNPCs);
	int imageID;


	assert(testEnemies->numIndividuals == 6);

	assert(testNPCs->numIndividuals == 1);

	for (y = 0; y < main_test_field->totalY; y++) {
		for (x = 0; x < main_test_field->totalX; x++) {
			imageID = (main_test_field->grid[x][y]->background)->imageID;
			main_test_field->grid[x][y]->background->image = malloc(
					sizeof(HBITMAP));
			main_test_field->grid[x][y]->background->image = LoadBitmap(
					GetModuleHandle(NULL), imageID);
			if (main_test_field->grid[x][y]->background->image == NULL) {
				printf("failed\n");
			}

		}
	}

//player is on field at their coordinates, name check
	individual * tmpIndividual = getIndividualFromField( main_test_field,testPlayer->playerCharacter->x, testPlayer->playerCharacter->y);
	assert(!strcmp(tmpIndividual->name,	testPlayer->name));

	//enemies are on field at their coordinates, name check
	tmpIndividual = getIndividualFromField( main_test_field,testEnemies->individuals[0]->playerCharacter->x, testEnemies->individuals[0]->playerCharacter->y);
	assert(!strcmp(tmpIndividual->name, testEnemies->individuals[0]->name));

	tmpIndividual = getIndividualFromField( main_test_field,testEnemies->individuals[1]->playerCharacter->x, testEnemies->individuals[1]->playerCharacter->y);
	assert(!strcmp(tmpIndividual->name, testEnemies->individuals[1]->name));

	tmpIndividual = getIndividualFromField( main_test_field,testEnemies->individuals[2]->playerCharacter->x, testEnemies->individuals[2]->playerCharacter->y);
	assert(!strcmp(tmpIndividual->name, testEnemies->individuals[2]->name));

	tmpIndividual = getIndividualFromField( main_test_field,testEnemies->individuals[3]->playerCharacter->x, testEnemies->individuals[3]->playerCharacter->y);
	assert(!strcmp(tmpIndividual->name, testEnemies->individuals[3]->name));

	tmpIndividual = getIndividualFromField( main_test_field,testEnemies->individuals[4]->playerCharacter->x, testEnemies->individuals[4]->playerCharacter->y);
	assert(!strcmp(tmpIndividual->name, testEnemies->individuals[4]->name));

	tmpIndividual = getIndividualFromField( main_test_field,testEnemies->individuals[5]->playerCharacter->x, testEnemies->individuals[5]->playerCharacter->y);
	assert(!strcmp(tmpIndividual->name, testEnemies->individuals[5]->name));

	int i, individualsPassed = 0;
	for(i = 0; i < testEnemies->MAX_INDIVIDUALS; i++){

		if(testEnemies->individuals[i] != NULL){
			individualsPassed++;
			enemyAction((testEnemies->individuals[i]), main_test_field, testPlayer);

			if(individualsPassed == testEnemies->numIndividuals){
				break;
			}
		}

	}

	//skeleton 1&5 too far away, same starting position
	tmpIndividual =testEnemies->individuals[0];
	assert(tmpIndividual->playerCharacter->x == 10 &&
			tmpIndividual->playerCharacter->y == 0 );
	tmpIndividual =testEnemies->individuals[5];
	assert(tmpIndividual->playerCharacter->x == 9 &&
			tmpIndividual->playerCharacter->y == 15 );

//	//skeletons 2,3,4,5 move towards the player
	tmpIndividual =testEnemies->individuals[1];
	assert(tmpIndividual->playerCharacter->x == 8 &&
			tmpIndividual->playerCharacter->y == 2 );
	tmpIndividual =testEnemies->individuals[2];
	assert(tmpIndividual->playerCharacter->x == 7 &&
			tmpIndividual->playerCharacter->y == 1 );
	tmpIndividual =testEnemies->individuals[3];
	assert(tmpIndividual->playerCharacter->x == 8 &&
			tmpIndividual->playerCharacter->y == 1 );
	tmpIndividual =testEnemies->individuals[4];
	assert(tmpIndividual->playerCharacter->x == 6 &&
			tmpIndividual->playerCharacter->y == 0 );


	//get full node path of closest skeleton to player
	nodeArr * resultArr = getFullNodePath(main_test_field, testPlayer->playerCharacter->x, testPlayer->playerCharacter->y,
			testEnemies->individuals[4]->playerCharacter->x, testEnemies->individuals[4]->playerCharacter->y);

	assert(resultArr->nodeArray[0]->x == 2 && resultArr->nodeArray[0]->y == 0);
	assert(resultArr->nodeArray[1]->x == 3 && resultArr->nodeArray[1]->y == 0);
	assert(resultArr->nodeArray[2]->x == 4 && resultArr->nodeArray[2]->y == 0);
	assert(resultArr->nodeArray[3]->x == 5 && resultArr->nodeArray[3]->y == 0);
	assert(resultArr->nodeArray[4]->x == 6 && resultArr->nodeArray[4]->y == 0);

	//get the node path to the first open space within the movement range
	resultArr = getSpaceClosestToPlayer(main_test_field, testEnemies->individuals[4], testPlayer);

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

	//consume herb as active duration item
	modifyItem(testPlayer->backpack->inventoryArr[0], testPlayer);

	//herb heals the player for two
	assert(testPlayer->hp ==  16);

	startTurn(testPlayer);
	assert(testPlayer->hp ==  18);
	startTurn(testPlayer);
	assert(testPlayer->hp ==  20);
	startTurn(testPlayer);
	assert(testPlayer->hp ==  22);
	startTurn(testPlayer);
	assert(testPlayer->hp ==  22);

	//warp player back to original spot
	moveIndividualSpace(main_test_field,testPlayer,1,1);

	//move the enemies two more times, attack the player
	individualsPassed = 0;
	for(i = 0; i < testEnemies->MAX_INDIVIDUALS; i++){

		if(testEnemies->individuals[i] != NULL){
			individualsPassed++;
			enemyAction((testEnemies->individuals[i]), main_test_field, testPlayer);

			if(individualsPassed == testEnemies->numIndividuals){
				break;
			}
		}

	}

	individualsPassed = 0;
	for(i = 0; i < testEnemies->MAX_INDIVIDUALS; i++){

		if(testEnemies->individuals[i] != NULL){
			individualsPassed++;
			enemyAction((testEnemies->individuals[i]), main_test_field, testPlayer);

			if(individualsPassed == testEnemies->numIndividuals){
				break;
			}
		}

	}

	//while skeletons 2, 3, and 4 all moved without being blocked, 5 reacts to the blocked path
	//and moves to [3,1]
	tmpIndividual = testEnemies->individuals[4];
	assert(tmpIndividual->playerCharacter->x == 3 && tmpIndividual->playerCharacter->y == 1 );

	//after being attacked 3 times, player health is 15
	assert(testPlayer->hp == 17);

	//skeleton0 cannot attack the player, vis versa
	assert(!individualWithinRange(testEnemies->individuals[0],testPlayer));
	assert(!individualWithinRange(testPlayer,testEnemies->individuals[0]));

	//skeleton5 cannot reach the player, but the player can attack
	assert(!individualWithinRange(testEnemies->individuals[4],testPlayer));
	assert(individualWithinRange(testPlayer,testEnemies->individuals[4]));

	//player attacks skeleton5, skeleton5 dies
	assert(attackIndividual(testPlayer,testEnemies->individuals[4]));

	//warping player next to doorway
	moveIndividualSpace(main_test_field,testPlayer,6,9);

	//fails, not a transitional space
	assert(!attemptToTransit(&main_test_field, testPlayer, testEnemies, testNPCs, testShiftData, mapTestDirectory));

	//warping player to doorway
	moveIndividualSpace(main_test_field,testPlayer,6,8);

	//works, player is in a transitional space - new field is loaded
	assert(attemptToTransit(&main_test_field, testPlayer, testEnemies, testNPCs, testShiftData,mapTestDirectory));

	printf("Player location %d,%d\n", testPlayer->playerCharacter->x,testPlayer->playerCharacter->y);

	//player was transported to new map, new position is [2,3]
	assert(testPlayer->playerCharacter->x == 2 && testPlayer->playerCharacter->y == 3);

	//transport back
	assert(attemptToTransit(&main_test_field, testPlayer, testEnemies, testNPCs, testShiftData,mapTestDirectory));

	//player is back at map1 doorway
	assert(testPlayer->playerCharacter->x == 6 && testPlayer->playerCharacter->y == 8);

	//try talking to npc, too far away
	assert(!tryTalk(testNPCs, testPlayer,0,2));

	//warp player closer to npc
	moveIndividualSpace(main_test_field,testPlayer,1,1);

	//talking will be successful now:
	assert(tryTalk(testNPCs, testPlayer,0,2));

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

	individual * tmpNPC = getIndividualFromRegistry(testNPCs->individuals[0]->ID);

	//process the CHR Check, successful
	processEvent(getEventFromCurrentMessage(), testPlayer, testNPCs, testEnemies, main_test_field);

	createPermanentAbiltyTest(testPlayer);
	createSelfDurationAbility(testPlayer);
	createTargetedAbility(testPlayer);

	/*use self duraiton ability,
	 * poison testPlayer and npc,
	 *  -2 AC/-1 attack,
	 *   npc becomes enemy
	 */

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
	assert(testPlayer->hp == 17);

	//use duration ability on everyone within range
	useAbilityOnIndividualsInAOERange(testPlayer, NULL, testPlayer, testNPCs, testEnemies, main_test_field, testPlayer->playerCharacter->x, testPlayer->playerCharacter->y);

	//clear selectedAbility
	testPlayer->activeAbilities->selectedAbility = NULL;

	//since duration ability was harmful, npc is now an enemy
	assert(!individualInGroup(tmpNPC, testNPCs));
	assert(individualInGroup(tmpNPC, testEnemies));

	//After spell cast hp check
	assert(testPlayer->hp == 14);

	//player and the NPC are poisoned
	assert(testPlayer->activeStatuses->statuses[0]->effect == STATUS_POISONED);
	assert(tmpNPC->activeStatuses->statuses[0]->effect == STATUS_POISONED);

	//status magnitude
	assert(testPlayer->activeStatuses->statuses[0]->turnsRemaining == 2);
	assert(testPlayer->activeStatuses->statuses[0]->damage == 1);
	assert(testPlayer->activeStatuses->statuses[0]->diceDamage == 4);

	//after new turn, poison has fewer turns, damages individual
	startTurn(testPlayer);
	assert(testPlayer->activeStatuses->statuses[0]->turnsRemaining == 1);
	assert(testPlayer->hp == 11);

	/*
	 * use targeted ability on enemy
	 */
	refreshAbilityView(testPlayer->abilities->numAbilities, testPlayer->abilities->abilitiesList);

	//go to targeted ability from view
	selectNextAbility();//duration
	selectNextAbility();//targeted

	//Player doesn't have enough mana!
	assert(!canUseAbility(testPlayer, getAbilityToActivate()));

	//give the player some mana
	restoreMana(testPlayer, 10);

	//now the ability can be used
	assert(canUseAbility(testPlayer, getAbilityToActivate()));

	//player didn't die by using this ability (impossible, it's not an instant ability), abiltiy is set to selectedAbility
	assert(!useAbility(testPlayer, getAbilityToActivate()));

	//ability is NOT within range of selected enemy (skelly)
	assert(!cursorWithinAbilityRange(testPlayer, 0, 10));

	//ability is in range of closer enemy
	assert(cursorWithinAbilityRange(testPlayer, 2, 1));

	//get skelly4, the targeted enemy
	tmpIndividual = testEnemies->individuals[3];

	//skelly4 is in good health
	assert(tmpIndividual->hp == 2);

	//use ability on selected space,  harming skelly4
	useAbilityOnIndividualsInAOERange(testPlayer, NULL, testPlayer, testNPCs, testEnemies, main_test_field, 2, 1);

	//skelly4 was killed by two high-rolled d8s
	assert(tmpIndividual->hp == -13);

	//break down mock up
	destroyIndividual(testPlayer);
	clearGroup(testEnemies);
	clearGroup(testNPCs);
	destroyField(main_test_field, NULL);
	destroyThisDialogBox();
	destroyConsoleInstance();
	destroyTheGlobalRegister();
	destroyEventHandlers();
	return 0;
}

void createPermanentAbiltyTest(individual * testPlayer){
	//try create ability, fail (mana = -2)
	assert(!canCreateAbility());

	//down to STR
	selectNextEffect();

	//increase STR (+1)
	setAbilityCreationSelectedType(ABILITY_STR);
	interpretRightAbilityCreation();

	//try create ability, fail (mana = -1)
	assert(!canCreateAbility());

	//increase STR (+2)
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

	//player now has a perminant ability granting +2 strength
	assert(testPlayer->activeAbilities->numAbilities == 1);
	assert(testPlayer->abilities->numAbilities == 1);
	assert(strcmp(testPlayer->abilities->abilitiesList[0]->name,"ABCBA") == 0);
	assert(getAttributeSum(testPlayer, "STR") == 3);

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
	setAbilityCreationSelectedType(ABILITY_AOE);
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
	//go to targeted ability
	interpretRightAbilityCreation();

	//go down to range
	selectNextEffect();
	selectNextEffect();

	//range: 3
	setAbilityCreationSelectedType(ABILITY_RANGE);
	interpretRightAbilityCreation();

	//go down to dice damage
	selectNextEffect();
	selectNextEffect();

	//Dice Damage: d8
	setAbilityCreationSelectedType(ABILITY_DICE_DAMAGE);
	interpretRightAbilityCreation();
	interpretRightAbilityCreation();

	//go down to dice damage multiplier
	interpretRightAbilityCreation();

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
}

void createInstanceAbility(){

}

int test_main(individual * testPlayer, individualGroup* testEnemies, individualGroup* testNPCs, field* main_test_field, shiftData * testShiftData) {
//	printf("testing general\n");
//	test_general_all();
//	printf("testing character\n");
//	test_character_all();
//	mock_field_test();

	mainTest(testPlayer, testEnemies, testNPCs, main_test_field, testShiftData);

	return 0;
}
