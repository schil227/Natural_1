/*
 * access_registry.c
 *
 *  Created on: Apr 14, 2017
 *      Author: Adrian
 */
#include "./headers/access_registry_pub_methods.h"
#include<stdio.h>
static lockAuthority * thisLockAuth;

void initLockAuth(){
	thisLockAuth = malloc(sizeof(lockAuthority));
	thisLockAuth->fieldReadLock = 0;
	thisLockAuth->fieldWriteLock = 0;
	thisLockAuth->consoleReadLock = 0;
	thisLockAuth->consoleWriteLock = 0;
	thisLockAuth->hudReadLock = 0;
	thisLockAuth->hudWriteLock = 0;
	thisLockAuth->fieldInventoryReadLock = 0;
	thisLockAuth->fieldInventoryWriteLock = 0;
	thisLockAuth->individualGroupReadLock = 0;
	thisLockAuth->individualGroupWriteLock = 0;
	thisLockAuth->dialogReadLock = 0;
	thisLockAuth->dialogWriteLock = 0;
	thisLockAuth->lookReadLock = 0;
	thisLockAuth->lookWriteLock = 0;
	thisLockAuth->abilityCreationReadLock = 0;
	thisLockAuth->abilityCreationWriteLock = 0;
	thisLockAuth->worldMapReadLock = 0;
	thisLockAuth->worldMapWriteLock = 0;
}

int tryGetFieldReadLock(){
	if(thisLockAuth->fieldReadLock == 0){
		thisLockAuth->fieldReadLock = 1;
		return 1;
	}

	return 0;
}

int tryGetFieldWriteLock(){
	if(thisLockAuth->fieldWriteLock == 0){
		thisLockAuth->fieldWriteLock = 1;
		return 1;
	}

	return 0;
}

void releaseFieldReadLock(){
	thisLockAuth->fieldReadLock = 0;
}

void releaseFieldWriteLock(){
	thisLockAuth->fieldWriteLock = 0;
}

int tryGetConsoleReadLock(){
	if(thisLockAuth->consoleReadLock== 0){
		thisLockAuth->consoleReadLock = 1;
		return 1;
	}

	return 0;
}

int tryGetConsoleWriteLock(){
	if(thisLockAuth->consoleWriteLock == 0){
		thisLockAuth->consoleWriteLock = 1;
		return 1;
	}

	return 0;
}

void releaseConsoleReadLock(){
	thisLockAuth->consoleReadLock = 0;
}

void releaseConsoleWriteLock(){
	thisLockAuth->consoleWriteLock = 0;
}

int tryGetHudReadLock(){
	if(thisLockAuth->hudReadLock== 0){
		thisLockAuth->hudReadLock = 1;
		return 1;
	}

	return 0;
}

int tryGetHudWriteLock(){
	if(thisLockAuth->hudWriteLock == 0){
		thisLockAuth->hudWriteLock = 1;
		return 1;
	}

	return 0;
}

void releaseHudReadLock(){
	thisLockAuth->hudReadLock = 0;
}

void releaseHudWriteLock(){
	thisLockAuth->hudWriteLock = 0;
}

int tryGetFieldInventoryReadLock(){
	if(thisLockAuth->fieldInventoryReadLock== 0){
		thisLockAuth->fieldInventoryReadLock = 1;
		return 1;
	}

	return 0;
}

int tryGetFieldInventoryWriteLock(){
	if(thisLockAuth->fieldInventoryWriteLock == 0){
		thisLockAuth->fieldInventoryWriteLock = 1;
		return 1;
	}

	return 0;
}

void releaseFieldInventoryReadLock(){
	thisLockAuth->fieldInventoryReadLock = 0;
}

void releaseFieldInventoryWriteLock(){
	thisLockAuth->fieldInventoryWriteLock = 0;
}

int tryGetIndividualGroupReadLock(){
	if(thisLockAuth->individualGroupReadLock== 0){
		thisLockAuth->individualGroupReadLock = 1;
		return 1;
	}

	return 0;
}

int tryGetIndividualGroupWriteLock(){
	if(thisLockAuth->individualGroupWriteLock == 0){
		thisLockAuth->individualGroupWriteLock = 1;
		return 1;
	}

	return 0;
}

void releaseIndividualGroupReadLock(){
	thisLockAuth->individualGroupReadLock = 0;
}

void releaseIndividualGroupWriteLock(){
	thisLockAuth->individualGroupWriteLock = 0;
}

int tryGetDialogReadLock(){
	if(thisLockAuth->dialogReadLock== 0){
		thisLockAuth->dialogReadLock = 1;
		return 1;
	}

	return 0;
}

int tryGetDialogWriteLock(){
	if(thisLockAuth->dialogWriteLock == 0){
		thisLockAuth->dialogWriteLock = 1;
		return 1;
	}

	return 0;
}

void releaseDialogReadLock(){
	thisLockAuth->dialogReadLock = 0;
}

void releaseDialogWriteLock(){
	thisLockAuth->dialogWriteLock = 0;
}

int tryGetLookReadLock(){
	if(thisLockAuth->lookReadLock== 0){
		thisLockAuth->lookReadLock = 1;
		return 1;
	}

	return 0;
}

int tryGetLookWriteLock(){
	if(thisLockAuth->lookWriteLock == 0){
		thisLockAuth->lookWriteLock = 1;
		return 1;
	}

	return 0;
}

void releaseLookReadLock(){
	thisLockAuth->lookReadLock = 0;
}

void releaseLookWriteLock(){
	thisLockAuth->lookWriteLock = 0;
}

int tryGetWorldMapReadLock(){
	if(thisLockAuth->worldMapReadLock == 0){
		thisLockAuth->worldMapReadLock = 1;
		return 1;
	}

	return 0;
}

int tryGetWorldMapWriteLock(){
	if(thisLockAuth->worldMapWriteLock == 0){
		thisLockAuth->worldMapWriteLock = 1;
		return 1;
	}

	return 0;
}

void releaseWorldMapReadLock(){
	thisLockAuth->worldMapReadLock = 0;
}

void releaseWorldMapWriteLock(){
	thisLockAuth->worldMapWriteLock = 0;
}


int tryGetAbilityCreationReadLock(){
	if(thisLockAuth->abilityCreationReadLock == 0){
		thisLockAuth->abilityCreationReadLock = 1;
		return 1;
	}

	return 0;
}

int tryGetAbilityCreationWriteLock(){
	if(thisLockAuth->abilityCreationWriteLock == 0){
		thisLockAuth->abilityCreationWriteLock = 1;
		return 1;
	}

	return 0;
}

void releaseAbilityCreationReadLock(){
	thisLockAuth->abilityCreationReadLock = 0;
}

void releaseAbilityCreationWriteLock(){
	thisLockAuth->abilityCreationWriteLock = 0;
}

lockAuthority * getLockAuthDEBUG(){
	return thisLockAuth;
}
