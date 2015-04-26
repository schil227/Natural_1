/*
 * cursor.c
 *
 *  Created on: Apr 26, 2015
 *      Author: Adrian
 */
#include "./headers/cursor_pub_methods.h"

void drawCursor(HDC hdc, HDC hdcBuffer, cursor* thisCursor){
	HDC hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, thisCursor->cursorCharacter->imageMask);
	BitBlt(hdcBuffer, thisCursor->cursorCharacter->x, thisCursor->cursorCharacter->y, thisCursor->cursorCharacter->width, thisCursor->cursorCharacter->height, hdcMem, 0, 0, SRCAND);
	SelectObject(hdcMem, thisCursor->cursorCharacter->image);
	BitBlt(hdcBuffer, thisCursor->cursorCharacter->x, thisCursor->cursorCharacter->y, thisCursor->cursorCharacter->width, thisCursor->cursorCharacter->height, hdcMem, 0, 0, SRCPAINT);
	DeleteDC(hdcMem);
}
