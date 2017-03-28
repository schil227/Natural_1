/*
 * loop_controller_pub_methods.h
 *
 *  Created on: Nov 3, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_LOOP_CONTROLLER_PUB_METHODS_H_
#define SRC_HEADERS_LOOP_CONTROLLER_PUB_METHODS_H_

int inventoryLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, field * main_field, individual * player, groupContainer * thisGroupContainer, shiftData * viewShift, int * inActionMode);

int moveLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, int * moveMode, field * thisField, individual * thisIndividual,  groupContainer * thisGroupContainer, int * postMoveMode, shiftData * viewShift, int animateMoveSpeed);

void animateMoveLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, field * thisField,
		individual * thisIndividual, int speed,
		int * postMoveMode, shiftData * viewShift, int updateViewShift);
#endif /* SRC_HEADERS_LOOP_CONTROLLER_PUB_METHODS_H_ */
