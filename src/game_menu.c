/*
 * game_menu.c
 *
 *  Created on: Jul 15, 2017
 *      Author: Adrian
 */

#include "./headers/game_menu_pub_methods.h"

static gameMenu * thisGameMenu;

void initGameMenu(){
	thisGameMenu = malloc(sizeof(gameMenu));

	thisGameMenu->inGameMenuMode = 0;
	thisGameMenu->selectedMode = GAME_MENU_CONTINUE;
	thisGameMenu->menu = createCharacter(1420, RGB(255,0,255), 0, 0);
	thisGameMenu->fieldOutline = createCharacter(1413, RGB(255,0,255), 0, 0);
}

void enableGameMenuMode(){
	thisGameMenu->inGameMenuMode = 1;
	thisGameMenu->selectedMode = GAME_MENU_CONTINUE;
}

void disableGameMenuMode(){
	thisGameMenu->inGameMenuMode = 0;
}

int inGameMenuMode(){
	if(thisGameMenu != NULL){
		return thisGameMenu->inGameMenuMode;
	}

	return 0;
}

void gameMenuInterpretUp(){
	switch(thisGameMenu->selectedMode){
	case GAME_MENU_CONTINUE:
		thisGameMenu->selectedMode = GAME_MENU_MAINMENU;
		break;
	case GAME_MENU_LOAD:
		thisGameMenu->selectedMode = GAME_MENU_CONTINUE;
		break;
	case GAME_MENU_OPTIONS:
		thisGameMenu->selectedMode = GAME_MENU_LOAD;
		break;
	case GAME_MENU_MAINMENU:
		thisGameMenu->selectedMode = GAME_MENU_OPTIONS;
		break;
	}
}

void gameMenuInterpretDown(){
	switch(thisGameMenu->selectedMode){
	case GAME_MENU_CONTINUE:
		thisGameMenu->selectedMode = GAME_MENU_LOAD;
		break;
	case GAME_MENU_LOAD:
		thisGameMenu->selectedMode = GAME_MENU_OPTIONS;
		break;
	case GAME_MENU_OPTIONS:
		thisGameMenu->selectedMode = GAME_MENU_MAINMENU;
		break;
	case GAME_MENU_MAINMENU:
		thisGameMenu->selectedMode = GAME_MENU_CONTINUE;
		break;
	}
}

void gameMenuInterpretEscape(){
	disableGameMenuMode();
}

void gameMenuInterpretEnter(){
	switch(thisGameMenu->selectedMode){
	case GAME_MENU_CONTINUE:
		disableGameMenuMode();
		break;
	case GAME_MENU_LOAD:
		setupLoadMode();
		enableMainMenuMode(3, 1);
		disableGameMenuMode();
		break;
	case GAME_MENU_OPTIONS:
		break;
	case GAME_MENU_MAINMENU:
		enableMainMenuMode(0, 1);
		disableGameMenuMode();
		break;
	}
}

void drawGameMenu(HDC hdc, HDC hdcBuffer, RECT * rect){
	int xOff = 0, yOff = 0;
	RECT textBoxRect;

	xOff = max((rect->right / 2) - (thisGameMenu->menu->fixedWidth / 2) , 0);
	yOff = max((rect->bottom / 2) - (thisGameMenu->menu->fixedHeight / 2) , 0);

	drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff, yOff, thisGameMenu->menu);

	switch(thisGameMenu->selectedMode){
	case GAME_MENU_CONTINUE:
		drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 25, yOff + 23, thisGameMenu->fieldOutline);
		break;
	case GAME_MENU_LOAD:
		drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 25, yOff + 23 + 54, thisGameMenu->fieldOutline);
		break;
	case GAME_MENU_OPTIONS:
		drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 25, yOff + 23 + 54 * 2, thisGameMenu->fieldOutline);
		break;
	case GAME_MENU_MAINMENU:
		drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 25, yOff + 23 + 54 * 3, thisGameMenu->fieldOutline);
		break;
	}

	HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, DEFAULT_QUALITY, 0, "System");
	HFONT oldFont = SelectObject(hdcBuffer, hFont);
	SetTextColor(hdcBuffer, RGB(255, 200, 0));
	SetBkMode(hdcBuffer, TRANSPARENT);

	drawNewGameFormText(hdcBuffer, &textBoxRect, xOff + 25 + 81, yOff + 27, "Continue");
	drawNewGameFormText(hdcBuffer, &textBoxRect, xOff + 25 + 81, yOff + 27 + 54, "Load");
	drawNewGameFormText(hdcBuffer, &textBoxRect, xOff + 25 + 81, yOff + 27 + 54 * 2, "Options");
	drawNewGameFormText(hdcBuffer, &textBoxRect, xOff + 25 + 81, yOff + 27 + 54 * 3, "Main Menu");

	SelectObject(hdcBuffer, oldFont);
	DeleteObject(hFont);
	SetTextColor(hdcBuffer, RGB(0, 0, 0));
}
