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

	specialDrawInstance->numImagesToDraw = 0;
	specialDrawInstance->MAX_IMAGES_TO_DRAW = 50;

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

void increaseSpecialDrawDurationIfGreater(int newDelay){
	if(specialDrawInstance->durationInTimerTicks < newDelay){
		specialDrawInstance->durationInTimerTicks = newDelay;
	}
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
	if(specialDrawInstance->numImagesToDraw < specialDrawInstance->MAX_IMAGES_TO_DRAW){
		imageContainer * thisContainer = malloc(sizeof(imageContainer));
		thisContainer->image = thisCharacter;
		thisContainer->x = x;
		thisContainer->y = y;

		specialDrawInstance->imagesToDraw[specialDrawInstance->numImagesToDraw] = thisContainer;
		specialDrawInstance->numImagesToDraw++;
		return 1;
	}else{
		cwrite("!!CANNOT ADD CHARACTER TO SPECIAL DRAW!!");
		return 0;
	}
}

void drawSpecial(HDC hdc, HDC hdcBuffer, field * thisField, shiftData * viewShift){
	int i;

	for(i = 0; i < specialDrawInstance->numSpecialIndividuals; i++){
		if(!thisField->isDark || thisField->playerLoS >= max(abs(thisField->playerCords->x - specialDrawInstance->specialIndividuals[i]->playerCharacter->x), abs(thisField->playerCords->y - specialDrawInstance->specialIndividuals[i]->playerCharacter->y))){
			drawIndividual(hdc, hdcBuffer, specialDrawInstance->specialIndividuals[i], viewShift);
		}
	}

	for(i = 0; i < specialDrawInstance->numImagesToDraw; i++){
		drawUnboundAnimation(hdc, hdcBuffer, specialDrawInstance->imagesToDraw[i]->x, specialDrawInstance->imagesToDraw[i]->y, specialDrawInstance->imagesToDraw[i]->image, viewShift,0);
		updateAnimation(specialDrawInstance->imagesToDraw[i]->image);
	}
}

void resetSpecialDraw(){
	int i;

	for(i = 0; i < specialDrawInstance->numSpecialIndividuals; i++){
		specialDrawInstance->specialIndividuals[i] = NULL;
	}

	specialDrawInstance->numSpecialIndividuals = 0;

	for(i = 0; i < specialDrawInstance->numImagesToDraw; i++){
		destroyCharacter(specialDrawInstance->imagesToDraw[i]->image);
		free(specialDrawInstance->imagesToDraw[i]);
		specialDrawInstance->imagesToDraw[i] = NULL;
	}

	specialDrawInstance->numImagesToDraw = 0;
	specialDrawInstance->currentTimerTicks = 0;
	specialDrawInstance->durationInTimerTicks = 0;
	specialDrawInstance->specialDrawModeEnabled = 0;
}
