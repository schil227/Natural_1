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

	//Mana: %d/%d => 12 bytes (short ints)
	char manaOut[12];
	sprintf(manaOut, "Mana: %d/%d", player->mana, player->totalMana);

	//Actions:%d => 20 bytes
	char actionOut[10];
	sprintf(actionOut, "Actions:%d",player->remainingActions);

	//Damage Spread:[%d-%d] => 21 bytes
	char damSpread[21];
	sprintf(damSpread, "Damage Spread:[%d-%d]",player->minDam, player->maxDam);

	//Movement Range:%d => 17 bytes
	char mvmtRng[17];
	sprintf(mvmtRng, "Movement Range:%d", player->mvmt);

//	HFONT hfont = CreateFont(textYStep, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, DEFAULT_QUALITY, 0, "Courier");
//	HFONT oldfont = (HFONT)SelectObject(hdc, hfont);

	TextOut(hdc, 10, 10, hpOut, strlen(hpOut));
	TextOut(hdc, 10, 10+textYStep, manaOut, strlen(manaOut));
	TextOut(hdc, 10, 10+textYStep*2, actionOut, strlen(actionOut));
	TextOut(hdc, 10, 10+textYStep*3, damSpread, strlen(damSpread));
	TextOut(hdc, 10, 10+textYStep*4, mvmtRng, strlen(mvmtRng));
	TextOut(hdc, 10, 120, "STATS:", strlen("STATS:"));
	TextOut(hdc, 17, 120+textYStep, "STR", 3);
	TextOut(hdc, 47, 120+textYStep, ":0", 2);
	TextOut(hdc, 17, 120+textYStep*2, "CON", 3);
	TextOut(hdc, 47, 120+textYStep*2, ":0", 2);
	TextOut(hdc, 17, 120+textYStep*3, "DEX", 3);
	TextOut(hdc, 47, 120+textYStep*3, ":0", 2);
	TextOut(hdc, 17, 120+textYStep*4, "INT", 3);
	TextOut(hdc, 47, 120+textYStep*4, ":0", 2);
	TextOut(hdc, 17, 120+textYStep*5, "WIS", 3);
	TextOut(hdc, 47, 120+textYStep*5, ":0", 2);
	TextOut(hdc, 17, 120+textYStep*6, "WIL", 3);
	TextOut(hdc, 47, 120+textYStep*6, ":0", 2);
	TextOut(hdc, 17, 120+textYStep*7, "CHR", 3);
	TextOut(hdc, 47, 120+textYStep*7, ":0", 2);

//	SelectObject(hdc, oldfont);
//	DeleteObject(hfont);
}
