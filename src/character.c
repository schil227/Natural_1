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

fixedCharacter * createCharacter(int imageID, COLORREF rgb, int x, int y){
	fixedCharacter * thisCharacter = malloc(sizeof(fixedCharacter));
	BITMAP bm;

	thisCharacter->fixedImageID = imageID;
	thisCharacter->x = x;
	thisCharacter->y = y;
	thisCharacter->rgb = rgb;

	thisCharacter->fixedImage = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(imageID), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);//LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(imageID));

	thisCharacter->fixedImageMask = CreateBitmapMask(thisCharacter->fixedImage, rgb);

	GetObjectA(thisCharacter->fixedImage, sizeof(bm), &bm);

	thisCharacter->fixedHeight = bm.bmHeight;
	thisCharacter->fixedWidth = bm.bmWidth;

	return thisCharacter;
}

character * createCharacterFromAnimation(animation * thisAnimation){
	character * newCharacter = malloc(sizeof(fixedCharacter));

	newCharacter->direction = 0;

	newCharacter->thisAnimationContainer = initAnimationContainer();
	newCharacter->thisAnimationContainer->animations[0] = thisAnimation;
	newCharacter->thisAnimationContainer->numAnimations = 1;
	newCharacter->secondaryAnimationContainer = cloneAnimationContainer(newCharacter->thisAnimationContainer);

	return newCharacter;
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

animationState pickAnimationState(char * state){
	if(strcmp(state, "ANIMATION_IDLE") == 0){
		return ANIMATION_IDLE;
	}else if(strcmp(state, "ANIMATION_IDLE_EQUIPT") == 0){
		return ANIMATION_IDLE_EQUIPT;
	}else if(strcmp(state, "ANIMATION_ATTACK_SLASH") == 0){
		return ANIMATION_ATTACK_SLASH;
	}else if(strcmp(state, "ANIMATION_ATTACK_CHOP") == 0){
		return ANIMATION_ATTACK_CHOP;
	}else if(strcmp(state, "ANIMATION_ATTACK_BLUNT") == 0){
		return ANIMATION_ATTACK_BLUNT;
	}else if(strcmp(state, "ANIMATION_ATTACK_PIERCE") == 0){
		return ANIMATION_ATTACK_PIERCE;
	}else if(strcmp(state, "ANIMATION_ATTACK_BOW") == 0){
		return ANIMATION_ATTACK_BOW;
	}else if(strcmp(state, "ANIMATION_HARM") == 0){
		return ANIMATION_HARM;
	}else if(strcmp(state, "ANIMATION_DEATH") == 0){
		return ANIMATION_DEATH;
	}else if(strcmp(state, "ANIMATION_CAST") == 0){
		return ANIMATION_CAST;
	}else if(strcmp(state, "ANIMATION_CONSUME") == 0){
		return ANIMATION_CONSUME;
	}

	char outlog[128];
	sprintf(outlog, "!! STATE %s NOT FOUND : USING DEFAULT !!", state);
	cwrite(outlog);

	return ANIMATION_IDLE;
}

animation * createAnimationFromLine(char line[512]){
	char * strtok_save_pointer;
	char * durations;
	char * value = strtok_r(line, ";", &strtok_save_pointer);
	int i,r,g,b,totalDuration = 0;

	animationState state = pickAnimationState(value);

	animation * newAnimation = initAnimation(state);

	value = strtok_r(NULL, ";", &strtok_save_pointer);
	newAnimation->imageID = atoi(value);

	newAnimation->image = LoadImage(GetModuleHandle(NULL), newAnimation->imageID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);//LoadBitmap(GetModuleHandle(NULL) , newAnimation->imageID);

	value = strtok_r(NULL, ";", &strtok_save_pointer);
	r = atoi(value);

	value = strtok_r(NULL, ";", &strtok_save_pointer);
	g = atoi(value);

	value = strtok_r(NULL, ";", &strtok_save_pointer);
	b = atoi(value);

	newAnimation->rgb = RGB(r,g,b);

	newAnimation->imageMask = CreateBitmapMask(newAnimation->image, newAnimation->rgb);

	value = strtok_r(NULL, ";", &strtok_save_pointer);
	if(atoi(value) == -1){
		newAnimation->width = 100;
	}else{
		newAnimation->width = atoi(value);
	}

	value = strtok_r(NULL, ";", &strtok_save_pointer);
	if(atoi(value) == -1){
		newAnimation->height = 100;
	}else{
		newAnimation->height = atoi(value);
	}

	value = strtok_r(NULL, ";", &strtok_save_pointer);

	newAnimation->numFrames = atoi(value);

	durations = strtok_r(NULL, ";", &strtok_save_pointer);

	value = strtok(durations, ",");
	for(i = 0; i < newAnimation->numFrames; i++){
		newAnimation->durationInTicks[i] = atoi(value);
		totalDuration += atoi(value);

		if(i+1 < newAnimation->numFrames){
			value = strtok(NULL, ",");
		}
	}

	newAnimation->totalDuration = totalDuration;

	value = strtok_r(NULL, ";", &strtok_save_pointer);
	newAnimation->soundFrame = atoi(value);

	value = strtok_r(NULL, ";", &strtok_save_pointer);
	newAnimation->soundID = atoi(value);

	return newAnimation;
}

void destroyCharacter(character* thisCharacter){
//	if(thisCharacter != NULL){
//		if(thisCharacter->image != NULL){
//			DeleteObject(thisCharacter->image);
//		}
//
//		if(thisCharacter->imageMask != NULL){
//			DeleteObject(thisCharacter->imageMask);
//		}
//	}

//	destroy animations/containers
	free(thisCharacter);
}

void destroyFixedCharacter(fixedCharacter * thisCharacter){
//	if(thisCharacter != NULL){
//		if(thisCharacter->fixedImage != NULL){
//			DeleteObject(thisCharacter->fixedImage);
//		}
//
//		if(thisCharacter->fixedImageMask != NULL){
//			DeleteObject(thisCharacter->fixedImageMask);
//		}
//	}

	free(thisCharacter);
}

//void drawUnboundCharacter(HDC hdc, HDC hdcBuffer, int x, int y, character * thisCharacter, shiftData * viewShift){
//	HDC hdcMem = CreateCompatibleDC(hdc);
//	SelectObject(hdcMem, thisCharacter->imageMask);
//
//	BitBlt(hdcBuffer, x*40 - (viewShift->xShift)*40, y*40 - (viewShift->yShift)*40, thisCharacter->width , thisCharacter->height, hdcMem, 0, 0, SRCAND);
//
//	SelectObject(hdcMem, thisCharacter->image);
//
//	BitBlt(hdcBuffer, x*40 - (viewShift->xShift)*40, y*40 - (viewShift->yShift)*40, thisCharacter->width, thisCharacter->height, hdcMem, 0, 0, SRCPAINT);
//	DeleteDC(hdcMem);
//}

void drawUnboundCharacterAbsolute(HDC hdc, HDC hdcBuffer, int x, int y, fixedCharacter * thisCharacter){
	HDC hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, thisCharacter->fixedImageMask);

	BitBlt(hdcBuffer, x, y, thisCharacter->fixedWidth , thisCharacter->fixedHeight, hdcMem, 0, 0, SRCAND);

	SelectObject(hdcMem, thisCharacter->fixedImage);

	BitBlt(hdcBuffer, x, y, thisCharacter->fixedWidth, thisCharacter->fixedHeight, hdcMem, 0, 0, SRCPAINT);
	DeleteDC(hdcMem);
}

void drawUnboundCharacterByPixels(HDC hdc, HDC hdcBuffer, int x, int y, fixedCharacter * thisCharacter, shiftData * viewShift){
	HDC hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, thisCharacter->fixedImageMask);

	BitBlt(hdcBuffer, x, y, thisCharacter->fixedWidth , thisCharacter->fixedHeight, hdcMem, 0, 0, SRCAND);

	SelectObject(hdcMem, thisCharacter->fixedImage);

	BitBlt(hdcBuffer, x, y, thisCharacter->fixedWidth, thisCharacter->fixedHeight, hdcMem, 0, 0, SRCPAINT);
	DeleteDC(hdcMem);
}

void drawCharacterAnimation(HDC hdc, HDC hdcBuffer, character * thisCharacter, shiftData * viewShift, int useSecondaryAnimationContainer){
	HDC hdcMem = CreateCompatibleDC(hdc);

	int shitfX;

	HBITMAP selectedImage, selectedImageMask;

	if(useSecondaryAnimationContainer){
		shitfX = thisCharacter->secondaryAnimationContainer->animations[thisCharacter->secondaryAnimationContainer->currentAnimation]->currentFrame*100;
		selectedImage = thisCharacter->secondaryAnimationContainer->animations[thisCharacter->secondaryAnimationContainer->currentAnimation]->image;
		selectedImageMask = thisCharacter->secondaryAnimationContainer->animations[thisCharacter->secondaryAnimationContainer->currentAnimation]->imageMask;
	} else {
		shitfX = thisCharacter->thisAnimationContainer->animations[thisCharacter->thisAnimationContainer->currentAnimation]->currentFrame*100;
		selectedImage = thisCharacter->thisAnimationContainer->animations[thisCharacter->thisAnimationContainer->currentAnimation]->image;
		selectedImageMask = thisCharacter->thisAnimationContainer->animations[thisCharacter->thisAnimationContainer->currentAnimation]->imageMask;
	}
	SelectObject(hdcMem, selectedImageMask);
//
//	BitBlt(hdcBuffer, thisCharacter->x*50 - (viewShift->xShift)*50 - 25, thisCharacter->y*50 - (viewShift->yShift)*50 - 25,
////				thisIndividual->playerCharacter->width, thisIndividual->playerCharacter->height,
//			100,100,
//			hdcMem,
//			shitfX,
//			0,
//			SRCAND);

	BitBlt(hdcBuffer, thisCharacter->x*52 + (int)(thisCharacter->xOff*52) - (viewShift->xShift)*52 - 25, thisCharacter->y*52 + (int)(thisCharacter->yOff*52) - (viewShift->yShift)*52 - 25,
//				thisIndividual->playerCharacter->width, thisIndividual->playerCharacter->height,
			100,100,
			hdcMem,
			shitfX,
			0,
			SRCAND);

	SelectObject(hdcMem, selectedImage);

//	BitBlt(hdcBuffer, thisCharacter->x*50 - (viewShift->xShift)*50 - 25, thisCharacter->y*50 - (viewShift->yShift)*50 - 25,
////				thisIndividual->playerCharacter->width, thisIndividual->playerCharacter->height,
//			100,100,
//			hdcMem,
//			shitfX,
//			100,
//			SRCPAINT);

	BitBlt(hdcBuffer, thisCharacter->x*52 + (int)(thisCharacter->xOff*52) - (viewShift->xShift)*52 - 25,thisCharacter->y*52 + (int)(thisCharacter->yOff*52) - (viewShift->yShift)*52 - 25,
//				thisIndividual->playerCharacter->width, thisIndividual->playerCharacter->height,
			100,100,
			hdcMem,
			shitfX,
			0,
			SRCPAINT);

	DeleteDC(hdcMem);
}

void drawUnboundAnimation(HDC hdc, HDC hdcBuffer, int xCord, int yCord, character * thisCharacter, shiftData * viewShift, int useSecondaryAnimationContainer){
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP image, imageMask;

	int shitfX;
	if(useSecondaryAnimationContainer){
		shitfX = thisCharacter->secondaryAnimationContainer->animations[thisCharacter->secondaryAnimationContainer->currentAnimation]->currentFrame*100;
		image = thisCharacter->secondaryAnimationContainer->animations[thisCharacter->secondaryAnimationContainer->currentAnimation]->image;
		imageMask = thisCharacter->secondaryAnimationContainer->animations[thisCharacter->secondaryAnimationContainer->currentAnimation]->imageMask;
	} else{
		shitfX = thisCharacter->thisAnimationContainer->animations[thisCharacter->thisAnimationContainer->currentAnimation]->currentFrame*100;
		image = thisCharacter->thisAnimationContainer->animations[thisCharacter->thisAnimationContainer->currentAnimation]->image;
		imageMask = thisCharacter->thisAnimationContainer->animations[thisCharacter->thisAnimationContainer->currentAnimation]->imageMask;
	}

	SelectObject(hdcMem, imageMask);

	BitBlt(hdcBuffer, xCord*52 - (viewShift->xShift)*52 - 25, yCord*52 - (viewShift->yShift)*52 - 25,
//				thisIndividual->playerCharacter->width, thisIndividual->playerCharacter->height,
			100,100,
			hdcMem,
			shitfX,
			0,
			SRCAND);

	SelectObject(hdcMem, image);

	BitBlt(hdcBuffer, xCord*52 - (viewShift->xShift)*52 - 25, yCord*52 - (viewShift->yShift)*52 - 25,
//				thisIndividual->playerCharacter->width, thisIndividual->playerCharacter->height,
			100,100,
			hdcMem,
			shitfX,
			0,
			SRCPAINT);
	DeleteDC(hdcMem);
}

void drawOverlappingAnimation(HDC hdc, HDC hdcBuffer, int xCord, int yCord, character * thisCharacter, shiftData * viewShift, int useSecondaryAnimationContainer){
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP image, imageMask;

	int shitfX;
	if(useSecondaryAnimationContainer){
		shitfX = thisCharacter->secondaryAnimationContainer->animations[thisCharacter->secondaryAnimationContainer->currentAnimation]->currentFrame*100;
		image = thisCharacter->secondaryAnimationContainer->animations[thisCharacter->secondaryAnimationContainer->currentAnimation]->image;
		imageMask = thisCharacter->secondaryAnimationContainer->animations[thisCharacter->secondaryAnimationContainer->currentAnimation]->imageMask;
	} else{
		shitfX = thisCharacter->thisAnimationContainer->animations[thisCharacter->thisAnimationContainer->currentAnimation]->currentFrame*100;
		image = thisCharacter->thisAnimationContainer->animations[thisCharacter->thisAnimationContainer->currentAnimation]->image;
		imageMask = thisCharacter->thisAnimationContainer->animations[thisCharacter->thisAnimationContainer->currentAnimation]->imageMask;
	}

	SelectObject(hdcMem, imageMask);

	BitBlt(hdcBuffer, xCord*52 - (viewShift->xShift)*52 - 25, yCord*52 - (viewShift->yShift)*52 - 25,
//				thisIndividual->playerCharacter->width, thisIndividual->playerCharacter->height,
			100,100,
			hdcMem,
			shitfX,
			0,
			SRCAND);

	SelectObject(hdcMem, image);

	BitBlt(hdcBuffer, xCord*52 - (viewShift->xShift)*52 - 25, yCord*52 - (viewShift->yShift)*52 - 25,
//				thisIndividual->playerCharacter->width, thisIndividual->playerCharacter->height,
			100,100,
			hdcMem,
			shitfX,
			0,
			SRCPAINT);
	DeleteDC(hdcMem);
}


void drawUnboundShadowAnimation(HDC hdc, HDC hdcBuffer, int xCord, int yCord, character * thisCharacter, shiftData * viewShift, int useSecondaryAnimationContainer){
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP imageMask = NULL;

	int shitfX;
	if(useSecondaryAnimationContainer){
		shitfX = thisCharacter->secondaryAnimationContainer->animations[thisCharacter->secondaryAnimationContainer->currentAnimation]->currentFrame*100;
		imageMask = thisCharacter->secondaryAnimationContainer->animations[thisCharacter->secondaryAnimationContainer->currentAnimation]->imageMask;
	} else{
		shitfX = thisCharacter->thisAnimationContainer->animations[thisCharacter->thisAnimationContainer->currentAnimation]->currentFrame*100;
		imageMask = thisCharacter->thisAnimationContainer->animations[thisCharacter->thisAnimationContainer->currentAnimation]->imageMask;
	}

	SelectObject(hdcMem, imageMask);

	BitBlt(hdcBuffer, xCord*52 - (viewShift->xShift)*52 - 25, yCord*52 - (viewShift->yShift)*52 - 25,
			100,100,
			hdcMem,
			shitfX,
			0,
			SRCAND);

	DeleteDC(hdcMem);
}

void drawUnboundAnimationByPixels(HDC hdc, HDC hdcBuffer, character * thisCharacter, shiftData * viewShift, int xCord, int yCord, int useSecondaryAnimationContainer){
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP image, imageMask;

	int shitfX;
	if(useSecondaryAnimationContainer){
		shitfX = thisCharacter->secondaryAnimationContainer->animations[thisCharacter->secondaryAnimationContainer->currentAnimation]->currentFrame*100;
		image = thisCharacter->secondaryAnimationContainer->animations[thisCharacter->secondaryAnimationContainer->currentAnimation]->image;
		imageMask = thisCharacter->secondaryAnimationContainer->animations[thisCharacter->secondaryAnimationContainer->currentAnimation]->imageMask;
	} else{
		shitfX = thisCharacter->thisAnimationContainer->animations[thisCharacter->thisAnimationContainer->currentAnimation]->currentFrame*100;
		image = thisCharacter->thisAnimationContainer->animations[thisCharacter->thisAnimationContainer->currentAnimation]->image;
		imageMask = thisCharacter->thisAnimationContainer->animations[thisCharacter->thisAnimationContainer->currentAnimation]->imageMask;
	}

	SelectObject(hdcMem, imageMask);

	BitBlt(hdcBuffer, xCord, yCord,
			100,100,
			hdcMem,
			shitfX,
			0,
			SRCAND);

	SelectObject(hdcMem, image);

	BitBlt(hdcBuffer,xCord, yCord,
			100, 100,
			hdcMem,
			shitfX,
			0,
			SRCPAINT);
	DeleteDC(hdcMem);
}

/*
 * Note that this method was created because using a mask with rotated characters was very taxing for some reason.
 */
void drawRotatedBackgroundByPixel(HDC hdc, HDC hdcBuffer, character * thisCharacter, shiftData * viewShift, int xCord, int yCord, int useSecondaryAnimationContainer){
	HDC hdcMem = CreateCompatibleDC(hdc);

	int shitfX = thisCharacter->thisAnimationContainer->animations[thisCharacter->thisAnimationContainer->currentAnimation]->currentFrame*100;
	HBITMAP image = thisCharacter->thisAnimationContainer->animations[thisCharacter->thisAnimationContainer->currentAnimation]->image;

	SelectObject(hdcMem, image);

	LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;
	///// START PERF TIMER /////
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);
	////////////////////////////

	BitBlt(hdcBuffer,xCord, yCord,
			100, 100,
			hdcMem,
			shitfX,
			0,
			SRCPAINT);//SRCPAINT
	DeleteDC(hdcMem);


	///// END PERF TIMER /////
	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;

	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

//	printf("draw timing: %llu\n",ElapsedMicroseconds.QuadPart);
	char outLog[256];
	sprintf(outLog, "End drawall field process: %llu",ElapsedMicroseconds.QuadPart);
	cwrite(outLog);
	/////////////////////////
}

void updateAnimation(character * thisCharacter){
	thisCharacter->thisAnimationContainer->clockTickCount++;

	if(thisCharacter->thisAnimationContainer->clockTickDelay > 0){
		thisCharacter->thisAnimationContainer->clockTickDelay--;
		if(thisCharacter->thisAnimationContainer->clockTickDelay == 0){
			thisCharacter->thisAnimationContainer->animations[thisCharacter->thisAnimationContainer->currentAnimation]->currentFrame = 0;
			thisCharacter->thisAnimationContainer->animations[thisCharacter->thisAnimationContainer->nextAnimationAfterDelay]->currentFrame = 0;

			thisCharacter->thisAnimationContainer->currentAnimation = thisCharacter->thisAnimationContainer->nextAnimationAfterDelay;
			thisCharacter->thisAnimationContainer->clockTickCount = 0;
		}
	}

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

void setDelayAnimation(animationContainer * thisAnimationContainer, animationState state, int delay){
	int i;

	for(i = 0; i < thisAnimationContainer->numAnimations; i++){
		if(thisAnimationContainer->animations[i]->state == state){
			thisAnimationContainer->nextAnimationAfterDelay = i;
			thisAnimationContainer->clockTickDelay = delay;

			return;
		}
	}
}

void setAnimation(animationContainer * thisAnimationContainer, animationState state){
	int i;

	for(i = 0; i < thisAnimationContainer->numAnimations; i++){
		if(thisAnimationContainer->animations[i]->state == state){
			thisAnimationContainer->animations[i]->currentFrame = 0;
			thisAnimationContainer->animations[thisAnimationContainer->currentAnimation]->currentFrame = 0;

			thisAnimationContainer->currentAnimation = i;
			thisAnimationContainer->clockTickCount = 0;

			return;
		}
	}
}

void setDefaultAnimation(animationContainer * thisAnimationContainer, animationState state){
	int i;

	for(i = 0; i < thisAnimationContainer->numAnimations; i++){
		if(thisAnimationContainer->animations[i]->state == state){
			thisAnimationContainer->animations[i]->currentFrame = 0;
			thisAnimationContainer->animations[thisAnimationContainer->currentAnimation]->currentFrame = 0;

			thisAnimationContainer->currentAnimation = i;
			thisAnimationContainer->clockTickCount = 0;

			thisAnimationContainer->defaultAnimation = i;

			return;
		}
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
	newContainer->nextAnimationAfterDelay = 0;
	newContainer->currentAnimation = 0;
	newContainer->defaultAnimation = 0;
	newContainer->numAnimations = 0;
	newContainer->animationsEnabled = 0;

	for(i = 0; i < newContainer->MAX_ANIMATIONS; i++){
		newContainer->animations[i] = NULL;
	}

	return newContainer;
}

animationContainer * cloneAnimationContainer(animationContainer * baseAnimationContainer){
	int i;
	animationContainer * newContainer = malloc(sizeof(animationContainer));

	newContainer->MAX_ANIMATIONS = 15;
	newContainer->clockTickCount = baseAnimationContainer->clockTickCount;
	newContainer->clockTickDelay = baseAnimationContainer->clockTickDelay;
	newContainer->nextAnimationAfterDelay = baseAnimationContainer->nextAnimationAfterDelay;
	newContainer->currentAnimation = baseAnimationContainer->currentAnimation;
	newContainer->defaultAnimation = baseAnimationContainer->defaultAnimation;
	newContainer->numAnimations = baseAnimationContainer->numAnimations;
	newContainer->animationsEnabled = baseAnimationContainer->animationsEnabled;

	for(i = 0; i < newContainer->numAnimations; i++){
		newContainer->animations[i] = cloneAnimation(baseAnimationContainer->animations[i]);
	}

	return newContainer;
}

animation * initAnimation(animationState state){
	int i;
	animation * thisAnimation = malloc(sizeof(animation));

	thisAnimation->height = 0;
	thisAnimation->width = 0;

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

animation * cloneAnimation(animation * thisAnimation){
	int i;
	animation * newAnimation = initAnimation(thisAnimation->state);

	newAnimation->imageID = thisAnimation->imageID;
	newAnimation->rgb = thisAnimation->rgb;

	newAnimation->image = thisAnimation->image;//LoadBitmap(GetModuleHandle(NULL) , newAnimation->imageID);
	newAnimation->imageMask =  thisAnimation->imageMask;//CreateBitmapMask(newAnimation->imageID, newAnimation->rgb);

	newAnimation->height = thisAnimation->height;
	newAnimation->width = thisAnimation->width;

	for(i = 0; i < newAnimation->MAX_FRAMES; i++){
		newAnimation->durationInTicks[i] = thisAnimation->durationInTicks[i];
	}

	newAnimation->MAX_FRAMES = thisAnimation->MAX_FRAMES;
	newAnimation->numFrames = thisAnimation->numFrames;
	newAnimation->totalDuration = thisAnimation->totalDuration;
	newAnimation->animationX = thisAnimation->animationX;
	newAnimation->currentFrame = thisAnimation->currentFrame;
	newAnimation->soundFrame = thisAnimation->soundFrame;
	newAnimation->soundID = thisAnimation->soundID;

	return newAnimation;
}

void addAnimationToContainer(animationContainer * thisContainer, animation * thisAnimation){
	if(thisAnimation != NULL && thisContainer->numAnimations < thisContainer->MAX_ANIMATIONS){
		thisContainer->animations[thisContainer->numAnimations] = thisAnimation;
		thisContainer->numAnimations++;
	}
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

void copyPixel(int iFrom, int jFrom, int iTo, int jTo, int startingPosition, long totalWidth, char * lpPixels){
	int toIndex = iTo*3 + startingPosition*3 + jTo*totalWidth*3;
	int fromIndex = iFrom*3 + startingPosition*3 + jFrom*totalWidth*3;

	lpPixels[toIndex] = lpPixels[fromIndex];
	lpPixels[toIndex + 1] = lpPixels[fromIndex + 1];
	lpPixels[toIndex + 2] = lpPixels[fromIndex + 2];
}

void rotateBitmap90Degrees(int startingPosition, int frameWidth, int frameHeight, long totalWidth, char * lpPixels){
	int i,j,index;

	int i1,j1,i2,j2,i3,j3;
	char tmpR,tmpG,tmpB;

	int yCenter = (frameHeight / 2);
	int xCenter = ((frameWidth/ 2) + 0.5f);

	//transformation is incorrect, doing a flip instead of rotation
	for(j = 0; j < yCenter; j++){
		for(i = 0; i < xCenter; i++){
			index = i*3 + startingPosition*3 + j*totalWidth*3;

			// Store tmp pixel
			tmpB = lpPixels[index];
			tmpG = lpPixels[index + 1];
			tmpR = lpPixels[index + 2];

			i1 = -1*(j) + 99;
			j1 = i;

			i2 = -1*(j1) + 99;
			j2 = i1;

			i3 = -1*(j2) + 99;
			j3 = i2;

			//make sure j is being correctly calculated
			copyPixel(i1, j1, i, j, startingPosition, totalWidth, lpPixels);
			copyPixel(i2, j2, i1, j1, startingPosition, totalWidth, lpPixels);
			copyPixel(i3, j3, i2, j2, startingPosition, totalWidth, lpPixels);

			lpPixels[i3*3 + startingPosition*3 + j3*totalWidth*3] = tmpB;
			lpPixels[i3*3 + startingPosition*3 + j3*totalWidth*3 + 1] = tmpG;
			lpPixels[i3*3 + startingPosition*3 + j3*totalWidth*3 + 2] = tmpR;

			if(i == 49 && j == 49){
				printf("asdf");
				index++;
			}
		}
	}
}

void rotateAnimationFrames(HDC hdc, HDC hdcBuffer, animation * thisAnimation, int direction){
	int i;
//	HDC hdc = GetModuleHandle(hwnd);
//	HDC hdcBuffer = CreateCompatibleDC(hdc);

	BITMAPINFO bitMapInfo = {0};
	bitMapInfo.bmiHeader.biSize = sizeof(bitMapInfo.bmiHeader);

	HBITMAP thisImage = LoadImage(GetModuleHandle(NULL), thisAnimation->imageID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

	//Fill out bitMapInfo
	if(!GetDIBits(hdcBuffer, thisImage, 0, 0, NULL, &bitMapInfo, DIB_RGB_COLORS)){
		printf("Couldint get the bitMapInfo.\n");
		return;
	}

	//Create pixel buffer
	char * lpPixels = malloc(sizeof(char)*(bitMapInfo.bmiHeader.biSizeImage));

	bitMapInfo.bmiHeader.biCompression = BI_RGB;
	bitMapInfo.bmiHeader.biHeight = abs(bitMapInfo.bmiHeader.biHeight);

	int result = GetDIBits(hdcBuffer, thisImage, 0, bitMapInfo.bmiHeader.biHeight, lpPixels, &bitMapInfo, DIB_RGB_COLORS);

	if(result == 0){
		printf("Couldint get the bitmap.\n");
		free(lpPixels);
		return;
	}

	if(direction == 3){
		for(i = 0; i < thisAnimation->numFrames; i++){
			rotateBitmap90Degrees(i*thisAnimation->width, thisAnimation->width, thisAnimation->height, bitMapInfo.bmiHeader.biWidth, lpPixels);
//			int j, k, l;
//			int width = bitMapInfo.bmiHeader.biWidth;

//			for(j = 0; j < bitMapInfo.bmiHeader.biSizeImage; j+=3){
//				if(isGreaterThanPercentage(j, bitMapInfo.bmiHeader.biSizeImage, 66)){
//					lpPixels[j] = 255;
//					lpPixels[j+1] = 0;
//					lpPixels[j+2] = 0;
//				} else if(isGreaterThanPercentage(j, bitMapInfo.bmiHeader.biSizeImage, 33)){
//					lpPixels[j] = 255;
//					lpPixels[j+1] = 0;
//					lpPixels[j+2] = 0;
//				} else{
//					lpPixels[j] = 0;
//					lpPixels[j+1] = 255;
//					lpPixels[j+2] = 0;
//				}
//			}

//			for (k = 0; k < bitMapInfo.bmiHeader.biHeight; k++){
//				for(j = 0; j < width*3; j+=3){
//					if(j+(k*width) >= bitMapInfo.bmiHeader.biSizeImage){
//						printf("limit!");
//						break;
//					}
//
//					lpPixels[j+(k*width*3)] = ((j+k) % 255);
//					lpPixels[j+1+(k*width*3)] = ((2*j+k) % 255);
//					lpPixels[j+2+(k*width*3)] = ((j+2*k) % 255);
//				}
//			}
		}
	}

	if(!SetDIBits(hdcBuffer, thisImage, 0, bitMapInfo.bmiHeader.biHeight, lpPixels, &bitMapInfo, DIB_RGB_COLORS)){
		printf("Couldn't set the bitmap.\n");
		free(lpPixels);
		return;
	}

	thisAnimation->image = thisImage;
	thisAnimation->imageMask = CreateBitmapMask(thisAnimation->image, RGB(255, 0, 255));
	free(lpPixels);
}

