/*
 * enemy_controller.c
 *
 *  Created on: May 4, 2015
 *      Author: Adrian
 */
#include"./headers/field_pub_methods.h"

typedef struct {
	int pathLength;
	space *currentSpace;
	struct node * nextNode;
} node;

void getSpaceClosestToPlayer(field * this_field, individual * player, individual * enemy){

}

void enemyAction(individual * enemy, field * thisField, individual * player){
	wanderAround(thisField, enemy);
}
