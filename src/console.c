/*
 * console.c
 *
 *  Created on: Oct 5, 2015
 *      Author: Adrian
 */
#include"./headers/console_pub_methods.h"

console * thisConsole;

console * initConsole(int imageID, int x, int y, int width, int height){
	console *  thisNewConsole = malloc(sizeof(console));
	thisNewConsole->consoleCharacter = malloc(sizeof(character));

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
	thisNewConsole->consoleCharacter->image = malloc(sizeof(HBITMAP));
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

//	if(thisConsole->numMessages > 5){
//		messageNode * tmpMessage = thisConsole->oldestMessageNode->nextMessageNode;
//		free(tmpMessage->previousMessageNode);
//		tmpMessage->previousMessageNode = NULL;
////		free(thisConsole->oldestMessageNode);
//		thisConsole->oldestMessageNode = tmpMessage;
//	}

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
	drawConsoleText(hdcBuffer, prc);
}

int calcNumIndexes(char currentMessage[256], int lineLength, HDC hdcBuffer, int leftIndent){
	SIZE size;
	int result=2, rowLengthCounter=0, spaceIndex = 0, i;

	for (i = 0; i < strlen(currentMessage); i++) {
		GetTextExtentPoint32(hdcBuffer, &currentMessage[i], 1, &size);
		rowLengthCounter += size.cx;

		if (currentMessage[i] == ' ') {
			spaceIndex = i;
		}

		if(rowLengthCounter > lineLength){
			//account for indent
			rowLengthCounter = leftIndent;

			if (i - spaceIndex < 3) {
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

int * createSpaceIndexArr(char currentMessage[256], int breakIndexArrSize, HDC hdcBuffer, int leftIndent) {
	int rowLengthCounter = 0, spaceIndex=0, spaceIndexArrIndex = 1, i;
	int * spaceIndexArr = malloc(sizeof(int) * breakIndexArrSize);
	spaceIndexArr[0] = 0;
	SIZE size;

	for (i = 0; i < strlen(currentMessage); i++) {

		GetTextExtentPoint32(hdcBuffer, &currentMessage[i], 1, &size);
		rowLengthCounter += size.cx;

		if (currentMessage[i] == ' ') {
			spaceIndex = i;
		}

		if (rowLengthCounter > thisConsole->rowLength) {
			rowLengthCounter = leftIndent; //not 0, account for indent
			if (i - spaceIndex < 3) {
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

void drawConsoleText(HDC hdcBuffer, RECT * prc){
	int linesAvailable = thisConsole->numRows;
	int left = prc->right - prc->right*0.95;
	int leftIndent = prc->right - prc->right*0.9;

	messageNode * currentMessageNode = thisConsole->currentMessageNode;
	RECT textBoxRect;
		textBoxRect.bottom = prc->bottom;
		textBoxRect.top =  prc->bottom - 30;
		textBoxRect.left = left;
		textBoxRect.right = prc->right;

	SetTextColor(hdcBuffer, RGB(255, 200, 0));
	SetBkMode(hdcBuffer, TRANSPARENT);

	while (linesAvailable > 0 && currentMessageNode != NULL) {
		int index, sizeOfSpaceIndexArr, *spaceIndexArr;
		char currentMessage[256];

		strcpy(currentMessage, currentMessageNode->message);
		sizeOfSpaceIndexArr = calcNumIndexes(currentMessage, thisConsole->rowLength, hdcBuffer, leftIndent);
		spaceIndexArr = createSpaceIndexArr(currentMessage,sizeOfSpaceIndexArr, hdcBuffer, leftIndent);



		textBoxRect.left = leftIndent; //make the indent

		for (index = sizeOfSpaceIndexArr-1; index > 0; index--) {

			if (index - 1 == 0) {
				textBoxRect.left = left;
			}

			if(linesAvailable > 0){
				//create the substring
				int numCharactersToIndex = spaceIndexArr[index] - spaceIndexArr[index - 1];

				if(index != sizeOfSpaceIndexArr-1 && currentMessage[spaceIndexArr[index]] != ' '){
					numCharactersToIndex++;
				}

				char messageSubLine[numCharactersToIndex];
				strncpy(messageSubLine, currentMessage + spaceIndexArr[index - 1], numCharactersToIndex);

				if(index != sizeOfSpaceIndexArr-1 && currentMessage[spaceIndexArr[index]] != ' '){
					messageSubLine[numCharactersToIndex-1] = '-';
				}

				messageSubLine[numCharactersToIndex] = '\0';

				//draw the substring to the window
				DrawText(hdcBuffer, messageSubLine, -1, &textBoxRect, DT_SINGLELINE);

				//move the drawing rectangle up 15 pixels
				textBoxRect.top = textBoxRect.top - 15;
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

	char* missedStr;
	if(attackRoll +1 == targetAC){
		missedStr = appendStrings(individualName, " just barely missed!\n");

	}else if(attackRoll > 6){
		missedStr = appendStrings(individualName, " missed!\n");

	}else if(attackRoll <= 6 && attackRoll > 3){
		char* tmp1 = appendStrings(individualName, " wiffed trying to hit ");
		char* tmp2 = appendStrings(targetName, "!\n");
		missedStr = appendStrings(tmp1,tmp2);
		free(tmp1);
		free(tmp2);
	}else{
		missedStr = appendStrings(individualName, " nearly fell down trying to attack.\n");

	}
	cwrite(missedStr);
	free(missedStr);
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

	char* hitStr;
	char takeOut[20]; //" takes %d damage!\n"

	if(damageUpperPercentile(damage, maxDam, 20)){
		char* highDam = appendStrings(individualName, " executes a brutal strike!\n");
		cwrite(highDam);
		free(highDam);
	}

	sprintf(takeOut, " takes %d damage!\n", damage);
	hitStr = appendStrings(targetName, takeOut);

	cwrite(hitStr);
	free(hitStr);
}

void sendDeathDialog(char* name, char* killer){
	char* deathStr;

	deathStr = appendStrings(name, " was slain by ");
	deathStr = appendStrings(deathStr, killer);
	deathStr = appendStrings(deathStr, "!\n");

	cwrite(deathStr);
	free(deathStr);
}

void cwrite(char* text){
	appendNewMessageNode(text);
//	AppendText(eConsole,text);
}
