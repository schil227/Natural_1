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
	int specialID;
	int dialogID;
} individualDialog;


typedef struct{
	int rootMessageID;
	int defaultDialogID;
	int displayEventID;
	int eventID;
	int MESSAGE_SIZE;
	char message[256];
	char processedMessage[256];

	char * messageInserts[5];
	int MAX_MESSAGE_INSERTS;
	int numMessageInserts;

	struct dialogMessage * targetMessage;
} dialogDecision;

typedef struct{
	int messageID;
	char message[256];
	char parsedMessage[256];
	int MESSAGE_SIZE;
	int numDialogDecision;
	int numDialogDecisionsParsed;

	int dialogCheckpoint; //if the user should start at this message
	int eventID; //the event this flag triggers

	char * messageInserts[5];
	int MAX_MESSAGE_INSERTS;
	int numMessageInserts;

	int nextMessageID;

	struct dialogMessage * nextMessage;
	dialogDecision * decisions[10];
	dialogDecision * parsedDecisions[10];
}dialogMessage;

typedef struct{
	int drawBox;
	int numRows;
	int numDialogMessages;
	int MAX_INDIVIDUAL_DIALOG_REGISTRY;
	int MAX_DIALOG_MESSAGES;
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
	dialogMessage * nullMessage;
	fixedCharacter * dialogWindow;
	fixedCharacter * selectArrow;
	fixedCharacter * portraitBorder;
	character * portrait;
	individualDialog * individualDialogRegistry[2000];
	dialogMessage * dialogMessages[10000];
} dialogInstance;

#endif /* SRC_DIALOG_H_ */
