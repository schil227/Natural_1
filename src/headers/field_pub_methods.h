/*
 * field_pub_methods.h
 *
 *  Created on: Mar 29, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_FIELD_PUB_METHODS_H_
#define SRC_HEADERS_FIELD_PUB_METHODS_H_

#include "./structs/field.h"
#include "./cursor_pub_methods.h"


field* initField(char* fieldFileName);
void drawField(HDC hdc, HDC hdcBuffer, field* thisField, int consoleHeightStartY, int sideBarStartX, shiftData * viewShift);
int moveIndividual(field *thisField, individual *thisIndividual, int direction);
void wanderAround(field * thisField, individual * thisIndividual);
space** getAdjacentSpaces(field *thisField, int x, int y);
space* getSpaceFromField(field* thisField, int x, int y);
int removeIndividualFromField(field * thisField, int x, int y);
int moveIndividualSpace(field *thisField, individual *thisIndividual, int x, int y);
int isSpacePassable(field* thisField, int x, int y);
individual* getIndividualAddressFromField(field* thisField, int x, int y);
int addItemToField(field * thisField, item * thisItem);
moveNode * alreadyContainsNode(moveNode * rootNode, int x, int y);
int attemptGetItemFromField(field * thisField, individual * thisIndividual);
int calcXMod(int direction, character * backgroundCharacter, shiftData * viewShift);
int calcYMod(int direction, character * backgroundCharacter, shiftData * viewShift);

#endif /* SRC_HEADERS_FIELD_PUB_METHODS_H_ */
