/*
 * sound.c
 *
 *  Created on: May 12, 2016
 *      Author: Adrian
 */
#include "./headers/sound_pub_methods.h"

static soundPlayer * soundPlayerInstance;
static int soundEnabled = 0;

void disableSound(){
	soundEnabled = 0;
}

void enableSound(){
	soundEnabled = 1;
}

void initSoundPlayerInstance(){
	soundPlayerInstance = malloc(sizeof(soundPlayer));

	soundPlayerInstance->music = malloc(sizeof(soundContainer));
	soundPlayerInstance->sound1 = malloc(sizeof(soundContainer));
	soundPlayerInstance->sound2 = malloc(sizeof(soundContainer));

	soundPlayerInstance->device = alcOpenDevice(NULL);
	soundPlayerInstance->context = alcCreateContext(soundPlayerInstance->device, NULL);
	alcMakeContextCurrent(soundPlayerInstance->context);

	alGenSources(1, &(soundPlayerInstance->music->source));
	alGenSources(1, &(soundPlayerInstance->sound1->source));
	alGenSources(1, &(soundPlayerInstance->sound2->source));

	soundPlayerInstance->music->sendInterrupt = 0;
	soundPlayerInstance->sound1->sendInterrupt = 0;
	soundPlayerInstance->sound2->sendInterrupt = 0;

	soundPlayerInstance->music->isPlaying = 0;
	soundPlayerInstance->sound1->isPlaying = 0;
	soundPlayerInstance->sound2->isPlaying = 0;

	soundPlayerInstance->music->shouldLoop = 1;
	soundPlayerInstance->sound1->shouldLoop = 0;
	soundPlayerInstance->sound2->shouldLoop = 0;

	soundPlayerInstance->onDeckSoundContainer = 1;

}

void destroySoundPlayer(){
//	alDeleteSources(1, sd->source);                                                //Delete the OpenAL Source
//	alDeleteBuffers(1, &buffers);                                                 //Delete the OpenAL Buffer
//	alcMakeContextCurrent(NULL);                                                //Make no context current
//	alcDestroyContext(context);                                                 //Destroy the OpenAL Context
//	alcCloseDevice(device);
}


DWORD WINAPI playSound(soundContainer * thisSoundContainer){
	int playAfterInturrupt = 0;

	do{
	playAfterInturrupt = 0;
	thisSoundContainer->isPlaying = 1;
	FILE *fp = NULL;
	fp=fopen(thisSoundContainer->fileName,"rb");

	alGenSources(1, &(thisSoundContainer->source));

	unsigned char * buf;
	char type[4];
	DWORD size, chunkSize;
	short formatType, channels;
	DWORD sampleRate, avgBytesPerSec;
	short bytesPerSample, bitsPerSample;
	DWORD dataSize;
	ALuint buffers[NUM_BUFFERS];

	fread(type, sizeof(char), 4, fp);
	fread(&size, sizeof(DWORD), 1, fp);
	fread(type, sizeof(char), 4, fp);
	fread(type, sizeof(char), 4, fp);
	fread(&chunkSize, sizeof(DWORD), 1, fp);
	fread(&formatType, sizeof(short), 1, fp);
	fread(&channels, sizeof(short), 1, fp);
	fread(&sampleRate, sizeof(DWORD), 1, fp);
	fread(&avgBytesPerSec, sizeof(DWORD), 1, fp);
	fread(&bytesPerSample, sizeof(short), 1, fp);
	fread(&bitsPerSample, sizeof(short), 1, fp);
	fread(type, sizeof(char), 4, fp);
	fread(&dataSize, sizeof(DWORD), 1, fp);

	ALuint frequency = sampleRate;
	ALenum format = 0;

	if (bitsPerSample == 8) {
		if (channels == 1)
			format = AL_FORMAT_MONO8;
		else if (channels == 2)
			format = AL_FORMAT_STEREO8;
	} else if (bitsPerSample == 16) {
		if (channels == 1)
			format = AL_FORMAT_MONO16;
		else if (channels == 2)
			format = AL_FORMAT_STEREO16;
	}

	alGenBuffers(NUM_BUFFERS, buffers);

	int ret;
	buf = malloc(BUFFER_SIZE);

	ret = fread(buf, 1, BUFFER_SIZE, fp);
	alBufferData(buffers[0], format, buf, ret, frequency);
	ret = fread(buf, 1, BUFFER_SIZE, fp);
	alBufferData(buffers[1], format, buf, ret, frequency);
	ret = fread(buf, 1, BUFFER_SIZE, fp);
	alBufferData(buffers[2], format, buf, ret, frequency);

	//Sound setting variables
	ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };      //Position of the source sound
	ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };      //Velocity of the source sound
	ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };        //Position of the listener
	ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };        //Velocity of the listener
	ALfloat ListenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 }; //Orientation of the listener

	alListenerfv(AL_POSITION, ListenerPos);       //Set position of the listener
	alListenerfv(AL_VELOCITY, ListenerVel);       //Set velocity of the listener
	alListenerfv(AL_ORIENTATION, ListenerOri); //Set orientation of the listener

	alSourcef(thisSoundContainer->source, AL_PITCH, 1.0f); //Set the pitch of the source
	alSourcef(thisSoundContainer->source, AL_GAIN, 1.0f); //Set the gain of the source
	alSourcefv(thisSoundContainer->source, AL_POSITION, SourcePos); //Set the position of the source
	alSourcefv(thisSoundContainer->source, AL_VELOCITY, SourceVel); //Set the velocity of the source
	alSourcei(thisSoundContainer->source, AL_LOOPING, AL_FALSE); //Set if source is looping sound

//PLAY
	alSourceQueueBuffers(thisSoundContainer->source, NUM_BUFFERS, buffers);
	alSourcePlay(thisSoundContainer->source);

	while (!feof(fp)) {
		ALuint buffer;
		ALint val;

		if(thisSoundContainer->sendInterrupt){
			break;
		}

		// Check if OpenAL is done with any of the queued buffers
		alGetSourcei(thisSoundContainer->source, AL_BUFFERS_PROCESSED, &val);
		if(thisSoundContainer->sendInterrupt){
			break;
		}
		if (val <= 0){
			continue;
		}

		// For each processed buffer...
		while (val--) {
			if(thisSoundContainer->sendInterrupt){
				break;
			}
			// Read the next chunk of decoded data from the stream
			ret = fread(buf, 1, BUFFER_SIZE, fp);

			// Pop the oldest queued buffer from the source, fill it
			// with the new data, then requeue it
			alSourceUnqueueBuffers(thisSoundContainer->source, 1, &buffer);
			alBufferData(buffer, format, buf, ret, frequency);
			alSourceQueueBuffers(thisSoundContainer->source, 1, &buffer);

			if (alGetError() != AL_NO_ERROR) {
				printf("Error buffering :(\n");
				return 1;
			}
		}

		if(thisSoundContainer->sendInterrupt){
			break;
		}
	}
	if(thisSoundContainer->sendInterrupt){
		thisSoundContainer->sendInterrupt = 0;
//		thisSoundContainer->isPlaying = 0;
		strcpy(thisSoundContainer->fileName, getSoundPathFromRegistry(thisSoundContainer->currentSoundId));
		playAfterInturrupt = 1;
		fclose(fp);                                   //Delete the sound data buffer

	}

	alDeleteSources(1, &thisSoundContainer->source);

	}while(thisSoundContainer->shouldLoop || playAfterInturrupt);

	thisSoundContainer->isPlaying = 0;
	return EXIT_SUCCESS;
}

void sendMusicInterrupt(int newId){
	if(!soundEnabled){
		return;
	}

	soundPlayerInstance->music->sendInterrupt = 1;
	soundPlayerInstance->music->currentSoundId = newId;
}

void setSoundID(int id, soundType thisSoundType){
	switch(thisSoundType){
		case SOUND_MUSIC:
			soundPlayerInstance->music->currentSoundId = id;
		break;
		case SOUND_SOUND1:
			soundPlayerInstance->sound1->currentSoundId = id;
		break;
		case SOUND_SOUND2:
			soundPlayerInstance->sound2->currentSoundId = id;
		break;
	}
}

void triggerSoundEffect(int id){
	if(!soundEnabled){
		return;
	}

	if(!soundPlayerInstance->sound1->isPlaying){
		soundPlayerInstance->onDeckSoundContainer = 2;
		soundPlayerInstance->sound1->currentSoundId = id;
		strcpy(soundPlayerInstance->sound1->fileName, getSoundPathFromRegistry(soundPlayerInstance->sound1->currentSoundId));
		CreateThread( NULL, 0, playSound, soundPlayerInstance->sound1, 0, NULL);
	}else if(!soundPlayerInstance->sound2->isPlaying){
		soundPlayerInstance->onDeckSoundContainer = 1;
		soundPlayerInstance->sound2->currentSoundId = id;
		strcpy(soundPlayerInstance->sound2->fileName, getSoundPathFromRegistry(soundPlayerInstance->sound2->currentSoundId));
		CreateThread( NULL, 0, playSound, soundPlayerInstance->sound2, 0, NULL);
	}
	else{ //both are playing, interrupt oldest
		if(soundPlayerInstance->onDeckSoundContainer == 1){
			soundPlayerInstance->sound1->currentSoundId = id;
			soundPlayerInstance->sound1->sendInterrupt = 1;
			soundPlayerInstance->onDeckSoundContainer = 2;
		} else if(soundPlayerInstance->onDeckSoundContainer == 2){
			soundPlayerInstance->sound2->currentSoundId = id;
			soundPlayerInstance->sound2->sendInterrupt = 1;
			soundPlayerInstance->onDeckSoundContainer = 1;
		}
	}
}

void testPlaySounds(){
	strcpy(soundPlayerInstance->music->fileName, getSoundPathFromRegistry(soundPlayerInstance->music->currentSoundId));
	strcpy(soundPlayerInstance->sound1->fileName, getSoundPathFromRegistry(soundPlayerInstance->sound1->currentSoundId));

	CreateThread( NULL, 0, playSound, soundPlayerInstance->music, 0, NULL);

	CreateThread( NULL, 0, playSound, soundPlayerInstance->sound1, 0, NULL);
}

soundMap * createSoundMapFromLine(char * line){
	soundMap * newSoundMap = malloc(sizeof(soundMap));

	char * value = strtok(line,";");
	newSoundMap->ID = atoi(value);

	value = strtok(NULL,";");
	strcpy(newSoundMap->fileName,value);
	newSoundMap->fileName[strlen(newSoundMap->fileName)-1] = '\0';

	return newSoundMap;
}
