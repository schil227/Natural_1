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
//#include "./headers/character_pub_methods.h"
#include "./headers/field_pub_methods.h"
#include "./headers/cursor_pub_methods.h"

const char g_szClassName[] = "MyWindowClass";
const int rateOfChange_player_x = 8;
const int rateOfChange_player_y = 8;
int numMessages = 0;
int mouseButtonCount = 0;
//HBITMAP g_hbmPlayer = NULL;
//HBITMAP g_hbmPlayerMask = NULL;
HWND g_toolbar = NULL;
int * cursorMode;
int initCursorMode = 0;

individual* player;
individual* skeleton;
cursor* thisCursor;
field* main_field;

int trueInt = 1;
int falseInt = 0;

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

//HBITMAP CreateBitmapMask(HBITMAP hbmColor, COLORREF crTransparent) {
//	HDC hdcMemColor, hdcMemMask;
//	HBITMAP hbmMask;
//	BITMAP bm;
//
//	//create a monochrome mask bitmap (1 bit)
//
//	GetObject(hbmColor, sizeof(BITMAP), &bm);
//	hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);
//
//	//Get some graphic handlers that are compatable with the display driver
//
//	hdcMemColor = CreateCompatibleDC(0);
//	hdcMemMask = CreateCompatibleDC(0);
//
//	SelectObject(hdcMemColor, hbmColor);
//	SelectObject(hdcMemMask, hbmMask);
//
//	//Set the background color of the color image to the color
//	//we want transparent
//	SetBkColor(hdcMemColor, crTransparent);
//
//	//Copy the bits from the color image to the black/white mask
//	//everything with the background color ends up white
//	//while everything else is black
//
//	BitBlt(hdcMemMask, 0, 0, bm.bmWidth, bm.bmHeight, hdcMemColor, 0, 0,
//	SRCCOPY);
//
//	//Take mask and use it to turn transparent color in the original
//	//color image to black so the transparency effect will work
//
//	BitBlt(hdcMemColor, 0, 0, bm.bmWidth, bm.bmHeight, hdcMemMask, 0, 0,
//	SRCINVERT);
//
//	//clean up
//
//	DeleteDC(hdcMemColor);
//	DeleteDC(hdcMemMask);
//
//	return hbmMask;
//}

void drawAll(HDC hdc, RECT* prc) {
	HDC hdcBuffer = CreateCompatibleDC(hdc);
	HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, prc->right, prc->bottom);
	HBITMAP hbmOldBuffer = SelectObject(hdcBuffer, hbmBuffer); //copy of hbmBuffer

	drawField(hdc, hdcBuffer, main_field);
	if (player->hp > 0) {
		drawPlayer(hdc, hdcBuffer, player);
	}
	if (skeleton->hp > 0) {
		drawPlayer(hdc, hdcBuffer, skeleton);
	}
	if (*cursorMode) {
		drawCursor(hdc, hdcBuffer, thisCursor);
	}

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
		skeleton = initIndividual();
		thisCursor = initCursor(2004,RGB(224, 64, 192),0,0);

		if (defineIndividual(player, 2001, RGB(255, 70, 255), "adr", 0, 0, 10, 2, 10, 0, 2, 3)) {
			MessageBox(hwnd, "Failed to make player", "Notice",
			MB_OK | MB_ICONINFORMATION);
		}
		if (defineIndividual(skeleton, 2005, RGB(255, 0, 255), "skelly", 10, 1, 10, 2, 3, 1, 1, 3)) {
			MessageBox(hwnd, "Failed to make player", "Notice",
			MB_OK | MB_ICONINFORMATION);
		}

		int x;
		int y;
		main_field = initField(
				"C:\\Users\\Adrian\\workspace\\Natural_1\\src\\map1.txt");
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

		printf("1,0:%p, 2,0:%p\n",
				getSpaceFromField(main_field, 1, 0)->currentIndividual,
				getSpaceFromField(main_field, 2, 0)->currentIndividual);
		printf("+++ skeleton x:%d, y:%d\n", skeleton->playerCharacter->x,
				skeleton->playerCharacter->y);
		moveIndividual(main_field, skeleton, 8);
		printf("1,0:%p, 2,0:%p\n",
				getSpaceFromField(main_field, 1, 0)->currentIndividual,
				getSpaceFromField(main_field, 2, 0)->currentIndividual);
		printf("--- skeleton x:%d, y:%d\n", skeleton->playerCharacter->x,
				skeleton->playerCharacter->y);
//		getSpaceFromField(main_field, 1, 0)->currentIndividual = skeleton;



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
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
//			DeleteObject(g_hbmPlayer);
//		DeleteObject(g_hbmPlayerMask);
		destroyIndividual(player);

		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_KEYDOWN: {
		switch (LOWORD(wParam)) {
		case 0x34: //left
		case 0x64:
			moveIndividual(main_field, player, 4);
			player->remainingActions = player->remainingActions - 1;
			if (player->remainingActions <= 0) {
				endTurn(player);
				enemyAction(skeleton, main_field, player);
			}
			break;
		case 0x36: //right
		case 0x66:
			moveIndividual(main_field, player, 6);
			player->remainingActions = player->remainingActions - 1;
			if (player->remainingActions <= 0) {
				endTurn(player);
				enemyAction(skeleton, main_field, player);
			}

			break;
		case 0x38: //up
		case 0x68:
			moveIndividual(main_field, player, 8);
			player->remainingActions = player->remainingActions - 1;
			if (player->remainingActions <= 0) {
				endTurn(player);
				enemyAction(skeleton, main_field, player);
			}

			break;
		case 0x32: //down
		case 0x62:
			moveIndividual(main_field, player, 2);
			player->remainingActions = player->remainingActions - 1;
			if (player->remainingActions <= 0) {
				endTurn(player);
				enemyAction(skeleton, main_field, player);
			}
			break;
		case 0x31: //down left
		case 0x61:
			moveIndividual(main_field, player, 1);
			player->remainingActions = player->remainingActions - 1;
			if (player->remainingActions <= 0) {
				endTurn(player);
				enemyAction(skeleton, main_field, player);
			}

			break;
		case 0x37: //up left
		case 0x67:
			moveIndividual(main_field, player, 7);
			player->remainingActions = player->remainingActions - 1;
			if (player->remainingActions <= 0) {
				endTurn(player);
				enemyAction(skeleton, main_field, player);
			}

			break;
		case 0x39: //up right
		case 0x69:
			moveIndividual(main_field, player, 9);
			player->remainingActions = player->remainingActions - 1;
			if (player->remainingActions <= 0) {
				endTurn(player);
				enemyAction(skeleton, main_field, player);
			}

			break;
		case 0x33: //down right
		case 0x63:
			moveIndividual(main_field, player, 3);
			player->remainingActions = player->remainingActions - 1;
			if (player->remainingActions <= 0) {
				endTurn(player);
				enemyAction(skeleton, main_field, player);
			}

			break;
		case 0x41:
			*cursorMode = trueInt;
			initCursorMode = 1;
			break;
		case 0x42:
			getSpaceClosestToPlayer(main_field,skeleton,player);
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (*cursorMode) {
		if (initCursorMode) {
			printf("playerX:%d\n", player->playerCharacter->x);
			thisCursor->cursorCharacter->x = player->playerCharacter->x;
			thisCursor->cursorCharacter->y = player->playerCharacter->y;
			initCursorMode = 0;
		}

		return cursorLoop(hwnd, msg, wParam, lParam, cursorMode, thisCursor, main_field, player, skeleton);
	} else {
		return mainLoop(hwnd, msg, wParam, lParam);
	}

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow) {

	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;
	srand(time(NULL));
	cursorMode = &falseInt;

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
	CW_USEDEFAULT, 460, 240,
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
