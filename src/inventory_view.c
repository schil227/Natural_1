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
	thisInventoryView->selectArrow = createCharacter(3001, RGB(255,0,255), x, y);
	thisInventoryView->scrollArrow = createCharacter(3002, RGB(255,0,255), x, y);
	thisInventoryView->itemFrame = createCharacter(3003, RGB(255,0,255), x, y);
	thisInventoryView->selectedItem = NULL;
	thisInventoryView->slotsPerScreen = slotsPerScreen;

	for(i = 0; i < 4; i++){
		thisInventoryView->viewedItems[i] = NULL;
	}

	return 0;
}

void refreshInventory(inventory * playerInventory){
	int i,j=0,availableSlots;
	availableSlots = min(thisInventoryView->playerItems->inventorySize, thisInventoryView->slotsPerScreen);

	thisInventoryView->playerItems = playerInventory;

	for(i = 0; i < 40; i++){
		if(j == availableSlots){
			break;
		}

		if(thisInventoryView->playerItems->inventoryArr[i] != NULL){
			thisInventoryView->viewedItems[j] =  thisInventoryView->playerItems->inventoryArr[i];
			j++;
		}
	}

}

void drawInventoryView(HDC hdc, HDC hdcBuffer, shiftData * viewShift){
	int i;
	drawUnboundCharacterByPixels(hdc, hdcBuffer,
			thisInventoryView->inventoryBackground->x,
			thisInventoryView->inventoryBackground->y,
			thisInventoryView->inventoryBackground, viewShift);

	for(i = 0; i < thisInventoryView->slotsPerScreen; i++){
		if(thisInventoryView->viewedItems[i] != NULL){
			drawUnboundCharacterByPixels(hdc, hdcBuffer,
								thisInventoryView->inventoryBackground->x+40,
								thisInventoryView->inventoryBackground->y + 50+50*i,
								thisInventoryView->itemFrame,
								viewShift);
			drawUnboundCharacterByPixels(hdc, hdcBuffer,
					thisInventoryView->inventoryBackground->x+40,
					thisInventoryView->inventoryBackground->y + 50+50*i,
					thisInventoryView->viewedItems[i]->itemCharacter,
					viewShift);
		}
	}
}

