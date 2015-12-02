/*
 * console_public_methods.h
 *
 *  Created on: Oct 5, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_CONSOLE_PUB_METHODS_H_
#define SRC_HEADERS_CONSOLE_PUB_METHODS_H_
#include "./structs/console.h"
void appendNewMessageNode(char * message);
char* appendStrings(char* str1, char* str2);
void clearMessages();
void cwrite(char* text);

#endif /* SRC_HEADERS_CONSOLE_PUB_METHODS_H_ */
