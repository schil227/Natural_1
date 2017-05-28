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

	thisGlobalRegister->MAX_ANIMATIONS = 10000;
	thisGlobalRegister->numAnimations = 0;

	thisGlobalRegister->MAX_TEMPLATE_ABILITIES = 4;
	thisGlobalRegister->numTemplateAbilities = 0;

	thisGlobalRegister->MAX_PERMINENT_ABILITIES= 100;
	thisGlobalRegister->numPerminentAbilities = 0;

	thisGlobalRegister->MAX_SELF_ABILITIES = 100;
	thisGlobalRegister->numSelfAbilities = 0;

	thisGlobalRegister->MAX_TARGETED_ABILITIES = 100;
	thisGlobalRegister->numTargetedAbilities = 0;

	thisGlobalRegister->MAX_INSTANT_ABILITIES = 100;
	thisGlobalRegister->numInstantAbilities = 0;

	thisGlobalRegister->MAX_MAPS = 1000;
	thisGlobalRegister->numMaps = 0;

	thisGlobalRegister->MAX_DESCRIPTIONS = 500;
	thisGlobalRegister->numDescriptions = 0;
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

event * getEventFromRegistryByTypeAndIntA(int type, int intA){
	int i;

	for(i = 0; i < thisGlobalRegister->numEvents; i++){
		if(thisGlobalRegister->eventRegistry[i]->eventType == type && thisGlobalRegister->eventRegistry[i]->intA == intA){
			return thisGlobalRegister->eventRegistry[i];
		}
	}
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

animation * getAnimationFromRegistry(int id){
	int i;

	for(i = 0; i < thisGlobalRegister->numAnimations; i++){
		if(thisGlobalRegister->animationRegistry[i]->imageID == id){
			return thisGlobalRegister->animationRegistry[i];
		}
	}

	char * errLog[128];
	sprintf(errLog, "!!IMAGE NOT FOUND IN REGISTRY - %d!!", id);
	cwrite(errLog);

	return NULL;
}

animation * cloneAnimationFromRegistry(int id){
	int i;

	if(id == -1){
		return NULL;
	}

	for(i = 0; i < thisGlobalRegister->numAnimations; i++){
		if(thisGlobalRegister->animationRegistry[i]->imageID == id){
			return cloneAnimation(thisGlobalRegister->animationRegistry[i]);
		}
	}

	char * errLog[128];
	sprintf(errLog, "!!IMAGE NOT FOUND IN REGISTRY - %d!!", id);
	cwrite(errLog);

	return NULL;
}

ability * getTemplateAbilityFromRegistry(int id){
	int i;

	for(i = 0; i < thisGlobalRegister->numTemplateAbilities; i++){
		if(thisGlobalRegister->templateAbilities[i]->ID == id){
			return thisGlobalRegister->templateAbilities[i];
		}
	}

	return NULL;
}

ability * getPerminentAbilityFromRegistry(int id){
	int i;

	for(i = 0; i < thisGlobalRegister->numPerminentAbilities; i++){
		if(thisGlobalRegister->perminentAbilities[i]->ID == id){
			return thisGlobalRegister->perminentAbilities[i];
		}
	}

	return NULL;
}

ability * getSelfAbilityFromRegistry(int id){
	int i;

	for(i = 0; i < thisGlobalRegister->numSelfAbilities; i++){
		if(thisGlobalRegister->selfAbilities[i]->ID == id){
			return thisGlobalRegister->selfAbilities[i];
		}
	}

	return NULL;
}

ability * getTargetedAbilityFromRegistry(int id){
	int i;

	for(i = 0; i < thisGlobalRegister->numTargetedAbilities; i++){
		if(thisGlobalRegister->targetedAbilities[i]->ID == id){
			return thisGlobalRegister->targetedAbilities[i];
		}
	}

	return NULL;
}

ability * getInstantAbilityFromRegistry(int id){
	int i;

	for(i = 0; i < thisGlobalRegister->numInstantAbilities; i++){
		if(thisGlobalRegister->instantAbilities[i]->ID == id){
			return thisGlobalRegister->instantAbilities[i];
		}
	}

	return NULL;
}

ability * getAbilityFromRegistryFromType(int id, int abilityType){
	switch(abilityType){
	case 0:
		return getPerminentAbilityFromRegistry(id);
	case 1:
			return getSelfAbilityFromRegistry(id);
	case 2:
			return getTargetedAbilityFromRegistry(id);
	case 3:
			return getInstantAbilityFromRegistry(id);
	}

	return NULL;
}

mapInfo * getMapInfoFromRegistry(int id){
	int i;

	for(i = 0; i < thisGlobalRegister->numMaps; i++){
		if(thisGlobalRegister->mapInfoArr[i]->id == id){
			return thisGlobalRegister->mapInfoArr[i];
		}
	}

	char * errLog[128];
	sprintf(errLog, "!!MAPINFO NOT FOUND IN REGISTRY - %d!!", id);
	cwrite(errLog);

	return NULL;
}

mapInfo * getCurrentMapInfoFromRegistry(){
	int i;

	for(i = 0; i < thisGlobalRegister->numMaps; i++){
		if(thisGlobalRegister->mapInfoArr[i]->isCurrentMap){
			return thisGlobalRegister->mapInfoArr[i];
		}
	}

	char * errLog[128];
	sprintf(errLog, "!!COULD NOT FIND CURRENT MAPINFO IN REGISTRY!!");
	cwrite(errLog);

	return NULL;
}

char * getDescriptionFromID(int id){
	int i;

	for(i = 0; i < thisGlobalRegister->numDescriptions; i++){
		if(thisGlobalRegister->descriptions[i]->ID == id){
			return thisGlobalRegister->descriptions[i]->description;
		}
	}

	char * errLog[128];
	sprintf(errLog, "!!DESCRIPTIONMAP NOT FOUND IN REGISTRY - %d!!", id);
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

int addTemplateAbilityToRegistry(ability * thisAbility){
	if(thisGlobalRegister->numTemplateAbilities < thisGlobalRegister->MAX_TEMPLATE_ABILITIES){
		thisGlobalRegister->templateAbilities[thisGlobalRegister->numTemplateAbilities] = thisAbility;
		thisGlobalRegister->numTemplateAbilities++;
		return 1;
	}

	cwrite("!!MAX ABILITIES IN REGISTRY!!");

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

int addAnimationToRegistry(animation * thisAnimation){
	if(thisGlobalRegister->numAnimations< thisGlobalRegister->MAX_ANIMATIONS){
		thisGlobalRegister->animationRegistry[thisGlobalRegister->numAnimations] = thisAnimation;
		thisGlobalRegister->numAnimations++;
		return 1;
	}

	cwrite("!!MAX ANIMATIONS IN REGISTRY!!");

	return 0;
}

int addAbilityToRegistryByType(ability * newAbility){
	switch(newAbility->type){
	case 'p':
		return addPerminentAbilityToRegistry(newAbility);
	case 'd':
		return addSelfAbilityToRegistry(newAbility);
	case 't':
		return addTargetAbilityToRegistry(newAbility);
	case 'i':
		return addInstantAbilityToRegistry(newAbility);
	}
}

int addPerminentAbilityToRegistry(ability * thisAbility){
	if(thisGlobalRegister->numPerminentAbilities < thisGlobalRegister->MAX_PERMINENT_ABILITIES){
		thisGlobalRegister->perminentAbilities[thisGlobalRegister->numPerminentAbilities] = thisAbility;
		thisGlobalRegister->numPerminentAbilities++;
		return 1;
	}

	cwrite("!! MAX PERMINENT ABILITIES IN REGISTRY !!");

	return 0;
}

int addSelfAbilityToRegistry(ability * thisAbility){
	if(thisGlobalRegister->numSelfAbilities < thisGlobalRegister->MAX_SELF_ABILITIES){
		thisGlobalRegister->selfAbilities[thisGlobalRegister->numSelfAbilities] = thisAbility;
		thisGlobalRegister->numSelfAbilities++;
		return 1;
	}

	cwrite("!! MAX SELF ABILITIES IN REGISTRY !!");

	return 0;
}

int addTargetAbilityToRegistry(ability * thisAbility){
	if(thisGlobalRegister->numTargetedAbilities < thisGlobalRegister->MAX_TARGETED_ABILITIES){
		thisGlobalRegister->targetedAbilities[thisGlobalRegister->numTargetedAbilities] = thisAbility;
		thisGlobalRegister->numTargetedAbilities++;
		return 1;
	}

	cwrite("!! MAX TARGETED ABILITIES IN REGISTRY !!");

	return 0;
}

int addInstantAbilityToRegistry(ability * thisAbility){
	if(thisGlobalRegister->numInstantAbilities < thisGlobalRegister->MAX_INSTANT_ABILITIES){
		thisGlobalRegister->instantAbilities[thisGlobalRegister->numInstantAbilities] = thisAbility;
		thisGlobalRegister->numInstantAbilities++;
		return 1;
	}

	cwrite("!! MAX INSTANT ABILITIES IN REGISTRY !!");

	return 0;
}

int addMapInfoToRegistry(mapInfo * thisMap){
	if(thisGlobalRegister->numMaps < thisGlobalRegister->MAX_MAPS){
		thisGlobalRegister->mapInfoArr[thisGlobalRegister->numMaps] = thisMap;
		thisGlobalRegister->numMaps++;
		return 1;
	}

	cwrite("!!MAX MAPS IN REGISTRY!!");

	return 0;
}

int addDescriptionMapToRegistry(descriptionMap * thisDescription){
	if(thisGlobalRegister->numDescriptions < thisGlobalRegister->MAX_DESCRIPTIONS){
		thisGlobalRegister->descriptions[thisGlobalRegister->numDescriptions] = thisDescription;
		thisGlobalRegister->numDescriptions++;
		return 1;
	}

	cwrite("!!MAX DESCRIPTIONMAPS IN REGISTRY!!");

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

	for(i = 0; i < thisGlobalRegister->numAnimations; i++){
		destroyAnimation(thisGlobalRegister->animationRegistry[i]);
	}

	for(i = 0; i < thisGlobalRegister->numMaps; i++){
		free(thisGlobalRegister->mapInfoArr[i]);
	}

	for(i = 0; i < thisGlobalRegister->numDescriptions; i++){
		free(thisGlobalRegister->descriptions[i]);
	}

//	for(i = 0; i < thisGlobalRegister->numEvents; i++){
//		free(thisGlobalRegister->eventRegistry[i]);
//	}
//
//	for(i = 0; i < thisGlobalRegister->numSounds; i++){
//		free(thisGlobalRegister->soundMapRegistry[i]);
//	}

	//TODO: gotta free up the registry better.
	free(thisGlobalRegister);
}

void loadGlobalRegister(char * saveMapDirectory, char * mapDirectory, char * individualsData, char * itemsData, char * eventsData, char * soundsData,
						char * animationData, char * permenantAbilitiesData, char * selfAbilitiesData, char * targetedAbilitiesData,
						char * instantAbilitiesData, char * mapInfo, char * descriptions){
	// Priority loading:
	//individuals dependant on xAbilities
	loadPermenantAbilitiesToRegistry(saveMapDirectory, permenantAbilitiesData);
	loadSelfAbilitiesToRegistry(saveMapDirectory, selfAbilitiesData);
	loadTargetedAbilitiesToRegistry(saveMapDirectory, targetedAbilitiesData);
	loadInstantAbilitiesToRegistry(saveMapDirectory, instantAbilitiesData);

	loadAnimationsToRegistry(mapDirectory, animationData);
	loadIndividualsToRegistry(saveMapDirectory, individualsData);
	loadItemsToRegistry(saveMapDirectory, itemsData);
	loadEventsToRegistry(mapDirectory, eventsData);
	loadSoundsToRegistry(mapDirectory, soundsData);
	loadMapDataToRegistry(saveMapDirectory, mapInfo);
	loadDescriptionsToRegistry(mapDirectory, descriptions);
}

void loadIndividualsToRegistry(char* directory, char * individualsFileName){
	char * fullFileName = appendStrings(directory, individualsFileName);
	//fullFileName[strlen(fullFileName)-1] = '\0'; //remove '\n' at end of line
	FILE * FP = fopen(fullFileName, "r");
	char line[2048];

	while(fgets(line,2048,FP) != NULL){
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
	int registeredCounterLine = 0;
	char * fullFileName = appendStrings(directory, itemsFileName);
	//fullFileName[strlen(fullFileName)-1] = '\0'; //remove '\n' at end of line
	FILE * FP = fopen(fullFileName, "r");
	char line[1024];

	while(fgets(line,1024,FP) != NULL){
		if (line[0] != '#') {
			if(!registeredCounterLine){
				setItemIDIncrement(atoi(line));
				registeredCounterLine = 1;
			}else{
				item * newItem = createFieldItemFromFile(line);

				if(newItem->npcID != 0){ // equip item to individual
					individual * tmpIndividual = getIndividualFromRegistry(newItem->npcID);

					if(tmpIndividual != NULL){
						addItemToInventory(tmpIndividual->backpack, newItem);
						if(newItem->isEquipt){
							if(newItem->type == 'a'){
								tmpIndividual->armorItem = newItem->itemCharacter;
							}else if(newItem->type == 's'){
								tmpIndividual->shieldItem = newItem->itemCharacter;
							}else if(newItem->type == 'w'){
								tmpIndividual->weaponItem = newItem->itemCharacter;
							}
						}
					}else{
						char * errStr[128];
						sprintf(errStr, "!!FAILED ADDING ITEM TO INDIVIDUAL ID : %d!!\0", newItem->npcID);
						cwrite(errStr);
					}
				}
				addItemToRegistry(newItem);
			}
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

void loadAnimationsToRegistry(char* directory, char* animationFileName){
	char * fullFileName = appendStrings(directory, animationFileName);
	//fullFileName[strlen(fullFileName)-1] = '\0'; //remove '\n' at end of line
	FILE * FP = fopen(fullFileName, "r");
	char line[128];

	while(fgets(line,512,FP) != NULL){
		if (line[0] != '#') {
			animation * newAnimation = createAnimationFromLine(line);
			addAnimationToRegistry(newAnimation);
		}
	}

	fclose(FP);
	free(fullFileName);
}

void loadPermenantAbilitiesToRegistry(char* directory, char* permenantAbilitiesFileName){
	char * fullFileName = appendStrings(directory, permenantAbilitiesFileName);

	FILE * FP = fopen(fullFileName, "r");
	char line[128];

	while(fgets(line,128,FP) != NULL){
		if (line[0] != '#') {
			ability * newAbility = createPermenantAbilityFromLine(line);
			addPerminentAbilityToRegistry(newAbility);
		}
	}

	fclose(FP);
	free(fullFileName);
}

void loadSelfAbilitiesToRegistry(char* directory, char* selfAbilitiesFileName){
	char * fullFileName = appendStrings(directory, selfAbilitiesFileName);

	FILE * FP = fopen(fullFileName, "r");
	char line[128];

	while(fgets(line,128,FP) != NULL){
		if (line[0] != '#') {
			ability * newAbility = createDurationAbilityFromLine(line);
			addSelfAbilityToRegistry(newAbility);
		}
	}

	fclose(FP);
	free(fullFileName);
}

void loadTargetedAbilitiesToRegistry(char* directory, char* targetedAbilitiesFileName){
	char * fullFileName = appendStrings(directory, targetedAbilitiesFileName);

	FILE * FP = fopen(fullFileName, "r");
	char line[128];

	while(fgets(line,128,FP) != NULL){
		if (line[0] != '#') {
			ability * newAbility = createTargetedAbilityFromLine(line);
			addTargetAbilityToRegistry(newAbility);
		}
	}

	fclose(FP);
	free(fullFileName);
}

void loadInstantAbilitiesToRegistry(char* directory, char* instantAbilitiesFileName){
	char * fullFileName = appendStrings(directory, instantAbilitiesFileName);

	FILE * FP = fopen(fullFileName, "r");
	char line[128];

	while(fgets(line,128,FP) != NULL){
		if (line[0] != '#') {
			ability * newAbility = createInstantAbilityFromLine(line);
			addInstantAbilityToRegistry(newAbility);
		}
	}

	fclose(FP);
	free(fullFileName);
}

void loadMapDataToRegistry(char * directory, char * mapData){
	char * fullFileName = appendStrings(directory, mapData);
	FILE * FP = fopen(fullFileName, "r");
	char line[1024];

	while(fgets(line,1024,FP) != NULL){
		if (line[0] != '#') {
			mapInfo * tmpMap = initMapInfo();
			createMapInfoFromLine(tmpMap, line);
			addMapInfoToRegistry(tmpMap);
		}
	}

	fclose(FP);
	free(fullFileName);
}

void loadDescriptionsToRegistry(char * directory, char * mapData){
	char * fullFileName = appendStrings(directory, mapData);
	FILE * FP = fopen(fullFileName, "r");
	char line[272];

	while(fgets(line,272,FP) != NULL){
		if (line[0] != '#') {
			descriptionMap * tmpDescription = malloc(sizeof(descriptionMap));
			tmpDescription->ID = 0;
			tmpDescription->description[0] = '\0';

			createDescriptionMapFromLine(tmpDescription, line);
			addDescriptionMapToRegistry(tmpDescription);
		}
	}

	fclose(FP);
	free(fullFileName);
}

void createDescriptionMapFromLine(descriptionMap * tmpDescription, char * line){
	char * strtok_save_pointer;
	char * value;

	value = strtok_r(line, ";", &strtok_save_pointer);
	tmpDescription->ID = atoi(value);

	value = strtok_r(NULL, ";", &strtok_save_pointer);
	strcpy(tmpDescription->description, value);
}

void deleteItemFromRegistry(int id){
	int i;

	for(i = 0; i < thisGlobalRegister->numItems; i++){
		if(thisGlobalRegister->itemRegistry[i]->ID == id){
			thisGlobalRegister->numItems--;
			thisGlobalRegister->itemRegistry[i] = thisGlobalRegister->itemRegistry[thisGlobalRegister->numItems];
			thisGlobalRegister->itemRegistry[thisGlobalRegister->numItems] = NULL;

			return;
		}
	}

	char * errLog[128];
	sprintf(errLog, "!!ITEM NOT FOUND IN REGISTRY - %d!!", id);
	cwrite(errLog);

	return;
}

int numAbilitiesInGlobalRegistry(){
	return thisGlobalRegister->numPerminentAbilities + thisGlobalRegister->numSelfAbilities + thisGlobalRegister->numTargetedAbilities + thisGlobalRegister->numInstantAbilities;
}

void populateMapGeneratorImages(character * characterSet[1000], int * numImages, int * maxImages){
	int i, j = 0;

	for(i = 0; i < thisGlobalRegister->numAnimations; i++){
		if(thisGlobalRegister->animationRegistry[i] != NULL && thisGlobalRegister->animationRegistry[i]->imageID >= 7500 && thisGlobalRegister->animationRegistry[i]->imageID < 8500){
			if(j < *maxImages){
				characterSet[j] = createCharacterFromAnimation(thisGlobalRegister->animationRegistry[i]);
				characterSet[j++] = createCharacterFromAnimation(thisGlobalRegister->animationRegistry[i]);
				characterSet[j++] = createCharacterFromAnimation(thisGlobalRegister->animationRegistry[i]);
				characterSet[j++] = createCharacterFromAnimation(thisGlobalRegister->animationRegistry[i]);
				characterSet[j++] = createCharacterFromAnimation(thisGlobalRegister->animationRegistry[i]);

//				j++;
			}else{
				break;
			}
		}
	}

	*numImages = j;
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

//////// Writing to Files //////////

void writeIndividualsToFile(char * directory, char * saveDirectory, char * individualsFileName){
	int i = 0;

	while(!tryGetIndividualGroupReadLock()){}
	while(!tryGetIndividualGroupWriteLock()){}

	char fullFileName[256];
	i += sprintf(fullFileName, "%s", directory);
	i += sprintf(fullFileName + i, "%s", saveDirectory);

	_mkdir(fullFileName);

	i += sprintf(fullFileName + i, "%s", individualsFileName);

	FILE * FP = fopen(fullFileName, "w");

	for(i = 0; i < thisGlobalRegister->numIndividuals; i++){
		char * line = getIndividualAsLine(thisGlobalRegister->individualRegistry[i]);
		fprintf(FP, "%s\n",  line);
		free(line);
	}

	fclose(FP);

	releaseIndividualGroupWriteLock();
	releaseIndividualGroupReadLock();
}

void writeItemsToFile(char * directory, char * saveDirectory, char * itemsFileName){
	int i = 0;

	while(!tryGetIndividualGroupReadLock()){}
	while(!tryGetIndividualGroupWriteLock()){}

	char fullFileName[256];
	i += sprintf(fullFileName, "%s", directory);
	i += sprintf(fullFileName + i, "%s", saveDirectory);

	_mkdir(fullFileName);

	i += sprintf(fullFileName + i, "%s", itemsFileName);

	FILE * FP = fopen(fullFileName, "w");

	//Add the current item id counter
	fprintf(FP, "%d\n", getCurrentItemIDIncrement());

	for(i = 0; i < thisGlobalRegister->numItems; i++){
		char * line = getItemAsLine(thisGlobalRegister->itemRegistry[i]);

		//there's a '/n' at the end of the description, no need to include
		fprintf(FP, "%s",  line);
		free(line);
	}

	fclose(FP);

	releaseIndividualGroupWriteLock();
	releaseIndividualGroupReadLock();
}

void writeMapInfoToFile(char * directory, char * saveDirectory, char * mapInfoFileName){
	int i = 0;

	while(!tryGetFieldReadLock()){}
	while(!tryGetFieldWriteLock()){}

	char fullFileName[256];
	i += sprintf(fullFileName, "%s", directory);
	i += sprintf(fullFileName + i, "%s", saveDirectory);

	_mkdir(fullFileName);

	i += sprintf(fullFileName + i, "%s", mapInfoFileName);

	FILE * FP = fopen(fullFileName, "w");

	for(i = 0; i < thisGlobalRegister->numMaps; i++){
		char * line = getMapInfoAsLine(thisGlobalRegister->mapInfoArr[i]);
		fprintf(FP, "%s\n",  line);
		free(line);
	}

	fclose(FP);

	releaseFieldWriteLock();
	releaseFieldReadLock();
}

void writePermenantAbilitiesToFile(char * directory, char * saveDirectory, char * mapInfoFileName){
	int i = 0;

	while(!tryGetFieldReadLock()){}
	while(!tryGetFieldWriteLock()){}

	char fullFileName[256];
	i += sprintf(fullFileName, "%s", directory);
	i += sprintf(fullFileName + i, "%s", saveDirectory);

	_mkdir(fullFileName);

	i += sprintf(fullFileName + i, "%s", mapInfoFileName);

	FILE * FP = fopen(fullFileName, "w");
	for(i = 0; i < thisGlobalRegister->numPerminentAbilities; i++){
		char * line = getPermenantAbilityAsLine(thisGlobalRegister->perminentAbilities[i]);
		fprintf(FP, "%s\n",  line);
		free(line);
	}
	fclose(FP);

	releaseFieldWriteLock();
	releaseFieldReadLock();
}

void writeDurationAbilitiesToFile(char * directory, char * saveDirectory, char * mapInfoFileName){
	int i = 0;

	while(!tryGetFieldReadLock()){}
	while(!tryGetFieldWriteLock()){}

	char fullFileName[256];
	i += sprintf(fullFileName, "%s", directory);
	i += sprintf(fullFileName + i, "%s", saveDirectory);

	_mkdir(fullFileName);

	i += sprintf(fullFileName + i, "%s", mapInfoFileName);

	FILE * FP = fopen(fullFileName, "w");

	for(i = 0; i < thisGlobalRegister->numSelfAbilities; i++){
		char * line = getDurationAbilityAsLine(thisGlobalRegister->selfAbilities[i]);
		fprintf(FP, "%s\n",  line);
		free(line);
	}

	fclose(FP);

	releaseFieldWriteLock();
	releaseFieldReadLock();
}

void writeTargetedAbilitiesToFile(char * directory, char * saveDirectory, char * mapInfoFileName){
	int i = 0;

	while(!tryGetFieldReadLock()){}
	while(!tryGetFieldWriteLock()){}

	char fullFileName[256];
	i += sprintf(fullFileName, "%s", directory);
	i += sprintf(fullFileName + i, "%s", saveDirectory);

	_mkdir(fullFileName);

	i += sprintf(fullFileName + i, "%s", mapInfoFileName);

	FILE * FP = fopen(fullFileName, "w");

	for(i = 0; i < thisGlobalRegister->numTargetedAbilities; i++){
		char * line = getTargetAbilityAsLine(thisGlobalRegister->targetedAbilities[i]);
		fprintf(FP, "%s\n",  line);
		free(line);
	}

	fclose(FP);

	releaseFieldWriteLock();
	releaseFieldReadLock();
}

void writeInstantAbilitiesToFile(char * directory, char * saveDirectory, char * mapInfoFileName){
	int i = 0;

	while(!tryGetFieldReadLock()){}
	while(!tryGetFieldWriteLock()){}

	char fullFileName[256];
	i += sprintf(fullFileName, "%s", directory);
	i += sprintf(fullFileName + i, "%s", saveDirectory);

	_mkdir(fullFileName);

	i += sprintf(fullFileName + i, "%s", mapInfoFileName);

	FILE * FP = fopen(fullFileName, "w");

	for(i = 0; i < thisGlobalRegister->numInstantAbilities; i++){
		char * line = getInstantAbilityAsLine(thisGlobalRegister->instantAbilities[i]);
		fprintf(FP, "%s\n",  line);
		free(line);
	}

	fclose(FP);

	releaseFieldWriteLock();
	releaseFieldReadLock();
}
