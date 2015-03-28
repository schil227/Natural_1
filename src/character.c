/*
 * character.c
 *
 *  Created on: Mar 25, 2015
 *      Author: Adrian
 */

#include<string.h>
#include"./headers/character_pub_methods.h"

void nameCharacter(character* thisCharacter, char* name){
	thisCharacter->name = malloc(sizeof(name));
	strcpy(thisCharacter->name, name);
	thisCharacter->nameLength = strlen(name);
}

void destroyCharacter(character* thisCharacter){
	if(thisCharacter->nameLength != 0){
		free(thisCharacter->name);
	}

	free(thisCharacter);
}
