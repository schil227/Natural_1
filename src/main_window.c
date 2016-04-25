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

const char g_szClassName[] = "MyWindowClass";
const char  g_szClassNameSideBar[] = "MySideBarClass";
const char * mapDirectory = "C:\\Users\\Adrian\\C\\Natural_1_new_repo\\resources\\maps\\";//".\\resources\\maps\\";//
static char * mapTestDirectory = "C:\\Users\\Adrian\\C\\Natural_1_new_repo\\unit_tests\\testMaps\\";

int mainWindowWidth = 640;
int mainWindowHeight = 820;

HWND g_sidebar = NULL;

int moveMode = 0;
int initMoveMode = 0;
int postMoveMode = 0;

int initEnemyActionMode = 0;
int enemyActionMode = 0;
int postEnemyActionMode = 0;

//int initInventoryMode = 0;
//int inventoryMode = 0;

int enemyTurn = 0;

individual* player;

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

	for(index = 0; index < npcs->MAX_INDIVIDUALS; index++){
		int individualsPassed = 0;
		individual * tmp = npcs->individuals[index];

		if(tmp != NULL){
			individualsPassed++;
			drawIndividual(hdc, hdcBuffer, npcs->individuals[index], viewShift);

			if(individualsPassed == npcs->numIndividuals){
				break;
			}
		}
	}

	for(index = 0; index < enemies->MAX_INDIVIDUALS; index++){
		int individualsPassed = 0;
		individual * tmp = enemies->individuals[index];

		if(tmp != NULL){
			individualsPassed++;
			drawIndividual(hdc, hdcBuffer, enemies->individuals[index], viewShift);

			if(individualsPassed == enemies->numIndividuals){
				break;
			}
		}
	}

	//draw animated enemy/npc over others
	if(enemies->currentIndividualIndex != -1){
		drawIndividual(hdc, hdcBuffer, enemies->individuals[enemies->currentIndividualIndex], viewShift);
	}

	if(npcs->currentIndividualIndex != -1){
		drawIndividual(hdc, hdcBuffer, npcs->individuals[npcs->currentIndividualIndex], viewShift);
	}

	if (inCursorMode()) {
		drawCursor(hdc, hdcBuffer, viewShift);
	}

	if (moveMode){
		moveNode * tmp = thisMoveNodeMeta->rootMoveNode;
		character * tmpCharacter = (thisMoveNodeMeta->shadowCharacter);
		while(tmp->nextMoveNode != NULL){

			drawUnboundCharacter(hdc, hdcBuffer, tmp->x,tmp->y, tmpCharacter, viewShift);
			tmp = (moveNode*)tmp->nextMoveNode;
		}

		drawUnboundCharacter(hdc, hdcBuffer,tmp->x,tmp->y,tmpCharacter, viewShift);

	}

	if(inInventoryViewMode()){
		drawInventoryView(hdc, hdcBuffer, viewShift);
	}

	drawThisConsole(hdc,hdcBuffer,prc);

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
		initThisCursor(2004,RGB(224, 64, 192),0,0);
		initThisConsole(2010,0,0,300,200);
		initThisDialogBox(2012,10,10,RGB(255, 70, 255));
		initThisInventoryView(3000, 100, 100, 4, player->backpack);
		initAbilityCreationInstance(3500,RGB(255, 0, 255), 10, 10, mapDirectory, "effects_template.txt");
		initThisAbilityView(3504, RGB(255, 0, 255), 10, 10);
		initNameBoxInstance(3503, RGB(255,0,255), 20, 20);
		initalizeTheGlobalRegister();
		initEventHandlers();
		loadTriggerMaps(mapDirectory, "onAttackTriggerMap.txt","onHarmTriggerMap.txt","onDeathTriggerMap.txt");
		appendNewMessageNode("You leave the forest.");
		appendNewMessageNode("The sun briefly blinds you as you step forth. There's a building in the distance, however it appears to be well guarded by several undead warriors.");

		loadIndividualsToRegistry(mapDirectory,"individuals.txt");
		loadItemsToRegistry(mapDirectory, "items.txt");
		loadEventsToRegistry(mapDirectory, "events.txt");
//		loadEffectsToRegistry(mapDirectory, "effects_template.txt");

		if (defineIndividual(player, 2001, 0, RGB(255, 70, 255), "adr", 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1, 20, 2, 4, 13, 3, 4, 1, 1, "MAX", 2, 3,0,0,0,0,0,0,0,0,0,0,0,0,0,50)) {
			MessageBox(hwnd, "Failed to make player", "Notice",
			MB_OK | MB_ICONINFORMATION);
		}

		dialogMessage * thisMessage = malloc(sizeof(dialogMessage));
		strcpy(thisMessage->message,"I am a message!\0");
		setCurrentMessage(thisMessage);

		int x, y;
		main_field = loadMap("map1.txt", mapDirectory, player, enemies, npcs);
		int imageID;

		for (y = 0; y < main_field->totalY; y++) {
			for (x = 0; x < main_field->totalX; x++) {
				imageID = (main_field->grid[x][y]->background)->imageID;
				main_field->grid[x][y]->background->image = LoadBitmap(
						GetModuleHandle(NULL), imageID);
				if (main_field->grid[x][y]->background->image == NULL) {
					printf("failed\n");
				}

			}
		}

		ret = SetTimer(hwnd, ID_TIMER, 50, NULL); //fires every 50 ms!

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
		case 0x45: //e key (enter)
			{
				attemptToTransit(&main_field, player, enemies, npcs, viewShift, mapDirectory);
			}
			break;
		case 0x47://g key (get)
			{
				attemptGetItemFromField(main_field,player);
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
				refreshCursor(CURSOR_TALK, player->playerCharacter->x, player->playerCharacter->y);
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
		case 0x57: //w key (wait)
			decreaseTurns(player, &enemyActionMode, &initEnemyActionMode, 1);

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

LRESULT CALLBACK SideBarWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg)
	{
		case WM_CREATE:
		{
			UINT ret2;
			ret2 = SetTimer(hwnd, ID_TIMER, 50, NULL);
		}
		break;
		case WM_SIZE:
		{
			HWND hEdit;
			RECT rcClient;

			GetClientRect(hwnd, &rcClient);

			hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
			SetWindowPos(hEdit, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
		}
		break;
		case WM_PAINT:
		{
			RECT rec;
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps); //GetDC(hwnd);
//
			GetClientRect(hwnd, &rec);

			DrawSideBar(hwnd,hdc, rec, player);
//
//
			EndPaint(hwnd,&ps);
			ReleaseDC(hwnd, hdc);
		}
		break;
		case WM_TIMER:
		{
//			printf("got the time!\n");
			RECT rec;
			HDC hdc = GetDC(hwnd);

			GetClientRect(hwnd, &rec);

			DrawSideBar(hwnd,hdc, rec, player);


			ReleaseDC(hwnd, hdc);
		}
		break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
		break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if(shouldDrawDialogBox()){
		return dialogLoop(hwnd, msg, wParam, lParam, player, npcs, enemies, main_field);
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
		if(abilityViewLoop(hwnd, msg, wParam, lParam, player, &enemyActionMode, &initEnemyActionMode)){
			removeIndividualFromField(main_field, player->playerCharacter->x, player->playerCharacter->y);
			triggerEventOnDeath(player->ID);
			removeFromExistance(player->ID);
		}
		return 0;
	}else if (inCursorMode()) {
		return cursorLoop(hwnd, msg, wParam, lParam, &enemyActionMode, &initEnemyActionMode, main_field, player, enemies, npcs, viewShift);
	} else if(moveMode){


		if(initMoveMode){
			initMoveMode = 0;
			character * shadowCharacter = createCharacter(player->playerCharacter->imageID,player->playerCharacter->rgb,
								player->playerCharacter->x, player->playerCharacter->y);

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

			decreaseTurns(player, &enemyActionMode, &initEnemyActionMode, 1);

			free(thisMoveNodeMeta->rootMoveNode);
			destroyCharacter(thisMoveNodeMeta->shadowCharacter);
			free(thisMoveNodeMeta);
		}

		return 0;
	} else if(inInventoryViewMode()){
		inventoryLoop(hwnd, msg, wParam, lParam, main_field, player, enemies, viewShift);
	}else if(enemyActionMode){
		if(enemies->numIndividuals == 0){
			enemyActionMode = 0;
			initEnemyActionMode = 0;

			if(startTurn(player)){

			}

		} else {
			if (initEnemyActionMode) {
				initEnemyActionMode = 0;

				//note:the address of the pointer to thisMoveNodeMeta is passed in because it is malloc'd inside the method
				if(initializeEnemyTurn(enemies, player, main_field, &thisMoveNodeMeta)){
					enemyActionMode = 0;
					postEnemyActionMode = 1;
					return 0;
				}

			}

			//initialization complete, beginning animation loop (once finished, enemyActionMode is toggled)

			animateMoveLoop(hwnd, msg, wParam, lParam, main_field,
					(enemies->individuals[enemies->currentIndividualIndex]),
					thisMoveNodeMeta, 5, &enemyActionMode, viewShift, 0);

			//animation is complete, destroy moveNodeMeta and enter postEnemyActionMode
			if (!enemyActionMode) {

				if (thisMoveNodeMeta != NULL && thisMoveNodeMeta->rootMoveNode != NULL) {
					freeUpMovePath(thisMoveNodeMeta->rootMoveNode);
				}
				free(thisMoveNodeMeta);

				postEnemyActionMode = 1;
			}
		}

	}else if(postEnemyActionMode){
		postEnemyActionMode = 0;

		//attack playerif currentIndividual didn't die and in range
		if(enemies->individuals[enemies->currentIndividualIndex] != NULL){
			enemyAttackAction(enemies->individuals[enemies->currentIndividualIndex],player);

		}
		//determine if need to go back into enemyActionMode
		nextAvailableIndividualIndex(enemies);
		if(enemies->currentIndividualIndex > -1){
			enemyActionMode = 1;
			initEnemyActionMode = 1;
		}else{
			if(startTurn(player)){

			}

			//If player's out of actions, start enemy turn again
			if(player->remainingActions < 0){
				endTurn(player);
				enemyActionMode = 1;
				initEnemyActionMode = 1;
			}
		}

	}else {
		return mainLoop(hwnd, msg, wParam, lParam);
	}

}

int triggerEvent(int eventID){
	processEvent(eventID, player, npcs, enemies, main_field);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow) {

	WNDCLASSEX wc;
	WNDCLASSEX wc2;
	HWND hwnd;
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
	viewShift = initShiftData();
	initThisCursor(2004,RGB(224, 64, 192),0,0);
	initThisConsole(2010,0,0,300,200);
	initThisDialogBox(2012,10,10,RGB(255, 70, 255));
	initalizeTheGlobalRegister();
	initEventHandlers();
	initAbilityCreationInstance(3500,RGB(255, 0, 255), 10, 10, mapTestDirectory, "test_effects_template.txt");
	initThisAbilityView(3504, RGB(255, 0, 255), 10, 10);
	initNameBoxInstance(3503, RGB(255,0,255), 20, 20);
	loadTriggerMaps(mapTestDirectory, "test_onAttackTriggerMap.txt","test_onHarmTriggerMap.txt","test_onDeathTriggerMap.txt");
	loadIndividualsToRegistry(mapTestDirectory,"test_individuals.txt");
	loadItemsToRegistry(mapTestDirectory, "test_items.txt");
	loadEventsToRegistry(mapTestDirectory, "test_events.txt");

	if (defineIndividual(player, 2001, 0, RGB(255, 70, 255), "adr\0", 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 20, 2, 4, 13, 3, 10, 1, 1, "MAX\0", 2, 4,0,0,0,0,0,0,0,0,0,0,0,0,0,50)) {
	}

	main_field = loadMap("test_map1.txt", mapTestDirectory, player, enemies, npcs);

	test_main(player, enemies, npcs, main_field, viewShift);

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

	wc2.cbSize = sizeof(WNDCLASSEX); //Size of the structure
	wc2.style = 0; //Class styles (usually zero)
	wc2.lpfnWndProc = SideBarWndProc; //Pointer to the window procedure for this window class
	wc2.cbClsExtra = 0; //amount of extra data for this class in memory
	wc2.cbWndExtra = 0; //amount of extra data per window of this type
	wc2.hInstance = hInstance; //handle to app instance (input param)
	wc2.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON)); //Icon shown when user presses alt+tab
	wc2.hCursor = LoadCursorA(NULL, IDC_ARROW); //cursor that will be displayed over win.
	wc2.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1); //background brush to set color of window
//	wc2.lpszMenuName = MAKEINTRESOURCE(IDR_MYMENU);
	; // name of menu resource to use for the windows
	wc2.lpszClassName = g_szClassNameSideBar; //name to identify class with
	wc2.hIconSm = (HICON) LoadImage(GetModuleHandle(NULL),
	MAKEINTRESOURCE(IDI_MYICON), IMAGE_ICON, 16, 16, 0); //small icon to show in taskbar

	if (!RegisterClassEx(&wc2)) {
		MessageBox(NULL, "Window Registration Failed!", "Error",
		MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	//create the window (handle)
	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName, "Natural 1",
	WS_OVERLAPPEDWINDOW,
	CW_USEDEFAULT,
	CW_USEDEFAULT, mainWindowWidth, mainWindowHeight,
	NULL, NULL, hInstance,
	NULL);

	//ALWAYS CHECK THE RETURN VALUES!!!
	if (hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!", "Error!",
		MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow); //display window

	g_sidebar = CreateWindowEx(
			0,
			g_szClassNameSideBar,
			"Status",
			WS_OVERLAPPED, //WS_OVERLAPPEDWINDOW
			CW_USEDEFAULT, CW_USEDEFAULT, sidebarWindowWidth, sidebarWindowHeight,
			hwnd, NULL, hInstance, NULL);

	if (g_sidebar != NULL) {
		ShowWindow(g_sidebar, SW_SHOW);
	} else {
		MessageBox(hwnd, "Could not create g_sidebar", "Warning!",
		MB_OK | MB_ICONINFORMATION);
	}

	//ShowWindow(g_toolbar, SW_HIDE);
	MoveWindow(hwnd,100,100, mainWindowWidth, mainWindowHeight, TRUE);
	MoveWindow(g_sidebar,100+mainWindowWidth,100,sidebarWindowWidth, sidebarWindowHeight, TRUE);

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
