/*
 * field.c
 *
 *  Created on: Mar 29, 2015
 *      Author: Adrian
 */

#include"./headers/field_pub_methods.h"

characterArray getCharacterArrayFromField(field* thisField, int x, int y){
	if(x < thisField->width && x <= 0 && y < thisField->height && y <= 0){
		return thisField->grid[x][y];
	}

	characterArray *chararr = malloc(sizeof(characterArray));
	return *chararr;
}
