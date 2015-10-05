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
	thisNewConsole->newestMessageNode->nextMessageNode = NULL;
	thisNewConsole->newestMessageNode->previousMessageNode = NULL;

	thisNewConsole->consoleCharacter->x = x;
	thisNewConsole->consoleCharacter->y = y;
	thisNewConsole->consoleCharacter->width = width;
	thisNewConsole->consoleCharacter->height = height;
	thisNewConsole->consoleCharacter->imageID = imageID;
	thisNewConsole->consoleCharacter->image = malloc(sizeof(HBITMAP));
	thisNewConsole->consoleCharacter->image = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(imageID));

	thisNewConsole->rowLength = 40;
	thisNewConsole->numRows = 10;
 return thisNewConsole;
}

void initThisConsole(int imageID, int x, int y, int width, int height){
	thisConsole = initConsole(imageID,x,y,width,height);
	printf("thisConsole->x:%d\n", thisConsole->consoleCharacter->x);
}

void appendNewMessageNode(char * message){
	messageNode * thisMessage = malloc(sizeof(messageNode));
	strcpy(thisMessage->message,message);
	thisMessage->nextMessageNode = NULL;

	thisConsole->newestMessageNode->nextMessageNode = thisMessage;
	thisMessage->previousMessageNode = thisConsole->newestMessageNode;
	if(thisConsole->currentMessageNode == thisConsole->newestMessageNode){
			thisConsole->currentMessageNode = thisMessage;
	}

	thisConsole->newestMessageNode = thisMessage;
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
	drawConsoleText(hdcBuffer, prc);
}

void drawConsoleText(HDC hdcBuffer, RECT * prc){
	int linesAvailable = thisConsole->numRows;
	messageNode * currentMessageNode = thisConsole->currentMessageNode;

	SetTextColor(hdcBuffer, RGB(255, 200, 0));
	SetBkMode(hdcBuffer, TRANSPARENT);

	RECT textRect;
		textRect.bottom = prc->bottom;
		textRect.top =  prc->bottom - 30;
		textRect.left = 50;
		textRect.right = prc->right;

	while (linesAvailable > 0 && currentMessageNode != NULL) {
		int breakIndex[10], counter = 0, spaceIndex, arrSize = 0, i;
		char currentMessage[256];
		strcpy(currentMessage, currentMessageNode->message);

		for (i = 0; i < strlen(currentMessage); i++) {

			counter++;

			if (currentMessage[i] == ' ') {
				spaceIndex = i;
			}

			if (counter > thisConsole->rowLength) {
				counter = 0;
				breakIndex[arrSize] = spaceIndex;
				arrSize++;
			}

		}



		if (arrSize == 0) { // beginning to end of string, no newlines
			DrawText(hdcBuffer, currentMessage, -1, &textRect, DT_SINGLELINE);

			linesAvailable--;
			textRect.top = textRect.top - 15;
		}else{
			textRect.left = 60; //make the indent
			for (i = arrSize; i >= 0; i--) {
				if (i == arrSize) { //last element, to end of string
					int numCharactersToEnd = strlen(currentMessage) - breakIndex[i-1];
					char tmp[numCharactersToEnd];
					strncpy(tmp, currentMessage+breakIndex[i-1],numCharactersToEnd);
					tmp[numCharactersToEnd] = '\0';

					DrawText(hdcBuffer, tmp, -1, &textRect, DT_SINGLELINE);
				} else if (i == 0) { //beginning to first index
					textRect.left = 50;
					int numCharactersToIndex = breakIndex[i];
					char tmp[numCharactersToIndex];
					strncpy(tmp,currentMessage,numCharactersToIndex);
					tmp[numCharactersToIndex] = '\0';

					DrawText(hdcBuffer, tmp, -1, &textRect, DT_SINGLELINE);
				} else {
					int numCharactersToIndex = breakIndex[i] - breakIndex[i-1];
					char tmp[numCharactersToIndex];
					strncpy(tmp,currentMessage+breakIndex[i-1], numCharactersToIndex);
					tmp[numCharactersToIndex] = '\0';

					DrawText(hdcBuffer, tmp, -1, &textRect, DT_SINGLELINE);
				}
				linesAvailable--;
				textRect.top = textRect.top - 15;
			}


		}

		currentMessageNode = currentMessageNode->previousMessageNode;

	}

	free(currentMessageNode);
}



