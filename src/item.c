/*
 * item.c
 *
 *  Created on: Oct 26, 2015
 *      Author: Adrian
 */
#include"./headers/item_pub_methods.h"

static int itemIDIncrement = 3000;

item * createItem(int npcID, COLORREF rgb, int x, int y, int ID, char type, char *name, char *description, int interactableObjectType, double weaponStrMod,
		int strMod, int dexMod, int conMod, int willMod, int intMod, int wisMod, int chrMod, int luckMod,
		char weaponDamageType, char armorClass, char itemType, int price, int owner, int isStolen, int totalHealthMod, int healthMod, int totalManaMod,
		int manaMod, int food, int acMod, int attackMod, int damMod, int maxDamMod, int minDamMod, int minTurns, int maxTurns,
		int mvmtMod, int rangeMod, int darkLoSMod, int bluntDRMod, int chopDRMod, int slashDRMod, int pierceDRMod, int earthDRMod,
		int fireDRMod, int waterDRMod, int lightningDRMod, int isEquipt, animationContainer * thisAnimationContainer, animationContainer * secondaryAnimationContainer){

	item * thisItem = malloc(sizeof(item));
	thisItem->itemCharacter = malloc(sizeof(character));
	thisItem->itemCharacter->direction = 0;
	thisItem->itemCharacter->x = x;
	thisItem->itemCharacter->y = y;
	thisItem->itemCharacter->xOff = 0;
	thisItem->itemCharacter->yOff = 0;

	thisItem->itemCharacter->thisAnimationContainer = thisAnimationContainer;
	thisItem->itemCharacter->secondaryAnimationContainer = secondaryAnimationContainer;
	thisItem->itemCharacter->darkAnimationContainer = NULL;

	thisItem->npcID = npcID;
	thisItem->ID = ID;
	thisItem->type = type;
	strcpy(thisItem->name, name);
	strcpy(thisItem->description, description);

	thisItem->interactableObjectType = interactableObjectType;
	thisItem->weaponDamageType= weaponDamageType;
	thisItem->armorClass = armorClass;
	thisItem->itemType = itemType;
	thisItem->price = price;
	thisItem->owner = owner;
	thisItem->isStolen = isStolen;
	thisItem->weaponStrMod = weaponStrMod;

	thisItem->strMod = strMod;
	thisItem->dexMod = dexMod;
	thisItem->conMod = conMod;
	thisItem->willMod = willMod;
	thisItem->intMod = intMod;
	thisItem->wisMod = wisMod;
	thisItem->chrMod = chrMod;
	thisItem->luckMod = luckMod;

	thisItem->totalHealthMod = totalHealthMod;
	thisItem->healthMod = healthMod;
	thisItem->totalManaMod = totalManaMod;
	thisItem->manaMod = manaMod;
	thisItem->food = food;
	thisItem->acMod = acMod;
	thisItem->attackMod = attackMod;
	thisItem->damMod = damMod;
	thisItem->maxDamMod = maxDamMod;
	thisItem->minDamMod = minDamMod;
	thisItem->minTurns = minTurns;
	thisItem->maxTurns = maxTurns;
	thisItem->mvmtMod = mvmtMod;
	thisItem->rangeMod = rangeMod;
	thisItem->darkLoSMod = darkLoSMod;

	thisItem->bluntDRMod = bluntDRMod;
	thisItem->chopDRMod = chopDRMod;
	thisItem->slashDRMod = slashDRMod;
	thisItem->pierceDRMod = pierceDRMod;
	thisItem->earthDRMod = earthDRMod;
	thisItem->fireDRMod = fireDRMod;
	thisItem->waterDRMod = waterDRMod;
	thisItem->lightningDRMod = lightningDRMod;
	thisItem->isEquipt = isEquipt;

	return thisItem;
}

void destroyItem(item * thisItem){
	if(thisItem->itemCharacter != NULL){
		destroyCharacter(thisItem->itemCharacter);
	}

	free(thisItem);
}

void setItemIDIncrement(int counter){
	if(counter > itemIDIncrement){
		itemIDIncrement = counter;
	}
}

int getCurrentItemIDIncrement(){
	return itemIDIncrement;
}

item * cloneItem(item * thisItem, int keepOwner){
	item * newItem = malloc(sizeof(item));
	newItem->itemCharacter = malloc(sizeof(character));
	newItem->itemCharacter->x = thisItem->itemCharacter->x;
	newItem->itemCharacter->y = thisItem->itemCharacter->y;
	newItem->itemCharacter->xOff = 0;
	newItem->itemCharacter->yOff = 0;
	newItem->itemCharacter->direction = thisItem->itemCharacter->direction;

	newItem->itemCharacter->thisAnimationContainer = cloneAnimationContainer(thisItem->itemCharacter->thisAnimationContainer);
	newItem->itemCharacter->secondaryAnimationContainer = cloneAnimationContainer(thisItem->itemCharacter->secondaryAnimationContainer);
	newItem->itemCharacter->darkAnimationContainer = NULL;

	newItem->npcID = thisItem->npcID;
	newItem->ID = itemIDIncrement++;
	newItem->type = thisItem->type;
	strcpy(newItem->name, thisItem->name);
	strcpy(newItem->description, thisItem->description);

	newItem->interactableObjectType = thisItem->interactableObjectType;
	newItem->weaponDamageType= thisItem->weaponDamageType;
	newItem->armorClass = thisItem->armorClass;
	newItem->itemType = thisItem->itemType;
	newItem->price = thisItem->price;

	if(keepOwner){
		newItem->owner = thisItem->owner;
	}

	newItem->isStolen = 0;

	newItem->totalHealthMod = thisItem->totalHealthMod;
	newItem->healthMod = thisItem->healthMod;
	newItem->totalManaMod = thisItem->totalManaMod;
	newItem->manaMod = thisItem->manaMod;
	newItem->food = thisItem->food;
	newItem->acMod = thisItem->acMod;
	newItem->attackMod = thisItem->attackMod;
	newItem->damMod = thisItem->damMod;
	newItem->maxDamMod = thisItem->maxDamMod;
	newItem->minDamMod = thisItem->minDamMod;
	newItem->minTurns = thisItem->minTurns;
	newItem->maxTurns = thisItem->maxTurns;
	newItem->mvmtMod = thisItem->mvmtMod;
	newItem->rangeMod = thisItem->rangeMod;
	newItem->darkLoSMod = thisItem->darkLoSMod;

	newItem->bluntDRMod = thisItem->bluntDRMod;
	newItem->chopDRMod = thisItem->chopDRMod;
	newItem->slashDRMod = thisItem->slashDRMod;
	newItem->pierceDRMod = thisItem->pierceDRMod;
	newItem->earthDRMod = thisItem->earthDRMod;
	newItem->fireDRMod = thisItem->fireDRMod;
	newItem->waterDRMod = thisItem->waterDRMod;
	newItem->lightningDRMod = thisItem->lightningDRMod;
	newItem->isEquipt = thisItem->isEquipt;

	return newItem;
}

char * getItemAsLine(item * thisItem){
	int i = 0, j;

	animationContainer * thisAnimationContainer = thisItem->itemCharacter->thisAnimationContainer;

	char * line = malloc(sizeof(char) * 2048);
	line[0] = '\0';

	i = sprintf(line + i, "%d;", thisItem->npcID);
	i += sprintf(line + i, "%d;", thisItem->ID);
	i += sprintf(line + i, "%c;", thisItem->type);
	i += sprintf(line + i, "%c;", thisItem->weaponDamageType);
	i += sprintf(line + i, "%c;", thisItem->armorClass);
	i += sprintf(line + i, "%c;", thisItem->itemType);
	i += sprintf(line + i, "%d;", thisItem->price);
	i += sprintf(line + i, "%d;", thisItem->owner);
	i += sprintf(line + i, "%d;", thisItem->isStolen);
	i += sprintf(line + i, "%d;", 255); // r
	i += sprintf(line + i, "%d;", 0); // g
	i += sprintf(line + i, "%d;", 255); // b
	i += sprintf(line + i, "%s;", thisItem->name);
	i += sprintf(line + i, "%d;", thisItem->itemCharacter->x);
	i += sprintf(line + i, "%d;", thisItem->itemCharacter->y);
	i += sprintf(line + i, "%s;", getInteractString(thisItem->interactableObjectType));
	i += sprintf(line + i, "%.1f;", thisItem->weaponStrMod);

	i += sprintf(line + i, "%d;", thisItem->strMod);
	i += sprintf(line + i, "%d;", thisItem->dexMod);
	i += sprintf(line + i, "%d;", thisItem->conMod);
	i += sprintf(line + i, "%d;", thisItem->willMod);
	i += sprintf(line + i, "%d;", thisItem->intMod);
	i += sprintf(line + i, "%d;", thisItem->wisMod);
	i += sprintf(line + i, "%d;", thisItem->chrMod);
	i += sprintf(line + i, "%d;", thisItem->luckMod);

	i += sprintf(line + i, "%d;", thisItem->totalHealthMod);
	i += sprintf(line + i, "%d;", thisItem->healthMod);
	i += sprintf(line + i, "%d;", thisItem->totalManaMod);
	i += sprintf(line + i, "%d;", thisItem->manaMod);
	i += sprintf(line + i, "%d;", thisItem->food);
	i += sprintf(line + i, "%d;", thisItem->acMod);
	i += sprintf(line + i, "%d;", thisItem->attackMod);
	i += sprintf(line + i, "%d;", thisItem->damMod);
	i += sprintf(line + i, "%d;", thisItem->maxDamMod);
	i += sprintf(line + i, "%d;", thisItem->minDamMod);
	i += sprintf(line + i, "%d;", thisItem->minTurns);
	i += sprintf(line + i, "%d;", thisItem->maxTurns);
	i += sprintf(line + i, "%d;", thisItem->mvmtMod);
	i += sprintf(line + i, "%d;", thisItem->rangeMod);
	i += sprintf(line + i, "%d;", thisItem->darkLoSMod);

	i += sprintf(line + i, "%d;", thisItem->bluntDRMod);
	i += sprintf(line + i, "%d;", thisItem->chopDRMod);
	i += sprintf(line + i, "%d;", thisItem->slashDRMod);
	i += sprintf(line + i, "%d;", thisItem->pierceDRMod);
	i += sprintf(line + i, "%d;", thisItem->earthDRMod);
	i += sprintf(line + i, "%d;", thisItem->fireDRMod);
	i += sprintf(line + i, "%d;", thisItem->waterDRMod);
	i += sprintf(line + i, "%d;", thisItem->lightningDRMod);
	i += sprintf(line + i, "%d;", thisItem->isEquipt);
	i += sprintf(line + i, "%d;", thisAnimationContainer->animationsEnabled);
	i += sprintf(line + i, "%d;", thisAnimationContainer->defaultAnimation);
	i += sprintf(line + i, "%d;", thisItem->itemCharacter->secondaryAnimationContainer->defaultAnimation);

	i += sprintf(line + i, "%d;", getAnimationIDFromTypeToLine(thisAnimationContainer, ANIMATION_IDLE));
	i += sprintf(line + i, "%d;", getAnimationIDFromTypeToLine(thisAnimationContainer, ANIMATION_IDLE_EQUIPT));
	i += sprintf(line + i, "%d;", getAnimationIDFromTypeToLine(thisAnimationContainer, ANIMATION_ATTACK_SLASH));
	i += sprintf(line + i, "%d;", getAnimationIDFromTypeToLine(thisAnimationContainer, ANIMATION_ATTACK_CHOP));
	i += sprintf(line + i, "%d;", getAnimationIDFromTypeToLine(thisAnimationContainer, ANIMATION_ATTACK_BLUNT));
	i += sprintf(line + i, "%d;", getAnimationIDFromTypeToLine(thisAnimationContainer, ANIMATION_ATTACK_PIERCE));
	i += sprintf(line + i, "%d;", getAnimationIDFromTypeToLine(thisAnimationContainer, ANIMATION_ATTACK_BOW));
	i += sprintf(line + i, "%d;", getAnimationIDFromTypeToLine(thisAnimationContainer, ANIMATION_HARM));
	i += sprintf(line + i, "%d;", getAnimationIDFromTypeToLine(thisAnimationContainer, ANIMATION_DEATH));
	i += sprintf(line + i, "%d;", getAnimationIDFromTypeToLine(thisAnimationContainer, ANIMATION_CAST));
	i += sprintf(line + i, "%d;", getAnimationIDFromTypeToLine(thisAnimationContainer, ANIMATION_CONSUME));

	i += sprintf(line + i, "%s", thisItem->description);

	return line;
}

int itemIsBuffing(item * thisItem){
	if(thisItem->weaponStrMod > 0 ||
		thisItem->strMod > 0 ||
		thisItem->dexMod > 0 ||
		thisItem->conMod > 0 ||
		thisItem->willMod > 0 ||
		thisItem->intMod > 0 ||
		thisItem->wisMod > 0 ||
		thisItem->chrMod > 0 ||
		thisItem->luckMod > 0 ||
		thisItem->totalHealthMod > 0 ||
		thisItem->totalManaMod > 0 ||
		thisItem->acMod > 0 ||
		thisItem->attackMod > 0 ||
		thisItem->damMod > 0 ||
		thisItem->maxDamMod > 0 ||
		thisItem->minDamMod > 0 ||
		thisItem->mvmtMod > 0 ||
		thisItem->rangeMod > 0 ||
		thisItem->darkLoSMod > 0 ||
		thisItem->bluntDRMod > 0 ||
		thisItem->chopDRMod > 0 ||
		thisItem->slashDRMod > 0 ||
		thisItem->pierceDRMod > 0 ||
		thisItem->earthDRMod > 0 ||
		thisItem->fireDRMod > 0 ||
		thisItem->waterDRMod > 0 ||
		thisItem->lightningDRMod > 0){
		return 1;
	}

	return 0;
}

void drawItem(HDC hdcBuffer, item * thisItem, int turns, RECT * effectRect){
	char value[16];

	if(turns > 0){
		sprintf(value, "Turns: %d", turns);

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	if(thisItem->strMod != 0){
		setCursorColor(hdcBuffer, thisItem->strMod, 1);
		if(thisItem->strMod > 0){
			sprintf(value, "STR: +%d", thisItem->strMod);
		}else{
			sprintf(value, "STR: %d", thisItem->strMod);
		}

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	if(thisItem->dexMod != 0){
		setCursorColor(hdcBuffer, thisItem->dexMod, 1);
		if(thisItem->dexMod > 0){
			sprintf(value, "DEX: +%d", thisItem->dexMod);
		}else{
			sprintf(value, "DEX: %d", thisItem->dexMod);
		}

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	if(thisItem->conMod != 0){
		setCursorColor(hdcBuffer, thisItem->conMod, 1);
		if(thisItem->conMod > 0){
			sprintf(value, "CON: +%d", thisItem->conMod);
		}else{
			sprintf(value, "CON: %d", thisItem->conMod);
		}

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	if(thisItem->intMod != 0){
		setCursorColor(hdcBuffer, thisItem->intMod, 1);
		if(thisItem->intMod > 0){
			sprintf(value, "INT: +%d", thisItem->intMod);
		}else{
			sprintf(value, "INT: %d", thisItem->intMod);
		}

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	if(thisItem->wisMod != 0){
		setCursorColor(hdcBuffer, thisItem->wisMod, 1);
		if(thisItem->wisMod > 0){
			sprintf(value, "WIS: +%d", thisItem->wisMod);
		}else{
			sprintf(value, "WIS: %d", thisItem->wisMod);
		}

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	if(thisItem->willMod != 0){
		setCursorColor(hdcBuffer, thisItem->willMod, 1);
		if(thisItem->willMod > 0){
			sprintf(value, "WILL: +%d", thisItem->willMod);
		}else{
			sprintf(value, "WILL: %d", thisItem->willMod);
		}

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	if(thisItem->chrMod != 0){
		setCursorColor(hdcBuffer, thisItem->chrMod, 1);
		if(thisItem->chrMod > 0){
			sprintf(value, "CHR: +%d", thisItem->chrMod);
		}else{
			sprintf(value, "CHR: %d", thisItem->chrMod);
		}

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	if(thisItem->luckMod != 0){
		setCursorColor(hdcBuffer, thisItem->luckMod, 1);
		if(thisItem->luckMod > 0){
			sprintf(value, "LUCK: +%d", thisItem->luckMod);
		}else{
			sprintf(value, "LUCK: %d", thisItem->luckMod);
		}

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	if(thisItem->totalHealthMod != 0){
		setCursorColor(hdcBuffer, thisItem->totalHealthMod, 1);
		if(thisItem->totalHealthMod > 0){
			sprintf(value, "Total HP: +%d", thisItem->totalHealthMod);
		}else{
			sprintf(value, "Total HP: %d", thisItem->totalHealthMod);
		}

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	if(thisItem->healthMod != 0){
		setCursorColor(hdcBuffer, thisItem->healthMod, 1);
		if(thisItem->healthMod > 0){
			sprintf(value, "HP: +%d/Turn", thisItem->healthMod);
		}else{
			sprintf(value, "HP: %d/Turn", thisItem->healthMod);
		}

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	if(thisItem->totalManaMod != 0){
		setCursorColor(hdcBuffer, thisItem->totalManaMod, 1);
		if(thisItem->totalManaMod > 0){
			sprintf(value, "Total Mana: +%d", thisItem->totalManaMod);
		}else{
			sprintf(value, "Total Mana: %d", thisItem->totalManaMod);
		}

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	if(thisItem->manaMod != 0){
		setCursorColor(hdcBuffer, thisItem->manaMod, 1);
		if(thisItem->manaMod > 0){
			sprintf(value, "Mana: +%d/Turn", thisItem->manaMod);
		}else{
			sprintf(value, "Mana: %d/Turn", thisItem->manaMod);
		}

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	if(thisItem->acMod != 0){
		setCursorColor(hdcBuffer, thisItem->acMod, 1);
		if(thisItem->acMod > 0){
			sprintf(value, "AC: +%d", thisItem->acMod);
		}else{
			sprintf(value, "AC: %d", thisItem->acMod);
		}

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	if(thisItem->attackMod != 0){
		setCursorColor(hdcBuffer, thisItem->attackMod, 1);
		if(thisItem->attackMod > 0){
			sprintf(value, "Attack: +%d", thisItem->attackMod);
		}else{
			sprintf(value, "Attack: %d", thisItem->attackMod);
		}

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	if(thisItem->type == 'w' && thisItem->isEquipt){
		if(thisItem->weaponStrMod != 0){
			setCursorColor(hdcBuffer, thisItem->weaponStrMod, 1);
			if(thisItem->weaponStrMod > 0){
				sprintf(value, "STR Dam Mod: x%.1f", thisItem->weaponStrMod);
			}else{
				sprintf(value, "STR Dam Mod: x%.1f", thisItem->weaponStrMod);
			}

			DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
				effectRect->top = effectRect->top + 12;
				effectRect->bottom = effectRect->bottom + 12;
		}
	}

	if(thisItem->damMod > 0 || (thisItem->maxDamMod > 0)){
		strcpy(value, "Dam: ");
		if(thisItem->maxDamMod > 0){
			sprintf(value + strlen(value), "[%d-%d]", max(1, thisItem->minDamMod), thisItem->maxDamMod);
		}

		if(thisItem->maxDamMod > 0 && thisItem->damMod > 0){
			sprintf(value + strlen(value), " + ");
		}

		if(thisItem->damMod > 0){
			sprintf(value + strlen(value), "%d", thisItem->damMod);
		}

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	if(thisItem->mvmtMod != 0){
		setCursorColor(hdcBuffer, thisItem->mvmtMod, 1);
		if(thisItem->mvmtMod > 0){
			sprintf(value, "Mvmt: +%d", thisItem->mvmtMod);
		}else{
			sprintf(value, "Mvmt: %d", thisItem->mvmtMod);
		}

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	if(thisItem->rangeMod != 0){
		setCursorColor(hdcBuffer, thisItem->rangeMod, 1);
		if(thisItem->rangeMod > 0){
			sprintf(value, "Range: +%d", thisItem->rangeMod);
		}else{
			sprintf(value, "Range: %d", thisItem->rangeMod);
		}

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	if(thisItem->darkLoSMod != 0){
		setCursorColor(hdcBuffer, thisItem->darkLoSMod, 1);
		if(thisItem->darkLoSMod > 0){
			sprintf(value, "Dark Sight: +%d", thisItem->darkLoSMod);
		}else{
			sprintf(value, "Dark Sight: %d", thisItem->darkLoSMod);
		}

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	if(thisItem->bluntDRMod != 0){
		setCursorColor(hdcBuffer, thisItem->bluntDRMod, 1);
		if(thisItem->bluntDRMod > 0){
			sprintf(value, "Blunt DR: +%d", thisItem->bluntDRMod);
		}else{
			sprintf(value, "Blunt DR: %d", thisItem->bluntDRMod);
		}

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	if(thisItem->chopDRMod != 0){
		setCursorColor(hdcBuffer, thisItem->chopDRMod, 1);
		if(thisItem->chopDRMod > 0){
			sprintf(value, "Chop DR: +%d", thisItem->chopDRMod);
		}else{
			sprintf(value, "Chop DR: %d", thisItem->chopDRMod);
		}

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	if(thisItem->pierceDRMod != 0){
		setCursorColor(hdcBuffer, thisItem->pierceDRMod, 1);
		if(thisItem->pierceDRMod > 0){
			sprintf(value, "Pierce DR: +%d", thisItem->pierceDRMod);
		}else{
			sprintf(value, "Pierce DR: %d", thisItem->pierceDRMod);
		}

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	if(thisItem->slashDRMod != 0){
		setCursorColor(hdcBuffer, thisItem->slashDRMod, 1);
		if(thisItem->slashDRMod > 0){
			sprintf(value, "Slash DR: +%d", thisItem->slashDRMod);
		}else{
			sprintf(value, "Slash DR: %d", thisItem->slashDRMod);
		}

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	int calculatedDR = thisItem->earthDRMod;

	if(calculatedDR != 0){
		setCursorColor(hdcBuffer, calculatedDR, 1);
		if(calculatedDR > 0){
			sprintf(value, "Earth DR: +%d", calculatedDR);
		}else{
			sprintf(value, "Earth DR: %d", calculatedDR);
		}

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	calculatedDR = thisItem->fireDRMod;

	if(calculatedDR != 0){
		setCursorColor(hdcBuffer, calculatedDR, 1);
		if(calculatedDR > 0){
			sprintf(value, "Fire DR: +%d", calculatedDR);
		}else{
			sprintf(value, "Fire DR: %d", calculatedDR);
		}

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	calculatedDR = thisItem->waterDRMod;

	if(calculatedDR != 0){
		setCursorColor(hdcBuffer, calculatedDR, 1);
		if(calculatedDR > 0){
			sprintf(value, "Water DR: +%d", calculatedDR);
		}else{
			sprintf(value, "Water DR: %d", calculatedDR);
		}

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	calculatedDR = thisItem->lightningDRMod;

	if(calculatedDR != 0){
		setCursorColor(hdcBuffer, calculatedDR, 1);
		if(calculatedDR > 0){
			sprintf(value, "Lightning DR: +%d", calculatedDR);
		}else{
			sprintf(value, "Lightning DR: %d", calculatedDR);
		}

		DrawText(hdcBuffer, value, -1, effectRect, DT_SINGLELINE);
			effectRect->top = effectRect->top + 12;
			effectRect->bottom = effectRect->bottom + 12;
	}

	SetTextColor(hdcBuffer, RGB(255, 200, 0));
}
