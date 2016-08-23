/*
 * loop_controller_pub_methods.h
 *
 *  Created on: Nov 3, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_LOOP_CONTROLLER_PUB_METHODS_H_
#define SRC_HEADERS_LOOP_CONTROLLER_PUB_METHODS_H_

int inventoryLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, field * main_field, individual * player, shiftData * viewShift);

int moveLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, int * moveMode,
		field * thisField, individual * thisIndividual, moveNodeMeta * thisMoveNodeMeta, int * postMoveMode, shiftData * viewShift);

void animateMoveLoop(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, field * thisField,
		individual * thisIndividual, moveNodeMeta * thisMoveNodeMeta, int speed,
		int * postMoveMode, shiftData * viewShift, int updateViewShift);
#endif /* SRC_HEADERS_LOOP_CONTROLLER_PUB_METHODS_H_ */
