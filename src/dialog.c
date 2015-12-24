/*
 * dialog.c
 *
 *  Created on: Dec 10, 2015
 *      Author: Adrian
 */
#include"./headers/dialog_pub_methods.h"
#include<stdio.h>

dialogBox * thisDialogBox;

dialogBox * initDialogBox(int imageID, int x, int y, COLORREF rgb){
	dialogBox * toReturn = malloc(sizeof(dialogBox));
	toReturn->currentMessage = NULL;
	toReturn->dialogMessages = NULL;
	toReturn->numRows = 6;
	toReturn->numDialogMessages = 0;
	toReturn->decisionIndex = 0;
	toReturn->dialogWindow = createCharacter(imageID, rgb, x, y);
	toReturn->selectArrow = createCharacter(3001, RGB(255,0,255), x + 20, y);
	toReturn->drawBox = 0;

	return toReturn;
}

void initThisDialogBox(int imageID, int x, int y, COLORREF rgb){
	thisDialogBox = initDialogBox(imageID, x, y, rgb);
}

int shouldDrawDialogBox(){
	return thisDialogBox->drawBox;
}

void setDialogMessages(dialogMessage ** messageArr, int numMessages){
	thisDialogBox->dialogMessages = messageArr;
	thisDialogBox->numDialogMessages = numMessages;
}

void clearDialogMessages(){
	//TODO: step through and free all messages
	thisDialogBox->dialogMessages = NULL;
	thisDialogBox->numDialogMessages = 0;
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

	if(thisDialogBox->currentMessage->numDialogDecision > 0){
		int i, rowToStartOn;

		for(i = 0; i < thisDialogBox->currentMessage->numDialogDecision; i++){
			rowToStartOn = calcNumIndexes(drawMessageNode.message, rowLength,hdcBuffer, (textBoxRect.right - textBoxRect.right*0.9));
			thisDialogBox->decisionIndexRow[i] = rowToStartOn;
			dialogDecision * tmpDecision = thisDialogBox->currentMessage->decisions[i];
			char tmpDecisionStr[70];
			strcpy(tmpDecisionStr, "&     ");
			strcat(tmpDecisionStr, tmpDecision->message);

			strcat(drawMessageNode.message, tmpDecisionStr);
		}





		SelectObject(hdcMem, thisDialogBox->selectArrow->image);

		BitBlt(hdcBuffer, thisDialogBox->selectArrow->x, thisDialogBox->selectArrow->y + thisDialogBox->dialogWindow->height - (15*(thisDialogBox->numRows -  thisDialogBox->decisionIndexRow[thisDialogBox->decisionIndex])),
				thisDialogBox->selectArrow->width, thisDialogBox->selectArrow->height, hdcMem, 0, 0, SRCCOPY);

	}

	DeleteDC(hdcMem);

	drawConsoleText(hdcBuffer, &textBoxRect, &drawMessageNode, thisDialogBox->numRows, rowLength);
}

void setCurrentMessage(dialogMessage * currentMessage){
	thisDialogBox->currentMessage = currentMessage;
}

void nextDialogDecision(){
	if(thisDialogBox->decisionIndex+1 < thisDialogBox->currentMessage->numDialogDecision){
		thisDialogBox->decisionIndex++;
	}else{ //roll over to 0
		thisDialogBox->decisionIndex = 0;
	}
}

void previousDialogDecision(){
	if(thisDialogBox->decisionIndex != 0){
		thisDialogBox->decisionIndex--;
	}else{
		thisDialogBox->decisionIndex = thisDialogBox->currentMessage->numDialogDecision-1;
	}
}

void selectDecision(){
	dialogDecision * theDecision = thisDialogBox->currentMessage->decisions[thisDialogBox->decisionIndex];
	setCurrentMessage(theDecision->targetMessage);
}

void advanceDialog(){
	if(thisDialogBox->currentMessage->nextMessage != NULL || thisDialogBox->currentMessage->numDialogDecision > 0){
		if(thisDialogBox->currentMessage->nextMessage != NULL){
			thisDialogBox->currentMessage = thisDialogBox->currentMessage->nextMessage;
		}else{
			selectDecision();
			thisDialogBox->decisionIndex = 0;
		}
	}else{ //no more messages, stop drawing
		thisDialogBox->currentMessage->nextMessage = NULL;
		toggleDrawDialogBox();
	}
}

void setSimpleDialogMessage(char * string){
	thisDialogBox->currentMessage = malloc(sizeof(dialogMessage));
	strcpy(thisDialogBox->currentMessage->message, string);
	thisDialogBox->currentMessage->numDialogDecision = 0;
	thisDialogBox->currentMessage->nextMessage = NULL;
}

void toggleDrawDialogBox(){
	thisDialogBox->drawBox = (thisDialogBox->drawBox+1)%2;
}

int setCurrentMessageByID(int messageID){
	int i;

	for(i = 0; i < thisDialogBox->numDialogMessages; i++){
		if(thisDialogBox->dialogMessages[i]->messageID == messageID){
			setCurrentMessage(thisDialogBox->dialogMessages[i]);
			return 1;
		}
	}

	//dialog ID not found
	return 0;
}

dialogDecision * createDialogDecisionFromLine(char * line){
	dialogDecision * newDialogDecision = malloc(sizeof(dialogDecision));

	char * value = strtok(line,";");
	newDialogDecision->rootMessageID = atoi(value);

	value = strtok(NULL,";");
	strcpy(newDialogDecision->message, value);

	value = strtok(NULL,";");
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
	newDialogMessage->dialogCheckpoint = atoi(value);

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

	newDialogMessage->decisions[0] = NULL;

	return newDialogMessage;
}

dialogMessage * findNextDialogMessage(dialogMessage * thisMessage, dialogMessage ** messageArr, int numMessages){
	int i;

	if(thisMessage->nextMessageID == 0){
		return NULL;
	}
	for (i = 0; i < numMessages; i++) {
		if (thisMessage->nextMessageID == messageArr[i]->messageID) {
			return messageArr[i];
		}
	}

	cwrite("*CONFIGURATION ERROR: NEXT NODE ID NOT FOUND!!*");
	return NULL;
}

void addDecisionToDialogMessage(dialogMessage * thisMessage, dialogDecision * thisDecision){
	int i;

	for(i = 0; i < 10; i++){
		if(thisMessage->decisions[i] == NULL){
			thisMessage->decisions[i] = thisDecision;

			if(i+1 < 10){
				thisMessage->decisions[i+1] = NULL;
			}

			return;
		}
	}

}

void loadDialog(char * fileName, char * directory){
	char * fullFileName = appendStrings(directory, fileName);
	fullFileName[strlen(fullFileName)-1] = '\0'; //remove '\n' at end of line
	FILE * FP = fopen(fullFileName, "r");
	int numMessages, i, j, foundNode = 0, nextNotNull;
	char line[512];

	if(!fgets(line,160,FP)){ //get num messages, exit if file is empty
		return;
	}

	numMessages = atoi(line);

	dialogMessage ** messageArr = malloc(sizeof(dialogMessage)*numMessages);

	for(i = 0; i < numMessages; i++){
		fgets(line, 512, FP);
		messageArr[i] = createDialogMessageFromLine(line);
	}

	//Populate DialogMessage's next DialogMessage
	for(i = 0; i < numMessages; i++){
		messageArr[i]->nextMessage = findNextDialogMessage(messageArr[i], messageArr, numMessages);
	}

	int rootFound = 0, targetFound = 0;
	while(fgets(line, 512,FP)){
		dialogDecision * tmpDecision = createDialogDecisionFromLine(line);

		for(i = 0; i < numMessages; i++){
			if(messageArr[i]->messageID == tmpDecision->rootMessageID){
				rootFound = 1;
				addDecisionToDialogMessage(messageArr[i], tmpDecision);
			}

			if(messageArr[i]->messageID == tmpDecision->targetMessageID){
				targetFound = 1;
				tmpDecision->targetMessage = messageArr[i];
			}

			if(rootFound && targetFound){
				rootFound = 0;
				targetFound = 0;

				break;
			}
		}
	}

	setDialogMessages(messageArr, numMessages);
}
