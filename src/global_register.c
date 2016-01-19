/*
 * global_register.c
 *
 *  Created on: Oct 22, 2015
 *      Author: Adrian
 */
#include<math.h>
#include"./headers/structs/global_register.h"

static globalRegister * thisGlobalRegister;

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

	thisGlobalRegister->MAX_INDIVIDUALS = 1000;
	thisGlobalRegister->numIndividuals = 0;
	thisGlobalRegister->MAX_ITEMS = 5000;
	thisGlobalRegister->numItems = 0;

}

individual * getIndividualFromRegistry(int id){
	int i;

	for(i = 0; i < thisGlobalRegister->numIndividuals; i++){
		if(thisGlobalRegister->individualRegistry[i]->ID == id){
			return thisGlobalRegister->individualRegistry[i];
		}
	}

	cwrite("!!INDIVIDUAL NOT FOUND IN REGISTRY!!");

	return NULL;
}

item * getItemFromRegistry(int id){
	int i;

	for(i = 0; i < thisGlobalRegister->numItems; i++){
		if(thisGlobalRegister->itemRegistry[i]->ID == id){
			return thisGlobalRegister->itemRegistry[i];
		}
	}

	cwrite("!!ITEM NOT FOUND IN REGISTRY!!");

	return NULL;
}

int addIndividualToRegistry(individual * thisIndividual){
	if(thisGlobalRegister->numIndividuals < thisGlobalRegister->MAX_INDIVIDUALS){
		thisGlobalRegister->individualRegistry[thisGlobalRegister->numIndividuals] = thisIndividual;
		thisGlobalRegister->numIndividuals++;
		return 1;
	}

	cwrite("!!MAX INDIVIDUALS IN REGISTRY!!");

	return 0;
}

int addItemToRegistry(item * thisItem){
	if(thisGlobalRegister->numItems < thisGlobalRegister->MAX_ITEMS){
		thisGlobalRegister->itemRegistry[thisGlobalRegister->numItems] = thisItem;
		thisGlobalRegister->numItems++;
		return 1;
	}

	cwrite("!!MAX ITEMS IN REGISTRY!!");

	return 0;
}

int removeIndividualFromRegistryByID(int id){
	int i;

	if(thisGlobalRegister->numIndividuals == 0){
		cwrite("!!CANNOT REMOVE: INDIVIDUAL REGISTRY EMPTY!!");
		return 0;
	}

	for(i = 0; i < thisGlobalRegister->numIndividuals; i++){
		if(thisGlobalRegister->individualRegistry[i]->ID == id){
			destroyIndividual(thisGlobalRegister->individualRegistry[i]);

			//rebalance, place last element at i
			thisGlobalRegister->numIndividuals;
			thisGlobalRegister->individualRegistry[i] = thisGlobalRegister->individualRegistry[thisGlobalRegister->numIndividuals];
			thisGlobalRegister->individualRegistry[thisGlobalRegister->numIndividuals] = NULL;

			return 1;
		}
	}

	cwrite("!!CANNOT REMOVE: INDIVIDUAL NOT FOUND!!");

	return 0;

}

int removeItemFromRegistryByID(int id){
	int i;

	if(thisGlobalRegister->numItems == 0){
		cwrite("!!CANNOT REMOVE: ITEM REGISTRY EMPTY!!");
		return 0;
	}

	for(i = 0; i < thisGlobalRegister->numItems; i++){
		if(thisGlobalRegister->itemRegistry[i]->ID == id){
			destroyItem(thisGlobalRegister->itemRegistry[i]);

			//rebalance, place last element at i
			thisGlobalRegister->numItems;
			thisGlobalRegister->itemRegistry[i] = thisGlobalRegister->itemRegistry[thisGlobalRegister->numItems];
			thisGlobalRegister->itemRegistry[thisGlobalRegister->numItems] = NULL;

			return 1;
		}
	}

	cwrite("!!CANNOT REMOVE: ITEM NOT FOUND!!");

	return 0;
}

/*
 * TODO: On cutover, make sure destroy individual doesn't destroy items.
 * That will be handed at the registry level
 */
void destroyTheGlobalRegister(){
	int i;

	for(i = 0; i < thisGlobalRegister->numIndividuals; i++){
		destroyIndividual(thisGlobalRegister->individualRegistry[i]);
	}

	for(i = 0; i < thisGlobalRegister->numItems; i++){
		destroyItem(thisGlobalRegister->itemRegistry[i]);
	}

	free(thisGlobalRegister);
}

void * loadIndividualsToRegistry(char* directory, char * individualsFileName){
	char * fullFileName = appendStrings(directory, individualsFileName);
	//fullFileName[strlen(fullFileName)-1] = '\0'; //remove '\n' at end of line
	FILE * FP = fopen(fullFileName, "r");
	char line[160];


	while(fgets(line,160,FP) != NULL){
		if (line[0] != '#') {
			individual * newIndividual = initIndividual();
			createIndividualFromLine(newIndividual, line);
			addIndividualToRegistry(newIndividual);
		}
	}

	fclose(FP);
	free(fullFileName);
}

void * loadItemsToRegistry(char* directory, char * itemsFileName){
	char * fullFileName = appendStrings(directory, itemsFileName);
	//fullFileName[strlen(fullFileName)-1] = '\0'; //remove '\n' at end of line
	FILE * FP = fopen(fullFileName, "r");
	char line[512];


	while(fgets(line,512,FP) != NULL){
		if (line[0] != '#') {
			item * newItem = createFieldItemFromFile(line);

			if(newItem->npcID != 0){ // equip item to individual
				individual * tmpIndividual = getIndividualFromRegistry(newItem->npcID);

				if(tmpIndividual != NULL){
					addItemToIndividual(tmpIndividual, newItem);
				}else{
					char * errStr[128];
					sprintf(errStr, "!!FAILED ADDING ITEM TO INDIVIDUAL ID : %d!!\0", newItem->npcID);
					cwrite(errStr);
				}
			}
			addItemToRegistry(newItem);

		}
	}

	fclose(FP);
	free(fullFileName);
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
