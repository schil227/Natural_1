/*
 * interactable.c
 *
 *  Created on: May 30, 2017
 *      Author: Adrian
 */

#include "./headers/interactable_pub_methods.h"

interactType getInteractType(char * type){
	if(strcmp(type, "INTERACT_DEFAULT") == 0){
		return INTERACT_DEFAULT;
	}else if(strcmp(type, "INTERACT_DOOR_FOREST_DUNGEON") == 0){
		return INTERACT_DOOR_FOREST_DUNGEON;
	}else if(strcmp(type, "INTERACT_CHEST") == 0){
		return INTERACT_CHEST;
	}else if(strcmp(type, "INTERACT_FISHING") == 0){
		return INTERACT_FISHING;
	}else if(strcmp(type, "INTERACT_SWITCH") == 0){
		return INTERACT_SWITCH;
	}else if(strcmp(type, "INTERACT_SIGN") == 0){
		return INTERACT_SIGN;
	}

	char outlog[128];
	sprintf(outlog, "!! INTERACT TYPE %s NOT FOUND : USING DEFAULT !!", type);
	cwrite(outlog);

	return INTERACT_DEFAULT;
}

int canPassThroughInteractableObject(interactable * thisObject){
	if(thisObject == NULL || thisObject->isPassable){
		return 1;
	} else{
		return 0;
	}
}

int canAttackThroughInteractableObject(interactable * thisObject){
	if(thisObject == NULL || thisObject->canAttackThrough){
		return 1;
	} else{
		return 0;
	}
}

int canSeeThroughInteractableObject(interactable * thisObject){
	if(thisObject == NULL || thisObject->canSeeThrough){
		return 1;
	} else{
		return 0;
	}
}

char * getInteractString(interactType type){
	if(type == INTERACT_DEFAULT){
		return "INTERACT_DEFAULT";
	}else if(type == INTERACT_DOOR_FOREST_DUNGEON){
		return "INTERACT_DOOR_FOREST_DUNGEON";
	}else if(type == INTERACT_CHEST){
		return "INTERACT_CHEST";
	}else if(type == INTERACT_FISHING){
		return "INTERACT_FISHING";
	}else if(type == INTERACT_SWITCH){
		return "INTERACT_SWITCH";
	}else if(type == INTERACT_SIGN){
		return "INTERACT_SIGN";
	}

	return "INTERACT_DEFAULT";
}


interactable * loadInteractableObjectFromLine(char * line){
	char * strtok_save_pointer;
	char * value;

	interactable * thisInteractableObject = malloc(sizeof(interactable));
	thisInteractableObject->thisCharacter = malloc(sizeof(character));

	value = strtok_r(line, ";", &strtok_save_pointer);
	thisInteractableObject->ID = atoi(value);

	value = strtok_r(NULL, ";", &strtok_save_pointer);
	thisInteractableObject->type = getInteractType(value);

	value = strtok_r(NULL, ";", &strtok_save_pointer);
	thisInteractableObject->thisCharacter->x = atoi(value);

	value = strtok_r(NULL, ";", &strtok_save_pointer);
	thisInteractableObject->thisCharacter->y = atoi(value);

	thisInteractableObject->thisCharacter->direction = 0;
	thisInteractableObject->thisCharacter->xOff = 0;
	thisInteractableObject->thisCharacter->yOff = 0;

	value = strtok_r(NULL, ";", &strtok_save_pointer);
	thisInteractableObject->triggerDialogID = atoi(value);

	value = strtok_r(NULL, ";", &strtok_save_pointer);
	thisInteractableObject->triggerEventID = atoi(value);

	value = strtok_r(NULL, ";", &strtok_save_pointer);
	thisInteractableObject->isEnabled = atoi(value);

	value = strtok_r(NULL, ";", &strtok_save_pointer);
	thisInteractableObject->shouldDraw = atoi(value);

	value = strtok_r(NULL, ";", &strtok_save_pointer);
	thisInteractableObject->isPassable = atoi(value);

	value = strtok_r(NULL, ";", &strtok_save_pointer);
	thisInteractableObject->canAttackThrough = atoi(value);

	value = strtok_r(NULL, ";", &strtok_save_pointer);
	thisInteractableObject->canSeeThrough = atoi(value);

	value = strtok_r(NULL, ";", &strtok_save_pointer);
	thisInteractableObject->isRespawning = atoi(value);

	value = strtok_r(NULL, ";", &strtok_save_pointer);
	thisInteractableObject->finalEvent = atoi(value);

	thisInteractableObject->thisCharacter->thisAnimationContainer = initAnimationContainer();

	value = strtok_r(NULL, ";", &strtok_save_pointer);
	addAnimationToContainer(thisInteractableObject->thisCharacter->thisAnimationContainer, cloneAnimationFromRegistry(atoi(value)));

	value = strtok_r(NULL, ";", &strtok_save_pointer);
	addAnimationToContainer(thisInteractableObject->thisCharacter->thisAnimationContainer, cloneAnimationFromRegistry(atoi(value)));

	value = strtok_r(NULL, ";", &strtok_save_pointer);
	addAnimationToContainer(thisInteractableObject->thisCharacter->thisAnimationContainer, cloneAnimationFromRegistry(atoi(value)));

	value = strtok_r(NULL, ";", &strtok_save_pointer);
	addAnimationToContainer(thisInteractableObject->thisCharacter->thisAnimationContainer, cloneAnimationFromRegistry(atoi(value)));

	thisInteractableObject->thisCharacter->secondaryAnimationContainer = cloneAnimationContainer(thisInteractableObject->thisCharacter->thisAnimationContainer);
	thisInteractableObject->thisCharacter->darkAnimationContainer = NULL;

	return thisInteractableObject;
}

char * getInteractableObjectAsLine(interactable * thisObject){
	int i = 0;
	char * line = malloc(sizeof(char) * 256);

	i += sprintf(line + i, "%d;", thisObject->ID);
	i += sprintf(line + i, "%s;", getInteractString(thisObject->type));
	i += sprintf(line + i, "%d;", thisObject->thisCharacter->x);
	i += sprintf(line + i, "%d;", thisObject->thisCharacter->y);
	i += sprintf(line + i, "%d;", thisObject->triggerDialogID);
	i += sprintf(line + i, "%d;", thisObject->triggerEventID);
	i += sprintf(line + i, "%d;", thisObject->isEnabled);
	i += sprintf(line + i, "%d;", thisObject->shouldDraw);
	i += sprintf(line + i, "%d;", thisObject->isPassable);
	i += sprintf(line + i, "%d;", thisObject->canAttackThrough);
	i += sprintf(line + i, "%d;", thisObject->canSeeThrough);
	i += sprintf(line + i, "%d;", thisObject->isRespawning);
	i += sprintf(line + i, "%d;", thisObject->finalEvent);
	i += sprintf(line + i, "%d;", getAnimationIDFromTypeToLine(thisObject->thisCharacter->thisAnimationContainer, ANIMATION_INTERACTABLE_IDLE));
	i += sprintf(line + i, "%d;", getAnimationIDFromTypeToLine(thisObject->thisCharacter->thisAnimationContainer, ANIMATION_INTERACTABLE_ACTION_1));
	i += sprintf(line + i, "%d;", getAnimationIDFromTypeToLine(thisObject->thisCharacter->thisAnimationContainer, ANIMATION_INTERACTABLE_ACTION_2));
	i += sprintf(line + i, "%d;", getAnimationIDFromTypeToLine(thisObject->thisCharacter->thisAnimationContainer, ANIMATION_INTERACTABLE_ACTION_FINAL));

	return line;
}
