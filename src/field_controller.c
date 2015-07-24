/*
 * field_controller.c
 *
 *  Created on: Jul 23, 2015
 *      Author: Adrian
 */

#include "./headers/field_controller_pub_methods.h"

enemies * initEnemies(){
	enemies * thisEnemies = malloc(sizeof(enemies));
	thisEnemies->numEnemies = 0;
	return thisEnemies;
}

int addEnemyToEnemies(enemies * thisEnemies, individual * enemey){
	if(thisEnemies->numEnemies < 50){
		int index = thisEnemies->numEnemies;
		thisEnemies->enemies[index] = enemey;
		thisEnemies->numEnemies = thisEnemies->numEnemies + 1;
		return 1;
	}

	return 0;
}

individual *  deleteEnemyFromEnemies(enemies * thisEnemies, individual * enemey){
	int index;
	int numEnemies = thisEnemies->numEnemies;
	for( index = 0; index < numEnemies; index++){
		if(thisEnemies->enemies[index] == enemey){
			individual * toReturn = thisEnemies->enemies[index];
			if(index == numEnemies - 1){ //last index
				thisEnemies->enemies[index] = NULL;
			}else{
				thisEnemies->enemies[index] = thisEnemies->enemies[numEnemies-1];
				thisEnemies->enemies[numEnemies-1] = NULL;
			}

			thisEnemies->numEnemies = thisEnemies->numEnemies - 1;
			return toReturn;

		}
	}

	return NULL;

}
