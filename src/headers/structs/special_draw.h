/*
 * special_draw.h
 *
 *  Created on: Jun 2, 2016
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_SPECIAL_DRAW_H_
#define SRC_HEADERS_STRUCTS_SPECIAL_DRAW_H_
#include "../field_pub_methods.h"

typedef struct{
	int x;
	int y;
	character * image;
}imageContainer;

typedef struct{
	individual * specialIndividuals[50];
	imageContainer * imagesToDraw[50];

	int specialDrawModeEnabled;
	int durationInTimerTicks;
	int currentTimerTicks;

	int numSpecialIndividuals;
	int numImagesToDraw;
	int MAX_SPECIAL_INDIVIDUALS;
	int MAX_IMAGES_TO_DRAW;
} specialDraw;

#endif /* SRC_HEADERS_STRUCTS_SPECIAL_DRAW_H_ */
