/*
 * pause.c
 *
 *  Created on: May 7, 2017
 *      Author: Adrian
 */
#include "./headers/pause_pub_methods.h"

static pauseViewContainer * thisPauseView;

initPauseView(int pauseViewWindowID){
	thisPauseView = malloc(sizeof(pauseViewContainer));
	thisPauseView->pauseWindow = createCharacter(pauseViewWindowID, RGB(255,0,255), 0, 0);
	thisPauseView->isPaused = 0;
}

int isPaused(){
	return thisPauseView->isPaused;
}

void togglePaused(){
	thisPauseView->isPaused = (thisPauseView->isPaused+1) % 2;
}

void drawPauseWindow(HDC hdc, HDC hdcBuffer, RECT * rect){
	int x = (rect->right/2.0) - (thisPauseView->pauseWindow->fixedWidth / 2.0);
	int y = (rect->bottom/2.0) - (thisPauseView->pauseWindow->fixedHeight / 2.0);

	drawUnboundCharacterByPixels(hdc, hdcBuffer, x, y, thisPauseView->pauseWindow);
}
