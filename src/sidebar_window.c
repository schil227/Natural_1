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

	char hpOut[32];
	sprintf(hpOut, "HP: %d/%d", getAttributeSum(player, "health"), (getAttributeSum(player, "totalHealth") + getAttributeSum(player, "CON") * 2));

	char manaOut[32];
	sprintf(manaOut, "Mana: %d/%d", getAttributeSum(player, "mana"), (getAttributeSum(player, "totalMana")+ getAttributeSum(player, "WILL") * 2));

	char actionOut[16];
	sprintf(actionOut, "Actions:%d",player->remainingActions);

	char damSpread[32];
	sprintf(damSpread, "Damage Spread:[%d-%d]", calcMinDam(player), calcMaxDam(player));

	char mvmtRng[32];
	sprintf(mvmtRng, "Movement Range:%d", getAttributeSum(player, "mvmt"));

	char goldOut[16];
	sprintf(goldOut,"Gold: %d", player->gold);




//	HFONT hfont = CreateFont(textYStep, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, DEFAULT_QUALITY, 0, "Courier");
//	HFONT oldfont = (HFONT)SelectObject(hdc, hfont);

	char tmpNum[3];
	int tmpValue;

	TextOut(hdc, 10, 10, hpOut, strlen(hpOut));
	TextOut(hdc, 10, 10+textYStep, manaOut, strlen(manaOut));
	TextOut(hdc, 10, 10+textYStep*2, actionOut, strlen(actionOut));
	TextOut(hdc, 10, 10+textYStep*3, damSpread, strlen(damSpread));
	TextOut(hdc, 10, 10+textYStep*4, mvmtRng, strlen(mvmtRng));
	TextOut(hdc, 10, 120, "STATS:", strlen("STATS:"));

	tmpValue = getAttributeSum(player, "STR");
	TextOut(hdc, 17, 120+textYStep, "STR:", 4);
	sprintf(tmpNum,"%d",tmpValue);
	TextOut(hdc, tmpValue >=0? 61 : 57, 120+textYStep, tmpNum, strlen(tmpNum));

	tmpValue = getAttributeSum(player, "DEX");
	TextOut(hdc, 17, 120+textYStep*2, "DEX:", 4);
	sprintf(tmpNum,"%d",tmpValue);
	TextOut(hdc, tmpValue >=0? 61 : 57, 120+textYStep*2, tmpNum, strlen(tmpNum));

	tmpValue = getAttributeSum(player, "CON");
	TextOut(hdc, 17, 120+textYStep*3, "CON:", 4);
	sprintf(tmpNum,"%d",tmpValue);
	TextOut(hdc, tmpValue >=0? 61 : 57, 120+textYStep*3, tmpNum, strlen(tmpNum));

	tmpValue = getAttributeSum(player, "INT");
	TextOut(hdc, 17, 120+textYStep*4, "INT:", 4);
	sprintf(tmpNum,"%d",tmpValue);
	TextOut(hdc, tmpValue >=0? 61 : 57, 120+textYStep*4, tmpNum, strlen(tmpNum));

	tmpValue = getAttributeSum(player, "WIS");
	TextOut(hdc, 17, 120+textYStep*5, "WIS:", 4);
	sprintf(tmpNum,"%d",tmpValue);
	TextOut(hdc, tmpValue >=0? 61 : 57, 120+textYStep*5, tmpNum, strlen(tmpNum));

	tmpValue = getAttributeSum(player, "WILL");
	TextOut(hdc, 17, 120+textYStep*6, "WILL:", 5);
	sprintf(tmpNum,"%d",tmpValue);
	TextOut(hdc,tmpValue >=0? 61 : 57, 120+textYStep*6, tmpNum, strlen(tmpNum));

	tmpValue = getAttributeSum(player, "CHR");
	TextOut(hdc, 17, 120+textYStep*7, "CHR:", 4);
	sprintf(tmpNum,"%d",tmpValue);
	TextOut(hdc, tmpValue >=0? 61 : 57, 120+textYStep*7, tmpNum, strlen(tmpNum));

	tmpValue = getAttributeSum(player, "LUCK");
	TextOut(hdc, 17, 120+textYStep*8, "LUCK:", 5);
	sprintf(tmpNum,"%d",tmpValue);
	TextOut(hdc, tmpValue >=0? 61 : 57, 120+textYStep*8, tmpNum, strlen(tmpNum));

	TextOut(hdc, 17, 120+textYStep*10, goldOut, strlen(goldOut));

//	SelectObject(hdc, oldfont);
//	DeleteObject(hfont);
}
