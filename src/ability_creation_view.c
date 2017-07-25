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
	strcpy(thisAbilityCreationInstance->descriptionPermanentSTR,"The Strength of the player; added to the player's base STR.");
	strcpy(thisAbilityCreationInstance->descriptionPermanentDEX,"The Dexterity of the player; added to the player's base DEX.");
	strcpy(thisAbilityCreationInstance->descriptionPermanentCON,"The Constitution of the player; added to the player's base CON.");
	strcpy(thisAbilityCreationInstance->descriptionPermanentWIS,"The Wisdom of the player; added to the player's base WIS.");
	strcpy(thisAbilityCreationInstance->descriptionPermanentINT,"The Intelligence of the player; added to the player's base INT.");
	strcpy(thisAbilityCreationInstance->descriptionPermanentWILL,"The Willpower of the player; added to the player's base WILL.");
	strcpy(thisAbilityCreationInstance->descriptionPermanentCHR,"The Charisma of the player; added to the player's base CHR.");
	strcpy(thisAbilityCreationInstance->descriptionPermanentLUCK,"The Luck of the player; added to the player's base LUCK.");
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
	strcpy(thisAbilityCreationInstance->descriptionDurationAOE,"The area of affect of the spell.");
	strcpy(thisAbilityCreationInstance->descriptionDurationDiceDuration,"How long the ability will last by random dice roll. Cumulative with Duration.");
	strcpy(thisAbilityCreationInstance->descriptionDurationDuration,"How long the ability will last. Cumulative with DiceDuration. ");
	strcpy(thisAbilityCreationInstance->descriptionDurationActions,"The number of actions the player forfeits by using the ability. Useful to reduce the cost of the ability at the risk of not being able to move afterwords.");
	strcpy(thisAbilityCreationInstance->descriptionDurationSTR,"Additional points of Strength; temporarily added to the individual's base STR.");
	strcpy(thisAbilityCreationInstance->descriptionDurationDEX,"Additional points of Dexterity; temporarily added to the individual's base DEX.");
	strcpy(thisAbilityCreationInstance->descriptionDurationCON,"Additional points of Constitution; temporarily added to the individual's base CON.");
	strcpy(thisAbilityCreationInstance->descriptionDurationWIS,"Additional points of Wisdom; temporarily added to the individual's base WIS.");
	strcpy(thisAbilityCreationInstance->descriptionDurationINT,"Additional points of Intelligence; temporarily added to the individual's base INT.");
	strcpy(thisAbilityCreationInstance->descriptionDurationWILL,"Additional points of WillPower; temporarily added to the individual's base WILL.");
	strcpy(thisAbilityCreationInstance->descriptionDurationCHR,"Additional points of Charisma; temporarily added to the individual's base CHR.");
	strcpy(thisAbilityCreationInstance->descriptionDurationLUCK,"Additional points of luck; temporarily added to the individual's base LUCK.");
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
	strcpy(thisAbilityCreationInstance->descriptionTargetedAOE,"The area of affect of the ability.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedDiceDuration,"How long the ability will last by random dice roll. Cumulative with Duration.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedDuration,"How long the ability will last. Cumulative with DiceDuration. ");
	strcpy(thisAbilityCreationInstance->descriptionTargetedActions,"The number of actions the player forfeits by using the ability. Useful to reduce the cost of the ability at the risk of not being able to move afterwords.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedSTR,"Additional points of Strength; temporarily added to the individual's base STR.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedDEX,"Additional points of Dexterity; temporarily added to the individual's base DEX.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedCON,"Additional points of Constitution; temporarily added to the individual's base CON.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedWIS,"Additional points of Wisdom; temporarily added to the individual's base WIS.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedINT,"Additional points of Intelligence; temporarily added to the individual's base INT.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedWILL,"Additional points of WillPower; temporarily added to the individual's base WILL.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedCHR,"Additional points of Charisma; temporarily added to the individual's base CHR.");
	strcpy(thisAbilityCreationInstance->descriptionTargetedLUCK,"Additional points of luck; temporarily added to the individual's base LUCK.");
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

	thisAbilityCreationInstance->currentTemplateIndex = 0;
	thisAbilityCreationInstance->MAX_ABILITY_TEMPLATES = 4;
	thisAbilityCreationInstance->numAbilityTemplates = 0;

	thisAbilityCreationInstance->effectCurrentIndex = 0;
	thisAbilityCreationInstance->effectStartingIndex = 0;
	thisAbilityCreationInstance->MAX_FIELDS_ON_WINDOW = 15;
	thisAbilityCreationInstance->effectEndingIndex = thisAbilityCreationInstance->MAX_FIELDS_ON_WINDOW;
	thisAbilityCreationInstance->mode = DEFAULT_ABILITY;
	thisAbilityCreationInstance->idCounter = 0;
	thisAbilityCreationInstance->bonusMana = 0;
	thisAbilityCreationInstance->totalBonusMana = 0;

	thisAbilityCreationInstance->creationWindow = createCharacter(imageID, rgb, x, y);
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

void enableAbilityCreateMode(int bonusMana, abilityCreationMode createMode, creation_modes createModeType){
	thisAbilityCreationInstance->createMode = createMode;
	thisAbilityCreationInstance->mode = createModeType;
	thisAbilityCreationInstance->bonusMana = bonusMana;
	thisAbilityCreationInstance->totalBonusMana = bonusMana;

	createMode == ABILITY_CREATE_EXCEPT_PERMENANT ? thisAbilityCreationInstance->templateIndex = 1 : 0;

	if(thisAbilityCreationInstance->mode == DEFAULT_ABILITY){
		free(thisAbilityCreationInstance->abilityInsance);
	}

	thisAbilityCreationInstance->abilityInsance = cloneAbility(thisAbilityCreationInstance->abilityTemplates[thisAbilityCreationInstance->templateIndex]);
	thisAbilityCreationInstance->inCreateMode = 1;
}

void disableAbilityCreateMode(){
	thisAbilityCreationInstance->inCreateMode = 0;
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
	RECT textRect, descriptionRect;

	textRect.top = thisAbilityCreationInstance->creationWindow->y + 40;
	textRect.left = thisAbilityCreationInstance->creationWindow->x + 30;
	textRect.bottom = textRect.top + 40;
	textRect.right = textRect.left + 240;

	descriptionRect.top = thisAbilityCreationInstance->creationWindow->y + 355;
	descriptionRect.left = thisAbilityCreationInstance->creationWindow->x + 10;
	descriptionRect.bottom = descriptionRect.top + 100;
	descriptionRect.right = descriptionRect.left + 310;

	SetTextColor(hdcBuffer, RGB(255, 200, 0));

	//draw create window
	SelectObject(hdcMem, thisAbilityCreationInstance->creationWindow->fixedImage);
	BitBlt(hdcBuffer, thisAbilityCreationInstance->creationWindow->x, thisAbilityCreationInstance->creationWindow->y, thisAbilityCreationInstance->creationWindow->fixedWidth, thisAbilityCreationInstance->creationWindow->fixedHeight, hdcMem, 0, 0, SRCCOPY);

	char tmpLine[128];

	sprintf(tmpLine,"Type: %s", thisAbilityCreationInstance->abilityInsance->typeName);
	DrawText(hdcBuffer, tmpLine, strlen(tmpLine), &textRect, DT_SINGLELINE);
	tmpLine[0] = '\0';

	if(thisAbilityCreationInstance->effectCurrentIndex == -1){
		drawUnboundCharacterByPixels(hdc,hdcBuffer,textRect.left - 20,textRect.top,thisAbilityCreationInstance->selector);
	}

	moveRECTRight(&textRect, 150);
	sprintf(tmpLine,"Mana Cost: %i", thisAbilityCreationInstance->abilityInsance->totalManaCost);
	DrawText(hdcBuffer, tmpLine, strlen(tmpLine), &textRect, DT_SINGLELINE);
	tmpLine[0] = '\0';
	moveRECTRight(&textRect, -150);

	//scroll up arrow
	if(thisAbilityCreationInstance->effectStartingIndex > 0){
		drawUnboundCharacterByPixels(hdc, hdcBuffer,thisAbilityCreationInstance->creationWindow->x + 40,
				thisAbilityCreationInstance->creationWindow->y + 57, thisAbilityCreationInstance->scrollUpArrow);
	}

	//scroll down arrow
	if(thisAbilityCreationInstance->effectEndingIndex < thisAbilityCreationInstance->abilityInsance->numEnabledEffects){
		drawUnboundCharacterByPixels(hdc, hdcBuffer,thisAbilityCreationInstance->creationWindow->x + 40,
				thisAbilityCreationInstance->creationWindow->y + 77 + 17 * thisAbilityCreationInstance->MAX_FIELDS_ON_WINDOW,
				thisAbilityCreationInstance->scrollDownArrow);
	}

	moveRECTDown(&textRect, 20);

	effectIndex = 0;

	processTypeMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->damageTypeEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_DAMAGE_TYPE, "Damage Type", 0, thisAbilityCreationInstance->abilityInsance->damageType);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->rangeEnabled,
			 hdc, hdcBuffer, &textRect, ABILITY_RANGE, "Range", 0, thisAbilityCreationInstance->abilityInsance->range);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->targetedEnabled,
			 hdc, hdcBuffer, &textRect, ABILITY_TARGETED, "Target", 0, thisAbilityCreationInstance->abilityInsance->targeted);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->extraAttackEnabled,
			 hdc, hdcBuffer, &textRect, ABILITY_EXTRA_ATTACK, "Extra Attack", 0, thisAbilityCreationInstance->abilityInsance->extraAttack);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->diceDamageEnabled,
			 hdc, hdcBuffer, &textRect, ABILITY_DICE_DAMAGE, "Dice Damage", 0, thisAbilityCreationInstance->abilityInsance->diceDamage);

	if(thisAbilityCreationInstance->abilityInsance->diceDamageEnabled
			&& thisAbilityCreationInstance->abilityInsance->diceDamage->effectAndManaArray[thisAbilityCreationInstance->abilityInsance->diceDamage->selectedIndex]->effectMagnitude > 0){
		textRect.left += 20;
		processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->diceDamageMultiplierEnabled,
					 hdc, hdcBuffer, &textRect, ABILITY_DICE_DAMAGE_MULTIPLIER, "Dice Damage Multiplier", 0, thisAbilityCreationInstance->abilityInsance->diceDamageMultiplier);
		textRect.left -= 20;
	}

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->damageEnabled,
			 hdc, hdcBuffer, &textRect, ABILITY_DAMAGE, "damage", 0, thisAbilityCreationInstance->abilityInsance->damage);

	if(thisAbilityCreationInstance->abilityInsance->statusEnabled){
		processTypeMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->statusEnabled,
				hdc, hdcBuffer, &textRect, ABILITY_STATUS, "Status", 0, thisAbilityCreationInstance->abilityInsance->status);

		char type[16];
		strcpy(type, thisAbilityCreationInstance->abilityInsance->status->typeAndManaArray[thisAbilityCreationInstance->abilityInsance->status->selectedIndex]->type);

		//Dont show status options when status is type 'None'
		if(strcmp(type, "None") != 0){
			textRect.left += 20;
			processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->diceStatusDurationEnabled,
					 hdc, hdcBuffer, &textRect, ABILITY_STATUS_DICE_DURATION, "Status Dice Duration", 0, thisAbilityCreationInstance->abilityInsance->diceStatusDuration);

			processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->statusDurationEnabled,
					 hdc, hdcBuffer, &textRect, ABILITY_STATUS_DURATION, "Status Duration", 0, thisAbilityCreationInstance->abilityInsance->statusDuration);

			//only show damage options for damaging statuses.
			if(strcmp(type, "Poison") == 0 || strcmp(type, "Burn") == 0 || strcmp(type, "Bleed") == 0){
				processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->statusDiceDamageEnabled,
						 hdc, hdcBuffer, &textRect, ABILITY_STATUS_DICE_DAMAGE, "Status Dice Damage", 0, thisAbilityCreationInstance->abilityInsance->statusDiceDamage);

				processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->statusDamageEnabled,
						 hdc, hdcBuffer, &textRect, ABILITY_STATUS_DAMAGE, "Status Damage", 0, thisAbilityCreationInstance->abilityInsance->statusDamage);
			}

			textRect.left -= 20;
		}
	}
	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->aoeEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_AOE, "aoe", 0, thisAbilityCreationInstance->abilityInsance->aoe);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->durationEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_DURATION, "duration", 0, thisAbilityCreationInstance->abilityInsance->duration);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->durationModEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_DURATION_MOD, "durationMod", 0, thisAbilityCreationInstance->abilityInsance->durationMod);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->actionsEnabled,
					 hdc, hdcBuffer, &textRect, ABILITY_ACTIONS, "Actions", 0, thisAbilityCreationInstance->abilityInsance->actions);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->STREnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_STR, "STR", 0, thisAbilityCreationInstance->abilityInsance->STR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->DEXEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_DEX, "DEX", 0, thisAbilityCreationInstance->abilityInsance->DEX);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->CONEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_CON, "CON", 0, thisAbilityCreationInstance->abilityInsance->CON);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->WILLEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_WILL, "WILL", 0, thisAbilityCreationInstance->abilityInsance->WILL);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->INTEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_INT, "INT", 0, thisAbilityCreationInstance->abilityInsance->INT);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->WISEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_WIS, "WIS", 0, thisAbilityCreationInstance->abilityInsance->WIS);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->CHREnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_CHR, "CHR", 0, thisAbilityCreationInstance->abilityInsance->CHR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->LUCKEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_LUCK, "LUCK", 0, thisAbilityCreationInstance->abilityInsance->LUCK);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->acEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_AC, "ac", 0, thisAbilityCreationInstance->abilityInsance->ac);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->attackEnabled,
					 hdc, hdcBuffer, &textRect, ABILITY_ATTACK, "attack", 0, thisAbilityCreationInstance->abilityInsance->attack);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->damageModEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_DAMAGE_MOD, "damageMod", 0, thisAbilityCreationInstance->abilityInsance->damageMod);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->mvmtEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_MVMT, "movement", 0, thisAbilityCreationInstance->abilityInsance->mvmt);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->diceHPEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_DICE_HP, "diceHP", 0, thisAbilityCreationInstance->abilityInsance->diceHP);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->hpEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_HP, "hp", 0, thisAbilityCreationInstance->abilityInsance->hp);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->baseHPEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_BASE_HP, "baseHP", 0, thisAbilityCreationInstance->abilityInsance->baseHP);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->baseManaEnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_BASE_MANA, "baseMana", 0, thisAbilityCreationInstance->abilityInsance->baseMana);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->bluntDREnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_BLUNT_DR, "bluntDR", thisAbilityCreationInstance->abilityInsance->type == 't'? 2:1, thisAbilityCreationInstance->abilityInsance->bluntDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->chopDREnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_CHOP_DR, "chopDR", thisAbilityCreationInstance->abilityInsance->type == 't'? 2:1, thisAbilityCreationInstance->abilityInsance->chopDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->pierceDREnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_PIERCE_DR, "pierceDR", thisAbilityCreationInstance->abilityInsance->type == 't'? 2:1, thisAbilityCreationInstance->abilityInsance->pierceDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->slashDREnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_SLASH_DR, "slashDR", thisAbilityCreationInstance->abilityInsance->type == 't'? 2:1, thisAbilityCreationInstance->abilityInsance->slashDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->earthDREnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_EARTH_DR, "earthDR", thisAbilityCreationInstance->abilityInsance->type == 't'? 2:1, thisAbilityCreationInstance->abilityInsance->earthDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->fireDREnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_FIRE_DR, "fireDR", thisAbilityCreationInstance->abilityInsance->type == 't'? 2:1, thisAbilityCreationInstance->abilityInsance->fireDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->waterDREnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_WATER_DR, "waterDR", thisAbilityCreationInstance->abilityInsance->type == 't'? 2:1, thisAbilityCreationInstance->abilityInsance->waterDR);

	processEffectMapListRendering(&effectIndex, thisAbilityCreationInstance->abilityInsance->lightningDREnabled,
				 hdc, hdcBuffer, &textRect, ABILITY_LIGHTNING_DR, "lightningDR", thisAbilityCreationInstance->abilityInsance->type == 't'? 2:1, thisAbilityCreationInstance->abilityInsance->lightningDR);

	drawAbilityCreateDesciption(hdcBuffer, &descriptionRect);

	SetTextColor(hdcBuffer, RGB(0, 0, 0));

	DeleteDC(hdcMem);
}

void drawAbilityCreateDesciption(HDC hdcBuffer, RECT * descriptionRect){
	if(thisAbilityCreationInstance->effectCurrentIndex == -1){
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

		return;
	}


	switch(thisAbilityCreationInstance->selectedType){
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
	case ABILITY_AOE:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationAOE, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedAOE, -1, descriptionRect, DT_WORDBREAK);
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
	case ABILITY_STR:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'p':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionPermanentSTR, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationSTR, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedSTR, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_DEX:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'p':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionPermanentDEX, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationDEX, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedDEX, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_CON:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'p':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionPermanentCON, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationCON, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedCON, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_WILL:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'p':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionPermanentWILL, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationWILL, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedWILL, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_INT:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'p':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionPermanentINT, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationINT, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedINT, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_WIS:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'p':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionPermanentWIS, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationWIS, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedWIS, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_CHR:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'p':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionPermanentCHR, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationCHR, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedCHR, -1, descriptionRect, DT_WORDBREAK);
				break;
		}
		break;
	case ABILITY_LUCK:
		switch(thisAbilityCreationInstance->abilityInsance->type){
			case 'p':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionPermanentLUCK, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 'd':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionDurationLUCK, -1, descriptionRect, DT_WORDBREAK);
				break;
			case 't':
				DrawText(hdcBuffer, thisAbilityCreationInstance->descriptionTargetedLUCK, -1, descriptionRect, DT_WORDBREAK);
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

void processEffectMapListRendering(int * effectIndex, int isEnabled, HDC hdc, HDC hdcBuffer, RECT * textRect, effect_types type, char * fieldName, int isDR, effectAndManaMapList * mapList){
	if(isEnabled){
		if(*effectIndex >= thisAbilityCreationInstance->effectStartingIndex && *effectIndex < thisAbilityCreationInstance->effectEndingIndex){
			drawEffectMapList(hdcBuffer, textRect, fieldName, isDR, mapList);
			if(thisAbilityCreationInstance->effectCurrentIndex == *effectIndex){
				setAbilityCreationSelectedType(type);
				drawUnboundCharacterByPixels(hdc,hdcBuffer,textRect->left - 20,textRect->top-2,thisAbilityCreationInstance->selector);
				drawUnboundCharacterByPixels(hdc,hdcBuffer,textRect->right,textRect->top-2,thisAbilityCreationInstance->leftRightArrow);
			}

		}
		(*effectIndex)++;
	}
}

void processTypeMapListRendering(int * effectIndex, int isEnabled, HDC hdc, HDC hdcBuffer, RECT * textRect, effect_types type, char * fieldName, int isDR, typeAndManaMapList * mapList){
	if(isEnabled){
		if(*effectIndex >= thisAbilityCreationInstance->effectStartingIndex && *effectIndex < thisAbilityCreationInstance->effectEndingIndex){
			drawTypeMapList(hdcBuffer, textRect, fieldName, isDR, mapList);
			if(thisAbilityCreationInstance->effectCurrentIndex == *effectIndex){
				setAbilityCreationSelectedType(type);
				drawUnboundCharacterByPixels(hdc,hdcBuffer,textRect->left - 20,textRect->top-2,thisAbilityCreationInstance->selector);
				drawUnboundCharacterByPixels(hdc,hdcBuffer,textRect->right,textRect->top-2,thisAbilityCreationInstance->leftRightArrow);
			}

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

void selectNextEffect(){
	if(thisAbilityCreationInstance->effectCurrentIndex + 1 == thisAbilityCreationInstance->effectEndingIndex &&
			thisAbilityCreationInstance->effectEndingIndex  < thisAbilityCreationInstance->abilityInsance->numEnabledEffects){
		shiftEffectListUp();
	} else if(thisAbilityCreationInstance->effectCurrentIndex + 1 < thisAbilityCreationInstance->effectEndingIndex){
		thisAbilityCreationInstance->effectCurrentIndex++;
	}
}

void selectPreviousEffect(){
	if(thisAbilityCreationInstance->effectCurrentIndex == thisAbilityCreationInstance->effectStartingIndex &&
			thisAbilityCreationInstance->effectStartingIndex  > 0){
		shiftEffectListDown();
	} else if(thisAbilityCreationInstance->effectCurrentIndex > -1){
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

void interpretRightAbilityCreation(){
	if(thisAbilityCreationInstance->effectCurrentIndex >= 0){
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

	}else if(thisAbilityCreationInstance->effectCurrentIndex == -1){
		changeAbilityTemplate(1);
	}
}

void setAbilityName(char * newName){
	strcpy(thisAbilityCreationInstance->abilityInsance->name, newName);
}

void interpretLeftAbilityCreation(int range, int mvmt, int totalHP, int totalMana){
	if(thisAbilityCreationInstance->effectCurrentIndex >= 0){
		effectAndManaMapList * tmpMap = getMapListFromEffectType();
		if(tmpMap == NULL){
			typeAndManaMapList * tmpTypeMap = getTypeMapListFromEffectType();
			selectPreviousType(tmpTypeMap);
		}else{
			decreaseEffect(tmpMap, range, mvmt, totalHP, totalMana);
		}
	}else if(thisAbilityCreationInstance->effectCurrentIndex == -1){
		changeAbilityTemplate(-1);
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
	case ABILITY_AOE:
		return thisAbilityCreationInstance->abilityInsance->aoe;
	case ABILITY_DURATION:
		return thisAbilityCreationInstance->abilityInsance->duration;
	case ABILITY_DURATION_MOD:
		return thisAbilityCreationInstance->abilityInsance->durationMod;
	case ABILITY_ACTIONS:
		return thisAbilityCreationInstance->abilityInsance->actions;
	case ABILITY_STR:
		return thisAbilityCreationInstance->abilityInsance->STR;
	case ABILITY_DEX:
		return thisAbilityCreationInstance->abilityInsance->DEX;
	case ABILITY_CON:
		return thisAbilityCreationInstance->abilityInsance->CON;
	case ABILITY_WILL:
		return thisAbilityCreationInstance->abilityInsance->WILL;
	case ABILITY_INT:
		return thisAbilityCreationInstance->abilityInsance->INT;
	case ABILITY_WIS:
		return thisAbilityCreationInstance->abilityInsance->WIS;
	case ABILITY_CHR:
		return thisAbilityCreationInstance->abilityInsance->CHR;
	case ABILITY_LUCK:
		return thisAbilityCreationInstance->abilityInsance->LUCK;
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
		case ABILITY_BASE_HP:
		case ABILITY_CON:{
			int abilityBaseHP = 0, abilityCON = 0;

			if(thisAbilityCreationInstance->abilityInsance->CONEnabled){
				abilityCON = thisAbilityCreationInstance->abilityInsance->CON->effectAndManaArray[thisAbilityCreationInstance->abilityInsance->CON->selectedIndex]->effectMagnitude;
			}

			if(thisAbilityCreationInstance->abilityInsance->baseHPEnabled){
				abilityBaseHP = thisAbilityCreationInstance->abilityInsance->baseHP->effectAndManaArray[thisAbilityCreationInstance->abilityInsance->baseHP->selectedIndex]->effectMagnitude;
			}

			if(thisAbilityCreationInstance->selectedType == ABILITY_CON){
				abilityCON -= 1;
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
		case ABILITY_BASE_MANA:
		case ABILITY_WILL:{
			int abilityBaseMana = 0, abilityWill = 0;

			if(thisAbilityCreationInstance->abilityInsance->WILLEnabled){
				abilityWill = 2 * thisAbilityCreationInstance->abilityInsance->WILL->effectAndManaArray[thisAbilityCreationInstance->abilityInsance->WILL->selectedIndex]->effectMagnitude;
			}

			if(thisAbilityCreationInstance->abilityInsance->baseManaEnabled){
				abilityBaseMana = thisAbilityCreationInstance->abilityInsance->baseMana->effectAndManaArray[thisAbilityCreationInstance->abilityInsance->baseMana->selectedIndex]->effectMagnitude;
			}

			if(thisAbilityCreationInstance->selectedType == ABILITY_WILL){
				abilityWill -= 1;
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



void decreaseEffect(effectAndManaMapList * selectedMap, int range, int mvmt, int totalHP, int totalMana){
	if(selectedMap->selectedIndex > 0 && canDecreaseEffect(selectedMap, range, mvmt, totalHP, totalMana)){
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

void toggleAbilityWaitForNameMode(){
	thisAbilityCreationInstance->waitingForName = (thisAbilityCreationInstance->waitingForName + 1) % 2;
}

int inAbilityWaitForNameMode(){
	return thisAbilityCreationInstance->waitingForName;
}

ability * getNewAbility(){
	ability * newAbility = cloneAbility(thisAbilityCreationInstance->abilityInsance);
	newAbility->ID = thisAbilityCreationInstance->idCounter;
	thisAbilityCreationInstance->idCounter++;

	return newAbility;
}
