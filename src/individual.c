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

void drawPlayer(HDC hdc, HDC hdcBuffer, individual* player){
		HDC hdcMem = CreateCompatibleDC(hdc);
		SelectObject(hdcMem, player->playerCharacter->imageMask);

		BitBlt(hdcBuffer, player->playerCharacter->x*40, player->playerCharacter->y*40, player->playerCharacter->width, player->playerCharacter->height, hdcMem, 0, 0, SRCAND);

		SelectObject(hdcMem, player->playerCharacter->image);// player->image);

		BitBlt(hdcBuffer, player->playerCharacter->x*40, player->playerCharacter->y*40, player->playerCharacter->width, player->playerCharacter->height, hdcMem, 0, 0, SRCPAINT); //was SRCPAINT
		DeleteDC(hdcMem);
}
