/*
 * test_character.c
 *
 *  //it is important
 *  //to test your own character
 *  //have integrity
 *  ~Haiku
 *
 *  Created on: Mar 26, 2015
 *      Author: Adrian
 */

#include "./test_headers/all_character.h"
#include<assert.h>
#include<string.h>

void test_character_all(){
//	printf("asdf");
	character* my_character = malloc(sizeof(character));
	printf("asdf\n");
	nameCharacter(my_character, "Adrian");

	assert(strcmp(my_character->name,"Adrian")==0);
	printf("set name: %s, len: %d\n", my_character->name, my_character->nameLength);
	destroyCharacter(my_character);
}
