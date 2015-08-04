/*
 * sidebar_window.c
 *
 *  Created on: Aug 2, 2015
 *      Author: Adrian
 */

#include<windows.h>
#include"./headers/individual_pub_methods.h"

void DrawSideBar(HWND hwnd, HDC hdc, RECT rec, individual * player){
	//paint background color
	int textYStep = 20;
	HBRUSH NewBrush = CreateSolidBrush(RGB(240,240,240));
	SelectObject(hdc, NewBrush);
	Rectangle(hdc, 0,0,rec.right, rec.bottom);
	DeleteObject(NewBrush);

	//HP: %d/%d => 10 bytes (short ints)
	char hpOut[10];
	sprintf(hpOut, "HP: %d/%d", player->hp, player->totalHP);

	//Actions:%d => 20 bytes
	char actionOut[10];
	sprintf(actionOut, "Actions:%d",player->remainingActions);

	//Damage Spread:[%d-%d] => 21 bytes
	char damSpread[21];
	sprintf(damSpread, "Damage Spread:[%d-%d]",player->minDam, player->maxDam);

	//Movement Range:%d => 17 bytes
	char mvmtRng[17];
	sprintf(mvmtRng, "Movement Range:%d", player->mvmt);

	TextOut(hdc, 10, 10, hpOut, strlen(hpOut));
	TextOut(hdc, 10, 10+textYStep, actionOut, strlen(actionOut));
	TextOut(hdc, 10, 10+textYStep*2, damSpread, strlen(damSpread));
	TextOut(hdc, 10, 10+textYStep*3, mvmtRng, strlen(mvmtRng));
	TextOut(hdc, 10, 100, "STATS:", strlen("STATS:"));
	TextOut(hdc, 17, 100+textYStep, "STR:0", 5);
	TextOut(hdc, 17, 100+textYStep*2, "CON:0", 5);
	TextOut(hdc, 17, 100+textYStep*3, "DEX:0", 5);
	TextOut(hdc, 17, 100+textYStep*4, "INT:0", 5);
	TextOut(hdc, 17, 100+textYStep*5, "WIS:0", 5);
	TextOut(hdc, 17, 100+textYStep*6, "WIL:0", 5);
	TextOut(hdc, 17, 100+textYStep*7, "CHR:0", 5);
}
