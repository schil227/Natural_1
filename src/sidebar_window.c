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

	//Gold: %d => 7 bytes
	char goldOut[8];
	sprintf(goldOut,"Gold: %d", player->gold);




//	HFONT hfont = CreateFont(textYStep, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, DEFAULT_QUALITY, 0, "Courier");
//	HFONT oldfont = (HFONT)SelectObject(hdc, hfont);

	char tmpNum[2];

	TextOut(hdc, 10, 10, hpOut, strlen(hpOut));
	TextOut(hdc, 10, 10+textYStep, manaOut, strlen(manaOut));
	TextOut(hdc, 10, 10+textYStep*2, actionOut, strlen(actionOut));
	TextOut(hdc, 10, 10+textYStep*3, damSpread, strlen(damSpread));
	TextOut(hdc, 10, 10+textYStep*4, mvmtRng, strlen(mvmtRng));
	TextOut(hdc, 10, 120, "STATS:", strlen("STATS:"));
	TextOut(hdc, 17, 120+textYStep, "STR:", 4);
	sprintf(tmpNum,"%d",player->STR);
	TextOut(hdc, player->STR >=0? 61 : 57, 120+textYStep, tmpNum, strlen(tmpNum));
	TextOut(hdc, 17, 120+textYStep*3, "DEX:", 4);
	sprintf(tmpNum,"%d",player->DEX);
	TextOut(hdc, player->DEX >=0? 61 : 57, 120+textYStep*2, tmpNum, strlen(tmpNum));
	TextOut(hdc, 17, 120+textYStep*2, "CON:", 4);
	sprintf(tmpNum,"%d",player->CON);
	TextOut(hdc, player->CON >=0? 61 : 57, 120+textYStep*3, tmpNum, strlen(tmpNum));
	TextOut(hdc, 17, 120+textYStep*4, "INT:", 4);
	sprintf(tmpNum,"%d",player->INT);
	TextOut(hdc, player->INT >=0? 61 : 57, 120+textYStep*4, tmpNum, strlen(tmpNum));
	TextOut(hdc, 17, 120+textYStep*5, "WIS:", 4);
	sprintf(tmpNum,"%d",player->WIS);
	TextOut(hdc, player->WIS >=0? 61 : 57, 120+textYStep*5, tmpNum, strlen(tmpNum));
	TextOut(hdc, 17, 120+textYStep*6, "WILL:", 5);
	sprintf(tmpNum,"%d",player->WILL);
	TextOut(hdc, player->WILL >=0? 61 : 57, 120+textYStep*6, tmpNum, strlen(tmpNum));
	TextOut(hdc, 17, 120+textYStep*7, "CHR:", 4);
	sprintf(tmpNum,"%d",player->CHR);
	TextOut(hdc, player->CHR >=0? 61 : 57, 120+textYStep*7, tmpNum, strlen(tmpNum));
	TextOut(hdc, 17, 120+textYStep*8, "LUCK:", 5);
	sprintf(tmpNum,"%d",player->LUCK);
	TextOut(hdc, player->LUCK >=0? 61 : 57, 120+textYStep*8, tmpNum, strlen(tmpNum));

	TextOut(hdc, 17, 120+textYStep*10, goldOut, strlen(goldOut));

//	SelectObject(hdc, oldfont);
//	DeleteObject(hfont);
}
