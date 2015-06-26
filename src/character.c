/*
 * character.c
 *
 *  Created on: Mar 25, 2015
 *      Author: Adrian
 */

#include<string.h>
#include"./headers/character_pub_methods.h"

void nameCharacter(character* thisCharacter, char* name){
	thisCharacter->name = malloc(sizeof(name));
	strcpy(thisCharacter->name, name);
	thisCharacter->nameLength = strlen(name);
}

void destroyCharacter(character* thisCharacter){
	if(thisCharacter->nameLength != 0){
		free(thisCharacter->name);
	}

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
