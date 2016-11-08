/*
 * console.h
 *
 *  Created on: Oct 5, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_CONSOLE_H_
#define SRC_HEADERS_STRUCTS_CONSOLE_H_

#include "../character_pub_methods.h"

typedef struct{
	char message[256];
	struct messageNode * nextMessageNode;
	struct messageNode * previousMessageNode;
} messageNode;

typedef struct {
	fixedCharacter * consoleCharacter;
	messageNode * newestMessageNode;
	messageNode * currentMessageNode;
	messageNode * oldestMessageNode;
	int numMessages;
	int numRows;
	int rowLength;
} console;

#endif /* SRC_HEADERS_STRUCTS_CONSOLE_H_ */
