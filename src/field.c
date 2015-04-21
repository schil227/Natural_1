/*
 * field.c
 *
 *  Created on: Mar 29, 2015
 *      Author: Adrian
 */

#include"./headers/field_pub_methods.h"
#include<stdio.h>
#include<string.h>

individual* getIndividualFromField(field* thisField, int x, int y){
	if(x < thisField->displayedWidth && x <= 0 && y < thisField->displayedHeight && y <= 0){

		return (thisField->grid[x][y])->currentIndividual;
	}

	return NULL;
}

character* getBackgroundFromField(field* thisField, int x, int y){
	if(x < thisField->displayedWidth && x <= 0 && y < thisField->displayedHeight && y <= 0){
		return (thisField->grid[x][y])->background;
	}

	return NULL;
}

//character* getItemArrayFromField(field* thisField, int x, int y){
//	if(x < thisField->displayedWidth && x <= 0 && y < thisField->displayedHeight && y <= 0){
//		return (thisField->grid[x][y]).items;
//	}
//
//	return NULL;
//}

/*
 * take in a character, output the corrosponding background name (string)
*/
int generateBackground(char backgroundSymbol){


	printf("backgroundChar: %c\n", backgroundSymbol);

	if(backgroundSymbol == 'c'){
		printf("Found  a C!\n");
		return 2003;
	}else{
		return 2002; //grass background
	}
}

field* initField(char* fieldFileName){
	field* thisField = malloc(sizeof(field));
	FILE * fp = fopen(fieldFileName, "r");
	char line[80];
	int init_y = 0;
	int init_x;
	while(fgets(line,80,fp) != NULL){

		for(init_x = 0; init_x < strlen(line); init_x++){

			space* newSpace = malloc(sizeof(space));
			character* backgroundCharacter = malloc(sizeof(character));
			char currentChar = line[init_x];

			//initialize background
			backgroundCharacter->width = 40;
			backgroundCharacter->height = 40;
			backgroundCharacter->x = init_x * 40;
			backgroundCharacter->y = init_y * 40;
			backgroundCharacter->name = &currentChar;
			backgroundCharacter->imageID = generateBackground(currentChar);

			newSpace->background = backgroundCharacter;
			thisField->grid[init_x][init_y] = newSpace;

		}
		init_y++;

		puts(line);
		printf("len:%d\n", strlen(line));
	}

	thisField->totalX = init_x;
	thisField->totalY = init_y;

	fclose(fp);

	return thisField;
}


void updateFiled(field* thisField, char* fieldFileName){
	int x=0,y=0;
	puts(fieldFileName);
	FILE * fp = fopen(fieldFileName, "r");
	char line[80];
	int fieldSize = (thisField->displayedWidth +1 )* thisField->displayedHeight;
	int mapIndex = 0;
	char* theMapChars = malloc(sizeof(char)*fieldSize + 1);
	printf("fieldsize: %d\n", sizeof(char)*fieldSize + 1);
//	printf("The Map size %d:\n",strlen(theMapChars));
	//move down to the right level
	while(y < thisField->displayedY){
		fgets(line,100,fp);
		y++;
	}

	while(y < (thisField->displayedY + thisField->displayedHeight)){
		fgets(line,100,fp);
		x = thisField->displayedX;
		while(x < thisField->displayedX + thisField->displayedWidth){
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


//	free(theMapChars); //breaking here for some reason
	fclose(fp);
}


int b_main(){
	field* thisField;
//	thisField.displayedX = 1;
//	thisField.displayedY = 0;
//	thisField.displayedWidth = 4;
//	thisField.displayedHeight = 3;
	thisField = initField( "C:\\Users\\Adrian\\workspace\\Natural_1\\src\\map1.txt");
//	updateFiled(&thisField, "C:\\Users\\Adrian\\workspace\\Natural_1\\src\\map1.txt");//"./map1.txt");

	return 0;

}
