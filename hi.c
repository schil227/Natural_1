/*
 * hi.c
 *
 *  Created on: Feb 12, 2015
 *      Author: Adrian
 */

#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include "general.h"
#include "resource.h"


const char g_szClassName[] = "MyWindowClass";
int numMessages = 0;
int mouseButtonCount = 0;

HWND g_toolbar = NULL;

BOOL CALLBACK ToolDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam){
	int len = 0;
	switch(Message){
		case WM_COMMAND:

			switch(LOWORD(wParam)){
				case IDC_SECOND: // get the Name
					len = GetWindowTextLength(GetDlgItem(hwnd, IDC_TEXT));
					if (len == 0) {
							MessageBox(hwnd, "Longer name, please.", "Awesome Message Title~", MB_OK | MB_ICONEXCLAMATION);
					}else{
						char * textStr = (char*)GlobalAlloc(GPTR, len + 1);
						GetDlgItemText(hwnd, IDC_TEXT, textStr, len + 1);
//						printf("string: %s\n", textStr);
						char * str = (char *)join("Name: ",textStr);
//						printf("str: %s\n", str);
						MessageBox(hwnd, str,"Awesome Message Title~", MB_OK | MB_ICONEXCLAMATION);
						free(str);
					}
				break;
			}
		break;
		case WM_DESTROY:
			DestroyWindow(g_toolbar);
			PostQuitMessage(0);
		break;
		default:
			return FALSE;
	}
	return TRUE;
}

BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam){
	switch(Message){
		case WM_INITDIALOG:
			//do processing needed before the window appears
		return TRUE;
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case IDOK:
					EndDialog(hwnd,IDOK); //destroys dialog box, returns the second value
				break;
				case IDCANCEL:
					EndDialog(hwnd,IDCANCEL);
				break;
			}
		break;
		default:
			return FALSE;
	}
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch(msg){
		case WM_CREATE:
			{
				HFONT hfDefault;
				HWND hEdit;
				HWND hButton;

				hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "",
					WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
					0,0,100,100,hwnd, (HMENU) IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);

				if(hEdit == NULL){
					MessageBox(hwnd, "Failed to make edit window", "Notice",
						MB_OK | MB_ICONINFORMATION);
				}

				hButton = CreateWindowEx(WS_EX_CLIENTEDGE, "BUTTON", "WOOT",
						WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
						50,105,50,50, hwnd, (HMENU) IDC_MAIN_BUTTON,GetModuleHandle(NULL), NULL);

				hfDefault = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
				SendMessage(hEdit, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM(FALSE, 0));
				SendMessage(hButton, WM_SETFONT, (WPARAM) hfDefault, MAKELPARAM(FALSE, 0));
			}
		break;
		case WM_SIZE:
		{
			HWND hEdit;
			HWND hButton;
			RECT rcClient;

			GetClientRect(hwnd, &rcClient);

			hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
			SetWindowPos(hEdit, NULL, 0, 0, rcClient.right, rcClient.bottom-100, SWP_NOZORDER);

			hButton = GetDlgItem(hwnd, IDC_MAIN_BUTTON);
			SetWindowPos(hButton, NULL, 5, rcClient.bottom-50,50,25,SWP_NOZORDER);
		}
		break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
		break;
		case WM_LBUTTONDOWN:
			mouseButtonCount++;
			//charArrStruct *myCharArr = bigTriangle(mouseButtonCount);
		{
			char mouseClicks[1];
			sprintf(mouseClicks,"%d",mouseButtonCount);
//			HINSTANCE hInstance = GetModuleHandle(NULL);
			printf("pushin down that button! %d\n", mouseButtonCount);
//			GetModuleFileNameA(hInstance, szFileName, MAX_PATH);

		//	printf("%s\n", myCharArr->charArr);

			MessageBox(hwnd,"bananas", "num clicks:",
					MB_OK | MB_ICONINFORMATION);
		//	free(myCharArr->charArr);
		//	free(myCharArr);

		}
		break;
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case ID_HELP_ABOUT:
				{
					int ret = DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUT), hwnd, AboutDlgProc);
					if(ret == IDOK){
						MessageBox(hwnd, "Dialog exited from clicking ok.", "Notice", MB_OK | MB_ICONINFORMATION);
					}
		            else if(ret == IDCANCEL){
		                MessageBox(hwnd, "Dialog exited from clicking cancel.", "Notice",
		                    MB_OK | MB_ICONINFORMATION);
		            }
		            else if(ret == -1){
		                MessageBox(hwnd, "Dialog failed!", "Error",
		                    MB_OK | MB_ICONINFORMATION);
		            }
				}
				break;
				case ID_FILE_EXIT:
					PostQuitMessage(0);
				break;
				case ID_DIALOG_SHOW:
					ShowWindow(g_toolbar,SW_SHOW);
				break;
				case ID_DIALOG_HIDE:
					ShowWindow(g_toolbar,SW_HIDE);
				break;
				case IDC_MAIN_BUTTON:
				//	MessageBox(hwnd, "WOOT", "Error", MB_OK | MB_ICONINFORMATION);
					openFile(hwnd);
				break;
			}
		break;
		default:
//			printf("msg:%d, count:%d\n",msg,numMessages);
			numMessages++;
			return DefWindowProc(hwnd,msg,wParam,lParam);
	}
	return 0;
}


BOOL LoadTextFileToEdit(HWND hwnd, LPCTSTR pszFileName){
	HANDLE hFile;
	BOOL bSuccess = FALSE;

	hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
				OPEN_EXISTING, 0, NULL);
	if(hFile!=INVALID_HANDLE_VALUE){
		DWORD dwFileSize;

		dwFileSize = GetFileSize(hFile, NULL);
		if(dwFileSize != 0xFFFFFFFF){

			LPSTR pszFileText;

			pszFileText = (LPSTR)GlobalAlloc(GPTR, dwFileSize +1);
			if(pszFileText != NULL){

				DWORD dwRead;
				if(ReadFile(hFile, pszFileText, dwFileSize, &dwRead, NULL)){
					pszFileText[dwFileSize] = 0;
					if(SetWindowText(hwnd, pszFileText)){
						bSuccess = TRUE;
					}
				}
				GlobalFree(pszFileText);
			}


		}
		CloseHandle(hFile);
	}
	return bSuccess;
}

void openFile(HWND hwnd){
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = ""; //file name can only be as large as MAX_PATH

	ZeroMemory(&ofn, sizeof(ofn)); //NULLs out unused attrbs

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "txt";

	if(GetOpenFileName(&ofn)){
		HWND hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
		LoadTextFileToEdit(hEdit, szFileName);
	}

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow) {

	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;


	//step 1: registering the window class
	wc.cbSize = sizeof(WNDCLASSEX); //Size of the structure
	wc.style = 0; //Class styles (usually zero)
	wc.lpfnWndProc = WndProc; //Pointer to the window procedure for this window class
	wc.cbClsExtra = 0; //amount of extra data for this class in memory
	wc.cbWndExtra = 0; //amount of extra data per window of this type
	wc.hInstance = hInstance; //handle to app instance (input param)
	wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON)); //Icon shown when user presses alt+tab
	wc.hCursor = LoadCursorA(NULL,IDC_ARROW); //cursor that will be displayed over win.
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); //background brush to set color of window
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MYMENU);; // name of menu resource to use for the windows
	wc.lpszClassName = g_szClassName; //name to identify class with
	wc.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON), IMAGE_ICON, 16, 16, 0); //small icon to show in taskbar

	if(!RegisterClassEx(&wc)){
		MessageBox(NULL, "Window Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	//create the window (handle)
	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,
			g_szClassName,
			"Window Title :D!",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			240,360,
			NULL,NULL,
			hInstance,
			NULL);

	//ALWAYS CHECK THE RETURN VALUES!!!
	if(hwnd == NULL){
		MessageBox(NULL,"Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow); //display window

	g_toolbar = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_TOOLBAR), hwnd, ToolDlgProc);
	if(g_toolbar != NULL){
		ShowWindow(g_toolbar, SW_SHOW);
	}else{
		MessageBox(hwnd,"Could not create the toolbar dialog", "Warning!", MB_OK | MB_ICONINFORMATION);
	}


	UpdateWindow(hwnd); //redraw

	/*
	 * GetMessage gets a message from the queue, e.g. when the user moves
	 * the mouse or types a key, clicks, etc.
	 *
	 * If there is no message, it BLOCKS, meaning it waits for a message
	 */
	while(GetMessage(&Msg,NULL,0,0) > 0){
		/*
		 * TranslateMessage does additional processing on keyboard events,
		 * like generating WM_CHAR message to go along with WM_KEYDOWN
		 * messages
		 *
		 * DispachMessage sends the message out to the window
		 */
//		if (!IsDialogMessageA(g_toolbar,&Msg)) {
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
//		}
	}


	return Msg.wParam;


}
