/*
 * field_pub_methods.h
 *
 *  Created on: Mar 29, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_FIELD_PUB_METHODS_H_
#define SRC_HEADERS_FIELD_PUB_METHODS_H_
#include "./structs/field.h"

field* initField(char* fieldFileName);
void drawField(HDC hdc, HDC hdcBuffer, field* this_field);

#endif /* SRC_HEADERS_FIELD_PUB_METHODS_H_ */