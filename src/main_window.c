/*
 * hi.c
 *
 *  Created on: Feb 12, 2015
 *      Author: Adrian
 */

#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include "./headers/general.h"
#include "./headers/main_window.h"
//#include "./headers/character_pub_methods.h"
#include "./headers/field_pub_methods.h"
#include "./headers/cursor_pub_methods.h"

const char g_szClassName[] = "MyWindowClass";
const int rateOfChange_player_x = 8;
const int rateOfChange_player_y = 8;
int numMessages = 0;
int mouseButtonCount = 0;
//HBITMAP g_hbmPlayer = NULL;
HBITMAP g_hbmPlayerMask = NULL;
HWND g_toolbar = NULL;
int cursorMode = 0;
int initCursorMode = 0;

individual* player;
individual* skeleton;
cursor* thisCursor;
field* main_field;

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

HBITMAP CreateBitmapMask(HBITMAP hbmColor, COLORREF crTransparent) {
	HDC hdcMemColor, hdcMemMask;
	HBITMAP hbmMask;
	BITMAP bm;

	//create a monochrome mask bitmap (1 bit)

	GetObject(hbmColor, sizeof(BITMAP), &bm);
	hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

	//Get some graphic handlers that are compatable with the display driver

	hdcMemColor = CreateCompatibleDC(0);
	hdcMemMask = CreateCompatibleDC(0);

	SelectObject(hdcMemColor, hbmColor);
	SelectObject(hdcMemMask, hbmMask);

	//Set the background color of the color image to the color
	//we want transparent
	SetBkColor(hdcMemColor, crTransparent);

	//Copy the bits from the color image to the black/white mask
	//everything with the background color ends up white
	//while everything else is black

	BitBlt(hdcMemMask, 0, 0, bm.bmWidth, bm.bmHeight, hdcMemColor, 0, 0,
			SRCCOPY);

	//Take mask and use it to turn transparent color in the original
	//color image to black so the transparency effect will work

	BitBlt(hdcMemColor, 0, 0, bm.bmWidth, bm.bmHeight, hdcMemMask, 0, 0,
			SRCINVERT);

	//clean up

	DeleteDC(hdcMemColor);
	DeleteDC(hdcMemMask);

	return hbmMask;
}

void drawAll(HDC hdc, RECT* prc) {
	HDC hdcBuffer = CreateCompatibleDC(hdc);
	HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, prc->right, prc->bottom);
	HBITMAP hbmOldBuffer = SelectObject(hdcBuffer, hbmBuffer); //copy of hbmBuffer


	drawField(hdc,hdcBuffer, main_field);
	if(player->hp > 0){
		drawPlayer(hdc,hdcBuffer, player);
	}
	if(skeleton->hp > 0){
		drawPlayer(hdc,hdcBuffer, skeleton);
	}
	if(cursorMode){
		drawCursor(hdc,hdcBuffer,thisCursor);
	}

	BitBlt(hdc, 0, 0, prc->right, prc->bottom, hdcBuffer, 0, 0, SRCCOPY);

	SelectObject(hdcBuffer, hbmOldBuffer);
	DeleteDC(hdcBuffer);
	DeleteObject(hbmBuffer);
}

int mainLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch (msg) {
	case WM_CREATE: {
		BITMAP bm;
		UINT ret;

		player = initIndividual();
		skeleton = initIndividual();

//		player = malloc(sizeof(individual));
//		player->playerCharacter = malloc(sizeof(character));
//		player->name = malloc(sizeof(char)*32);
		strcpy(player->name, "adrian");

//		skeleton = malloc(sizeof(individual));
//		skeleton->playerCharacter = malloc(sizeof(character));
//		skeleton->name = malloc(sizeof(char)*32);
		strcpy(skeleton->name, "skelly");

		thisCursor = malloc(sizeof(cursor));
		thisCursor->cursorCharacter = malloc(sizeof(character));
		int x;
		int y;
		main_field = initField("C:\\Users\\Adrian\\workspace\\Natural_1\\src\\map1.txt");
		int imageID;
		int isPassable;
		for (y = 0; y < main_field->totalY; y++) {
//			printf("b\n");
			for (x = 0; x < main_field->totalX; x++) {
				imageID = (main_field->grid[x][y]->background)->imageID;
				isPassable = (main_field->grid[x][y])->isPassable;
//				printf("is Passable:%d\n", isPassable);
				main_field->grid[x][y]->background->image = malloc(sizeof(HBITMAP));
				main_field->grid[x][y]->background->image = LoadBitmap(GetModuleHandle(NULL), imageID);
				if(main_field->grid[x][y]->background->image == NULL){
					printf("failed\n");
				}

			}
		}

		//player->image = NULL;
		player->playerCharacter->imageID = 2001;
//		player->playerCharacter->image = malloc(sizeof(HBITMAP));
		player->playerCharacter->image = LoadBitmap(GetModuleHandle(NULL),
				MAKEINTRESOURCE(player->playerCharacter->imageID));

		if (player->playerCharacter->image == NULL) {
			MessageBox(hwnd, "Failed to make player", "Notice",
			MB_OK | MB_ICONINFORMATION);
		}

		player->playerCharacter->imageMask = CreateBitmapMask(player->playerCharacter->image, RGB(255, 70, 255)); //transparent is black

		GetObjectA(player->playerCharacter->image, sizeof(bm), &bm);

		player->playerCharacter->height = bm.bmHeight;
		player->playerCharacter->width = bm.bmWidth;

		player->playerCharacter->x = 0;
		player->playerCharacter->y = 0;
		player->hp = 10;
		player->maxDam = 5;



		skeleton->playerCharacter->imageID = 2005;
//		skeleton->playerCharacter->image = malloc(sizeof(HBITMAP));
		skeleton->playerCharacter->image = LoadBitmap(GetModuleHandle(NULL),
				MAKEINTRESOURCE(skeleton->playerCharacter->imageID));

		if (skeleton->playerCharacter->image == NULL) {
			MessageBox(hwnd, "Failed to make skeleton", "Notice",
			MB_OK | MB_ICONINFORMATION);
		}

		skeleton->playerCharacter->imageMask = CreateBitmapMask(skeleton->playerCharacter->image, RGB(255, 0, 255)); //transparent is black

		GetObjectA(skeleton->playerCharacter->image, sizeof(bm), &bm);

		skeleton->playerCharacter->height = bm.bmHeight;
		skeleton->playerCharacter->width = bm.bmWidth;

		skeleton->playerCharacter->x = 1;
		skeleton->playerCharacter->y = 0;
		skeleton->hp=10;
		skeleton->maxDam = 3;

		printf("1,0:%p, 2,0:%p\n", getSpaceFromField(main_field,1,0)->currentIndividual,  getSpaceFromField(main_field,2,0)->currentIndividual);
		printf("+++ skeleton x:%d, y:%d\n",skeleton->playerCharacter->x, skeleton->playerCharacter->y);
		moveIndividual(main_field,skeleton,6);
		printf("1,0:%p, 2,0:%p\n", getSpaceFromField(main_field,1,0)->currentIndividual,  getSpaceFromField(main_field,2,0)->currentIndividual);
		printf("--- skeleton x:%d, y:%d\n",skeleton->playerCharacter->x, skeleton->playerCharacter->y);
//		getSpaceFromField(main_field, 1, 0)->currentIndividual = skeleton;




		thisCursor->cursorCharacter->imageID = 2004;
		thisCursor->cursorCharacter->image = malloc(sizeof(HBITMAP));
		thisCursor->cursorCharacter->image = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(thisCursor->cursorCharacter->imageID));
		thisCursor->cursorCharacter->x = 0;
		thisCursor->cursorCharacter->y = 0;
		thisCursor->cursorCharacter->width = 40;
		thisCursor->cursorCharacter->height = 40;
		thisCursor->cursorCharacter->imageMask = CreateBitmapMask(thisCursor->cursorCharacter->image, RGB(224,64,192));


		ret = SetTimer(hwnd, ID_TIMER, 50, NULL); //fires every 50 ms!

		if (ret == 0) {
			MessageBox(hwnd, "Could not SetTimer()!", "Error",
					MB_OK | MB_ICONEXCLAMATION);
		}
//				hfDefault = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
//				SendMessage(hEdit, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM(FALSE, 0));
//				SendMessage(hButton, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM(FALSE, 0));
	}
		break;
	case WM_PAINT: //NOTE: NEVER USE MESSAGES IN A WM_PAINT LOOP, AS IT WILL
	{			   //SPAWN MORE MESSAGES!

		RECT rect;
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		GetClientRect(hwnd, &rect);
		drawAll(hdc,&rect);
//		drawPlayer(hdc, &rect);

		EndPaint(hwnd, &ps);
	}
		break;
	case WM_TIMER: {
		RECT rect;
		HDC hdc = GetDC(hwnd);
		GetClientRect(hwnd, &rect);
		drawAll(hdc,&rect);
//			updatePlayer(&rect);
//		drawPlayer(hdc, &rect);

		ReleaseDC(hwnd, hdc);
	}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
//			DeleteObject(g_hbmPlayer);
		DeleteObject(g_hbmPlayerMask);
		destroyIndividual(player);

		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_KEYDOWN: {
		switch (LOWORD(wParam)) {
		case 0x34: //left
		case 0x64:
			moveIndividual(main_field, player,4);
			break;
		case 0x36: //right
		case 0x66:
			moveIndividual(main_field, player,6);
			break;
		case 0x38: //up
		case 0x68:
			moveIndividual(main_field, player,8);
			break;
		case 0x32: //down
		case 0x62:
			moveIndividual(main_field, player,2);
			break;
		case 0x31: //down left
		case 0x61:
			moveIndividual(main_field, player,1);
			break;
		case 0x37: //up left
		case 0x67:
			moveIndividual(main_field, player,7);
			break;
		case 0x39: //up right
		case 0x69:
			moveIndividual(main_field, player,9);
			break;
		case 0x33: //down right
		case 0x63:
			moveIndividual(main_field, player,3);
			break;
		case 0x41:
			cursorMode = 1;
			initCursorMode = 1;
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

int cursorLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch (msg) {
	case WM_KEYDOWN: {
			switch (LOWORD(wParam)) {
			case 0x34: //left
			case 0x64:
				moveCursor(main_field, thisCursor,4);
//				thisCursor->cursorCharacter->x -= 40;
				break;
			case 0x36:
			case 0x66:
				moveCursor(main_field, thisCursor, 6);
//				thisCursor->cursorCharacter->x += 40;
				break;
			case 0x38:
			case 0x68:
				moveCursor(main_field, thisCursor,8);
//				thisCursor->cursorCharacter->y -= 40;
				break;
			case 0x32:
			case 0x62:
				moveCursor(main_field, thisCursor,2);
//				thisCursor->cursorCharacter->y += 40;
				break;
			case 0x31: //down left
			case 0x61:
				moveCursor(main_field, thisCursor,1);
				break;
			case 0x37: //up left
			case 0x67:
				moveCursor(main_field, thisCursor,7);
				break;
			case 0x39: //up right
			case 0x69:
				moveCursor(main_field, thisCursor,9);
				break;
			case 0x33: //down right
			case 0x63:
				moveCursor(main_field, thisCursor,3);
				break;
			case 0x1B: //escape
				cursorMode = 0;
				break;
			case 0x0D: //enter
				{
					int cX,cY;
					cX = thisCursor->cursorCharacter->x;
					cY = thisCursor->cursorCharacter->y;

//					individual * tmp = getIndividualFromField(main_field, thisCursor->cursorCharacter->x,thisCursor->cursorCharacter->y);

					printf("tmp:%p, skeleton:%p -- %d,%d\n",getIndividualFromField(main_field,cX,cY),skeleton,thisCursor->cursorCharacter->x,thisCursor->cursorCharacter->y);

					if(getIndividualFromField(main_field,cX,cY)	== skeleton){
						printf("attacked!");
						attackIndividual(player,skeleton);
						cursorMode = 0;
					}

//					destroyIndividual(tmp);
				}
				break;
			}
			case WM_TIMER: {
				RECT rect;
				HDC hdc = GetDC(hwnd);
				GetClientRect(hwnd, &rect);
				drawAll(hdc,&rect);

				ReleaseDC(hwnd, hdc);
			}
			break;
			case WM_CLOSE:
				DestroyWindow(hwnd);
				break;
			case WM_DESTROY:
				DeleteObject(g_hbmPlayerMask);
				destroyIndividual(player);
				destroyCursor(thisCursor);
				PostQuitMessage(0);
				break;
			default:
				return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if(cursorMode){
		if(initCursorMode){
			printf("playerX:%d\n",player->playerCharacter->x);
			thisCursor->cursorCharacter->x = player->playerCharacter->x;
			thisCursor->cursorCharacter->y = player->playerCharacter->y;
			initCursorMode = 0;
		}

		return cursorLoop(hwnd, msg, wParam, lParam);
	}else{
		return mainLoop(hwnd, msg, wParam, lParam);
	}

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow) {

	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

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

	//create the window (handle)
	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_szClassName, "Window Title :D!",
	WS_OVERLAPPEDWINDOW,
	CW_USEDEFAULT,
	CW_USEDEFAULT, 240, 360,
	NULL, NULL, hInstance,
	NULL);

	//ALWAYS CHECK THE RETURN VALUES!!!
	if (hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!", "Error!",
				MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow); //display window

	g_toolbar =
			CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_TOOLBAR), hwnd, ToolDlgProc);
	if (g_toolbar != NULL) {
		ShowWindow(g_toolbar, SW_SHOW);
	} else {
		MessageBox(hwnd, "Could not create the toolbar dialog", "Warning!",
				MB_OK | MB_ICONINFORMATION);
	}
	ShowWindow(g_toolbar, SW_HIDE);

	UpdateWindow(hwnd); //redraw

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
//		if (!IsDialogMessageA(g_toolbar,&Msg)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
//		}
	}

	return Msg.wParam;

}
