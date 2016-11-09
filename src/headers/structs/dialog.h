/*
 * dialog.h
 *
 *  Created on: Dec 7, 2015
 *      Author: Adrian
 */

#ifndef SRC_DIALOG_H_
#define SRC_DIALOG_H_
#include "../console_pub_methods.h"

typedef struct{
	int individualID;
	int dialogID;
} individualDialog;

typedef struct{
	int messageID;
	char message[256];
	int numDialogDecision;

	int dialogCheckpoint; //if the user should start at this message
	int eventID; //the event this flag triggers

	int nextMessageID;
	struct dialogMessage * nextMessage;
	struct dialogDecision * decisions[10];
}dialogMessage;

typedef struct{
	int rootMessageID;
	int targetMessageID;
	char message[64];
	dialogMessage * targetMessage;
} dialogDecision;

typedef struct{
	int drawBox;
	int numRows;
	int numDialogMessages;
	int decisionIndex;
	int decisionIndexRow[10];
	int speakingIndividualID;

	//For making the character speak
	int speakMode;
	int speakDrawLength;
	int speakDrawSpeedInTicks;
	int numTicks;
	int speakSoundID;

	dialogMessage * currentMessage;
	fixedCharacter * dialogWindow;
	fixedCharacter * selectArrow;
	individualDialog * individualDialogRegistry[1000];
	dialogMessage ** dialogMessages;
} dialogInstance;

#endif /* SRC_DIALOG_H_ */
