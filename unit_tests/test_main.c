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
#include"./test_headers/test_character.h"
#include"../src/headers/field_pub_methods.h"
#include"../src/headers/enemy_controller_pub_methods.h"

int mock_field_test() {
	individual * player = malloc(sizeof(individual));
	player->playerCharacter = malloc(sizeof(character));
	player->playerCharacter->x = 0;
	player->playerCharacter->y = 0;

	field *thisField =
			initField(
					"C:\\Users\\Adrian\\workspace\\Natural_1\\unit_tests\\testmap1.txt");
	space **impassableSpace = getSpaceFromField(thisField, 2, 0);
	printf("field width:%d, field height:%d\n", thisField->totalX,
			thisField->totalY);
	assert(!(*impassableSpace)->isPassable);

	assert(player->playerCharacter->x == 0);
	assert(player->playerCharacter->y == 0);
	moveIndividual(thisField, player, 3); //moving to an empty space (SE)
	assert(player->playerCharacter->x == 1);
	assert(player->playerCharacter->y == 1);
	moveIndividual(thisField, player, 3); //try to move to an impassable space (SE)
	assert(player->playerCharacter->x == 1);
	assert(player->playerCharacter->y == 1);
	moveIndividual(thisField, player, 2); //move to an empty space (S)
	assert(player->playerCharacter->x == 1);
	assert(player->playerCharacter->y == 2);

	//testing Adjacent Spaces
	space** tmpAdjacentSpaces = getAdjacentSpaces(thisField, 1, 1);
	int numSpaces = 0;
	while (tmpAdjacentSpaces[numSpaces] != '\0') {
		numSpaces++;
	}
	assert(numSpaces == 9);

	tmpAdjacentSpaces = getAdjacentSpaces(thisField, 0, 1);
	numSpaces = 0;
	while (tmpAdjacentSpaces[numSpaces] != '\0') {
		numSpaces++;
	}
	assert(numSpaces == 6);

	tmpAdjacentSpaces = getAdjacentSpaces(thisField, 0, 0);
	numSpaces = 0;
	while (tmpAdjacentSpaces[numSpaces] != '\0') {
		numSpaces++;
	}
	assert(numSpaces == 4);


	/*
	 * .----.----.
	 * |[0] |[2] |
	 * |----|----|
	 * |[1] |[3] |
	 * '----'----'
	 */
	assert(tmpAdjacentSpaces[0] == getSpaceFromField(thisField, 0,0));
	assert(tmpAdjacentSpaces[1] == getSpaceFromField(thisField, 0,1));
	assert(tmpAdjacentSpaces[2] == getSpaceFromField(thisField, 1,0));
	assert(tmpAdjacentSpaces[3] == getSpaceFromField(thisField, 1,1));

	return 0;
}

int path_and_attack_test() {
	//setup

	BITMAP bm;
	UINT ret;

	individual * player = initIndividual();
	individual * skeleton = initIndividual();

	assert(!defineIndividual(player, 2001, RGB(255, 70, 255), "adr", 0, 0, 10, 2, 10, 0, 2, 3));
	assert(!defineIndividual(skeleton, 2005, RGB(255, 0, 255), "skelly", 10, 1, 10, 2, 3, 1, 1, 3));

	int x;
	int y;
	field * main_field = initField( "C:\\Users\\Adrian\\workspace\\Natural_1\\src\\testmap1.txt");
	int imageID;

	for (y = 0; y < main_field->totalY; y++) {
		for (x = 0; x < main_field->totalX; x++) {
			imageID = (main_field->grid[x][y]->background)->imageID;
			main_field->grid[x][y]->background->image = malloc(sizeof(HBITMAP));
			main_field->grid[x][y]->background->image = LoadBitmap(
			GetModuleHandle(NULL), imageID);
			if (main_field->grid[x][y]->background->image == NULL) {
				printf("failed\n");
			}

		}
	}

	assert(setIndividualSpace(main_field, player, 0, 0));
	assert(setIndividualSpace(main_field, skeleton, 10, 0));

	nodeArr * resultArr = getSpaceClosestToPlayer(main_field, skeleton, player);

	assert(resultArr->nodeArray[11]->x == 0 && resultArr->nodeArray[11]->y == 0);
	assert(resultArr->nodeArray[10]->x == 1 && resultArr->nodeArray[10]->y == 0);
	assert(resultArr->nodeArray[9]->x == 2 && resultArr->nodeArray[9]->y == 0);
	assert(resultArr->nodeArray[8]->x == 3 && resultArr->nodeArray[8]->y == 0);
	assert(resultArr->nodeArray[7]->x == 4 && resultArr->nodeArray[7]->y == 0);
	assert(resultArr->nodeArray[6]->x == 5 && resultArr->nodeArray[6]->y == 0);
	assert(resultArr->nodeArray[5]->x == 6 && resultArr->nodeArray[5]->y == 0);
	assert(resultArr->nodeArray[4]->x == 7 && resultArr->nodeArray[4]->y == 1);
	assert(resultArr->nodeArray[3]->x == 8 && resultArr->nodeArray[3]->y == 2);
	assert(resultArr->nodeArray[2]->x == 9 && resultArr->nodeArray[2]->y == 3);
	assert(resultArr->nodeArray[1]->x == 10 && resultArr->nodeArray[1]->y == 2);
	assert(resultArr->nodeArray[0]->x == 10 && resultArr->nodeArray[0]->y == 1);

	int i;
	int size = min(resultArr->size, skeleton->mvmt);

	moveNode * rootMoveNode = malloc(sizeof(rootMoveNode));
	moveNode ** tmpMoveNode = &rootMoveNode;

	for(i = 0; i < size; i++){

		printf("moving to: [%d,%d]\n",resultArr->nodeArray[i]->x,resultArr->nodeArray[i]->y);
		if(setIndividualSpace(main_field,skeleton, resultArr->nodeArray[i]->x, resultArr->nodeArray[i]->y)==0){

			break; // path is blocked by individual
		}
	}

	//skeleton should be at space [9,3]
	assert(getIndividualFromField(main_field,9,3) == skeleton);

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
