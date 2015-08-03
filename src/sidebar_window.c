/*
 * sidebar_window.c
 *
 *  Created on: Aug 2, 2015
 *      Author: Adrian
 */

#include<windows.h>
#include"./headers/individual_pub_methods.h"

void DrawSideBar(HWND hwnd, HDC hdc, RECT rec, individual * player){
	HBRUSH NewBrush = CreateSolidBrush(RGB(240,240,240));
//	SelectObject(hdc, NewBrush);
	char hpOut[10]; //HP: %d/%d => 10 bytes (short ints)
	sprintf(hpOut, "HP: %d/%d", player->hp, player->totalHP);

//	Rectangle(hdc, 0,0,rec.right, rec.bottom);

//	DeleteObject(NewBrush);

	TextOut(hdc, 10, 10, hpOut, strlen(hpOut));


}
