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
	character * scrollArrow;
	inventory * playerItems;
	int slotsPerScreen;
	item * selectedItem;
	item * viewedItems[4];

} inventoryView;

#endif /* SRC_HEADERS_STRUCTS_INVENTORY_VIEW_H_ */
