/*
 * inventory_view.c
 *
 *  Created on: Nov 3, 2015
 *      Author: Adrian
 */
#include"./headers/inventory_view_pub_methods.h"

inventoryView * thisInventoryView;

int initThisInventoryView(int imageID, int x, int y, int slotsPerScreen, inventory * playerInventory){
	int i;

	thisInventoryView = malloc(sizeof(inventoryView));
	thisInventoryView->playerItems = playerInventory;
	thisInventoryView->inventoryBackground = createCharacter(imageID, RGB(255,0,255), x, y);
	thisInventoryView->selectArrow = createCharacter(1504, RGB(255,0,255), x, y);
	thisInventoryView->scrollUpArrow = createCharacter(1505, RGB(255,0,255), x, y);
	thisInventoryView->scrollDownArrow = createCharacter(1507, RGB(255,0,255), x, y);
	thisInventoryView->itemFrame = createCharacter(1506, RGB(255,0,255), x, y);
	thisInventoryView->slotsPerScreen = slotsPerScreen;
	thisInventoryView->selectedItemIndex = 0;
	thisInventoryView->viewMode = 0;
	thisInventoryView->buyMode = 0;
	thisInventoryView->pickpocketMode = 0;
	thisInventoryView->fieldGetMode = 0;

	for(i = 0; i < slotsPerScreen; i++){
		thisInventoryView->viewedItems[i] = NULL;
	}

	return 0;
}

void destroyTheInventoryView(){
	destroyFixedCharacter(thisInventoryView->inventoryBackground);
	destroyFixedCharacter(thisInventoryView->itemFrame);
	destroyFixedCharacter(thisInventoryView->scrollDownArrow);
	destroyFixedCharacter(thisInventoryView->scrollUpArrow);
	destroyFixedCharacter(thisInventoryView->selectArrow);

	free(thisInventoryView);
}

int inInventoryViewMode(){
	if(thisInventoryView != NULL){
		return thisInventoryView->viewMode;
	}

	return 0;
}

void enableInventoryViewMode(inventory * individualInventory){
	thisInventoryView->viewMode = 1;
	thisInventoryView->playerItems = individualInventory;
	refreshInventory(individualInventory);
}

void disableInventoryViewMode(){
	thisInventoryView->viewMode = 0;
}

void refreshInventory(inventory * playerInventory){
	int i,j=0,availableSlots;
	int isPickPocketMode = inPickPocketMode();

	if(thisInventoryView->viewedItems[0] != NULL){
		for(i = 0; i < thisInventoryView->slotsPerScreen; i++){
			thisInventoryView->viewedItems[i] = NULL;
		}
	}

	availableSlots = min(thisInventoryView->playerItems->inventorySize, thisInventoryView->slotsPerScreen);

	thisInventoryView->playerItems = playerInventory;

	for(i = 0; i < thisInventoryView->playerItems->MAX_ITEMS; i++){
		if(j == availableSlots){
			break;
		}

		if(thisInventoryView->playerItems->inventoryArr[i] != NULL && (!isPickPocketMode || (isPickPocketMode && !thisInventoryView->playerItems->inventoryArr[i]->isEquipt))){
			thisInventoryView->viewedItems[j] = thisInventoryView->playerItems->inventoryArr[i];
			j++;
		}
	}

	if(thisInventoryView->selectedItemIndex != 0){

		while(thisInventoryView->selectedItemIndex > 0){

			if(thisInventoryView->viewedItems[thisInventoryView->selectedItemIndex] != NULL){
				break;
			}

			thisInventoryView->selectedItemIndex--;
		}

	}else if(thisInventoryView->viewedItems[0] != NULL){
		thisInventoryView->selectedItemIndex = 0;
	}

}

void drawInventoryView(HDC hdc, HDC hdcBuffer, shiftData * viewShift){
	int i;

	RECT textBoxRect;
		textBoxRect.bottom = thisInventoryView->inventoryBackground->fixedHeight + thisInventoryView->inventoryBackground->y;
		textBoxRect.top =  thisInventoryView->inventoryBackground->y+60;
		textBoxRect.right = thisInventoryView->inventoryBackground->fixedWidth + thisInventoryView->inventoryBackground->x + 10;
		textBoxRect.left = thisInventoryView->inventoryBackground->x + 95;

	//draw inventory view
	drawUnboundCharacterByPixels(hdc, hdcBuffer,
		thisInventoryView->inventoryBackground->x,
		thisInventoryView->inventoryBackground->y,
		thisInventoryView->inventoryBackground);

	for(i = 0; i < thisInventoryView->slotsPerScreen; i++){
		if(thisInventoryView->viewedItems[i] != NULL){
			char itemStr[80];
			buildItemStr(itemStr, thisInventoryView->viewedItems[i]);

//			drawUnboundAnimationByPixels(hdc, hdcBuffer, thisInventoryView->itemFrame, viewShift,
//					thisInventoryView->inventoryBackground->x+40, thisInventoryView->inventoryBackground->y + 50+50*i, 0);

			drawUnboundCharacterByPixels(hdc, hdcBuffer,
					thisInventoryView->inventoryBackground->x + 40,
					thisInventoryView->inventoryBackground->y + 40 + 60 * i,
					thisInventoryView->itemFrame);

			drawUnboundAnimationByPixels(hdc, hdcBuffer, thisInventoryView->viewedItems[i]->itemCharacter,
					thisInventoryView->inventoryBackground->x+15, thisInventoryView->inventoryBackground->y + 15 + 60*i, 1);

//			//draw item frame
//			drawUnboundCharacterByPixels(hdc, hdcBuffer,
//				thisInventoryView->inventoryBackground->x+40,
//				thisInventoryView->inventoryBackground->y + 50+50*i,
//				thisInventoryView->itemFrame,
//				viewShift);
//
//			//draw item
//			drawUnboundCharacterByPixels(hdc, hdcBuffer,
//				thisInventoryView->inventoryBackground->x+40,
//				thisInventoryView->inventoryBackground->y + 50+50*i,
//				thisInventoryView->viewedItems[i]->itemCharacter,
//				viewShift);

			//draw item text
			SetTextColor(hdcBuffer, RGB(255, 200, 0));
			SetBkMode(hdcBuffer, TRANSPARENT);
			DrawText(hdcBuffer, itemStr, -1, &textBoxRect, DT_SINGLELINE); //thisInventoryView->viewedItems[i]->name

			//draw item cost
			if(thisInventoryView->buyMode){
				char * cost[16];
				sprintf(cost, "$%d", thisInventoryView->viewedItems[i]->price);
				textBoxRect.left = textBoxRect.left + 125;
				DrawText(hdcBuffer, cost, -1, &textBoxRect, DT_SINGLELINE);
				textBoxRect.left = textBoxRect.left - 125;
			}

			SetTextColor(hdcBuffer, RGB(0, 0, 0));
			textBoxRect.top = textBoxRect.top + 60;

			//draw selection arrow
			if(thisInventoryView->selectedItemIndex == i){
				drawUnboundCharacterByPixels(hdc, hdcBuffer,
					thisInventoryView->inventoryBackground->x+20,
					thisInventoryView->inventoryBackground->y + 62+60*i,
					thisInventoryView->selectArrow);
			}
		}
	}

	if(canScrollDown()){
		drawUnboundCharacterByPixels(hdc, hdcBuffer,
							thisInventoryView->inventoryBackground->x+100,
							thisInventoryView->inventoryBackground->y +
								thisInventoryView->inventoryBackground->fixedHeight - 30,
							thisInventoryView->scrollDownArrow);
	}

	if(canScrollUp()){
		drawUnboundCharacterByPixels(hdc, hdcBuffer,
							thisInventoryView->inventoryBackground->x+100,
							thisInventoryView->inventoryBackground->y + 15,
							thisInventoryView->scrollUpArrow);
	}
}

void buildItemStr(char itemStr[80], item * theItem){
	strcpy(itemStr, theItem->name);

	if((theItem->type == 'w' || theItem->type == 'a') && theItem->isEquipt){
		strcat(itemStr, " (E)");
	}
}

//is there a next item?
int canSelectNextItemDown(){
	int totalItems;

	//basecase: there are no items
	if(thisInventoryView->viewedItems[0] == NULL){
		return 0;
	}

	if(selectedIndexIsntLastPlayerItem()){
		return 1;
	}else{
		return 0;
	}
}

int canSelectPreviousItemUp(){
	int totalItems;

	//basecase: there are no items
	if(thisInventoryView->viewedItems[0] == NULL){
		return 0;
	}

	if(selectedIndexIsntFirstPlayerItem()){
		return 1;
	}else{
		return 0;
	}
}

int canScrollDown(){
	int i, index, nextItem = 0;
	item * tmpItem;
	if(thisInventoryView->viewedItems[0] == NULL || thisInventoryView->slotsPerScreen >= thisInventoryView->playerItems->inventorySize ){
		return 0;
	}

	index = thisInventoryView->slotsPerScreen-1;

	tmpItem = thisInventoryView->viewedItems[index];

	for(i = 0; i < thisInventoryView->playerItems->MAX_ITEMS; i++){
		if(nextItem && thisInventoryView->playerItems->inventoryArr[i] != NULL){
			return 1;
		}

		if(tmpItem == thisInventoryView->playerItems->inventoryArr[i]){
			nextItem = 1;
		}
	}

	return 0;
}

int canScrollUp(){
	int i, nextItem = 0;
	item * tmpItem;
	if(thisInventoryView->viewedItems[0] == NULL || thisInventoryView->slotsPerScreen >= thisInventoryView->playerItems->inventorySize ){
		return 0;
	}

	tmpItem = thisInventoryView->viewedItems[0];

	for(i = 40; i >= 0; i--){
		if(nextItem && thisInventoryView->playerItems->inventoryArr[i] != NULL){
			return 1;
		}

		if(tmpItem == thisInventoryView->playerItems->inventoryArr[i]){
			nextItem = 1;
		}
	}

	return 0;
}
int selectedIndexIsntFirstPlayerItem(){
	int i;

	for(i = 0; i < thisInventoryView->playerItems->MAX_ITEMS; i++){
		if(thisInventoryView->playerItems->inventoryArr[i] != NULL){
			if( thisInventoryView->playerItems->inventoryArr[i]  ==
					thisInventoryView->viewedItems[thisInventoryView->selectedItemIndex]){
				return 0;
			}else{
				return 1;
			}

		}
	}
	return 0;
}

int selectedIndexIsntLastPlayerItem(){
	int i,nextItem = 0;

	for(i = 0; i < thisInventoryView->playerItems->MAX_ITEMS; i++){
		if(thisInventoryView->playerItems->inventoryArr[i] != NULL){
			if(nextItem && thisInventoryView->playerItems->inventoryArr[i] != NULL){
				return 1;
			}

			if( thisInventoryView->playerItems->inventoryArr[i]  ==
					thisInventoryView->viewedItems[thisInventoryView->selectedItemIndex]){
				nextItem = 1;
			}

		}
	}
	return 0;
}

void selectNextItemDown(){
	if(canSelectNextItemDown()){
		//selecting last element
		if(thisInventoryView->selectedItemIndex == thisInventoryView->slotsPerScreen-1){
			shiftItemsUp();
		}else{
			thisInventoryView->selectedItemIndex++;
		}
	}
}

void selectPreviousItemUp(){
	if(canSelectPreviousItemUp()){
		//selecting last element
		if(thisInventoryView->selectedItemIndex == 0){
			shiftItemsDown();
		}else{
			thisInventoryView->selectedItemIndex--;
		}
	}
}

void shiftPreviousIfNecessairy(){
	if(canSelectPreviousItemUp()){
		//selecting last element
		if(thisInventoryView->selectedItemIndex == 0){
			shiftItemsDown();
		}else{
			thisInventoryView->selectedItemIndex--;
		}
	}
}

void shiftItemsDown(){
	int i, getNextItem = 0;
	item * tmpItem;

	for (i = thisInventoryView->slotsPerScreen - 1; i > 0; i--) {
		thisInventoryView->viewedItems[i] = thisInventoryView->viewedItems[i - 1];
	}

	tmpItem = thisInventoryView->viewedItems[0];

	for (i = 40; i >= 0; i--) {
		if (getNextItem && thisInventoryView->playerItems->inventoryArr[i] != NULL) {
			thisInventoryView->viewedItems[0] = thisInventoryView->playerItems->inventoryArr[i];
			break;
		}
		if (tmpItem == thisInventoryView->playerItems->inventoryArr[i]) {
			getNextItem = 1;
		}
	}
}

void shiftItemsUp(){
	int i, getNextItem = 0, lastIndex;
	item * tmpItem;

	lastIndex = thisInventoryView->slotsPerScreen - 1;

	for (i = 0; i <= thisInventoryView->slotsPerScreen - 2; i++) {
		thisInventoryView->viewedItems[i] = thisInventoryView->viewedItems[i + 1];
	}

	tmpItem = thisInventoryView->viewedItems[lastIndex];

	for (i = 0; i < 40; i++) {
		if (getNextItem && thisInventoryView->playerItems->inventoryArr[i] != NULL) {
			thisInventoryView->viewedItems[lastIndex] = thisInventoryView->playerItems->inventoryArr[i];
			break;
		}
		if (tmpItem == thisInventoryView->playerItems->inventoryArr[i]) {
			getNextItem = 1;
		}
	}
}

item * getSelectedItem(){
	//check if item array is empty
	if(thisInventoryView->viewedItems[0] == 0){
		return NULL;
	}else{
		return thisInventoryView->viewedItems[thisInventoryView->selectedItemIndex];
	}
}

void enableInventoryPickPocketMode(){
	thisInventoryView->pickpocketMode = 1;
}

void disableInventoryPickPocketMode(){
	thisInventoryView->pickpocketMode = 0;
}

int inPickPocketMode(){
	return thisInventoryView->pickpocketMode;
}

void enableInventoryBuyMode(){
	thisInventoryView->buyMode = 1;
}

void disableInventoryBuyMode(){
	thisInventoryView->buyMode = 0;
}

int inBuyMode(){
	return thisInventoryView->buyMode;
}

void enableItemFieldGetMode(){
	thisInventoryView->fieldGetMode = 1;
}

void disableItemFieldGetMode(){
	thisInventoryView->fieldGetMode = 0;
}

int inFieldGetMode(){
	return thisInventoryView->fieldGetMode;
}
