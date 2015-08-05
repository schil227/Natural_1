/*
 * console_window.c
 *
 *  Created on: Jul 31, 2015
 *      Author: Adrian
 */

#include"./headers/console_window_pub_methods.h"

HWND eConsole = NULL;

LRESULT CALLBACK ConsWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch(msg)
	{
		case WM_CREATE:
		{
			RECT rec;
			GetClientRect(hwnd,&rec);

			eConsole = CreateWindowEx(0, "EDIT", "",
				WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_READONLY | ES_MULTILINE | ES_AUTOVSCROLL, //ES_READONLY
				0, 0, rec.right, rec.bottom, hwnd, NULL, GetModuleHandle(NULL), NULL);

			if(eConsole == NULL)
				MessageBox(hwnd, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);

			SendMessage(eConsole, WM_SETFONT, DEFAULT_GUI_FONT, MAKELPARAM(FALSE, 0));

			SetWindowText(eConsole, TEXT("The quick brown fox jumped over the lazy dog. He jumped quite high over him.\r\n"));
			int i = 0;
			for(; i < 100; i++){
				AppendText(eConsole,"JUMP!\r\n");
				SendMessage(eConsole, EM_LINESCROLL, 0, 1100);//update this
			}
		}
		break;
		case WM_SIZE:
		{
			HWND hEdit;
			RECT rcClient;

			GetClientRect(hwnd, &rcClient);

//			hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
			SetWindowPos(hEdit, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
		}

		break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
		break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void cwrite(char* text){
	AppendText(eConsole,text);
}
