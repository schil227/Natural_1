/*
 * pause.h
 *
 *  Created on: May 7, 2017
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_PAUSE_H_
#define SRC_HEADERS_STRUCTS_PAUSE_H_
#include "../character_pub_methods.h"

typedef struct{
	int isPaused;
	fixedCharacter * pauseWindow;
} pauseViewContainer;

#endif /* SRC_HEADERS_STRUCTS_PAUSE_H_ */
