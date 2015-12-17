/*
 * dialog.c
 *
 *  Created on: Dec 10, 2015
 *      Author: Adrian
 */
#include"./headers/dialog_pub_methods.h"

dialogBox * thisDialogBox;

dialogBox * initDialogBox(int imageID, int x, int y, COLORREF rgb){
	dialogBox * toReturn = malloc(sizeof(dialogBox));
	toReturn->currentMessage = NULL;
	toReturn->rootMessage = NULL;
	toReturn->dialogWindow = createCharacter(imageID, rgb, x, y);

	toReturn->drawBox = 0;

	return toReturn;
}

void initThisDialogBox(int imageID, int x, int y, COLORREF rgb){
	thisDialogBox = initDialogBox(imageID, x, y, rgb);
}

int shouldDrawDialogBox(){
	return thisDialogBox->drawBox;
}

void drawDialogBox(HDC hdc, HDC hdcBuffer, RECT * prc){
	int rowLength;

	RECT textBoxRect;
		textBoxRect.top =  thisDialogBox->dialogWindow->y + 23;// prc->bottom - 30;
		textBoxRect.left = thisDialogBox->dialogWindow->x + 10;//prc->right - prc->right*0.95;
		textBoxRect.bottom = thisDialogBox->dialogWindow->y +  thisDialogBox->dialogWindow->height;  // prc->bottom;
		textBoxRect.right = thisDialogBox->dialogWindow->x +  thisDialogBox->dialogWindow->width;

	messageNode drawMessageNode;
	strcpy(drawMessageNode.message,thisDialogBox->currentMessage->message);
	drawMessageNode.nextMessageNode = NULL;
	drawMessageNode.previousMessageNode = NULL;

	rowLength = (thisDialogBox->dialogWindow->width - thisDialogBox->dialogWindow->width * 0.1);

	HDC hdcMem = CreateCompatibleDC(hdc);

	SelectObject(hdcMem, thisDialogBox->dialogWindow->image);

	BitBlt(hdcBuffer, thisDialogBox->dialogWindow->x, thisDialogBox->dialogWindow->y,
			thisDialogBox->dialogWindow->width, thisDialogBox->dialogWindow->height, hdcMem, 0, 0, SRCCOPY);

	DeleteDC(hdcMem);

//	DrawText(hdcBuffer, thisDialogBox->currentMessage->message, -1, &textBoxRect, DT_SINGLELINE);
	drawConsoleText(hdcBuffer, &textBoxRect, &drawMessageNode, 5, rowLength);
}

void setCurrentMessage(dialogMessage * currentMessage){
	thisDialogBox->currentMessage = currentMessage;
}

void setSimpleDialogMessage(char * string){
	thisDialogBox->currentMessage = malloc(sizeof(dialogMessage));
	strcpy(thisDialogBox->currentMessage->message, string);
}

void toggleDrawDialogBox(){
	thisDialogBox->drawBox = (thisDialogBox->drawBox+1)%2;
}
