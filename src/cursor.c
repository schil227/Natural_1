/*
 * cursor.c
 *
 *  Created on: Apr 26, 2015
 *      Author: Adrian
 */
#include "./headers/cursor_pub_methods.h"

void drawCursor(HDC hdc, HDC hdcBuffer, cursor* thisCursor, shiftData * viewData){
	HDC hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, thisCursor->cursorCharacter->imageMask);
	BitBlt(hdcBuffer, thisCursor->cursorCharacter->x*40 - (viewData->xShift)*40, thisCursor->cursorCharacter->y*40 - (viewData->yShift)*40, thisCursor->cursorCharacter->width, thisCursor->cursorCharacter->height, hdcMem, 0, 0, SRCAND);
	SelectObject(hdcMem, thisCursor->cursorCharacter->image);
	BitBlt(hdcBuffer, thisCursor->cursorCharacter->x*40 - (viewData->xShift)*40, thisCursor->cursorCharacter->y*40 - (viewData->yShift)*40, thisCursor->cursorCharacter->width, thisCursor->cursorCharacter->height, hdcMem, 0, 0, SRCPAINT);
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


