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
	character * inventoryBackground;
	character * itemFrame;
	character * selectArrow;
	character * scrollUpArrow;
	character * scrollDownArrow;
	inventory * playerItems;
	item * viewedItems[10];
	int slotsPerScreen;
	int selectedItemIndex;
	int buyMode;
} inventoryView;

#endif /* SRC_HEADERS_STRUCTS_INVENTORY_VIEW_H_ */
