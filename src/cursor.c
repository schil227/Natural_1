/*
 * cursor.c
 *
 *  Created on: Apr 26, 2015
 *      Author: Adrian
 */
#include "./headers/cursor_pub_methods.h"

void drawCursor(HDC hdc, HDC hdcBuffer, cursor* thisCursor, int xShift, int yShift){
	HDC hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, thisCursor->cursorCharacter->imageMask);
	BitBlt(hdcBuffer, thisCursor->cursorCharacter->x*40 - xShift*40, thisCursor->cursorCharacter->y*40 - yShift*40, thisCursor->cursorCharacter->width, thisCursor->cursorCharacter->height, hdcMem, 0, 0, SRCAND);
	SelectObject(hdcMem, thisCursor->cursorCharacter->image);
	BitBlt(hdcBuffer, thisCursor->cursorCharacter->x*40 - xShift*40, thisCursor->cursorCharacter->y*40 - yShift*40, thisCursor->cursorCharacter->width, thisCursor->cursorCharacter->height, hdcMem, 0, 0, SRCPAINT);
	DeleteDC(hdcMem);
}

void destroyCursor(cursor* thisCursor){
	if(thisCursor->cursorCharacter){
		free(thisCursor->cursorCharacter);
	}
	free(thisCursor);
}

cursor * initCursor(int imageID, COLORREF rgb, int x, int y) {
	BITMAP bm;
	cursor * thisCursor = malloc(sizeof(cursor));
	thisCursor->cursorCharacter = malloc(sizeof(character));

	thisCursor->cursorCharacter->imageID = imageID;
	thisCursor->cursorCharacter->image = LoadBitmap(GetModuleHandle(NULL),
	MAKEINTRESOURCE(imageID));

	if (thisCursor->cursorCharacter->image == NULL) {
		return 1;
	}

	thisCursor->cursorCharacter->imageMask = CreateBitmapMask(
			thisCursor->cursorCharacter->image, rgb);

	GetObjectA(thisCursor->cursorCharacter->image, sizeof(bm), &bm);

	thisCursor->cursorCharacter->width = bm.bmWidth;
	thisCursor->cursorCharacter->height = bm.bmHeight;
	thisCursor->cursorCharacter->x = x;
	thisCursor->cursorCharacter->y = y;

	return thisCursor;
}

int cursorLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, int * cursorMode, int * postCursorMode, cursor * thisCursor, field * main_field, individual * player, enemies  * thisEnemies, int* xShift, int* yShift) {
	int toReturn = 0;
	switch (msg) {
	case WM_KEYDOWN: {
		switch (LOWORD(wParam)) {
		case 0x34: //left
		case 0x64:
		case 0x36:
		case 0x66:
		case 0x38:
		case 0x68:
		case 0x32:
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
			int direction;
			direction = LOWORD(wParam) % 16;
			moveCursor(main_field,thisCursor,direction, xShift, yShift);
			break;
		}
		case 0x1B: //escape
			*cursorMode = 0;
			break;
		case 0x0D: //enter
		{
			int cX, cY, index;
			cX = thisCursor->cursorCharacter->x;
			cY = thisCursor->cursorCharacter->y;

			individual ** tmp = getIndividualAddressFromField(main_field, cX, cY);

			for (index = 0; index < thisEnemies->numEnemies; index++) {

				individual * tmpEnemy = thisEnemies->enemies[index];

				if (*tmp == tmpEnemy && individualWithinRange(player, tmpEnemy)) {
					printf("attacked!");
					if(attackIndividual(player, tmpEnemy)){
						deleteEnemyFromEnemies(thisEnemies,tmpEnemy);
						removeIndividualFromField(main_field, tmpEnemy->playerCharacter->x, tmpEnemy->playerCharacter->y);
						destroyIndividual(tmpEnemy);
					}
					*cursorMode = 0;
					*postCursorMode = 1;

					break;
				}

			}

//					destroyIndividual(tmp);
		}
			break;
		}
		case WM_TIMER:
		{
			RECT rect;
			HDC hdc = GetDC(hwnd);
			GetClientRect(hwnd, &rect);
			drawAll(hdc, &rect);

			ReleaseDC(hwnd, hdc);
		}
		break;
		case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
		case WM_DESTROY:
//		DeleteObject(g_hbmPlayerMask);
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
