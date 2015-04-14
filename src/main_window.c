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
#include "./headers/character_pub_methods.h"
#include "./headers/field_pub_methods.h"

const char g_szClassName[] = "MyWindowClass";
const int rateOfChange_player_x = 8;
const int rateOfChange_player_y = 8;
int numMessages = 0;
int mouseButtonCount = 0;
//HBITMAP g_hbmPlayer = NULL;
HBITMAP g_hbmPlayerMask = NULL;
HWND g_toolbar = NULL;

character* player;
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

//Make 'em bounce around like a good screensaver.
void updatePlayer(RECT* prc) {

//	if(g_playerInfo.x < 0 || g_playerInfo.x + g_playerInfo.width >= prc->right){
//		g_playerInfo.dx = g_playerInfo.dx*-1;
//	}
//
//	if(g_playerInfo.y < 0 || g_playerInfo.y + g_playerInfo.height >= prc->bottom){
//		g_playerInfo.dy = g_playerInfo.dy*-1;
//	}
//
//	g_playerInfo.x += g_playerInfo.dx;
//	g_playerInfo.y += g_playerInfo.dy;

}

void drawField(HDC hdc, RECT* prc) {
	HDC hdcBuffer = CreateCompatibleDC(hdc);
	HDC hdcBuffer2 = CreateCompatibleDC(hdc);
	HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, prc->right, prc->bottom);
	HBITMAP hbmOldBuffer = SelectObject(hdcBuffer, hbmBuffer); //copy of hbmBuffer

	HDC hdcMem = CreateCompatibleDC(hdc);
	HDC hdcMem2 = CreateCompatibleDC(hdc);
	HBITMAP hbmOld = SelectObject(hdcMem, g_hbmPlayerMask);
	int x;
	int y;

	//field
	for (y = 0; y < main_field->totalY; y++) {
		for (x = 0; x < main_field->totalX; x++) {

//			BitBlt(hdcBuffer, main_field->grid[x][y]->background->x,
//					main_field->grid[x][y]->background->y,
//					main_field->grid[x][y]->background->width,
//					main_field->grid[x][y]->background->height,
//						hdcMem, 0, 0, SRCAND);

			SelectObject(hdcMem, main_field->grid[x][y]->background->image);

			BitBlt(hdcBuffer, main_field->grid[x][y]->background->x,
					main_field->grid[x][y]->background->y,
					main_field->grid[x][y]->background->width,
					main_field->grid[x][y]->background->height, hdcMem, 0, 0,
					SRCCOPY);

		}
	}

	//player

	BitBlt(hdc, 0, 0, prc->right, prc->bottom, hdcBuffer, 0, 0, SRCCOPY);


	SelectObject(hdcMem2, g_hbmPlayerMask);

	BitBlt(hdcBuffer, player->x, player->y, player->width, player->height, hdcMem2, 0, 0, SRCAND);
//
	SelectObject(hdcMem2, player->image);// player->image);

	BitBlt(hdcBuffer, player->x, player->y, player->width, player->height, hdcMem2, 0, 0, SRCPAINT); //was SRCPAINT
//
	BitBlt(hdc, 0, 0, prc->right, prc->bottom, hdcBuffer, 0, 0, SRCCOPY);

	SelectObject(hdcMem2, hbmOld);
	DeleteDC(hdcMem);
	DeleteDC(hdcMem2);
	SelectObject(hdcBuffer, hbmOldBuffer);
	DeleteDC(hdcBuffer);
	DeleteObject(hbmBuffer);
}

void drawPlayer(HDC hdc, RECT* prc) {
	HDC hdcBuffer = CreateCompatibleDC(hdc);
	HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, prc->right, prc->bottom);
	HBITMAP hbmOldBuffer = SelectObject(hdcBuffer, hbmBuffer); //copy of hbmBuffer

	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmOld = SelectObject(hdcMem, g_hbmPlayerMask);

//	FillRect(hdc, prc, (HBRUSH) GetStockObject(LTGRAY_BRUSH));

	BitBlt(hdcBuffer, player->x, player->y, player->width, player->height,
			hdcMem, 0, 0, SRCAND);

	SelectObject(hdcMem, player->image);
	BitBlt(hdcBuffer, player->x, player->y, player->width, player->height,
			hdcMem, 0, 0, SRCPAINT);

	BitBlt(hdc, 0, 0, prc->right, prc->bottom, hdcBuffer, 0, 0, SRCCOPY);

	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);

	SelectObject(hdcBuffer, hbmOldBuffer);
	DeleteDC(hdcBuffer);
	DeleteObject(hbmBuffer);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CREATE: {
		BITMAP bm;
		UINT ret;
		player = malloc(sizeof(character));
//		main_field = malloc(sizeof(field));
		int x;
		int y;
//		printf("a\n");
		main_field = initField("C:\\Users\\Adrian\\workspace\\Natural_1\\src\\map1.txt");
////		printf("asdf\n");
		int imageID;
		for (y = 0; y < main_field->totalY; y++) {
			printf("b\n");
			for (x = 0; x < main_field->totalX; x++) {
				imageID = (main_field->grid[x][y]->background)->imageID;
				printf("id:%d\n", imageID);
				main_field->grid[x][y]->background->image = malloc(sizeof(HBITMAP));
				main_field->grid[x][y]->background->image = LoadBitmap(GetModuleHandle(NULL), imageID);
				if(main_field->grid[x][y]->background->image == NULL){
					printf("failed\n");
				}
			}
		}

		//player->image = NULL;
		player->imageID = 2001;
		player->image = malloc(sizeof(HBITMAP));
		player->image = LoadBitmap(GetModuleHandle(NULL),
				MAKEINTRESOURCE(player->imageID));

		if (player->image == NULL) {
			MessageBox(hwnd, "Failed to make player", "Notice",
			MB_OK | MB_ICONINFORMATION);
		}

		g_hbmPlayerMask = CreateBitmapMask(player->image, RGB(255, 255, 255)); //transparent is black
		if (g_hbmPlayerMask == NULL) {
			MessageBox(hwnd, "Failed to make player mask", "Notice",
			MB_OK | MB_ICONINFORMATION);
		}

		GetObjectA(player->image, sizeof(bm), &bm);
//				ZeroMemory(player, sizeof(player));

		player->height = bm.bmHeight;
		player->width = bm.bmWidth;

//				g_playerInfo.dx = rateOfChange_player_x;
//				g_playerInfo.dy = rateOfChange_player_y;

		player->x = 50;
		player->y = 50;

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
		drawField(hdc,&rect);
//		drawPlayer(hdc, &rect);

		EndPaint(hwnd, &ps);
	}
		break;
	case WM_TIMER: {
		RECT rect;
		HDC hdc = GetDC(hwnd);
		GetClientRect(hwnd, &rect);
		drawField(hdc,&rect);
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
		destroyCharacter(player);

		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_KEYDOWN: {
		switch (LOWORD(wParam)) {
		case 0x34: //left
		case 0x64:
			player->x -= 50;
			break;
		case 0x36:
		case 0x66:
			player->x += 50;
			break;
		case 0x38:
		case 0x68:
			player->y -= 50;
			break;
		case 0x32:
		case 0x62:
			player->y += 50;
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
