/*
 * cursor_pub_methods.h
 *
 *  Created on: Apr 26, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_CURSOR_PUB_METHODS_H_
#define SRC_HEADERS_CURSOR_PUB_METHODS_H_

#include"./structs/cursor.h"
#include"./field_pub_methods.h"
#include"./field_controller_pub_methods.h"

void drawCursor(HDC hdc, HDC hdcBuffer, cursor *thisCursor, int xShift, int yShift);
void destroyCursor(cursor* thisCursor);

#endif /* SRC_HEADERS_CURSOR_PUB_METHODS_H_ */
