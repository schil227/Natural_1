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

	thisGlobalRegister->MAX_EVENTS = 1000;
	thisGlobalRegister->numEvents = 0;

	thisGlobalRegister->MAX_EFFECTS = 500;
	thisGlobalRegister->numEffects = 0;

	thisGlobalRegister->MAX_SOUNDS = 300;
	thisGlobalRegister->numSounds = 0;

	thisGlobalRegister->MAX_IMAGES = 500;
	thisGlobalRegister->numImages = 0;
}

individual * getIndividualFromRegistry(int id){
	int i;

	for(i = 0; i < thisGlobalRegister->numIndividuals; i++){
		if(thisGlobalRegister->individualRegistry[i]->ID == id){
			return thisGlobalRegister->individualRegistry[i];
		}
	}

	char * errLog[128];
	sprintf(errLog, "!!INDIVIDUAL NOT FOUND IN REGISTRY - %d!!", id);
	cwrite(errLog);

	return NULL;
}

item * getItemFromRegistry(int id){
	int i;

	for(i = 0; i < thisGlobalRegister->numItems; i++){
		if(thisGlobalRegister->itemRegistry[i]->ID == id){
			return thisGlobalRegister->itemRegistry[i];
		}
	}

	char * errLog[128];
	sprintf(errLog, "!!ITEM NOT FOUND IN REGISTRY - %d!!", id);
	cwrite(errLog);

	return NULL;
}

event * getEventFromRegistry(int id){
	int i;

	for(i = 0; i < thisGlobalRegister->numEvents; i++){
		if(thisGlobalRegister->eventRegistry[i]->ID == id){
			return thisGlobalRegister->eventRegistry[i];
		}
	}

	char * errLog[128];
	sprintf(errLog, "!!EVENT NOT FOUND IN REGISTRY - %d!!", id);
	cwrite(errLog);

	return NULL;
}

char * getSoundPathFromRegistry(int id){
	int i;

	for(i = 0; i < thisGlobalRegister->numSounds; i++){
		if(thisGlobalRegister->soundMapRegistry[i]->ID == id){
			return thisGlobalRegister->soundMapRegistry[i]->fileName;
		}
	}

	char * errLog[128];
	sprintf(errLog, "!!SOUND NOT FOUND IN REGISTRY - %d!!", id);
	cwrite(errLog);

	return NULL;
}

character * getImageFromRegistry(int id){
	int i;

	for(i = 0; i < thisGlobalRegister->numImages; i++){
		if(thisGlobalRegister->imageRegistry[i]->imageID == id){
			return thisGlobalRegister->imageRegistry[i];
		}
	}

	char * errLog[128];
	sprintf(errLog, "!!IMAGE NOT FOUND IN REGISTRY - %d!!", id);
	cwrite(errLog);

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

int addEventToRegistry(event * thisEvent){
	if(thisGlobalRegister->numEvents < thisGlobalRegister->MAX_EVENTS){
		thisGlobalRegister->eventRegistry[thisGlobalRegister->numEvents] = thisEvent;
		thisGlobalRegister->numEvents++;
		return 1;
	}

	cwrite("!!MAX EVENTS IN REGISTRY!!");

	return 0;
}

int addAbilityToRegistry(ability * thisAbility){
	if(thisGlobalRegister->numEffects < thisGlobalRegister->MAX_EFFECTS){
		thisGlobalRegister->abilityRegistry[thisGlobalRegister->numEffects] = thisAbility;
		thisGlobalRegister->numEffects++;
		return 1;
	}

	cwrite("!!MAX EFFECTS IN REGISTRY!!");

	return 0;
}

int addSoundToRegistry(soundMap * thisSoundMap){
	if(thisGlobalRegister->numSounds < thisGlobalRegister->MAX_SOUNDS){
		thisGlobalRegister->soundMapRegistry[thisGlobalRegister->numSounds] = thisSoundMap;
		thisGlobalRegister->numSounds++;
		return 1;
	}

	cwrite("!!MAX SOUNDS IN REGISTRY!!");

	return 0;
}

int addImageToRegistry(character * image){
	if(thisGlobalRegister->numImages< thisGlobalRegister->MAX_IMAGES){
		thisGlobalRegister->imageRegistry[thisGlobalRegister->numImages] = image;
		thisGlobalRegister->numImages++;
		return 1;
	}

	cwrite("!!MAX IMAGES IN REGISTRY!!");

	return 0;
}

/*

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
			thisGlobalRegister->numIndividuals--;
			thisGlobalRegister->individualRegistry[i] = thisGlobalRegister->individualRegistry[thisGlobalRegister->numIndividuals];
			thisGlobalRegister->individualRegistry[thisGlobalRegister->numIndividuals] = NULL;

			return 1;
		}
	}

	char * errLog[128];
	sprintf(errLog, "!!CANNOT REMOVE: INDIVIDUAL NOT FOUND IN REGISTRY - %d!!", id);
	cwrite(errLog);

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
			thisGlobalRegister->numItems--;
			thisGlobalRegister->itemRegistry[i] = thisGlobalRegister->itemRegistry[thisGlobalRegister->numItems];
			thisGlobalRegister->itemRegistry[thisGlobalRegister->numItems] = NULL;

			return 1;
		}
	}

	char * errLog[128];
	sprintf(errLog, "!!CANNOT REMOVE: ITEM NOT FOUND IN REGISTRY - %d!!", id);
	cwrite(errLog);

	return 0;
}

int removeEventFromRegistry(int id){
	int i;

	if(thisGlobalRegister->numEvents == 0){
		cwrite("!!CANNOT REMOVE: EVENT REGISTRY EMPTY!!");
		return 0;
	}

	for(i = 0; i < thisGlobalRegister->numEvents; i++){
		if(thisGlobalRegister->eventRegistry[i]->ID == id){
			free(thisGlobalRegister->eventRegistry[i]);

			//rebalance, place last item at i
			thisGlobalRegister->numEvents--;
			thisGlobalRegister->eventRegistry[i] = thisGlobalRegister->eventRegistry[thisGlobalRegister->numEvents];
			thisGlobalRegister->eventRegistry[thisGlobalRegister->numEvents] = NULL;

			return 1;
		}
	}

	char * errLog[128];
	sprintf(errLog, "!!CANNOT REMOVE: EVENT NOT FOUND IN REGISTRY - %d!!", id);
	cwrite(errLog);

	return 0;
}

*/

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

	for(i = 0; i < thisGlobalRegister->numImages; i++){
		destroyCharacter(thisGlobalRegister->imageRegistry[i]);
	}
	free(thisGlobalRegister);
}

void loadIndividualsToRegistry(char* directory, char * individualsFileName){
	char * fullFileName = appendStrings(directory, individualsFileName);
	//fullFileName[strlen(fullFileName)-1] = '\0'; //remove '\n' at end of line
	FILE * FP = fopen(fullFileName, "r");
	char line[256];


	while(fgets(line,256,FP) != NULL){
		if (line[0] != '#') {
			individual * newIndividual = initIndividual();
			createIndividualFromLine(newIndividual, line);
			addIndividualToRegistry(newIndividual);
		}
	}

	fclose(FP);
	free(fullFileName);
}

void loadItemsToRegistry(char* directory, char * itemsFileName){
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
					addItemToIndividual(tmpIndividual->backpack, newItem);
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

void loadEventsToRegistry(char* directory, char* eventsFileName){
	char * fullFileName = appendStrings(directory, eventsFileName);
	//fullFileName[strlen(fullFileName)-1] = '\0'; //remove '\n' at end of line
	FILE * FP = fopen(fullFileName, "r");
	char line[512];

	while(fgets(line,512,FP) != NULL){
		if (line[0] != '#') {
			event * newEvent = createEventFromFile(line);
			addEventToRegistry(newEvent);
		}
	}

	fclose(FP);
	free(fullFileName);
}

void loadEffectsToRegistry(char* directory, char* effectsFileName){
	char * fullFileName = appendStrings(directory, effectsFileName);
	FILE * FP = fopen(fullFileName, "r");
	char line[2048];

	while(fgets(line,2048,FP) != NULL){
		if (line[0] != '#') {
			ability * newAbility = createAbilityFromLine(line);
			newAbility->totalManaCost = calculateManaCost(newAbility);
			addAbilityToRegistry(newAbility);
		}
	}

	fclose(FP);
	free(fullFileName);
}

void loadSoundsToRegistry(char* directory, char* soundFileName){
	char * fullFileName = appendStrings(directory, soundFileName);
	FILE * FP = fopen(fullFileName, "r");
	char line[160];

	while(fgets(line,160,FP) != NULL){
		if (line[0] != '#') {
			soundMap * newSoundMap = createSoundMapFromLine(line);
			addSoundToRegistry(newSoundMap);
		}
	}

	fclose(FP);
	free(fullFileName);
}

void loadImagesToRegistry(char* directory, char* imageFileName){
	char * fullFileName = appendStrings(directory, imageFileName);
	//fullFileName[strlen(fullFileName)-1] = '\0'; //remove '\n' at end of line
	FILE * FP = fopen(fullFileName, "r");
	char line[128];

	while(fgets(line,128,FP) != NULL){
		if (line[0] != '#') {
			character * newCharacter = createCharacterFromLine(line);
			addImageToRegistry(newCharacter);
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
