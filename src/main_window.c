/*
 * hi.c
 *
 *  Created on: Feb 12, 2015
 *      Author: Adrian
 */

#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include<time.h>
#include "./headers/general.h"
#include "./headers/main_window.h"
#include "./headers/field_controller_pub_methods.h"
#include "./headers/cursor_pub_methods.h"
#include "./headers/loop_controller_pub_methods.h"
#include"./headers/console_window_pub_methods.h"
#include"./headers/console_pub_methods.h"
#include"./headers/dialog_pub_methods.h"
#include"./headers/sound_pub_methods.h"

const char g_szClassName[] = "MyWindowClass";
const char  g_szClassNameSideBar[] = "MySideBarClass";
const char * mapDirectory = "C:\\Users\\Adrian\\C\\Natural_1_new_repo\\resources\\maps\\";//".\\resources\\maps\\";//
static char * mapTestDirectory = "C:\\Users\\Adrian\\C\\Natural_1_new_repo\\unit_tests\\testMaps\\";

int mainWindowWidth = 640;
int mainWindowHeight = 820;

int moveMode = 0;
int initMoveMode = 0;
int postMoveMode = 0;

int enemyTurn = 0;

individual* player;

static groupContainer * thisGroupContainer;

individualGroup* enemies;
individualGroup* npcs;
field* main_field;
moveNodeMeta * thisMoveNodeMeta;

shiftData * viewShift;

BOOL CALLBACK ToolDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	int len = 0;
	switch (Message) {
	case WM_COMMAND:

		switch (LOWORD(wParam)) {
		case IDC_SECOND: // get the Name
			len = GetWindowTextLength(GetDlgItem(hwnd, IDC_TEXT));
			if (len == 0) {
				MessageBox(hwnd, "Longer name, please.",
						"Awesome Message Title~", MB_OK | MB_ICONEXCLAMATION);
			} else {
				char * textStr = (char*) GlobalAlloc(GPTR, len + 1);
				GetDlgItemText(hwnd, IDC_TEXT, textStr, len + 1);
//						printf("string: %s\n", textStr);
				char * str = (char *) join("Name: ", textStr);
//						printf("str: %s\n", str);
				MessageBox(hwnd, str, "Awesome Message Title~",
				MB_OK | MB_ICONEXCLAMATION);
				free(str);
			}
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT Message, WPARAM wParam,
		LPARAM lParam) {
	switch (Message) {
	case WM_INITDIALOG:
		//do processing needed before the window appears
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			EndDialog(hwnd, IDOK); //destroys dialog box, returns the second value
			break;
		case IDCANCEL:
			EndDialog(hwnd, IDCANCEL);
			break;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

void tryUpdateXShift(shiftData * viewShift, int newX){
	if(newX - viewShift->xShift < 3 && viewShift->xShift > 0){
		viewShift->xShift = viewShift->xShift - 1;
	}else if(newX - viewShift->xShift > 9){
		viewShift->xShift = viewShift->xShift + 1;
	}
}

void tryUpdateYShift(shiftData * viewShift, int newY){
	if(newY - viewShift->yShift < 3 && viewShift->yShift > 0){
		viewShift->yShift = viewShift->yShift - 1;
	}else if(newY - viewShift->yShift > 7){
		viewShift->yShift = viewShift->yShift + 1;
	}
}

void drawAll(HDC hdc, RECT* prc) {
	HDC hdcBuffer = CreateCompatibleDC(hdc);
	HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, prc->right, prc->bottom);
	HBITMAP hbmOldBuffer = SelectObject(hdcBuffer, hbmBuffer); //copy of hbmBuffer
	int index;

	drawField(hdc, hdcBuffer, main_field, viewShift);
	drawItemsFromField(hdc, hdcBuffer, main_field->thisFieldInventory, viewShift);
	if (player->hp > 0) {
		drawIndividual(hdc, hdcBuffer, player, viewShift);
	}

	drawGroups(hdc, hdcBuffer, thisGroupContainer, viewShift);

	if(isSpecialDrawModeEnabled()){
		drawSpecial(hdc, hdcBuffer, viewShift);
	}

	if (inCursorMode()) {
		drawCursor(hdc, hdcBuffer, viewShift);
	}

	if (moveMode){
		moveNode * tmp = thisMoveNodeMeta->rootMoveNode;
		character * tmpCharacter = (thisMoveNodeMeta->shadowCharacter);
		while(tmp->nextMoveNode != NULL){
			tmp = (moveNode*)tmp->nextMoveNode;
			drawUnboundShadowAnimation(hdc, hdcBuffer, tmp->x,tmp->y, tmpCharacter, viewShift, 0);

		}

		if(tmp->x != player->playerCharacter->x && tmp->y != player->playerCharacter->y){
			drawUnboundShadowAnimation(hdc, hdcBuffer, tmp->x,tmp->y, tmpCharacter, viewShift, 0);
		}


	}

	if(inInventoryViewMode()){
		drawInventoryView(hdc, hdcBuffer, viewShift);
	}

	drawThisConsole(hdc,hdcBuffer,prc);

	drawThisSideBar(hdc, hdcBuffer, prc, player);

	if(inAbilityViewMode()){
		drawThisAbilityView(hdc, hdcBuffer, prc);
	}

	if (inAbilityCreateMode()){
		drawAbilityCreateWindow(hdc, hdcBuffer, prc);
	}

	if(inNameBoxMode()){
		drawNameBoxInstance(hdc, hdcBuffer, prc);
	}

	if(shouldDrawDialogBox()){
		drawDialogBox(hdc, hdcBuffer,prc);
	}

//	DrawText(hdcBuffer, intro, -1, Rectangle(NULL, 50, 550, 150, 600) , DT_SINGLELINE );

	BitBlt(hdc, 0, 0, prc->right, prc->bottom, hdcBuffer, 0, 0, SRCCOPY);

	SelectObject(hdcBuffer, hbmOldBuffer);
	DeleteDC(hdcBuffer);
	DeleteObject(hbmBuffer);
}

int mainLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CREATE: {
		BITMAP bm;
		UINT ret;

		player = initIndividual();
		enemies = initGroup();
		npcs = initGroup();
		thisGroupContainer = initGroupContainer(enemies,npcs, NULL, NULL, NULL);
		initalizeTheGlobalRegister();

		initThisConsole(1500,0,0,300,200);
		initSidebarInstance(1502,0,0,185,400);
		initThisDialogBox(1501,10,10,RGB(255, 70, 255));
		initThisInventoryView(1503, 100, 100, 4, player->backpack);
		initAbilityCreationInstance(9500,RGB(255, 0, 255), 10, 10, mapDirectory, "effects_template.txt");
		initThisAbilityView(9504, RGB(255, 0, 255), 10, 10);
		initNameBoxInstance(9503, RGB(255,0,255), 20, 20);
		initSpecialDrawInstance();
		initEventHandlers();
		loadTriggerMaps(mapDirectory, "onAttackTriggerMap.txt","onHarmTriggerMap.txt","onDeathTriggerMap.txt", "onPickupTriggerMap.txt");
		appendNewMessageNode("You leave the forest.");
		appendNewMessageNode("The sun briefly blinds you as you step forth. There's a building in the distance, however it appears to be well guarded by several undead warriors.");

		loadGlobalRegister(mapDirectory, "individuals.txt", "items.txt", "events.txt", "sounds.txt", "images.txt", "duration_abilities.txt", "targeted_abilities.txt");
		loadDialog("dialog.txt", mapDirectory);

		initThisCursor(1508);

		enableSound();

		setSoundID(1, SOUND_MUSIC);
		setSoundID(2, SOUND_SOUND1);

		testPlaySounds();

		animationContainer * playerAnimationContainer = initAnimationContainer();
		animationContainer * secondaryAnimationContainer = NULL;
//		char line[] = "2,30,30,-1";
//		loadAnimationFromLine(playerAnimationContainer, ANIMATION_IDLE, line);
//		char line2[] = "7,5,5,5,5,5,5,5,3,7";
//		loadAnimationFromLine(playerAnimationContainer, ANIMATION_ATTACK_SLASH, line2);
//		char line3[] = "13,10,10,10,10,10,10,10,10,10,10,10,10,100,-1";
//		loadAnimationFromLine(playerAnimationContainer, ANIMATION_DEATH, line3);

		addAnimationToContainer(playerAnimationContainer, cloneAnimationFromRegistry(2001));
		addAnimationToContainer(playerAnimationContainer, cloneAnimationFromRegistry(2002));
		addAnimationToContainer(playerAnimationContainer, cloneAnimationFromRegistry(2003));
		addAnimationToContainer(playerAnimationContainer, cloneAnimationFromRegistry(2004));

		secondaryAnimationContainer = cloneAnimationContainer(playerAnimationContainer);

		specialDialogs * playerDialog = malloc(sizeof(specialDialogs));
		playerDialog->activeDialog = DIALOG_DEFAULT;
		playerDialog->sawPlayerCrime = 0;
		playerDialog->attackedByPlayer = 0;
		playerDialog->stolenFromByPlayer = 0;
		playerDialog->afraidOfPlayer = 0;
		playerDialog->playerIsMarkedForDeath = 0;

		if (defineIndividual(player, 0, 1, RGB(255, 0, 255), "adr", 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1, 20, 2, 4, 13, 3, 4, 1, 1, "MAX", 2, 4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,50,0,100,100,100,0, playerDialog, NULL, playerAnimationContainer, secondaryAnimationContainer)) {
			MessageBox(hwnd, "Failed to make player", "Notice",
			MB_OK | MB_ICONINFORMATION);
		}

		dialogMessage * thisMessage = malloc(sizeof(dialogMessage));
		strcpy(thisMessage->message,"I am a message!\0");
		setCurrentMessage(thisMessage);

		int x, y;
		main_field = loadMap("map1.txt", mapDirectory, player, thisGroupContainer);

		ret = SetTimer(hwnd, ID_TIMER, 16, NULL); //fires every 16 ms - 60 fps, 32 - 30 fps

		if (ret == 0) {
			MessageBox(hwnd, "Could not SetTimer()!", "Error",
			MB_OK | MB_ICONEXCLAMATION);
		}

		viewShift = initShiftData();

	}
		break;
	case WM_PAINT: //NOTE: NEVER USE MESSAGES IN A WM_PAINT LOOP, AS IT WILL
	{			   //SPAWN MORE MESSAGES!

		RECT rect;
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		GetClientRect(hwnd, &rect);
		drawAll(hdc, &rect);

		EndPaint(hwnd, &ps);
		ReleaseDC(hwnd, hdc);
	}
		break;
	case WM_TIMER: {
		RECT rect;
		HDC hdc = GetDC(hwnd);
		GetClientRect(hwnd, &rect);
		drawAll(hdc, &rect);

		ReleaseDC(hwnd, hdc);
	}
		break;
	case WM_CLOSE:
		KillTimer(hwnd,ID_TIMER);
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		destroyIndividual(player);
		destroyField(main_field, NULL);
		destroyConsoleInstance();
		destroyTheInventoryView();
		destroyTheGlobalRegister();
		destroyEventHandlers();
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_KEYDOWN: {
		switch (LOWORD(wParam)) {

		case 0x41: //a key (attack)
			toggleInCursorMode();
			refreshCursor(CURSOR_ATTACK, player->playerCharacter->x, player->playerCharacter->y);
			viewShift->xShiftOld = viewShift->xShift;
			viewShift->yShiftOld = viewShift->yShift;
			break;
		case 0x53: //s key (move)
			moveMode = 1;
			initMoveMode = 1;
			break;
		case 0x45: //e key (enter area)
			{
				attemptToTransit(&main_field, player, thisGroupContainer, viewShift, mapDirectory);
			}
			break;
		case 0x46: //f key
			{
//				sendMusicInterrupt(1);
//				triggerSoundEffect(9);
				cwrite("BREAKPOINT LINE");
			}
			break;
		case 0x47://g key (get)
			{
				attemptGetItemFromField(main_field,player);
			}
			break;
		case 0x48: //h key
			{
//				sendMusicInterrupt(1);
				triggerSoundEffect(6);
			}
			break;
		case 0x49://i key (inventory)
			{
				enableInventoryViewMode(player->backpack);
//				initInventoryMode = 1;
//				inventoryMode = 1;
			}
			break;
		case 0x54://t key (talk)
			{
				toggleInCursorMode();
				if(player->isSneaking){
					refreshCursor(CURSOR_PICKPOCKET, player->playerCharacter->x, player->playerCharacter->y);
				}else{
					refreshCursor(CURSOR_TALK, player->playerCharacter->x, player->playerCharacter->y);
				}

				viewShift->xShiftOld = viewShift->xShift;
				viewShift->yShiftOld = viewShift->yShift;
			}
			break;
		case 0x43://c key (ability create)
			{
				toggleCreateMode();
			}
			break;
		case 0x52://r key (ability view)
			{
				toggleAbilityViewMode();
				refreshAbilityView(player->abilities->numAbilities, player->abilities->abilitiesList);
			}
			break;
		case 0x51: //q key (toggle sneak)
			player->isSneaking = (player->isSneaking + 1) % 2;

			break;
		case 0x57: //w key (wait)
			decreaseTurns(player, thisGroupContainer, 1);

			break;
		}

	}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_HELP_ABOUT: {
			int ret =
					DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUT), hwnd, AboutDlgProc);
			if (ret == IDOK) {
				MessageBox(hwnd, "Dialog exited from clicking ok.", "Notice",
				MB_OK | MB_ICONINFORMATION);
			} else if (ret == IDCANCEL) {
				MessageBox(hwnd, "Dialog exited from clicking cancel.",
						"Notice",
						MB_OK | MB_ICONINFORMATION);
			} else if (ret == -1) {
				MessageBox(hwnd, "Dialog failed!", "Error",
				MB_OK | MB_ICONINFORMATION);
			}
		}
			break;
		case ID_FILE_EXIT:
			PostQuitMessage(0);
			break;
		}
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if(isSpecialDrawModeEnabled()){
		return specialDrawLoop(hwnd, msg, wParam, lParam);
	}else if(shouldDrawDialogBox()){
		return dialogLoop(hwnd, msg, wParam, lParam, player, thisGroupContainer, main_field);
	}else if(inNameBoxMode()){
		return nameLoop(hwnd, msg, wParam, lParam, player);
	} else if(inAbilityCreateMode()){

		if(inAbilityWaitForNameMode()){//Name loop finished, check for name
			toggleAbilityWaitForNameMode();
			if(!nameEmpty()){
				setAbilityName(getNameFromInstance());
				addAbilityToIndividual(player, getNewAbility());
				changeAbilityTemplate(0);
				resetNameBoxInstance();

				cwrite("Ability created!");
			}
		}else{
			return createAbilityLoop(hwnd, msg, wParam, lParam, player);
		}
	}else if(inAbilityViewMode()){
		abilityViewLoop(hwnd, msg, wParam, lParam, player, main_field);
		return 0;
	}else if (inCursorMode()) {
		return cursorLoop(hwnd, msg, wParam, lParam, main_field, player, thisGroupContainer, viewShift);
	} else if(moveMode){
		if(initMoveMode){
			initMoveMode = 0;
			character * shadowCharacter = malloc(sizeof(character));
			//createCharacter(player->playerCharacter->imageID,player->playerCharacter->rgb, player->playerCharacter->x, player->playerCharacter->y);
			shadowCharacter->thisAnimationContainer = cloneAnimationContainer(player->playerCharacter->thisAnimationContainer);
			shadowCharacter->secondaryAnimationContainer = cloneAnimationContainer(player->playerCharacter->secondaryAnimationContainer);

			shadowCharacter->x = player->playerCharacter->x;
			shadowCharacter->y = player->playerCharacter->y;
			shadowCharacter->direction = player->playerCharacter->direction;

			thisMoveNodeMeta = malloc(sizeof(moveNodeMeta));
//			thisMoveNodeMeta->shadowCharacter = malloc(sizeof(character));
			thisMoveNodeMeta->rootMoveNode = malloc(sizeof(moveNode));
			thisMoveNodeMeta->sum = 0;
			thisMoveNodeMeta->pathLength = 0;
			thisMoveNodeMeta->shadowCharacter = shadowCharacter;

			printf("x:%d,y:%d\n",thisMoveNodeMeta->shadowCharacter->x, thisMoveNodeMeta->shadowCharacter->y);

			thisMoveNodeMeta->rootMoveNode->x = player->playerCharacter->x;
			thisMoveNodeMeta->rootMoveNode->y = player->playerCharacter->y;
			thisMoveNodeMeta->rootMoveNode->nextMoveNode = NULL;
			thisMoveNodeMeta->rootMoveNode->hasTraversed = 1;

			viewShift->xShiftOld = viewShift->xShift;
			viewShift->yShiftOld = viewShift->yShift;

		}

		return moveLoop(hwnd, msg, wParam, lParam, &moveMode, main_field, player, thisMoveNodeMeta, &postMoveMode, viewShift);
	} else if(postMoveMode){
//		printf("looping in moveMode\n");
		animateMoveLoop(hwnd,msg, wParam, lParam,main_field,player,thisMoveNodeMeta,5,&postMoveMode, viewShift, 1);
		if (!postMoveMode) {

			freeUpMovePath(thisMoveNodeMeta->rootMoveNode->nextMoveNode);

			decreaseTurns(player, thisGroupContainer, 1);

			free(thisMoveNodeMeta->rootMoveNode);
			destroyCharacter(thisMoveNodeMeta->shadowCharacter);
			free(thisMoveNodeMeta);
		}

		return 0;
	} else if(inInventoryViewMode()){
		inventoryLoop(hwnd, msg, wParam, lParam, main_field, player, viewShift);
	}else if(thisGroupContainer->groupMoveMode){
		animateMoveLoop(hwnd, msg, wParam, lParam, main_field,
				(thisGroupContainer->selectedGroup->individuals[thisGroupContainer->selectedGroup->currentIndividualIndex]),
				thisMoveNodeMeta, 5, &thisGroupContainer->groupMoveMode, viewShift, 0);

		//animation is complete, destroy moveNodeMeta and enter postEnemyActionMode
		if (!thisGroupContainer->groupMoveMode) {

			if (thisMoveNodeMeta != NULL
					&& thisMoveNodeMeta->rootMoveNode != NULL) {
				freeUpMovePath(thisMoveNodeMeta->rootMoveNode);
			}
			free(thisMoveNodeMeta);

			thisGroupContainer->postGroupActionMode = 1;
		}
	}else if(thisGroupContainer->groupActionMode){
		if(thisGroupContainer->selectedGroup->numIndividuals == 0){
			thisGroupContainer->groupActionMode = 0;
			thisGroupContainer->initGroupActionMode = 0;

			if(!setNextActiveGroup(thisGroupContainer)){
				if(startTurn(player)){

				}

				//If player's out of actions, start enemy turn again
				if(player->remainingActions < 0){
					endTurn(player);
					thisGroupContainer->groupActionMode = 1;
					thisGroupContainer->initGroupActionMode = 1;
					setNextActiveGroup(thisGroupContainer);
				}
			}else{
				thisGroupContainer->groupActionMode = 1;
				thisGroupContainer->initGroupActionMode = 1;
			}
		} else {
			if (thisGroupContainer->initGroupActionMode) {
				thisGroupContainer->initGroupActionMode = 0;

				//note:the address of the pointer to thisMoveNodeMeta is passed in because it is malloc'd inside the method
				if(initializeEnemyTurn(thisGroupContainer->selectedGroup, player, main_field, &thisMoveNodeMeta)){
					thisGroupContainer->groupActionMode = 0;
					thisGroupContainer->postGroupActionMode = 1;
					return 0;
				}

				if(!thisGroupContainer->selectedGroup->individuals[thisGroupContainer->selectedGroup->currentIndividualIndex]->remainingActions > 0){
					thisGroupContainer->groupActionMode = 0;
					thisGroupContainer->postGroupActionMode = 1;
					return 0;
				}
			}
			thisGroupContainer->groupActionMode = 0;

			//If not moving

			if(!performAction(thisGroupContainer->selectedGroup->individuals[thisGroupContainer->selectedGroup->currentIndividualIndex], player, thisGroupContainer, main_field, &thisMoveNodeMeta)){
				thisGroupContainer->postGroupActionMode = 1;
			}else{
				thisGroupContainer->groupMoveMode = 1;
			}
		}

	}else if(thisGroupContainer->postGroupActionMode){
		thisGroupContainer->postGroupActionMode = 0;

		if(thisGroupContainer->selectedGroup->individuals[thisGroupContainer->selectedGroup->currentIndividualIndex] != NULL ){

			if(thisGroupContainer->selectedGroup->individuals[thisGroupContainer->selectedGroup->currentIndividualIndex]->remainingActions > 0){
				thisGroupContainer->groupActionMode = 1;
				return 1;
			} else {
				endTurn(thisGroupContainer->selectedGroup->individuals[thisGroupContainer->selectedGroup->currentIndividualIndex]);
			}
		}

		//determine if need to go back into enemyActionMode
		nextAvailableIndividualIndex(thisGroupContainer->selectedGroup);
		if(thisGroupContainer->selectedGroup->currentIndividualIndex > -1){
			thisGroupContainer->groupActionMode = 1;
			thisGroupContainer->initGroupActionMode = 1;
		}else{
			if(!setNextActiveGroup(thisGroupContainer)){
				if(startTurn(player)){

				}

				//If player's out of actions, start enemy turn again
				if(player->remainingActions < 0){
					endTurn(player);
					thisGroupContainer->groupActionMode = 1;
					thisGroupContainer->initGroupActionMode = 1;
					setNextActiveGroup(thisGroupContainer);
				}
			}else{
				thisGroupContainer->groupActionMode = 1;
				thisGroupContainer->initGroupActionMode = 1;
			}
		}

	}else {
		return mainLoop(hwnd, msg, wParam, lParam);
	}

}

int triggerEvent(int eventID){
	return processEvent(eventID, player, thisGroupContainer, main_field);
}

char * getContextData(char * contextKey){
	return processContextKey(contextKey, player, thisGroupContainer, main_field);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow) {

	HWND hwnd;

	WNDCLASSEX wc;
	WNDCLASSEX wc2;

	MSG Msg;

	int consoleWindowWidth = 480;
	int consoleWindowHeight = 175;
	int sidebarWindowWidth = 175;
	int sidebarWindowHeight = 655;

	//run the tests!
	//init rand for tests
	srand(0);
	int i;

	//initialize for tests
	player = initIndividual();
	enemies = initGroup();
	npcs = initGroup();
	thisGroupContainer = initGroupContainer(enemies,npcs, NULL, NULL, NULL);

	viewShift = initShiftData();
	initThisConsole(1500,0,0,300,200);
	initSidebarInstance(1502,0,0,185,400);
	initSpecialDrawInstance();
	initThisDialogBox(1501,10,10,RGB(255, 70, 255));

	initalizeTheGlobalRegister();
	initEventHandlers();
	initAbilityCreationInstance(9500,RGB(255, 0, 255), 10, 10, mapTestDirectory, "test_effects_template.txt");
	initThisAbilityView(9504, RGB(255, 0, 255), 10, 10);
	initNameBoxInstance(9503, RGB(255,0,255), 20, 20);
	loadTriggerMaps(mapTestDirectory, "test_onAttackTriggerMap.txt","test_onHarmTriggerMap.txt","test_onDeathTriggerMap.txt", "test_onPickupTriggerMap.txt");

	loadGlobalRegister(mapTestDirectory, "test_individuals.txt", "test_items.txt", "test_events.txt", "sounds.txt", "images.txt", "duration_abilities.txt", "targeted_abilities.txt");

	loadDialog("dialog.txt", mapTestDirectory);

	initThisCursor(1508,RGB(224, 64, 192),0,0);
	initSoundPlayerInstance();

	animationContainer * playerAnimationContainer = initAnimationContainer();
	animationContainer * secondaryAnimationContainer = NULL;

//	char line[] = "2,60,60,-1";
//	loadAnimationFromLine(playerAnimationContainer, ANIMATION_IDLE, line);
//	char line2[] = "7,1000,1000,1000,1000,1000,1000,1000,3,7";
//	loadAnimationFromLine(playerAnimationContainer, ANIMATION_ATTACK_SLASH, line2);
//	char line3[] = "13,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,-1";
//	loadAnimationFromLine(playerAnimationContainer, ANIMATION_DEATH, line3);

	addAnimationToContainer(playerAnimationContainer, cloneAnimationFromRegistry(2001));
	addAnimationToContainer(playerAnimationContainer, cloneAnimationFromRegistry(2002));
	addAnimationToContainer(playerAnimationContainer, cloneAnimationFromRegistry(2003));
	addAnimationToContainer(playerAnimationContainer, cloneAnimationFromRegistry(2004));

	secondaryAnimationContainer = cloneAnimationContainer(playerAnimationContainer);

	specialDialogs * playerDialog = malloc(sizeof(specialDialogs));
	playerDialog->activeDialog = DIALOG_DEFAULT;
	playerDialog->sawPlayerCrime = 0;
	playerDialog->attackedByPlayer = 0;
	playerDialog->stolenFromByPlayer = 0;
	playerDialog->afraidOfPlayer = 0;
	playerDialog->playerIsMarkedForDeath = 0;

	if (defineIndividual(player, 0, 1, RGB(255, 0, 255), "adr\0", 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 20, 2, 4, 13, 3, 10, 1, 1, "MAX\0", 2, 4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,50,0,100,100,100,0, playerDialog, NULL, playerAnimationContainer, secondaryAnimationContainer)) {
	}

	main_field = loadMap("test_map1.txt", mapTestDirectory, player, thisGroupContainer);

	test_main(player, thisGroupContainer, main_field, viewShift);

	//break down mock up
	destroyIndividual(player);
	clearGroup(thisGroupContainer->enemies);
	clearGroup(thisGroupContainer->npcs);
//	destroyField(main_field, NULL);
	destroyThisDialogBox();
	destroyConsoleInstance();
	destroyTheGlobalRegister();
	destroyEventHandlers();
	destroySpecialDrawInstance();
	thisGroupContainer->groupActionMode = 0;
	thisGroupContainer->initGroupActionMode = 0;

	srand(time(NULL));
	for(i = 0; i < 10; i++){
		printf("%d, ", rand()%100);
	}

	//step 1: registering the window class
	wc.cbSize = sizeof(WNDCLASSEX); //Size of the structure
	wc.style = 0; //Class styles (usually zero)
	wc.lpfnWndProc = WndProc; //Pointer to the window procedure for this window class
	wc.cbClsExtra = 0; //amount of extra data for this class in memory
	wc.cbWndExtra = 0; //amount of extra data per window of this type
	wc.hInstance = hInstance; //handle to app instance (input param)
	wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON)); //Icon shown when user presses alt+tab
	wc.hCursor = LoadCursorA(NULL, IDC_ARROW); //cursor that will be displayed over win.
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1); //background brush to set color of window
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MYMENU);
	; // name of menu resource to use for the windows
	wc.lpszClassName = g_szClassName; //name to identify class with
	wc.hIconSm = (HICON) LoadImage(GetModuleHandle(NULL),
	MAKEINTRESOURCE(IDI_MYICON), IMAGE_ICON, 16, 16, 0); //small icon to show in taskbar

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!", "Error",
		MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

//	create the window (handle)
	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName, "Natural 1",
	WS_OVERLAPPEDWINDOW,
	CW_USEDEFAULT,
	CW_USEDEFAULT, mainWindowWidth, mainWindowHeight,
	NULL, NULL, hInstance,
	NULL);

	if (hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!", "Error!",
		MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow); //display window

	MoveWindow(hwnd,100,100, mainWindowWidth, mainWindowHeight, TRUE);

	UpdateWindow(hwnd); //redraw
	SetActiveWindow(hwnd);



	/*
	 * GetMessage gets a message from the queue, e.g. when the user moves
	 * the mouse or types a key, clicks, etc.
	 *
	 * If there is no message, it BLOCKS, meaning it waits for a message
	 */
	while (GetMessage(&Msg, NULL, 0, 0) > 0) {
		/*
		 * TranslateMessage does additional processing on keyboard events,
		 * like generating WM_CHAR message to go along with WM_KEYDOWN
		 * messages
		 *
		 * DispachMessage sends the message out to the window
		 */
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return Msg.wParam;

}


