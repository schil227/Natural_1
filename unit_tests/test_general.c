/*
 * test_general.c
 *
 *  Created on: Mar 20, 2015
 *      Author: Adrian
 */

//
#include<assert.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
//#include "./test_headers/all_general.h"

void test_general_all(){
	assert(1==1);
	assert(1!=2);
	assert(100==100);

	char*a = "a";
	char*b = "b";
	char*ab = "ab";

	char*joined = join(a,b);

	int fac_1 = 0;
	int fac_2 = 1;
	int fac_3 = 10;


	assert(factorial(fac_1,1)==1);
	assert(factorial(fac_2,0)==1);
	assert(factorial(fac_3,0)==55);

	assert(strcmp(ab,joined)==0);
	assert(strcmp(join(b,a), "ba")==0);

	printf("done testing general\n");
}
