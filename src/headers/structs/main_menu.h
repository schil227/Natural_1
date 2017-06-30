/*
 * main_menu.h
 *
 *  Created on: Jun 29, 2017
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_MAIN_MENU_H_
#define SRC_HEADERS_STRUCTS_MAIN_MENU_H_

#include"../individual_pub_methods.h"

typedef enum {MENU_TITLE, MENU_CREATE, MENU_CREATE_ABILITY, MENU_LOAD} mainMenuMode;

typedef enum {TITLE_NEW_GAME, TITLE_LOAD, TITLE_EXIT} titleScreenOption;

typedef struct{
	int titleOptionsDisplayed;
	titleScreenOption selectedOption;

	fixedCharacter * titleScreen;
	fixedCharacter * options;
}titleMenu;

typedef struct{
	int inMenuMode;
	mainMenuMode currentMenu;

	titleMenu * title;

	fixedCharacter * creationScreen;
	fixedCharacter * rightSelectArrow;
	fixedCharacter * leftSelectArrow;
	individual * newPlayer;
}mainMenu;

#endif /* SRC_HEADERS_STRUCTS_MAIN_MENU_H_ */
