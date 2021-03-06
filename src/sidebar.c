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

	thisSidebarInstance->thisCharacter = malloc(sizeof(fixedCharacter));
	thisSidebarInstance->thisCharacter->x = x;
	thisSidebarInstance->thisCharacter->y = y;
	thisSidebarInstance->thisCharacter->fixedWidth = width;
	thisSidebarInstance->thisCharacter->fixedHeight = height;
	thisSidebarInstance->thisCharacter->fixedImageID = imageID;
	thisSidebarInstance->thisCharacter->fixedImage = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(imageID));
}

void drawThisSideBar(HDC hdc, HDC hdcBuffer, RECT * prc, individual * player){
	HDC hdcMem = CreateCompatibleDC(hdc);

	int x = prc->right - thisSidebarInstance->thisCharacter->fixedWidth;
	int y = 0;
	int width = thisSidebarInstance->thisCharacter->fixedWidth;
	int height = prc->bottom - 200;
	int totalFood = 0;

	SelectObject(hdcMem, thisSidebarInstance->thisCharacter->fixedImage);

	StretchBlt(hdcBuffer,
				 x, y,
				 width, height,
	 			 hdcMem,
	 			 0,0,
				 thisSidebarInstance->thisCharacter->fixedWidth, thisSidebarInstance->thisCharacter->fixedHeight,
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
	sprintf(manaOut, "Mana: %d/%d", player->mana, (getAttributeSum(player, "baseMana")+ getAttributeSum(player, "WILL") * 2));

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

	SIZE size;
	GetTextExtentPoint32(hdcBuffer, "Food: ", 6, &size);

	DrawText(hdcBuffer, "Food: ", 6, &rect, DT_SINGLELINE);
	rect.left += size.cx;

	char * foodStr[8];
	sprintf(foodStr, "%d", (int)player->food);
	GetTextExtentPoint32(hdcBuffer, foodStr, strlen(foodStr), &size);

	if(player->food > 300){
		SetTextColor(hdcBuffer, RGB(0, 162, 255));
	}else if(player->food >= 50 && player->food < 100 ){
		SetTextColor(hdcBuffer, RGB(255, 100, 0));
	}else if(player->food < 50){
		SetTextColor(hdcBuffer, RGB(255, 0, 0));
	}
	DrawText(hdcBuffer, foodStr, strlen(foodStr), &rect, DT_SINGLELINE);
	rect.left += size.cx;

	SetTextColor(hdcBuffer, RGB(255, 200, 0));

	totalFood = getTotalFood(player);

	foodStr[0] = '\0';
	sprintf(foodStr, "/%d", totalFood);
	DrawText(hdcBuffer, foodStr, strlen(foodStr), &rect, DT_SINGLELINE);

	rect.top += textYStep;
	rect.left = x + 10;

//	SelectObject(hdc, oldfont);
//	DeleteObject(hfont);

	SetTextColor(hdcBuffer, RGB(0, 0, 0));
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

int getSidebarWidth(){
	return thisSidebarInstance->thisCharacter->fixedWidth;
}
