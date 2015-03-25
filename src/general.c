/*
 * general.c
 *
 *  Created on: Feb 13, 2015
 *      Author: Adrian
 */

#include<Math.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "./headers/general.h"

//make a arr struct to contain the array, type, and size

int factorial(int n, int sum){
	if(n == 0){
		return sum;
	}else{
		sum += n;
		n -=1;
		return factorial(n, sum);
	}

}

int pyramidSize(int size){
	if(size <= 1){
		return size;
	}
	return( factorial((size*2),0) - factorial(size,0));
}

char * getPyramidString(int size){
	charArrStruct *my_pyramid = bigTriangle(size);
	return my_pyramid->charArr;
}

charArrStruct *bigTriangle(int size){

	int arrSize = pyramidSize(size);
	charArrStruct *pyramid = malloc(sizeof(charArrStruct));

	pyramid->size = sizeof(charArrStruct);
	pyramid->arrSize = arrSize;
	pyramid->charArr = malloc(sizeof(char)*arrSize);

	int arrIndex = 0;
	//char pyramidArr[arrSize] = malloc(sizeof(char)*arrSize);

	int i;
	for(i = 0; i < size; i++){
		int j;
		int foo = size - i;
		for(j = 0; j < (2*size) - foo; j++){
			if(j < foo-1){
				pyramid->charArr[arrIndex] = '~';
			}else{
				pyramid->charArr[arrIndex] = '#';
			}
			arrIndex++;
		}
		pyramid->charArr[arrIndex] = '\n';
		arrIndex++;

	}
	pyramid->charArr[arrIndex] = '\0';

//	printf("size of array:%d\nCalc'd size:%d\n", (sizeof(pyramidArr)/sizeof(char)), arrSize);

	//pyramid->charArr = pyramidArr;

//	printf("pointer1:%p\n",((void*)&pyramid->charArr[0]));
//	printf("value1:%c\n",(pyramid->charArr[1]));
//	//print pyramid
//		int p;
//		for(p = 0; p < pyramid->arrSize; p++){
//			printf("%c",(pyramid->charArr[p]));
//		}

	return pyramid;

}

int bmain(void){
	int size = 4;
	charArrStruct *my_pyramid = bigTriangle(size);
	//printf("size:%d\n",(my_pyramid->arrSize));

	//char* arr = my_pyramid->charArr;

	printf("\nafter method\n\n");

	if (my_pyramid != NULL) {
		printf("pointer2:%p\n", ((void*) &my_pyramid->charArr[0]));
		printf("value2:%c\n", (my_pyramid->charArr[1]));

		int i;
		for (i = 0; i < my_pyramid->arrSize; i++) {
			printf("%c", my_pyramid->charArr[i]);
		}

		free(my_pyramid->charArr);
		free(my_pyramid);
	}
	return 0;
}

char * returnArr(void){
	char * stuff = malloc(4);
	if (stuff != NULL) {
		stuff[0] = 'a';
		stuff[1] = 'b';
		stuff[2] = 'c';
		stuff[3] = 'd';
	}
	printf("location of stuff:%p\n",stuff);
	int i;
	for(i = 0; i < 4; i++){
		printf("%c\n",stuff[i]);
	}

	return stuff;
}

char *join(char *str1, char *str2){
	char * resultStr = malloc(strlen(str1) + strlen(str2) + 1);

	if(resultStr){
		strcpy(resultStr,str1);
		strcat(resultStr,str2);
	}

	return resultStr;
}

//int babmain(void){
//	char* myarr;
//	myarr = returnArr();
//	int i;
//	printf("\n");
//	printf("location of myarr:%p\n", myarr);
//	for(i = 0; i < 4; i++){
//		printf("%c\n",myarr[i]);
//	}
//	free(myarr);
//}
