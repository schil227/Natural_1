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
	CREATE_DR, CREATE_CRIT, CREATE_MVMT, CREATE_NUM_ABILITIES, CREATE_FATE_TOKENS, CREATE_DONE, CREATE_ABILITY_SELECT, CREATE_ABILITY_NUM_ABILITIES, CREATE_ABILITY_BONUS_MANA} newGameCreateField;
typedef enum {SPREAD_12_4, SPREAD_10_6, SPREAD_8_8, SPREAD_6_10, SPREAD_4_12} newGameSpread;
typedef enum {NEW_GAME_CREATE, NEW_GAME_ABILITIES, NEW_GAME_FINALIZE} newGameForm;
typedef enum {LOAD_MODE, SAVE_MODE} loadSaveMode;

typedef struct {
	int saveNumber;
	char name[16];
	int level;
} loadSaveData;

typedef struct{
	int titleOptionsDisplayed;
	titleScreenOption selectedOption;

	fixedCharacter * titleScreen;
	fixedCharacter * options;
}titleMenu;

typedef struct{
	newGameForm currentForm;
	newGameCreateField currentField;
	newGameSpread currentSpread;
	character * avatars[4];
	int inEditMode;
	int statPoints;
	int numAbilites;
	int numAvatars;
	int currentAvatar;
	int inAbilityEditMode;
	int bonusMana;
	individual * newPlayer;
	abilityList * newAbilities;
	int selectedAbility;

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
	fixedCharacter * entryAddAbility;
	RECT * descriptionRect;

	char * descriptionName[350];
	char * descriptionSpread[350];
	char * descriptionAvatar[350];
	char * descriptionSTR[350];
	char * descriptionDEX[350];
	char * descriptionCON[350];
	char * descriptionINT[350];
	char * descriptionWIS[350];
	char * descriptionWILL[350];
	char * descriptionCHR[350];
	char * descriptionLUCK[350];
	char * descriptionDone[350];
	char * descriptionHP[350];
	char * descriptionMana[350];
	char * descriptionFood[350];
	char * descriptionAttack[350];
	char * descriptionAC[350];
	char * descriptionDam[350];
	char * descriptionDR[350];
	char * descriptionCrit[350];
	char * descriptionMvmt[350];
	char * descriptionNumAbilities[350];
	char * descriptionFateTokens[350];
	char * descriptionBonusMana[350];
	char * descriptionClassAbility[350];
	char * descriptionAbility[350];
	char * descriptionAbilityDone[350];

	fixedCharacter * creationScreen;
	fixedCharacter * abilityCreationScreen;
}newGameMenu;

typedef struct{
	loadSaveMode mode;

	loadSaveData * saves[10];
	int numSaveData;
	int selectedData;
	int readyToLoad;
	int readyToSave;
	int scrollCount;
	int savesPerScreen;
	int inGameLoadMode;

	fixedCharacter * loadView;
	fixedCharacter * loadSelect;
	fixedCharacter * scrollUpArrow;
	fixedCharacter * scrollDownArrow;
}loadMenu;

typedef struct{
	int inMenuMode;
	int waitingForNameMode;
	int reloadBaseGame;
	mainMenuMode currentMenu;

	titleMenu * title;
	newGameMenu * newGame;
	loadMenu * load;

	fixedCharacter * rightSelectArrow;
	fixedCharacter * leftSelectArrow;
}mainMenu;

#endif /* SRC_HEADERS_STRUCTS_MAIN_MENU_H_ */
