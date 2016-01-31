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

char * mapTestDirectory = "C:\\Users\\Adrian\\C\\Natural_1_new_repo\\unit_tests\\testMaps\\";//".\\unit_tests\\testMaps\\";//

individual* testPlayer;
individualGroup* testEnemies;
individualGroup* testNPCs;
field* main_test_field;
cursor* thisTestCursor;
shiftData* testShiftData;

int path_and_attack_test() {
	//setup

	BITMAP bm;
	UINT ret;

	testPlayer = initIndividual();
	testEnemies = initGroup();
	testNPCs = initGroup();
	testShiftData = initShiftData();
	initThisConsole(2010,0,0,300,200);
	initThisDialogBox(2012,10,10,RGB(255, 70, 255));
	initalizeTheGlobalRegister();
	initEventHandlers();
	loadTriggerMaps(mapTestDirectory, "test_onAttackTriggerMap.txt","test_onHarmTriggerMap.txt","test_onDeathTriggerMap.txt");
	loadIndividualsToRegistry(mapTestDirectory,"test_individuals.txt");
	loadItemsToRegistry(mapTestDirectory, "test_items.txt");
	loadEventsToRegistry(mapTestDirectory, "test_events.txt");

	if (defineIndividual(testPlayer, 2001, 0, RGB(255, 70, 255), "adr\0", 0, 1, 1, 20, 2, 4, 13, 3, 10, 1, "MAX\0", 2, 4,0,0,0,0,0,0,0,0,0,0,0,0,0)) {
	}

	int x, y;
	main_test_field = loadMap("test_map1.txt", mapTestDirectory, testPlayer, testEnemies, testNPCs);
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

	int i;
	for(i = 0; i < testEnemies->numIndividuals; i++){
		enemyAction((testEnemies->individuals[i]), main_test_field, testPlayer);
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
	assert(testPlayer->hp ==  12);

	//consume herb as active duration item
	modifyItem(testPlayer->backpack->inventoryArr[0], testPlayer);

	//herb heals the player for two
	assert(testPlayer->hp ==  14);

	startTurn(testPlayer);
	assert(testPlayer->hp ==  16);
	startTurn(testPlayer);
	assert(testPlayer->hp ==  18);
	startTurn(testPlayer);
	assert(testPlayer->hp ==  20);
	startTurn(testPlayer);
	assert(testPlayer->hp ==  20);

	//warp player back to original spot
	moveIndividualSpace(main_test_field,testPlayer,1,1);

	//move the enemies two more times, attack the player
	for(i = 0; i < testEnemies->numIndividuals; i++){
		enemyAction((testEnemies->individuals[i]), main_test_field, testPlayer);
	}
	for(i = 0; i < testEnemies->numIndividuals; i++){
		enemyAction((testEnemies->individuals[i]), main_test_field, testPlayer);
	}

	//while skeletons 2, 3, and 4 all moved without being blocked, 5 reacts to the blocked path
	//and moves to [3,1]
	tmpIndividual = testEnemies->individuals[4];
	assert(tmpIndividual->playerCharacter->x == 3 && tmpIndividual->playerCharacter->y == 1 );

	//after being attacked 3 times, player health is 14
	assert(testPlayer->hp == 14);

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

	//event attached to dialog, npc becomes hostile
	assert(getEventFromCurrentMessage());

	individual * tmpNPC = getIndividualFromRegistry(testNPCs->individuals[0]->ID);

	//process the becomeEnemy() function, npc is removed from testNPCs and is added to enemies
	processEvent(getEventFromCurrentMessage(), testPlayer, testNPCs, testEnemies, main_test_field);

	//npc not in NPCs
	assert(!individualInGroup(tmpNPC, testNPCs));

	//npc is in testEnemies
	assert(individualInGroup(tmpNPC, testEnemies));

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


int test_main() {
//	printf("testing general\n");
//	test_general_all();
//	printf("testing character\n");
//	test_character_all();
//	mock_field_test();

	path_and_attack_test();

	return 0;
}
