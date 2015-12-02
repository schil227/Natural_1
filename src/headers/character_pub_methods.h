/*
 * character_pub_methods.h
 *
 *  Created on: Mar 27, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_CHARACTER_PUB_METHODS_H_
#define SRC_HEADERS_CHARACTER_PUB_METHODS_H_
#include "./structs/character.h"

character * createCharacter(int imageID, COLORREF rgb, int x, int y);
void nameCharacter(character* thisCharacter, char* name);
void destroyCharacter(character* thisCharacter);
void moveCharacter(character* thisCharacter, int newX, int newY);
void drawCharacter(HDC hdc, HDC hdcBuffer, character * thisCharacter, shiftData * viewShift);
void drawUnboundCharacter(HDC hdc, HDC hdcBuffer, int x, int y, character * thisCharacter, shiftData * viewShift);
HBITMAP CreateBitmapMask(HBITMAP hbmColor, COLORREF crTransparent);
#endif /* SRC_HEADERS_CHARACTER_PUB_METHODS_H_ */
