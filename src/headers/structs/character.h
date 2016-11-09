/*
 * character.h
 *
 *  Created on: Mar 25, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_CHARACTER_H_
#define SRC_HEADERS_CHARACTER_H_
#include<stdlib.h>
#include<windows.h>

typedef enum {
	ANIMATION_IDLE = 0,
	ANIMATION_IDLE_EQUIPT = 1,
	ANIMATION_ATTACK_SLASH = 2,
	ANIMATION_ATTACK_CHOP = 3,
	ANIMATION_ATTACK_BLUNT = 4,
	ANIMATION_ATTACK_PIERCE = 5,
	ANIMATION_ATTACK_BOW = 6,
	ANIMATION_HARM = 7,
	ANIMATION_DEATH = 8,
	ANIMATION_CAST = 9,
	ANIMATION_CONSUME = 10
} animationState;

typedef struct {
	int imageID;
	HBITMAP image;
	HBITMAP imageMask;

	int width;
	int height;

	COLORREF rgb;

	animationState state;
	int animationX;
	int durationInTicks[20]; // e.g. [10, 15, 2, 1]
	int totalDuration;
	int numFrames;
	int MAX_FRAMES;
	int currentFrame;

	int soundFrame;
	int soundID;
} animation;

typedef struct {
	animation * animations[15];
	int numAnimations;
	int MAX_ANIMATIONS;
	int currentAnimation;
	int clockTickCount;
	int clockTickDelay;
	int nextAnimationAfterDelay;
	int defaultAnimation;
	int animationsEnabled;
} animationContainer;

typedef struct {
	size_t size;
	int direction;
	int x;
	int y;
	animationContainer * thisAnimationContainer;
	animationContainer * secondaryAnimationContainer;
} character;


typedef struct {
	size_t size;
	int x;
	int y;

	int fixedImageID;

	int fixedWidth;
	int fixedHeight;
	int direction;

	COLORREF rgb;

	HBITMAP fixedImage;
	HBITMAP fixedImageMask;
} fixedCharacter;

typedef struct {
	int xShift;
	int yShift;
	int xShiftOld;
	int yShiftOld;
} shiftData;

#endif /* SRC_HEADERS_CHARACTER_H_ */
