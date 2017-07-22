/*
 * look_view.h
 *
 *  Created on: May 3, 2017
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_LOOK_VIEW_H_
#define SRC_HEADERS_STRUCTS_LOOK_VIEW_H_
#include "../field_pub_methods.h"

typedef enum {LOOK_INDIVIDUAL, LOOK_ITEM, LOOK_SPACE, LOOK_INTERACTABLE} lookType;

typedef struct{
	lookType thisType;
	character * thisCharacter;
	individual * thisIndividual;
	char description[256];
} lookData;

typedef struct{
	int inLookMode;
	int inLookScrollMode;

	lookData * thisLookData[50];
	int currentLookDataIndex;
	int numLookData;
	int MAX_LOOKDATA;

	fixedCharacter * mainWindow;
	fixedCharacter * enterImage;
	fixedCharacter * upScrollArrow;
	fixedCharacter * downScrollArrow;
	fixedCharacter * frame;
} lookView;


#endif /* SRC_HEADERS_STRUCTS_LOOK_VIEW_H_ */
