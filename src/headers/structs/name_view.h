/*
 * name_view.h
 *
 *  Created on: Mar 27, 2016
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_NAME_VIEW_H_
#define SRC_HEADERS_STRUCTS_NAME_VIEW_H_

#include "../character_pub_methods.h"

typedef struct{
	char charArr[65];
	char name[16];

	int inNameMode;
	int column;
	int row;
	int currentCharArrIndex;
	int currentNameIndex;
	int MAX_ROWS;
	int MAX_COLUMNS;
	int MAX_NAME_LENGTH;

	fixedCharacter * nameBox;
}nameBoxInstance;

#endif /* SRC_HEADERS_STRUCTS_NAME_VIEW_H_ */
