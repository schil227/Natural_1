/*
 * character_pub_methods.h
 *
 *  Created on: Mar 27, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_CHARACTER_PUB_METHODS_H_
#define SRC_HEADERS_CHARACTER_PUB_METHODS_H_
#include "./structs/character.h"

fixedCharacter * createCharacter(int imageID, COLORREF rgb, int x, int y);
void destroyCharacter(character* thisCharacter);
void destroyFixedCharacter(fixedCharacter* thisCharacter);
void moveCharacter(character* thisCharacter, int newX, int newY);
void drawCharacter(HDC hdc, HDC hdcBuffer, character * thisCharacter, shiftData * viewShift);
HBITMAP CreateBitmapMask(HBITMAP hbmColor, COLORREF crTransparent);

animationContainer * initAnimationContainer();
animationContainer * cloneAnimationContainer(animationContainer * baseAnimationContainer);
animation * initAnimation(animationState state);
animation * cloneAnimation(animation * thisAnimation);

#endif /* SRC_HEADERS_CHARACTER_PUB_METHODS_H_ */
