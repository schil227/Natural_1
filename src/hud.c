/*
 * hud.c
 *
 *  Created on: Jan 26, 2017
 *      Author: Adrian
 */

#include "./headers/hud_pub_methods.h"
#define ACTION_MODE_IMAGE_ID 1509
#define POISON_MODE_IMAGE_ID 1510
#define BURNING_MODE_IMAGE_ID 1511
#define BERZERK_MODE_IMAGE_ID 1512
#define PARALYZED_MODE_IMAGE_ID 1513
#define BLEEDING_MODE_IMAGE_ID 1514
#define CONFUSED_MODE_IMAGE_ID 1515
#define SILENCED_MODE_IMAGE_ID 1516
#define SLEEPING_MODE_IMAGE_ID 1517
#define SNEAKING_MODE_IMAGE_ID 1518
#define ATTACK_SPACE_MARKER_IMAGE_ID 10002

static hudInstance * thisHudInstance;

void initHudInstance(){
	thisHudInstance = malloc(sizeof(hudInstance));
	thisHudInstance->actionModeCharacter = createCharacter(ACTION_MODE_IMAGE_ID, RGB(255,0,255), 0, 0);
	thisHudInstance->sneakingCharacter = createCharacter(SNEAKING_MODE_IMAGE_ID, RGB(255,0,255), 0, 0);
	thisHudInstance->poisonedCharacter = createCharacter(POISON_MODE_IMAGE_ID, RGB(255,0,255), 0, 0);
	thisHudInstance->burningCharacter = createCharacter(BURNING_MODE_IMAGE_ID, RGB(255,0,255), 0, 0);
	thisHudInstance->berzerkCharacter = createCharacter(BERZERK_MODE_IMAGE_ID, RGB(255,0,255), 0, 0);
	thisHudInstance->paralyzedCharacter = createCharacter(PARALYZED_MODE_IMAGE_ID, RGB(255,0,255), 0, 0);
	thisHudInstance->bleedingCharacter = createCharacter(BLEEDING_MODE_IMAGE_ID, RGB(255,0,255), 0, 0);
	thisHudInstance->confusedCharacter = createCharacter(CONFUSED_MODE_IMAGE_ID, RGB(255,0,255), 0, 0);
	thisHudInstance->silencedCharacter = createCharacter(SILENCED_MODE_IMAGE_ID, RGB(255,0,255), 0, 0);
	thisHudInstance->sleepCharacter = createCharacter(SLEEPING_MODE_IMAGE_ID, RGB(255,0,255), 0, 0);
	thisHudInstance->attackSpaceMarker = createCharacterFromAnimation(cloneAnimationFromRegistry(ATTACK_SPACE_MARKER_IMAGE_ID));

	thisHudInstance->isPoisoned = 0;
	thisHudInstance->isBurning = 0;
	thisHudInstance->isBerzerking = 0;
	thisHudInstance->isParalyzed = 0;
	thisHudInstance->isConfused = 0;
	thisHudInstance->isBleeding = 0;
	thisHudInstance->isSilenced = 0;
	thisHudInstance->isSleeping = 0;

	thisHudInstance->drawAttackSpaces = 0;
	thisHudInstance->attackSpaces = NULL;
}

void updateStatus(individual * player){
	int i;

	thisHudInstance->isPoisoned = 0;
	thisHudInstance->isBurning = 0;
	thisHudInstance->isBerzerking = 0;
	thisHudInstance->isParalyzed = 0;
	thisHudInstance->isConfused = 0;
	thisHudInstance->isBleeding = 0;
	thisHudInstance->isSilenced = 0;
	thisHudInstance->isSleeping = 0;

	for(i = 0; i < player->activeStatuses->numStatuses; i++){
		status * tmpStatus = player->activeStatuses->statuses[i];

		if(tmpStatus != NULL){
			switch(tmpStatus->effect){
				case STATUS_POISONED:
					thisHudInstance->isPoisoned = 1;
					break;
				case STATUS_BURNING:
					thisHudInstance->isBurning = 1;
					break;
				case STATUS_BERZERK:
					thisHudInstance->isBerzerking = 1;
					break;
				case STATUS_PARALYZED:
					thisHudInstance->isParalyzed = 1;
					break;
				case STATUS_CONFUSED:
					thisHudInstance->isConfused = 1;
					break;
				case STATUS_BLEEDING:
					thisHudInstance->isBleeding = 1;
					break;
				case STATUS_SILENCED:
					thisHudInstance->isSilenced = 1;
					break;
				case STATUS_SLEEPING:
					thisHudInstance->isSleeping = 1;
					break;
			}
		}
	}
}

void drawHudNotifications(HDC hdc, HDC hdcBuffer, RECT * prc, individual * player, int inActionMode){
	int offset = 0;
	HDC hdcMem = CreateCompatibleDC(hdc);

	int standardImageHeight = thisHudInstance->actionModeCharacter->fixedHeight;
	int standardImageWidth = thisHudInstance->actionModeCharacter->fixedWidth;
	int x = prc->right - 185 - standardImageWidth;
	int y = prc->bottom - 200 - standardImageHeight;

	if(inActionMode){
		SelectObject(hdcMem, thisHudInstance->actionModeCharacter->fixedImage);
		BitBlt(hdcBuffer, x, y, standardImageWidth, standardImageHeight, hdcMem, 0, 0, SRCCOPY);
		offset++;
	}

	if(player->isSneaking){
		SelectObject(hdcMem, thisHudInstance->sneakingCharacter->fixedImage);
		BitBlt(hdcBuffer, x, y - (standardImageHeight*offset), standardImageWidth, standardImageHeight, hdcMem, 0, 0, SRCCOPY);
		offset++;
	}

	if(player->activeStatuses->numStatuses > 0){
		updateStatus(player);

		if(thisHudInstance->isPoisoned){
			SelectObject(hdcMem, thisHudInstance->poisonedCharacter->fixedImage);
			BitBlt(hdcBuffer, x, y - (standardImageHeight*offset), standardImageWidth, standardImageHeight, hdcMem, 0, 0, SRCCOPY);
			offset++;
		}

		if(thisHudInstance->isBurning){
			SelectObject(hdcMem, thisHudInstance->burningCharacter->fixedImage);
			BitBlt(hdcBuffer, x, y - (standardImageHeight*offset), standardImageWidth, standardImageHeight, hdcMem, 0, 0, SRCCOPY);
			offset++;
		}

		if(thisHudInstance->isBerzerking){
			SelectObject(hdcMem, thisHudInstance->berzerkCharacter->fixedImage);
			BitBlt(hdcBuffer, x, y - (standardImageHeight*offset), standardImageWidth, standardImageHeight, hdcMem, 0, 0, SRCCOPY);
			offset++;
		}

		if(thisHudInstance->isParalyzed){
			SelectObject(hdcMem, thisHudInstance->paralyzedCharacter->fixedImage);
			BitBlt(hdcBuffer, x, y - (standardImageHeight*offset), standardImageWidth, standardImageHeight, hdcMem, 0, 0, SRCCOPY);
			offset++;
		}

		if(thisHudInstance->isConfused){
			SelectObject(hdcMem, thisHudInstance->confusedCharacter->fixedImage);
			BitBlt(hdcBuffer, x, y - (standardImageHeight*offset), standardImageWidth, standardImageHeight, hdcMem, 0, 0, SRCCOPY);
			offset++;
		}

		if(thisHudInstance->isBleeding){
			SelectObject(hdcMem, thisHudInstance->bleedingCharacter->fixedImage);
			BitBlt(hdcBuffer, x, y - (standardImageHeight*offset), standardImageWidth, standardImageHeight, hdcMem, 0, 0, SRCCOPY);
			offset++;
		}

		if(thisHudInstance->isSilenced){
			SelectObject(hdcMem, thisHudInstance->silencedCharacter->fixedImage);
			BitBlt(hdcBuffer, x, y - (standardImageHeight*offset), standardImageWidth, standardImageHeight, hdcMem, 0, 0, SRCCOPY);
			offset++;
		}

		if(thisHudInstance->isSleeping){
			SelectObject(hdcMem, thisHudInstance->sleepCharacter->fixedImage);
			BitBlt(hdcBuffer, x, y - (standardImageHeight*offset), standardImageWidth, standardImageHeight, hdcMem, 0, 0, SRCCOPY);
			offset++;
		}
	}

	DeleteDC(hdcMem);
}

void enableHUDAttackSpaces(cordArr * arr){
	if(arr != NULL){
		thisHudInstance->attackSpaces = arr;
	}

	thisHudInstance->drawAttackSpaces = 1;
}

void disableHUDAttackSpaces(){
	int i;

	thisHudInstance->drawAttackSpaces = 0;

	if(thisHudInstance->attackSpaces == NULL){
		return;
	}

	destroyCordArr(thisHudInstance->attackSpaces);

	thisHudInstance->attackSpaces = NULL;
}

void drawHUDAttackSpaces(HDC hdc, HDC hdcBuffer, RECT * prc, shiftData * viewShift){
	int i;

	if(!thisHudInstance->drawAttackSpaces ||thisHudInstance->attackSpaces == NULL){
		return;
	}

	for(i = 0; i < thisHudInstance->attackSpaces->numCords; i++){
		if(thisHudInstance->attackSpaces->cords[i] != NULL){
			drawOverlappingAnimation(hdc, hdcBuffer, thisHudInstance->attackSpaces->cords[i]->x, thisHudInstance->attackSpaces->cords[i]->y, thisHudInstance->attackSpaceMarker, viewShift, 0);
		}
	}
}
