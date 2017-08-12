/*
 * ability_creation_view.c
 *
 *  Created on: Apr 1, 2016
 *      Author: Adrian
 */
#include"./headers/abilities_pub_methods.h"
#include<string.h>
#include<stdio.h>

static abilityCreationInstance * thisAbilityCreationInstance;

void setUpDescriptions(){
	strcpy(thisAbilityCreationInstance->descriptionPermanent,"A Permanent ability is always active. The total mana cost is equal to the level of the ability.");
	strcpy(thisAbilityCreationInstance->descriptionPermanentRange,"The physical attack range of the player; added to the base attack range of their weapon.");
	strcpy(thisAbilityCreationInstance->descriptionPermanentAC,"The Armor Class of the player; added to the player's base AC.");
	strcpy(thisAbilityCreationInstance->descriptionPermanentAttack,"The Attack of the player; added to the player's base Attack.");
	strcpy(thisAbilityCreationInstance->descriptionPermanentDamageMod,"Extra damage done by the player; added to the player's base Damage.");
	strcpy(thisAbilityCreationInstance->descriptionPermanentMvmt,"How far the player can move; added to the player's base Mvmt.");
	strcpy(thisAbilityCreationInstance->descriptionPermanentBaseHp,"The total number of hit points the player has; added to the player's baseHP.");
	strcpy(thisAbilityCreationInstance->descriptionPermanentBaseMana,"The total number of mana points the player has; added to the player's baseMana.");
	strcpy(thisAbilityCreationInstance->descriptionPermanentBluntDR,"The player's resistance to blunt damage; added to the player's BluntDR.");
	strcpy(thisAbilityCreationInstance->descriptionPermanentChopDR,"The player's resistance to chop damage; added to the player's ChopDR.");
	strcpy(thisAbilityCreationInstance->descriptionPermanentPierceDR,"The player's resistance to pierce damage; added to the player's PierceDR.");
	strcpy(thisAbilityCreationInstance->descriptionPermanentSlashDR,"The player's resistance to slash damage; added to the player's SlashDR.");
	strcpy(thisAbilityCreationInstance->descriptionPermanentEarthDR,"The player's resistance to earth damage; added to the player's EarthDR.");
	strcpy(thisAbilityCreationInstance->descriptionPermanentFireDR,"The player's resistance to fire damage; added to the player's FireDR.");
	strcpy(thisAbilityCreationInstance->descriptionPermanentWaterDR,"The player's resistance to water damage; added to the player's WaterDR.");
	strcpy(thisAbilityCreationInstance->descriptionPermanentLightningDR,"The player's resistance to lightning damage; added to the player's LightningDR.");

	strcpy(thisAbilityCreationInstance->descriptionDuration,"The effects of the duration ability are applied to the player and any individuals within it's area of effect (AOE)");
	strcpy(thisAbilityCreationInstance->descriptionDurationRange,"The physical attack range of the player; temporarily added to the base attack range of their weapon.");
	strcpy(thisAbilityCreationInstance->descriptionDurationDiceDamage,"Random damage that only the player receives when casting the spell. Can be used to lessen the cost of the ability at the risk hp loss. Cumulative with Damage.");
	strcpy(thisAbilityCreationInstance->descriptionDurationDiceDamageMult,"Multiplies the random damage. Can be used to lessen the cost of the ability at the risk hp loss. Cumulative with Damage. Cumulative with Damage.");
	strcpy(thisAbilityCreationInstance->descriptionDurationDamage,"Damage that only the player receives when casting the spell. Can be used to lessen the cost of the ability at the risk hp loss. Cumulative with DiceDamage.");
	strcpy(thisAbilityCreationInstance->descriptionDurationStatus,"Status affliction that only the player receives when casting the spell.  Can be used to lessen the cost of the ability at the risk of a harmful status.");
	strcpy(thisAbilityCreationInstance->descriptionDurationStatusDiceDamage,"Random damage that only the player receives for the duration of the status when casting the spell. Can be used to lessen the cost of the ability at the risk hp loss. Cumulative with StatusDamage.");
	strcpy(thisAbilityCreationInstance->descriptionDurationStatusDamage,"Damage that only the player receives for the duration of the status when casting the spell. Can be used to lessen the cost of the ability at the risk hp loss. Cumulative with StatusDiceDamage.");
	strcpy(thisAbilityCreationInstance->descriptionDurationStatusDiceDuration,"The random number of turns the status will last. Cumulative with StatusDiceDuration.");
	strcpy(thisAbilityCreationInstance->descriptionDurationStatusDuration,"The number of turns the status will last. Cumulative with StatusDuration.");
	strcpy(thisAbilityCreationInstance->descriptionDurationAOENova,"The area of affect of the spell around the player.");
	strcpy(thisAbilityCreationInstance->descriptionDurationAOELine,"The area of affect of the spell from the player to the target spot.");
	strcpy(thisAbilityCreationInstance->descriptionDurationDiceDuration,"How long the ability will last by random dice roll. Cumulative with Duration.");
	strcpy(thisAbilityCreationInstance->descriptionDurationDuration,"How long the ability will last. Cumulative with DiceDuration. ");
	strcpy(thisAbilityCreationInstance->descriptionDurationActions,"The number of actions the player forfeits by using the ability. Useful to reduce the cost of the ability at the risk of not being able to move afterwords.");
	strcpy(thisAbilityCreationInstance->descriptionDurationAC,"Additional points of Armor Class; temporarily added to the individual's base AC.");
	strcpy(thisAbilityCreationInstance->descriptionDurationAttack,"Additional points of Attack; temporarily added to the individual's base Attack.");
	strcpy(thisAbilityCreationInstance->descriptionDurationDamageMod,"Extra damage done by the individual; temporarily added to the individual's base Damage.");
	strcpy(thisAbilityCreationInstance->descriptionDurationMvmt,"How far the individual can move; added to the individual's base Mvmt.");
	strcpy(thisAbilityCreationInstance->descriptionDurationDiceHp,"Heals the individual a random number of Hp.");
	strcpy(thisAbilityCreationInstance->descriptionDurationHp,"Heals the individual a set number of Hp.");
	strcpy(thisAbilityCreationInstance->descriptionDurationBaseHp,"Increases the individual's total HP; temporarily added to the individual's base HP (Does not heal the individual).");
	strcpy(thisAbilityCreationInstance->descriptionDurationBaseMana,"Increases the individual's total mana; temporarily added to the individual's base mana (Does not restore the individual's mana).");
	strcpy(thisAbilityCreationInstance->descriptionDurationBluntDR,"The individual's resistance to blunt damage; temporarily added to the player's BluntDR.");
	strcpy(thisAbilityCreationInstance->descriptionDurationChopDR,"The individual's resistance to chop damage; temporarily added to the player's ChopDR.");
	strcpy(thisAbilityCreationInstance->descriptionDurationPierceDR,"The individual's resistance to pierce damage; temporarily added to the player's PierceDR.");
	strcpy(thisAbilityCreationInstance->descriptionDurationSlashDR,"The individual's resistance to slash damage; temporarily added to the player's SlashDR.");
	strcpy(thisAbilityCreationInstance->descriptionDurationEarthDR,"The individual's resistance to earth damage; temporarily added to the player's EarthDR.");
	strcpy(thisAbilityCreationInstance->descriptionDurationFireDR,"The individual's resistance to fire damage; temporarily added to the player's FireDR.");
	strcpy(thisAbilityCreationInstance->descriptionDurationWaterDR,"The individual's resistance to water damage; temporarily added to the player's WaterDR.");
	strcpy(thisAbilityCreationInstance->descriptionDurationLightningDR,"The individual's resistance to lightning damage; temporarily added to the player's LightningDR.");

	strcpy(thisAbilityCreationInstance->descriptionTargeted,"The affects of the targeted ability are on the individuals within range.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedDamageType,"The damage type individuals will receive from the ability: Earth, Fire, Water or Lightning. Does not apply to status damage.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedRange,"The distance in spaces from the player that the ability can be cast.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedTargeted,"If the ability is targeted, it will auto-hit all individuals within its area of effect (AOE). Otherwise, individuals will roll to see if they are affected by it.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedDiceDamage,"The random amount of damage the targeted individuals will receive. Cumulative with Damage.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedDiceDamageMult,"Multiplies the random damage.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedDamage,"The amount of damage the targeted individuals will receive. Cumulative with DiceDamage.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedStatus,"The Status the targeted individual will be afflicted with.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedStatusDiceDamage,"The random amount of damage the targeted individuals will receive from the status. Cumulative with StatusDamage.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedStatusDamage,"The amount of damage the targeted individuals will receive from the status. Cumulative with StatusDiceDamag");
	strcpy(thisAbilityCreationInstance->descriptionTargetedStatusDiceDuration,"The random duration of the status afflicted on the target individual. Cumulative with StatusDuration.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedStatusDuration,"The duration of the status afflicted on the target individual. Cumulative with StatusDiceDuration.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedAOENova,"The area of affect of the spell around the targeted spot.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedAOELine,"The area of affect of the spell between two targeted spots.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedDiceDuration,"How long the ability will last by random dice roll. Cumulative with Duration.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedDuration,"How long the ability will last. Cumulative with DiceDuration. ");
	strcpy(thisAbilityCreationInstance->descriptionTargetedActions,"The number of actions the player forfeits by using the ability. Useful to reduce the cost of the ability at the risk of not being able to move afterwords.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedAC,"Additional points of Armor Class; temporarily added to the individual's base AC.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedAttack,"Additional points of Attack; temporarily added to the individual's base Attack.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedDamageMod,"Extra damage done by the individual; temporarily added to the individual's base Damage.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedMvmt,"How far the individual can move; temporarily added to the individual's base Mvmt.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedDiceHp,"Heals the individual a random number of Hp.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedHp,"Heals the individual a set number of Hp.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedBaseHp,"Increases the individual's total HP; temporarily added to the individual's base HP (Does not heal the individual).");
	strcpy(thisAbilityCreationInstance->descriptionTargetedBaseMana,"Increases the individual's total mana; temporarily added to the individual's base mana (Does not restore the individual's mana).");
	strcpy(thisAbilityCreationInstance->descriptionTargetedBluntDR,"The individual's resistance to blunt damage; temporarily added to the player's BluntDR.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedChopDR,"The individual's resistance to chop damage; temporarily added to the player's ChopDR.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedPierceDR,"The individual's resistance to pierce damage; temporarily added to the player's PierceDR.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedSlashDR,"The individual's resistance to slash damage; temporarily added to the player's SlashDR.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedEarthDR,"The individual's resistance to earth damage; temporarily added to the player's EarthDR.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedFireDR,"The individual's resistance to fire damage; temporarily added to the player's FireDR.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedWaterDR,"The individual's resistance to lightning damage; temporarily added to the player's LightningDR.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedLightningDR,"The individual's resistance to lightning damage; temporarily added to the player's LightningDR.");

	strcpy(thisAbilityCreationInstance->descriptionInstant,"Instant abilities only affect the player for the turn they're cast. They don't require mana to cast.");
	strcpy(thisAbilityCreationInstance->descriptionInstantDamageType,"The damage type the player will receive from the ability: Earth, Fire, Water or Lightning. Does not apply to status damage.");
	strcpy(thisAbilityCreationInstance->descriptionInstantRange,"The physical attack range of the player; temporarily added to the base attack range of their weapon.");
	strcpy(thisAbilityCreationInstance->descriptionInstantExtraAttack,"The number of physical attacks the player will do.");
	strcpy(thisAbilityCreationInstance->descriptionInstantDiceDamage,"Random damage that the player receives when casting the ability. Can be used to lessen the cost of the ability at the risk hp loss. Cumulative with Damage.");
	strcpy(thisAbilityCreationInstance->descriptionInstantDiceDamageMult,"Multiplies the random damage.");
	strcpy(thisAbilityCreationInstance->descriptionInstantDamage,"Damage that the player receives when casting the ability. Can be used to lessen the cost of the ability at the risk hp loss. Cumulative with DiceDamage.");
	strcpy(thisAbilityCreationInstance->descriptionInstantStatus,"Status affliction that only the player receives when casting the ability.  Can be used to lessen the cost of the ability at the risk of a harmful status.");
	strcpy(thisAbilityCreationInstance->descriptionInstantStatusDiceDamage,"Random damage that only the player receives for the duration of the status when casting the ability. Can be used to lessen the cost of the ability at the risk hp loss. Cumulative with StatusDamage.");
	strcpy(thisAbilityCreationInstance->descriptionInstantStatusDamage,"Damage that only the player receives for the duration of the status when casting the ability. Can be used to lessen the cost of the ability at the risk hp loss. Cumulative with StatusDiceDamage.");
	strcpy(thisAbilityCreationInstance->descriptionInstantStatusDiceDuration,"The random number of turns the status will last. Cumulative with StatusDiceDuration.");
	strcpy(thisAbilityCreationInstance->descriptionInstantStatusDuration,"The number of turns the status will last. Cumulative with StatusDuration.");
	strcpy(thisAbilityCreationInstance->descriptionInstantActions,"The number of actions the player forfeits by using the ability. Useful to reduce the cost of the ability at the risk of not being able to move afterwords.");
	strcpy(thisAbilityCreationInstance->descriptionInstantAttack,"Additional points of Attack; temporarily added to the player's base Attack.");
	strcpy(thisAbilityCreationInstance->descriptionInstantDamageMod,"Extra damage done by the player; temporarily added to the player's base Damage.");
	strcpy(thisAbilityCreationInstance->descriptionInstantMvmt,"How far the player can move; temporarily added to the player's base Mvmt.");
	strcpy(thisAbilityCreationInstance->descriptionInstantDiceHp,"Heals the player a random number of Hp.");
	strcpy(thisAbilityCreationInstance->descriptionInstantHp,"Heals the player a set number of Hp.");
	strcpy(thisAbilityCreationInstance->descriptionInstantBaseHp,"Increases the player's total HP; temporarily added to the player's base HP (Does not heal the player).");
}

void initAbilityCreationInstance(int imageID, COLORREF rgb, int x, int y, char* directory, char* effectsFileNam){
	int i;

	thisAbilityCreationInstance = malloc(sizeof(abilityCreationInstance));
	thisAbilityCreationInstance->inCreateMode = 0;
	thisAbilityCreationInstance->inNameMode = 0;
	thisAbilityCreationInstance->templateIndex = 0;

	thisAbilityCreationInstance->waitingForName = 0;
	thisAbilityCreationInstance->createMode = ABILITY_CREATE_DEFAULT;
	thisAbilityCreationInstance->selectedType = ABILITY_TYPE;

	thisAbilityCreationInstance->currentTemplateIndex = 0;
	thisAbilityCreationInstance->MAX_ABILITY_TEMPLATES = 4;
	thisAbilityCreationInstance->numAbilityTemplates = 0;

	thisAbilityCreationInstance->MAX_FIELDS_ON_WINDOW = 4;
	thisAbilityCreationInstance->EFFECT_FIELD_DRAW_HEIGHT = 386;

	thisAbilityCreationInstance->effectIndex = 0;
	thisAbilityCreationInstance->effectIndexOffset = 0;

	thisAbilityCreationInstance->effectCurrentIndex = 0;
	thisAbilityCreationInstance->effectStartingIndex = 0;
	thisAbilityCreationInstance->effectEndingIndex = thisAbilityCreationInstance->MAX_FIELDS_ON_WINDOW;
	thisAbilityCreationInstance->mode = DEFAULT_ABILITY;
	thisAbilityCreationInstance->idCounter = 0;
	thisAbilityCreationInstance->bonusMana = 0;
	thisAbilityCreationInstance->totalBonusMana = 0;

	thisAbilityCreationInstance->creationWindow = createCharacter(imageID, rgb, x, y);

	thisAbilityCreationInstance->abilityEntrySelect = createCharacter(1422, rgb, x, y);
	thisAbilityCreationInstance->abilityEntryEdit = createCharacter(1423, rgb, x, y);
	thisAbilityCreationInstance->abilityTypeBox = createCharacter(1435, rgb, x, y);
	thisAbilityCreationInstance->abilityTypeBoxSelected = createCharacter(1437, rgb, x, y);
	thisAbilityCreationInstance->abilitySlider2Un = createCharacter(1425, rgb, x, y);
	thisAbilityCreationInstance->abilitySlider4Un = createCharacter(1426, rgb, x, y);
	thisAbilityCreationInstance->abilitySlider5B = createCharacter(1427, rgb, x, y);
	thisAbilityCreationInstance->abilitySlider6B2R = createCharacter(1436, rgb, x, y);
	thisAbilityCreationInstance->abilitySlider6UnB = createCharacter(1428, rgb, x, y);
	thisAbilityCreationInstance->abilitySlider6UnR = createCharacter(1429, rgb, x, y);
	thisAbilityCreationInstance->abilitySlider7B = createCharacter(1430, rgb, x, y);
	thisAbilityCreationInstance->abilitySlider9B3R = createCharacter(1431, rgb, x, y);
	thisAbilityCreationInstance->abilitySlider10B4R = createCharacter(1432, rgb, x, y);
	thisAbilityCreationInstance->abilitySlider11B = createCharacter(1433, rgb, x, y);
	thisAbilityCreationInstance->abilitySlider11Un = createCharacter(1434, rgb, x, y);

	thisAbilityCreationInstance->selector = createCharacter(1504, rgb, x, y);
	thisAbilityCreationInstance->leftRightArrow = createCharacter(9502, rgb, x, y);
	thisAbilityCreationInstance->scrollUpArrow = createCharacter(1505, rgb, x, y);
	thisAbilityCreationInstance->scrollDownArrow = createCharacter(1507, rgb, x, y);

	loadTemplateAbilities(directory, effectsFileNam);

	setUpDescriptions();

	thisAbilityCreationInstance->abilityInsance = cloneAbility(thisAbilityCreationInstance->abilityTemplates[thisAbilityCreationInstance->templateIndex]) ;
}

void loadTemplateAbilities(char* directory, char* effectsFileName){
	int i;
	char * fullFileName = appendStrings(directory, effectsFileName);
	FILE * FP = fopen(fullFileName, "r");
	char line[2048];

	i = 0;
	while(fgets(line,2048,FP)){
		if(i >= thisAbilityCreationInstance->MAX_ABILITY_TEMPLATES){
			cwrite("!! CANNOT ADD ABILITY TEMPLATES - MAXIMUM LEVEL MET !!");
			break;
		}

		ability * tmpAbility = createAbilityFromLine(line);
		addTemplateAbilityToRegistry(tmpAbility);
		thisAbilityCreationInstance->abilityTemplates[i] = cloneAbility(tmpAbility);
		thisAbilityCreationInstance->abilityTemplates[i]->ID = tmpAbility->ID;
		i++;
	}

	thisAbilityCreationInstance->numAbilityTemplates = i;

	fclose(FP);
	free(fullFileName);
}

void setAbilityCreationIDCounter(int startingCount){
	thisAbilityCreationInstance->idCounter = startingCount;
}

ability * getTemplateAbility(int ID){
	switch(ID){
		case 0:
		case 1:
		case 2:
		case 3:
			return thisAbilityCreationInstance->abilityTemplates[ID];
	}

	char errlog[32];
	sprintf(errlog, "!! ABILITY TEMPLATE ID NOT FOUND: %d !!", ID);
	cwrite(errlog);

	return NULL;
}

void enableAbilityCreateMode(int bonusMana, abilityCreationMode createMode, creation_modes createModeType, ability * abilityToUpgrade){
	thisAbilityCreationInstance->createMode = createMode;
	thisAbilityCreationInstance->mode = createModeType;
	thisAbilityCreationInstance->bonusMana = bonusMana;
	thisAbilityCreationInstance->totalBonusMana = bonusMana;

	createMode == ABILITY_CREATE_EXCEPT_PERMENANT ? thisAbilityCreationInstance->templateIndex = 1 : 0;

	if(thisAbilityCreationInstance->mode == DEFAULT_ABILITY){
		free(thisAbilityCreationInstance->abilityInsance);
	}

	if(createModeType == LEVELUP_ABILITY){
		thisAbilityCreationInstance->abilityInsance = cloneAbility(abilityToUpgrade);
		thisAbilityCreationInstance->abilityLevelUpOriginal = abilityToUpgrade;

		thisAbilityCreationInstance->abilityInsance->ID = thisAbilityCreationInstance->idCounter;
		thisAbilityCreationInstance->idCounter++;

		thisAbilityCreationInstance->abilityInsance->level++;
	}else{
		thisAbilityCreationInstance->abilityInsance = cloneAbility(thisAbilityCreationInstance->abilityTemplates[thisAbilityCreationInstance->templateIndex]);
	}

	thisAbilityCreationInstance->abilityInsance->totalManaCost = calculateManaCost(thisAbilityCreationInstance->abilityInsance, thisAbilityCreationInstance->totalBonusMana - thisAbilityCreationInstance->bonusMana);

	thisAbilityCreationInstance->selectedType = ABILITY_TYPE;
	thisAbilityCreationInstance->inCreateMode = 1;
}

void disableAbilityCreateMode(){
	while(tryGetAbilityCreationReadLock()){}
	while(tryGetAbilityCreationWriteLock()){}

	thisAbilityCreationInstance->inCreateMode = 0;

	releaseAbilityCreationWriteLock();
	releaseAbilityCreationReadLock();
}

int inAbilityCreateMode(){
	if(thisAbilityCreationInstance != NULL){
		return thisAbilityCreationInstance->inCreateMode;
	}else{
		return 0;
	}
}

void changeAbilityTemplate(int shift){
	if(thisAbilityCreationInstance->createMode == ABILITY_CREATE_PERMENANT_ONLY){
		return;
	}

	int newIndex = thisAbilityCreationInstance->templateIndex + shift;
	newIndex = newIndex < 0 ? thisAbilityCreationInstance->numAbilityTemplates -1 : newIndex % (thisAbilityCreationInstance->numAbilityTemplates);

	if(newIndex == 0 && thisAbilityCreationInstance->createMode == ABILITY_CREATE_EXCEPT_PERMENANT){
		if(shift > 0){
			newIndex++;
		}else{
			newIndex = thisAbilityCreationInstance->numAbilityTemplates - 1;
		}
	}

	thisAbilityCreationInstance->templateIndex = newIndex;

	if(thisAbilityCreationInstance->mode == DEFAULT_ABILITY){
		free(thisAbilityCreationInstance->abilityInsance);
	}else if(thisAbilityCreationInstance->mode == LEVELUP_ABILITY){
//		free(thisAbilityCreationInstance->abilityLevelUpOriginal);
		thisAbilityCreationInstance->abilityLevelUpOriginal = NULL;
	}

	thisAbilityCreationInstance->abilityInsance = cloneAbility(thisAbilityCreationInstance->abilityTemplates[thisAbilityCreationInstance->templateIndex]) ;
}

void drawManaCost(HDC hdcBuffer, ability * thisAbility, RECT * manaRect){
	char * manaCost[16];

	sprintf(manaCost, "Mana: %d", thisAbility->totalManaCost);

	DrawText(hdcBuffer, manaCost, strlen(manaCost), manaRect, DT_SINGLELINE);
}

void drawAbilityCreateWindow(HDC hdc, HDC hdcBuffer, RECT * prc){
	int effectIndex;
	HDC hdcMem = CreateCompatibleDC(hdc);
	RECT descriptionRect, textTmp;

	int xOff = thisAbilityCreationInstance->creationWindow->x;
	int yOff = thisAbilityCreationInstance->creationWindow->y;

	int currentDrawHeight = 52;
	int typeBoxOffset = 0;

	descriptionRect.top = yOff + 470;
	descriptionRect.left = xOff + 10;
	descriptionRect.bottom = descriptionRect.top + 250;
	descriptionRect.right = descriptionRect.left + 310;

	textTmp.top = yOff + 84;
	textTmp.left = xOff + 20;
	textTmp.bottom = textTmp.top + 200;
	textTmp.right = textTmp.left + 240;

	//draw create window
	SelectObject(hdcMem, thisAbilityCreationInstance->creationWindow->fixedImage);
	BitBlt(hdcBuffer, xOff, yOff, thisAbilityCreationInstance->creationWindow->fixedWidth, thisAbilityCreationInstance->creationWindow->fixedHeight, hdcMem, 0, 0, SRCCOPY);

	SetTextColor(hdcBuffer, RGB(255, 200, 0));

	char tmpLine[128];
	SIZE size;

	sprintf(tmpLine, "Type:");

	GetTextExtentPoint32(hdcBuffer, tmpLine, strlen(tmpLine), &size);

	if(thisAbilityCreationInstance->selectedType == ABILITY_TYPE){
		drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 60 + size.cx, yOff + 82, thisAbilityCreationInstance->abilityTypeBoxSelected);
		typeBoxOffset = (thisAbilityCreationInstance->abilityTypeBoxSelected->fixedWidth / 2);
	}else{
		drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 60 + size.cx, yOff + 82, thisAbilityCreationInstance->abilityTypeBox);
		typeBoxOffset = (thisAbilityCreationInstance->abilityTypeBox->fixedWidth / 2);
	}

	HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, DEFAULT_QUALITY, 0, "System");
	HFONT oldFont = SelectObject(hdcBuffer, hFont);
	SetTextColor(hdcBuffer, RGB(255, 200, 0));
	SetBkMode(hdcBuffer, TRANSPARENT);

	DrawText(hdcBuffer, tmpLine, -1, &textTmp, DT_SINGLELINE);

	textTmp.top = yOff + 105;
	textTmp.bottom = textTmp.top + 200;


	drawNewGameFormText(hdcBuffer, &textTmp, xOff + 60 + size.cx + typeBoxOffset, yOff + 85, thisAbilityCreationInstance->abilityInsance->typeName);

	drawNewGameFormNumber(hdcBuffer, &textTmp, xOff + 175, yOff + 39, thisAbilityCreationInstance->abilityInsance->totalManaCost);

	SelectObject(hdcBuffer, oldFont);

	DeleteObject(hFont);
	SetTextColor(hdcBuffer, RGB(0, 0, 0));

	//scroll up arrow
	if(thisAbilityCreationInstance->effectStartingIndex > 0){
//		drawUnboundCharacterByPixels(hdc, hdcBuffer,thisAbilityCreationInstance->creationWindow->x + 40,
//				thisAbilityCreationInstance->creationWindow->y + 57, thisAbilityCreationInstance->scrollUpArrow);
	}

	//scroll down arrow
	if(thisAbilityCreationInstance->effectEndingIndex < totalVisableFields()){
//		drawUnboundCharacterByPixels(hdc, hdcBuffer,thisAbilityCreationInstance->creationWindow->x + 40,
//				thisAbilityCreationInstance->creationWindow->y + 148 + 17 * thisAbilityCreationInstance->MAX_FIELDS_ON_WINDOW,
//				thisAbilityCreationInstance->scrollDownArrow);
	}

	yOff += 80;

	effectIndex = 0;

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->rangeEnabled,
			 hdc, hdcBuffer, ABILITY_RANGE, xOff, yOff, "Range", 0, thisAbilityCreationInstance->abilityInsance->range, &currentDrawHeight);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->targetedEnabled,
			 hdc, hdcBuffer, ABILITY_TARGETED, xOff, yOff, "Target", 0, thisAbilityCreationInstance->abilityInsance->targeted, &currentDrawHeight);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->extraAttackEnabled,
			 hdc, hdcBuffer, ABILITY_EXTRA_ATTACK, xOff, yOff, "Extra Attack", 0, thisAbilityCreationInstance->abilityInsance->extraAttack, &currentDrawHeight);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->diceDamageEnabled,
			 hdc, hdcBuffer, ABILITY_DICE_DAMAGE, xOff, yOff, "Dice Damage", 0, thisAbilityCreationInstance->abilityInsance->diceDamage, &currentDrawHeight);

	if(thisAbilityCreationInstance->abilityInsance->diceDamageEnabled
			&& thisAbilityCreationInstance->abilityInsance->diceDamage->effectAndManaArray[thisAbilityCreationInstance->abilityInsance->diceDamage->selectedIndex]->effectMagnitude > 0){
//		textRect.left += 20;
		processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->diceDamageMultiplierEnabled,
					 hdc, hdcBuffer, ABILITY_DICE_DAMAGE_MULTIPLIER, xOff, yOff, "Dice Damage Multiplier", 0, thisAbilityCreationInstance->abilityInsance->diceDamageMultiplier, &currentDrawHeight);
//		textRect.left -= 20;
	}

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->damageEnabled,
			 hdc, hdcBuffer, ABILITY_DAMAGE, xOff, yOff, "damage", 0, thisAbilityCreationInstance->abilityInsance->damage, &currentDrawHeight);

	if((thisAbilityCreationInstance->abilityInsance->diceDamageEnabled
			&& thisAbilityCreationInstance->abilityInsance->diceDamage->effectAndManaArray[thisAbilityCreationInstance->abilityInsance->diceDamage->selectedIndex]->effectMagnitude > 0)
			|| (thisAbilityCreationInstance->abilityInsance->damageEnabled
			&& thisAbilityCreationInstance->abilityInsance->damage->effectAndManaArray[thisAbilityCreationInstance->abilityInsance->damage->selectedIndex]->effectMagnitude > 0)){
		processTypeMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->damageTypeEnabled,
			 hdc, hdcBuffer, ABILITY_DAMAGE_TYPE, xOff + 20, yOff, "Damage Type", 0, thisAbilityCreationInstance->abilityInsance->damageType, &currentDrawHeight);
	}

	if(thisAbilityCreationInstance->abilityInsance->statusEnabled){
		processTypeMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->statusEnabled,
				hdc, hdcBuffer, ABILITY_STATUS, xOff, yOff, "Status", 0, thisAbilityCreationInstance->abilityInsance->status, &currentDrawHeight);

		char type[16];
		strcpy(type, thisAbilityCreationInstance->abilityInsance->status->typeAndManaArray[thisAbilityCreationInstance->abilityInsance->status->selectedIndex]->type);

		//Dont show status options when status is type 'None'
		if(strcmp(type, "None") != 0){
			processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->diceStatusDurationEnabled,
					 hdc, hdcBuffer, ABILITY_STATUS_DICE_DURATION, xOff + 20, yOff, "Dice Duration", 0, thisAbilityCreationInstance->abilityInsance->diceStatusDuration, &currentDrawHeight);

			processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->statusDurationEnabled,
					 hdc, hdcBuffer, ABILITY_STATUS_DURATION, xOff + 20, yOff, "Duration", 0, thisAbilityCreationInstance->abilityInsance->statusDuration, &currentDrawHeight);

			//only show damage options for damaging statuses.
			if(strcmp(type, "Poison") == 0 || strcmp(type, "Burn") == 0 || strcmp(type, "Bleed") == 0){
				processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->statusDiceDamageEnabled,
						 hdc, hdcBuffer, ABILITY_STATUS_DICE_DAMAGE, xOff + 20, yOff, "Dice Damage", 0, thisAbilityCreationInstance->abilityInsance->statusDiceDamage, &currentDrawHeight);

				processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->statusDamageEnabled,
						 hdc, hdcBuffer, ABILITY_STATUS_DAMAGE, xOff + 20, yOff, "Damage", 0, thisAbilityCreationInstance->abilityInsance->statusDamage, &currentDrawHeight);
			}
		}
	}

	if(!thisAbilityCreationInstance->abilityInsance->aoeLineEnabled || thisAbilityCreationInstance->abilityInsance->aoeLine->effectAndManaArray[thisAbilityCreationInstance->abilityInsance->aoeLine->selectedIndex]->effectMagnitude == 0){
		processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->aoeNovaEnabled,
				hdc, hdcBuffer, ABILITY_AOE_NOVA, xOff, yOff, "AoE Nova", 0, thisAbilityCreationInstance->abilityInsance->aoeNova, &currentDrawHeight);
	}

	if(!thisAbilityCreationInstance->abilityInsance->aoeNovaEnabled || thisAbilityCreationInstance->abilityInsance->aoeNova->effectAndManaArray[thisAbilityCreationInstance->abilityInsance->aoeNova->selectedIndex]->effectMagnitude == 0){
		processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->aoeLineEnabled,
				 hdc, hdcBuffer, ABILITY_AOE_LINE, xOff, yOff, "AoE Line", 0, thisAbilityCreationInstance->abilityInsance->aoeLine, &currentDrawHeight);
	}

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->durationEnabled,
				 hdc, hdcBuffer, ABILITY_DURATION, xOff, yOff, "duration", 0, thisAbilityCreationInstance->abilityInsance->duration, &currentDrawHeight);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->durationModEnabled,
				 hdc, hdcBuffer, ABILITY_DURATION_MOD, xOff, yOff, "durationMod", 0, thisAbilityCreationInstance->abilityInsance->durationMod, &currentDrawHeight);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->actionsEnabled,
					 hdc, hdcBuffer, ABILITY_ACTIONS, xOff, yOff, "Actions", 0, thisAbilityCreationInstance->abilityInsance->actions, &currentDrawHeight);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->acEnabled,
				 hdc, hdcBuffer, ABILITY_AC, xOff, yOff, "ac", 0, thisAbilityCreationInstance->abilityInsance->ac, &currentDrawHeight);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->attackEnabled,
					 hdc, hdcBuffer, ABILITY_ATTACK, xOff, yOff, "attack", 0, thisAbilityCreationInstance->abilityInsance->attack, &currentDrawHeight);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->damageModEnabled,
				 hdc, hdcBuffer, ABILITY_DAMAGE_MOD, xOff, yOff, "damageMod", 0, thisAbilityCreationInstance->abilityInsance->damageMod, &currentDrawHeight);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->mvmtEnabled,
				 hdc, hdcBuffer, ABILITY_MVMT, xOff, yOff, "movement", 0, thisAbilityCreationInstance->abilityInsance->mvmt, &currentDrawHeight);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->diceHPEnabled,
				 hdc, hdcBuffer, ABILITY_DICE_HP, xOff, yOff, "diceHP", 0, thisAbilityCreationInstance->abilityInsance->diceHP, &currentDrawHeight);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->hpEnabled,
				 hdc, hdcBuffer, ABILITY_HP, xOff, yOff, "hp", 0, thisAbilityCreationInstance->abilityInsance->hp, &currentDrawHeight);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->baseHPEnabled,
				 hdc, hdcBuffer, ABILITY_BASE_HP, xOff, yOff, "baseHP", 0, thisAbilityCreationInstance->abilityInsance->baseHP, &currentDrawHeight);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->baseManaEnabled,
				 hdc, hdcBuffer, ABILITY_BASE_MANA, xOff, yOff, "baseMana", 0, thisAbilityCreationInstance->abilityInsance->baseMana, &currentDrawHeight);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->foodEnabled,
				 hdc, hdcBuffer, ABILITY_FOOD, xOff, yOff, "food", 0, thisAbilityCreationInstance->abilityInsance->food, &currentDrawHeight);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->baseFoodEnabled,
				 hdc, hdcBuffer, ABILITY_BASE_FOOD, xOff, yOff, "baseFood", 0, thisAbilityCreationInstance->abilityInsance->baseFood, &currentDrawHeight);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->LoSEnabled,
				 hdc, hdcBuffer, ABILITY_LOS, xOff, yOff, "Line of Sight", 0, thisAbilityCreationInstance->abilityInsance->LoS, &currentDrawHeight);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->bluntDREnabled,
				 hdc, hdcBuffer, ABILITY_BLUNT_DR, xOff, yOff, "bluntDR", thisAbilityCreationInstance->abilityInsance->type == 't'? 2:1, thisAbilityCreationInstance->abilityInsance->bluntDR, &currentDrawHeight);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->chopDREnabled,
				 hdc, hdcBuffer, ABILITY_CHOP_DR, xOff, yOff, "chopDR", thisAbilityCreationInstance->abilityInsance->type == 't'? 2:1, thisAbilityCreationInstance->abilityInsance->chopDR, &currentDrawHeight);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->pierceDREnabled,
				 hdc, hdcBuffer, ABILITY_PIERCE_DR, xOff, yOff, "pierceDR", thisAbilityCreationInstance->abilityInsance->type == 't'? 2:1, thisAbilityCreationInstance->abilityInsance->pierceDR, &currentDrawHeight);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->slashDREnabled,
				 hdc, hdcBuffer, ABILITY_SLASH_DR, xOff, yOff, "slashDR", thisAbilityCreationInstance->abilityInsance->type == 't'? 2:1, thisAbilityCreationInstance->abilityInsance->slashDR, &currentDrawHeight);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->earthDREnabled,
				 hdc, hdcBuffer, ABILITY_EARTH_DR, xOff, yOff, "earthDR", thisAbilityCreationInstance->abilityInsance->type == 't'? 2:1, thisAbilityCreationInstance->abilityInsance->earthDR, &currentDrawHeight);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->fireDREnabled,
				 hdc, hdcBuffer, ABILITY_FIRE_DR, xOff, yOff, "fireDR", thisAbilityCreationInstance->abilityInsance->type == 't'? 2:1, thisAbilityCreationInstance->abilityInsance->fireDR, &currentDrawHeight);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->waterDREnabled,
				 hdc, hdcBuffer, ABILITY_WATER_DR, xOff, yOff, "waterDR", thisAbilityCreationInstance->abilityInsance->type == 't'? 2:1, thisAbilityCreationInstance->abilityInsance->waterDR, &currentDrawHeight);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->lightningDREnabled,
				 hdc, hdcBuffer, ABILITY_LIGHTNING_DR, xOff, yOff, "lightningDR", thisAbilityCreationInstance->abilityInsance->type == 't'? 2:1, thisAbilityCreationInstance->abilityInsance->lightningDR, &currentDrawHeight);

	SetTextColor(hdcBuffer, RGB(255, 200, 0));
	SetBkMode(hdcBuffer, TRANSPARENT);

	drawAbilityCreateDesciption(hdcBuffer, &descriptionRect);

	SetTextColor(hdcBuffer, RGB(0, 0, 0));

	DeleteDC(hdcMem);
}

void drawAbilityCreateDesciption(HDC hdcBuffer, RECT * descriptionRect){
	switch(thisAbilityCreationInstance->selectedType){
	case ABILITY_TYPE:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'p':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionPermanent, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDuration, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargeted, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'i':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionInstant, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_DAMAGE_TYPE:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedDamageType, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'i':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionInstantDamageType, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_RANGE:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'p':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionPermanentRange, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationRange, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedRange, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'i':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionInstantRange, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_TARGETED:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedTargeted, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_EXTRA_ATTACK:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'i':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionInstantExtraAttack, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_DICE_DAMAGE:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationDiceDamage, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedDiceDamage, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'i':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionInstantDiceDamage, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_DICE_DAMAGE_MULTIPLIER:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationDiceDamageMult, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedDiceDamageMult, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'i':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionInstantDiceDamageMult, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_DAMAGE:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationDamage, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedDamage, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'i':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionInstantDamage, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_STATUS:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationStatus, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedStatus, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'i':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionInstantStatus, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_STATUS_DAMAGE:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationStatusDamage, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedStatusDamage, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'i':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionInstantStatusDamage, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_STATUS_DICE_DAMAGE:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationStatusDiceDamage, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedStatusDiceDamage, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'i':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionInstantStatusDiceDamage, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_STATUS_DICE_DURATION:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationStatusDiceDuration, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedStatusDiceDuration, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'i':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionInstantStatusDiceDuration, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_STATUS_DURATION:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationStatusDuration, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedStatusDuration, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'i':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionInstantStatusDuration, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_AOE_NOVA:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationAOENova, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedAOENova, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
		case ABILITY_AOE_LINE:
			switch(thisAbilityCreationInstance->abilityInsance->type){
				case 'd':
					DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationAOELine, -1, descriptionRect, DT_WORDBREAK);
					break;
				case 't':
					DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedAOELine, -1, descriptionRect, DT_WORDBREAK);
					break;
			}
			break;
	case ABILITY_DURATION:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationDuration, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedDuration, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_DURATION_MOD:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationDiceDuration, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedDiceDuration, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_ACTIONS:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationActions, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedActions, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'i':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionInstantActions, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_AC:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'p':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionPermanentAC, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationAC, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedAC, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_ATTACK:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'p':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionPermanentAttack, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationAttack, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedAttack, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'i':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionInstantAttack, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_DAMAGE_MOD:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'p':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionPermanentDamageMod, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationDamageMod, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedDamageMod, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'i':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionInstantDamageMod, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_MVMT:
		switch(thisAbilityCreationInstance->abilityInsance->type){
		case 'p':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionPermanentMvmt, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationMvmt, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedMvmt, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'i':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionInstantMvmt, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_DICE_HP:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationDiceHp, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedDiceHp, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_HP:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationHp, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedHp, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_BASE_HP:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'p':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionPermanentBaseHp, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationBaseHp, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedBaseHp, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_BASE_MANA:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'p':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionPermanentBaseMana, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationBaseMana, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedBaseMana, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_BLUNT_DR:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'p':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionPermanentBluntDR, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationBluntDR, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedBluntDR, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_CHOP_DR:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'p':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionPermanentChopDR, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationChopDR, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedChopDR, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_PIERCE_DR:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'p':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionPermanentPierceDR, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationPierceDR, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedPierceDR, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_SLASH_DR:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'p':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionPermanentSlashDR, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationSlashDR, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedSlashDR, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_EARTH_DR:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'p':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionPermanentEarthDR, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationEarthDR, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedEarthDR, -1, descriptionRect, DT_WORDBREAK);
				break;
		}break;
	case ABILITY_FIRE_DR:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'p':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionPermanentFireDR, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationFireDR, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedFireDR, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_WATER_DR:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'p':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionPermanentWaterDR, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationWaterDR, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedWaterDR, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_LIGHTNING_DR:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'p':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionPermanentLightningDR, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationLightningDR, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedLightningDR, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	}
}

fixedCharacter * getSliderFromType(effect_types type){
	switch (type){
	case ABILITY_TARGETED:
		return thisAbilityCreationInstance->abilitySlider2Un;

	case ABILITY_DICE_DAMAGE:
	case ABILITY_DAMAGE:
	case ABILITY_STATUS_DAMAGE:
	case ABILITY_STATUS_DICE_DAMAGE:
		if(thisAbilityCreationInstance->abilityInsance->type == 't'){
			return thisAbilityCreationInstance->abilitySlider6UnB;
		}else{
			return thisAbilityCreationInstance->abilitySlider6UnR;
		}

	case ABILITY_STATUS_DURATION:
	case ABILITY_DURATION_MOD:
		return thisAbilityCreationInstance->abilitySlider11Un;

	case ABILITY_ACTIONS:
		return thisAbilityCreationInstance->abilitySlider6UnR;

	case ABILITY_AC:
		return thisAbilityCreationInstance->abilitySlider5B;

	case ABILITY_ATTACK:
		if(thisAbilityCreationInstance->abilityInsance->type == 'i'){
			return thisAbilityCreationInstance->abilitySlider4Un;
		}else{
			return thisAbilityCreationInstance->abilitySlider7B;
		}

	case ABILITY_DAMAGE_MOD:
		if(thisAbilityCreationInstance->abilityInsance->type == 'p' || thisAbilityCreationInstance->abilityInsance->type == 't'){
			return thisAbilityCreationInstance->abilitySlider9B3R;
		}else{
			return thisAbilityCreationInstance->abilitySlider6UnB;
		}

	case ABILITY_MVMT:
		if(thisAbilityCreationInstance->abilityInsance->type == 'p' || thisAbilityCreationInstance->abilityInsance->type == 't'){
			return thisAbilityCreationInstance->abilitySlider11B;
		}else{
			return thisAbilityCreationInstance->abilitySlider6UnB;
		}

	case ABILITY_STATUS_DICE_DURATION:
	case ABILITY_DURATION:
	case ABILITY_HP:
		return thisAbilityCreationInstance->abilitySlider6UnB;

	case ABILITY_BASE_HP:
	case ABILITY_BASE_MANA:
		if(thisAbilityCreationInstance->abilityInsance->type == 'p' || thisAbilityCreationInstance->abilityInsance->type == 't'){
			return thisAbilityCreationInstance->abilitySlider11B;
		}else{
			return thisAbilityCreationInstance->abilitySlider6UnB;
		}

	case ABILITY_FOOD:
	case ABILITY_BASE_FOOD:
		return thisAbilityCreationInstance->abilitySlider6B2R;

	case ABILITY_RANGE:
	case ABILITY_EXTRA_ATTACK:
	case ABILITY_DICE_DAMAGE_MULTIPLIER:
	case ABILITY_AOE_NOVA:
	case ABILITY_AOE_LINE:
	case ABILITY_DICE_HP:
	case ABILITY_LOS:
		return thisAbilityCreationInstance->abilitySlider4Un;

	case ABILITY_BLUNT_DR:
	case ABILITY_CHOP_DR:
	case ABILITY_PIERCE_DR:
	case ABILITY_SLASH_DR:
	case ABILITY_EARTH_DR:
	case ABILITY_FIRE_DR:
	case ABILITY_WATER_DR:
	case ABILITY_LIGHTNING_DR:
		if(thisAbilityCreationInstance->abilityInsance->type == 'd'){
			return thisAbilityCreationInstance->abilitySlider6UnB;
		}else{
			return thisAbilityCreationInstance->abilitySlider11B;
		}
		break;
	}

	return NULL;
}

void drawTextEffectMapListSlider(HDC hdc, HDC hdcBuffer, int xOff, int yOff, char * fieldName, effect_types type, int isDR, int currentDrawHeight, typeAndManaMapList * mapList){
	char fullField[256];
	int typeBoxOffset = 0;
	RECT textBox;
	textBox.top = yOff + currentDrawHeight;
	textBox.left = xOff + 20;
	textBox.bottom = textBox.top + 100;
	textBox.right = textBox.left + thisAbilityCreationInstance->creationWindow->fixedWidth;

	typeAndMana * effect = mapList->typeAndManaArray[mapList->selectedIndex];

	sprintf(fullField, "%s:", fieldName);

	if(thisAbilityCreationInstance->selectedType == type){
		drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 34, yOff + currentDrawHeight + 33, thisAbilityCreationInstance->abilityTypeBoxSelected);
		typeBoxOffset = (thisAbilityCreationInstance->abilityTypeBoxSelected->fixedWidth / 2);
	}else{
		drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 34, yOff + currentDrawHeight + 33, thisAbilityCreationInstance->abilityTypeBox);
		typeBoxOffset = (thisAbilityCreationInstance->abilityTypeBox->fixedWidth / 2);
	}

	HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, DEFAULT_QUALITY, 0, "System");
	HFONT oldFont = SelectObject(hdcBuffer, hFont);
	SetTextColor(hdcBuffer, RGB(255, 200, 0));
	SetBkMode(hdcBuffer, TRANSPARENT);

	DrawText(hdcBuffer, fullField, -1, &textBox, DT_SINGLELINE);

	drawNewGameFormText(hdcBuffer, &textBox, xOff + 34 + typeBoxOffset, yOff + currentDrawHeight + 38, effect->type);

	SelectObject(hdcBuffer, oldFont);
	DeleteObject(hFont);
	SetTextColor(hdcBuffer, RGB(0, 0, 0));
}

void drawEffectMapListSlider(HDC hdc, HDC hdcBuffer, int xOff, int yOff, char * fieldName, effect_types type, int isDR, int currentDrawHeight, effectAndManaMapList * mapList){
	char fullField[256];

	RECT textBox;
	textBox.top = yOff + currentDrawHeight;
	textBox.left = xOff + 20;
	textBox.bottom = textBox.top + 100;
	textBox.right = textBox.left + thisAbilityCreationInstance->creationWindow->fixedWidth;

	effectAndMana * effect = mapList->effectAndManaArray[mapList->selectedIndex];

	fixedCharacter * slider = getSliderFromType(type);

	if(mapList->selectedIndex == mapList->defaultStartingIndex){
		sprintf(fullField, "%s", fieldName);
	}else{
		if(isDR){
			sprintf(fullField, "%s: %d, Mana: (%.1f)", fieldName, effect->effectMagnitude, ((mapList->selectedIndex - mapList->defaultStartingIndex) / 2.0));
		}else{
			sprintf(fullField, "%s: %d, Mana: %d", fieldName, effect->effectMagnitude, effect->manaCost);
		}
	}

	HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, DEFAULT_QUALITY, 0, "System");
	HFONT oldFont = SelectObject(hdcBuffer, hFont);
	SetTextColor(hdcBuffer, RGB(255, 200, 0));
	SetBkMode(hdcBuffer, TRANSPARENT);

	DrawText(hdcBuffer, fullField, -1, &textBox, DT_SINGLELINE);

	SelectObject(hdcBuffer, oldFont);
	DeleteObject(hFont);
	SetTextColor(hdcBuffer, RGB(0, 0, 0));

	if(slider != NULL){
		drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 34, yOff + currentDrawHeight + 33, slider);
	}

	if(thisAbilityCreationInstance->selectedType == type){
		drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 41 + (27 * mapList->selectedIndex), yOff + currentDrawHeight + 33, thisAbilityCreationInstance->abilityEntryEdit);
	}else{
		drawUnboundCharacterByPixels(hdc, hdcBuffer, xOff + 41 + (27 * mapList->selectedIndex), yOff + currentDrawHeight + 33, thisAbilityCreationInstance->abilityEntrySelect);
	}
}

void processEffectMapListRendering(int * effectIndex, int isEnabled, HDC hdc, HDC hdcBuffer, effect_types type, int xOff, int yOff, char * fieldName, int isDR, effectAndManaMapList * mapList, int * currentDrawHeight){
	if(isEnabled){
		if(*effectIndex >= thisAbilityCreationInstance->effectStartingIndex && *effectIndex < thisAbilityCreationInstance->effectEndingIndex && (*currentDrawHeight + 82) < thisAbilityCreationInstance->EFFECT_FIELD_DRAW_HEIGHT){
			drawEffectMapListSlider(hdc, hdcBuffer, xOff, yOff, fieldName, type, isDR, *currentDrawHeight, mapList);
			*currentDrawHeight += 82;
		}
		(*effectIndex)++;
	}
}

void processTypeMapListRendering(int * effectIndex, int isEnabled, HDC hdc, HDC hdcBuffer, effect_types type, int xOff, int yOff, char * fieldName, int isDR, typeAndManaMapList * mapList, int * currentDrawHeight){
	if(isEnabled){
		if(*effectIndex >= thisAbilityCreationInstance->effectStartingIndex && *effectIndex < thisAbilityCreationInstance->effectEndingIndex && (*currentDrawHeight + 82) < thisAbilityCreationInstance->EFFECT_FIELD_DRAW_HEIGHT){
			drawTextEffectMapListSlider(hdc, hdcBuffer, xOff, yOff, fieldName, type, isDR, *currentDrawHeight, mapList);
			*currentDrawHeight += 82;
		}
		(*effectIndex)++;
	}
}


void setAbilityCreationSelectedType(effect_types type){
	thisAbilityCreationInstance->selectedType = type;
}

void drawEffectMapList(HDC hdcBuffer, RECT * textRect, char * fieldName, int isDR, effectAndManaMapList * mapList){
		moveRECTDown(textRect, 17);
		char * tmpString = getEffectAndManaString(fieldName, mapList);
		int cost;
		if(isDR == 1){
			cost= mapList->effectAndManaArray[mapList->selectedIndex]->effectMagnitude;
		}else if(isDR == 2){
			cost= abs(mapList->effectAndManaArray[mapList->selectedIndex]->effectMagnitude);
		}
		else{
			cost= mapList->effectAndManaArray[mapList->selectedIndex]->manaCost;
		}
		if(cost > 0){
			SetTextColor(hdcBuffer, RGB(0, 162, 255));
		}else if (cost < 0){
			SetTextColor(hdcBuffer, RGB(255, 0, 0));
		}

		DrawText(hdcBuffer, tmpString, strlen(tmpString), textRect, DT_SINGLELINE);

		SetTextColor(hdcBuffer, RGB(255, 200, 0));

		free(tmpString);
}

void drawTypeMapList(HDC hdcBuffer, RECT * textRect, char * fieldName, int isDR, typeAndManaMapList * mapList){
		moveRECTDown(textRect, 17);
		char * tmpString[64];

		sprintf(tmpString, "%s: %s", fieldName, mapList->typeAndManaArray[mapList->selectedIndex]->type);

		DrawText(hdcBuffer, tmpString, strlen(tmpString), textRect, DT_SINGLELINE);

		SetTextColor(hdcBuffer, RGB(255, 200, 0));

		free(tmpString);
}

char * getEffectAndManaString(char * propertyName, effectAndManaMapList * map){
	char * toReturn = malloc(sizeof(char) * 64);
	int effect = map->effectAndManaArray[map->selectedIndex]->effectMagnitude;
	int mana = map->effectAndManaArray[map->selectedIndex]->manaCost;

	sprintf(toReturn, "%s: %d, cost: %d", propertyName, effect, mana);

	return toReturn;
}

int abilityEffectValue(effectAndManaMapList * effect){
	if(effect != NULL){
		return effect->effectAndManaArray[effect->selectedIndex]->effectMagnitude;
	}

	return 0;
}

void updateAbilityCreateSelectedType(int goingUp){
	switch(thisAbilityCreationInstance->selectedType){
	case ABILITY_TYPE:
			switch(thisAbilityCreationInstance->abilityInsance->type){
				case 'p':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_TYPE : ABILITY_RANGE;
					break;
				case 'd':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_TYPE : ABILITY_RANGE;
					break;
				case 't':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_TYPE : ABILITY_RANGE;
					break;
				case 'i':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_TYPE : ABILITY_RANGE;
					break;
			}
		break;
	case ABILITY_RANGE:
			switch(thisAbilityCreationInstance->abilityInsance->type){
				case 'p':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_TYPE : ABILITY_AC;
					break;
				case 'd':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_TYPE : ABILITY_DICE_DAMAGE;
					break;
				case 't':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_TYPE : ABILITY_TARGETED;
					break;
				case 'i':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_TYPE : ABILITY_EXTRA_ATTACK;
					break;
			}
		break;
	case ABILITY_TARGETED:
			switch(thisAbilityCreationInstance->abilityInsance->type){
				case 't':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_RANGE : ABILITY_DICE_DAMAGE;
					break;
			}
		break;
	case ABILITY_EXTRA_ATTACK:
			switch(thisAbilityCreationInstance->abilityInsance->type){
				case 'i':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_RANGE : ABILITY_DICE_DAMAGE;
					break;
			}
		break;
	case ABILITY_DICE_DAMAGE:
			if(!goingUp){
				if(abilityEffectValue(thisAbilityCreationInstance->abilityInsance->diceDamage) > 0){
					thisAbilityCreationInstance->selectedType = ABILITY_DICE_DAMAGE_MULTIPLIER;
				}else{
					thisAbilityCreationInstance->selectedType = ABILITY_DAMAGE;
				}
				break;
			}

			switch(thisAbilityCreationInstance->abilityInsance->type){
				case 'd':
					thisAbilityCreationInstance->selectedType = ABILITY_RANGE;
					break;
				case 'i':
					thisAbilityCreationInstance->selectedType = ABILITY_EXTRA_ATTACK;
					break;
				case 't':
					thisAbilityCreationInstance->selectedType = ABILITY_TARGETED;
					break;
			}
		break;
	case ABILITY_DICE_DAMAGE_MULTIPLIER:
			switch(thisAbilityCreationInstance->abilityInsance->type){
				case 'd':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_DICE_DAMAGE : ABILITY_DAMAGE;
					break;
				case 't':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_DICE_DAMAGE : ABILITY_DAMAGE;
					break;
				case 'i':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_DICE_DAMAGE : ABILITY_DAMAGE;
					break;
			}
		break;
	case ABILITY_DAMAGE:
			if(goingUp){
				if(abilityEffectValue(thisAbilityCreationInstance->abilityInsance->diceDamage) > 0){
					thisAbilityCreationInstance->selectedType = ABILITY_DICE_DAMAGE_MULTIPLIER;
				}else{
					thisAbilityCreationInstance->selectedType = ABILITY_DICE_DAMAGE;
				}
				break;
			}

			switch(thisAbilityCreationInstance->abilityInsance->type){
				case 'd':
					thisAbilityCreationInstance->selectedType = ABILITY_STATUS;
					break;
				case 't':
				case 'i':
					if(abilityEffectValue(thisAbilityCreationInstance->abilityInsance->diceDamage) > 0 || abilityEffectValue(thisAbilityCreationInstance->abilityInsance->damage) > 0){
						thisAbilityCreationInstance->selectedType = ABILITY_DAMAGE_TYPE;
					}else{
						thisAbilityCreationInstance->selectedType = ABILITY_STATUS;
					}
					break;
			}
		break;
	case ABILITY_DAMAGE_TYPE:
			switch(thisAbilityCreationInstance->abilityInsance->type){
				case 't':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_DAMAGE : ABILITY_STATUS;
					break;
				case 'i':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_DAMAGE : ABILITY_STATUS;
					break;
			}
		break;
	case ABILITY_STATUS:
			if(goingUp){
				switch(thisAbilityCreationInstance->abilityInsance->type){
				case 'd':
					thisAbilityCreationInstance->selectedType = ABILITY_DAMAGE;
					break;
				case 't':
				case 'i':
					if(abilityEffectValue(thisAbilityCreationInstance->abilityInsance->diceDamage) > 0 || abilityEffectValue(thisAbilityCreationInstance->abilityInsance->damage) > 0){
						thisAbilityCreationInstance->selectedType = ABILITY_DAMAGE_TYPE;
					}else{
						thisAbilityCreationInstance->selectedType = ABILITY_DAMAGE;
					}
					break;
				}
			}else{
				char type[16];
				strcpy(type, thisAbilityCreationInstance->abilityInsance->status->typeAndManaArray[thisAbilityCreationInstance->abilityInsance->status->selectedIndex]->type);

				if(strcmp(type, "None") != 0){
					thisAbilityCreationInstance->selectedType = ABILITY_STATUS_DICE_DURATION;
				}else{
					if(thisAbilityCreationInstance->abilityInsance->type == 'i'){
						thisAbilityCreationInstance->selectedType = ABILITY_ACTIONS;
					}else{
						if(abilityEffectValue(thisAbilityCreationInstance->abilityInsance->aoeLine) > 0){
							thisAbilityCreationInstance->selectedType = ABILITY_AOE_LINE;
						}else{
							thisAbilityCreationInstance->selectedType = ABILITY_AOE_NOVA;
						}
					}
				}
			}

		break;
	case ABILITY_STATUS_DICE_DURATION:
		if(goingUp){
			thisAbilityCreationInstance->selectedType = ABILITY_STATUS;
		}else{
			thisAbilityCreationInstance->selectedType = ABILITY_STATUS_DURATION;
		}
		break;
	case ABILITY_STATUS_DURATION:
		if(goingUp){
			thisAbilityCreationInstance->selectedType = ABILITY_STATUS_DICE_DURATION;
		}else{
			char type[16];
			strcpy(type, thisAbilityCreationInstance->abilityInsance->status->typeAndManaArray[thisAbilityCreationInstance->abilityInsance->status->selectedIndex]->type);


			if(strcmp(type, "Poison") == 0 || strcmp(type, "Burn") == 0 || strcmp(type, "Bleed") == 0){
				thisAbilityCreationInstance->selectedType = ABILITY_STATUS_DICE_DAMAGE;
			}else{
				if(abilityEffectValue(thisAbilityCreationInstance->abilityInsance->aoeLine) > 0){
					thisAbilityCreationInstance->selectedType = ABILITY_AOE_LINE;
				}else{
					thisAbilityCreationInstance->selectedType = ABILITY_AOE_NOVA;
				}
			}
		}
		break;
	case ABILITY_STATUS_DICE_DAMAGE:
		thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_STATUS_DURATION : ABILITY_STATUS_DAMAGE;
		break;
	case ABILITY_STATUS_DAMAGE:
		if(goingUp){
			thisAbilityCreationInstance->selectedType = ABILITY_STATUS_DICE_DAMAGE;
		}else{
			if(thisAbilityCreationInstance->abilityInsance->type == 'i'){
				thisAbilityCreationInstance->selectedType = ABILITY_ACTIONS;
			}else{
				if(abilityEffectValue(thisAbilityCreationInstance->abilityInsance->aoeLine) > 0){
					thisAbilityCreationInstance->selectedType = ABILITY_AOE_LINE;
				}else{
					thisAbilityCreationInstance->selectedType = ABILITY_AOE_NOVA;
				}
			}
		}

		break;
	case ABILITY_AOE_NOVA:
		if(!goingUp){
			if(abilityEffectValue(thisAbilityCreationInstance->abilityInsance->aoeNova) > 0){
				thisAbilityCreationInstance->selectedType = ABILITY_DURATION;
			}else{
				thisAbilityCreationInstance->selectedType = ABILITY_AOE_LINE;
			}
		}else{
			char type[16];
			strcpy(type, thisAbilityCreationInstance->abilityInsance->status->typeAndManaArray[thisAbilityCreationInstance->abilityInsance->status->selectedIndex]->type);
			if(strcmp(type, "None") != 0){
				if(strcmp(type, "Poison") == 0 || strcmp(type, "Burn") == 0 || strcmp(type, "Bleed") == 0){
					thisAbilityCreationInstance->selectedType = ABILITY_STATUS_DAMAGE;
				}else{
					thisAbilityCreationInstance->selectedType = ABILITY_STATUS_DURATION;
				}
			}else{
				thisAbilityCreationInstance->selectedType = ABILITY_STATUS;
			}
		}
		break;
	case ABILITY_AOE_LINE:
		if(!goingUp){
			thisAbilityCreationInstance->selectedType = ABILITY_DURATION;
		}else{
			if(abilityEffectValue(thisAbilityCreationInstance->abilityInsance->aoeLine) > 0){
				char type[16];
				strcpy(type, thisAbilityCreationInstance->abilityInsance->status->typeAndManaArray[thisAbilityCreationInstance->abilityInsance->status->selectedIndex]->type);
				if(strcmp(type, "None") != 0){
					if(strcmp(type, "Poison") == 0 || strcmp(type, "Burn") == 0 || strcmp(type, "Bleed") == 0){
						thisAbilityCreationInstance->selectedType = ABILITY_STATUS_DAMAGE;
					}else{
						thisAbilityCreationInstance->selectedType = ABILITY_STATUS_DURATION;
					}
				}else{
					thisAbilityCreationInstance->selectedType = ABILITY_STATUS;
				}
			}else{
				thisAbilityCreationInstance->selectedType = ABILITY_AOE_NOVA;
			}
		}
		break;
	case ABILITY_DURATION:
		if(goingUp){
			if(abilityEffectValue(thisAbilityCreationInstance->abilityInsance->aoeNova) > 0){
				thisAbilityCreationInstance->selectedType = ABILITY_AOE_NOVA;
			}else{
				thisAbilityCreationInstance->selectedType = ABILITY_AOE_LINE;
			}
		}else{
			thisAbilityCreationInstance->selectedType = ABILITY_DURATION_MOD;
		}
		break;
	case ABILITY_DURATION_MOD:
		thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_DURATION : ABILITY_ACTIONS;
		break;
	case ABILITY_ACTIONS:
			switch(thisAbilityCreationInstance->abilityInsance->type){
				case 'd':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_DURATION_MOD : ABILITY_AC;
					break;
				case 't':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_DURATION_MOD : ABILITY_AC;
					break;
				case 'i':
					if(goingUp){
						char type[16];
						strcpy(type, thisAbilityCreationInstance->abilityInsance->status->typeAndManaArray[thisAbilityCreationInstance->abilityInsance->status->selectedIndex]->type);

						if(strcmp(type, "None") != 0){
							if(strcmp(type, "Poison") == 0 || strcmp(type, "Burn") == 0 || strcmp(type, "Bleed") == 0){
								thisAbilityCreationInstance->selectedType = ABILITY_STATUS_DAMAGE;
							}else{
								thisAbilityCreationInstance->selectedType = ABILITY_STATUS_DURATION;
							}
						}else{
							thisAbilityCreationInstance->selectedType = ABILITY_STATUS;
						}
					}else{
						thisAbilityCreationInstance->selectedType = ABILITY_ATTACK;
					}
					break;
			}
		break;
	case ABILITY_AC:
			switch(thisAbilityCreationInstance->abilityInsance->type){
				case 'p':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_RANGE : ABILITY_ATTACK;
					break;
				case 'd':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_ACTIONS : ABILITY_ATTACK;
					break;
				case 't':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_ACTIONS : ABILITY_ATTACK;
					break;
			}
		break;
	case ABILITY_ATTACK:
			switch(thisAbilityCreationInstance->abilityInsance->type){
				case 'p':
				case 'd':
				case 't':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_AC : ABILITY_DAMAGE_MOD;
					break;
				case 'i':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_ACTIONS : ABILITY_DAMAGE_MOD;
					break;
			}
		break;
	case ABILITY_DAMAGE_MOD:
			thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_ATTACK : ABILITY_MVMT;
		break;
	case ABILITY_MVMT:
			switch(thisAbilityCreationInstance->abilityInsance->type){
				case 'p':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_DAMAGE_MOD : ABILITY_BASE_HP;
					break;
				case 'd':
				case 't':
				case 'i':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_DAMAGE_MOD : ABILITY_DICE_HP;
					break;
			}
		break;
	case ABILITY_DICE_HP:
			switch(thisAbilityCreationInstance->abilityInsance->type){
				case 'd':
				case 't':
				case 'i':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_MVMT : ABILITY_HP;
					break;
			}
		break;
	case ABILITY_HP:
			switch(thisAbilityCreationInstance->abilityInsance->type){
				case 'd':
				case 't':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_DICE_HP : ABILITY_BASE_HP;
					break;
				case 'i':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_DICE_HP : ABILITY_HP;
					break;
			}
		break;
	case ABILITY_BASE_HP:
			switch(thisAbilityCreationInstance->abilityInsance->type){
				case 'p':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_MVMT : ABILITY_BASE_MANA;
					break;
				case 'd':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_HP : ABILITY_BASE_MANA;
					break;
				case 't':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_HP : ABILITY_BASE_MANA;
					break;
			}
		break;
	case ABILITY_BASE_MANA:
			switch(thisAbilityCreationInstance->abilityInsance->type){
				case 'p':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_BASE_HP : ABILITY_BASE_FOOD;
					break;
				case 'd':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_BASE_HP : ABILITY_FOOD;
					break;
				case 't':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_BASE_HP : ABILITY_BLUNT_DR;
					break;
			}
		break;
	case ABILITY_FOOD:
		thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_BASE_MANA : ABILITY_LOS;
		break;
	case ABILITY_BASE_FOOD:
		thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_BASE_MANA : ABILITY_LOS;
		break;
	case ABILITY_LOS:
			switch(thisAbilityCreationInstance->abilityInsance->type){
				case 'p':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_BASE_FOOD : ABILITY_BLUNT_DR;
					break;
				case 'd':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_FOOD : ABILITY_BLUNT_DR;
					break;
			}
		break;
	case ABILITY_BLUNT_DR:
			switch(thisAbilityCreationInstance->abilityInsance->type){
				case 'p':
				case 'd':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_LOS : ABILITY_CHOP_DR;
					break;
				case 't':
					thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_BASE_MANA : ABILITY_CHOP_DR;
					break;
			}
		break;
	case ABILITY_CHOP_DR:
		thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_BLUNT_DR : ABILITY_PIERCE_DR;
		break;
	case ABILITY_PIERCE_DR:
		thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_CHOP_DR : ABILITY_SLASH_DR;
		break;
	case ABILITY_SLASH_DR:
		thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_PIERCE_DR : ABILITY_EARTH_DR;
		break;
	case ABILITY_EARTH_DR:
		thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_SLASH_DR : ABILITY_FIRE_DR;
		break;
	case ABILITY_FIRE_DR:
		thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_EARTH_DR : ABILITY_WATER_DR;
		break;
	case ABILITY_WATER_DR:
		thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_FIRE_DR : ABILITY_LIGHTNING_DR;
		break;
	case ABILITY_LIGHTNING_DR:
		thisAbilityCreationInstance->selectedType = (goingUp) ? ABILITY_WATER_DR : ABILITY_LIGHTNING_DR;
		break;
	}
}

int totalVisableFields(){
	int numFields = thisAbilityCreationInstance->abilityInsance->numEnabledEffects;

	//check for damage type
	if(((thisAbilityCreationInstance->abilityInsance->diceDamageEnabled && abilityEffectValue(thisAbilityCreationInstance->abilityInsance->diceDamage) == 0)
			&& (thisAbilityCreationInstance->abilityInsance->damageEnabled && abilityEffectValue(thisAbilityCreationInstance->abilityInsance->damage) == 0))
			&& thisAbilityCreationInstance->abilityInsance->type != 'd'){
		numFields--;
	}

	//check for dice damage multiplier
	if(thisAbilityCreationInstance->abilityInsance->diceDamageEnabled && abilityEffectValue(thisAbilityCreationInstance->abilityInsance->diceDamage) == 0){
		numFields--;
	}

	//check for status sub options
	if(thisAbilityCreationInstance->abilityInsance->statusEnabled ){
		char type[16];
		strcpy(type, thisAbilityCreationInstance->abilityInsance->status->typeAndManaArray[thisAbilityCreationInstance->abilityInsance->status->selectedIndex]->type);

		if(strcmp(type, "None") != 0){
			if(strcmp(type, "Poison") != 0 && strcmp(type, "Burn") != 0 && strcmp(type, "Bleed") != 0){
				numFields -= 2;
			}
		}else{
			numFields -= 4;
		}
	}

	//check for one aoe field disabled
	if((thisAbilityCreationInstance->abilityInsance->aoeNovaEnabled && abilityEffectValue(thisAbilityCreationInstance->abilityInsance->aoeNova) > 0) || (thisAbilityCreationInstance->abilityInsance->aoeLineEnabled && abilityEffectValue(thisAbilityCreationInstance->abilityInsance->aoeLine) > 0)){
		numFields--;
	}

	return numFields;
}

//void abilityCreateMoveUp(){
//	updateAbilityCreateSelectedType(1);
//
//	if(thisAbilityCreationInstance->effectIndex - 1 < thisAbilityCreationInstance->effectIndexOffset && thisAbilityCreationInstance->effectIndexOffset - 1 >= 0){
//		thisAbilityCreationInstance->effectIndexOffset--;
//		thisAbilityCreationInstance->effectIndex--;
//	}else if(thisAbilityCreationInstance->effectIndex - 1 >= thisAbilityCreationInstance->effectIndexOffset){
//		thisAbilityCreationInstance->effectIndex--;
//	}
//}
//
//void abilityCreateMoveDown(){
//	updateAbilityCreateSelectedType(0);
//
//	int upperIndex = thisAbilityCreationInstance->effectIndexOffset + thisAbilityCreationInstance->MAX_FIELDS_ON_WINDOW;
//
//	if(thisAbilityCreationInstance->effectIndex + 1 > upperIndex && upperIndex + 1 < totalVisableFields()){
//		thisAbilityCreationInstance->effectIndexOffset++;
//		thisAbilityCreationInstance->effectIndex++;
//	}else if(thisAbilityCreationInstance->effectIndex + 1 < upperIndex){
//		thisAbilityCreationInstance->effectIndex++;
//	}
//}

void selectNextEffect(){
	updateAbilityCreateSelectedType(0);

	if(thisAbilityCreationInstance->effectCurrentIndex + 1 > thisAbilityCreationInstance->effectEndingIndex && thisAbilityCreationInstance->effectEndingIndex + 1 <= totalVisableFields()){
		shiftEffectListUp();
	} else if(thisAbilityCreationInstance->effectCurrentIndex + 1 <= thisAbilityCreationInstance->effectEndingIndex){
		thisAbilityCreationInstance->effectCurrentIndex++;
	}
}

void selectPreviousEffect(){
	updateAbilityCreateSelectedType(1);

	if(thisAbilityCreationInstance->effectCurrentIndex - 1 <= thisAbilityCreationInstance->effectStartingIndex && thisAbilityCreationInstance->effectStartingIndex - 1 >= 0){
		shiftEffectListDown();
	}else if(thisAbilityCreationInstance->effectCurrentIndex - 1 >= thisAbilityCreationInstance->effectStartingIndex){
		thisAbilityCreationInstance->effectCurrentIndex--;
	}
}

void shiftEffectListUp(){
	thisAbilityCreationInstance->effectStartingIndex++;
	thisAbilityCreationInstance->effectEndingIndex++;
	thisAbilityCreationInstance->effectCurrentIndex++;
}

void shiftEffectListDown(){
	thisAbilityCreationInstance->effectStartingIndex--;
	thisAbilityCreationInstance->effectEndingIndex--;
	thisAbilityCreationInstance->effectCurrentIndex--;
}

typeAndManaMapList * getTypeMapListFromEffectType(){
	switch(thisAbilityCreationInstance->selectedType){
	case ABILITY_DAMAGE_TYPE:
		return thisAbilityCreationInstance->abilityInsance->damageType;
	case ABILITY_STATUS:
		return thisAbilityCreationInstance->abilityInsance->status;
	}
}

typeAndManaMapList * getTypeMapListFromEffectTypeUpgradeClone(){
	switch(thisAbilityCreationInstance->selectedType){
	case ABILITY_DAMAGE_TYPE:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->damageType;
	case ABILITY_STATUS:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->status;
	}
}

void interpretRightAbilityCreation(){
	if(thisAbilityCreationInstance->selectedType != ABILITY_TYPE){
		if(thisAbilityCreationInstance->mode == LEVELUP_ABILITY){

			effectAndManaMapList * tmpMap = getMapListFromEffectType();

			if(tmpMap == NULL){
				if(thisAbilityCreationInstance->selectedType != ABILITY_DAMAGE_TYPE){
					typeAndManaMapList * tmpTypeMap = getTypeMapListFromEffectType();

					if(tmpTypeMap == NULL){
						cwrite("!! selectedType NOT FOUND !!");
					}else{
						selectNextType(tmpTypeMap);
	//					trySelectNextTypeUpgrade(tmpTypeMap, getTypeMapListFromEffectTypeUpgradeClone());
					}
				}
			}else{
				tryIncreaseUpgradeEffect(tmpMap, getMapListFromEffectTypeUpgradeClone());
			}

		}else{
			effectAndManaMapList * tmpMap = getMapListFromEffectType();

			if(tmpMap == NULL){
				typeAndManaMapList * tmpTypeMap = getTypeMapListFromEffectType();

				if(tmpTypeMap == NULL){
					cwrite("!! selectedType NOT FOUND !!");
				}else{
					selectNextType(tmpTypeMap);
				}

			}else{
				increaseEffect(tmpMap);
			}
		}
	}else{
		if(thisAbilityCreationInstance->mode != LEVELUP_ABILITY){
			changeAbilityTemplate(1);
		}
	}
}

void setAbilityName(char * newName){
	strcpy(thisAbilityCreationInstance->abilityInsance->name, newName);
}

void interpretLeftAbilityCreation(int range, int mvmt, int totalHP, int totalMana){
	if(thisAbilityCreationInstance->selectedType != ABILITY_TYPE){
		if(thisAbilityCreationInstance->mode == LEVELUP_ABILITY){
			effectAndManaMapList * tmpMap = getMapListFromEffectType();

			if(tmpMap == NULL){
				if(thisAbilityCreationInstance->selectedType != ABILITY_DAMAGE_TYPE){
					typeAndManaMapList * tmpTypeMap = getTypeMapListFromEffectType();
//					trySelectPreviousTypeUpgrade(tmpTypeMap, getTypeMapListFromEffectTypeUpgradeClone());
					selectPreviousType(tmpTypeMap);
				}
			}else{
				tryDecreaseUpgradeEffect(tmpMap, getMapListFromEffectTypeUpgradeClone(), range, mvmt, totalHP, totalMana);
			}
		}else{
			effectAndManaMapList * tmpMap = getMapListFromEffectType();

			if(tmpMap == NULL){
				typeAndManaMapList * tmpTypeMap = getTypeMapListFromEffectType();
				selectPreviousType(tmpTypeMap);
			}else{
				decreaseEffect(tmpMap, range, mvmt, totalHP, totalMana);
			}
		}
	}else{
		if(thisAbilityCreationInstance->mode != LEVELUP_ABILITY){
			changeAbilityTemplate(-1);
		}
	}
}

effectAndManaMapList * getMapListFromEffectType(){
	switch(thisAbilityCreationInstance->selectedType){
	case ABILITY_RANGE:
		return thisAbilityCreationInstance->abilityInsance->range;
	case ABILITY_TARGETED:
		return thisAbilityCreationInstance->abilityInsance->targeted;
	case ABILITY_EXTRA_ATTACK:
		return thisAbilityCreationInstance->abilityInsance->extraAttack;
	case ABILITY_DICE_DAMAGE:
		return thisAbilityCreationInstance->abilityInsance->diceDamage;
	case ABILITY_DICE_DAMAGE_MULTIPLIER:
		return thisAbilityCreationInstance->abilityInsance->diceDamageMultiplier;
	case ABILITY_DAMAGE:
		return thisAbilityCreationInstance->abilityInsance->damage;
	case ABILITY_STATUS_DICE_DAMAGE:
		return thisAbilityCreationInstance->abilityInsance->statusDiceDamage;
	case ABILITY_STATUS_DAMAGE:
		return thisAbilityCreationInstance->abilityInsance->statusDamage;
	case ABILITY_STATUS_DICE_DURATION:
		return thisAbilityCreationInstance->abilityInsance->diceStatusDuration;
	case ABILITY_STATUS_DURATION:
		return thisAbilityCreationInstance->abilityInsance->statusDuration;
	case ABILITY_AOE_NOVA:
		return thisAbilityCreationInstance->abilityInsance->aoeNova;
	case ABILITY_AOE_LINE:
		return thisAbilityCreationInstance->abilityInsance->aoeLine;
	case ABILITY_DURATION:
		return thisAbilityCreationInstance->abilityInsance->duration;
	case ABILITY_DURATION_MOD:
		return thisAbilityCreationInstance->abilityInsance->durationMod;
	case ABILITY_ACTIONS:
		return thisAbilityCreationInstance->abilityInsance->actions;
	case ABILITY_AC:
		return thisAbilityCreationInstance->abilityInsance->ac;
	case ABILITY_ATTACK:
		return thisAbilityCreationInstance->abilityInsance->attack;
	case ABILITY_DAMAGE_MOD:
		return thisAbilityCreationInstance->abilityInsance->damageMod;
	case ABILITY_MVMT:
		return thisAbilityCreationInstance->abilityInsance->mvmt;
	case ABILITY_HP:
		return thisAbilityCreationInstance->abilityInsance->hp;
	case ABILITY_DICE_HP:
		return thisAbilityCreationInstance->abilityInsance->diceHP;
	case ABILITY_BASE_HP:
		return thisAbilityCreationInstance->abilityInsance->baseHP;
	case ABILITY_BASE_MANA:
		return thisAbilityCreationInstance->abilityInsance->baseMana;
	case ABILITY_FOOD:
		return thisAbilityCreationInstance->abilityInsance->food;
	case ABILITY_BASE_FOOD:
		return thisAbilityCreationInstance->abilityInsance->baseFood;
	case ABILITY_LOS:
		return thisAbilityCreationInstance->abilityInsance->LoS;
	case ABILITY_BLUNT_DR:
		return thisAbilityCreationInstance->abilityInsance->bluntDR;
	case ABILITY_CHOP_DR:
		return thisAbilityCreationInstance->abilityInsance->chopDR;
	case ABILITY_PIERCE_DR:
		return thisAbilityCreationInstance->abilityInsance->pierceDR;
	case ABILITY_SLASH_DR:
		return thisAbilityCreationInstance->abilityInsance->slashDR;
	case ABILITY_EARTH_DR:
		return thisAbilityCreationInstance->abilityInsance->earthDR;
	case ABILITY_FIRE_DR:
		return thisAbilityCreationInstance->abilityInsance->fireDR;
	case ABILITY_WATER_DR:
		return thisAbilityCreationInstance->abilityInsance->waterDR;
	case ABILITY_LIGHTNING_DR:
		return thisAbilityCreationInstance->abilityInsance->lightningDR;
	}

	return NULL;
}

effectAndManaMapList * getMapListFromEffectTypeUpgradeClone(){
	switch(thisAbilityCreationInstance->selectedType){
	case ABILITY_RANGE:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->range;
	case ABILITY_TARGETED:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->targeted;
	case ABILITY_EXTRA_ATTACK:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->extraAttack;
	case ABILITY_DICE_DAMAGE:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->diceDamage;
	case ABILITY_DICE_DAMAGE_MULTIPLIER:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->diceDamageMultiplier;
	case ABILITY_DAMAGE:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->damage;
	case ABILITY_STATUS_DICE_DAMAGE:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->statusDiceDamage;
	case ABILITY_STATUS_DAMAGE:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->statusDamage;
	case ABILITY_STATUS_DICE_DURATION:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->diceStatusDuration;
	case ABILITY_STATUS_DURATION:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->statusDuration;
	case ABILITY_AOE_NOVA:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->aoeNova;
	case ABILITY_AOE_LINE:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->aoeLine;
	case ABILITY_DURATION:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->duration;
	case ABILITY_DURATION_MOD:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->durationMod;
	case ABILITY_ACTIONS:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->actions;
	case ABILITY_AC:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->ac;
	case ABILITY_ATTACK:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->attack;
	case ABILITY_DAMAGE_MOD:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->damageMod;
	case ABILITY_MVMT:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->mvmt;
	case ABILITY_HP:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->hp;
	case ABILITY_DICE_HP:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->diceHP;
	case ABILITY_BASE_HP:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->baseHP;
	case ABILITY_BASE_MANA:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->baseMana;
	case ABILITY_FOOD:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->food;
	case ABILITY_BASE_FOOD:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->baseFood;
	case ABILITY_LOS:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->LoS;
	case ABILITY_BLUNT_DR:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->bluntDR;
	case ABILITY_CHOP_DR:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->chopDR;
	case ABILITY_PIERCE_DR:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->pierceDR;
	case ABILITY_SLASH_DR:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->slashDR;
	case ABILITY_EARTH_DR:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->earthDR;
	case ABILITY_FIRE_DR:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->fireDR;
	case ABILITY_WATER_DR:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->waterDR;
	case ABILITY_LIGHTNING_DR:
		return thisAbilityCreationInstance->abilityLevelUpOriginal->lightningDR;
	}

	return NULL;
}

int canDecreaseEffect(effectAndManaMapList * selectedMap, int range, int mvmt, int totalHP, int totalMana){
	switch(thisAbilityCreationInstance->selectedType){
		case ABILITY_RANGE:
			if(range + selectedMap->effectAndManaArray[selectedMap->selectedIndex-1]->effectMagnitude > 0){
				return 1;
			}else{
				return 0;
			}
		break;
		case ABILITY_MVMT:
			if(mvmt + selectedMap->effectAndManaArray[selectedMap->selectedIndex-1]->effectMagnitude > 0){
				return 1;
			}else{
				return 0;
			}
		break;
		case ABILITY_BASE_HP:{
			int abilityBaseHP = 0, abilityCON = 0;

			if(thisAbilityCreationInstance->abilityInsance->baseHPEnabled){
				abilityBaseHP = thisAbilityCreationInstance->abilityInsance->baseHP->effectAndManaArray[thisAbilityCreationInstance->abilityInsance->baseHP->selectedIndex]->effectMagnitude;
			}

			if(thisAbilityCreationInstance->selectedType == ABILITY_BASE_HP){
				abilityBaseHP -= 1;
			}

			if(totalHP + abilityBaseHP + abilityCON * 2 > 0){
				return 1;
			}else{
				return 0;
			}
		}
		break;
		case ABILITY_BASE_MANA:{
			int abilityBaseMana = 0, abilityWill = 0;

			if(thisAbilityCreationInstance->abilityInsance->baseManaEnabled){
				abilityBaseMana = thisAbilityCreationInstance->abilityInsance->baseMana->effectAndManaArray[thisAbilityCreationInstance->abilityInsance->baseMana->selectedIndex]->effectMagnitude;
			}

			if(thisAbilityCreationInstance->selectedType == ABILITY_BASE_MANA){
				abilityBaseMana -= 1;
			}

			if(totalMana + abilityBaseMana + abilityWill * 2 > 0){
				return 1;
			}else{
				return 0;
			}
		}
		break;
	}

	return 1;
}

void addBonusMana(){
	if(thisAbilityCreationInstance->bonusMana == 0){
		return;
	}

	thisAbilityCreationInstance->bonusMana--;

	thisAbilityCreationInstance->abilityInsance->totalManaCost = calculateManaCost(thisAbilityCreationInstance->abilityInsance, thisAbilityCreationInstance->totalBonusMana - thisAbilityCreationInstance->bonusMana);
}

void removeBonusMana(){
	if(thisAbilityCreationInstance->bonusMana == thisAbilityCreationInstance->totalBonusMana){
		return;
	}

	thisAbilityCreationInstance->bonusMana++;

	thisAbilityCreationInstance->abilityInsance->totalManaCost = calculateManaCost(thisAbilityCreationInstance->abilityInsance, thisAbilityCreationInstance->totalBonusMana - thisAbilityCreationInstance->bonusMana);
}

int getSpentBonusMana(){
	return thisAbilityCreationInstance->totalBonusMana - thisAbilityCreationInstance->bonusMana;
}

void increaseEffect(effectAndManaMapList * selectedMap){
	if(selectedMap->selectedIndex + 1 < selectedMap->size){
		selectedMap->selectedIndex++;
	}

	thisAbilityCreationInstance->abilityInsance->totalManaCost = calculateManaCost(thisAbilityCreationInstance->abilityInsance, thisAbilityCreationInstance->totalBonusMana - thisAbilityCreationInstance->bonusMana);
}

void tryIncreaseUpgradeEffect(effectAndManaMapList * selectedMap, effectAndManaMapList * baseMap){
	if(selectedMap->selectedIndex + 1 < selectedMap->size
	 && selectedMap->effectAndManaArray[selectedMap->selectedIndex + 1]->manaCost >= baseMap->effectAndManaArray[baseMap->selectedIndex]->manaCost){
		selectedMap->selectedIndex++;
	}

	thisAbilityCreationInstance->abilityInsance->totalManaCost = calculateManaCost(thisAbilityCreationInstance->abilityInsance, thisAbilityCreationInstance->totalBonusMana - thisAbilityCreationInstance->bonusMana);
}

void decreaseEffect(effectAndManaMapList * selectedMap, int range, int mvmt, int totalHP, int totalMana){
	if(selectedMap->selectedIndex > 0 && canDecreaseEffect(selectedMap, range, mvmt, totalHP, totalMana)){
		selectedMap->selectedIndex--;
	}

	thisAbilityCreationInstance->abilityInsance->totalManaCost = calculateManaCost(thisAbilityCreationInstance->abilityInsance, thisAbilityCreationInstance->totalBonusMana - thisAbilityCreationInstance->bonusMana);
}

void tryDecreaseUpgradeEffect(effectAndManaMapList * selectedMap, effectAndManaMapList * baseMap, int range, int mvmt, int totalHP, int totalMana){
	if(selectedMap->selectedIndex > 0 && canDecreaseEffect(selectedMap, range, mvmt, totalHP, totalMana)
	 && selectedMap->effectAndManaArray[selectedMap->selectedIndex - 1]->manaCost >= baseMap->effectAndManaArray[baseMap->selectedIndex]->manaCost){
		selectedMap->selectedIndex--;
	}

	thisAbilityCreationInstance->abilityInsance->totalManaCost = calculateManaCost(thisAbilityCreationInstance->abilityInsance, thisAbilityCreationInstance->totalBonusMana - thisAbilityCreationInstance->bonusMana);
}

void selectNextType(typeAndManaMapList * thisMapList){
	if(thisMapList->selectedIndex +1 < thisMapList->size){
		thisMapList->selectedIndex++;
	}else{
		thisMapList->selectedIndex = 0;;
	}

	thisAbilityCreationInstance->abilityInsance->totalManaCost = calculateManaCost(thisAbilityCreationInstance->abilityInsance, thisAbilityCreationInstance->totalBonusMana - thisAbilityCreationInstance->bonusMana);
}

void selectPreviousType(typeAndManaMapList * thisMapList){
	if(thisMapList->selectedIndex > 0){
		thisMapList->selectedIndex--;
	}else{
		thisMapList->selectedIndex = thisMapList->size - 1;
	}

	thisAbilityCreationInstance->abilityInsance->totalManaCost = calculateManaCost(thisAbilityCreationInstance->abilityInsance, thisAbilityCreationInstance->totalBonusMana - thisAbilityCreationInstance->bonusMana);
}

int canCreateAbility(){
	if(thisAbilityCreationInstance->abilityInsance->type == 'p' && thisAbilityCreationInstance->abilityInsance->totalManaCost == 0){
		return 1;
	}else if((thisAbilityCreationInstance->abilityInsance->type == 'd' || thisAbilityCreationInstance->abilityInsance->type == 't') && thisAbilityCreationInstance->abilityInsance->totalManaCost > 0){
		return 1;
	}else if(thisAbilityCreationInstance->abilityInsance->type == 'i' && thisAbilityCreationInstance->abilityInsance->totalManaCost >= 0){
		return 1;
	}

	return 0;
}

void interpretCreateAbilityEnter(){
	if(canCreateAbility()){
		if(thisAbilityCreationInstance->mode == LEVELUP_ABILITY){
			disableAbilityCreateMode();
		}else{
			toggleNameMode();
			toggleAbilityWaitForNameMode();
		}
	}
}

void interpretCreateAbilityEscape(){
	if(thisAbilityCreationInstance->mode == LEVELUP_ABILITY){
//		free(thisAbilityCreationInstance->abilityInsance);
//		thisAbilityCreationInstance->abilityInsance = NULL;
		//dont want to save it
		disableLevelUpCreateAbilityMode();
	}

	disableAbilityCreateMode();
	free(thisAbilityCreationInstance->abilityInsance);
	thisAbilityCreationInstance->abilityInsance = NULL;
}

void toggleAbilityWaitForNameMode(){
	thisAbilityCreationInstance->waitingForName = (thisAbilityCreationInstance->waitingForName + 1) % 2;
}

int inAbilityWaitForNameMode(){
	return thisAbilityCreationInstance->waitingForName;
}

ability * getNewAbility(){
	ability * newAbility;

	if(thisAbilityCreationInstance->mode == LEVELUP_ABILITY){
		return thisAbilityCreationInstance->abilityInsance;
	}

	newAbility = cloneAbility(thisAbilityCreationInstance->abilityInsance);
	newAbility->ID = thisAbilityCreationInstance->idCounter;
	thisAbilityCreationInstance->idCounter++;

	return newAbility;
}
