/*
 * sidebar.c
 *
 *  Created on: Jun 11, 2016
 *      Author: Adrian
 */

#include "./headers/sidebar_pub_methods.h"

static sidebarInstance * thisSidebarInstance;

void initSidebarInstance(int imageID, int x, int y, int width, int height){
	thisSidebarInstance = malloc(sizeof(sidebarInstance));

	thisSidebarInstance->thisCharacter = malloc(sizeof(character));
	thisSidebarInstance->thisCharacter->x = x;
	thisSidebarInstance->thisCharacter->y = y;
	thisSidebarInstance->thisCharacter->width = width;
	thisSidebarInstance->thisCharacter->height = height;
	thisSidebarInstance->thisCharacter->imageID = imageID;
	thisSidebarInstance->thisCharacter->image = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(imageID));
}

void drawThisSideBar(HDC hdc, HDC hdcBuffer, RECT * prc, individual * player){
	HDC hdcMem = CreateCompatibleDC(hdc);

	int x = prc->right - thisSidebarInstance->thisCharacter->width;
	int y = 0;
	int width = thisSidebarInstance->thisCharacter->width;
	int height = prc->bottom - 200;


	SelectObject(hdcMem, thisSidebarInstance->thisCharacter->image);

	StretchBlt(hdcBuffer,
				 x, y,
				 width, height,
	 			 hdcMem,
	 			 0,0,
				 thisSidebarInstance->thisCharacter->width, thisSidebarInstance->thisCharacter->height,
	 			 SRCCOPY);

	DeleteDC(hdcMem);

	int textYStep = 20;
//	HBRUSH NewBrush = CreateSolidBrush(RGB(240,240,240));
//	SelectObject(hdc, NewBrush);
//	Rectangle(hdc, prc->right - thisSidebarInstance->thisCharacter->width, 0,thisSidebarInstance->thisCharacter->width, prc->bottom - 200);
//	DeleteObject(NewBrush);

	SetTextColor(hdcBuffer, RGB(255, 200, 0));
	SetBkMode(hdcBuffer, TRANSPARENT);

	char hpOut[32];
	sprintf(hpOut, "HP: %d/%d", player->hp, (getAttributeSum(player, "baseHP") + getAttributeSum(player, "CON") * 2));

	char manaOut[32];
	sprintf(manaOut, "Mana: %d/%d", getAttributeSum(player, "mana"), (getAttributeSum(player, "baseMana")+ getAttributeSum(player, "WILL") * 2));

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
	RECT rect;
	rect.left = x + 10;
	rect.top = 10;
	rect.right = x + width;
	rect.bottom = 10 + height;

	DrawText(hdcBuffer, hpOut, strlen(hpOut), &rect, DT_SINGLELINE);
	rect.top += textYStep;
	DrawText(hdcBuffer, manaOut, strlen(manaOut), &rect, DT_SINGLELINE);
	rect.top += textYStep;
	DrawText(hdcBuffer, actionOut, strlen(actionOut), &rect, DT_SINGLELINE);
	rect.top += textYStep;
	DrawText(hdcBuffer, damSpread, strlen(damSpread), &rect, DT_SINGLELINE);
	rect.top += textYStep;
	DrawText(hdcBuffer, mvmtRng, strlen(mvmtRng), &rect, DT_SINGLELINE);
	rect.top += 30;
	DrawText(hdcBuffer, "STATS:", 6, &rect, DT_SINGLELINE);
	rect.top += textYStep;

	rect.left += 7;
	printStat(hdcBuffer, &rect, "STR", "STR:", player, textYStep);
	printStat(hdcBuffer, &rect, "DEX", "DEX", player, textYStep);
	printStat(hdcBuffer, &rect, "CON", "CON:", player, textYStep);
	printStat(hdcBuffer, &rect, "INT", "INT:", player, textYStep);
	printStat(hdcBuffer, &rect, "WIS", "WIS:", player, textYStep);
	printStat(hdcBuffer, &rect, "WILL", "WILL:", player, textYStep);
	printStat(hdcBuffer, &rect, "CHR", "CHR:", player, textYStep);
	printStat(hdcBuffer, &rect, "LUCK", "LUCK:", player, textYStep);

	rect.left -= 7;
	rect.top += textYStep;

	DrawText(hdcBuffer, goldOut, strlen(goldOut), &rect, DT_SINGLELINE);
	rect.top += textYStep;
//	SelectObject(hdc, oldfont);
//	DeleteObject(hfont);
}


void printStat(HDC hdcBuffer, RECT * box, char * stat, char * statText, individual * player, int textYStep){
	int tmpValue = 0, boxShift = 0;
	char tmpNum[3];

	tmpValue = getAttributeSum(player, stat);
	DrawText(hdcBuffer, statText, strlen(statText), box, DT_SINGLELINE);
	sprintf(tmpNum,"%d",tmpValue);

	if(tmpValue >= 0){
		boxShift = 46;
	}else{
		boxShift = 42;
	}

	box->left += boxShift;

	DrawText(hdcBuffer, tmpNum, strlen(tmpNum), box, DT_SINGLELINE);

	box->left -= boxShift;
	box->top += textYStep;
}
