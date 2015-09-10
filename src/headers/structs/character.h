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

typedef struct {
	size_t size;
	char* name;
	int nameLength;

	int width;
	int height;
	int x;
	int y;
	int direction;

	int imageID;
	HBITMAP image;
	HBITMAP imageMask;
	COLORREF rgb;

} character;

typedef struct {
	int xShift;
	int yShift;
	int xShiftOld;
	int yShiftOld;
} ShiftData;


#endif /* SRC_HEADERS_CHARACTER_H_ */
