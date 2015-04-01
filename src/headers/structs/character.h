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

	HBITMAP image;
	HBITMAP imageMask;

} character;


#endif /* SRC_HEADERS_CHARACTER_H_ */
