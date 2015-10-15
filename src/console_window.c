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
//
//void AppendText(HWND hwnd, TCHAR *newText)
//{
//    // get edit control from dialog
//    HWND hwndOutput = hwnd;
//
//    // get the current selection
//    DWORD StartPos, EndPos;
//    SendMessage( hwndOutput, EM_GETSEL, &StartPos, &EndPos);
//
//    // move the caret to the end of the text
//    int outLength = GetWindowTextLength( hwndOutput );
//    SendMessage( hwndOutput, EM_SETSEL, outLength, outLength );
//
//    // insert the text at the new caret position
//    SendMessage( hwndOutput, EM_REPLACESEL, TRUE, newText);
//
//    // restore the previous selection
//    SendMessage( hwndOutput, EM_SETSEL, StartPos, EndPos );
//}
//
//char* appendStrings(char* str1, char* str2){
//	char* toReturn = (char *)malloc(strlen(str1) + strlen(str2)+3);
//	strcpy(toReturn,str1);
//	strcat(toReturn,str2);
//	return toReturn;
//}
//
//void sendMissedDialog(char* individualName, char* targetName, int attackRoll, int targetAC){
//
//	char* missedStr;
//	if(attackRoll +1 == targetAC){
//		missedStr = appendStrings(individualName, " just barely missed!\n");
//
//	}else if(attackRoll > 6){
//		missedStr = appendStrings(individualName, " missed!\n");
//
//	}else if(attackRoll <= 6 && attackRoll > 3){
//		char* tmp1 = appendStrings(individualName, " wiffed trying to hit ");
//		char* tmp2 = appendStrings(targetName, "!\n");
//		missedStr = appendStrings(tmp1,tmp2);
//		free(tmp1);
//		free(tmp2);
//	}else{
//		missedStr = appendStrings(individualName, " nearly fell down trying to attack.\n");
//
//	}
//	cwrite(missedStr);
//	free(missedStr);
//}
//
//int calcUpperPercentileThreshold(int num, int topPercentile){
//	return ((num*100)*(100-topPercentile))/10000;
//}
//
//int damageUpperPercentile(int damage, int maxDam, int nthP){
//	if(damage == maxDam || damage > calcUpperPercentileThreshold(maxDam, nthP)){
//		return 1;
//	}else{
//		return 0;
//	}
//}
//
//void sendHitDialog(char* individualName, char* targetName, int maxDam, int damage){
//
//	char* hitStr;
//	char takeOut[20]; //" takes %d damage!\n"
//
//	if(damageUpperPercentile(damage, maxDam, 20)){
//		char* highDam = appendStrings(individualName, " executes a brutal strike!\n");
//		cwrite(highDam);
//		free(highDam);
//	}
//
//	sprintf(takeOut, " takes %d damage!\n", damage);
//	hitStr = appendStrings(targetName, takeOut);
//
//	cwrite(hitStr);
//	free(hitStr);
//}
//
//void sendDeathDialog(char* name, char* killer){
//	char* deathStr;
//
//	deathStr = appendStrings(name, " was slain by ");
//	deathStr = appendStrings(deathStr, killer);
//	deathStr = appendStrings(deathStr, "!\n");
//
//	cwrite(deathStr);
//	free(deathStr);
//}
//
//void cwrite(char* text){
//	AppendText(eConsole,text);
//}
