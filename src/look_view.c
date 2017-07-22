/*
 * look_view.c
 *
 *  Created on: May 3, 2017
 *      Author: Adrian
 */
#include "./headers/look_view_pub_methods.h"

static lookView * thisLookView;

void initLookView(int imageID, int enterImageID){
	thisLookView = malloc(sizeof(lookView));

	thisLookView->inLookMode = 0;
	thisLookView->inLookScrollMode = 0;
	thisLookView->currentLookDataIndex = 0;
	thisLookView->numLookData = 0;
	thisLookView->MAX_LOOKDATA = 50;

	thisLookView->mainWindow = createCharacter(imageID, RGB(255,0,255), 0, 0);
	thisLookView->upScrollArrow = createCharacter(1505, RGB(255,0,255), 0, 0);
	thisLookView->downScrollArrow = createCharacter(1507, RGB(255,0,255), 0, 0);
	thisLookView->enterImage = createCharacter(enterImageID, RGB(255,0,255), 0, 0);
	thisLookView->frame = createCharacter(1506, RGB(255,0,255), 0, 0);
}

void destroyLookView(){
	int i;

	for(i = 0; i < thisLookView->numLookData; i++){
		free(thisLookView->thisLookData[i]);
	}

	destroyFixedCharacter(thisLookView->mainWindow);
	destroyFixedCharacter(thisLookView->upScrollArrow);
	destroyFixedCharacter(thisLookView->downScrollArrow);
	destroyFixedCharacter(thisLookView->enterImage);
	destroyFixedCharacter(thisLookView->frame);

	free(thisLookView);
}

int inLookMode(){
	return thisLookView->inLookMode;
}

void enableLookMode(){
	thisLookView->inLookMode = 1;
}

void disableLookMode(){
	thisLookView->inLookMode = 0;
}

int inLookViewScrollMode(){
	if(thisLookView != NULL){
		return thisLookView->inLookScrollMode;
	}

	return 0;
}

void enableLookScrollMode(){
	thisLookView->inLookScrollMode = 1;
}

void disableLookScrollMode(){
	thisLookView->inLookScrollMode = 0;
	thisLookView->currentLookDataIndex = 0;
}

void moveLookIndexUp(){
	if(thisLookView->currentLookDataIndex > 0){
		thisLookView->currentLookDataIndex--;
	}
}

void moveLookIndexDown(){
	if(thisLookView->numLookData > 3 && thisLookView->currentLookDataIndex < thisLookView->numLookData - 3){
		thisLookView->currentLookDataIndex++;
	}
}

void drawLookData(HDC hdc, HDC hdcBuffer, RECT * rect, int top, int left){
	int i;
	int xOff = left + 15;
	int yOff = thisLookView->enterImage->fixedHeight + 2;

	RECT textBoxRect;

	for(i = 0; i < min(thisLookView->numLookData, 3); i++){
		int index = i + thisLookView->currentLookDataIndex;
		int adjustedY = (top + (yOff + i*75 + 5));

		textBoxRect.top = adjustedY + 40;
		textBoxRect.bottom = textBoxRect.top + 30;
		textBoxRect.left = xOff + 100;
		textBoxRect.right = textBoxRect.left + 200;

		switch(thisLookView->thisLookData[index]->thisType){
			case LOOK_INDIVIDUAL:
				drawUnboundCharacterByPixels(hdc, hdcBuffer,
									xOff + 25,
									adjustedY + 25,
									thisLookView->frame);

				drawIndividualDefaultByPixels(hdc, hdcBuffer, thisLookView->thisLookData[index]->thisIndividual, xOff, adjustedY, 0);
				break;
			case LOOK_ITEM:
			case LOOK_INTERACTABLE:
				drawUnboundCharacterByPixels(hdc, hdcBuffer,
									xOff + 25,
									adjustedY + 25,
									thisLookView->frame);

				drawUnboundAnimationByPixels(hdc, hdcBuffer, thisLookView->thisLookData[index]->thisCharacter, xOff, adjustedY, 0);
				break;
			case LOOK_SPACE:
				drawUnboundAnimationByPixels(hdc, hdcBuffer, thisLookView->thisLookData[index]->thisCharacter, xOff, adjustedY, 0);
				break;
		}

		//neatly print text next to it
		SetTextColor(hdcBuffer, RGB(255, 200, 0));
		SetBkMode(hdcBuffer, TRANSPARENT);
		DrawText(hdcBuffer, thisLookView->thisLookData[index]->description, -1, &textBoxRect, DT_SINGLELINE);
		SetTextColor(hdcBuffer, RGB(0, 0, 0));
	}
}

void drawLookView(HDC hdc, HDC hdcBuffer, RECT * rect){
	int left = rect->right - thisLookView->mainWindow->fixedWidth;
	int top = rect->bottom - thisLookView->mainWindow->fixedHeight;

	HDC hdcMem = CreateCompatibleDC(hdc);

	SelectObject(hdcMem, thisLookView->mainWindow->fixedImage);

	BitBlt(hdcBuffer,
			left,top,
			rect->right, thisLookView->mainWindow->fixedHeight,
			hdcMem,
			0,0,
			SRCCOPY);

	if(!thisLookView->inLookScrollMode){
		SelectObject(hdcMem, thisLookView->enterImage->fixedImage);

		BitBlt(hdcBuffer, left, top,
				thisLookView->enterImage->fixedWidth, thisLookView->enterImage->fixedHeight,
				hdcMem, 0, 0, SRCCOPY);
	}

	//draw down arrow
	if(thisLookView->currentLookDataIndex + 3 < thisLookView->numLookData){
		drawUnboundCharacterByPixels(hdc, hdcBuffer,
				left + 40,
				rect->bottom - (thisLookView->downScrollArrow->fixedHeight + 10),
				thisLookView->downScrollArrow);
	}

	//draw up arrow
	if(thisLookView->currentLookDataIndex > 0){
		drawUnboundCharacterByPixels(hdc, hdcBuffer,
				left + 40,
				top + (thisLookView->upScrollArrow->fixedHeight + 7),
				thisLookView->upScrollArrow);
	}

	DeleteDC(hdcMem);

	if(thisLookView->numLookData > 0){
		drawLookData(hdc, hdcBuffer, rect, top, left);
	}
}

void clearLookData(){
	int i;

	for(i = 0; i < thisLookView->numLookData; i++){
		free(thisLookView->thisLookData[i]);
		thisLookView->thisLookData[i] = NULL;
	}

	thisLookView->numLookData = 0;
	thisLookView->currentLookDataIndex = 0;
}

void addLookDataToInstance(lookType thisType, character * thisCharacter, individual * thisIndividual, char * description){
	lookData * thisLookData;

	if(thisLookView->numLookData >= thisLookView->MAX_LOOKDATA){
		return;
	}

	thisLookData = malloc(sizeof(lookData));
	thisLookData->thisType = thisType;
	thisLookData->thisCharacter = thisCharacter;
	thisLookData->thisIndividual = thisIndividual;
	strcpy(thisLookData->description, description);

	thisLookView->thisLookData[thisLookView->numLookData] = thisLookData;
	thisLookView->numLookData++;
}

void populateLookDataInstance(field * thisField, individual * player, int x, int y){
	int i, darkLoS = 0;

	while(!tryGetLookReadLock()){}
	while(!tryGetLookWriteLock()){}

	clearLookData();

	if(thisField->isDark){
		darkLoS = getAttributeSum(player, "darkLoS");
	}

	individual * tmpIndividual = getIndividualFromField(thisField, x, y);
	populateCurrentSpaceInventory(thisField, x, y);

	if(tmpIndividual != NULL && (!thisField->isDark || darkLoS >= max(abs(player->playerCharacter->x - x), abs(player->playerCharacter->y - y)))){
		addLookDataToInstance(LOOK_INDIVIDUAL, NULL, tmpIndividual, tmpIndividual->name);
	}

	item * tmpItem;
	if(!thisField->isDark || darkLoS >= max(abs(player->playerCharacter->x - x), abs(player->playerCharacter->y - y))){
		for(i = 0; i < thisField->currentSpaceInventory->inventorySize; i++){
			tmpItem = thisField->currentSpaceInventory->inventoryArr[i];

			if(tmpItem != NULL){
				addLookDataToInstance(LOOK_ITEM, tmpItem->itemCharacter, NULL, tmpItem->name);
			}
		}
	}

	space * tmpSpace = getSpaceFromField(thisField, x, y);

	if(tmpSpace != NULL){

		if(tmpSpace->interactableObject != NULL && tmpSpace->interactableObject->isEnabled && tmpSpace->interactableObject->shouldDraw){
			addLookDataToInstance(LOOK_INTERACTABLE, tmpSpace->interactableObject->thisCharacter, NULL, tmpSpace->interactableObject->lookDescription);
		}

		int id = tmpSpace->background->thisAnimationContainer->animations[tmpSpace->background->thisAnimationContainer->defaultAnimation]->imageID;
		char * spaceDescription = getDescriptionFromID(id);

		if(spaceDescription != NULL){
			addLookDataToInstance(LOOK_SPACE, tmpSpace->background, NULL, spaceDescription);
		}
	}

	releaseLookWriteLock();
	releaseLookReadLock();
}
