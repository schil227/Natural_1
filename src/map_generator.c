/*
 * map_generator.c
 *
 *  Created on: May 23, 2017
 *      Author: Adrian
 */
#include "./headers/map_generator_pub_methods.h"
#include <stdio.h>
static mapGenerator * thisMapGenerator;

space * makeMapGeneratorSpace(int x, int y){
	space * tmpSpace = malloc(sizeof(space));
	tmpSpace->background =  createCharacterFromAnimation(cloneAnimationFromRegistry(7501)); //green space
	tmpSpace->background->x = x;
	tmpSpace->background->y = y;
	tmpSpace->background->xOff = 0;
	tmpSpace->background->yOff = 0;

	tmpSpace->canAttackThrough = 0;
	tmpSpace->canSeeThrough = 0;
	tmpSpace->currentIndividual = NULL;
	tmpSpace->isPassable = 0;
	tmpSpace->spaceIsReserved = 0;
	tmpSpace->thisTransitInfo = NULL;

	return tmpSpace;
}

void initMapGenerator(char * mapDirectory){
	int i,j;

	thisMapGenerator = malloc(sizeof(mapGenerator));

	thisMapGenerator->inSelectionMode = 0;
	thisMapGenerator->selectionStartingHeight = 0;
	thisMapGenerator->numCharactersPerRow = 10;
	thisMapGenerator->currentSelectedCharacterIndex  = 0;
	thisMapGenerator->numCharacters = 0;
	thisMapGenerator->maxCharacters = 1000;
	thisMapGenerator->selectedIndex = 2;
	thisMapGenerator->rotation = 0;

	populateMapGeneratorImages(thisMapGenerator->characterSet, &(thisMapGenerator->numCharacters), &(thisMapGenerator->maxCharacters));

	thisMapGenerator->cursor = createCharacterFromAnimation(cloneAnimationFromRegistry(1508));
	thisMapGenerator->cursor->x = 0;
	thisMapGenerator->cursor->y = 0;
	thisMapGenerator->cursor->xOff = 0;
	thisMapGenerator->cursor->yOff = 0;

	thisMapGenerator->selectedCharacter = thisMapGenerator->characterSet[2];
	thisMapGenerator->selectedCharacter->x = 0;
	thisMapGenerator->selectedCharacter->y = 0;
	thisMapGenerator->selectedCharacter->xOff = 0;
	thisMapGenerator->selectedCharacter->yOff = 0;

	thisMapGenerator->selectedCharacterBox = createCharacter(1531, RGB(255,0,255), 0, 0);
	thisMapGenerator->characterSelectBackground = createCharacter(1532, RGB(255,0,255), 0, 0);

	char loadMap[256];
	sprintf(loadMap, "%sgeneratedMaps\\loadmap.txt", mapDirectory);

	thisMapGenerator->thisField = initField(loadMap);

//	thisMapGenerator->thisField = malloc(sizeof(field));
//	thisMapGenerator->thisField->currentSpaceInventory = NULL;
	thisMapGenerator->thisField->isDark = 0;
	thisMapGenerator->thisField->id = 0;
	thisMapGenerator->thisField->playerCords = NULL;
	thisMapGenerator->thisField->playerLoS = 0;

	//fill in remaining spaces
	for(i = 0; i < 100; i++){
		for(j = 0; j < 100; j++){
			if(i < thisMapGenerator->thisField->totalX && j < thisMapGenerator->thisField->totalY){
				continue;
			}

			thisMapGenerator->thisField->grid[i][j] = makeMapGeneratorSpace(i, j);
		}
	}

	thisMapGenerator->cursorCords = malloc(sizeof(cord));
	thisMapGenerator->cursorCords->x = 0;
	thisMapGenerator->cursorCords->y = 0;
}

void enableMapGenerationSelectionMode(){
	thisMapGenerator->inSelectionMode = 1;

	thisMapGenerator->cursorCords->x = thisMapGenerator->cursor->x;
	thisMapGenerator->cursorCords->y = thisMapGenerator->cursor->y;
}

void disableMapGenerationSelectionMode(){
	thisMapGenerator->inSelectionMode = 0;

	thisMapGenerator->cursor->x = thisMapGenerator->cursorCords->x;
	thisMapGenerator->cursor->y = thisMapGenerator->cursorCords->y;
}

int inMapGenerationSelectionMode(){
	return thisMapGenerator->inSelectionMode;
}

void changeMapGeneratorHeight(int value){
	while(!tryGetFieldReadLock()){}

	int tmp = thisMapGenerator->thisField->totalY + value;

	if(tmp > 0 && tmp < 100){
		thisMapGenerator->thisField->totalY = tmp;
	}

	releaseFieldReadLock();
}

void changeMapGeneratorWidth(int value){
	while(!tryGetFieldReadLock()){}

	int tmp = thisMapGenerator->thisField->totalX + value;

	if(tmp > 0 && tmp < 100){
		thisMapGenerator->thisField->totalX = tmp;
	}

	releaseFieldReadLock();
}

void changeMapGeneratorSelectX(int value){
	int tmp = thisMapGenerator->selectedIndex + value;

	if(tmp >= 0 && tmp < thisMapGenerator->numCharacters){
		thisMapGenerator->selectedIndex = tmp;

		if(thisMapGenerator->selectedIndex < thisMapGenerator->selectionStartingHeight * thisMapGenerator->numCharactersPerRow){
			thisMapGenerator->selectionStartingHeight--;
		}else if(thisMapGenerator->selectedIndex > thisMapGenerator->selectionStartingHeight * thisMapGenerator->numCharactersPerRow  + thisMapGenerator->numCharactersPerRow * 5){
			thisMapGenerator->selectionStartingHeight++;
		}
	}
}

void changeMapGeneratorSelectY(int value){
	int tmp = thisMapGenerator->selectedIndex + thisMapGenerator->numCharactersPerRow * value;

	if(tmp >= 0 && tmp < thisMapGenerator->numCharacters){
		thisMapGenerator->selectedIndex = tmp;

		if(thisMapGenerator->selectedIndex < thisMapGenerator->selectionStartingHeight * thisMapGenerator->numCharactersPerRow){
			thisMapGenerator->selectionStartingHeight--;
		}else if(thisMapGenerator->selectedIndex > thisMapGenerator->selectionStartingHeight * thisMapGenerator->numCharactersPerRow  + thisMapGenerator->numCharactersPerRow * 5){
			thisMapGenerator->selectionStartingHeight++;
		}
	}
}

void rotateMapGenerationCharacter(HDC hdc, HDC hdcBuffer){
	thisMapGenerator->rotation = (thisMapGenerator->rotation + 1) % 4;
	thisMapGenerator->selectedCharacter->direction = thisMapGenerator->rotation;

	rotateAnimationFrames(hdc, hdcBuffer, thisMapGenerator->selectedCharacter->thisAnimationContainer->animations[0], thisMapGenerator->rotation);
}

int moveMapGeneratorCursor(int direction, shiftData * viewShift, RECT * rect){
	int newX = thisMapGenerator->cursor->x + xMoveChange(direction);
	int newY = thisMapGenerator->cursor->y + yMoveChange(direction);

	if(newX >= 0 && newX < thisMapGenerator->thisField->totalX && newY >=0 && newY < thisMapGenerator->thisField->totalY){
		thisMapGenerator->cursor->x = newX;
		thisMapGenerator->cursor->y = newY;

		tryUpdateXShift(viewShift, newX, rect->right - rect->left);
		tryUpdateYShift(viewShift, newY, rect->bottom - rect->top);
		return 1;
	}else{
		return 0;
	}
}

void setMapGenerationSpace(HDC hdc, HDC hdcBuffer){
	int x = thisMapGenerator->cursor->x;
	int y = thisMapGenerator->cursor->y;

	destroyCharacter(thisMapGenerator->thisField->grid[x][y]->background);

	thisMapGenerator->thisField->grid[x][y]->background = createCharacterFromAnimation(cloneAnimation(thisMapGenerator->selectedCharacter->thisAnimationContainer->animations[0]));
	thisMapGenerator->thisField->grid[x][y]->background->x = x;
	thisMapGenerator->thisField->grid[x][y]->background->y = y;
	thisMapGenerator->thisField->grid[x][y]->background->xOff = 0;
	thisMapGenerator->thisField->grid[x][y]->background->yOff = 0;
	thisMapGenerator->thisField->grid[x][y]->background->direction = thisMapGenerator->rotation;

	if(thisMapGenerator->rotation != 0){
		rotateAnimationFrames(hdc, hdcBuffer, thisMapGenerator->thisField->grid[x][y]->background->thisAnimationContainer->animations[0], thisMapGenerator->rotation);
	}
}

void setMapGenerationSelectedCharacter(){
	thisMapGenerator->selectedCharacter = thisMapGenerator->characterSet[thisMapGenerator->selectedIndex];
	thisMapGenerator->rotation = 0;
}

field * mapGeneratorField(){
	return thisMapGenerator->thisField;
}

void drawMapGeneratorCharacterSelection(HDC hdc, HDC hdcBuffer, RECT * prc){
	int i;
	int startingIndex = thisMapGenerator->selectionStartingHeight * thisMapGenerator->numCharactersPerRow;
	int endingIndex = startingIndex + thisMapGenerator->numCharactersPerRow * 5;
	int xOff = 0;
	int yOff = prc->bottom - (6 * 52);

	drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 15, yOff + 15, thisMapGenerator->characterSelectBackground);

	for(i = 0; i < thisMapGenerator->numCharacters; i++){
		if(i < startingIndex){
			continue;
		}

		if( i >= endingIndex){
			break;
		}

		drawUnboundAnimationByPixels(hdc, hdcBuffer, thisMapGenerator->characterSet[i], xOff, yOff, 0);

		if(i == thisMapGenerator->selectedIndex){
			drawUnboundAnimationByPixels(hdc, hdcBuffer, thisMapGenerator->cursor, xOff, yOff, 0);
		}

		xOff += 52;
		if(xOff >= thisMapGenerator->numCharactersPerRow * 52){
			xOff = 0;
			yOff += 52;
		}
	}
}

void drawMapGenerator(HDC hdc, RECT * prc, shiftData * viewShift){
	HDC hdcBuffer = CreateCompatibleDC(hdc);
	HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, prc->right, prc->bottom);
	HBITMAP hbmOldBuffer = SelectObject(hdcBuffer, hbmBuffer); //copy of hbmBuffer
	int index, xOff = 0, yOff = 0;

	while(!tryGetFieldReadLock()){}
	xOff = prc->right - thisMapGenerator->selectedCharacterBox->fixedWidth;
	yOff = prc->bottom - thisMapGenerator->selectedCharacterBox->fixedHeight;

	drawField(hdc, hdcBuffer, thisMapGenerator->thisField, viewShift);
	drawCharacterAnimation(hdc, hdcBuffer, thisMapGenerator->cursor, viewShift, 0);

	drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff, yOff, thisMapGenerator->selectedCharacterBox);
	drawUnboundAnimationByPixels(hdc, hdcBuffer, thisMapGenerator->selectedCharacter, xOff + 5, yOff + 5, 0);

	if(thisMapGenerator->inSelectionMode){
		drawMapGeneratorCharacterSelection(hdc, hdcBuffer, prc);
	}

	releaseFieldReadLock();

	BitBlt(hdc, 0, 0, prc->right, prc->bottom, hdcBuffer, 0, 0, SRCCOPY);

	SelectObject(hdcBuffer, hbmOldBuffer);
	DeleteDC(hdcBuffer);
	DeleteObject(hbmBuffer);
}

void writeGeneratedMapToFile(char * directory, char * fileName){
	int i,j,index;

	while(!tryGetFieldReadLock()){}
	while(!tryGetFieldWriteLock()){}

	char fullFileName[256];
	sprintf(fullFileName, "%s", directory);

	_mkdir(fullFileName);

	i += sprintf(fullFileName + strlen(fullFileName), "%s", fileName);

	FILE * FP = fopen(fullFileName, "w");

	//add transit.txt and id
	fprintf(FP, "%s\n",  "dummy.txt");
	fprintf(FP, "%s\n",  "0");

	for(i = 0; i < thisMapGenerator->thisField->totalY; i++){
		char line[2048];
		line[0] = '\0';
		index = 0;

		for(j = 0; j < thisMapGenerator->thisField->totalX; j++){
			character * tmpBackground = thisMapGenerator->thisField->grid[j][i]->background;
			index += sprintf(line + index, "%d,%d;", tmpBackground->thisAnimationContainer->animations[0]->imageID, tmpBackground->direction);
		}

		fprintf(FP, "%s\n",  line);
	}

	fclose(FP);

	releaseFieldWriteLock();
	releaseFieldReadLock();
}
