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

const char g_szClassName[] = "MyWindowClass";
const char  g_szClassNameCons[] = "MyConsoleClass";
const char  g_szClassNameSideBar[] = "MySideBarClass";
int numMessages = 0;
int mouseButtonCount = 0;

HWND g_sidebar = NULL;
HWND g_toolbar = NULL;
HWND eConsole = NULL;
int cursorMode = 0;
int initCursorMode = 0;
int postCursorMode = 0;

int moveMode = 0;
int initMoveMode = 0;
int postMoveMode = 0;

int enemyTurn = 0;

individual* player;

enemies* thisEnemies;
cursor* thisCursor;
field* main_field;
moveNodeMeta * thisMoveNodeMeta;

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
		DestroyWindow(g_toolbar);
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


void drawAll(HDC hdc, RECT* prc) {
	HDC hdcBuffer = CreateCompatibleDC(hdc);
	HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, prc->right, prc->bottom);
	HBITMAP hbmOldBuffer = SelectObject(hdcBuffer, hbmBuffer); //copy of hbmBuffer
	int index;
	PAINTSTRUCT ps;
	HDC consoleHDC = BeginPaint(g_toolbar, &ps);

	RECT rec;
	 //SetRect(&rec,10,10,100,100);
//	GetClientRect(g_toolbar, &rec);

//	if(!UpdateWindow(g_sidebar)){
//		printf("failed!!!\n");
//	}
//	UpdateWindow(g_toolbar);

//	EndPaint(g_toolbar, &ps);
//	ReleaseDC(g_toolbar,consoleHDC);



	drawField(hdc, hdcBuffer, main_field);
	if (player->hp > 0) {
		drawIndividual(hdc, hdcBuffer, player);
	}

	for(index = 0; index < thisEnemies->numEnemies; index++){
		drawIndividual(hdc, hdcBuffer, thisEnemies->enemies[index]);
	}

	if (cursorMode) {
		drawCursor(hdc, hdcBuffer, thisCursor);
	}

	if (moveMode){
		moveNode * tmp = thisMoveNodeMeta->rootMoveNode;
		character * tmpCharacter = (thisMoveNodeMeta->shadowCharacter);
		while(tmp->nextMoveNode != NULL){

			drawUnboundCharacter(hdc, hdcBuffer, tmp->x,tmp->y, tmpCharacter);
			tmp = (moveNode*)tmp->nextMoveNode;
		}

		drawUnboundCharacter(hdc, hdcBuffer,tmp->x,tmp->y,tmpCharacter);

	}

	BitBlt(hdc, 0, 0, prc->right, prc->bottom, hdcBuffer, 0, 0, SRCCOPY);

	SelectObject(hdcBuffer, hbmOldBuffer);
	DeleteDC(hdcBuffer);
	DeleteObject(hbmBuffer);
}

void AppendText(HWND hwnd, TCHAR *newText)
{
    // get edit control from dialog
    HWND hwndOutput = hwnd;

    // get the current selection
    DWORD StartPos, EndPos;
    SendMessage( hwndOutput, EM_GETSEL, &StartPos, &EndPos);

    // move the caret to the end of the text
    int outLength = GetWindowTextLength( hwndOutput );
    SendMessage( hwndOutput, EM_SETSEL, outLength, outLength );

    // insert the text at the new caret position
    SendMessage( hwndOutput, EM_REPLACESEL, TRUE, newText);

    // restore the previous selection
    SendMessage( hwndOutput, EM_SETSEL, StartPos, EndPos );
}

int mainLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CREATE: {
		BITMAP bm;
		UINT ret;

		player = initIndividual();
		individual* skeleton = initIndividual();
		individual* skeleton2 = initIndividual();
		thisEnemies = initEnemies();
		thisCursor = initCursor(2004,RGB(224, 64, 192),0,0);

		if (defineIndividual(player, 2001, RGB(255, 70, 255), "adr", 0, 0, 20, 2, 10, 0, 2, 4)) {
			MessageBox(hwnd, "Failed to make player", "Notice",
			MB_OK | MB_ICONINFORMATION);
		}

		if (defineIndividual(skeleton, 2005, RGB(255, 0, 255), "skelly", 10, 1, 8, 2, 3, 1, 1, 3)) {
			MessageBox(hwnd, "Failed to make player", "Notice",
			MB_OK | MB_ICONINFORMATION);
		}

		if (defineIndividual(skeleton2, 2005, RGB(255, 0, 255), "skelly2", 10, 2, 2, 2, 3, 1, 1, 3)) {
			MessageBox(hwnd, "Failed to make player", "Notice",
			MB_OK | MB_ICONINFORMATION);
		}

		addEnemyToEnemies(thisEnemies,skeleton2);
		addEnemyToEnemies(thisEnemies,skeleton);

		int x, y;
		main_field = initField("C:\\Users\\Adrian\\workspace\\Natural_1\\src\\map1.txt");
		int imageID;

		for (y = 0; y < main_field->totalY; y++) {
			for (x = 0; x < main_field->totalX; x++) {
				imageID = (main_field->grid[x][y]->background)->imageID;
				main_field->grid[x][y]->background->image = malloc(
						sizeof(HBITMAP));
				main_field->grid[x][y]->background->image = LoadBitmap(
						GetModuleHandle(NULL), imageID);
				if (main_field->grid[x][y]->background->image == NULL) {
					printf("failed\n");
				}

			}
		}

		setIndividualSpace(main_field,player, 0,0);
		setIndividualSpace(main_field,skeleton, 10,0);
		setIndividualSpace(main_field,skeleton2,10,1);

		ret = SetTimer(hwnd, ID_TIMER, 50, NULL); //fires every 50 ms!

		if (ret == 0) {
			MessageBox(hwnd, "Could not SetTimer()!", "Error",
			MB_OK | MB_ICONEXCLAMATION);
		}

	}
		break;
	case WM_PAINT: //NOTE: NEVER USE MESSAGES IN A WM_PAINT LOOP, AS IT WILL
	{			   //SPAWN MORE MESSAGES!

		RECT rect;
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		GetClientRect(hwnd, &rect);
		drawAll(hdc, &rect);
//		drawPlayer(hdc, &rect);

		EndPaint(hwnd, &ps);
	}
		break;
	case WM_TIMER: {
		RECT rect;
		HDC hdc = GetDC(hwnd);
		GetClientRect(hwnd, &rect);
		drawAll(hdc, &rect);
//			updatePlayer(&rect);
//		drawPlayer(hdc, &rect);

		ReleaseDC(hwnd, hdc);
	}
		break;
	case WM_CLOSE:
		KillTimer(hwnd,ID_TIMER);
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		destroyIndividual(player);

		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_KEYDOWN: {
		switch (LOWORD(wParam)) {

		case 0x41: //a key (attack)
			printf("starting \n");
			cursorMode = 1;
			initCursorMode = 1;
			printf("pressed a\n");
			break;
		case 0x53: //s key (move)
			moveMode = 1;
			initMoveMode = 1;
			break;
		case 0x57: //w key (wait)
			player->remainingActions = player->remainingActions - 1;

			if (player->remainingActions <= 0) {
				endTurn(player);
				int i;
				for(i = 0; i < thisEnemies->numEnemies; i++){
					enemyAction((thisEnemies->enemies[i]), main_field, player);
				}
			}

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
		case ID_DIALOG_SHOW:
			ShowWindow(g_toolbar, SW_SHOW);
			break;
		case ID_DIALOG_HIDE:
			ShowWindow(g_toolbar, SW_HIDE);
			break;
		}
		break;
	default:
//			printf("msg:%d, count:%d\n",msg,numMessages);
		numMessages++;
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

			GetClientRect(hwnd, &rec);

			DrawSideBar(hwnd,hdc, rec, player);


			EndPaint(hwnd,&ps);
			ReleaseDC(hwnd, hdc);
		}
		break;
		case WM_TIMER:
		{
//			printf("got the time!\n");
			RECT rec;
			PAINTSTRUCT ps;
			HDC hdc = GetDC(hwnd);

			GetClientRect(hwnd, &rec);

			DrawSideBar(hwnd,hdc, rec, player);


			EndPaint(hwnd,&ps);
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


LRESULT CALLBACK ConsWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg)
	{
		case WM_CREATE:
		{
			RECT rec;
			GetClientRect(hwnd,&rec);

			eConsole = CreateWindowEx(0, "EDIT", "",
				WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_READONLY | ES_MULTILINE | ES_AUTOVSCROLL, //ES_READONLY
				0, 0, rec.right, rec.bottom, hwnd, NULL, GetModuleHandle(NULL), NULL);

			if(eConsole == NULL)
				MessageBox(hwnd, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);

			SendMessage(eConsole, WM_SETFONT, DEFAULT_GUI_FONT, MAKELPARAM(FALSE, 0));

			SetWindowText(eConsole, TEXT("The quick brown fox jumped over the lazy dog. He jumped quite high over him.\r\n"));
			int i = 0;
			for(; i < 100; i++){
				AppendText(eConsole,"JUMP!\r\n");
				SendMessage(eConsole, EM_LINESCROLL, 0, 1100);//update this
			}
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

	if (cursorMode) {
		if (initCursorMode) {
			printf("playerX:%d\n", player->playerCharacter->x);
			thisCursor->cursorCharacter->x = player->playerCharacter->x;
			thisCursor->cursorCharacter->y = player->playerCharacter->y;
			initCursorMode = 0;

		}

		return cursorLoop(hwnd, msg, wParam, lParam, &cursorMode, &postCursorMode, thisCursor, main_field, player, thisEnemies);
	} else if(postCursorMode){
		postCursorMode = 0;

		player->remainingActions = player->remainingActions - 1;

		if (player->remainingActions <= 0) {
			endTurn(player);
			int i;
			for (i = 0; i < thisEnemies->numEnemies; i++) {
				enemyAction((thisEnemies->enemies[i]), main_field, player);
			}
		}

	} else if(moveMode){


		if(initMoveMode){
			initMoveMode = 0;
			character * shadowCharacter = createCharacter(player->playerCharacter->imageID,player->playerCharacter->rgb,
								player->playerCharacter->x, player->playerCharacter->y);

			thisMoveNodeMeta = malloc(sizeof(moveNodeMeta));
			thisMoveNodeMeta->shadowCharacter = malloc(sizeof(character));
			thisMoveNodeMeta->rootMoveNode = malloc(sizeof(moveNode));
			thisMoveNodeMeta->sum = 0;
			thisMoveNodeMeta->pathLength = 0;
			thisMoveNodeMeta->shadowCharacter = shadowCharacter;

			printf("x:%d,y:%d\n",thisMoveNodeMeta->shadowCharacter->x, thisMoveNodeMeta->shadowCharacter->y);

			thisMoveNodeMeta->rootMoveNode->x = player->playerCharacter->x;
			thisMoveNodeMeta->rootMoveNode->y = player->playerCharacter->y;
			thisMoveNodeMeta->rootMoveNode->nextMoveNode = NULL;
			thisMoveNodeMeta->rootMoveNode->hasTraversed = 1;

		}

	return moveLoop(hwnd, msg, wParam, lParam, &moveMode, main_field, player, thisMoveNodeMeta, &postMoveMode);
	} else if(postMoveMode){
//		printf("looping in moveMode\n");
		animateMoveLoop(hwnd,msg, wParam, lParam,main_field,player,thisMoveNodeMeta,5,&postMoveMode);
		if (!postMoveMode) {

			freeUpMovePath(thisMoveNodeMeta->rootMoveNode->nextMoveNode);

			player->remainingActions = player->remainingActions - 1;

			if (player->remainingActions <= 0) {
				endTurn(player);
				int i;
				for(i = 0; i < thisEnemies->numEnemies; i++){
					enemyAction((thisEnemies->enemies[i]), main_field, player);
				}
			}

			free(thisMoveNodeMeta->rootMoveNode);
			free(thisMoveNodeMeta);
		}

		return 0;
	} else {
		return mainLoop(hwnd, msg, wParam, lParam);
	}

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow) {

	WNDCLASSEX wc;
	WNDCLASSEX wc2;
	WNDCLASSEX wc3;
	HWND hwnd;
	MSG Msg;
	srand(time(NULL));
	int mainWindowWidth = 480;
	int mainWindowHeight = 480;
	int consoleWindowWidth = 480;
	int consoleWindowHeight = 175;
	int sidebarWindowWidth = 175;
	int sidebarWindowHeight = 655;


	//run the tests!
	test_main();

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
	wc2.lpfnWndProc = ConsWndProc; //Pointer to the window procedure for this window class
	wc2.cbClsExtra = 0; //amount of extra data for this class in memory
	wc2.cbWndExtra = 0; //amount of extra data per window of this type
	wc2.hInstance = hInstance; //handle to app instance (input param)
	wc2.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON)); //Icon shown when user presses alt+tab
	wc2.hCursor = LoadCursorA(NULL, IDC_ARROW); //cursor that will be displayed over win.
	wc2.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1); //background brush to set color of window
//	wc2.lpszMenuName = MAKEINTRESOURCE(IDR_MYMENU);
	; // name of menu resource to use for the windows
	wc2.lpszClassName = g_szClassNameCons; //name to identify class with
	wc2.hIconSm = (HICON) LoadImage(GetModuleHandle(NULL),
	MAKEINTRESOURCE(IDI_MYICON), IMAGE_ICON, 16, 16, 0); //small icon to show in taskbar

	if (!RegisterClassEx(&wc2)) {
		MessageBox(NULL, "Window Registration Failed!", "Error",
		MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}


	wc3.cbSize = sizeof(WNDCLASSEX); //Size of the structure
	wc3.style = 0; //Class styles (usually zero)
	wc3.lpfnWndProc = SideBarWndProc; //Pointer to the window procedure for this window class
	wc3.cbClsExtra = 0; //amount of extra data for this class in memory
	wc3.cbWndExtra = 0; //amount of extra data per window of this type
	wc3.hInstance = hInstance; //handle to app instance (input param)
	wc3.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON)); //Icon shown when user presses alt+tab
	wc3.hCursor = LoadCursorA(NULL, IDC_ARROW); //cursor that will be displayed over win.
	wc3.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1); //background brush to set color of window
//	wc2.lpszMenuName = MAKEINTRESOURCE(IDR_MYMENU);
	; // name of menu resource to use for the windows
	wc3.lpszClassName = g_szClassNameSideBar; //name to identify class with
	wc3.hIconSm = (HICON) LoadImage(GetModuleHandle(NULL),
	MAKEINTRESOURCE(IDI_MYICON), IMAGE_ICON, 16, 16, 0); //small icon to show in taskbar

	if (!RegisterClassEx(&wc3)) {
		MessageBox(NULL, "Window Registration Failed!", "Error",
		MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}




	//create the window (handle)
	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName, "Window Title :D!",
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

	g_toolbar = CreateWindowEx(
			0,
			g_szClassNameCons,
			"Console",
			WS_OVERLAPPED,
			CW_USEDEFAULT, CW_USEDEFAULT, consoleWindowWidth, consoleWindowHeight,
			hwnd, NULL, hInstance, NULL);

	if (g_toolbar != NULL) {
		ShowWindow(g_toolbar, SW_SHOW);
	} else {
		MessageBox(hwnd, "Could not create g_toolbar", "Warning!",
		MB_OK | MB_ICONINFORMATION);
	}

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
	MoveWindow(g_toolbar,100,100+mainWindowHeight, consoleWindowWidth, consoleWindowHeight, TRUE);
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
