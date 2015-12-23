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
	int eventType;

	int	individualID;
	int itemID;

} eventFlag;

typedef struct{
	int messageID;
	char message[256];
	int numDialogDecision;

	int dialogCheckpoint; //if the user should start at this message
	eventFlag * event; //the event this flag triggers

	int nextMessageID;
	struct dialogMessage * nextMessage;
	struct dialogDecision * decisions[10];
}dialogMessage;

typedef struct{
	int rootMessageID;
	int targetMessageID;
	char message[256];
	dialogMessage * targetMessage;
} dialogDecision;


typedef struct{
	int drawBox;
	dialogMessage * rootMessage;
	dialogMessage * currentMessage;
	character * dialogWindow;
} dialogBox;

#endif /* SRC_DIALOG_H_ */
