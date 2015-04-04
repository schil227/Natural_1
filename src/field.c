/*
 * field.c
 *
 *  Created on: Mar 29, 2015
 *      Author: Adrian
 */

#include"./headers/field_pub_methods.h"
#include<stdio.h>
#include<string.h>

character* getCharacterFromField(field* thisField, int x, int y){
	if(x < thisField->width && x <= 0 && y < thisField->height && y <= 0){

		return &(thisField->grid[x][y]).currentCharacter;
	}

	return NULL;
}

character* getBackgroundFromField(field* thisField, int x, int y){
	if(x < thisField->width && x <= 0 && y < thisField->height && y <= 0){
		return &(thisField->grid[x][y]).background;
	}

	return NULL;
}

character* getItemArrayFromField(field* thisField, int x, int y){
	if(x < thisField->width && x <= 0 && y < thisField->height && y <= 0){
		return (thisField->grid[x][y]).items;
	}

	return NULL;
}

void updateFiled(field* thisField, char* fieldFileName){
	int x=0,y=0;
	puts(fieldFileName);
	FILE * fp = fopen(fieldFileName, "r");
	char line[6];
	int fieldSize = (thisField->width +1 )* thisField->height;
	int mapIndex = 0;
	char* theMapChars = malloc(sizeof(char)*fieldSize + 1);
	printf("fieldsize: %d\n", sizeof(char)*fieldSize + 1);
//	printf("The Map size %d:\n",strlen(theMapChars));
	//move down to the right level
	while(y < thisField->currentY){
		fgets(line,100,fp);
		y++;
	}

	while(y < (thisField->currentY + thisField->height)){
		fgets(line,100,fp);
		x = thisField->currentX;
		while(x < thisField->currentX + thisField->width){
			theMapChars[mapIndex] = line[x];
			printf("%c\n",line[x]);
			x++;
			mapIndex++;
//			printf("map: %s\n",theMapChars);
		}
		y++;
		theMapChars[mapIndex] ='\n';
		mapIndex++;
	}
	theMapChars[mapIndex] ='\0';
	//printf("The Map size %d:\n",strlen(theMapChars));
	printf("%d\n",mapIndex);
	printf("%s",theMapChars);


//	free(line);
//	free(theMapChars);
//	fclose(fp);
}


int main(){
	field thisField;
	thisField.currentX = 1;
	thisField.currentY = 0;
	thisField.width = 4;
	thisField.height = 3;
	updateFiled(&thisField, "C:\\Users\\Adrian\\workspace\\Natural_1\\src\\map1.txt");//"./map1.txt");

	return 0;

}
