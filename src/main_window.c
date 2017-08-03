/*
 *  Created on: Feb 12, 2015
 *      Author: Adrian
 */
#define _WIN32_WINNT 0x0500
#include<stdio.h>
#include<windows.h>
#include <winbase.h>
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
#include "./headers/look_view_pub_methods.h"

int MAP_CREATION_MODE = 0;

//Debug timing data
//
// 	 	 	//Timer Example:
//
//			LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
//			LARGE_INTEGER Frequency;
//
//			QueryPerformanceFrequency(&Frequency);
//			QueryPerformanceCounter(&StartingTime);
//
//			//Activity to time
//
//			QueryPerformanceCounter(&EndingTime);
//			ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
//
//			ElapsedMicroseconds.QuadPart *= 1000000;
//			ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
//
//			char outLog[256];
//			sprintf(outLog, "Ind:%s find dangerous time2: %llu",friendlyIndividual->name,ElapsedMicroseconds.QuadPart);
//			cwrite(outLog);

const char g_szClassName[] = "MyWindowClass";
const char  g_szClassNameSideBar[] = "MySideBarClass";
const char * mapDirectory = "C:\\Users\\Adrian\\C\\Natural_1_new_repo\\resources\\maps\\";//".\\resources\\maps\\";//
static char * mapTestDirectory = "C:\\Users\\Adrian\\C\\Natural_1_new_repo\\unit_tests\\testMaps\\";

int mainWindowWidth = 640;
int mainWindowHeight = 820;
HWND hwnd_global;
HANDLE gDoneEvent;
HANDLE hTimerQueueTimer = NULL;
HANDLE hTimerQueue = NULL;

LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
LARGE_INTEGER Frequency;

int moveMode = 0;
int initMoveMode = 0;
int postMoveMode = 0;
int playerControlMode = 0;
int postPlayerControlMode = 0;

int freeTimer = 0;
int inActionMode = 0;
int actionModeTimer = 0;
int actionModeTimerTrigger = 5;
int inOptionalActionMode = 0;
int drawLock = 0;

int animateMoveSpeed = 5;

individual* player;

static groupContainer * thisGroupContainer;

individualGroup* enemies;
individualGroup* npcs;
field* main_field;

shiftData * viewShift;

HWND getGlobalHWnd(){
	if(hwnd_global == NULL){
		return NULL;
	}

	return hwnd_global;
}

void setPlayer(individual * newPlayer){
	player = newPlayer;
}

int getGameFieldAreaX(RECT * rect){
	return rect->right - rect->left - getSidebarWidth();
}

int getGameFieldAreaY(RECT * rect){
	return rect->bottom - rect->top - getConsoleHeight();
}


void transitViewShift(shiftData * viewShift, individual * player, field * thisField, RECT * rect){
	int xShift = floor(((player->playerCharacter->x * 50) - (getGameFieldAreaX(rect) / 2))/50);
	int yShift = floor(((player->playerCharacter->y * 50) - (getGameFieldAreaY(rect) / 2))/50);

	viewShift->xShift = max(xShift, 0);

	viewShift->yShift = max(yShift, 0);
}


void tryUpdateXShift(shiftData * viewShift, int newX, int gameFieldAreaX){
	int adjustedX = (newX - viewShift->xShift) * 50;

	//adjustedX less than 25% of gameField?
	if(!isGreaterThanPercentage(adjustedX, gameFieldAreaX, 35) && viewShift->xShift > 0){
		viewShift->xShift--;
	}else if(isGreaterThanPercentage(adjustedX, gameFieldAreaX, 65)){
		viewShift->xShift++;
	}
}

void tryUpdateYShift(shiftData * viewShift, int newY, int gameFieldAreaY){
	int adjustedY = (newY - viewShift->yShift) * 50;

	//adjustedY less than 25% of gameField?
	if(!isGreaterThanPercentage(adjustedY, gameFieldAreaY, 35) && viewShift->yShift > 0){
		viewShift->yShift--;
	}else if(isGreaterThanPercentage(adjustedY, gameFieldAreaY, 65)){
		viewShift->yShift++;
	}
}

void tryEnableOptionalActionMode(){
	inOptionalActionMode = 1;
	inActionMode = 1;
}

void tryDisableOptionalActionMode(){
	inOptionalActionMode = 0;
	inActionMode = shouldEnableActionMode();
}

int shouldEnableActionMode(){
	int i, individualsPassed = 0;
	int bounty = getCurrentBounty(player);
	individual * tmpIndividual;

	if(inOptionalActionMode){
		return 1;
	}

	if(thisGroupContainer->enemies->numIndividuals > 0){
		for(i = 0; i < thisGroupContainer->enemies->MAX_INDIVIDUALS; i++){
			tmpIndividual = thisGroupContainer->enemies->individuals[i];

			if(tmpIndividual != NULL){
				individualsPassed++;

				if(checkForTargets(tmpIndividual, player, thisGroupContainer, main_field)){
					return 1;
				}

				if(individualsPassed == thisGroupContainer->enemies->numIndividuals){
					individualsPassed = 0;
					break;
				}
			}
		}

		individualsPassed = 0;
	}


	if(thisGroupContainer->beasts->numIndividuals > 0){
		for(i = 0; i < thisGroupContainer->beasts->MAX_INDIVIDUALS; i++){
			tmpIndividual = thisGroupContainer->beasts->individuals[i];

			if(tmpIndividual != NULL){
				individualsPassed++;

				if(checkForTargets(tmpIndividual, player, thisGroupContainer, main_field)){
					return 1;
				}

				if(individualsPassed == thisGroupContainer->beasts->numIndividuals){
					individualsPassed = 0;
					break;
				}
			}
		}

		individualsPassed = 0;
	}

	if(thisGroupContainer->guards->numIndividuals > 0){
		for(i = 0; i < thisGroupContainer->guards->MAX_INDIVIDUALS; i++){
			tmpIndividual = thisGroupContainer->guards->individuals[i];

			if(tmpIndividual != NULL){
				individualsPassed++;

				findDangerousIndividualNearBy(tmpIndividual, player, thisGroupContainer, main_field, 8);

				if(tmpIndividual->targetedIndividual != NULL){
					return 1;
				}

				//TODO: make sure guard can move towards player
				if(bounty > 0 && isInLineOfSight(tmpIndividual, player, main_field)){
					return 1;
				}

				if(individualsPassed == thisGroupContainer->guards->numIndividuals){
					individualsPassed = 0;
					break;
				}
			}
		}

		individualsPassed = 0;
	}


	if(thisGroupContainer->npcs->numIndividuals > 0){
		for(i = 0; i < thisGroupContainer->npcs->MAX_INDIVIDUALS; i++){
			tmpIndividual = thisGroupContainer->npcs->individuals[i];

			if(tmpIndividual != NULL){
				individualsPassed++;

				findDangerousIndividualNearBy(tmpIndividual, player, thisGroupContainer, main_field, 8);

				if(tmpIndividual->targetedIndividual != NULL){
					return 1;
				}

				if(individualsPassed == thisGroupContainer->npcs->numIndividuals){
					individualsPassed = 0;
					break;
				}
			}
		}

		individualsPassed = 0;
	}

	if(thisGroupContainer->allies->numIndividuals > 0){
		for(i = 0; i < thisGroupContainer->allies->MAX_INDIVIDUALS; i++){
			tmpIndividual = thisGroupContainer->allies->individuals[i];

			if(tmpIndividual != NULL){
				individualsPassed++;

				findTargetIndividualForAlly(tmpIndividual, player, thisGroupContainer, main_field, 8);

				if(tmpIndividual->targetedIndividual != NULL){
					return 1;
				}

				if(individualsPassed == thisGroupContainer->allies->numIndividuals){
					individualsPassed = 0;
					break;
				}
			}
		}

		individualsPassed = 0;
	}

	return 0;
}

void forcePlayerTransit(int targetMapID, int transitID){
	while(!tryGetFieldReadLock()){}

	if(attemptToForceTransit(&main_field, player, thisGroupContainer, viewShift, mapDirectory, targetMapID, transitID)){
		RECT rect;
		GetClientRect(hwnd_global, &rect);
		HDC hdc = GetDC(hwnd_global);
		HDC hdcBuffer = CreateCompatibleDC(hdc);

		printf("WANT: transit\n");fflush(stdout);
		while(!tryGetFieldWriteLock()){}
		printf("GOT: transit\n");fflush(stdout);
		updateFieldGraphics(hdc, hdcBuffer, main_field);
		transitViewShift(viewShift, player, main_field, &rect);
		inActionMode = shouldEnableActionMode();

		releaseFieldWriteLock();

		printf("RELEASED: transit\n");fflush(stdout);
	}

	releaseFieldReadLock();
}

void drawGameMode(HDC hdc, RECT* prc) {
	HDC hdcBuffer = CreateCompatibleDC(hdc);
	HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, prc->right, prc->bottom);
	HBITMAP hbmOldBuffer = SelectObject(hdcBuffer, hbmBuffer); //copy of hbmBuffer
	int index;

	printf("WANT: dark\n"); fflush(stdout);
	while(!tryGetFieldReadLock()){}
	printf("GOT: dark\n");fflush(stdout);

	if(inWorldMapMode()){
		releaseFieldReadLock();
		printf("RELEASED: dark\n"); fflush(stdout);

		drawWorldMapInstance(hdc, hdcBuffer, player, viewShift);

		if(isPaused()){
			drawPauseWindow(hdc, hdcBuffer, prc);
		}

		BitBlt(hdc, 0, 0, prc->right, prc->bottom, hdcBuffer, 0, 0, SRCCOPY);

		SelectObject(hdcBuffer, hbmOldBuffer);
		DeleteDC(hdcBuffer);
		DeleteObject(hbmBuffer);
		return;
	}

	if(main_field == NULL){
		BitBlt(hdc, 0, 0, prc->right, prc->bottom, hdcBuffer, 0, 0, SRCCOPY);

		SelectObject(hdcBuffer, hbmOldBuffer);
		DeleteDC(hdcBuffer);
		DeleteObject(hbmBuffer);
		return;
	}

	if(main_field->isDark){
		main_field->playerLoS = getAttributeSum(player, "darkLoS");
		main_field->playerCords->x = player->playerCharacter->x;
		main_field->playerCords->y = player->playerCharacter->y;
	}

	printf("draw field\n");
	int consoleHeight = inLookMode() ? prc->bottom : prc->bottom - getConsoleHeight();
	drawField(hdc, hdcBuffer, main_field, consoleHeight,  prc->right - getSidebarWidth(), viewShift);

	drawHUDAttackSpaces(hdc, hdcBuffer, prc, viewShift);

	drawItemsFromField(hdc, hdcBuffer, main_field, viewShift);

	if (player->hp > 0) {
		drawIndividual(hdc, hdcBuffer, player, viewShift);
	}

	drawGroups(hdc, hdcBuffer, thisGroupContainer, main_field, viewShift);

	if(isSpecialDrawModeEnabled()){
		drawSpecial(hdc, hdcBuffer, main_field, viewShift);
	}

	if (inCursorMode()) {
		drawCursor(hdc, hdcBuffer, viewShift);
	}

	releaseFieldReadLock();
	printf("RELEASED: dark\n"); fflush(stdout);

	if (moveMode){
		moveNode * tmp = player->thisMoveNodeMeta->rootMoveNode;
		character * tmpCharacter = (player->thisMoveNodeMeta->shadowCharacter);

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

	if(inLookMode()){
		drawThisSideBar(hdc, hdcBuffer, prc, player);

		while(!tryGetLookReadLock()){}
		drawLookView(hdc, hdcBuffer, prc);
		releaseLookReadLock();
	}else{
		while(!tryGetConsoleReadLock()){}
		drawThisConsole(hdc,hdcBuffer,prc);
		releaseConsoleReadLock();

		drawThisSideBar(hdc, hdcBuffer, prc, player);

		drawHudNotifications(hdc, hdcBuffer, prc, player, inActionMode);
	}

	//popups
	if(inAbilityViewMode()){
		drawThisAbilityView(hdc, hdcBuffer, prc);
	}

	if (inAbilityCreateMode()){
		drawAbilityCreateWindow(hdc, hdcBuffer, prc);
	}

	if(inNameBoxMode()){
		drawNameBoxInstance(hdc, hdcBuffer, prc);
	}

	while(!tryGetDialogReadLock()){}
	if(shouldDrawDialogBox()){
		drawDialogBox(hdc, hdcBuffer,prc);
	}
	releaseDialogReadLock();

	if(inCharacterInfoView()){
		drawCharacterInfoView();
	}

	if(isPaused()){
		drawPauseWindow(hdc, hdcBuffer, prc);
	}

	if(inGameMenuMode()){
		drawGameMenu(hdc, hdcBuffer, prc);
	}

	BitBlt(hdc, 0, 0, prc->right, prc->bottom, hdcBuffer, 0, 0, SRCCOPY);

	SelectObject(hdcBuffer, hbmOldBuffer);
	DeleteDC(hdcBuffer);
	DeleteObject(hbmBuffer);
}

int sampleSpacer = 0;

LRESULT CALLBACK TimerProc(PVOID lpParam, BOOLEAN TimerOrWaitFired){
	//Wait for drawLock to be released
	if(drawLock)
	{
		return 0;
	}

	sampleSpacer++;

	if(sampleSpacer == 1){
		QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&StartingTime);
	}

	//claim lock
	drawLock = 1;

	HWND hwnd = hwnd_global;

	RECT rect;
	HDC hdc = GetDC(hwnd);
	GetClientRect(hwnd, &rect);

	if(inMainMenuMode()){
		HDC hdcBuffer = CreateCompatibleDC(hdc);
		HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
		HBITMAP hbmOldBuffer = SelectObject(hdcBuffer, hbmBuffer);

		drawMainMenu(hdc, hdcBuffer, &rect);

		if (inAbilityCreateMode()){
			drawAbilityCreateWindow(hdc, hdcBuffer, &rect);
		}

		if(inNameBoxMode()){
			drawNameBoxInstance(hdc, hdcBuffer, &rect);
		}

		BitBlt(hdc, 0, 0, rect.right, rect.bottom, hdcBuffer, 0, 0, SRCCOPY);
		SelectObject(hdcBuffer, hbmOldBuffer);
		DeleteDC(hdcBuffer);
		DeleteObject(hbmBuffer);
		ReleaseDC(hwnd, hdc);

		drawLock = 0;
		return 0;
	}

	drawGameMode(hdc, &rect);

	ReleaseDC(hwnd, hdc);

	if(isPaused()){
		drawLock = 0;
		return 0;
	}

	if(isSpecialDrawModeEnabled()){
		incrementSpecialDrawTimerTicks();
		if(specialDrawDurationMet()){
			resetSpecialDraw();
			PostMessage(hwnd, WM_MOUSEACTIVATE, 0, 0);
		}

		drawLock = 0;
		return 0;
	}

	if(shouldDrawDialogBox()){
		shouldSpeakTickTrigger();
		PostMessage(hwnd, WM_MOUSEACTIVATE, 0, 0);
		drawLock = 0;
		return 0;
	}

	if(postMoveMode){
		animateMove(rect, player, main_field, viewShift, &postMoveMode, animateMoveSpeed, 1);
		if (!postMoveMode) {
			moveNode * tmp = player->thisMoveNodeMeta->rootMoveNode;
			int numSpaces = 0;

			while(tmp->nextMoveNode != NULL){
				tmp = (moveNode*)tmp->nextMoveNode;
				numSpaces++;
			}

			double food = (numSpaces*1.0)/(getAttributeSum(player, "mvmt")*1.0);

			decreaseFood(player, food);

			freeUpMovePath(player->thisMoveNodeMeta->rootMoveNode->nextMoveNode);
			decreaseTurns(player, thisGroupContainer, 1, inActionMode);
			free(player->thisMoveNodeMeta->rootMoveNode);
			PostMessage(hwnd, WM_MOUSEACTIVATE, 0, 0);
		}

//		drawLock = 0;
//		return 0;
	}

	if(thisGroupContainer->groupMoveMode){
		int speed = animateMoveSpeed;

		if(!inActionMode){
			speed = 1;
		}

		individual * tmpIndividual = (thisGroupContainer->selectedGroup->individuals[thisGroupContainer->selectedGroup->currentIndividualIndex]);

		animateMove(rect, tmpIndividual, main_field, viewShift, &thisGroupContainer->groupMoveMode, animateMoveSpeed, 0);

		//animation is complete, destroy moveNodeMeta and enter postEnemyActionMode
		if (!thisGroupContainer->groupMoveMode) {
			if (tmpIndividual->thisMoveNodeMeta != NULL && tmpIndividual->thisMoveNodeMeta->rootMoveNode != NULL) {
				freeUpMovePath(tmpIndividual->thisMoveNodeMeta->rootMoveNode);
				tmpIndividual->thisMoveNodeMeta->rootMoveNode = NULL;
			}

			thisGroupContainer->postGroupActionMode = 1;
			PostMessage(hwnd, WM_MOUSEACTIVATE, 0, 0);
		}

		drawLock = 0;
		return 0;
	}

	if(thisGroupContainer->movingIndividuals->numIndividuals > 0){
		handleMovingIndividuals(thisGroupContainer, main_field, animateMoveSpeed);
	}

	//prevent AI inturruption
	if(moveMode || postMoveMode || inCursorMode() || moveMode || inInventoryViewMode() || inCharacterInfoView() ||
			inLookMode() || inAbilityViewMode() || inAbilityCreateMode() || inNameBoxMode() || shouldDrawDialogBox()){
		drawLock = 0;
		return 0;
	}

	freeTimer++;
	if(freeTimer > animateMoveSpeed + 30 && thisGroupContainer->movingIndividuals->numIndividuals == 0){
		freeTimer = 0;

		if(!inActionMode){
			decreaseFood(player, 0.025);

			if(!thisGroupContainer->groupActionMode){ // && thisGroupContainer->movingIndividuals->numIndividuals == 0
				thisGroupContainer->initGroupActionMode = 1;
				thisGroupContainer->groupActionMode = 1;
				setNextActiveGroup(thisGroupContainer);
				PostMessage(hwnd, WM_MOUSEACTIVATE, 0, 0);
			}
		}
	}

	if(sampleSpacer == 1){
		QueryPerformanceCounter(&EndingTime);
		ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;

		ElapsedMicroseconds.QuadPart *= 1000000;
		ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

		char outLog[256];
		sprintf(outLog, "draw: %llu",ElapsedMicroseconds.QuadPart);
		cwrite(outLog);
	}

	if(sampleSpacer > 30){
		sampleSpacer = 0;
	}

	drawLock = 0;
}

void destroyGame(){
	//will wait for timer callback to finish
	DeleteTimerQueueTimer(hTimerQueue, hTimerQueueTimer, INVALID_HANDLE_VALUE);

	destroyField(main_field, NULL);
	destroyFieldGraphicContainer();
	destroyIndividual(player);
	clearGroup(thisGroupContainer->enemies);
	clearGroup(thisGroupContainer->npcs);
	destroyThisDialogBox();
	destroyConsoleInstance();
	destroyTheGlobalRegister();
	destroyEventHandlers();
	destroySpecialDrawInstance();
	thisGroupContainer->groupActionMode = 0;
	thisGroupContainer->initGroupActionMode = 0;
}

void destroyAndLoad(HWND hwnd, int isFirstLoad, int saveSlot){
	HDC hdc = GetDC(hwnd);
	HDC hdcBuffer = CreateCompatibleDC(hdc);
	RECT * rect;
	GetClientRect(hwnd_global, rect);
	char saveMapDirectory[256];
	if(saveSlot >= 0 && saveSlot < 10){
		sprintf(saveMapDirectory, "%ssaves\\save%d\\", mapDirectory, saveSlot);
	}else{
		strcpy(saveMapDirectory, mapDirectory);
	}

	if(!isFirstLoad){
		destroyGame();
	}

	player = initIndividual();
	enemies = initGroup();
	npcs = initGroup();
	thisGroupContainer = initGroupContainer(enemies,npcs, NULL, NULL, NULL);
	initLockAuth();
	initalizeTheGlobalRegister();

	initThisConsole(1500,0,0,300,200);
	initSidebarInstance(1502,0,0,185,400);
	initThisDialogBox(1501,10,10,RGB(255, 70, 255));
	initThisInventoryView(1503, 100, 100, 4, player->backpack);
	initAbilityCreationInstance(1424,RGB(255, 0, 255), 10, 10, mapDirectory, "effects_template.txt");
	initThisAbilityView(9504, RGB(255, 0, 255), 10, 10);
	initNameBoxInstance(9503, RGB(255,0,255), 20, 20);
	initSpecialDrawInstance();
	initEventHandlers();
	loadTriggerMaps(mapDirectory, "onAttackTriggerMap.txt","onHarmTriggerMap.txt","onDeathTriggerMap.txt", "onPickupTriggerMap.txt");
	appendNewMessageNode("You leave the forest.");
	appendNewMessageNode("The sun briefly blinds you as you step forth. There's a building in the distance, however it appears to be well guarded by several undead warriors.");

	loadGlobalRegister(saveMapDirectory, mapDirectory, "individuals.txt", "items.txt", "events.txt", "sounds.txt", "images.txt", "permenant_abilities.txt", "duration_abilities.txt", "targeted_abilities.txt", "instant_abilities.txt", "mapInfo.txt", "descriptionLookup.txt", "interactableObjects.txt", "areaNodes.txt");
	setUpAnimationDrawAreas(hdc, hdcBuffer);
	loadDialog("dialog.txt", mapDirectory);
	setAbilityCreationIDCounter(1000 + numAbilitiesInGlobalRegistry());

	initHudInstance();
	initThisCursor(1508);
	initLookView(1519, 1520);
	initCharacterInfoView();
	initPauseView(1523);
	initWorldMapController(10003);

	player = getIndividualFromRegistry(1);

	mapInfo * loadMapInfo = getCurrentMapInfoFromRegistry();

	main_field = loadMap(loadMapInfo->mapName, mapDirectory, player, thisGroupContainer);
	updateFieldGraphics(hdc, hdcBuffer, main_field);
	initFieldGraphicContainer();
	setUpAnimationDrawAreas(hdc, hdcBuffer);

	gDoneEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (NULL == gDoneEvent)
	{
		printf("CreateEvent failed (%d)\n", GetLastError());
		exit(0);
	}

	hTimerQueue = CreateTimerQueue();
	if (NULL == hTimerQueue)
	{
		printf("CreateTimerQueue failed (%d)\n", GetLastError());
		exit(0);
	}

	if (!CreateTimerQueueTimer( &hTimerQueueTimer, hTimerQueue, TimerProc, 123 , 32, 32, 0))
	{
		printf("CreateTimerQueueTimer failed (%d)\n", GetLastError());
		exit(0);
	}

	viewShift = initShiftData();
	transitViewShift(viewShift, player, main_field, rect);

	inActionMode = shouldEnableActionMode();
}

void saveGame(int saveSlot){
	char saveDirectory[32];

	sprintf(saveDirectory, "saves\\save%d\\", saveSlot);

	writeSaveMetaFile(mapDirectory,saveDirectory,"saveMetaData.txt", player);
	writeIndividualsToFile(mapDirectory,saveDirectory,"individuals.txt");
	writeMapInfoToFile(mapDirectory,saveDirectory,"mapInfo.txt");
	writeItemsToFile(mapDirectory,saveDirectory,"items.txt");
	writeInteractableObjectToFile(mapDirectory,saveDirectory,"interactableObjects.txt");
	writeAreaNodesToFile(mapDirectory,saveDirectory,"areaNodes.txt");

	writePermenantAbilitiesToFile(mapDirectory,saveDirectory,"permenant_abilities.txt");
	writeDurationAbilitiesToFile(mapDirectory,saveDirectory,"duration_abilities.txt");
	writeTargetedAbilitiesToFile(mapDirectory,saveDirectory,"targeted_abilities.txt");
	writeInstantAbilitiesToFile(mapDirectory,saveDirectory,"instant_abilities.txt");
}

int mainLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CREATE: {
		BITMAP bm;
		UINT ret;

		HDC hdc = GetDC(hwnd);
		HDC hdcBuffer = CreateCompatibleDC(hdc);

		player = initIndividual();
		enemies = initGroup();
		npcs = initGroup();
		thisGroupContainer = initGroupContainer(enemies,npcs, NULL, NULL, NULL);
		initLockAuth();
		initalizeTheGlobalRegister();

		initThisConsole(1500,0,0,300,200);
		initSidebarInstance(1502,0,0,185,400);
		initThisDialogBox(1501,10,10,RGB(255, 70, 255));
		initThisInventoryView(1503, 100, 100, 4, player->backpack);
		initAbilityCreationInstance(1424,RGB(255, 0, 255), 10, 10, mapDirectory, "effects_template.txt");
		initThisAbilityView(9504, RGB(255, 0, 255), 10, 10);
		initNameBoxInstance(9503, RGB(255,0,255), 20, 20);
		initSpecialDrawInstance();
		initEventHandlers();
		loadTriggerMaps(mapDirectory, "onAttackTriggerMap.txt","onHarmTriggerMap.txt","onDeathTriggerMap.txt", "onPickupTriggerMap.txt");
		appendNewMessageNode("You leave the forest.");
		appendNewMessageNode("The sun briefly blinds you as you step forth. There's a building in the distance, however it appears to be well guarded by several undead warriors.");

		loadGlobalRegister(mapDirectory, mapDirectory, "individuals.txt", "items.txt", "events.txt", "sounds.txt", "images.txt",  "permenant_abilities.txt", "duration_abilities.txt", "targeted_abilities.txt", "instant_abilities.txt", "mapInfo.txt", "descriptionLookup.txt", "interactableObjects.txt", "areaNodes.txt");

		loadDialog("dialog.txt", mapDirectory);
		setAbilityCreationIDCounter(1000 + numAbilitiesInGlobalRegistry());

		initHudInstance();
		initThisCursor(1508);
		initLookView(1519, 1520);
		initCharacterInfoView();
		initPauseView(1523);
		initWorldMapController(10003);

		enableSound();

		setSoundID(1, SOUND_MUSIC);
		setSoundID(2, SOUND_SOUND1);

		testPlaySounds();

		player = getIndividualFromRegistry(1);

		dialogMessage * thisMessage = malloc(sizeof(dialogMessage));
		strcpy(thisMessage->message,"I am a message!\0");
		setCurrentMessage(thisMessage);

		setUpAnimationDrawAreas(hdc, hdcBuffer);
		viewShift = initShiftData();
		initMainMenu(1, mapDirectory);
		initGameMenu();

		main_field = loadMap("map1.txt", mapDirectory, player, thisGroupContainer);

		updateFieldGraphics(hdc, hdcBuffer, main_field);
		initFieldGraphicContainer();

	    gDoneEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	    if (NULL == gDoneEvent)
	    {
	        printf("CreateEvent failed (%d)\n", GetLastError());
	        exit(0);
	    }

	    hTimerQueue = CreateTimerQueue();
	    if (NULL == hTimerQueue)
	    {
	        printf("CreateTimerQueue failed (%d)\n", GetLastError());
	        exit(0);
	    }

	    if (!CreateTimerQueueTimer( &hTimerQueueTimer, hTimerQueue, TimerProc, 123 , 32, 32, 0))
	    {
	        printf("CreateTimerQueueTimer failed (%d)\n", GetLastError());
	        exit(0);
	    }

		inActionMode = shouldEnableActionMode();

		individual * tmpNPC = thisGroupContainer->npcs->individuals[0];
		tmpNPC->desiredLocation->x = 5;
		tmpNPC->desiredLocation->y = 9;
	}
		break;
	case WM_MOUSEMOVE:
	{
		break;
	}
//	case WM_PAINT: //NOTE: NEVER USE MESSAGES IN A WM_PAINT LOOP, AS IT WILL
//	{			   //SPAWN MORE MESSAGES!
//
//		RECT rect;
//		PAINTSTRUCT ps;
//		HDC hdc = BeginPaint(hwnd, &ps);
//
//		GetClientRect(hwnd, &rect);
//		drawAll(hdc, &rect);
//
//		EndPaint(hwnd, &ps);
//		ReleaseDC(hwnd, hdc);
//	}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		destroyGame();
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_KEYDOWN: {
		switch (LOWORD(wParam)) {
		case 0x1B:
			enableGameMenuMode();
			break;
		case 0x41: //a key (attack)
			toggleInCursorMode();
			refreshCursor(CURSOR_ATTACK, player->playerCharacter->x, player->playerCharacter->y);
			viewShift->xShiftOld = viewShift->xShift;
			viewShift->yShiftOld = viewShift->yShift;

			enableHUDAttackSpaces(generateAttackSpaces(player, main_field));
			break;
		case 0x53: //s key (move)
			moveMode = 1;
			initMoveMode = 1;
			break;
		case 0x45: //e key (enter area)
			{
				while(!tryGetFieldReadLock()){}

				if(attemptToTransit(&main_field, player, thisGroupContainer, viewShift, mapDirectory)){
					RECT rect;
					GetClientRect(hwnd, &rect);
					HDC hdc = GetDC(hwnd);
					HDC hdcBuffer = CreateCompatibleDC(hdc);

					printf("WANT: transit\n");fflush(stdout);
					while(!tryGetFieldWriteLock()){}
					printf("GOT: transit\n");fflush(stdout);
					updateFieldGraphics(hdc, hdcBuffer, main_field);
					transitViewShift(viewShift, player, main_field, &rect);
					inActionMode = shouldEnableActionMode();

					releaseFieldWriteLock();

					printf("RELEASED: transit\n");fflush(stdout);
				}

				releaseFieldReadLock();
			}
			break;
		case 0x46: //f key
			{
				destroyAndLoad(hwnd, 0, 0);
			}
			break;
		case 0x47://g key (get)
			{
				populateCurrentSpaceInventory(main_field, player->playerCharacter->x, player->playerCharacter->y);

				if(main_field->currentSpaceInventory->inventorySize == 1){
					item * tmpItem = main_field->currentSpaceInventory->inventoryArr[0];
					removeItemFromField(main_field, tmpItem);
					addItemToInventory(player->backpack, tmpItem);
					tmpItem->npcID = player->ID;

					triggerEventOnPickup(tmpItem->ID, player->isPlayer);

					if(tmpItem->owner != 0 && tmpItem->isStolen == 0){
						tmpItem->isStolen = 1;

						processCrimeEvent(CRIME_STEALING, tmpItem->price, tmpItem->owner, tmpItem->ID);
					}
				}else if(main_field->currentSpaceInventory->inventorySize > 1){
					enableItemFieldGetMode();
					enableInventoryViewMode(main_field->currentSpaceInventory);
				}
			}
			break;
		case 0x48: //h key
			{
//				sendMusicInterrupt(1);
				triggerSoundEffect(6);

				saveGame(0);
			}
			break;
		case 0x49://i key (inventory)
			{
				enableInventoryViewMode(player->backpack);
//				initInventoryMode = 1;
//				inventoryMode = 1;
			}
			break;
		case 0x4B://k key (inventory)
			{
				healIndividual(player, 1);
			}
			break;
		case 0x4C://l key (inventory)
			{
				toggleInCursorMode();
				refreshCursor(CURSOR_LOOK, player->playerCharacter->x, player->playerCharacter->y);

				populateLookDataInstance(main_field, player, player->playerCharacter->x, player->playerCharacter->y);

				viewShift->xShiftOld = viewShift->xShift;
				viewShift->yShiftOld = viewShift->yShift;

				enableLookMode();
			}
			break;
		case 0x4F://o key
			{
				restoreMana(player, 1);
			}
			break;
		case 0x50://p key (inventory)
			{
				togglePaused();
			}
			break;
		case 0x55://u key (info)
			{
				enableCharacterInfoView(player);
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
				enableAbilityCreateMode(0, ABILITY_CREATE_DEFAULT, DEFAULT_ABILITY);
			}
			break;
		case 0x52://r key (ability view)
			{
				if(!hasActiveStatusEffect(player, STATUS_SILENCED)){
					toggleAbilityViewMode();
					refreshAbilityView(player->abilities->numAbilities, player->abilities->abilitiesList);
				}
			}
			break;
		case 0x51: //q key (toggle sneak)
			player->isSneaking = (player->isSneaking + 1) % 2;

			break;
		case 0x57: //w key (wait)
			decreaseTurns(player, thisGroupContainer, 1, inActionMode);
			decreaseFood(player, 0.1);
			break;
		case 0x58: //x key (optional action mode)
			if(inOptionalActionMode){
				tryDisableOptionalActionMode();
			}else{
				tryEnableOptionalActionMode();
			}
			break;
		case 0x5A: //z key (interact)
			{
				toggleInCursorMode();
				refreshCursor(CURSOR_INTERACT, player->playerCharacter->x, player->playerCharacter->y);
			}
			break;
		case 0x34: //left
		case 0x64:
		case 0x36: //right
		case 0x66:
		case 0x38: //up
		case 0x68:
		case 0x32: //down
		case 0x62:
		case 0x31: //down left
		case 0x61:
		case 0x37: //up left
		case 0x67:
		case 0x39: //up right
		case 0x69:
		case 0x33: //down right
		case 0x63:
			{
				if(!inActionMode){
					//already moving?
					if(postMoveMode){
						break;
					}

					int dx = xMoveChange(LOWORD(wParam) % 16);
					int dy = yMoveChange(LOWORD(wParam) % 16);

					space * tmpSpace = getSpaceFromField(main_field, player->playerCharacter->x + dx, player->playerCharacter->y + dy);
					if(tmpSpace != NULL && tmpSpace->isPassable  && !tmpSpace->spaceIsReserved && canPassThroughInteractableObject(tmpSpace->interactableObject)){
						if (tmpSpace->currentIndividual == NULL) {
							tmpSpace->spaceIsReserved = 1;

							player->thisMoveNodeMeta->rootMoveNode = malloc(sizeof(moveNode));
							player->thisMoveNodeMeta->sum = 0;
							player->thisMoveNodeMeta->pathLength = 0;

							player->thisMoveNodeMeta->rootMoveNode->x = player->playerCharacter->x;
							player->thisMoveNodeMeta->rootMoveNode->y = player->playerCharacter->y;
							player->thisMoveNodeMeta->rootMoveNode->hasTraversed = 1;

							moveNode * nextNode = malloc(sizeof(moveNode));
							nextNode->x = player->playerCharacter->x + dx;
							nextNode->y = player->playerCharacter->y + dy;
							nextNode->nextMoveNode = NULL;
							nextNode->hasTraversed = 0;

							player->thisMoveNodeMeta->rootMoveNode->nextMoveNode = nextNode;

							removeIndividualFromField(main_field, player->playerCharacter->x, player->playerCharacter->y);

							postMoveMode = 1;
						}else{
							individual * tmpIndividual = tmpSpace->currentIndividual;

							if(tmpIndividual->currentGroupType == GROUP_ALLIES){
								swapPositionWithAlly(main_field, thisGroupContainer->movingIndividuals, player, tmpIndividual, animateMoveSpeed);
								postMoveMode = 1;
							}
						}
					}
				}
			}
		}
	}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
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

LRESULT CALLBACK MapGeneratorTimerProc(PVOID lpParam, BOOLEAN TimerOrWaitFired){
	//Wait for drawLock to be released
	if(drawLock)
	{
		return 0;
	}

	//claim lock
	drawLock = 1;

	HWND hwnd = hwnd_global;

	RECT rect;
	HDC hdc = GetDC(hwnd);
	GetClientRect(hwnd, &rect);

	drawMapGenerator(hdc, &rect, viewShift);

	ReleaseDC(hwnd, hdc);

	drawLock = 0;

	return 0;
}

LRESULT CALLBACK MapGeneratorProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch (msg) {
		case WM_CREATE: {
			HDC hdc = GetDC(hwnd);
			HDC hdcBuffer = CreateCompatibleDC(hdc);

			initLockAuth();
			initalizeTheGlobalRegister();

			loadGlobalRegister(mapDirectory, mapDirectory, "", "", "", "", "images.txt", "", "", "", "", "", "", "", "");
			setUpAnimationDrawAreas(hdc, hdcBuffer);
			initMapGenerator(mapDirectory);

			updateFieldGraphics(hdc, hdcBuffer, mapGeneratorField());

			DeleteDC(hdcBuffer);
			DeleteDC(hdc);

		    gDoneEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		    if (NULL == gDoneEvent)
		    {
		        printf("CreateEvent failed (%d)\n", GetLastError());
		        exit(0);
		    }

		    hTimerQueue = CreateTimerQueue();
		    if (NULL == hTimerQueue)
		    {
		        printf("CreateTimerQueue failed (%d)\n", GetLastError());
		        exit(0);
		    }

		    if (!CreateTimerQueueTimer( &hTimerQueueTimer, hTimerQueue, MapGeneratorTimerProc, 123 , 32, 32, 0))
		    {
		        printf("CreateTimerQueueTimer failed (%d)\n", GetLastError());
		        exit(0);
		    }

			viewShift = initShiftData();
		}
		break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			//will wait for timer callback to finish
			DeleteTimerQueueTimer(hTimerQueue, hTimerQueueTimer, INVALID_HANDLE_VALUE);

			PostQuitMessage(0);
			break;
		case WM_LBUTTONDOWN:
			break;
		case WM_KEYDOWN: {
			switch (LOWORD(wParam)) {
				case 0x34: //left
				case 0x64:
				{
					if(inMapGenerationSelectionMode()){
						changeMapGeneratorSelectX(-1);
						break;
					}
				}
				case 0x36: //right
				case 0x66:
				{
					if(inMapGenerationSelectionMode()){
						changeMapGeneratorSelectX(1);
						break;
					}
				}
				case 0x38: //up
				case 0x68:
				{
					if(inMapGenerationSelectionMode()){
						changeMapGeneratorSelectY(-1);
						break;
					}
				}
				case 0x32: //down
				case 0x62:
				{
					if(inMapGenerationSelectionMode()){
						changeMapGeneratorSelectY(1);
						break;
					}
				}
				case 0x31: //down left
				case 0x61:
				case 0x37: //up left
				case 0x67:
				case 0x39: //up right
				case 0x69:
				case 0x33: //down right
				case 0x63:
					{
						int direction;
						direction = LOWORD(wParam) % 16;

						RECT rect;
						GetClientRect(hwnd, &rect);

						moveMapGeneratorCursor(direction, viewShift, &rect);
					}
					break;
				case 0x41: //a key : bring up set image selector
					enableMapGenerationSelectionMode();
					break;
				case 0x52: //r key
					{
						while(!tryGetFieldReadLock()){}
						while(!tryGetFieldWriteLock()){}

						HDC hdc = GetDC(hwnd);
						HDC hdcBuffer = CreateCompatibleDC(hdc);

						rotateMapGenerationCharacter(hdc, hdcBuffer);

						DeleteDC(hdcBuffer);
						DeleteDC(hdc);

						releaseFieldWriteLock();
						releaseFieldReadLock();
					}
					break;
				case 0x44: //d key
					{
						char mapGenerationDirectory[256];

						sprintf(mapGenerationDirectory, "%s%s", mapDirectory, "generatedMaps\\");

						writeGeneratedMapToFile(mapGenerationDirectory, "mapX.txt");
						writeGeneratedMapToFile(mapGenerationDirectory, "loadmap.txt");
					}
					break;
				case 0x26: //UP key
					{
						changeMapGeneratorHeight(-1);
					}
					break;
				case 0x28: //DOWN key
					{
						changeMapGeneratorHeight(1);
					}
					break;

				case 0x25: //LEFT key
					{
						changeMapGeneratorWidth(-1);
					}
					break;

				case 0x27: //RIGHT key
					{
						changeMapGeneratorWidth(1);
					}
					break;
				case 0x0D: //enter
					{
						if(inMapGenerationSelectionMode()){
							setMapGenerationSelectedCharacter();
							disableMapGenerationSelectionMode();
						}else{
							while(!tryGetFieldReadLock()){}
							while(!tryGetFieldWriteLock()){}

							HDC hdc = GetDC(hwnd);
							HDC hdcBuffer = CreateCompatibleDC(hdc);

							setMapGenerationSpace(hdc, hdcBuffer);

							DeleteDC(hdcBuffer);
							DeleteDC(hdc);

							releaseFieldWriteLock();
							releaseFieldReadLock();
						}
					}
					break;
				case 0x1B: //escape
					{
						if(inMapGenerationSelectionMode()){
							disableMapGenerationSelectionMode();
						}
					}
					break;
			}
			break;
		}
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if(isPaused()){
		return pausedLoop(hwnd, msg, wParam, lParam);
	}

	//Required for menu mode
	if(inNameBoxMode()){
		nameLoop(hwnd, msg, wParam, lParam, player);
		return 0;
	}else if(inAbilityCreateMode()){
		if(inAbilityWaitForNameMode()){//Name loop finished, check for name
			toggleAbilityWaitForNameMode();
			if(!nameEmpty()){
				setAbilityName(getNameFromInstance());
				ability * newAbility = getNewAbility();


				if(inMainMenuMode()){
					addAbilityToNewGameAbilityMode(newAbility);
					disableAbilityCreateMode();
				}else{
					addAbilityToIndividual(player, newAbility);
					addAbilityToRegistryByType(newAbility);
					cwrite("Ability created!");
				}

				changeAbilityTemplate(0);
				resetNameBoxInstance();
			}

			PostMessage(hwnd, WM_MOUSEACTIVATE, wParam, lParam);
			return 0;
		}else{
			createAbilityLoop(hwnd, msg, wParam, lParam, player);
			if(!inAbilityCreateMode() && inMainMenuMode()){
				disableNewGameAbilityEditMode();
			}

			return 0;
		}
	}

	if(inMainMenuMode()){
		if(inMainMenuWaitForNameMode()){
			disableMainMenuWaitForNameMode();
			if(!nameEmpty()){
				MainMenuSetName(getNameFromInstance());
				resetNameBoxInstance();
			}
		}

		if(mainMenuReloadBaseGame()){
			destroyAndLoad(hwnd, 0, -1);
			disableMainMenuReloadBaseGame();
			reinitializeMainMenu();
		}

		mainMenuLoop(hwnd, msg, wParam, lParam);

		if(mainMenuReadyToLoad()){
			destroyAndLoad(hwnd, 0, getMainMenuLoadSlot());
			disableMainMenuMode();
		}else if(mainMenuReadyToSave()){
			saveGame(getMainMenuLoadSlot());
			disableMainMenuMode();
			setupLoadMode(1);
		}

		return 0;
	}

	if(inGameMenuMode()){
		return gameMenuLoop(hwnd, msg, wParam, lParam);
	}


	//Ignore inputs until drawing is finished.
	if(postMoveMode){
		return 0;
	}

	if(inWorldMapMode()){
		return worldMapLoop(hwnd, msg, wParam, lParam, &main_field, player, thisGroupContainer, viewShift, mapDirectory, &inActionMode);
	}else if(isSpecialDrawModeEnabled()){
		specialDrawLoop(hwnd, msg, wParam, lParam);
		return 0;
	}else if(shouldDrawDialogBox()){
		dialogLoop(hwnd, msg, wParam, lParam, player, thisGroupContainer, main_field, &inActionMode);
		return 0;
	}else if(inAbilityViewMode()){
		return abilityViewLoop(hwnd, msg, wParam, lParam, player, viewShift, main_field);
	}else if (inLookViewScrollMode()) {
		return lookViewScrollLoop(hwnd, msg, wParam, lParam);
	}else if (inCharacterInfoView()){
		return characterInfoViewLoop(hwnd, msg, wParam, lParam);
	}else if (inCursorMode()) {
		cursorLoop(hwnd, msg, wParam, lParam, main_field, player, thisGroupContainer, viewShift, &inActionMode, &playerControlMode, animateMoveSpeed);
		return 0;
	} else if(moveMode){
		if(initMoveMode){
			initMoveMode = 0;
			player->thisMoveNodeMeta->sum = 0;
			player->thisMoveNodeMeta->pathLength = 0;
			player->thisMoveNodeMeta->shadowCharacter->x = player->playerCharacter->x;
			player->thisMoveNodeMeta->shadowCharacter->y = player->playerCharacter->y;

			player->thisMoveNodeMeta->rootMoveNode = malloc(sizeof(moveNode));
			player->thisMoveNodeMeta->rootMoveNode->x = player->playerCharacter->x;
			player->thisMoveNodeMeta->rootMoveNode->y = player->playerCharacter->y;
			player->thisMoveNodeMeta->rootMoveNode->nextMoveNode = NULL;
			player->thisMoveNodeMeta->rootMoveNode->hasTraversed = 1;

			viewShift->xShiftOld = viewShift->xShift;
			viewShift->yShiftOld = viewShift->yShift;
		}

		moveLoop(hwnd, msg, wParam, lParam, &moveMode, main_field, player, thisGroupContainer, &postMoveMode, viewShift, animateMoveSpeed);
		return 0;
	} else if(inInventoryViewMode()){
		inventoryLoop(hwnd, msg, wParam, lParam, main_field, player, thisGroupContainer, viewShift, &inActionMode);
		return 0;
	}else if(thisGroupContainer != NULL && thisGroupContainer->groupMoveMode){
		PostMessage(hwnd, WM_MOUSEACTIVATE, wParam, lParam);
		return 0;
	}else if(thisGroupContainer != NULL && thisGroupContainer->groupActionMode){
		processActionLoop(hwnd, msg, wParam, lParam, player, thisGroupContainer, main_field, &inActionMode, &playerControlMode, animateMoveSpeed);
		PostMessage(hwnd, WM_MOUSEACTIVATE, wParam, lParam);
		return 0;
	}else if(thisGroupContainer != NULL && thisGroupContainer->postGroupActionMode){
		thisGroupContainer->postGroupActionMode = 0;

		if(thisGroupContainer->selectedGroup->individuals[thisGroupContainer->selectedGroup->currentIndividualIndex] != NULL ){

			if(thisGroupContainer->selectedGroup->individuals[thisGroupContainer->selectedGroup->currentIndividualIndex]->remainingActions > 0){
				thisGroupContainer->groupActionMode = 1;
				PostMessage(hwnd, WM_MOUSEACTIVATE, wParam, lParam);
				return 0;
			} else {
				endTurn(thisGroupContainer->selectedGroup->individuals[thisGroupContainer->selectedGroup->currentIndividualIndex]);
			}
		}

		//determine if need to go back into this group's action mode
		nextAvailableIndividualIndex(thisGroupContainer->selectedGroup);

		if(thisGroupContainer->selectedGroup->currentIndividualIndex > -1){
			thisGroupContainer->groupActionMode = 1;
			thisGroupContainer->initGroupActionMode = 1;
			PostMessage(hwnd, WM_MOUSEACTIVATE, wParam, lParam);
			return 0;
		}else{
			if(!setNextActiveGroup(thisGroupContainer)){

//				QueryPerformanceCounter(&EndingTime);
//				ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
//
//				ElapsedMicroseconds.QuadPart *= 1000000;
//				ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
//
//				char outLog[256];
//				sprintf(outLog, "End group process: %llu",ElapsedMicroseconds.QuadPart);
//				cwrite(outLog);

				if(startTurn(player)){

				}

				//If not in action mode, give player actions back, dont start AI turn
				if(!inActionMode){
					player->remainingActions = player->totalActions;
				}else if(player->remainingActions <= 0){
					endTurn(player);
					thisGroupContainer->groupActionMode = 1;
					thisGroupContainer->initGroupActionMode = 1;
					setNextActiveGroup(thisGroupContainer);
					PostMessage(hwnd, WM_MOUSEACTIVATE, wParam, lParam);
					return 0;
				}else if(hasActiveStatusEffect(player, STATUS_BERZERK) || hasActiveStatusEffect(player, STATUS_SLEEPING)){
					playerControlMode = 1;
					PostMessage(hwnd, WM_MOUSEACTIVATE, wParam, lParam);
					return 0;
				}
			}else{
				thisGroupContainer->groupActionMode = 1;
				thisGroupContainer->initGroupActionMode = 1;
				PostMessage(hwnd, WM_MOUSEACTIVATE, wParam, lParam);
				return 0;
			}
		}
	}else if(playerControlMode){
		processPlayerControlledLoop(hwnd, msg, wParam, lParam, player, thisGroupContainer, main_field,&inActionMode, &postMoveMode, &playerControlMode, &postPlayerControlMode);
		PostMessage(hwnd, WM_MOUSEACTIVATE, wParam, lParam);
		return 0;
	}else if(postPlayerControlMode){
		postPlayerControlMode = 0;

		if(player->remainingActions > 0 && (hasActiveStatusEffect(player, STATUS_BERZERK) || hasActiveStatusEffect(player, STATUS_SLEEPING))){
			playerControlMode = 1;
		}

		if(!inActionMode){
			player->remainingActions = player->totalActions;
		}else if(player->remainingActions <= 0){
			endTurn(player);
			thisGroupContainer->groupActionMode = 1;
			thisGroupContainer->initGroupActionMode = 1;
			setNextActiveGroup(thisGroupContainer);
		}

		PostMessage(hwnd, WM_MOUSEACTIVATE, wParam, lParam);
		return 0;
	}else {
		return mainLoop(hwnd, msg, wParam, lParam);
	}
}

//Note: The returned value is significant to the calling function. Therefore, only the first event result is returned.
//All remaining event results are ignored.
int triggerEvent(int eventID){
	int toReturn = processEvent(eventID, player, thisGroupContainer, main_field, &inActionMode);
	int nextEventID = getNextEventID(eventID);

	while(nextEventID){
		processEvent(nextEventID, player, thisGroupContainer, main_field, &inActionMode);
		nextEventID = getNextEventID(nextEventID);
	}

	return toReturn;
}

char * getContextData(char * contextKey){
	return processContextKey(contextKey, player, thisGroupContainer, main_field);
}

void runTests(){
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

	initLockAuth();
	initalizeTheGlobalRegister();
	initEventHandlers();
	initAbilityCreationInstance(1424,RGB(255, 0, 255), 10, 10, mapTestDirectory, "test_effects_template.txt");
	initThisAbilityView(9504, RGB(255, 0, 255), 10, 10);
	initNameBoxInstance(9503, RGB(255,0,255), 20, 20);
	loadTriggerMaps(mapTestDirectory, "test_onAttackTriggerMap.txt","test_onHarmTriggerMap.txt","test_onDeathTriggerMap.txt", "test_onPickupTriggerMap.txt");

	loadGlobalRegister(mapTestDirectory, mapTestDirectory, "test_individuals.txt", "test_items.txt", "test_events.txt", "sounds.txt", "images.txt", "permenant_abilities.txt", "duration_abilities.txt", "targeted_abilities.txt", "instant_abilities.txt", "test_mapInfo.txt", "descriptionLookup.txt", "interactableObjects.txt", "areaNodes.txt");
	loadDialog("dialog.txt", mapTestDirectory);
	setAbilityCreationIDCounter(1000 + numAbilitiesInGlobalRegistry());

	initThisCursor(1508);
	initSoundPlayerInstance();
	initLookView(1519, 1520);
	initCharacterInfoView();
	initPauseView(1523);
	initWorldMapController(10003);

	animationContainer * playerAnimationContainer = initAnimationContainer();
	animationContainer * secondaryAnimationContainer = NULL;

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

	player = getIndividualFromRegistry(2);

	main_field = loadMap("test_map1.txt", mapTestDirectory, player, thisGroupContainer);
	initFieldGraphicContainer();
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
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow) {

	HWND hwnd;

	WNDCLASSEX wc = {0};

	MSG Msg;

	int consoleWindowWidth = 480;
	int consoleWindowHeight = 175;
	int sidebarWindowWidth = 175;
	int sidebarWindowHeight = 655;

	QueryPerformanceFrequency(&Frequency);

	//run the tests!
	//init rand for tests
	srand(0);
	int i;

	if(!MAP_CREATION_MODE){
		runTests();
	}

	srand(time(NULL));
	for(i = 0; i < 10; i++){
		printf("%d, ", rand()%100);
	}

	//step 1: registering the window class
	wc.cbSize = sizeof(WNDCLASSEX); //Size of the structure
	wc.style = 0; //Class styles (usually zero)

	if(MAP_CREATION_MODE){
		wc.lpfnWndProc = MapGeneratorProc; //Pointer to the window procedure for this window class
	}else{
		wc.lpfnWndProc = WndProc; //Pointer to the window procedure for this window class
	}

	wc.cbClsExtra = 0; //amount of extra data for this class in memory
	wc.cbWndExtra = 0; //amount of extra data per window of this type
	wc.hInstance = hInstance; //handle to app instance (input param)
	wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON)); //Icon shown when user presses alt+tab
	wc.hCursor = LoadCursorA(NULL, IDC_ARROW); //cursor that will be displayed over win.
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1); //background brush to set color of window
	//wc.lpszMenuName = MAKEINTRESOURCE(IDR_MYMENU);
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
	// NOTE: triggers WM_CREATE
	hwnd_global = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName, "Natural 1",
	WS_OVERLAPPEDWINDOW,
	CW_USEDEFAULT,
	CW_USEDEFAULT, mainWindowWidth, mainWindowHeight,
	NULL, NULL, hInstance,
	NULL);

	hwnd = hwnd_global;

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
	while (GetMessage(&Msg, NULL, 0, 0) > 0) { //GetMessage
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
