/*
 * loop_controller.c
 *
 *  Created on: Nov 3, 2015
 *      Author: Adrian
 */
#include "./headers/field_controller_pub_methods.h"
#include "./headers/cursor_pub_methods.h"

int cursorLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, int * cursorMode, int * postCursorMode, cursor * thisCursor,
		field * main_field, individual * player, individualGroup  * enemies, individualGroup  * npcs, shiftData * viewShift) {
	int toReturn = 0;
	switch (msg) {
	case WM_KEYDOWN: {
		switch (LOWORD(wParam)) {
		case 0x34: //left
		case 0x64:
		case 0x36:
		case 0x66:
		case 0x38:
		case 0x68:
		case 0x32:
		case 0x62:
		case 0x31: //down left
		case 0x61:
		case 0x37: //up left
		case 0x67:
		case 0x39: //up right
		case 0x69:
		case 0x33: //down right
		case 0x63:
		{
			int direction;
			direction = LOWORD(wParam) % 16;
			printf("cursor y:%d \n", viewShift->yShift);
			moveCursor(main_field,thisCursor,direction, viewShift);
			break;
		}
		case 0x1B: //escape
			*cursorMode = 0;
			viewShift->xShift = viewShift->xShiftOld;
			viewShift->yShift = viewShift->yShiftOld;
			break;
		case 0x0D: //enter //TODO: when attacking, supply both enemies and NPCs, ensure the character cannot attack themselves
		{
			if (*cursorMode == 1) {//attack the individual
				int cX, cY, index;
				cX = thisCursor->cursorCharacter->x;
				cY = thisCursor->cursorCharacter->y;

				for (index = 0; index < enemies->numIndividuals; index++) {

					individual * tmpEnemy = enemies->individuals[index];

					if (tmpEnemy->playerCharacter->x == cX && tmpEnemy->playerCharacter->y == cY && individualWithinRange(player, tmpEnemy)) {
						printf("attacked!");
						if (attackIndividual(player, tmpEnemy)) {
							deleteIndividiaulFromGroup(enemies, tmpEnemy);
							removeIndividualFromField(main_field, tmpEnemy->playerCharacter->x, tmpEnemy->playerCharacter->y);
//							destroyIndividual(tmpEnemy);
						}
						*cursorMode = 0;
						*postCursorMode = 1;

						viewShift->xShift = viewShift->xShiftOld;
						viewShift->yShift = viewShift->yShiftOld;
						break;
					}

				}
			}else if(*cursorMode == 2){ //talk to the individual
				int cX, cY;
				cX = thisCursor->cursorCharacter->x;
				cY = thisCursor->cursorCharacter->y;

				if(!tryTalk(npcs,player,cX,cY) && !tryTalk(enemies,player,cX,cY)){
					cwrite("There's nobody there.");
				}

				*cursorMode = 0;
				*postCursorMode = 1;
			}

		}
			break;
		}
		case WM_TIMER:
		{
			RECT rect;
			HDC hdc = GetDC(hwnd);
			GetClientRect(hwnd, &rect);
			drawAll(hdc, &rect);

			ReleaseDC(hwnd, hdc);
		}
		break;
		case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
		case WM_DESTROY:
		destroyCursor(thisCursor);
		PostQuitMessage(0);
		break;
		default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	}
	return 0;
}

int nameLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, individual * player){
	switch(msg){
	case WM_KEYDOWN:{
		switch (LOWORD(wParam)) {
			case 0x1B:{ //esc
				toggleNameMode();
				resetNameBoxInstance();
				break;
			}
			case 0x0D: { //enter
				if(selectCharacter()){
					toggleNameMode();
				}
				break;
			}
			case 0x38:
			case 0x68:{ //'8'
				selectLetterUp();
				break;
			}
			case 0x32:
			case 0x62:{ //'2' key
				selectLetterDown();
				break;
			}
			case 0x36:
			case 0x66:{ //'6'
				selectLetterRight();
				break;
			}
			case 0x34:
			case 0x64:{ //'4' key
				selectLetterLeft();
				break;
			}
		}
		case WM_TIMER:
		{
			RECT rect;
			HDC hdc = GetDC(hwnd);
			GetClientRect(hwnd, &rect);
			drawAll(hdc, &rect);
			ReleaseDC(hwnd, hdc);
		}
			break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	}
	return 0;
}


int createAbilityLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, individual * player){
	switch(msg){
	case WM_KEYDOWN:{

		switch (LOWORD(wParam)) {
			case 0x1B:{ //esc
				toggleCreateMode();
			}
			case 0x0D: { //enter
				if(canCreateAbility()){
					toggleNameMode();
					toggleAbilityWaitForNameMode();
				}
				break;
			}
			case 0x38:
			case 0x68:{ //'8'
				selectPreviousEffect();
				break;
			}
			case 0x32:
			case 0x62:{ //'2' key
				selectNextEffect();
				break;
			}
			case 0x36:
			case 0x66:{ //'6'
				interpretRightAbilityCreation();
				break;
			}
			case 0x34:
			case 0x64:{ //'4' key
				interpretLeftAbilityCreation(player->range, player->mvmt, player->totalHP, player->totalMana);
				break;
			}
			case 0x43:{ //'c' key
//				if(canCreateAbility()){
//					toggleNameMode();
//					addAbilityToIndividual(player, getNewAbility());
//					changeAbilityTemplate(0);
//				}
				break;
			}
		}
		case WM_TIMER:
		{
			RECT rect;
			HDC hdc = GetDC(hwnd);
			GetClientRect(hwnd, &rect);
			drawAll(hdc, &rect);
			ReleaseDC(hwnd, hdc);
		}
			break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	}
	return 0;
}

int dialogLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, individual * player, individualGroup * npcs, individualGroup * enemies, field * thisField){
	switch(msg){
	case WM_KEYDOWN:{
		switch (LOWORD(wParam)) {
			case 0x1B: //esc
			case 0x0D: { //enter

				int eventID = getEventFromCurrentMessage();

				if(eventID != 0){
					processEvent(eventID, player, npcs, enemies, thisField);
				}

				advanceDialog();

				break;
			}
			case 0x38:
			case 0x68:{ //'8'
				previousDialogDecision();
				break;
			}
			case 0x32:
			case 0x62:{ //'2' key
				nextDialogDecision();
				break;
			}

		}
		case WM_TIMER:
		{
			RECT rect;
			HDC hdc = GetDC(hwnd);
			GetClientRect(hwnd, &rect);
			drawAll(hdc, &rect);
			ReleaseDC(hwnd, hdc);
		}
			break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	}
	return 0;
}

int inventoryLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, field * main_field, individual * player, individualGroup  * thisEnemies, shiftData * viewShift) {
	int toReturn = 0;
	switch (msg) {
	case WM_KEYDOWN: {

//		if(shouldDrawDialogBox()){
//			switch (LOWORD(wParam)) {
//				case 0x1B:
//				{	//stop drawing dialog box
//					toggleDrawDialogBox();
//				}
//			}
//		}else{
			switch (LOWORD(wParam)) {
			case 0x38:
			case 0x68: {
				selectPreviousItemUp();
			}
				break;
			case 0x32:
			case 0x62: {
				selectNextItemDown();

			}
				break;
				break;
			case 0x1B: //escape
				disableInventoryViewMode();

				if(inBuyMode()){
					disableInventoryBuyMode();
				}

				break;
			case 0x0D: //enter
			{
				item * tmpItem = getSelectedItem();

				if (tmpItem != NULL) {
					if(inBuyMode()){
						attemptToBuyItem(tmpItem, player);
					}else{
						modifyItem(tmpItem, player);
						refreshInventory(player->backpack);
					}

				}
			}
				break;
			case 0x44: //(d)escription
			{
				item * tmpItem = getSelectedItem();

				if (tmpItem != NULL) {
					toggleDrawDialogBox();
//					char tmpDescription[256];
//					strcpy(tmpDescription, tmpItem->description);
//					char * value = strtok(tmpDescription, "\\");
//					cwrite(value);
//					value = strtok(NULL, "\\");
//					cwrite(value);

					setSimpleDialogMessage(tmpItem->description);
				}
			}
				break;
			}
//		}
		case WM_TIMER:
		{
			RECT rect;
			HDC hdc = GetDC(hwnd);
			GetClientRect(hwnd, &rect);
			drawAll(hdc, &rect);

			ReleaseDC(hwnd, hdc);
		}
		break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			destroyTheGlobalRegister();
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	}
	return 0;
}


int moveLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, int * moveMode,
		field * thisField, individual * thisIndividual, moveNodeMeta * thisMoveNodeMeta, int * postMoveMode, shiftData * viewShift) {
	switch (msg) {
	case WM_KEYDOWN: {
		switch (LOWORD(wParam)) {
		//number->direction on numpad
		case 0x31:
		case 0x61:
		case 0x32:
		case 0x62:
		case 0x33:
		case 0x63:
		case 0x34:
		case 0x64:
		case 0x36:
		case 0x66:
		case 0x37:
		case 0x67:
		case 0x38:
		case 0x68:
		case 0x39:
		case 0x69:
			{

				int dx = xMoveChange(LOWORD(wParam) % 16);
				int dy = yMoveChange(LOWORD(wParam) % 16);
				moveNode ** currentNode = &(thisMoveNodeMeta->rootMoveNode);

				while((*currentNode)->nextMoveNode != NULL){
//					printf("current x:%d y:%d\n",(*currentNode)->x,(*currentNode)->y);
					moveNode * tmpMoveNode = (moveNode *)(*currentNode)->nextMoveNode;
					currentNode = &tmpMoveNode;
				}

//				printf("current selected x:%d y:%d\n",(*currentNode)->x,(*currentNode)->y);

				space ** tmpSpace = (space **)getSpaceAddressFromField(thisField, (*currentNode)->x + dx, (*currentNode)->y + dy);
				if ( tmpSpace != NULL && ((*tmpSpace)->currentIndividual == NULL || (*tmpSpace)->currentIndividual == thisIndividual) && (*tmpSpace)->isPassable) {


					moveNode ** oldNode = (moveNode **)alreadyContainsNode(thisMoveNodeMeta->rootMoveNode,(*currentNode)->x + dx, (*currentNode)->y + dy );

					if(oldNode == NULL){
					if (thisMoveNodeMeta->pathLength < thisIndividual->mvmt) {


						moveNode * nextNode = malloc(sizeof(moveNode));
						nextNode->x = (*currentNode)->x + dx;
						nextNode->y = (*currentNode)->y + dy;
						nextNode->nextMoveNode = NULL;
						nextNode->hasTraversed = 0;
						(*currentNode)->nextMoveNode = (moveNode *)nextNode;

						thisMoveNodeMeta->pathLength = thisMoveNodeMeta->pathLength + 1;

						tryUpdateXShift(viewShift, nextNode->x);
						tryUpdateYShift(viewShift, nextNode->y);
					}
					}else{ //node already exists
						int removedNodes = freeUpMovePath((moveNode *)(*oldNode)->nextMoveNode);
						(*oldNode)->nextMoveNode = NULL;
						thisMoveNodeMeta->pathLength =  thisMoveNodeMeta->pathLength - removedNodes;
						tryUpdateXShift(viewShift, (*oldNode)->x);
						tryUpdateYShift(viewShift, (*oldNode)->y);
					}

				}

//				free(currentNode);
			}
			break;
		case 0x1B: //escape
			*moveMode = 0;
			viewShift->xShift = viewShift->xShiftOld;
			viewShift->yShift = viewShift->yShiftOld;
			break;
		case 0x0D: //enter
		{
			*moveMode = 0;
			if(thisMoveNodeMeta->rootMoveNode->nextMoveNode != NULL){
				*postMoveMode = 1;
				getSpaceFromField(thisField, thisIndividual->playerCharacter->x, thisIndividual->playerCharacter->y)->currentIndividual = NULL;
				viewShift->xShift = viewShift->xShiftOld;
				viewShift->yShift = viewShift->yShiftOld;
			}

		}
			break;
		}
		case WM_TIMER:
		{
			RECT rect;
			HDC hdc = GetDC(hwnd);
			GetClientRect(hwnd, &rect);
			drawAll(hdc, &rect);
			ReleaseDC(hwnd, hdc);
		}
		break;
		case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
		case WM_DESTROY:

		PostQuitMessage(0);
		break;
		default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	}
	return 0;
}

void animateMoveLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, field * thisField,
		individual * thisIndividual, moveNodeMeta * thisMoveNodeMeta, int speed,
		int * postMoveMode, shiftData * viewShift, int updateViewShift){
	switch (msg) {

		case WM_TIMER: {
		RECT rect;
		HDC hdc = GetDC(hwnd);
		GetClientRect(hwnd, &rect);
		drawAll(hdc, &rect);
		thisMoveNodeMeta->sum = thisMoveNodeMeta->sum +1;
		ReleaseDC(hwnd, hdc);

		if(thisMoveNodeMeta->sum > speed){
			thisMoveNodeMeta->sum = 0;
			moveNode ** tmpMoveNode = &(thisMoveNodeMeta->rootMoveNode);

			while((*tmpMoveNode)->hasTraversed){
				moveNode * nextTmpMoveNode = (moveNode *) (*tmpMoveNode)->nextMoveNode;
				tmpMoveNode = &nextTmpMoveNode;
			}

			(*tmpMoveNode)->hasTraversed = 1;

//			moveIndividualSpace(thisField,thisIndividual,(*tmpMoveNode)->x, (*tmpMoveNode)->y);
			thisIndividual->playerCharacter->x = (*tmpMoveNode)->x;
			thisIndividual->playerCharacter->y = (*tmpMoveNode)->y;

			if(updateViewShift){
				tryUpdateXShift(viewShift, (*tmpMoveNode)->x);
				tryUpdateYShift(viewShift, (*tmpMoveNode)->y);
			}

			if((*tmpMoveNode)->nextMoveNode == NULL){
				setIndividualSpace(thisField, thisIndividual,(*tmpMoveNode)->x,(*tmpMoveNode)->y);
				*postMoveMode = 0;
//				freeUpMovePath((moveNode *) rootMoveNode->nextMoveNode);
//				rootMoveNode->nextMoveNode = NULL;
			}
		}


		}
		break;
		default: {

		}
	}
}
