/*Welcome to Rats In A Room, the first Rat Oriented Programming Language! (Some experts argue it's a Room
oriented language, but pay them no mind, they're in the pocket of Big Cheese)*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool active =false;
char[][] activeRats=[];//for garbage collection, maybe organise by Crazy? stack
char[][] ratInFocus=[];//stack
bool erno=false;
char** text; //spilt by whitespace
int maxCharacterCount=0;
int maxWordCount=0;
int wordHeadNum=0;
int characterHeadNum=0;


int main(char[] filePath)
{
	
	

	while(hasNext(&text, &textHeadPointer)){
		char[] currentWord=next(text, textHeadPointer);
		translateWord(currentWord);
	}


    return 0;  
}


function translateWord(char[] currentWord){
	if(erno){return;}
	if(strstr(currentWord,".")!=NULL){
		int size_t
		int *t strchr(currentWord,'.');
		char[] *firstWord=currentWord;
		char[] *restOfWord=currentWord+t;
		translateWord(currentWord);
		translateWord(restOfWord);
		return 0;
	}	

	if (currentWord.includes("killRat")){interpretFunction(currentWord, "killRat");}
	else if (currentWord.includes("speak")){interpretFunction(currentWord, "speak");}
	else if (currentWord.includes("hold")){interpretFunction(currentWord, "hold");}
	else if (currentWord.includes("take")){interpretFunction(currentWord, "take");}
	//else if (currentWord.includes("linguistic case")){{interpretFunction(currentWord, "take");}}
	//else if (currentWord.includes("mathematics case")){{interpretFunction(currentWord, "take");}}
	else{
		switch currentWord{
			case "Crazy?": interpretCrazy(); break;
			case "RubberRoom": if(currentState!="RubberRoom"){currentState="RubberRoom";}else{compilatioin error}break;
			default: if(currentState=="RubberRoom"){ focusOnRat(currentWord);break;}
		}		
	}
	

}


function interpretFunction(currentWord, instruction){

}

function interpretCrazy(){
	if(!active){
		active=true;
		write("while(true){");
		write(RubberRoom=new array;)
	}else{
		write("}")
	}
	return;
}

function write(char code){
	if(code.includes(";")){write(code.remove(";"));code=";\n"}
	print(code);
}

//make a new rat if it doesn't exist, then focus on a rat
function focusOnRat(char ratName){
	if(!activeRats.contains(ratName)){activeRats.push(ratName);write("new object "+ratName+";");}
	//double check the syntax in C, maybe use a class, but each rat should have this function
	rat.makeCrazy=new function (char phrase){system.out.console.log(phrase);}

	ratInFocus=ratname; write(ratName);
}

//util functions

//stack behavior, takes array and headPointer
char hasNext(int* array, int* headPointer){
	if(sizeOf(array))
}


readFile(char[] filePath){
	FILE *file = fopen(filePath, "r");

	bool readingWhiteSpace=false;
	bool hitFirstWord=false;
	int tempTextCapacity=512;
	char tempText[]


	char currentCharacter;
	while ((currentCharacter = fgetc(file)) != EOF){
		
		if(currentCharacter==whiteSpace){//fill in what == whiteSpace means
			readingWhiteSpace=true;
			characterHeadNum=0;
		} else{
			readingWhiteSpace=false;
			if(hitFirstWord)wordHeadNum++;
			hitFirstWord=true;
		}

		if(!readingWhiteSpace){
			characterCount++;			

			if (characterCount >= tempTextCapacity) {
				tempTextCapacity = tempTextCapacity*2; 
				int *temp= realloc(tempText, tempTextCapacity * sizeof(char));
				if (temp == NULL) {
					printf("Error: Memory allocation failed!\n");
					free(tempText); // Clean up any previously allocated memory
					fclose(file);
					erno=true;
					return 1;
				}
				tempText = temp; // Update tempText to point to the new memory block
        	}
			characterHeadNum++;
			tempText[(wordHeadNum*maxCharacterCount)+characterHeadNum]=currentCharacter;	
		}
	}

	//put tempText into an appropriatly sized text[][] https://www.geeksforgeeks.org/c/c-pointer-to-pointer-double-pointer
	// Allocate memory for rows
    text = (char**)malloc(wordHeadNum * sizeof(char*)); 

    // Allocate memory for each colum
	int i;
    for (i = 0; i < wordHeadNum; i++){
        text[i] = (char*)malloc(maxCharacterCount * sizeof(char));
	}

	int j;
	for (i = 0; i < wordHeadNum; i++){
		while(tempText[i*maxCharacterCount+]!=null)
	}

	for ()

	
	free(tempText);
	return 0;
}

//1D to 2D arrayArithmatic (word, character)
// return (word*maxCharacterCount)+character

