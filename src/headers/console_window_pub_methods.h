/*
 * console_window_pub_methods.h
 *
 *  Created on: Aug 4, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_CONSOLE_WINDOW_PUB_METHODS_H_
#define SRC_HEADERS_CONSOLE_WINDOW_PUB_METHODS_H_

#include<windows.h>
LRESULT CALLBACK ConsWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void cwrite(char* text);
#endif /* SRC_HEADERS_CONSOLE_WINDOW_PUB_METHODS_H_ */
