/*
 * inventory_view.h
 *
 *  Created on: Nov 3, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_INVENTORY_VIEW_H_
#define SRC_HEADERS_STRUCTS_INVENTORY_VIEW_H_
#include"../individual_pub_methods.h"
typedef struct {
	fixedCharacter * inventoryBackground;
	fixedCharacter * itemFrame;
	fixedCharacter * selectArrow;
	fixedCharacter * scrollUpArrow;
	fixedCharacter * scrollDownArrow;
	inventory * playerItems;
	item * viewedItems[10];
	int slotsPerScreen;
	int selectedItemIndex;
	int viewMode;
	int buyMode;
	int pickpocketMode;
	int fieldGetMode;
	int objectGetMode;
} inventoryView;

#endif /* SRC_HEADERS_STRUCTS_INVENTORY_VIEW_H_ */
