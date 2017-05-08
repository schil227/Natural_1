/*
 * loop_controller.c
 *
 *  Created on: Nov 3, 2015
 *      Author: Adrian
 */
#include "./headers/field_controller_pub_methods.h"
#include "./headers/cursor_pub_methods.h"

int cursorLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam,
		field * main_field, individual * player, groupContainer * thisGroupContainer, shiftData * viewShift, int * inActionMode, int * playerControlMode, int animateMoveSpeed) {
	switch (msg) {
	case WM_KEYDOWN: {
		switch (LOWORD(wParam)) {
		case 0x50://p key (inventory)
			{
				togglePaused();
			}
			break;
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

			RECT rect;
			GetClientRect(hwnd, &rect);

			if(canMoveCursor(player)){
				moveCursor(main_field,direction, viewShift, &rect);

				if(getCursorMode() == CURSOR_LOOK){
					populateLookDataInstance(main_field, player, getCursorX(), getCursorY());
				}
			}
			break;
		}
		case 0x1B: //escape
			if (getCursorMode() == CURSOR_ABILITY){
				player->activeAbilities->selectedAbility = NULL;
			}else if(getCursorMode() == CURSOR_LOOK){
				disableLookMode();
			}

			toggleInCursorMode();
			disableHUDAttackSpaces();

			viewShift->xShift = viewShift->xShiftOld;
			viewShift->yShift = viewShift->yShiftOld;
			break;
		case 0x0D: //enter
		{
			if (getCursorMode() == CURSOR_ATTACK) {

				if(getIndividualFromField(main_field, getCursorX(), getCursorY()) == NULL || !cordWithinRange(player, getCursorX(), getCursorY())){
					break;
				}

				if(hasActiveStatusEffect(player, STATUS_CONFUSED) && isGreaterThanPercentage(rand() % 100, 100, 50)){
					player->thisBehavior->gotConfused = 1;

					char logOut[128];
					sprintf(logOut, "%s is confused!", player->name);
					cwrite(logOut);

					individual * target = getIndiscriminateIndividualInRange(player, main_field, getAttributeSum(player, "range"));

					// player potentially harms self only 25% of the time
					if(target == NULL || (target->isPlayer && isGreaterThanPercentage(rand() % 100, 100, 75))){
						decreaseTurns(player, thisGroupContainer, 1, *inActionMode);
						viewShift->xShift = viewShift->xShiftOld;
						viewShift->yShift = viewShift->yShiftOld;

						toggleInCursorMode();
						player->thisBehavior->gotConfused = 0;

						disableHUDAttackSpaces();
						break;
					}

					setCursorCoords(target->playerCharacter->x, target->playerCharacter->y);
				}

				if(tryAttackIndividual(thisGroupContainer, player, main_field, getCursorX(), getCursorY()) || player->thisBehavior->gotConfused){

					viewShift->xShift = viewShift->xShiftOld;
					viewShift->yShift = viewShift->yShiftOld;
					decreaseTurns(player, thisGroupContainer, 1, *inActionMode);
					decreaseFood(player, 1.0);

					player->thisBehavior->gotConfused = 0;

					*inActionMode = shouldEnableActionMode();
//					char outLog[12];
//					sprintf(outLog, "AM attacked: %d", *inActionMode);
//					cwrite(outLog);

					disableHUDAttackSpaces();

					toggleInCursorMode();
				}
			}else if(getCursorMode() == CURSOR_TALK){ //talk to the individual
				if(tryTalkIndividualFromField(player, main_field, getCursorX(), getCursorY())){
					viewShift->xShift = viewShift->xShiftOld;
					viewShift->yShift = viewShift->yShiftOld;
					decreaseTurns(player, thisGroupContainer, 1, *inActionMode);
					toggleInCursorMode();
				}

			}else if (getCursorMode() == CURSOR_ABILITY){
				if(!cursorWithinAbilityRange(player, getCursorX(), getCursorY())){
					break;
				}

				if(hasActiveStatusEffect(player, STATUS_CONFUSED) && isGreaterThanPercentage(rand() % 100, 100, 50)){
					player->thisBehavior->gotConfused = 1;

					char logOut[128];
					sprintf(logOut, "%s is confused!", player->name);
					cwrite(logOut);

					individual * target = getIndiscriminateIndividualInRange(player, main_field, getSelectedAbilityRange(player));

					// player successfully cast ability on self 75% of the time
					if(target == NULL || (target->isPlayer && isGreaterThanPercentage(rand() % 100, 100, 75))){
						player->activeAbilities->selectedAbility = NULL;
						decreaseTurns(player, thisGroupContainer, 1, *inActionMode);
						viewShift->xShift = viewShift->xShiftOld;
						viewShift->yShift = viewShift->yShiftOld;

						toggleInCursorMode();
						player->thisBehavior->gotConfused = 0;
						break;
					}

					setCursorCoords(target->playerCharacter->x, target->playerCharacter->y);
				}

				int numActions = 1;

				useAbilityOnIndividualsInAOERange(player, player, thisGroupContainer, main_field, getCursorX(), getCursorY());

				if(player->activeAbilities->selectedAbility->actionsEnabled){
					numActions += player->activeAbilities->selectedAbility->actions->effectAndManaArray[player->activeAbilities->selectedAbility->actions->selectedIndex]->effectMagnitude;
				}

				player->activeAbilities->selectedAbility = NULL;
				viewShift->xShift = viewShift->xShiftOld;
				viewShift->yShift = viewShift->yShiftOld;

				decreaseTurns(player, thisGroupContainer, numActions, *inActionMode);
				decreaseFood(player, 0.5);

				player->thisBehavior->gotConfused = 0;

				if(hasActiveStatusEffect(player, STATUS_BERZERK)){
					*playerControlMode = 1;
				}

				*inActionMode = shouldEnableActionMode();
//				char outLog[12];
//				sprintf(outLog, "AM ability: %d", *inActionMode);
//				cwrite(outLog);

				toggleInCursorMode();

			}else if(getCursorMode() == CURSOR_PICKPOCKET){
				if(tryPickPocketIndividualFromField(player, main_field, getCursorX(), getCursorY()) == 0){
					decreaseTurns(player, thisGroupContainer, 1, *inActionMode);
				}

				viewShift->xShift = viewShift->xShiftOld;
				viewShift->yShift = viewShift->yShiftOld;

				*inActionMode = shouldEnableActionMode();
//				char outLog[12];
//				sprintf(outLog, "AM pickpocket: %d", *inActionMode);
//				cwrite(outLog);

				toggleInCursorMode();
			}else if(CURSOR_LOOK){
				enableLookScrollMode();
			}

		}
			break;
		}
		break;
		}
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
	return 0;
}

int pausedLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
	case WM_KEYDOWN:{
		switch(LOWORD(wParam)){
		case 0x1B: //escape
		case 0x50://p key (inventory)
			{
				togglePaused();
			}
			break;
		}
		break;
	}
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

int specialDrawLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
	case WM_KEYDOWN:{
		switch(LOWORD(wParam)){
		case 0x50://p key (inventory)
			{
				togglePaused();
			}
			break;
		}
		break;
	}
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
			case 0x50://p key (inventory)
			{
				togglePaused();
				break;
			}
		}
		break;
		}
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
			case 0x50://p key (inventory)
			{
				togglePaused();
				break;
			}
		}
		break;
		}
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
			case 0x50://p key (inventory)
				{
					togglePaused();
				}
				break;
		}
		break;
		}
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

int dialogLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, individual * player, groupContainer * thisGroupContainer, field * thisField, int * inActionMode){
	switch(msg){
	case WM_KEYDOWN:{
		switch (LOWORD(wParam)) {
			case 0x1B: //esc
			case 0x0D: { //enter
				while(!tryGetDialogReadLock()){}
				while(!tryGetDialogWriteLock()){}

				if(disableSpeakModeIfEnabled()){
					releaseDialogWriteLock();
					releaseDialogReadLock();
					break;
				}

				int eventID = getEventFromCurrentMessage();

				if(eventID != 0){
					triggerEvent(eventID);
//					processEvent(eventID, player, thisGroupContainer, thisField);
				}

				if(!advanceDialog()){
					*inActionMode = shouldEnableActionMode();
//					char outLog[12];
//					sprintf(outLog, "speak: %d", *inActionMode);
//					cwrite(outLog);
				}

				releaseDialogWriteLock();
				releaseDialogReadLock();
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
			case 0x50://p key (inventory)
			{
				togglePaused();
			}
			break;
		}
		break;
		}
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

int lookViewScrollLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
	case WM_KEYDOWN: {
		switch(LOWORD(wParam)){
		case 0x38:
		case 0x68: {
			moveLookIndexUp();
		}
			break;

		case 0x32:
		case 0x62: {
			moveLookIndexDown();

		}
			break;

		case 0x1B: //escape
		{
			disableLookScrollMode();
		}
			break;
		case 0x50://p key (inventory)
		{
			togglePaused();
		}
		break;
	}
	break;
	}
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

int inventoryLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, field * main_field, individual * player, groupContainer * thisGroupContainer, shiftData * viewShift, int * inActionMode) {
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
		case 0x1B: //escape
			disableInventoryViewMode();

			if (inBuyMode()) {
				disableInventoryBuyMode();
			}

			if(inPickPocketMode()){
				disableInventoryPickPocketMode();
				decreaseTurns(player, thisGroupContainer, 1, *inActionMode);
			}

			if(inFieldGetMode()){
				disableItemFieldGetMode();
			}

			break;
		case 0x0D: //enter
			{
				item * tmpItem = getSelectedItem();

				if (tmpItem != NULL) {
					if (inBuyMode()) {
						attemptToBuyItem(tmpItem, player);
					} else if(inPickPocketMode()){
						int isLastItem = !selectedIndexIsntLastPlayerItem();

						int pickpocketSuccess = attemptToPickPocketItem(tmpItem, player);

						if(pickpocketSuccess && isLastItem){
							selectPreviousItemUp();
						}else if(!pickpocketSuccess){
							disableInventoryPickPocketMode();
							disableInventoryViewMode();

							decreaseTurns(player, thisGroupContainer, 1, *inActionMode);
						}
					} else if (inFieldGetMode()){
						addItemToInventory(player->backpack, tmpItem);
						removeItemFromInventory(main_field->currentSpaceInventory, tmpItem);
						removeItemFromField(main_field, tmpItem);
						triggerEventOnPickup(tmpItem->ID, player->isPlayer);

						if(tmpItem->owner != 0 && tmpItem->isStolen == 0){
							tmpItem->isStolen = 1;

							processCrimeEvent(CRIME_STEALING, tmpItem->price, tmpItem->owner, tmpItem->ID);
						}

						refreshInventory(main_field->currentSpaceInventory);
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
					while(!tryGetDialogReadLock()){}
					while(!tryGetDialogWriteLock()){}

					toggleDrawDialogBox();
					setSimpleDialogMessage(tmpItem->description);

					releaseDialogWriteLock();
					releaseDialogReadLock();
				}
			}
			break;
		case 0x58: //x key, drop
			{
				//only drop in inventory mode
				if(!inBuyMode() && !inPickPocketMode() && !inFieldGetMode()){
					item * tmpItem = getSelectedItem();

					if(tmpItem == NULL){
						break;
					}

					populateCurrentSpaceInventory(main_field, player->playerCharacter->x, player->playerCharacter->y);

					if(main_field->currentSpaceInventory->inventorySize >= 10){
						cwrite("Can't drop, too many items.");
						break;
					}

					if(tmpItem->isEquipt){
						modifyItem(tmpItem, player);
					}

					tmpItem->itemCharacter->x = player->playerCharacter->x;
					tmpItem->itemCharacter->y = player->playerCharacter->y;

					if(addItemToField(main_field, tmpItem)){
						mapInfo * thisMap = getMapInfoFromRegistry(main_field->id);
						addItemToMapInfo(thisMap, tmpItem->ID);
					}

					removeItemFromInventory(player->backpack, tmpItem);
					setDefaultAnimation(tmpItem->itemCharacter->thisAnimationContainer, ANIMATION_IDLE);

					refreshInventory(player->backpack);
				}
			}
			break;
		case 0x50://p key (inventory)
			{
				togglePaused();
			}
			break;
		}
		break;
		}
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
	return 0;
}

int moveLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, int * moveMode, field * thisField, individual * thisIndividual, groupContainer * thisGroupContainer, int * postMoveMode, shiftData * viewShift, int animateMoveSpeed) {
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
				RECT rect;
				GetClientRect(hwnd, &rect);

				int dx = xMoveChange(LOWORD(wParam) % 16);
				int dy = yMoveChange(LOWORD(wParam) % 16);
				moveNode ** currentNode = &(thisIndividual->thisMoveNodeMeta->rootMoveNode);

				while((*currentNode)->nextMoveNode != NULL){
//					printf("current x:%d y:%d\n",(*currentNode)->x,(*currentNode)->y);
					moveNode * tmpMoveNode = (moveNode *)(*currentNode)->nextMoveNode;
					currentNode = &tmpMoveNode;
				}

				space * tmpSpace = getSpaceFromField(thisField, (*currentNode)->x + dx, (*currentNode)->y + dy);
				if ( tmpSpace != NULL && tmpSpace->isPassable && (tmpSpace->currentIndividual == NULL || isAlly(thisIndividual, tmpSpace->currentIndividual))) {
					moveNode ** oldNode = (moveNode **)alreadyContainsNode(((moveNode *)thisIndividual->thisMoveNodeMeta->rootMoveNode),(*currentNode)->x + dx, (*currentNode)->y + dy );

					int screenX =  rect.right - rect.left - getSidebarWidth();
					int screenY =  rect.bottom - rect.top - getConsoleHeight();

					if(oldNode == NULL){
						if (thisIndividual->thisMoveNodeMeta->pathLength < getAttributeSum(thisIndividual, "mvmt")) {
							moveNode * nextNode = malloc(sizeof(moveNode));
							nextNode->x = (*currentNode)->x + dx;
							nextNode->y = (*currentNode)->y + dy;
							nextNode->nextMoveNode = NULL;
							nextNode->hasTraversed = 0;
							(*currentNode)->nextMoveNode = (moveNode *)nextNode;

							thisIndividual->thisMoveNodeMeta->pathLength++;

							tryUpdateXShift(viewShift, nextNode->x, screenX);
							tryUpdateYShift(viewShift, nextNode->y, screenY);
						}
					}else{ //node already exists
						moveNode * tmpNode = *oldNode;
						int removedNodes = freeUpMovePath(tmpNode->nextMoveNode);
						thisIndividual->thisMoveNodeMeta->pathLength = thisIndividual->thisMoveNodeMeta->pathLength - removedNodes;
						tryUpdateXShift(viewShift, tmpNode->x, screenX);
						tryUpdateYShift(viewShift, tmpNode->y, screenY);
						(*oldNode)->nextMoveNode = NULL;
					}
				}
			}

			break;
		case 0x1B: //escape
			*moveMode = 0;
			viewShift->xShift = viewShift->xShiftOld;
			viewShift->yShift = viewShift->yShiftOld;
			break;
		case 0x0D: //enter
		{
			moveNode * tmpMoveNode = thisIndividual->thisMoveNodeMeta->rootMoveNode;
			while(tmpMoveNode->nextMoveNode != NULL){
				tmpMoveNode = tmpMoveNode->nextMoveNode;
			}

			space * tmpSpace = getSpaceFromField(thisField, tmpMoveNode->x, tmpMoveNode->y);

			if ( tmpSpace != NULL && tmpSpace->isPassable && tmpSpace->currentIndividual == NULL) {
				*moveMode = 0;
				if(thisIndividual->thisMoveNodeMeta->rootMoveNode->nextMoveNode != NULL){
					*postMoveMode = 1;
					getSpaceFromField(thisField, thisIndividual->playerCharacter->x, thisIndividual->playerCharacter->y)->currentIndividual = NULL;
					viewShift->xShift = viewShift->xShiftOld;
					viewShift->yShift = viewShift->yShiftOld;
				}
			}
		}
			break;
		case 0x50://p key (inventory)
			{
				togglePaused();
			}
			break;
		}
		break;
		}
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

void processPlayerControlledLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam,
		individual * player, groupContainer * thisGroupContainer, field * thisField, int * inActionMode, int * postMoveMode, int * playerControlMode, int * postPlayerControlMode){
	switch (msg) {
		case WM_KEYDOWN:{
			switch(LOWORD(wParam)){
			case 0x50://p key (inventory)
				{
					togglePaused();
				}
				break;
			}
			break;
		}
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}

	if(controlledPlayerAction(player, thisGroupContainer, thisField, *inActionMode)){
		*postMoveMode = 1;
	}

	*playerControlMode = 0;
	*postPlayerControlMode = 1;
}

void processActionLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam,
		individual * player, groupContainer * thisGroupContainer, field * thisField, int * inActionMode, int * playerControlMode, int animateMoveSpeed){
	switch (msg) {
		case WM_KEYDOWN:{
			switch(LOWORD(wParam)){
			case 0x50://p key (inventory)
				{
					togglePaused();
				}
				break;
			}
			break;
		}
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}

	*inActionMode = shouldEnableActionMode();

	if(thisGroupContainer->selectedGroup->numIndividuals == 0){
		thisGroupContainer->groupActionMode = 0;
		thisGroupContainer->initGroupActionMode = 0;

		if(!setNextActiveGroup(thisGroupContainer)){
			if(startTurn(player)){

			}

			//If player's out of actions, start enemy turn again
			if(player->remainingActions < 0){
				endTurn(player);

				if(!(*inActionMode)){
					player->remainingActions = player->totalActions;
				}else{
					thisGroupContainer->groupActionMode = 1;
					thisGroupContainer->initGroupActionMode = 1;
					setNextActiveGroup(thisGroupContainer);
				}

			}else if (hasActiveStatusEffect(player, STATUS_BERZERK) || hasActiveStatusEffect(player, STATUS_SLEEPING)){
				*playerControlMode = 1;
			}
		}else{
			thisGroupContainer->groupActionMode = 1;
			thisGroupContainer->initGroupActionMode = 1;
		}
	} else {
		if (thisGroupContainer->initGroupActionMode) {
			thisGroupContainer->initGroupActionMode = 0;

			if(initializeEnemyTurn(thisGroupContainer->selectedGroup, player, thisField)){
				thisGroupContainer->groupActionMode = 0;
				thisGroupContainer->postGroupActionMode = 1;
				return;
			}

			if(!thisGroupContainer->selectedGroup->individuals[thisGroupContainer->selectedGroup->currentIndividualIndex]->remainingActions > 0){
				thisGroupContainer->groupActionMode = 0;
				thisGroupContainer->postGroupActionMode = 1;
				return;
			}
		}

		thisGroupContainer->groupActionMode = 0;

		individual * tmpIndividual = thisGroupContainer->selectedGroup->individuals[thisGroupContainer->selectedGroup->currentIndividualIndex];

		//If not moving
		if(!performAction(tmpIndividual, player, thisGroupContainer, thisField, *inActionMode)){
			thisGroupContainer->postGroupActionMode = 1;
		}else{
			if(*inActionMode){
				thisGroupContainer->groupMoveMode = 1;
			}else{
				addIndividualToGroup(thisGroupContainer->movingIndividuals, tmpIndividual);

				if(tmpIndividual->remainingActions > 0){
					tmpIndividual->remainingActions = 0;
				}

				thisGroupContainer->postGroupActionMode = 1;

				// Initialize the x/y offsets
				int xChange = tmpIndividual->thisMoveNodeMeta->dummyCord->x - tmpIndividual->playerCharacter->x;
				int yChange = tmpIndividual->thisMoveNodeMeta->dummyCord->y - tmpIndividual->playerCharacter->y;

				tmpIndividual->playerCharacter->xOff = (((animateMoveSpeed*1.0) - (tmpIndividual->thisMoveNodeMeta->sum*1.0)) / (animateMoveSpeed*1.0)) * xChange;
				tmpIndividual->playerCharacter->yOff = (((animateMoveSpeed*1.0) - (tmpIndividual->thisMoveNodeMeta->sum*1.0)) / (animateMoveSpeed*1.0)) * yChange;
			}
		}
	}
}
