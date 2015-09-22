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
#include"../src/headers/enemy_controller_pub_methods.h"
	char * mapTestDirectory = "C:\\Users\\Adrian\\C\\Natural_1_new_repo\\resources\\maps\\";// "C:\\Users\\Adrian\\C\\Natural_1_new_repo\\unit_tests\\testMaps\\";

individual* testPlayer;
enemies* thisTestEnemies;
field* main_test_field;
cursor* thisTestCursor;
int path_and_attack_test() {
	//setup

	BITMAP bm;
	UINT ret;

//	printf("gimmie one!\n");
//	getchar();

	testPlayer = initIndividual();
//
	thisTestEnemies = initEnemies();
//	printf("prod");
//	thisTestCursor = initCursor(2004,RGB(224, 64, 192),0,0);
//
//
//
	if (defineIndividual(testPlayer, 2001, RGB(255, 70, 255), "adr\0", 0, 1, 1, 20, 2, 13, 3, 10, 1, "MAX\0", 2, 4)) {
	}
//
//	int x, y;
	main_test_field = loadMap("map1.txt", mapTestDirectory, testPlayer, thisTestEnemies);
//	int imageID;
////	int imageID;
//
//	printf("enemies: %d\n",thisTestEnemies->numEnemies);
	assert(thisTestEnemies->numEnemies == 6);
////
//	for (y = 0; y < main_test_field->totalY; y++) {
//		for (x = 0; x < main_test_field->totalX; x++) {
//			imageID = (main_test_field->grid[x][y]->background)->imageID;
//			main_test_field->grid[x][y]->background->image = malloc(
//					sizeof(HBITMAP));
//			main_test_field->grid[x][y]->background->image = LoadBitmap(
//					GetModuleHandle(NULL), imageID);
//			if (main_test_field->grid[x][y]->background->image == NULL) {
//				printf("failed\n");
//			}
//
//		}
//	}
//
//	//player is on field at their coordinates, name check
//	individual * tmpIndividual = getIndividualFromField( main_test_field,testPlayer->playerCharacter->x, testPlayer->playerCharacter->y);
//
//	printf("name:%s\n",tmpIndividual->name);
//
//	assert(!strcmp(tmpIndividual->name,	testPlayer->name));

	//enemies are on field at their coordinates, name check
//	assert(!strcmp(getIndividualFromField(main_field,thisEnemies->enemies[0]->playerCharacter->x, thisEnemies->enemies[0]->playerCharacter->y)->name,
//			thisEnemies->enemies[0]->name));
//	assert(!strcmp(getIndividualFromField(main_field,thisEnemies->enemies[1]->playerCharacter->x, thisEnemies->enemies[1]->playerCharacter->y)->name,
//				thisEnemies->enemies[1]->name));
//	assert(!strcmp(getIndividualFromField(main_field,thisEnemies->enemies[2]->playerCharacter->x, thisEnemies->enemies[2]->playerCharacter->y)->name,
//				thisEnemies->enemies[2]->name));
//	assert(!strcmp(getIndividualFromField(main_field,thisEnemies->enemies[3]->playerCharacter->x, thisEnemies->enemies[3]->playerCharacter->y)->name,
//				thisEnemies->enemies[3]->name));
//	assert(!strcmp(getIndividualFromField(main_field,thisEnemies->enemies[4]->playerCharacter->x, thisEnemies->enemies[4]->playerCharacter->y)->name,
//				thisEnemies->enemies[4]->name));
//	assert(!strcmp(getIndividualFromField(main_field,thisEnemies->enemies[5]->playerCharacter->x, thisEnemies->enemies[5]->playerCharacter->y)->name,
//				thisEnemies->enemies[5]->name));

//	nodeArr * resultArr = getSpaceClosestToPlayer(main_field, skeleton, player);
//
//	assert(resultArr->nodeArray[11]->x == 0 && resultArr->nodeArray[11]->y == 0);
//	assert(resultArr->nodeArray[10]->x == 1 && resultArr->nodeArray[10]->y == 0);
//	assert(resultArr->nodeArray[9]->x == 2 && resultArr->nodeArray[9]->y == 0);
//	assert(resultArr->nodeArray[8]->x == 3 && resultArr->nodeArray[8]->y == 0);
//	assert(resultArr->nodeArray[7]->x == 4 && resultArr->nodeArray[7]->y == 0);
//	assert(resultArr->nodeArray[6]->x == 5 && resultArr->nodeArray[6]->y == 0);
//	assert(resultArr->nodeArray[5]->x == 6 && resultArr->nodeArray[5]->y == 0);
//	assert(resultArr->nodeArray[4]->x == 7 && resultArr->nodeArray[4]->y == 1);
//	assert(resultArr->nodeArray[3]->x == 8 && resultArr->nodeArray[3]->y == 2);
//	assert(resultArr->nodeArray[2]->x == 9 && resultArr->nodeArray[2]->y == 3);
//	assert(resultArr->nodeArray[1]->x == 10 && resultArr->nodeArray[1]->y == 2);
//	assert(resultArr->nodeArray[0]->x == 10 && resultArr->nodeArray[0]->y == 1);
//
//	int i;
//	int size = min(resultArr->size, skeleton->mvmt);
//
//	moveNode * rootMoveNode = malloc(sizeof(rootMoveNode));
//	moveNode ** tmpMoveNode = &rootMoveNode;
//
//	for(i = 0; i < size; i++){
//
//		printf("moving to: [%d,%d]\n",resultArr->nodeArray[i]->x,resultArr->nodeArray[i]->y);
//		if(setIndividualSpace(main_field,skeleton, resultArr->nodeArray[i]->x, resultArr->nodeArray[i]->y)==0){
//
//			break; // path is blocked by individual
//		}
//	}
//
//	//skeleton should be at space [9,3]
//	assert(getIndividualFromField(main_field,9,3) == skeleton);
//
//	//too far away to attack
//	assert(!attackIfInRange(skeleton, player));
//
//	//warp player next to skeleton
//	setIndividualSpace(main_field,player,8,3);
//
//	//is within range - attack
//	assert(attackIfInRange(player, skeleton));
//
//	//was attacked for 9 (1/10 hp)
//	printf("skelly health:%d\n", skeleton->hp);
//	assert(skeleton->hp  == 1);
//
//	//skeleton will be killed (0/10 hp)
//	assert(attackIndividual(player, skeleton));

	free(testPlayer);
	free(thisTestCursor);
	free(thisTestEnemies);
	free(main_test_field);
	return 0;
}


int test_main() {
//	printf("testing general\n");
//	test_general_all();
//	printf("testing character\n");
//	test_character_all();
//	mock_field_test();

//	path_and_attack_test();

	return 0;
}
