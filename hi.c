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
#define ID_FILE_EXIT 9001
#define ID_STUFF_GO 9002

const char g_szClassName[] = "MyWindowClass";
int numMessages = 0;
int mouseButtonCount = 0;
HWND g_toolbar = NULL;

BOOL CALLBACK ToolDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam){
	switch(Message){
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case IDC_FIRST:
					MessageBox(hwnd,"You top-button pressing Jerk.", "Awesome Message Title~", MB_OK | MB_ICONEXCLAMATION);
				break;
				case IDC_SECOND:
					MessageBox(hwnd,"Bottom button? Ya looser.", "Awesome Message Title~", MB_OK | MB_ICONEXCLAMATION);
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
				case ID_DIALOG_SHOW:
					ShowWindow(g_toolbar,SW_SHOW);
				break;
				case ID_DIALOG_HIDE:
					ShowWindow(g_toolbar,SW_HIDE);
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
	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,g_szClassName,"Window Title :D!",
			WS_OVERLAPPEDWINDOW,CW_USEDEFAULT, CW_USEDEFAULT,240,120,NULL,NULL,
			hInstance,NULL);

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
		if (IsDialogMessageA(g_toolbar,&Msg)) {
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	}


	return Msg.wParam;


}
