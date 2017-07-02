/*
 * main_menu.c
 *
 *  Created on: Jun 29, 2017
 *      Author: Adrian
 */

#include "./headers/main_menu_pub_methods.h"

static mainMenu * thisMainMenu;

void initMainMenu(int inMenuMode){
	thisMainMenu = malloc(sizeof(mainMenu));
	thisMainMenu->title = malloc(sizeof(titleMenu));
	thisMainMenu->newGame = malloc(sizeof(newGameMenu));
	thisMainMenu->inMenuMode = inMenuMode;
	thisMainMenu->currentMenu = MENU_TITLE;

	thisMainMenu->leftSelectArrow = createCharacter(1402, RGB(255,0,255), 0, 0);
	thisMainMenu->rightSelectArrow = createCharacter(1504, RGB(255,0,255), 0, 0);

	thisMainMenu->title->titleScreen = createCharacter(1400, RGB(255,0,255), 0, 0);
	thisMainMenu->title->options = createCharacter(1403, RGB(255,0,255), 0, 0);
	thisMainMenu->title->titleOptionsDisplayed = 0;
	thisMainMenu->title->selectedOption = TITLE_NEW_GAME;
	thisMainMenu->rightSelectArrow->y = 13;

	thisMainMenu->newGame->creationScreen = createCharacter(1401, RGB(255,0,255), 0, 0);
	thisMainMenu->newGame->newPlayer = getIndividualFromRegistry(0);
}

void disableMainMenuMode(){
	thisMainMenu->inMenuMode = 0;
}

void enableMainMenuMode(){
	thisMainMenu->inMenuMode = 1;
}

int inMainMenuMode(){
	if(thisMainMenu != NULL){
		return thisMainMenu->inMenuMode;
	}

	return 0;
}

void mainMenuTitleUp(){
	if(!thisMainMenu->title->titleOptionsDisplayed){
		return;
	}

	switch(thisMainMenu->title->selectedOption){
	case TITLE_NEW_GAME:
		thisMainMenu->title->selectedOption = TITLE_EXIT;
		thisMainMenu->rightSelectArrow->y = 91;
		break;
	case TITLE_LOAD:
		thisMainMenu->title->selectedOption = TITLE_NEW_GAME;
		thisMainMenu->rightSelectArrow->y = 13;
		break;
	case TITLE_EXIT:
		thisMainMenu->title->selectedOption = TITLE_LOAD;
		thisMainMenu->rightSelectArrow->y = 52;
		break;
	}
}

void mainMenuTitleDown(){
	if(!thisMainMenu->title->titleOptionsDisplayed){
		return;
	}

	switch(thisMainMenu->title->selectedOption){
	case TITLE_NEW_GAME:
		thisMainMenu->title->selectedOption = TITLE_LOAD;
		thisMainMenu->rightSelectArrow->y = 52;
		break;
	case TITLE_LOAD:
		thisMainMenu->title->selectedOption = TITLE_EXIT;
		thisMainMenu->rightSelectArrow->y = 91;
		break;
	case TITLE_EXIT:
		thisMainMenu->title->selectedOption = TITLE_NEW_GAME;
		thisMainMenu->rightSelectArrow->y = 13;
		break;
	}
}

void mainMenuTitleSelect(){
	if(thisMainMenu->title->titleOptionsDisplayed){
		switch(thisMainMenu->title->selectedOption){
		case TITLE_NEW_GAME:
			thisMainMenu->currentMenu = MENU_NEW_GAME;
			break;
		case TITLE_LOAD:
			disableMainMenuMode();
			break;
		case TITLE_EXIT:
			PostQuitMessage(0);
			break;
		}
	}else{
		thisMainMenu->title->titleOptionsDisplayed = 1;
	}
}

void drawMainMenuTitle(HDC hdc, HDC hdcBuffer, RECT * rect){
	int xOff = 0, yOff = 0;

	xOff = max((rect->right / 2) - (thisMainMenu->title->titleScreen->fixedWidth / 2) , 0);
	yOff = max((rect->bottom / 2) - (thisMainMenu->title->titleScreen->fixedHeight / 2) , 0);

	drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff, yOff, thisMainMenu->title->titleScreen);

	if(thisMainMenu->title->titleOptionsDisplayed){
		xOff += 90;
		yOff += 375;

		drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff, yOff, thisMainMenu->title->options);

		drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + thisMainMenu->rightSelectArrow->x - 9, yOff + thisMainMenu->rightSelectArrow->y, thisMainMenu->rightSelectArrow);
	}
}

void drawMainMenu(HDC hdc, HDC hdcBuffer, RECT * rect){
	switch(thisMainMenu->currentMenu){
		case MENU_TITLE:
			drawMainMenuTitle(hdc, hdcBuffer, rect);
	}
}
