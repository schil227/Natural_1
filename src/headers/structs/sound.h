/*
 * sound.h
 *
 *  Created on: May 12, 2016
 *      Author: Adrian
 */

#ifndef SRC_HEADERS_STRUCTS_SOUND_H_
#define SRC_HEADERS_STRUCTS_SOUND_H_
#include <AL/al.h>
#include <AL/alc.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#define NUM_BUFFERS 3
#define BUFFER_SIZE 4096
#define NUM_SOURCES 3

typedef struct {
	int currentSoundId;
	int isPlaying;
	int sendInturrupt;
	char fileName[512];
	ALuint source;
} soundContainer;

typedef struct {
	ALCdevice *device;
	ALCcontext *context;

	soundContainer * music;
	soundContainer * sound1;
	soundContainer * sound2;
} soundPlayer;

#endif /* SRC_HEADERS_STRUCTS_SOUND_H_ */

