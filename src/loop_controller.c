/*
 * loop_controller.c
 *
 *  Created on: Nov 3, 2015
 *      Author: Adrian
 */
#include "./headers/field_controller_pub_methods.h"
#include "./headers/cursor_pub_methods.h"

int cursorLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, int * cursorMode, int * postCursorMode, cursor * thisCursor, field * main_field, individual * player, enemies  * thisEnemies, shiftData * viewShift) {
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
		case 0x0D: //enter
		{
			int cX, cY, index;
			cX = thisCursor->cursorCharacter->x;
			cY = thisCursor->cursorCharacter->y;

			individual ** tmp = (individual**) getIndividualAddressFromField(main_field, cX, cY);

			for (index = 0; index < thisEnemies->numEnemies; index++) {

				individual * tmpEnemy = thisEnemies->enemies[index];

				if (*tmp == tmpEnemy && individualWithinRange(player, tmpEnemy)) {
					printf("attacked!");
					if(attackIndividual(player, tmpEnemy)){
						deleteEnemyFromEnemies(thisEnemies,tmpEnemy);
						removeIndividualFromField(main_field, tmpEnemy->playerCharacter->x, tmpEnemy->playerCharacter->y);
						destroyIndividual(tmpEnemy);
					}
					*cursorMode = 0;
					*postCursorMode = 1;

					viewShift->xShift = viewShift->xShiftOld;
					viewShift->yShift = viewShift->yShiftOld;
					break;
				}

			}

//					destroyIndividual(tmp);
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
//		DeleteObject(g_hbmPlayerMask);
		destroyIndividual(player);
		destroyCursor(thisCursor);
		PostQuitMessage(0);
		break;
		default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	}
	return 0;
}

int inventoryLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, int * inventoryMode, field * main_field, individual * player, enemies  * thisEnemies, shiftData * viewShift) {
	int toReturn = 0;
	switch (msg) {
	case WM_KEYDOWN: {
		switch (LOWORD(wParam)) {
			case 0x38:
			case 0x68:{
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
				*inventoryMode = 0;
				break;
			case 0x0D: //enter
			{
				item * tmpItem = getSelectedItem();

				if(tmpItem != NULL){
					modifyItem(tmpItem, player);
					refreshInventory(player->backpack);
//					*inventoryMode = 0;
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
			destroyIndividual(player);
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

				free(currentNode);
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

		destroyIndividual(thisIndividual);
		PostQuitMessage(0);
		break;
		default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	}
	return 0;
}

void animateMoveLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, field * thisField,
		individual * thisIndividual, moveNodeMeta * thisMoveNodeMeta, int speed, int * postMoveMode,
		shiftData * viewShift){
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

			setIndividualSpace(thisField,thisIndividual,(*tmpMoveNode)->x, (*tmpMoveNode)->y);

			tryUpdateXShift(viewShift, (*tmpMoveNode)->x);
			tryUpdateYShift(viewShift, (*tmpMoveNode)->y);

			if((*tmpMoveNode)->nextMoveNode == NULL){
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
