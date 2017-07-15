/*
 * game_menu.h
 *
 *  Created on: Jul 15, 2017
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_GAME_MENU_H_
#define SRC_HEADERS_STRUCTS_GAME_MENU_H_
#include "../character_pub_methods.h"

typedef enum {GAME_MENU_CONTINUE, GAME_MENU_LOAD, GAME_MENU_OPTIONS, GAME_MENU_MAINMENU} gameMenuMode;

typedef struct{
	int inGameMenuMode;
	gameMenuMode selectedMode;

	fixedCharacter * menu;
	fixedCharacter * fieldOutline;
} gameMenu;

#endif /* SRC_HEADERS_STRUCTS_GAME_MENU_H_ */
