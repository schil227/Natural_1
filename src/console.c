/*
 * console.c
 *
 *  Created on: Oct 5, 2015
 *      Author: Adrian
 */
#include"./headers/console_pub_methods.h"

static console * thisConsole;

console * initConsole(int imageID, int x, int y, int width, int height){
	console *  thisNewConsole = malloc(sizeof(console));
	thisNewConsole->consoleCharacter = malloc(sizeof(character));
	thisNewConsole->numMessages = 0;

	thisNewConsole->newestMessageNode = malloc(sizeof(messageNode));
	thisNewConsole->currentMessageNode = thisNewConsole->newestMessageNode;
	thisNewConsole->oldestMessageNode = thisNewConsole->newestMessageNode;
	thisNewConsole->newestMessageNode->nextMessageNode = NULL;
	thisNewConsole->newestMessageNode->previousMessageNode = NULL;
	strcpy(thisNewConsole->newestMessageNode->message, "");

	thisNewConsole->consoleCharacter->x = x;
	thisNewConsole->consoleCharacter->y = y;
	thisNewConsole->consoleCharacter->width = width;
	thisNewConsole->consoleCharacter->height = height;
	thisNewConsole->consoleCharacter->imageID = imageID;
	thisNewConsole->consoleCharacter->image = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(imageID));

	thisNewConsole->rowLength = 40;
	thisNewConsole->numRows = 11;
 return thisNewConsole;
}

void initThisConsole(int imageID, int x, int y, int width, int height){
	thisConsole = initConsole(imageID,x,y,width,height);
	printf("thisConsole->x:%d\n", thisConsole->consoleCharacter->x);
}

void destroyConsoleInstance(){
	messageNode * currentNode = thisConsole->currentMessageNode;
	if (currentNode != NULL) {
		messageNode * tmpNode = currentNode->nextMessageNode;
		while (tmpNode != NULL) {
			messageNode * swapNode = tmpNode->nextMessageNode;
			free(tmpNode);
			tmpNode = swapNode;
		}

		tmpNode = currentNode->previousMessageNode;
		while (tmpNode != NULL) {
			messageNode * swapNode = tmpNode->previousMessageNode;
			free(tmpNode);
			tmpNode = swapNode;
		}
		free(currentNode);
	}

	destroyCharacter(thisConsole->consoleCharacter);
	free(thisConsole);

}

void appendNewMessageNode(char * message){
	messageNode * thisMessage = malloc(sizeof(messageNode));
	strcpy(thisMessage->message,message);
	thisMessage->nextMessageNode = NULL;

	(thisConsole->newestMessageNode)->nextMessageNode = thisMessage;
	thisMessage->previousMessageNode = thisConsole->newestMessageNode;
	if(thisConsole->currentMessageNode == thisConsole->newestMessageNode){
			thisConsole->currentMessageNode = thisMessage;
	}

	thisConsole->newestMessageNode = thisMessage;
	thisConsole->numMessages++;

	printf("appended message #%d\n",thisConsole->numMessages);

}

void clearMessages(){
	messageNode * currentNode = thisConsole->currentMessageNode;
	if (currentNode != NULL) {
		messageNode * tmpNode = currentNode->nextMessageNode;
		while (tmpNode != NULL) {
			messageNode * swapNode = tmpNode->nextMessageNode;
			free(tmpNode);
			tmpNode = swapNode;
		}

		tmpNode = currentNode->previousMessageNode;
		while (tmpNode != NULL) {
			messageNode * swapNode = tmpNode->previousMessageNode;
			free(tmpNode);
			tmpNode = swapNode;
		}
		free(currentNode);
	}

	thisConsole->newestMessageNode = malloc(sizeof(messageNode));
	thisConsole->currentMessageNode = thisConsole->newestMessageNode;
	thisConsole->oldestMessageNode = thisConsole->newestMessageNode;
	thisConsole->newestMessageNode->nextMessageNode = NULL;
	thisConsole->newestMessageNode->previousMessageNode = NULL;
	strcpy(thisConsole->newestMessageNode->message, "");
	thisConsole->numMessages = 0;

}

void drawThisConsole(HDC hdc, HDC hdcBuffer, RECT * prc){
	HDC hdcMem = CreateCompatibleDC(hdc);

	SelectObject(hdcMem, thisConsole->consoleCharacter->image);

	StretchBlt(hdcBuffer,
	 			 0,prc->bottom - 200,
	 			 prc->right,200,
	 			 hdcMem,
	 			 0,0,
				 thisConsole->consoleCharacter->width, thisConsole->consoleCharacter->height,
	 			 SRCCOPY);

	DeleteDC(hdcMem);

	thisConsole->rowLength = prc->right - (prc->right * 0.10);

	RECT textBoxRect;
		textBoxRect.bottom = prc->bottom;
		textBoxRect.top =  prc->bottom - 30;
		textBoxRect.left = prc->right - prc->right*0.95;
		textBoxRect.right = prc->right;

	drawConsoleText(hdcBuffer, &textBoxRect, thisConsole->currentMessageNode, thisConsole->numRows, thisConsole->rowLength);
}

int calcNumIndexes(char currentMessage[256], int lineLength, HDC hdcBuffer, int leftIndent){
	SIZE size;
	int result=2, rowLengthCounter=0, spaceIndex = 0, i;

	for (i = 0; i < strlen(currentMessage); i++) {
		GetTextExtentPoint32(hdcBuffer, &currentMessage[i], 1, &size);
		rowLengthCounter += size.cx;

		if (currentMessage[i] == ' ' || currentMessage[i] == '&') {
			spaceIndex = i;
		}

		if(rowLengthCounter > lineLength || currentMessage[i] == '&'){
			//account for indent
			rowLengthCounter = leftIndent;

			if (i - spaceIndex < 3 && currentMessage[i] != '&') {
				//need to account for the added characters after the space break on next line
				int j;

				for(j = i; j > spaceIndex; j--){
					GetTextExtentPoint32(hdcBuffer, &currentMessage[j], 1, &size);
					rowLengthCounter += size.cx;
				}
			}

			result++;
		}

	}
	return result;
}

int * createSpaceIndexArr(char currentMessage[256], int breakIndexArrSize, int rowlength, HDC hdcBuffer, int leftIndent) {
	int rowLengthCounter = 0, spaceIndex=0, spaceIndexArrIndex = 1, i;
	int * spaceIndexArr = malloc(sizeof(int) * breakIndexArrSize);
	spaceIndexArr[0] = 0;
	SIZE size;

	for (i = 0; i < strlen(currentMessage); i++) {

		GetTextExtentPoint32(hdcBuffer, &currentMessage[i], 1, &size);
		rowLengthCounter += size.cx;

		if (currentMessage[i] == ' ' || currentMessage[i] == '&') {
			spaceIndex = i;
		}

		if (rowLengthCounter > rowlength || currentMessage[i] == '&') {
			rowLengthCounter = leftIndent; //not 0, account for indent
			if (i - spaceIndex < 3  && currentMessage[i] != '&') {
				spaceIndexArr[spaceIndexArrIndex] = spaceIndex;

				//need to account for the added characters after the space break on next line
				int j;
				for(j = i; j > spaceIndex; j--){
					GetTextExtentPoint32(hdcBuffer, &currentMessage[j], 1, &size);
					rowLengthCounter += size.cx;
				}


			} else {
				spaceIndexArr[spaceIndexArrIndex] = i;
			}
			spaceIndexArrIndex++;
		}

	}

	spaceIndexArr[spaceIndexArrIndex] = i;
	spaceIndexArrIndex++;

	return spaceIndexArr;
}

void drawConsoleText(HDC hdcBuffer, RECT * textBoxRect, messageNode * currentMessage, int numRows, int rowLength){
	int linesAvailable = numRows;//thisConsole->numRows;
	int left = textBoxRect->left;
	int leftIndent = textBoxRect->right - textBoxRect->right*0.9;

	messageNode * currentMessageNode = currentMessage; //thisConsole->currentMessageNode;

	SetTextColor(hdcBuffer, RGB(255, 200, 0));
	SetBkMode(hdcBuffer, TRANSPARENT);

	while (linesAvailable > 0 && currentMessageNode != NULL) {
		int index, sizeOfSpaceIndexArr, *spaceIndexArr;
		char theMessage[256];

		strcpy(theMessage, currentMessageNode->message);
		sizeOfSpaceIndexArr = calcNumIndexes(theMessage, rowLength, hdcBuffer, leftIndent);
		spaceIndexArr = createSpaceIndexArr(theMessage,sizeOfSpaceIndexArr, rowLength, hdcBuffer, leftIndent);

		textBoxRect->left = leftIndent; //make the indent

		for (index = sizeOfSpaceIndexArr-1; index > 0; index--) {

			if (index - 1 == 0) {
				textBoxRect->left = left;
			}

			if(linesAvailable > 0){
				//create the substring
				int numCharactersToIndex = spaceIndexArr[index] - spaceIndexArr[index - 1];

				if(index != sizeOfSpaceIndexArr-1 && (theMessage[spaceIndexArr[index]] != ' ' && theMessage[spaceIndexArr[index]] != '&')){
					numCharactersToIndex++;
				}

				char messageSubLine[numCharactersToIndex];
				strncpy(messageSubLine, theMessage + spaceIndexArr[index - 1], numCharactersToIndex);

				if(index != sizeOfSpaceIndexArr-1 && (theMessage[spaceIndexArr[index]] != ' ' && theMessage[spaceIndexArr[index]] != '&')){
					messageSubLine[numCharactersToIndex-1] = '-';
				}

				messageSubLine[numCharactersToIndex] = '\0';

				//draw the substring to the window
				if(!DrawText(hdcBuffer, messageSubLine, -1, textBoxRect, DT_SINGLELINE)){
					printf("drawtextFailed!");
				}

				//move the drawing rectangle up 15 pixels
				textBoxRect->top = textBoxRect->top - 15;
			}

			linesAvailable--;
		}

		currentMessageNode = currentMessageNode->previousMessageNode;
		free(spaceIndexArr);

	}
}

void AppendText(HWND hwnd, TCHAR *newText)
{
    // get edit control from dialog
    HWND hwndOutput = hwnd;

    // get the current selection
    DWORD StartPos, EndPos;
    SendMessage( hwndOutput, EM_GETSEL, &StartPos, &EndPos);

    // move the caret to the end of the text
    int outLength = GetWindowTextLength( hwndOutput );
    SendMessage( hwndOutput, EM_SETSEL, outLength, outLength );

    // insert the text at the new caret position
    SendMessage( hwndOutput, EM_REPLACESEL, TRUE, newText);

    // restore the previous selection
    SendMessage( hwndOutput, EM_SETSEL, StartPos, EndPos );
}

char* appendStrings(char* str1, char* str2){
	char* toReturn = (char *)malloc(strlen(str1) + strlen(str2)+3);
	strcpy(toReturn,str1);
	strcat(toReturn,str2);
	return toReturn;
}

void sendMissedDialog(char* individualName, char* targetName, int attackRoll, int targetAC){

	char missedStr[256];

	strcpy(missedStr, individualName);

	if(attackRoll +1 == targetAC){
		strcat(missedStr, " just barely missed!\n");

	}else if(attackRoll > 6){
		strcat(missedStr, " missed!\n");

	}else if(attackRoll <= 6 && attackRoll > 3){
		strcat(missedStr, " wiffed trying to hit ");
		strcat(missedStr, targetName);
		strcat(missedStr, "!\n");
	}else{
		strcat(missedStr, " nearly fell down trying to attack.\n");

	}
	cwrite(missedStr);
}

int calcUpperPercentileThreshold(int num, int topPercentile){
	return ((num*100)*(100-topPercentile))/10000;
}

int damageUpperPercentile(int damage, int maxDam, int nthP){
	if(damage == maxDam || damage > calcUpperPercentileThreshold(maxDam, nthP)){
		return 1;
	}else{
		return 0;
	}
}

void sendHitDialog(char* individualName, char* targetName, int maxDam, int damage){

	char hitStr[256];
	char takeOut[32]; //" takes %d damage!\n"

	if(damageUpperPercentile(damage, maxDam, 20)){
		char highDam[256];

		strcpy(highDam, individualName);
		strcat(highDam, " executes a brutal strike!\n");
		cwrite(highDam);
	}

	if(damage == 0){
		strcpy(hitStr, targetName);
		strcat(hitStr, " deflected the blow\n");
	}else{
		sprintf(takeOut, " takes %d damage!\n", damage);
		strcpy(hitStr, targetName);
		strcat(hitStr, takeOut);
	}

	cwrite(hitStr);
}

void sendDeathDialog(char* name, char* killer){
	char deathStr[256];

	strcpy(deathStr, name);
	strcat(deathStr, " was slain by ");
	strcat(deathStr, killer);
	strcat(deathStr, "!\n");

	cwrite(deathStr);
}

void cwrite(char* text){
	appendNewMessageNode(text);
//	AppendText(eConsole,text);
}
