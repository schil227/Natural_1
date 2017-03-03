/*
 * dialog.c
 *
 *  Created on: Dec 10, 2015
 *      Author: Adrian
 */
#include"./headers/dialog_pub_methods.h"
#include<stdio.h>

#define YIELD_DIALOG_ID 1030
#define WONT_YIELD_DIALOG_ID 1034
static dialogInstance * thisDialogInstance;

dialogInstance * initDialogBox(int imageID, int x, int y, COLORREF rgb){


	dialogInstance * toReturn = malloc(sizeof(dialogInstance));
	toReturn->dialogWindow = malloc(sizeof(fixedCharacter));
	toReturn->selectArrow = malloc(sizeof(fixedCharacter));

	toReturn->currentMessage = NULL;
	toReturn->numRows = 6;
	toReturn->numDialogMessages = 0;
	toReturn->decisionIndex = 0;

	toReturn->dialogWindow = createCharacter(imageID, rgb, x, y);
	toReturn->selectArrow = createCharacter(1504, RGB(255,0,255), x + 20, y);
	toReturn->drawBox = 0;
	toReturn->speakingIndividualID = 0;
	toReturn->individualDialogRegistry[0] = NULL;
	toReturn->MAX_INDIVIDUAL_DIALOG_REGISTRY = 2000;
	toReturn->MAX_DIALOG_MESSAGES = 10000;

	toReturn->speakMode = 1;
	toReturn->speakDrawLength = 1;
	toReturn->speakDrawSpeedInTicks = 1;
	toReturn->numTicks = 0;
	toReturn->speakSoundID = 8;

	toReturn->nullMessage = malloc(sizeof(dialogMessage));
	toReturn->nullMessage->messageID = 0;
	toReturn->nullMessage->numDialogDecision = 0;
	toReturn->nullMessage->numDialogDecisionsParsed = 0;
	toReturn->nullMessage->nextMessage = NULL;
	toReturn->nullMessage->message[0] = '\0';
	toReturn->nullMessage->parsedMessage[0] = '\0';

	return toReturn;
}

void initThisDialogBox(int imageID, int x, int y, COLORREF rgb){

	thisDialogInstance = initDialogBox(imageID, x, y, rgb);
}

void destroyThisDialogBox(){
	destroyCharacter(thisDialogInstance->dialogWindow);
	destroyCharacter(thisDialogInstance->selectArrow);
	free(thisDialogInstance);

	thisDialogInstance = NULL;
}

int shouldDrawDialogBox(){
	if(thisDialogInstance != NULL){
		return thisDialogInstance->drawBox;
	}

	return 0;
}

void clearDialogMessages(){
	//TODO: step through and free all messages
	thisDialogInstance->numDialogMessages = 0;
}

void shouldSpeakTickTrigger(){
	if(thisDialogInstance->speakMode){
		thisDialogInstance->numTicks++;
		if(thisDialogInstance->numTicks > thisDialogInstance->speakDrawSpeedInTicks){
			thisDialogInstance->numTicks = 0;
			thisDialogInstance->speakDrawLength++;
			triggerSoundEffect(thisDialogInstance->speakSoundID);
		}
	}
}

void drawDialogBox(HDC hdc, HDC hdcBuffer, RECT * prc){
	int rowLength;

	RECT textBoxRect;
		textBoxRect.left = thisDialogInstance->dialogWindow->x + 10;//prc->right - prc->right*0.95;
		textBoxRect.bottom = thisDialogInstance->dialogWindow->y +  thisDialogInstance->dialogWindow->fixedHeight;  // prc->bottom;
		textBoxRect.top = textBoxRect.bottom - 30;
		textBoxRect.right = textBoxRect.left +  thisDialogInstance->dialogWindow->fixedWidth - 20;

	messageNode drawMessageNode;
	if(thisDialogInstance->speakMode && thisDialogInstance->speakDrawLength == strlen(thisDialogInstance->currentMessage->parsedMessage)){
		thisDialogInstance->speakMode = 0;
		thisDialogInstance->numTicks = 0;
	}

	if(thisDialogInstance->speakMode){
		strncpy(drawMessageNode.message,thisDialogInstance->currentMessage->parsedMessage, thisDialogInstance->speakDrawLength);
		drawMessageNode.message[thisDialogInstance->speakDrawLength] = '\0';
	} else{
		strcpy(drawMessageNode.message,thisDialogInstance->currentMessage->parsedMessage);
	}

	drawMessageNode.nextMessageNode = NULL;
	drawMessageNode.previousMessageNode = NULL;

	rowLength = textBoxRect.right - textBoxRect.left;//(thisDialogBox->dialogWindow->width - thisDialogBox->dialogWindow->width * 0.1);

	HDC hdcMem = CreateCompatibleDC(hdc);

	SelectObject(hdcMem, thisDialogInstance->dialogWindow->fixedImage);

	BitBlt(hdcBuffer, thisDialogInstance->dialogWindow->x, thisDialogInstance->dialogWindow->y,
			thisDialogInstance->dialogWindow->fixedWidth, thisDialogInstance->dialogWindow->fixedHeight, hdcMem, 0, 0, SRCCOPY);

	if(thisDialogInstance->currentMessage->numDialogDecisionsParsed > 0 && !thisDialogInstance->speakMode){
		int i, rowToStartOn;

		for(i = 0; i < thisDialogInstance->currentMessage->numDialogDecisionsParsed; i++){
			rowToStartOn = calcNumIndexes(drawMessageNode.message, rowLength, hdcBuffer, (int)(textBoxRect.right - textBoxRect.right*0.9));
			thisDialogInstance->decisionIndexRow[i] = rowToStartOn;
			dialogDecision * tmpDecision = thisDialogInstance->currentMessage->parsedDecisions[i];
			char tmpDecisionStr[70];
			strcpy(tmpDecisionStr, "&     ");
			strcat(tmpDecisionStr, tmpDecision->processedMessage);

			strcat(drawMessageNode.message, tmpDecisionStr);
		}

		thisDialogInstance->numRows = calcNumIndexes(drawMessageNode.message, rowLength, hdcBuffer, (int)(textBoxRect.right - textBoxRect.right*0.9)) + 1;

		SelectObject(hdcMem, thisDialogInstance->selectArrow->fixedImage);

		int rowsFromTheTopRowInPixels = (15*(thisDialogInstance->numRows -  thisDialogInstance->decisionIndexRow[thisDialogInstance->decisionIndex]));

		BitBlt(hdcBuffer, thisDialogInstance->selectArrow->x,
				thisDialogInstance->selectArrow->y + thisDialogInstance->dialogWindow->fixedHeight - rowsFromTheTopRowInPixels,
				thisDialogInstance->selectArrow->fixedWidth, thisDialogInstance->selectArrow->fixedHeight, hdcMem, 0, 0, SRCCOPY);
	}

	DeleteDC(hdcMem);

	drawConsoleText(hdcBuffer, &textBoxRect, &drawMessageNode, thisDialogInstance->numRows, rowLength);
}

void updateParsedMessage(){
	int i;
	strcpy(thisDialogInstance->currentMessage->parsedMessage, thisDialogInstance->currentMessage->message);

	if(thisDialogInstance->currentMessage->numMessageInserts > 0){
		for(i = 0; i < thisDialogInstance->currentMessage->numMessageInserts; i++){
			char * tmpStr = getContextData(thisDialogInstance->currentMessage->messageInserts[i]);
			if(tmpStr != NULL){
				if(strlen(tmpStr) + strlen(thisDialogInstance->currentMessage->parsedMessage) < thisDialogInstance->currentMessage->MESSAGE_SIZE){
					insertIntoMessage(thisDialogInstance->currentMessage->parsedMessage, tmpStr);
				}

				free(tmpStr);
			}
		}
	}

	if(thisDialogInstance->currentMessage->numDialogDecision > 0){

		//clear old ones
		if(thisDialogInstance->currentMessage->numDialogDecisionsParsed > 0){
			for(i = 0; i < thisDialogInstance->currentMessage->numDialogDecisionsParsed; i++){
				destroyDialogDecision(thisDialogInstance->currentMessage->parsedDecisions[i]);
			}
		}

		thisDialogInstance->currentMessage->numDialogDecisionsParsed = 0;

		for(i = 0; i < thisDialogInstance->currentMessage->numDialogDecision; i++){
			if(thisDialogInstance->currentMessage->decisions[i]->displayEventID == 0 || triggerEvent(thisDialogInstance->currentMessage->decisions[i]->displayEventID) ){
				dialogDecision * newParsedDecision = cloneDecisionDialog(thisDialogInstance->currentMessage->decisions[i]);

				thisDialogInstance->currentMessage->parsedDecisions[thisDialogInstance->currentMessage->numDialogDecisionsParsed] = newParsedDecision;
				thisDialogInstance->currentMessage->numDialogDecisionsParsed++;

				if(newParsedDecision->numMessageInserts > 0){
					int j;

					for(j = 0; j < newParsedDecision->numMessageInserts; j++){
						char * tmpStr = getContextData(newParsedDecision->messageInserts[j]);
						if(tmpStr != NULL){
							if(strlen(tmpStr) + strlen(newParsedDecision->processedMessage) < newParsedDecision->MESSAGE_SIZE){
								insertIntoMessage(newParsedDecision->processedMessage, tmpStr);
							}

							free(tmpStr);
						}
					}
				}


			}
		}
	}
}

void setCurrentMessage(dialogMessage * currentMessage){
	if(currentMessage == NULL){ //stop speaking
		thisDialogInstance->currentMessage = thisDialogInstance->nullMessage;
//		thisDialogInstance->speakingIndividualID = 0;
		toggleDrawDialogBox();
	}else{
		thisDialogInstance->currentMessage = currentMessage;
		updateParsedMessage();
	}
}

void setSpeakingIndividualID(int ID){
	thisDialogInstance->speakingIndividualID = ID;
}

int getSpeakingIndividualID(){
	return thisDialogInstance->speakingIndividualID;
}

void nextDialogDecision(){
	if(thisDialogInstance->decisionIndex+1 < thisDialogInstance->currentMessage->numDialogDecisionsParsed){
		thisDialogInstance->decisionIndex++;
	}else{ //roll over to 0
		thisDialogInstance->decisionIndex = 0;
	}
}

void previousDialogDecision(){
	if(thisDialogInstance->decisionIndex != 0){
		thisDialogInstance->decisionIndex--;
	}else{
		thisDialogInstance->decisionIndex = thisDialogInstance->currentMessage->numDialogDecisionsParsed-1;
	}
}

dialogMessage * getDialogMessageByID(int id){
	int i;

	if(id == 0){
		return NULL;
	}

	for(i = 0; i < thisDialogInstance->numDialogMessages; i++){
		if(thisDialogInstance->dialogMessages[i]->messageID == id){
			return thisDialogInstance->dialogMessages[i];
		}
	}

	char outlog[128];
	sprintf(outlog, "!! COULD NOT FIND DIALOG MESSAGE BY ID: %d !!", id);
	cwrite(outlog);

	return NULL;
}

void selectDecision(){
	dialogDecision * theDecision = thisDialogInstance->currentMessage->parsedDecisions[thisDialogInstance->decisionIndex];


	if(theDecision->eventID == 0 && theDecision->targetMessage == 0){
		setCurrentMessage(NULL);
	}else if(theDecision->eventID > 0){
		dialogMessage * nextMessage = getDialogMessageByID(triggerEvent(theDecision->eventID));
		setCurrentMessage(nextMessage);
	}else{
		setCurrentMessage(theDecision->targetMessage);
	}
}

int disableSpeakModeIfEnabled(){
	if(thisDialogInstance->speakMode){
		thisDialogInstance->speakMode = 0;
		return 1;
	}

	return 0;
}

void insertIntoMessage(char * sourceString, char * insertValue){
	int i, index = -1;

	for(i = 0; i < strlen(sourceString); i++){
		if(sourceString[i] == '@'){
			index = i;
			break;
		}
	}

	if(index == -1){
		return;
	}

	int restLength = strlen(sourceString) - index;
	printf("index: %d, length: %d, length-index: %d\n",index, strlen(sourceString), restLength);

	char * rest = malloc(sizeof(char) * restLength);

	strncpy(rest, &sourceString[index+1], restLength);
	sourceString[index] = '\0';
	strcat(sourceString, insertValue);
	strcat(sourceString, rest);

	free(rest);
}

int advanceDialog(){
	if(thisDialogInstance->currentMessage->nextMessage != NULL || thisDialogInstance->currentMessage->numDialogDecisionsParsed > 0){
		if(thisDialogInstance->currentMessage->nextMessage != NULL){
			thisDialogInstance->speakMode = 1;
			thisDialogInstance->speakDrawLength = 1;
			thisDialogInstance->currentMessage = thisDialogInstance->currentMessage->nextMessage;
			updateParsedMessage();

		}else{
			selectDecision();
			thisDialogInstance->decisionIndex = 0;
			thisDialogInstance->speakMode = 1;
			thisDialogInstance->speakDrawLength = 1;
		}

		if (thisDialogInstance->currentMessage->dialogCheckpoint) {
			setIndividualDialog(thisDialogInstance->currentMessage->messageID);
		}

		return 1;
	}else{ //no more messages, stop drawing
		thisDialogInstance->currentMessage->nextMessage = NULL;
//		thisDialogInstance->currentMessage->eventID = 0;
		thisDialogInstance->speakingIndividualID = 0;
		toggleDrawDialogBox();
		return 0;
	}
}

int getEventFromCurrentMessage(){
	return thisDialogInstance->currentMessage->eventID;
}

void setSimpleDialogMessage(char * string){
	thisDialogInstance->currentMessage = malloc(sizeof(dialogMessage));
	strcpy(thisDialogInstance->currentMessage->message, string);
	strcpy(thisDialogInstance->currentMessage->parsedMessage, string);
	thisDialogInstance->currentMessage->numDialogDecision = 0;
	thisDialogInstance->currentMessage->numDialogDecisionsParsed = 0;
	thisDialogInstance->currentMessage->nextMessage = NULL;
	thisDialogInstance->currentMessage->eventID = 0;
}

void toggleDrawDialogBox(){
	thisDialogInstance->drawBox = (thisDialogInstance->drawBox+1)%2;
}

int setCurrentMessageByIndividualID(int individualID, int isNPCHostileTowardPlayer, int alreadyYielded){
	int i;

	for(i = 0; i < thisDialogInstance->MAX_INDIVIDUAL_DIALOG_REGISTRY; i++){
		if(thisDialogInstance->individualDialogRegistry[i] == NULL){
			return 0;
		}else if(thisDialogInstance->individualDialogRegistry[i]->individualID == individualID){
			if(isNPCHostileTowardPlayer){
				if(alreadyYielded){
					return setCurrentMessageByMessageID(WONT_YIELD_DIALOG_ID);
				}else{
					return setCurrentMessageByMessageID(YIELD_DIALOG_ID);
				}
			}else if(thisDialogInstance->individualDialogRegistry[i]->specialID != 0){
				return setCurrentMessageByMessageID(thisDialogInstance->individualDialogRegistry[i]->specialID);
			}else{
				return setCurrentMessageByMessageID(thisDialogInstance->individualDialogRegistry[i]->dialogID);
			}
		}
	}

	//dialog ID not found
	return 0;
}

int setNextMessageByID(int nextMessageID){
	int i;

	if(thisDialogInstance->currentMessage->nextMessageID != 0){
		cwrite("!!CANNOT SET nextMessage - currentMessage->nextMessage IS NOT NULL!!");
		return 0;
	}

	for (i = 0; i < thisDialogInstance->numDialogMessages; i++) {
		if (thisDialogInstance->dialogMessages[i]->messageID == nextMessageID) {
			setNextMessage(thisDialogInstance->dialogMessages[i]);
			return 1;
		}
	}

	return 0;
}

int setNextMessage(dialogMessage * nextMessage){
	thisDialogInstance->currentMessage->nextMessageID = nextMessage->messageID;
	thisDialogInstance->currentMessage->nextMessage = nextMessage;
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

int removeSpecialDialog(int individualID){
	int i = 0;
	individualDialog * tmpDialog;

	for(i = 0; i < thisDialogInstance->MAX_INDIVIDUAL_DIALOG_REGISTRY; i++){
		tmpDialog = thisDialogInstance->individualDialogRegistry[i];

		if(tmpDialog != NULL && tmpDialog->individualID == individualID ){
			tmpDialog->specialID = 0;
			return 1;
		}
	}

	return 0;
}

int setSpecialDialogId(int individualID, int specialID){
	int i = 0;
	individualDialog * tmpDialog;

	for(i = 0; i < thisDialogInstance->MAX_INDIVIDUAL_DIALOG_REGISTRY; i++){
		tmpDialog = thisDialogInstance->individualDialogRegistry[i];

		if(tmpDialog != NULL && tmpDialog->individualID == individualID ){
			tmpDialog->specialID = specialID;
			return 1;
		}
	}

	return 0;
}

int loadOrAddIndividualDialog(int individualID, int dialogID, int overrideValue){
	int i;

	if(dialogID == 0 ){
		return 0;
	}

	for(i = 0; i < thisDialogInstance->MAX_INDIVIDUAL_DIALOG_REGISTRY; i++){
		if(thisDialogInstance->individualDialogRegistry[i] == NULL){ //entry not found, create new entry
			individualDialog * thisDialogEntry = malloc(sizeof(individualDialog));
			thisDialogEntry->individualID = individualID;
			thisDialogEntry->dialogID = dialogID;
			thisDialogEntry->specialID = 0;

			thisDialogInstance->individualDialogRegistry[i] = thisDialogEntry;
			if(i+1 < thisDialogInstance->MAX_INDIVIDUAL_DIALOG_REGISTRY){
				thisDialogInstance->individualDialogRegistry[i+1] = NULL;
			}

			return dialogID;
		}else if(thisDialogInstance->individualDialogRegistry[i]->individualID == individualID){ //entry already exists
			if(overrideValue){
				thisDialogInstance->individualDialogRegistry[i]->dialogID = dialogID;
				return dialogID;
			}

			if(thisDialogInstance->individualDialogRegistry[i]->specialID != 0){
				return thisDialogInstance->individualDialogRegistry[i]->specialID;
			}
			return thisDialogInstance->individualDialogRegistry[i]->dialogID;
		}
	}

	cwrite("*MAX NPCs IN individualDialogRegistry!*");
	return -1;
}

void setIndividualDialog(int dialogID){
	int i;

	for(i = 0; i < thisDialogInstance->MAX_INDIVIDUAL_DIALOG_REGISTRY; i++){
		if(thisDialogInstance->individualDialogRegistry[i] != NULL && thisDialogInstance->individualDialogRegistry[i]->individualID == thisDialogInstance->speakingIndividualID){
			thisDialogInstance->individualDialogRegistry[i]->dialogID = dialogID;
			return;
		}
	}
}

dialogDecision * createDialogDecisionFromLine(char * line){
	int i;
	char * strtok_save_pointer;
	char * inserts;
	dialogDecision * newDialogDecision = malloc(sizeof(dialogDecision));

	newDialogDecision->MESSAGE_SIZE = 256;
	newDialogDecision->MAX_MESSAGE_INSERTS = 5;
	newDialogDecision->numMessageInserts = 0;

	char * value = strtok_r(line,";",&strtok_save_pointer);
	newDialogDecision->rootMessageID = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	strcpy(newDialogDecision->message, value);
	strcpy(newDialogDecision->processedMessage, newDialogDecision->message);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newDialogDecision->defaultDialogID = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newDialogDecision->displayEventID = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newDialogDecision->eventID = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newDialogDecision->numMessageInserts = atoi(value);

	inserts = strtok_r(NULL,";",&strtok_save_pointer);

	value = strtok(inserts, ",");
	for(i = 0; i < newDialogDecision->numMessageInserts; i++){
		char * tmpVal = malloc(sizeof(char) * 32);
		strcpy(tmpVal, value);

		//strip trailing \n
		int len = strlen(tmpVal) - 1;
		if(tmpVal[len] == '\n'){
			tmpVal[len] = '\0';
		}

		newDialogDecision->messageInserts[i] = tmpVal;

		if(i+1 < newDialogDecision->numMessageInserts){
			value = strtok(NULL, ",");
		}
	}

	return newDialogDecision;
}

dialogDecision * cloneDecisionDialog(dialogDecision * thisDecision){
	int i;
	dialogDecision * newDecision = malloc(sizeof(dialogDecision));

	newDecision->MAX_MESSAGE_INSERTS = thisDecision->MAX_MESSAGE_INSERTS;
	newDecision->MESSAGE_SIZE = thisDecision->MESSAGE_SIZE;
	newDecision->numMessageInserts = thisDecision->numMessageInserts;
	newDecision->defaultDialogID = thisDecision->defaultDialogID;
	newDecision->displayEventID = thisDecision->displayEventID;
	newDecision->eventID = thisDecision->eventID;
	newDecision->rootMessageID = thisDecision->rootMessageID;
	strcpy(newDecision->message,thisDecision->message);
	strcpy(newDecision->processedMessage, thisDecision->processedMessage);

	for(i = 0; i < newDecision->numMessageInserts; i++){
		char * tmpStr = malloc(sizeof(char) * 32);
		strcpy(tmpStr, thisDecision->messageInserts[i]);
		newDecision->messageInserts[i] = tmpStr;
	}

	//Figure out how targetMessage is used,
	newDecision->targetMessage = thisDecision->targetMessage;

	return newDecision;
}

dialogMessage * createDialogMessageFromLine(char * line){
	dialogMessage * newDialogMessage = malloc(sizeof(dialogMessage));
	int i;
	char * strtok_save_pointer;
	char * inserts;
	char * value = strtok_r(line,";",&strtok_save_pointer);

	newDialogMessage->MAX_MESSAGE_INSERTS = 5;
	newDialogMessage->MESSAGE_SIZE = 256;

	newDialogMessage->messageID = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	strcpy(newDialogMessage->message, value);
	strcpy(newDialogMessage->parsedMessage, newDialogMessage->message);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newDialogMessage->numDialogDecision = atoi(value);
	newDialogMessage->numDialogDecisionsParsed = 0;

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newDialogMessage->nextMessageID = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newDialogMessage->dialogCheckpoint = atoi(value);

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newDialogMessage->eventID = atoi(value);

	newDialogMessage->decisions[0] = NULL;

	value = strtok_r(NULL,";",&strtok_save_pointer);
	newDialogMessage->numMessageInserts = atoi(value);

	inserts = strtok_r(NULL,";",&strtok_save_pointer);

	value = strtok(inserts, ",");
	for(i = 0; i < newDialogMessage->numMessageInserts; i++){
		char * tmpVal = malloc(sizeof(char) * 32);
		strcpy(tmpVal, value);

		//strip trailing \n
		int len = strlen(tmpVal) - 1;
		if(tmpVal[len] == '\n'){
			tmpVal[len] = '\0';
		}

		newDialogMessage->messageInserts[i] = tmpVal;

		if(i+1 < newDialogMessage->numMessageInserts){
			value = strtok(NULL, ",");
		}
	}

	return newDialogMessage;
}

void destroyDialogDecision(dialogDecision * thisDecision){
	int i;

	for(i = 0; i < thisDecision->numMessageInserts; i++){
		free(thisDecision->messageInserts[i]);
	}

	free(thisDecision);
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
	FILE * FP = fopen(fullFileName, "r");
	int numMessages = 0, i, j, foundNode = 0, nextNotNull;
	char line[512];

	fgets(line, 512, FP);

	while(line[0] != '\n'){
		thisDialogInstance->dialogMessages[numMessages] = createDialogMessageFromLine(line);
		numMessages++;
		fgets(line, 512, FP);
	}

	thisDialogInstance->numDialogMessages = numMessages;

	//Populate DialogMessage's next DialogMessage
	for(i = 0; i < numMessages; i++){
		thisDialogInstance->dialogMessages[i]->nextMessage = findNextDialogMessage(thisDialogInstance->dialogMessages[i], thisDialogInstance->dialogMessages, numMessages);
	}

	int rootFound = 0, targetFound = 0;
	while(fgets(line, 512,FP)){
		dialogDecision * tmpDecision = createDialogDecisionFromLine(line);

		tmpDecision->targetMessage = NULL;
		for(i = 0; i < numMessages; i++){
			if(thisDialogInstance->dialogMessages[i]->messageID == tmpDecision->rootMessageID){
				rootFound = 1;
				addDecisionToDialogMessage(thisDialogInstance->dialogMessages[i], tmpDecision);
			}

			if(thisDialogInstance->dialogMessages[i]->messageID == tmpDecision->defaultDialogID){
				targetFound = 1;
				tmpDecision->targetMessage = thisDialogInstance->dialogMessages[i];
			}

			if(rootFound && targetFound){
				rootFound = 0;
				targetFound = 0;
				break;
			}
		}
	}
}
