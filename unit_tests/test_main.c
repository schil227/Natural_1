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

int mock_field_test(){
	individual * player = malloc(sizeof(individual));
	player->playerCharacter = malloc(sizeof(character));
	player->playerCharacter->x = 0;
	player->playerCharacter->y = 0;

	field *thisField = initField("C:\\Users\\Adrian\\workspace\\Natural_1\\unit_tests\\testmap1.txt");
	space **impassableSpace = getSpaceFromField(thisField,2,0);
	printf("field width:%d, field height:%d\n",thisField->totalX,thisField->totalY);
	assert(!(*impassableSpace)->isPassable);

	assert(player->playerCharacter->x == 0);
	assert(player->playerCharacter->y == 0);
	moveIndividual(thisField, player,3); //moving to an empty space (SE)
	assert(player->playerCharacter->x == 1);
	assert(player->playerCharacter->y == 1);
	moveIndividual(thisField, player,3); //try to move to an impassable space
	assert(player->playerCharacter->x == 1);
	assert(player->playerCharacter->y == 1);
	moveIndividual(thisField, player,2); //move to an empty space (S)
	assert(player->playerCharacter->x == 1);
	assert(player->playerCharacter->y == 2);
	return 0;
}

int main(){
//	printf("testing general\n");
//	test_general_all();
//	printf("testing character\n");
//	test_character_all();
	mock_field_test();

	return 0;
}
