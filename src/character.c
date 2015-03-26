/*
 * character.c
 *
 *  Created on: Mar 25, 2015
 *      Author: Adrian
 */
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"./headers/character.h"

int nameCharacter(character* thisCharacter, char* name){
	strcpy(thisCharacter->name = name, name);
}
