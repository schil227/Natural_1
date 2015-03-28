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

int d_main(){
	printf("testing general\n");
	test_general_all();
	printf("testing character\n");
	test_character_all();
	return 0;
}
