/*
 * character.c
 *
 *  Created on: Mar 25, 2015
 *      Author: Adrian
 */

#include<string.h>
#include"./headers/character_pub_methods.h"

/*
 * public domain strtok_r() by Charlie Gordon
 *
 *   from comp.lang.c  9/14/2007
 *
 *      http://groups.google.com/group/comp.lang.c/msg/2ab1ecbb86646684
 *
 *     (Declaration that it's public domain):
 *      http://groups.google.com/group/comp.lang.c/msg/7c7b39328fefab9c
 */

char* strtok_r(
    char *str,
    const char *delim,
    char **nextp)
{
    char *ret;

    if (str == NULL)
    {
        str = *nextp;
    }

    str += strspn(str, delim);

    if (*str == '\0')
    {
        return NULL;
    }

    ret = str;

    str += strcspn(str, delim);

    if (*str)
    {
        *str++ = '\0';
    }

    *nextp = str;

    return ret;
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

character * createCharacterFromLine(char line[128]){
	int imageId, r, g, b, x, y;

	char * value = strtok(line,";");
	imageId = atoi(value);

	value = strtok(NULL,";");
	r = atoi(value);

	value = strtok(NULL,";");
	g = atoi(value);

	value = strtok(NULL,";");
	b = atoi(value);

	value = strtok(NULL,";");
	x = atoi(value);

	value = strtok(NULL,";");
	y = atoi(value);

	return createCharacter(imageId, RGB(r,g,b), x, y);
}

void destroyCharacter(character* thisCharacter){
	if(thisCharacter != NULL){
		if(thisCharacter->image != NULL){
			DeleteObject(thisCharacter->image);
		}

		if(thisCharacter->imageMask != NULL){
			DeleteObject(thisCharacter->imageMask);
		}
	}
	free(thisCharacter);
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

void drawUnboundCharacterAbsolute(HDC hdc, HDC hdcBuffer, int x, int y, character * thisCharacter){
	HDC hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, thisCharacter->imageMask);

	BitBlt(hdcBuffer, x, y, thisCharacter->width , thisCharacter->height, hdcMem, 0, 0, SRCAND);

	SelectObject(hdcMem, thisCharacter->image);

	BitBlt(hdcBuffer, x, y, thisCharacter->width, thisCharacter->height, hdcMem, 0, 0, SRCPAINT);
	DeleteDC(hdcMem);
}

void drawUnboundCharacterByPixels(HDC hdc, HDC hdcBuffer, int x, int y, character * thisCharacter, shiftData * viewShift){
	HDC hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, thisCharacter->imageMask);

	BitBlt(hdcBuffer, x, y, thisCharacter->width , thisCharacter->height, hdcMem, 0, 0, SRCAND);

	SelectObject(hdcMem, thisCharacter->image);

	BitBlt(hdcBuffer, x, y, thisCharacter->width, thisCharacter->height, hdcMem, 0, 0, SRCPAINT);
	DeleteDC(hdcMem);
}

void updateAnimation(character * thisCharacter){
	thisCharacter->thisAnimationContainer->clockTickCount++;

	animation * thisAnimationSet = thisCharacter->thisAnimationContainer->animations[thisCharacter->thisAnimationContainer->currentAnimation];

	if(thisAnimationSet->durationInTicks[thisAnimationSet->currentFrame] < thisCharacter->thisAnimationContainer->clockTickCount){
	  if(thisAnimationSet->currentFrame + 1 == thisAnimationSet->numFrames){
		thisAnimationSet->currentFrame = 0;
		thisCharacter->thisAnimationContainer->currentAnimation = thisCharacter->thisAnimationContainer->defaultAnimation; // Go back to default animation
	  }else{
		thisAnimationSet->currentFrame++;
	  }
	  thisCharacter->thisAnimationContainer->clockTickCount = 0;
	}
}

int getYImageShift(animationContainer * thisAnimationContainer){
	return (thisAnimationContainer->animations[thisAnimationContainer->currentAnimation]->currentFrame);
}

shiftData * initShiftData(){
	shiftData * newShiftData = malloc(sizeof(shiftData));
	newShiftData->xShift = 0;
	newShiftData->yShift = 0;
	newShiftData->xShiftOld = 0;
	newShiftData->yShiftOld = 0;

	return newShiftData;
}

animationContainer * initAnimationContainer(){
	int i;
	animationContainer * newContainer = malloc(sizeof(animationContainer));

	newContainer->MAX_ANIMATIONS = 15;
	newContainer->clockTickCount = 0;
	newContainer->clockTickDelay = 0;
	newContainer->currentAnimation = 0;
	newContainer->defaultAnimation = 0;
	newContainer->numAnimations = 0;
	newContainer->animationsEnabled = 0;

	for(i = 0; i < newContainer->MAX_ANIMATIONS; i++){
		newContainer->animations[i] = NULL;
	}

	return newContainer;
}

animation * initAnimation(animationState state){
	int i;
	animation * thisAnimation = malloc(sizeof(animation));

	thisAnimation->state = state;
	thisAnimation->MAX_FRAMES = 20;
	thisAnimation->numFrames = 0;
	thisAnimation->totalDuration = 0;
	thisAnimation->animationX = 0;
	thisAnimation->currentFrame = 0;
	thisAnimation->soundFrame = -1;
	thisAnimation->soundID = -1;

	for(i = 0; i < thisAnimation->MAX_FRAMES; i++){
		thisAnimation->durationInTicks[i] = 0;
	}

	return thisAnimation;
}

void loadAnimationFromLine(animationContainer * thisContainer, animationState state, char * line){
	animation * thisAnimation = initAnimation(state);
	int intVal, i;

	char * value = NULL;

	value = strtok(line,",");
	intVal = atoi(value);

	if(intVal == -1){
		return;
	}

	thisAnimation->numFrames = intVal;

	for(i = 0; i < thisAnimation->numFrames; i++){
		value = strtok(NULL,",");
		intVal = atoi(value);

		thisAnimation->durationInTicks[i] = intVal;
		thisAnimation->totalDuration += intVal;
	}

	value = strtok(NULL,",");
	intVal = atoi(value);

	if(intVal != -1){
		thisAnimation->soundFrame = intVal;

		value = strtok(NULL,",");
		thisAnimation->soundID = atoi(value);
	}

	thisAnimation->animationX = thisContainer->numAnimations;
	thisContainer->animations[thisContainer->numAnimations] = thisAnimation;

	thisContainer->numAnimations++;
}
