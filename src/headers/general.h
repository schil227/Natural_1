/*
 * general.h
 *
 *  Created on: Feb 28, 2015
 *      Author: Adrian
 */

#ifndef GENERAL_H_
#define GENERAL_H_

typedef struct {
//	size_t typeSize;
	size_t size;
	char* charArr;
	int arrSize;

} charArrStruct;

charArrStruct *bigTriangle(int size);
char *join(char* str2,char* str1);

#endif /* GENERAL_H_ */
