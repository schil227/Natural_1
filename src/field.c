/*
 * field.c
 *
 *  Created on: Mar 29, 2015
 *      Author: Adrian
 */

#include"./headers/field_pub_methods.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>


individual* getIndividualFromField(field* thisField, int x, int y){
	if(x < thisField->totalX && x >= 0 && y < thisField->totalY && y >= 0){

		return thisField->grid[x][y]->currentIndividual;
	}

	return NULL;
}

individual* getIndividualAddressFromField(field* thisField, int x, int y){
	if(x < thisField->totalX && x >= 0 && y < thisField->totalY && y >= 0){

		return &(thisField->grid[x][y]->currentIndividual);
	}

	return NULL;
}

character* getBackgroundFromField(field* thisField, int x, int y){
	if(x < thisField->totalX && x >= 0 && y < thisField->totalY && y >= 0){
		return (thisField->grid[x][y])->background;
	}

	return NULL;
}

space* getSpaceFromField(field* thisField, int x, int y){
	if(x < thisField->totalX && x >= 0 && y < thisField->totalY && y >= 0){
		return thisField->grid[x][y];
	}

	return NULL;
}

int spaceIsAvailable(field* thisField, int x, int y){
	if(x < thisField->totalX && x >= 0 && y < thisField->totalY && y >= 0){
		if(thisField->grid[x][y]->currentIndividual != NULL){
			return 1;
		}
	}

	return 0;
}

int isSpacePassable(field* thisField, int x, int y){
	if(x < thisField->totalX && x >= 0 && y < thisField->totalY && y >= 0){
			if(thisField->grid[x][y]->isPassable){
				return 1;
			}
	}

	return 0;
}

space* getSpaceAddressFromField(field* thisField, int x, int y){
	printf("trying to get space \n");
	if(x < thisField->totalX && x >= 0 && y < thisField->totalY && y >= 0){
		return &(thisField->grid[x][y]);
	}
	printf("was a null space! \n");
	return NULL;
}


int removeIndividualFromField(field * thisField, int x, int y){
	if((thisField->grid[x][y])->currentIndividual != NULL){
		(thisField->grid[x][y])->currentIndividual = NULL;
		return 1;
	}

	return 0;
}

/*
 * getAdjacentSpaces: returns (up to) 9 spaces surrounding a space as
 * indicated by the x and y values
 */
space** getAdjacentSpaces(field *thisField, int x, int y){
	space** adjacentSpaces = malloc(sizeof(space*)*9);
	space* tmpSpace = malloc(sizeof(tmpSpace));
	int dx,dy,index=0;
	for(dx = -1; dx < 2; dx++){
		for(dy = -1; dy < 2; dy++){
			tmpSpace = getSpaceFromField(thisField,x+dx, y+dy);
			if(tmpSpace != NULL){
				adjacentSpaces[index] = tmpSpace;
				index++;
			}
		}
	}
	adjacentSpaces[index] = '\0';
	return adjacentSpaces;
}

/*
 * xMoveChange and yMoveChange
 * calculates the grid move for x and y depending on the keystroke.
 * for example, to move left the player would hit 4 (dir), decreasing the x, y would stay the same
 */
int xMoveChange(int dir){
	switch(dir){
	case 1:
	case 4:
	case 7:
		return -1;
	case 3:
	case 6:
	case 9:
		return 1;
	}

	//if 2,5,8, no x change
	return 0;
}

int yMoveChange(int dir){
	switch(dir){
	case 7:
	case 8:
	case 9:
		return -1;
	case 1:
	case 2:
	case 3:
		return 1;
	}

	//if 4,5,6 no y change
	return 0;
}

int moveIndividual(field *thisField, individual *thisIndividual, int direction){
	int inX = thisIndividual->playerCharacter->x;
	int inY = thisIndividual->playerCharacter->y;
	//space * currentSpace = getSpaceFromField(thisField,inX,inY);
//	space * newSpace;
	int newX = thisIndividual->playerCharacter->x + xMoveChange(direction);
	int newY = thisIndividual->playerCharacter->y + yMoveChange(direction);

	//check if in bounds, and newSpace exists
	if(!(newX >= 0 && newX < thisField->totalX && newY >=0 && newY < thisField->totalY)){
		return 0;
	}

	//space exists, wont be null
//	newSpace = getSpaceFromField(thisField,newX,newY);

	//can the individual go to this space?
	if(getSpaceFromField(thisField,newX,newY)->isPassable && getSpaceFromField(thisField,newX,newY)->currentIndividual == NULL){
		getSpaceFromField(thisField,inX,inY)->currentIndividual = NULL;
		getSpaceFromField(thisField,newX,newY)->currentIndividual = thisIndividual;
		thisIndividual->playerCharacter->x = newX;
		thisIndividual->playerCharacter->y = newY;
		return 1;
	}else{
//		printf("is not passable");
		return 0;
	}
}

int setIndividualSpace(field *thisField, individual *thisIndividual, int x, int y){

	//check if in bounds
	if(!(x >= 0 && x < thisField->totalX && y >=0 && y < thisField->totalY)){
			return 0;
	}

	if(getSpaceFromField(thisField,x,y)->currentIndividual == NULL){
		getSpaceFromField(thisField, thisIndividual->playerCharacter->x, thisIndividual->playerCharacter->y)->currentIndividual = NULL;
		getSpaceFromField(thisField, x, y)->currentIndividual = thisIndividual;
		thisIndividual->playerCharacter->x = x;
		thisIndividual->playerCharacter->y = y;
		return 1;
	}

	//space occoupied
	return 0;

}

/*
 * FromJump: the players old space may not exist anymore - removing logic to remove currentIndividual from old space
 */
int setIndividualSpaceFromJump(field *thisField, individual *thisIndividual, int x, int y){

	//check if in bounds
	if(!(x >= 0 && x < thisField->totalX && y >=0 && y < thisField->totalY)){
			return 0;
	}

	if(getSpaceFromField(thisField,x,y)->currentIndividual == NULL){
//		getSpaceFromField(thisField, thisIndividual->playerCharacter->x, thisIndividual->playerCharacter->y)->currentIndividual = NULL;
		getSpaceFromField(thisField, x, y)->currentIndividual = thisIndividual;
		thisIndividual->playerCharacter->x = x;
		thisIndividual->playerCharacter->y = y;
		return 1;
	}

	//space occoupied
	return 0;

}


//int setIndividualTmpSpace(field *thisField, individual *thisIndividual, int x, int y){
//
//	//check if in bounds
//	if(!(x >= 0 && x < thisField->totalX && y >=0 && y < thisField->totalY)){
//			return 0;
//	}
//
//	if(getSpaceFromField(thisField,x,y)->tmpIndividual == NULL){
//		space * oldSpace = getSpaceFromField(thisField, thisIndividual->playerCharacter->x, thisIndividual->playerCharacter->y);
//		space * newSpace = getSpaceFromField(thisField, x, y);
//		newSpace->tmpIndividual = malloc(sizeof(individual*));
//		oldSpace->tmpIndividual = NULL;
//		free(oldSpace->tmpIndividual);
//
//		newSpace->tmpIndividual = thisIndividual;
//		thisIndividual->playerCharacter->x = x;
//		thisIndividual->playerCharacter->y = y;
//		return 1;
//	}
//
//	//space occoupied
//	return 0;
//
//}

int moveCursor(field *thisField, cursor *thisCursor, int direction, shiftData * viewShift){
	int newX = thisCursor->cursorCharacter->x + xMoveChange(direction);
	int newY = thisCursor->cursorCharacter->y + yMoveChange(direction);
	printf("newX:%d, newY:%d\n",newX, newY);
	//check if in bounds, and newSpace exists
	printf("y:%d \n", viewShift->yShift);
	if(newX >= 0 && newX < thisField->totalX && newY >=0 && newY < thisField->totalY){
		thisCursor->cursorCharacter->x = newX;
		thisCursor->cursorCharacter->y = newY;
		tryUpdateXShift(viewShift, newX);
		tryUpdateYShift(viewShift, newY);
		return 1;
	}else{
		return 0;
	}

}


void wanderAround(field * thisField, individual * thisIndividual){
 int direction = rand() % 10+1;
 moveIndividual(thisField, thisIndividual, direction);
}
/*
 * take in a character, output the corrosponding background name (string)
*/
int generateBackground(char backgroundSymbol){
//	printf("backgroundChar: %c\n", backgroundSymbol);

	if(backgroundSymbol == 'c'){
//		printf("Found  a C!\n");
		return 2502;
	}else if (backgroundSymbol == 'g'){
		return 2501;
	}else if (backgroundSymbol == '-'){
		return 2511;
	}else if (backgroundSymbol == '='){
		return 2512;
	}else if (backgroundSymbol == 'r'){
		return 2513;
	}else if (backgroundSymbol == 'u'){
		return 2514;
	}else if (backgroundSymbol == 'o'){
		return 2515;
	}else if (backgroundSymbol == 'w'){
		return 2521;
	}else if (backgroundSymbol == 't'){
		return 2561;
	}else if (backgroundSymbol == 'p'){
		return 2541;
	}else if (backgroundSymbol == '\\'){
		return 2542;
	}else if (backgroundSymbol == 'f'){
		return 2551;
	}else if (backgroundSymbol == 'x'){
		return 2531;
	}else if (backgroundSymbol == 's'){
		return 2532;
	}else if (backgroundSymbol == 'd'){
		return 2533;
	}else{
		return 2501;
	}
}

field * loadMap(char * mapName, char* directory, individual * player, enemies* thisEnemies){

	char transitMap[80];
	char enemyMap[80];
	char * fullMapName = appendStrings(directory, mapName);
//	fullMapName[strlen(fullMapName)-1] = '\0'; //remove /n at end

	FILE * fp = fopen(fullMapName, "r");

	//transit map filename
	fgets(transitMap,80,fp);
	printf("transit map name: %s\n", transitMap);

	//enemy filename
	fgets(enemyMap,80,fp);
	printf("transit map name: %s\n", transitMap);
	printf("enemy map name:%s\n", enemyMap);

	fclose(fp);

	clearEnemies(thisEnemies);
	loadEnemies(thisEnemies, enemyMap, directory);

	field* thisField = initField(fullMapName);

	makeTransitSpaces(transitMap, directory, thisField, player);

	setIndividualSpace(thisField,player, player->playerCharacter->x, player->playerCharacter->y);
	setEnemiesToField(thisField, thisEnemies);

	return thisField;
}

void makeTransitSpaces(char * transitMap, char* directory, field * thisField, individual * player){
	char * fullTransitFile = appendStrings(directory, transitMap);
	fullTransitFile[strlen(fullTransitFile)-1] = '\0'; //remove '\n' at end of line
	FILE * enemyFP = fopen(fullTransitFile, "r");
	char line[80];

	printf("%s\n",fullTransitFile);

	while(fgets(line,80,enemyFP) != NULL){
		space * tmpSpace;
		int id, x, y, targetID;
		char * targetTransitMap;

		char * transitInstance = strtok(line,",");
		id = atoi(transitInstance);

		transitInstance = strtok(NULL, ",");
		x = atoi(transitInstance);

		transitInstance = strtok(NULL, ",");
		y = atoi(transitInstance);

		transitInstance = strtok(NULL, ",");
		targetTransitMap = transitInstance;

		transitInstance = strtok(NULL, ",");
		targetID = atoi(transitInstance);

		tmpSpace = getSpaceFromField(thisField,x,y);

		thisField->grid[x][y]->thisTransitInfo = malloc(sizeof(transitInfo));
		thisField->grid[x][y]->thisTransitInfo->transitID = id;
		strcpy(thisField->grid[x][y]->thisTransitInfo->transitMap,targetTransitMap);
		thisField->grid[x][y]->thisTransitInfo->targetMapTransitID = targetID;

		printf("defined transit space [%d,%d]: id:%d, map:%s, targetID:%d\n",x,y, id, targetTransitMap, targetID);

		//spawn player at this location
		if(player->jumpTarget == id){
			printf("Jumping player:[%d,%d]\n", x, y);
			setIndividualSpaceFromJump(thisField,player,x,y);
			player->jumpTarget = 0;
		}
	}

	fclose(enemyFP);

}

field* initField(char* fieldFileName){
	field* thisField = malloc(sizeof(field));
	FILE * fp = fopen(fieldFileName, "r");
	char line[80];
	int init_y = 0;
	int init_x = 0;
	int xIndex;

	//used to get rid of the first 2 lines of data (see loadMap)
	fgets(line,80,fp);
	fgets(line,80,fp);

	while(fgets(line,80,fp) != NULL){
		init_x = 0;
		for(xIndex = 0; xIndex < strlen(line); xIndex+=2){

			space* newSpace = malloc(sizeof(space));
			newSpace->currentIndividual = malloc(sizeof(individual));
			newSpace->thisTransitInfo = malloc(sizeof(transitInfo));
			character* backgroundCharacter = malloc(sizeof(character));
			char currentChar = line[xIndex];

			//initialize background
			backgroundCharacter->nameLength = 0;
			backgroundCharacter->width = 40;
			backgroundCharacter->height = 40;
			backgroundCharacter->x = init_x;
			backgroundCharacter->y = init_y;
			backgroundCharacter->imageID = generateBackground(currentChar);
			if(currentChar == 'c'
				|| currentChar == '-'
				|| currentChar == '='
				|| currentChar == 'r'
				|| currentChar == 'u'
				|| currentChar == 'o'
				|| currentChar == 's'
				|| currentChar == 'x'
				|| currentChar == 'w'
				|| currentChar == 't'){
				newSpace->isPassable = 0;
			}else{
				newSpace->isPassable = 1;
			}

			currentChar = line[xIndex+1];
			if (currentChar == '>') {
				backgroundCharacter->direction = 3;
			}else if (currentChar == 'v'){
				backgroundCharacter->direction = 2;
			}else if (currentChar == '<'){
				backgroundCharacter->direction = 1;
			}else{
				backgroundCharacter->direction = 0;
			}

			newSpace->background = backgroundCharacter;
			newSpace->currentIndividual = NULL;
			newSpace->thisTransitInfo = NULL;
//			newSpace->targetMapTransitID = 0;
//			newSpace->transitID = 0;
//			newSpace->transitMap = "";
			thisField->grid[init_x][init_y] = newSpace;
			init_x++;
		}

		init_y++;

//		puts(line);
//		printf("len:%d\n", strlen(line));
	}

	thisField->totalX = init_x;
	thisField->totalY = init_y;

	fclose(fp);

	return thisField;
}


void updateFiled(field* thisField, char* fieldFileName){
	int x=0,y=0;
	puts(fieldFileName);
	FILE * fp = fopen(fieldFileName, "r");
	char line[80];
	int fieldSize = (thisField->displayedWidth +1 )* thisField->displayedHeight;
	int mapIndex = 0;
	char* theMapChars = malloc(sizeof(char)*fieldSize + 1);
	printf("fieldsize: %d\n", sizeof(char)*fieldSize + 1);
//	printf("The Map size %d:\n",strlen(theMapChars));
	//move down to the right level
	while(y < thisField->displayedY){
		fgets(line,100,fp);
		y++;
	}

	while(y < (thisField->displayedY + thisField->displayedHeight)){
		fgets(line,100,fp);
		x = thisField->displayedX;
		while(x < thisField->displayedX + thisField->displayedWidth){
			theMapChars[mapIndex] = line[x];
			printf("%c\n",line[x]);
			x++;
			mapIndex++;
//			printf("map: %s\n",theMapChars);
		}
		y++;
		theMapChars[mapIndex] ='\n';
		mapIndex++;
	}
	theMapChars[mapIndex] ='\0';
	//printf("The Map size %d:\n",strlen(theMapChars));
	printf("%d\n",mapIndex);
	printf("%s",theMapChars);


//	free(theMapChars); //breaking here for some reason
	fclose(fp);
}


void drawField(HDC hdc, HDC hdcBuffer, field* this_field, shiftData * viewShift){

	HDC hdcMem = CreateCompatibleDC(hdc);
	int x;
	int y;

	for (y = 0; y < this_field->totalY; y++) {
		for (x = 0; x < this_field->totalX; x++) {

			character * tmpBackground = this_field->grid[x][y]->background;

			if(tmpBackground->direction != 0){
				drawRotatedBackground(hdcMem, hdcBuffer, tmpBackground, viewShift);
			}else{

			SelectObject(hdcMem, tmpBackground->image);

			BitBlt(hdcBuffer,
				tmpBackground->x*40 - (viewShift->xShift)*40,
				tmpBackground->y*40 - (viewShift->yShift)*40,
				tmpBackground->width,
				tmpBackground->height, hdcMem, 0, 0,
				SRCCOPY);
			}

		}
	}

	DeleteDC(hdcMem);
}

void drawRotatedBackground(HDC hdcMem, HDC hdcBuffer, character * backgroundCharacter, shiftData * viewShift){

	int direction = backgroundCharacter->direction;
	float cosine = (float) cos(direction*M_PI/2.0);
	float sine = (float) sin(direction*M_PI/2.0);

	// Compute dimensions of the resulting bitmap
	// First get the coordinates of the 3 corners other than origin
	int x1 = (int) (40 * sine);
	int y1 = (int) (40 * cosine);
	int x2 = (int) (40 * cosine + 40 * sine);
	int y2 = (int) (40 * cosine - 40 * sine);
	int x3 = (int) (40 * cosine);
	int y3 = (int) (-40 * sine);

	int minx = min(0, min(x1, min(x2,x3)));
	int miny = min(0, min(y1, min(y2,y3)));
	int maxx = max(0, max(x1, max(x2,x3)));
	int maxy = max(0, max(y1, max(y2,y3)));

	SetGraphicsMode(hdcBuffer, GM_ADVANCED);
	XFORM xForm;
	xForm.eM11 = cosine;
	xForm.eM12 = -sine;
	xForm.eM21 = sine;
	xForm.eM22 = cosine;
	xForm.eDx = (float) -minx;
	xForm.eDy = (float) -miny;

	if (!SetWorldTransform(hdcBuffer, &xForm)) {
		printf("setworldtransform failed");
	}

	SelectObject(hdcMem, backgroundCharacter->image);

	int xMod = calcXMod(direction, backgroundCharacter, viewShift);
	int yMod = calcYMod(direction, backgroundCharacter, viewShift);

	BitBlt(hdcBuffer,
			xMod,//*(backgroundCharacter->x - (viewShift->xShift) * 40),
			yMod,//*(backgroundCharacter->y - (viewShift->yShift) * 40),
			backgroundCharacter->width, backgroundCharacter->height, hdcMem, 0,
			0,
			SRCCOPY);

	//set world transform back to normal
	xForm.eM11 = 1;
	xForm.eM12 = 0;
	xForm.eM21 = 0;
	xForm.eM22 = 1;
	xForm.eDx = 0;
	xForm.eDy = 0;
	if (!SetWorldTransform(hdcBuffer, &xForm)) {
		printf("setworldtransform failed");
	}

}

int calcXMod(int direction, character * backgroundCharacter, shiftData * viewShift){
	if(direction == 1){
		return -1*(backgroundCharacter->y*40 - viewShift->yShift*40);
	}else if(direction == 2){
		return -1*(backgroundCharacter->x*40-viewShift->xShift*40);
	}
	else{
		return (backgroundCharacter->y*40 -viewShift->yShift*40);
	}
}

int calcYMod(int direction, character * backgroundCharacter, shiftData * viewShift){
	if(direction == 1){
		return (backgroundCharacter->x*40-viewShift->xShift*40);
	}else if(direction == 2){
		return -1*(backgroundCharacter->y*40-viewShift->yShift*40);
	}else{
		return -1*(backgroundCharacter->x*40-viewShift->xShift*40);
	}
}

moveNode * nodeAlreadyTraversed(moveNode ** nodes, int x, int y){
	int i = 0;
	while(nodes[i] != NULL){
		if(nodes[i]->x == x && nodes[i]->y == y){
			return nodes[i];
		}
	}

	return NULL;
}

moveNode * alreadyContainsNode(moveNode * rootNode, int x, int y) {
	moveNode * currentNode = rootNode;

	while (currentNode->nextMoveNode != NULL) {
		if (currentNode->x == x && currentNode->y == y) {
			return &currentNode;
		}

		currentNode = (moveNode *)currentNode->nextMoveNode;
	}

	//check last node
	if (currentNode->x == x && currentNode->y == y) {
		return &currentNode;

	}

	return NULL;
}

int freeUpMovePath(moveNode * currentNode){
	int sum = 1;
	if(currentNode->nextMoveNode !=NULL){
		sum += freeUpMovePath((moveNode *)currentNode->nextMoveNode);
	}

	free(currentNode);
	return sum;
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

void printField(field * thisField){
	int i,j;

	for(i = 0; i < thisField->totalX; i++){
		for(j = 0; j < thisField->totalY; j++){
			printf("[%d,%d] : %d\n",i,j, getIndividualFromField(thisField,i,j));//spaceIsAvailable(thisField,i,j));
		}
	}
}
