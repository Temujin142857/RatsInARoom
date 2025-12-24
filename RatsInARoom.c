/*Welcome to Rats In A Room, the first Rat Oriented Programming Language! (Some experts argue it's a Room
oriented language, but pay them no mind, they're in the pocket of Big Cheese)*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//max length of a shortword will be 25 characters

const int MAXWORDLENGTH=32;
const int MAXRATS=144;


struct ccStack {
	int head;
	char array[MAXRATS][MAXWORDLENGTH];
};

struct textStack {
	int head;
	char **array;
};

struct iStack {
	int head;
	int array[];
};

int erno=0;//bool to detect if an error has occured in the program
int active = 0;//bool to indicate if reader is active


struct ccStack activeRats={-1};//for garbage collection, maybe organise by Crazy?
struct iStack ratsInFocus={-1};
struct iStack currentValues={-1};



int bracketDepth=0;


int currentState=0;
//2 is in the rubber room
//3 is focused on a rat
//4 is value has been extracted from rat

//99 is expecting endline



//util functions

//stack behavior, takes array and headPointer
int iHasNext(struct iStack *stack){
	return stack->head!=-1&&stack->array[stack->head]!=0;
}

int ccHasNext(struct ccStack *stack){
	return stack->head!=-1&&stack->array[stack->head]!=0;
}

int textHasNext(struct textStack *stack){
	return stack->head!=-1&&stack->array[stack->head]!=0;
}

int ccPush(struct ccStack *stack, char newElement[MAXWORDLENGTH]){
	if (stack->head >= MAXRATS - 1) {
        return 0; //stack overflow
    }

	stack->head++;
	strcpy(stack->array[stack->head],newElement);
	return 1;	
}

int textPush(struct textStack *stack, char newElement[MAXWORDLENGTH]){
	if (stack->head >= MAXRATS - 1) {
        return 0; //stack overflow
    }

	stack->head++;
	strcpy(stack->array[stack->head],newElement);
	return 1;	
}

char* ccPeek(struct ccStack *stack){
	if(stack->head!=-1){
		return stack->array[stack->head];
	}  else{ return 0; } 
}

char* textPeek(struct textStack *stack){
	if(stack->head!=-1){
		return stack->array[stack->head];
	}  else{ return 0; } 
}

int iPeek(struct iStack *stack){
	if(stack->head!=-1){
		return stack->array[stack->head];
	} else{ return 0; } 
}

char* ccPop(struct ccStack *stack){
	if(stack->head!=-1){
		char* temp= stack->array[stack->head];
		strcpy(stack->array[stack->head],"");
		stack->head--;
		return temp;
	}  else{ return 0; } 
}

char* textPop(struct textStack *stack){
	if(stack->head!=-1){
		char* temp= stack->array[stack->head];
		strcpy(stack->array[stack->head],"");
		stack->head--;
		return temp;
	}  else{ return 0; } 
}

int iPop(struct iStack *stack){
	if(stack->head!=-1){
		int temp= stack->array[stack->head];
		stack->array[stack->head]=0;
		stack->head--;
		return temp;
	}  else{ return 0; } 
}

int sIncludes(){

}


//make sure to review the memory management before doing this part 
struct textStack readFile(char filePath[]){
	FILE *file = fopen(filePath, "r");

	int readingWhiteSpace=0;
	int hitFirstWord=0;
	int wordCount=0;
	int tempTextCapacity=512;
	char *tempText=(char*)malloc(sizeof(char)*tempTextCapacity);
	int *wordLengths=(int*)malloc(sizeof(int)*tempTextCapacity);
	int globalCharacterCount=0;
	int characterHeadNum=0;


	char currentCharacter;
	while ((currentCharacter = fgetc(file)) != EOF){
		
		if(currentCharacter==whiteSpace){//fill in what == whiteSpace means
			readingWhiteSpace=1;			
		} else{
			if(readingWhiteSpace==1){wordCount++;characterHeadNum=0;}
			readingWhiteSpace=0;
			hitFirstWord=1;
		}

		if(!readingWhiteSpace){
			globalCharacterCount++;			

			if (globalCharacterCount >= tempTextCapacity) {
				tempTextCapacity = tempTextCapacity*2; 
				tempText= realloc(tempText, tempTextCapacity * sizeof(char));
				wordLengths=realloc(wordLengths, tempTextCapacity * sizeof(int));
				if (tempText == NULL||wordLengths==NULL) {
					printf("Error: Memory allocation failed!\n");
					free(tempText); // Clean up any previously allocated memory
					fclose(file);
					erno=1;
					struct textStack failure= {-1};
					return failure;
				}
        	}
			characterHeadNum++;
			tempText[(wordCount*MAXWORDLENGTH)+characterHeadNum]=currentCharacter;	
		}
	}
	fclose(file);

	char **array = (int **)malloc(wordCount * sizeof(int *));

	int characterCount=0;
	for (int i=0;i<wordCount;i++){
		array[i] = (char *)malloc(wordLengths[i] * sizeof(char));
		for (int j = 0; j < wordLengths[i]; j++){			
			array[i][j]=tempText[characterCount];
			characterCount++;
		}
	}

	free(tempText);
	//put tempText into the text stack
    struct textStack text = {wordCount, array};
	return text;
}

//1D to 2D arrayArithmatic (word, character)
// return (word*maxCharacterCount)+character


int printToOutputFile(){

}


//speak, kill, hold, take, instruct, do
char lexicalAnalyser(char currentWord[]){
	switch(currentWord[0]){
		case 'C': if(strcmp(currentWord, "Crazy?")==0){return 1;}break;
		case 'R': if(strcmp(currentWord, "RubberRoom")==0){return 2;}break;
		case 's': if(strcmp(currentWord, "speak")==0){return 11;}break;
		case 'k': if(strcmp(currentWord, "kill")==0){return 12;}break;
		case 'h': if(strcmp(currentWord, "hold")==0){return 13;}break;
		case 't': if(strcmp(currentWord, "take")==0){return 14;}break;
		case 'i': if(strcmp(currentWord, "instruct")==0){return 15;}break;
		case 'd': if(strcmp(currentWord, "do")==0){return 16;}break;
		default: return 0;break;		
	}
}


char *translateWord(char currentWord[]){//takes a word, returns a pointer to the char[] which should be printed into the compiled file
	if(erno){return;}
	//if the current word is a long word, break it into short words and pass it back to this function
	if(strstr(currentWord,".")!=NULL){
		int size_t;
		int *t =strchr(currentWord,'.');
		char firstWord[]=currentWord;
		char restOfWord[]=currentWord+*t;
		translateWord(currentWord);
		translateWord(restOfWord);
		return 0;
	}	

	//also need to figure out how to split by brackets

	char* token;
	char tokenValue=lexicalAnalyser(currentWord);
	if (tokenValue==-1){erno=1;return -1;}
	else if(tokenValue==0){token=currentWord;}
	

	switch (tokenValue){
		case 1: interpretCrazy(); break;
		case 2: if(currentState==1){currentState=2;}else{throwSyntaxError(2);}break;

		case 11: if(currentState!=3){throwSyntaxError(10);break;}if(!ratHasValue(peek(ratsInFocus))){throwSyntaxError(11);break;}
				printToOutputFile("printf(Value(RubberRoom[",peek(ratsInFocus), "]))");pop(ratsInFocus);currentState=99;break;

		case 12: if(currentState!=3){throwSyntaxError(10);break;}if(!peek(ratsInFocus)){throwSyntaxError(11);break;}
				printToOutputFile("RubberRoom[", peek(ratsInFocus),"]=NULL");pop(ratsInFocus);currentState=99;break;	
		
		case 13: if(currentState!=3){throwSyntaxError(10);break;}				
				char value[]="extracted value";
				printToOutputFile("Save(RubberRoom[", peek(ratsInFocus), "],");
				//extract value from in brackets, note it might be a value taken from a rat, check that it's a string or int or float
		
		case 14: if(currentState!=3){throwSyntaxError(10);break;}if(!ratHasValue(peek(ratsInFocus))){throwSyntaxError(14);break;}



		case 0: if(currentState==2){focusOnRat(currentWord);break;}
	}		
}



int main(char filePath[])
{	

	readFile(filePath);


	//print the constant functions:
	//Value
	//Save

	//Initialise the RubberRoom

	/*

	while(ccHasNext(&text)){
		char currentWord[]=ccPop(&text);
		translateWord(currentWord);
		cleanup();
	}
	*/
	free(text.array);

    return 0;  
}






/*


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

	ratsInFocus.push(ratname); write(ratName);
}


int throwSyntaxError(info){
	//10 is no rat in focus
}


//this gets called after each full word
function cleanup(){
	//clear rats in focus
}

*/


