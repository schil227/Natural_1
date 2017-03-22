/*
 * global_register.h
 *
 *  Created on: Oct 22, 2015
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_GLOBAL_REGISTER_H_
#define SRC_HEADERS_STRUCTS_GLOBAL_REGISTER_H_

#include"../individual_pub_methods.h"
#include"../event_pub_methods.h"
#include"../abilities_pub_methods.h"
#include"../sound_pub_methods.h"
#include<stdio.h>

typedef struct {
	unsigned int existanceArray[1000];
	int sizeOfInt;

	individual * individualRegistry[1000];
	item * itemRegistry[5000];
	event * eventRegistry[1000];
	ability * templateAbilities[500];
	soundMap * soundMapRegistry[300];
	animation * animationRegistry[10000];
	ability * perminentAbilities[100];
	ability * selfAbilities[100];
	ability * targetedAbilities[100];
	ability * instantAbilities[100];
	mapInfo * mapInfoArr[1000];

	int MAX_INDIVIDUALS;
	int numIndividuals;

	int MAX_ITEMS;
	int numItems;

	int MAX_EVENTS;
	int numEvents;

	int MAX_EFFECTS;
	int numEffects;

	int MAX_SOUNDS;
	int numSounds;

	int MAX_ANIMATIONS;
	int numAnimations;

	int MAX_TEMPLATE_ABILITIES;
	int numTemplateAbilities;

	int numPerminentAbilities;
	int MAX_PERMINENT_ABILITIES;

	int numSelfAbilities;
	int MAX_SELF_ABILITIES;

	int numTargetedAbilities;
	int MAX_TARGETED_ABILITIES;

	int numInstantAbilities;
	int MAX_INSTANT_ABILITIES;

	int numMaps;
	int MAX_MAPS;
} globalRegister;

#endif /* SRC_HEADERS_STRUCTS_GLOBAL_REGISTER_H_ */
