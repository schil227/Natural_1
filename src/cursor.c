/*
 * cursor.c
 *
 *  Created on: Apr 26, 2015
 *      Author: Adrian
 */
#include "./headers/cursor_pub_methods.h"

static cursor * thisCursorInstance;

void initThisCursor(int imageID) {
	thisCursorInstance = malloc(sizeof(cursor));
	thisCursorInstance->cursorCharacter = createCharacterFromAnimation(cloneAnimationFromRegistry(imageID));// createCharacter(imageID, rgb, x, y);
	thisCursorInstance->cursorCharacter->x = 0;
	thisCursorInstance->cursorCharacter->y = 0;
	thisCursorInstance->cursorCharacter->direction = 0;

	thisCursorInstance->inCursorMode = 0;
}

void drawCursor(HDC hdc, HDC hdcBuffer, shiftData * viewData){
	drawCharacterAnimation(hdc, hdcBuffer, thisCursorInstance->cursorCharacter, viewData, 0);
//	HDC hdcMem = CreateCompatibleDC(hdc);
//	SelectObject(hdcMem, thisCursorInstance->cursorCharacter->imageMask);
//	BitBlt(hdcBuffer, thisCursorInstance->cursorCharacter->x*40 - (viewData->xShift)*40, thisCursorInstance->cursorCharacter->y*40 - (viewData->yShift)*40, thisCursorInstance->cursorCharacter->width, thisCursorInstance->cursorCharacter->height, hdcMem, 0, 0, SRCAND);
//	SelectObject(hdcMem, thisCursorInstance->cursorCharacter->image);
//	BitBlt(hdcBuffer, thisCursorInstance->cursorCharacter->x*40 - (viewData->xShift)*40, thisCursorInstance->cursorCharacter->y*40 - (viewData->yShift)*40, thisCursorInstance->cursorCharacter->width, thisCursorInstance->cursorCharacter->height, hdcMem, 0, 0, SRCPAINT);
//	DeleteDC(hdcMem);
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

int canMoveCursor(individual * thisIndividual){

	//If it's a self-duration ability, dont move cursor from player position
	if(thisCursorInstance->thisMode == CURSOR_ABILITY && thisIndividual->activeAbilities->selectedAbility != NULL
			&&  thisIndividual->activeAbilities->selectedAbility->type == 'd'){
		return 0;
	}

	return 1;
}

int moveCursor(field *thisField, int direction, shiftData * viewShift){
	int newX = thisCursorInstance->cursorCharacter->x + xMoveChange(direction);
	int newY = thisCursorInstance->cursorCharacter->y + yMoveChange(direction);
	if(newX >= 0 && newX < thisField->totalX && newY >=0 && newY < thisField->totalY){
		setCursorCoords(newX, newY);
		tryUpdateXShift(viewShift, newX);
		tryUpdateYShift(viewShift, newY);
		return 1;
	}else{
		return 0;
	}

}

void destroyThisCursor(){
	if(thisCursorInstance->cursorCharacter){
		destroyCharacter(thisCursorInstance->cursorCharacter);
	}
	free(thisCursorInstance);
}



