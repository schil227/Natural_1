/*
 * loop_controller.c
 *
 *  Created on: Nov 3, 2015
 *      Author: Adrian
 */
#include "./headers/field_controller_pub_methods.h"
#include "./headers/cursor_pub_methods.h"
#include <stdio.h>

int cursorLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam,
		field * main_field, individual * player, groupContainer * thisGroupContainer, shiftData * viewShift, int * inActionMode, int * playerControlMode, int animateMoveSpeed) {
	switch (msg) {
	case WM_KEYDOWN: {
		switch (LOWORD(wParam)) {
		case 0x50://p key
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

			if(canMoveCursor(player, direction)){
				moveCursor(main_field,direction, viewShift, &rect);

				if(getCursorMode() == CURSOR_LOOK){
					populateLookDataInstance(main_field, player, getCursorX(), getCursorY());
				}else if(getCursorMode() == CURSOR_ABILITY_AOE_LINE){
					ability * tmpAbility = player->activeAbilities->selectedAbility;

					cord tmpCord;
					tmpCord.x = getCursorX();
					tmpCord.y = getCursorY();

					int lineLength = tmpAbility->aoeLine->effectAndManaArray[tmpAbility->aoeLine->selectedIndex]->effectMagnitude;

					cordArr * newAOECords = getCordsBetweenPoints(getCursorTmpX(), getCursorTmpY(), &tmpCord, lineLength, 0, 0.95, main_field);
					updateCursorAOESpaces(newAOECords);
				}
			}
			break;
		}
		case 0x1B: //escape
			if (getCursorMode() == CURSOR_ABILITY){
				player->activeAbilities->selectedAbility = NULL;
			}else if(getCursorMode() == CURSOR_LOOK){
				disableLookMode();
			}else if(getCursorMode() == CURSOR_ABILITY_AOE_LINE){
				clearCursorAOESpaces();
				refreshCursor(CURSOR_ABILITY, getCursorTmpX(), getCursorTmpY());
				break;
			}

			toggleInCursorMode();
			disableHUDAttackSpaces();

			viewShift->xShift = viewShift->xShiftOld;
			viewShift->yShift = viewShift->yShiftOld;
			break;
		case 0x0D: //enter
		{
			if (getCursorMode() == CURSOR_ATTACK) {

				if(!cordWithinRange(player, getCursorX(), getCursorY())){ // getIndividualFromField(main_field, getCursorX(), getCursorY()) == NULL
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
			}else if (getCursorMode() == CURSOR_ABILITY_AOE_LINE){
				ability * tmpAbility = player->activeAbilities->selectedAbility;

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

					break;
					//TODO: set tmpCords to target, randomize cursorCords to be within range
//					setCursorCoords(target->playerCharacter->x, target->playerCharacter->y);
				}

				int numActions = 1;

				clearCursorAOESpaces();

				useAbilityOnIndividualsInAOERange(player, player, thisGroupContainer, main_field, getCursorTmpX(), getCursorTmpY(), getCursorX(), getCursorY());

				if(tmpAbility->actionsEnabled){
					numActions += tmpAbility->actions->effectAndManaArray[tmpAbility->actions->selectedIndex]->effectMagnitude;
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

				toggleInCursorMode();

			}else if (getCursorMode() == CURSOR_ABILITY){
				if(!cursorWithinAbilityRange(player, getCursorX(), getCursorY())){
					break;
				}

				ability * tmpAbility = player->activeAbilities->selectedAbility;

				//If the selected ability has a line AOE, get second cord
				if(tmpAbility->aoeLineEnabled && tmpAbility->aoeLine->effectAndManaArray[tmpAbility->aoeLine->selectedIndex]->effectMagnitude > 0){
					setCursorTmpCord(getCursorX(), getCursorY());
					refreshCursor(CURSOR_ABILITY_AOE_LINE, getCursorX(), getCursorY());

					cord tmpCord;
					tmpCord.x = getCursorX();
					tmpCord.y = getCursorY();

					int lineLength = tmpAbility->aoeLine->effectAndManaArray[tmpAbility->aoeLine->selectedIndex]->effectMagnitude;

					cordArr * newAOECords = getCordsBetweenPoints(getCursorTmpX(), getCursorTmpY(), &tmpCord, lineLength, 0, 0.95, main_field);
					updateCursorAOESpaces(newAOECords);
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

				useAbilityOnIndividualsInAOERange(player, player, thisGroupContainer, main_field, getCursorX(), getCursorY(), getCursorX(), getCursorY());

				if(tmpAbility->actionsEnabled){
					numActions += tmpAbility->actions->effectAndManaArray[tmpAbility->actions->selectedIndex]->effectMagnitude;
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

				toggleInCursorMode();

			}else if(getCursorMode() == CURSOR_PICKPOCKET){
				if(tryPickPocketIndividualFromField(player, main_field, getCursorX(), getCursorY()) == 0){
					decreaseTurns(player, thisGroupContainer, 1, *inActionMode);
				}

				viewShift->xShift = viewShift->xShiftOld;
				viewShift->yShift = viewShift->yShiftOld;

				*inActionMode = shouldEnableActionMode();

				toggleInCursorMode();
			}else if(getCursorMode() == CURSOR_LOOK){
				enableLookScrollMode();
			}else if(getCursorMode() == CURSOR_INTERACT){
				space * tmpSpace = getSpaceFromField(main_field, getCursorX(), getCursorY());

				if(tmpSpace != NULL && tmpSpace->interactableObject != NULL){
					interactable * thisObject =  tmpSpace->interactableObject;
					player->currentInteractableObject = thisObject;

					if(thisObject->isEnabled){
						if(thisObject->inFinalMode && thisObject->finalEvent != -1){
							triggerEvent(thisObject->finalEvent);
						}else if(thisObject->triggerDialogID != -1){
							if(setCurrentMessageByMessageID(thisObject->triggerDialogID)){
								toggleDrawDialogBox();
							}
						}else if(thisObject->triggerEventID != -1){
							triggerEvent(thisObject->triggerEventID);
						}

						decreaseTurns(player, thisGroupContainer, 1, *inActionMode);
						toggleInCursorMode();
					}
				}
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
		case 0x50://p key
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

int windowTransitionLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
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

int specialDrawLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
	case WM_KEYDOWN:{
		switch(LOWORD(wParam)){
		case 0x50://p key
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
			case 0x50://p key
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
				interpretCreateAbilityEscape();
				break;
			}
			case 0x0D: { //enter
				interpretCreateAbilityEnter();
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
			case 0x41://a key
			{
				addBonusMana();
				break;
			}
			case 0x44://d key
			{
				removeBonusMana();
				break;
			}
			case 0x50://p key (pause)
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

int abilityViewLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, individual * player, shiftData * viewShift, field * thisField){
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
					tryEnableOptionalActionMode();

					if(useAbility(player,getAbilityToActivate())){
						removeIndividualFromField(thisField, player->playerCharacter->x, player->playerCharacter->y);
						triggerEventOnDeath(player->ID, player->isPlayer);
						removeIndividualFromExistance(player->ID);
						return 0;
					}

					if(player->activeAbilities->selectedAbility != NULL && (player->activeAbilities->selectedAbility->type == 't' || player->activeAbilities->selectedAbility->type == 'd')){
						toggleInCursorMode();
						viewShift->xShiftOld = viewShift->xShift;
						viewShift->yShiftOld = viewShift->yShift;
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
			case 0x50://p key
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
			case 0x50://p key
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

int characterInfoViewLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
	case WM_KEYDOWN: {
		switch(LOWORD(wParam)){
		case 0x34:
		case 0x64: {
			showInfoView();
		}
			break;

		case 0x36:
		case 0x66: {
			showEffectsView();

		}
			break;
		case 0x38:
		case 0x68: {
			handleUpOnCharacterInfoView();
		}
			break;

		case 0x32:
		case 0x62: {
			handleDownOnCharacterInfoView();
		}
			break;
		case 0x0D: //enter
			handleEnterOnCharacterInfoView();
			break;
		case 0x1B: //escape
		{
			disableCharacterInfoView();
			showInfoView();
		}
			break;
		case 0x50://p key
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

int worldMapLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, field ** thisField, individual * player, groupContainer * thisGroupContainer, shiftData * viewShift, char * mapDirectory, int * inActionMode){
	switch(msg){
	case WM_KEYDOWN: {
		switch(LOWORD(wParam)){
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
		case 0x69: {
			tryMoveWorldMap(LOWORD(wParam) % 16);
		}
			break;

		case 0x0D: //enter
		{
			if(transitFromAreaNode(getCurrentAreaNode(), thisField, player, thisGroupContainer, viewShift, mapDirectory, 1)){
				disableWorldMapMode();

				RECT rect;
				GetClientRect(hwnd, &rect);
				HDC hdc = GetDC(hwnd);
				HDC hdcBuffer = CreateCompatibleDC(hdc);

				printf("WANT: transit\n");fflush(stdout);
				while(!tryGetFieldWriteLock()){}
				printf("GOT: transit\n");fflush(stdout);
				updateFieldGraphics(hdc, hdcBuffer, *thisField);
				transitViewShift(viewShift, player, *thisField, &rect);
				inActionMode = shouldEnableActionMode();

				releaseFieldWriteLock();

				printf("RELEASED: transit\n");fflush(stdout);
			}
		}
			break;
		case 0x50://p key
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
		case 0x50://p key
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

			if(inObjectGetMode()){
				disableObjectGetMode();
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
						tmpItem->npcID = player->ID;

						triggerEventOnPickup(tmpItem->ID, player->isPlayer);

						if(tmpItem->owner != 0 && tmpItem->isStolen == 0){
							tmpItem->isStolen = 1;

							processCrimeEvent(CRIME_STEALING, tmpItem->price, tmpItem->owner, tmpItem->ID);
						}

						refreshInventory(main_field->currentSpaceInventory);
					} else if(inObjectGetMode()){
						addItemToInventory(player->backpack, tmpItem);
						removeItemFromInventory(player->currentInteractableObject->objectInventory, tmpItem);
						tmpItem->npcID = player->ID;

						triggerEventOnPickup(tmpItem->ID, player->isPlayer);

						if(tmpItem->owner != 0 && tmpItem->isStolen == 0){
							tmpItem->isStolen = 1;

							processCrimeEvent(CRIME_STEALING, tmpItem->price, tmpItem->owner, tmpItem->ID);
						}

						refreshInventory(player->currentInteractableObject->objectInventory);
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
		case 0x50://p key
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
				if ( tmpSpace != NULL && tmpSpace->isPassable && !tmpSpace->spaceIsReserved && canPassThroughInteractableObject(tmpSpace->interactableObject) && (tmpSpace->currentIndividual == NULL || isAlly(thisIndividual, tmpSpace->currentIndividual))) {
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

			if ( tmpSpace != NULL && tmpSpace->isPassable && canPassThroughInteractableObject(tmpSpace->interactableObject) && tmpSpace->currentIndividual == NULL) {
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
		case 0x50://p key
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

int processPlayerControlledLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam,
		individual * player, groupContainer * thisGroupContainer, field * thisField, int * inActionMode, int * postMoveMode, int * playerControlMode, int * postPlayerControlMode){
	int toReturn = 0;
	switch (msg) {
		case WM_KEYDOWN:{
			switch(LOWORD(wParam)){
			case 0x50://p key
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
			toReturn = DefWindowProc(hwnd, msg, wParam, lParam);
	}

	if(controlledPlayerAction(player, thisGroupContainer, thisField, *inActionMode)){
		*postMoveMode = 1;
	}

	*playerControlMode = 0;
	*postPlayerControlMode = 1;

	return toReturn;
}

int processActionLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam,
		individual * player, groupContainer * thisGroupContainer, field * thisField, int * inActionMode, int * playerControlMode, int animateMoveSpeed){
	int toReturn = 0;
	switch (msg) {
		case WM_KEYDOWN:{
			switch(LOWORD(wParam)){
			case 0x50://p key
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
			toReturn = DefWindowProc(hwnd, msg, wParam, lParam);
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
				return toReturn;
			}

			if(!thisGroupContainer->selectedGroup->individuals[thisGroupContainer->selectedGroup->currentIndividualIndex]->remainingActions > 0){
				thisGroupContainer->groupActionMode = 0;
				thisGroupContainer->postGroupActionMode = 1;
				return toReturn;
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

	return toReturn;
}

int gameMenuLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch (msg) {
		case WM_KEYDOWN:{
			switch(LOWORD(wParam)){
			case 0x0D://enter
				gameMenuInterpretEnter();
				break;
			case 0x1B:
				gameMenuInterpretEscape();
				break;
			case 0x38:
			case 0x68: //'8'
				gameMenuInterpretUp();
				break;
			case 0x32:
			case 0x62: //'2' key
				gameMenuInterpretDown();
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

int mainMenuLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch (msg) {
		case WM_KEYDOWN:{
			switch(LOWORD(wParam)){
			case 0x0D://enter
				mainMenuInterpretEnter();
				break;
			case 0x1B:
				mainMenuInterpretEscape();
				break;
			case 0x38:
			case 0x68: //'8'
				mainMenuInterpretUp();
				break;
			case 0x32:
			case 0x62: //'2' key
				mainMenuInterpretDown();
				break;
			case 0x34:
			case 0x64: //'4'
				mainMenuInterpretLeft();
				break;
			case 0x36:
			case 0x66: //'6' key
				mainMenuInterpretRight();
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

int levelUpLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch (msg) {
		case WM_KEYDOWN:{
			switch(LOWORD(wParam)){
			case 0x0D://enter
				interpretLevelUpViewEnter();
				break;
			case 0x1B:
				interpretLevelUpViewEscape();
				break;
			case 0x38:
			case 0x68: //'8'
				interpretLevelUpViewVertical(1);
				break;
			case 0x32:
			case 0x62: //'2' key
				interpretLevelUpViewVertical(0);
				break;
			case 0x34:
			case 0x64: //'4'
				interpretLevelUpViewHorizontal(1);
				break;
			case 0x36:
			case 0x66: //'6' key
				interpretLevelUpViewHorizontal(0);
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
