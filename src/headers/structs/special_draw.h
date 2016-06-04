/*
 * special_draw.h
 *
 *  Created on: Jun 2, 2016
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_SPECIAL_DRAW_H_
#define SRC_HEADERS_STRUCTS_SPECIAL_DRAW_H_
#include "../individual_pub_methods.h"

typedef struct{
	individual * specialIndividuals[50];
	character * charactersToDraw[50];

	int specialDrawModeEnabled;
	int durationInTimerTicks;
	int currentTimerTicks;

	int numSpecialIndividuals;
	int numCharactersToDraw;
	int MAX_SPECIAL_INDIVIDUALS;
	int MAX_CHARACTERS_TO_DRAW;
} specialDraw;

#endif /* SRC_HEADERS_STRUCTS_SPECIAL_DRAW_H_ */
