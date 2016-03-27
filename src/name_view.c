/*
 * name_view.c
 *
 *  Created on: Mar 27, 2016
 *      Author: Adrian
 */

#include "./headers/name_view_pub_methods.h"

static nameBoxInstance * thisNameBoxInstance;

void initNameBoxInstance(int imageID,COLORREF rgb, int x, int y){
	int i;
	thisNameBoxInstance = malloc(sizeof(nameBoxInstance));

	thisNameBoxInstance->inNameMode = 0;

	strcpy(thisNameBoxInstance->charArr,"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789.- ");

	thisNameBoxInstance->column = 0;
	thisNameBoxInstance->row = 0;
	thisNameBoxInstance->currentCharArrIndex = 0;
	thisNameBoxInstance->currentNameIndex = 0;

	thisNameBoxInstance->MAX_ROWS = 5;
	thisNameBoxInstance->MAX_COLUMNS = 12;
	thisNameBoxInstance->MAX_NAME_LENGTH = 16;

	for(i = 0; i < thisNameBoxInstance->MAX_NAME_LENGTH; i++){
		thisNameBoxInstance->name[i] = '\0';
	}

	thisNameBoxInstance->nameBox = createCharacter(imageID,rgb,x,y);

}

void resetNameBoxInstance(){
	int i;

	thisNameBoxInstance->column = 0;
	thisNameBoxInstance->row = 0;
	thisNameBoxInstance->currentCharArrIndex = 0;
	thisNameBoxInstance->currentNameIndex = 0;

	for(i = 0; i < thisNameBoxInstance->MAX_NAME_LENGTH; i++){
		thisNameBoxInstance->name[i] = '\0';
	}

}

void drawNameBoxInstance(HDC hdc, HDC hdcBuffer, RECT * prc){
	HDC hdcMem = CreateCompatibleDC(hdc);

	RECT textSelectRect;
	textSelectRect.top = thisNameBoxInstance->nameBox->y + 33 + 16*thisNameBoxInstance->row;
	textSelectRect.left = thisNameBoxInstance->nameBox->x + 15 + 21*thisNameBoxInstance->column;
	textSelectRect.bottom = textSelectRect.top + 20;
	textSelectRect.right = textSelectRect.left + 15;

	RECT textNameRect;
	textNameRect.top = thisNameBoxInstance->nameBox->y + 10;
	textNameRect.left = thisNameBoxInstance->nameBox->x + 75;
	textNameRect.bottom = textNameRect.top + 20;
	textNameRect.right = textNameRect.left + 150;

	SelectObject(hdcMem, thisNameBoxInstance->nameBox->image);
	BitBlt(hdcBuffer, thisNameBoxInstance->nameBox->x, thisNameBoxInstance->nameBox->y, thisNameBoxInstance->nameBox->width, thisNameBoxInstance->nameBox->height, hdcMem, 0, 0, SRCCOPY);

	//cursor
	DrawText(hdcBuffer, "_", 1, &textSelectRect, DT_SINGLELINE);

	//name
	DrawText(hdcBuffer, thisNameBoxInstance->name, strlen(thisNameBoxInstance->name), &textNameRect, DT_SINGLELINE);

	DeleteDC(hdcMem);
}

void destroyNameBoxInstance(){
	destroyCharacter(thisNameBoxInstance->nameBox);

	free(thisNameBoxInstance);
}

void toggleNameMode(){
	thisNameBoxInstance->inNameMode = (thisNameBoxInstance->inNameMode + 1) % 2;
}

int inNameBoxMode(){
	if(thisNameBoxInstance != NULL){
		return thisNameBoxInstance->inNameMode;
	}
	return 0;
}

char * getNameFromInstance(){
	return thisNameBoxInstance->name;
}

char getCharFromArr(){
	int index = thisNameBoxInstance->column + thisNameBoxInstance->row*13;

	return thisNameBoxInstance->charArr[index];
}

int nameEmpty(){
	if(thisNameBoxInstance->name[0] == '\0'){
		return 1;
	}
	return 0;
}

int selectCharacter(){
	thisNameBoxInstance->name[thisNameBoxInstance->currentNameIndex] = getCharFromArr();

	if(thisNameBoxInstance->currentNameIndex < thisNameBoxInstance->MAX_NAME_LENGTH){
		thisNameBoxInstance->currentNameIndex++;
	}

	return 0;
}

void backspaceName(){
	thisNameBoxInstance->name[thisNameBoxInstance->currentNameIndex] = '\0';
	if(thisNameBoxInstance->currentNameIndex > 0){
		thisNameBoxInstance->currentNameIndex--;
	}
}

void selectLetterUp(){
	if(thisNameBoxInstance->row > 0){
		thisNameBoxInstance->row--;
	}
}

void selectLetterDown(){
	if(thisNameBoxInstance->row < thisNameBoxInstance->MAX_ROWS){
		thisNameBoxInstance->row++;
	}
}

void selectLetterLeft(){
	if(thisNameBoxInstance->column > 0){
		thisNameBoxInstance->column--;
	}
}

void selectLetterRight(){
	if(thisNameBoxInstance->column < thisNameBoxInstance->MAX_COLUMNS){
		thisNameBoxInstance->column++;
	}
}
