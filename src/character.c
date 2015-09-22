/*
 * character.c
 *
 *  Created on: Mar 25, 2015
 *      Author: Adrian
 */

#include<string.h>
#include"./headers/character_pub_methods.h"


void destroyCharacter(character* thisCharacter){
	free(thisCharacter);
}

void moveCharacter(character* thisCharacter, int newX, int newY){
	thisCharacter->x = newX;
	thisCharacter->y = newY;
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

character * createCharacter(int imageID, COLORREF rgb, int x, int y){
	character * thisCharacter = malloc(sizeof(character));
	BITMAP bm;

	thisCharacter->image = malloc(sizeof(HBITMAP));
	thisCharacter->imageMask = malloc(sizeof(HBITMAP));

	thisCharacter->imageID = imageID;
	thisCharacter->x = x;
	thisCharacter->y = y;
	thisCharacter->rgb = rgb;

	thisCharacter->image = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(imageID));

	thisCharacter->imageMask = CreateBitmapMask(thisCharacter->image, rgb);

	GetObjectA(thisCharacter->image, sizeof(bm), &bm);

	thisCharacter->height = bm.bmHeight;
	thisCharacter->width = bm.bmWidth;

	return thisCharacter;
}

void drawCharacter(HDC hdc, HDC hdcBuffer, character * thisCharacter, shiftData * viewShift){
	HDC hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, thisCharacter->imageMask);

	BitBlt(hdcBuffer, thisCharacter->x*40 - (viewShift->xShift)*40, thisCharacter->y*40 - (viewShift->yShift)*40, thisCharacter->width, thisCharacter->height, hdcMem, 0, 0, SRCAND);

	SelectObject(hdcMem, thisCharacter->image);

	BitBlt(hdcBuffer, thisCharacter->x*40 - (viewShift->xShift)*40, thisCharacter->y*40 - (viewShift->yShift)*40, thisCharacter->width, thisCharacter->height, hdcMem, 0, 0, SRCPAINT);
	DeleteDC(hdcMem);
}

void drawUnboundCharacter(HDC hdc, HDC hdcBuffer, int x, int y, character * thisCharacter, shiftData * viewShift){
	HDC hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, thisCharacter->imageMask);

	BitBlt(hdcBuffer, x*40 - (viewShift->xShift)*40, y*40 - (viewShift->yShift)*40, thisCharacter->width , thisCharacter->height, hdcMem, 0, 0, SRCAND);

	SelectObject(hdcMem, thisCharacter->image);

	BitBlt(hdcBuffer, x*40 - (viewShift->xShift)*40, y*40 - (viewShift->yShift)*40, thisCharacter->width, thisCharacter->height, hdcMem, 0, 0, SRCPAINT);
	DeleteDC(hdcMem);
}

shiftData * initShiftData(){
	shiftData * newShiftData = malloc(sizeof(shiftData));
	newShiftData->xShift = 0;
	newShiftData->yShift = 0;
	newShiftData->xShiftOld = 0;
	newShiftData->yShiftOld = 0;

	return newShiftData;
}
