/*
 * dialog.c
 *
 *  Created on: Dec 10, 2015
 *      Author: Adrian
 */
#include"./headers/dialog_pub_methods.h"
#include<stdio.h>

static dialogInstance * thisDialogInstance;

dialogInstance * initDialogBox(int imageID, int x, int y, COLORREF rgb){
	dialogInstance * toReturn = malloc(sizeof(dialogInstance));
	toReturn->currentMessage = NULL;
	toReturn->dialogMessages = NULL;
	toReturn->numRows = 6;
	toReturn->numDialogMessages = 0;
	toReturn->decisionIndex = 0;
	toReturn->dialogWindow = createCharacter(imageID, rgb, x, y);
	toReturn->selectArrow = createCharacter(3001, RGB(255,0,255), x + 20, y);
	toReturn->drawBox = 0;
	toReturn->speakingIndividualID = 0;
	toReturn->individualDialogRegistry[0] = NULL;

	return toReturn;
}

void initThisDialogBox(int imageID, int x, int y, COLORREF rgb){
	thisDialogInstance = initDialogBox(imageID, x, y, rgb);
}

int shouldDrawDialogBox(){
	if(thisDialogInstance != NULL){
		return thisDialogInstance->drawBox;
	}

	return 0;
}

void setDialogMessages(dialogMessage ** messageArr, int numMessages){
	thisDialogInstance->dialogMessages = messageArr;
	thisDialogInstance->numDialogMessages = numMessages;
}

void clearDialogMessages(){
	//TODO: step through and free all messages
	thisDialogInstance->dialogMessages = NULL;
	thisDialogInstance->numDialogMessages = 0;
}

void drawDialogBox(HDC hdc, HDC hdcBuffer, RECT * prc){
	int rowLength;

	RECT textBoxRect;
		textBoxRect.left = thisDialogInstance->dialogWindow->x + 10;//prc->right - prc->right*0.95;
		textBoxRect.bottom = thisDialogInstance->dialogWindow->y +  thisDialogInstance->dialogWindow->height;  // prc->bottom;
		textBoxRect.top = textBoxRect.bottom - 30;
		textBoxRect.right = textBoxRect.left +  thisDialogInstance->dialogWindow->width - 20;

	messageNode drawMessageNode;
	strcpy(drawMessageNode.message,thisDialogInstance->currentMessage->message);
	drawMessageNode.nextMessageNode = NULL;
	drawMessageNode.previousMessageNode = NULL;

	rowLength = textBoxRect.right - textBoxRect.left;//(thisDialogBox->dialogWindow->width - thisDialogBox->dialogWindow->width * 0.1);

	HDC hdcMem = CreateCompatibleDC(hdc);

	SelectObject(hdcMem, thisDialogInstance->dialogWindow->image);

	BitBlt(hdcBuffer, thisDialogInstance->dialogWindow->x, thisDialogInstance->dialogWindow->y,
			thisDialogInstance->dialogWindow->width, thisDialogInstance->dialogWindow->height, hdcMem, 0, 0, SRCCOPY);

	if(thisDialogInstance->currentMessage->numDialogDecision > 0){
		int i, rowToStartOn;

		for(i = 0; i < thisDialogInstance->currentMessage->numDialogDecision; i++){
			rowToStartOn = calcNumIndexes(drawMessageNode.message, rowLength,hdcBuffer, (textBoxRect.right - textBoxRect.right*0.9));
			thisDialogInstance->decisionIndexRow[i] = rowToStartOn;
			dialogDecision * tmpDecision = thisDialogInstance->currentMessage->decisions[i];
			char tmpDecisionStr[70];
			strcpy(tmpDecisionStr, "&     ");
			strcat(tmpDecisionStr, tmpDecision->message);

			strcat(drawMessageNode.message, tmpDecisionStr);
		}

		SelectObject(hdcMem, thisDialogInstance->selectArrow->image);

		BitBlt(hdcBuffer, thisDialogInstance->selectArrow->x, thisDialogInstance->selectArrow->y + thisDialogInstance->dialogWindow->height - (15*(thisDialogInstance->numRows -  thisDialogInstance->decisionIndexRow[thisDialogInstance->decisionIndex])),
				thisDialogInstance->selectArrow->width, thisDialogInstance->selectArrow->height, hdcMem, 0, 0, SRCCOPY);

	}

	DeleteDC(hdcMem);

	drawConsoleText(hdcBuffer, &textBoxRect, &drawMessageNode, thisDialogInstance->numRows, rowLength);
}

void setCurrentMessage(dialogMessage * currentMessage){
	thisDialogInstance->currentMessage = currentMessage;
}

void setSpeakingIndividualID(int ID){
	thisDialogInstance->speakingIndividualID = ID;
}

void nextDialogDecision(){
	if(thisDialogInstance->decisionIndex+1 < thisDialogInstance->currentMessage->numDialogDecision){
		thisDialogInstance->decisionIndex++;
	}else{ //roll over to 0
		thisDialogInstance->decisionIndex = 0;
	}
}

void previousDialogDecision(){
	if(thisDialogInstance->decisionIndex != 0){
		thisDialogInstance->decisionIndex--;
	}else{
		thisDialogInstance->decisionIndex = thisDialogInstance->currentMessage->numDialogDecision-1;
	}
}

void selectDecision(){
	dialogDecision * theDecision = thisDialogInstance->currentMessage->decisions[thisDialogInstance->decisionIndex];
	setCurrentMessage(theDecision->targetMessage);
}

void advanceDialog(){
	if(thisDialogInstance->currentMessage->nextMessage != NULL || thisDialogInstance->currentMessage->numDialogDecision > 0){
		if(thisDialogInstance->currentMessage->nextMessage != NULL){
			thisDialogInstance->currentMessage = thisDialogInstance->currentMessage->nextMessage;
		}else{
			selectDecision();
			thisDialogInstance->decisionIndex = 0;
		}

		if (thisDialogInstance->currentMessage->dialogCheckpoint) {
			setIndividualDialog(thisDialogInstance->currentMessage->messageID);
		}
	}else{ //no more messages, stop drawing
		thisDialogInstance->currentMessage->nextMessage = NULL;
		thisDialogInstance->currentMessage->eventID = 0;
		thisDialogInstance->speakingIndividualID = 0;
		toggleDrawDialogBox();
	}
}

int getEventFromCurrentMessage(){
	return thisDialogInstance->currentMessage->eventID;
}

void setSimpleDialogMessage(char * string){
	thisDialogInstance->currentMessage = malloc(sizeof(dialogMessage));
	strcpy(thisDialogInstance->currentMessage->message, string);
	thisDialogInstance->currentMessage->numDialogDecision = 0;
	thisDialogInstance->currentMessage->nextMessage = NULL;
}

void toggleDrawDialogBox(){
	thisDialogInstance->drawBox = (thisDialogInstance->drawBox+1)%2;
}

int setCurrentMessageByIndividualID(int individualID){
	int i;

	for(i = 0; i < 500; i++){
		if(thisDialogInstance->individualDialogRegistry[i] == NULL){
			return 0;
		}else if(thisDialogInstance->individualDialogRegistry[i]->individualID == individualID){
			return setCurrentMessageByMessageID(thisDialogInstance->individualDialogRegistry[i]->dialogID);
		}
	}

	//dialog ID not found
	return 0;
}

int setCurrentMessageByMessageID(int messageID){
	int i;

	for (i = 0; i < thisDialogInstance->numDialogMessages; i++) {
		if (thisDialogInstance->dialogMessages[i]->messageID == messageID) {
			setCurrentMessage(thisDialogInstance->dialogMessages[i]);
			return 1;
		}
	}

	return 0;
}

int loadOrAddIndividualDialog(int individualID, int dialogID){
	int i;

	if(dialogID == 0){
		return 0;
	}

	for(i = 0; i < 500; i++){
		if(thisDialogInstance->individualDialogRegistry[i] == NULL){ //entry not found, create new entry
			individualDialog * thisDialogEntry = malloc(sizeof(individualDialog));
			thisDialogEntry->individualID = individualID;
			thisDialogEntry->dialogID = dialogID;

			thisDialogInstance->individualDialogRegistry[i] = thisDialogEntry;
			if(i+1 < 500){
				thisDialogInstance->individualDialogRegistry[i+1] = NULL;
			}

			return dialogID;
		}else if(thisDialogInstance->individualDialogRegistry[i]->individualID == individualID){ //entry already exists
			return thisDialogInstance->individualDialogRegistry[i]->dialogID;
		}
	}

	cwrite("*MAX NPCs IN individualDialogRegistry!*");
	return -1;
}

void setIndividualDialog(int dialogID){
	int i;

	for(i = 0; i < 500; i++){
		if(thisDialogInstance->individualDialogRegistry[i]->individualID == thisDialogInstance->speakingIndividualID){
			thisDialogInstance->individualDialogRegistry[i]->dialogID = dialogID;
			return;
		}
	}
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
	newDialogMessage->eventID = atoi(value);

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
