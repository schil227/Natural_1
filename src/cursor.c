/*
 * cursor.c
 *
 *  Created on: Apr 26, 2015
 *      Author: Adrian
 */
#include "./headers/cursor_pub_methods.h"

#define ATTACK_SPACE_MARKER_IMAGE_ID 10002

static cursor * thisCursorInstance;

void initThisCursor(int imageID) {
	thisCursorInstance = malloc(sizeof(cursor));
	thisCursorInstance->cursorCharacter = createCharacterFromAnimation(cloneAnimationFromRegistry(imageID));// createCharacter(imageID, rgb, x, y);
	thisCursorInstance->cursorCharacter->x = 0;
	thisCursorInstance->cursorCharacter->y = 0;
	thisCursorInstance->cursorCharacter->xOff = 0;
	thisCursorInstance->cursorCharacter->yOff = 0;
	thisCursorInstance->cursorCharacter->direction = 0;

	thisCursorInstance->tmpCord = malloc(sizeof(cord));
	thisCursorInstance->tmpCord->x = 0;
	thisCursorInstance->tmpCord->y = 0;

	thisCursorInstance->aoeSpaceCoordinates = initCordArr();
	thisCursorInstance->attackSpace = createCharacterFromAnimation(cloneAnimationFromRegistry(ATTACK_SPACE_MARKER_IMAGE_ID));

	thisCursorInstance->inCursorMode = 0;
}

void drawCursor(HDC hdc, HDC hdcBuffer, shiftData * viewData){
	int i;

	if(thisCursorInstance->thisMode == CURSOR_ABILITY_AOE_LINE){
		for(i = 0; i < thisCursorInstance->aoeSpaceCoordinates->numCords; i++){
			drawUnboundAnimation(hdc, hdcBuffer, thisCursorInstance->aoeSpaceCoordinates->cords[i]->x, thisCursorInstance->aoeSpaceCoordinates->cords[i]->y, thisCursorInstance->attackSpace, viewData, 0);
		}
	}

	drawCharacterAnimation(hdc, hdcBuffer, thisCursorInstance->cursorCharacter, viewData, 0);
}

void toggleInCursorMode(){
	thisCursorInstance->inCursorMode = (thisCursorInstance->inCursorMode + 1) % 2;
}

int inCursorMode(){
	if(thisCursorInstance != NULL){
		return thisCursorInstance->inCursorMode;
	}else{
		return 0;
	}
}

void refreshCursor(cursorModes thisMode, int x, int y){
	thisCursorInstance->thisMode = thisMode;
	setCursorCoords(x,y);
}

cursorModes getCursorMode(){
	return thisCursorInstance->thisMode;
}

int getCursorX(){
	return thisCursorInstance->cursorCharacter->x;
}

int getCursorY(){
	return thisCursorInstance->cursorCharacter->y;
}

void setCursorCoords(int x, int y){
	thisCursorInstance->cursorCharacter->x = x;
	thisCursorInstance->cursorCharacter->y = y;
}

int getCursorTmpX(){
	return thisCursorInstance->tmpCord->x;
}

int getCursorTmpY(){
	return thisCursorInstance->tmpCord->y;
}

void setCursorTmpCord(int x, int y){
	thisCursorInstance->tmpCord->x = x;
	thisCursorInstance->tmpCord->y = y;
}

int canMoveCursor(individual * thisIndividual, int direction){
	ability * tmpAbility = thisIndividual->activeAbilities->selectedAbility;

	//If it's a self-duration ability, dont move cursor from player position
	if(thisCursorInstance->thisMode == CURSOR_ABILITY && tmpAbility != NULL){
		if(tmpAbility->type == 'd'){
			return 0;
		}

		if(tmpAbility->type == 't'){
			int range = tmpAbility->range->effectAndManaArray[tmpAbility->range->selectedIndex]->effectMagnitude;

			int newX = getNewCursorX(direction);
			int newY = getNewCursorY(direction);

			if(range < max(abs(newX - thisIndividual->playerCharacter->x), abs(newY - thisIndividual->playerCharacter->y))){
				return 0;
			}
		}
	}

	//stay within the line length when moving
	if(thisCursorInstance->thisMode == CURSOR_ABILITY_AOE_LINE && tmpAbility != NULL){
		int lineLength = tmpAbility->aoeLine->effectAndManaArray[tmpAbility->aoeLine->selectedIndex]->effectMagnitude;

		int newX = getNewCursorX(direction);
		int newY = getNewCursorY(direction);

		if(!cordArrContainsCoordinates(thisCursorInstance->aoeSpaceCoordinates, newX, newY) &&
				lineLength < max(abs(newX - thisCursorInstance->tmpCord->x), abs(newY - thisCursorInstance->tmpCord->y))){
			return 0;
		}
	}

	return 1;
}

int getNewCursorX(int direction){
	return thisCursorInstance->cursorCharacter->x + xMoveChange(direction);
}

int getNewCursorY(int direction){
	return thisCursorInstance->cursorCharacter->y + yMoveChange(direction);
}

int moveCursor(field *thisField, int direction, shiftData * viewShift, RECT * rect){
	int newX = thisCursorInstance->cursorCharacter->x + xMoveChange(direction);
	int newY = thisCursorInstance->cursorCharacter->y + yMoveChange(direction);

	if(newX >= 0 && newX < thisField->totalX && newY >=0 && newY < thisField->totalY){
		setCursorCoords(newX, newY);
		tryUpdateXShift(viewShift, newX, getGameFieldAreaX(rect));
		tryUpdateYShift(viewShift, newY, getGameFieldAreaY(rect));
		return 1;
	}else{
		return 0;
	}
}

void destroyThisCursor(){
	if(thisCursorInstance->cursorCharacter){
		destroyCharacter(thisCursorInstance->cursorCharacter);
	}

	free(thisCursorInstance->tmpCord);
	destroyCordArr(thisCursorInstance->aoeSpaceCoordinates);

	free(thisCursorInstance);
}

void updateCursorAOESpaces(cordArr * newCords){
	if(newCords == NULL){
		return;
	}

	while(!tryGetFieldReadLock()){}
	while(!tryGetFieldWriteLock()){}

	destroyCordArr(thisCursorInstance->aoeSpaceCoordinates);

	thisCursorInstance->aoeSpaceCoordinates = newCords;

	releaseFieldWriteLock();
	releaseFieldReadLock();
}

void clearCursorAOESpaces(){
	while(!tryGetFieldReadLock()){}
	while(!tryGetFieldWriteLock()){}

	destroyCordArr(thisCursorInstance->aoeSpaceCoordinates);

	thisCursorInstance->aoeSpaceCoordinates = initCordArr();

	releaseFieldWriteLock();
	releaseFieldReadLock();
}
