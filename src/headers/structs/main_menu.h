/*
 * main_menu.h
 *
 *  Created on: Jun 29, 2017
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_MAIN_MENU_H_
#define SRC_HEADERS_STRUCTS_MAIN_MENU_H_

#include"../individual_pub_methods.h"

typedef enum {MENU_TITLE, MENU_NEW_GAME, MENU_NEW_GAME_ABILITY, MENU_LOAD} mainMenuMode;
typedef enum {TITLE_NEW_GAME, TITLE_LOAD, TITLE_EXIT} titleScreenOption;
typedef enum {CREATE_NAME, CREATE_SPREAD, CREATE_STR, CREATE_DEX, CREATE_CON, CREATE_INT, CREATE_WIS, CREATE_WILL,
	CREATE_CHR, CREATE_LUCK, CREATE_AVATAR, CREATE_HP, CREATE_MANA, CREATE_FOOD, CREATE_ATTACK, CREATE_AC, CREATE_DAM,
	CREATE_CRIT, CREATE_MVMT, CREATE_NUM_ABILITIES, CREATE_FATE_TOKENS, CREATE_DONE} newGameCreateField;
typedef enum {SPREAD_12_4, SPREAD_10_6, SPREAD_8_8, SPREAD_6_10, SPREAD_4_12} newGameSpread;


typedef struct{
	int titleOptionsDisplayed;
	titleScreenOption selectedOption;

	fixedCharacter * titleScreen;
	fixedCharacter * options;
}titleMenu;

typedef struct{
	newGameCreateField currentField;
	newGameSpread currentSpread;
	int inEditMode;
	int statPoints;
	int numAbilites;

	individual * newPlayer;

	fixedCharacter * entry1Select;
	fixedCharacter * entry1Edit;
	fixedCharacter * entry2Select;
	fixedCharacter * entry2Edit;
	fixedCharacter * entry3Select;
	fixedCharacter * entry3Edit;
	fixedCharacter * entry4Select;
	fixedCharacter * entry4Edit;
	fixedCharacter * entry5Select;
	fixedCharacter * entry5Edit;
	fixedCharacter * entry6Select;
	fixedCharacter * entry6Edit;

	fixedCharacter * descriptionName;
	fixedCharacter * descriptionSpread;
	fixedCharacter * descriptionAvatar;
	fixedCharacter * descriptionSTR;
	fixedCharacter * descriptionDEX;
	fixedCharacter * descriptionCON;
	fixedCharacter * descriptionINT;
	fixedCharacter * descriptionWIS;
	fixedCharacter * descriptionWILL;
	fixedCharacter * descriptionCHR;
	fixedCharacter * descriptionLUCK;
	fixedCharacter * descriptionDone;
	fixedCharacter * descriptionHP;
	fixedCharacter * descriptionMana;
	fixedCharacter * descriptionFood;
	fixedCharacter * descriptionAttack;
	fixedCharacter * descriptionAC;
	fixedCharacter * descriptionDam;
	fixedCharacter * descriptionCrit;
	fixedCharacter * descriptionMvmt;
	fixedCharacter * descriptionNumAbilities;
	fixedCharacter * descriptionFateTokens;

	fixedCharacter * creationScreen;
}newGameMenu;

typedef struct{
	int inMenuMode;
	int waitingForNameMode;
	mainMenuMode currentMenu;

	titleMenu * title;
	newGameMenu * newGame;

	fixedCharacter * rightSelectArrow;
	fixedCharacter * leftSelectArrow;
	individual * newPlayer;
}mainMenu;

#endif /* SRC_HEADERS_STRUCTS_MAIN_MENU_H_ */
