/*
 * loop_controller.c
 *
 *  Created on: Nov 3, 2015
 *      Author: Adrian
 */
#include "./headers/field_controller_pub_methods.h"
#include "./headers/cursor_pub_methods.h"

int cursorLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam,
		field * main_field, individual * player, groupContainer * thisGroupContainer, shiftData * viewShift) {
	switch (msg) {
	case WM_KEYDOWN: {
		switch (LOWORD(wParam)) {
		case 0x34: //left
		case 0x64:
		case 0x36: //right
		case 0x66:
		case 0x38: //up
		case 0x68:
		case 0x32: //down
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
			if(canMoveCursor(player)){
				moveCursor(main_field,direction, viewShift);
			}
			break;
		}
		case 0x1B: //escape
			if (getCursorMode() == CURSOR_ABILITY){
				player->activeAbilities->selectedAbility = NULL;
			}
			toggleInCursorMode();
			viewShift->xShift = viewShift->xShiftOld;
			viewShift->yShift = viewShift->yShiftOld;
			break;
		case 0x0D: //enter //TODO: when attacking, supply both enemies and NPCs, ensure the character cannot attack themselves
		{
			if (getCursorMode() == CURSOR_ATTACK) {//attack the individual
				if(tryAttackIndividual(thisGroupContainer, player, main_field, getCursorX(), getCursorY())){

					viewShift->xShift = viewShift->xShiftOld;
					viewShift->yShift = viewShift->yShiftOld;
					decreaseTurns(player, thisGroupContainer, 1);
					toggleInCursorMode();
				}

			}else if(getCursorMode() == CURSOR_TALK){ //talk to the individual
				tryTalkGroups(thisGroupContainer, player, getCursorX(), getCursorY());

				viewShift->xShift = viewShift->xShiftOld;
				viewShift->yShift = viewShift->yShiftOld;
				decreaseTurns(player, thisGroupContainer, 1);
				toggleInCursorMode();
			}else if (getCursorMode() == CURSOR_ABILITY){
				if(cursorWithinAbilityRange(player, getCursorX(), getCursorY())){
					int numActions = 1;
					useAbilityOnIndividualsInAOERange(player, player, thisGroupContainer, main_field, getCursorX(), getCursorY());

					if(player->activeAbilities->selectedAbility->actionsEnabled){
						numActions += player->activeAbilities->selectedAbility->actions->effectAndManaArray[player->activeAbilities->selectedAbility->actions->selectedIndex]->effectMagnitude;
					}

					player->activeAbilities->selectedAbility = NULL;
					viewShift->xShift = viewShift->xShiftOld;
					viewShift->yShift = viewShift->yShiftOld;

					decreaseTurns(player, thisGroupContainer, numActions);
					toggleInCursorMode();
				}
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
		destroyThisCursor();
		PostQuitMessage(0);
		break;
		default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	}
	return 0;
}

int specialDrawLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
	case WM_TIMER:
	{
		RECT rect;
		HDC hdc = GetDC(hwnd);
		GetClientRect(hwnd, &rect);
		drawAll(hdc, &rect);
		ReleaseDC(hwnd, hdc);
		incrementSpecialDrawTimerTicks();
		if(specialDrawDurationMet()){
			resetSpecialDraw();
		}
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
				int totalHP = player->baseHP + player->CON * 2;
				int totalMana = player->baseMana + player->WILL * 2;
				interpretLeftAbilityCreation(player->range, player->mvmt, totalHP, totalMana);
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


int abilityViewLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, individual * player, field * thisField){
	switch(msg){
	case WM_KEYDOWN:{

		switch (LOWORD(wParam)) {
			case 0x1B:{ //esc
				toggleAbilityViewMode();
				resetAbilityView();
			}
			break;
			case 0x0D: { //enter
				if(canUseAbility(player,getAbilityToActivate())){
					if(useAbility(player,getAbilityToActivate())){
						removeIndividualFromField(thisField, player->playerCharacter->x, player->playerCharacter->y);
						triggerEventOnDeath(player->ID, player->isPlayer);
						removeFromExistance(player->ID);
						return 0;
					}

					if(player->activeAbilities->selectedAbility != NULL && (player->activeAbilities->selectedAbility->type == 't' || player->activeAbilities->selectedAbility->type == 'd')){
						toggleInCursorMode();
						refreshCursor(CURSOR_ABILITY, player->playerCharacter->x, player->playerCharacter->y);
					}

					toggleAbilityViewMode();
					resetAbilityView();
				}
				break;
			}
			case 0x38:
			case 0x68:{ //'8'
				selectPreviousAbility();
				break;
			}
			case 0x32:
			case 0x62:{ //'2' key
				selectNextAbility();
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

int dialogLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, individual * player, groupContainer * thisGroupContainer, field * thisField){
	switch(msg){
	case WM_KEYDOWN:{
		switch (LOWORD(wParam)) {
			case 0x1B: //esc
			case 0x0D: { //enter
				if(disableSpeakModeIfEnabled()){
					break;
				}

				int eventID = getEventFromCurrentMessage();

				if(eventID != 0){
					processEvent(eventID, player, thisGroupContainer, thisField);
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
			shouldSpeakTickTrigger();
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

int inventoryLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, field * main_field, individual * player, shiftData * viewShift) {
	switch (msg) {
	case WM_KEYDOWN: {

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

			if (inBuyMode()) {
				disableInventoryBuyMode();
			}

			break;
		case 0x0D: //enter
		{
			item * tmpItem = getSelectedItem();

			if (tmpItem != NULL) {
				if (inBuyMode()) {
					attemptToBuyItem(tmpItem, player);
				} else {
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
				setSimpleDialogMessage(tmpItem->description);
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
					if (thisMoveNodeMeta->pathLength < getAttributeSum(thisIndividual, "mvmt")) {


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

			thisIndividual->playerCharacter->x = (*tmpMoveNode)->x;
			thisIndividual->playerCharacter->y = (*tmpMoveNode)->y;

			if(updateViewShift){
				tryUpdateXShift(viewShift, (*tmpMoveNode)->x);
				tryUpdateYShift(viewShift, (*tmpMoveNode)->y);
			}

			if((*tmpMoveNode)->nextMoveNode == NULL){
				setIndividualSpace(thisField, thisIndividual,(*tmpMoveNode)->x,(*tmpMoveNode)->y);
				*postMoveMode = 0;
			}
		}


		}
		break;
		default: {

		}
	}
}
