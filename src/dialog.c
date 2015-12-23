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
		textBoxRect.left = thisDialogBox->dialogWindow->x + 10;//prc->right - prc->right*0.95;
		textBoxRect.bottom = thisDialogBox->dialogWindow->y +  thisDialogBox->dialogWindow->height;  // prc->bottom;
		textBoxRect.top = textBoxRect.bottom - 30;
		textBoxRect.right = textBoxRect.left +  thisDialogBox->dialogWindow->width;

	messageNode drawMessageNode;
	strcpy(drawMessageNode.message,thisDialogBox->currentMessage->message);
	drawMessageNode.nextMessageNode = NULL;
	drawMessageNode.previousMessageNode = NULL;

	rowLength = textBoxRect.right - textBoxRect.left;//(thisDialogBox->dialogWindow->width - thisDialogBox->dialogWindow->width * 0.1);

	HDC hdcMem = CreateCompatibleDC(hdc);

	SelectObject(hdcMem, thisDialogBox->dialogWindow->image);

	BitBlt(hdcBuffer, thisDialogBox->dialogWindow->x, thisDialogBox->dialogWindow->y,
			thisDialogBox->dialogWindow->width, thisDialogBox->dialogWindow->height, hdcMem, 0, 0, SRCCOPY);

	DeleteDC(hdcMem);

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

dialogDecision * createDialogDecisionFromLine(char * line){
	dialogDecision * newDialogDecision = malloc(sizeof(dialogDecision));

	char * value = strtok(line,";");
	newDialogDecision->rootMessageID = atoi(value);

	value = strtok(NULL,";");
	strcpy(newDialogDecision->message, value);

	value = strtok(line,";");
	newDialogDecision->targetMessageID = atoi(value);

	return newDialogDecision;
}

dialogMessage * createDialogMessageFromLine(char * line){
	dialogMessage * newDialogMessage = malloc(sizeof(dialogMessage));
	int eventType;

	char * value = strtok(line,";");
	newDialogMessage->messageID = atoi(value);

	value = strtok(NULL,";");
	strcpy(newDialogMessage->message, value);

	value = strtok(NULL,";");
	newDialogMessage->numDialogDecision = atoi(value);

	value = strtok(NULL,";");
	newDialogMessage->nextMessageID = atoi(value);

	value = strtok(NULL,";");
	eventType = atoi(value);

	if(eventType){
		eventFlag * newEventFlag = malloc(sizeof(eventFlag));
		newEventFlag->eventType = eventType;

		value = strtok(NULL,";");
		newEventFlag->individualID = atoi(value);

		value = strtok(NULL,";");
		newEventFlag->itemID = atoi(value);

		newDialogMessage->event = newEventFlag;
	}else{
		newDialogMessage->event = NULL;
	}

	return newDialogMessage;
}

dialogMessage * findNextDialogMessage(dialogMessage * thisMessage, dialogMessage ** messageArr, int numMessages){
	int i;

	if(thisMessage->nextMessageID != 0){
		return NULL;
	}
	for (i = 0; i < numMessages; i++) {
		if (thisMessage->nextMessageID == messageArr[i]->nextMessageID) {
			return messageArr[i];
		}
	}

	cwrite("*NEXT NODE ID NOT FOUND!!*");
	return NULL;
}
