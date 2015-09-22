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
void drawField(HDC hdc, HDC hdcBuffer, field* this_field, shiftData * viewShift);
int moveIndividual(field *thisField, individual *thisIndividual, int direction);
void wanderAround(field * thisField, individual * thisIndividual);
int moveCursor(field *thisField, cursor *thisCursor, int direction, shiftData * viewShift);
space** getAdjacentSpaces(field *thisField, int x, int y);
space* getSpaceFromField(field* thisField, int x, int y);
int moveLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, int * moveMode,field * thisField, individual * thisIndividual, moveNodeMeta * thisMoveNodeMeta, int * postMoveMode, shiftData * viewShift);
void animateMoveLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, field * thisField, individual * thisIndividual, moveNodeMeta * thisMoveNodeMeta, int speed, int * postMoveMode, shiftData * viewShift);
int removeIndividualFromField(field * thisField, int x, int y);
#endif /* SRC_HEADERS_FIELD_PUB_METHODS_H_ */
