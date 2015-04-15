/*
 * individual.c
 *
 *  Created on: Apr 14, 2015
 *      Author: Adrian
 */

#include"./headers/individual_pub_methods.h"

void distroyIndividual(individual* thisIndividual){
	if(thisIndividual->playerCharacter){ //Null check
		destroyCharacter(thisIndividual->playerCharacter);
	}

	free(thisIndividual);

}
