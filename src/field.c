/*
 * field.c
 *
 *  Created on: Mar 29, 2015
 *      Author: Adrian
 */

#include"./headers/field_pub_methods.h"
#include<stdio.h>
#include<string.h>

individual* getIndividualFromField(field* thisField, int x, int y){
	if(x < thisField->totalX && x >= 0 && y < thisField->totalY && y >= 0){

		return (thisField->grid[x][y])->currentIndividual;
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
		return &thisField->grid[x][y];
	}

	return NULL;
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
	space ** currentSpace = getSpaceFromField(thisField,thisIndividual->playerCharacter->x,thisIndividual->playerCharacter->y);
	space ** newSpace;
	int newX = thisIndividual->playerCharacter->x + xMoveChange(direction);
	int newY = thisIndividual->playerCharacter->y + yMoveChange(direction);

	//check if in bounds, and newSpace exists
	if(!(newX >= 0 && newX < thisField->totalX && newY >=0 && newY < thisField->totalY)){
		return 0;
	}

	//space exists, wont be null
	newSpace = getSpaceFromField(thisField,newX,newY);


	//can the individual go to this space?
	if((*newSpace)->isPassable && (*newSpace)->currentIndividual == NULL){
		(*currentSpace)->currentIndividual = NULL;
		(*newSpace)->currentIndividual = thisIndividual;
		thisIndividual->playerCharacter->x = newX;
		thisIndividual->playerCharacter->y = newY;
		return 1;
	}else{
//		printf("is not passable");
		return 0;
	}

}
int moveCursor(field *thisField, cursor *thisCursor, int direction){
	int newX = thisCursor->cursorCharacter->x + xMoveChange(direction);
	int newY = thisCursor->cursorCharacter->y + yMoveChange(direction);
	printf("newX:%d, newY:%d\n",newX, newY);
	//check if in bounds, and newSpace exists
	if(newX >= 0 && newX < thisField->totalX && newY >=0 && newY < thisField->totalY){
		thisCursor->cursorCharacter->x = newX;
		thisCursor->cursorCharacter->y = newY;
		return 1;
	}else{
		return 0;
	}

}

/*
 * take in a character, output the corrosponding background name (string)
*/
int generateBackground(char backgroundSymbol){


	printf("backgroundChar: %c\n", backgroundSymbol);

	if(backgroundSymbol == 'c'){
		printf("Found  a C!\n");
		return 2003;
	}else{
		return 2002; //grass background
	}
}

field* initField(char* fieldFileName){
	field* thisField = malloc(sizeof(field));
	FILE * fp = fopen(fieldFileName, "r");
	char line[80];
	int init_y = 0;
	int init_x;
	while(fgets(line,80,fp) != NULL){

		for(init_x = 0; init_x < strlen(line); init_x++){

			space* newSpace = malloc(sizeof(space));
			newSpace->currentIndividual = malloc(sizeof(individual));
//			newSpace->background = malloc(sizeof(character));
//			newSpace->currentIndividual->playerCharacter = malloc(sizeof(character));
			character* backgroundCharacter = malloc(sizeof(character));
			char currentChar = line[init_x];

			//initialize background
			backgroundCharacter->width = 40;
			backgroundCharacter->height = 40;
			backgroundCharacter->x = init_x * 40;
			backgroundCharacter->y = init_y * 40;
			backgroundCharacter->name = &currentChar;
			backgroundCharacter->imageID = generateBackground(currentChar);
			if(backgroundCharacter->imageID == 2003){
				newSpace->isPassable = 0;
			}else{
				newSpace->isPassable = 1;
			}
			newSpace->background = backgroundCharacter;
			newSpace->currentIndividual = NULL;
			thisField->grid[init_x][init_y] = newSpace;

		}
		init_y++;

		puts(line);
		printf("len:%d\n", strlen(line));
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


void drawField(HDC hdc, HDC hdcBuffer, field* this_field){

	HDC hdcMem = CreateCompatibleDC(hdc);
	int x;
	int y;

	//field
	for (y = 0; y < this_field->totalY; y++) {
		for (x = 0; x < this_field->totalX; x++) {

			SelectObject(hdcMem, this_field->grid[x][y]->background->image);

			BitBlt(hdcBuffer, this_field->grid[x][y]->background->x,
					this_field->grid[x][y]->background->y,
					this_field->grid[x][y]->background->width,
					this_field->grid[x][y]->background->height, hdcMem, 0, 0,
					SRCCOPY);

		}
	}

	DeleteDC(hdcMem);
}

int b_main(){
	field* thisField;
//	thisField.displayedX = 1;
//	thisField.displayedY = 0;
//	thisField.displayedWidth = 4;
//	thisField.displayedHeight = 3;
	thisField = initField( "C:\\Users\\Adrian\\workspace\\Natural_1\\src\\map1.txt");
//	updateFiled(&thisField, "C:\\Users\\Adrian\\workspace\\Natural_1\\src\\map1.txt");//"./map1.txt");

	return 0;

}
