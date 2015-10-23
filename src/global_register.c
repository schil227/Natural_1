/*
 * global_register.c
 *
 *  Created on: Oct 22, 2015
 *      Author: Adrian
 */
#include<math.h>
#include"./headers/structs/global_register.h"

globalRegister * thisGlobalRegister;

void initalizeTheGlobalRegister(){
	int i;
	unsigned int maxInt;

	thisGlobalRegister = malloc(sizeof(globalRegister));
	thisGlobalRegister->sizeOfInt = sizeof(int) * 8;

	maxInt = pow(2,thisGlobalRegister->sizeOfInt) -1;

	//in the beginning, everything exists
	for(i = 0; i < 1000; i++){
		thisGlobalRegister->existanceArray[i] = maxInt;
	}

}

void destroyTheGlobalRegister(){
	free(thisGlobalRegister);
}

void removeFromExistance(int id){
	clearBit(thisGlobalRegister->existanceArray,id);
}

int doesExist(int id){
	return getBit(thisGlobalRegister->existanceArray, id);
}

void setBit(int * a, int k){

	int i = k/32; // this is the index in the array
	int pos = k%32; // this is the bit in the arr

	unsigned int flag = 1; // flag = 0000...0001 (32 bits)

	flag = flag << pos; // shift that bit right pos times

	a[i] = a[i] | flag; // OR the two values together to merge
}

void clearBit(int * a, int k){
	int i = k/32;
	int pos = k%32;

	unsigned int flag = 1;

	flag = flag << pos;
	flag = ~flag;

	a[i] = a[i] & flag;
}

int getBit(int * a, int k){
	int i = k/32;
	int pos = k%32;

	unsigned int flag = 1;
	flag = flag << pos;
	flag = a[i] & flag;
	flag = flag >> pos;

	return flag;
}
