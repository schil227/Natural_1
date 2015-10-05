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
//	size_t size;
	char message[256];
	struct messageNode * nextMessageNode;
	struct messageNode * previousMessageNode;
} messageNode;

typedef struct {
//	size_t size;
	character * consoleCharacter;
	messageNode * newestMessageNode;
	messageNode * currentMessageNode;
	int numMessages;
	int numRows;
	int rowLength;
} console;
#endif /* SRC_HEADERS_STRUCTS_CONSOLE_H_ */
