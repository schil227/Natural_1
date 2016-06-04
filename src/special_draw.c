/*
 * special_draw.c
 *
 *  Created on: Jun 2, 2016
 *      Author: Adrian
 */
#include "./headers/special_draw_pub_methods.h"

static specialDraw * specialDrawInstance;

void initSpecialDrawInstance(){
	specialDrawInstance = malloc(sizeof(specialDraw));

	specialDrawInstance->numCharactersToDraw = 0;
	specialDrawInstance->MAX_CHARACTERS_TO_DRAW = 50;

	specialDrawInstance->numSpecialIndividuals = 0;
	specialDrawInstance->MAX_SPECIAL_INDIVIDUALS = 50;

	specialDrawInstance->currentTimerTicks = 0;
	specialDrawInstance->durationInTimerTicks = 0;
	specialDrawInstance->specialDrawModeEnabled = 0;
}

void destroySpecialDrawInstance(){
	free(specialDrawInstance);

	specialDrawInstance = NULL;
}

int isSpecialDrawModeEnabled(){
	if(specialDrawInstance != NULL){
		return specialDrawInstance->specialDrawModeEnabled;
	}

	return 0;
}

void enableSpecialDrawMode(){
	specialDrawInstance->specialDrawModeEnabled = 1;
}

void disableSpecialDrawMode(){
	specialDrawInstance->specialDrawModeEnabled = 0;
}

void setDurationInTimerTicks(int timerTicks){
	specialDrawInstance->durationInTimerTicks = timerTicks;
}

void incrementSpecialDrawTimerTicks(){
	specialDrawInstance->currentTimerTicks++;
}

int specialDrawDurationMet(){
	return specialDrawInstance->currentTimerTicks >= specialDrawInstance->durationInTimerTicks;
}

int addSpecialIndividual(individual * thisIndividual){
	if(specialDrawInstance->numSpecialIndividuals < specialDrawInstance->MAX_SPECIAL_INDIVIDUALS){
		specialDrawInstance->specialIndividuals[specialDrawInstance->numSpecialIndividuals] = thisIndividual;
		specialDrawInstance->numSpecialIndividuals++;
		return 1;
	}else{
		cwrite("!!CANNOT ADD SPECIAL INDIVIDUAL TO SPECIAL DRAW!!");
		return 0;
	}
}

int addCharacterToSpecialDrawWithCoords(character * thisCharacter, int x, int y){
	thisCharacter->x = x;
	thisCharacter->y = y;

	if(specialDrawInstance->numCharactersToDraw < specialDrawInstance->MAX_CHARACTERS_TO_DRAW){
		specialDrawInstance->charactersToDraw[specialDrawInstance->numCharactersToDraw] = thisCharacter;
		specialDrawInstance->numCharactersToDraw++;
		return 1;
	}else{
		cwrite("!!CANNOT ADD CHARACTER TO SPECIAL DRAW!!");
		return 0;
	}
}

int addCharacterToSpecialDraw(character * thisCharacter){
	if(specialDrawInstance->numCharactersToDraw < specialDrawInstance->MAX_CHARACTERS_TO_DRAW){
		specialDrawInstance->charactersToDraw[specialDrawInstance->numCharactersToDraw] = thisCharacter;
		specialDrawInstance->numCharactersToDraw++;
		return 1;
	}else{
		cwrite("!!CANNOT ADD CHARACTER TO SPECIAL DRAW!!");
		return 0;
	}
}

void drawSpecial(HDC hdc, HDC hdcBuffer, shiftData * viewShift){
	int i;

	for(i = 0; i < specialDrawInstance->numSpecialIndividuals; i++){
		drawIndividual(hdc, hdcBuffer, specialDrawInstance->specialIndividuals[i], viewShift);
	}

	for(i = 0; i < specialDrawInstance->numCharactersToDraw; i++){
		drawCharacter(hdc, hdcBuffer, specialDrawInstance->charactersToDraw[i], viewShift);
	}
}

void resetSpecialDraw(){
	int i;

	for(i = 0; i < specialDrawInstance->numSpecialIndividuals; i++){
		specialDrawInstance->specialIndividuals[i] = NULL;
	}

	specialDrawInstance->numSpecialIndividuals = 0;

	for(i = 0; i < specialDrawInstance->numCharactersToDraw; i++){
		specialDrawInstance->charactersToDraw[i] = NULL;
	}

	specialDrawInstance->numCharactersToDraw = 0;

	specialDrawInstance->currentTimerTicks = 0;
	specialDrawInstance->durationInTimerTicks = 0;

	specialDrawInstance->specialDrawModeEnabled = 0;
}
