/*Welcome to Rats In A Room, the first Rat Oriented Programming Language! (Some experts argue it's a Room
oriented language, but pay them no mind, they're in the pocket of Big Cheese)*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//max length of a shortword will be 25 characters

const int MAXWORDLENGTH=32;
const int MAXRATS=144;
const int MAXBUFFERLENGTH=1024;
const int AMOUNTOFSYMBOLS=8;

const char symbols[AMOUNTOFSYMBOLS][10]={"Crazy?","RubberRoom", "speak", "kill", "hold", "take", "instruct", "do" };

const int symbolsFoot=1;
const int iRatsFoot=symbolsFoot+AMOUNTOFSYMBOLS;
const int literalsFoot=iRatsFoot+MAXRATS;
/*
int iLiteralsFoot=iRatsFoot+MAXRATS;
int cLiteralsFoot=iRatsFoot+MAXRATS+1;
int SLiteralsFoot=iRatsFoot+MAXRATS+2;
int fLiteralsFoot=iRatsFoot+MAXRATS+3;
*/

struct rat {
	char* name;
	int valueType;
	//0 for no valueType
	//1 for int
	//2 for string

	int functionType;
	int *functionParams;
	//0 for no params
	//1 for int
	//2 for string
	//9 for rat

};

struct ratStack {
	char **array;
	int head;
};


struct ccStack {
	int head;
	char **array;
};

struct cStack{
	int head;
	char *array;
};

struct fStack{
	int head;
	float *array;
};

struct textStack {
	int head;
	char **array;
};

struct iStack {
	int head;
	int *array;
};

struct outputBufferStack{
	int head;
	char array[1024];
};

int erno=0;//bool to detect if an error has occured in the program
int active = 0;//bool to indicate if reader is active
struct outputBufferStack outputBuffer={-1};
char *outputFileName;


struct ratStack rats={-1};//for garbage collection, maybe organise by Crazy?
struct iStack ratsInFocus={-1};
struct iStack currentValues={-1};

struct iStack iLiterals={-1};
struct ccStack sLiterals={-1};
struct cStack cLiterals={-1};
struct fStack fLiterals={-1};
/* literal sorage will follow the following logic:
token=symbolsFoot + 4*indexInStorage+(0 or 1 or 2 or 3) depending on if it's int, string, char or float
Retreival will work like indexInStorage=(token-symbolsFoot)%4 determine if it's int, string, char or float, use floor(token/4) to deternime it's indexInStorage
*/


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

//release the memory of a textstack
void textFree(struct textStack *text)
{
    if (!text || !text->array) return;

    for (size_t i = 0; i < text->head; i++) {
        free(text->array[i]);
    }

    free(text->array);

    // defensive: prevent use-after-free
    text->array = NULL;
    text->head = 0;
}

int sIncludes(){

}


//rat specific functions

int validateArgs(){

}




//make sure to review the memory management before doing this part 
struct textStack readFile(char filePath[]){
	printf("attempting to open file: %s\n", filePath);
	FILE *file = fopen(filePath, "r");
	printf("successfully opened file\n");

	int readingWhiteSpace=1;
	int hitFirstWord=0;
	int wordCount=0;
	int tempTextCapacity=512;
	char *tempText=(char*)malloc(sizeof(char)*tempTextCapacity);
	int *wordLengths=(int*)malloc(sizeof(int)*tempTextCapacity);
	int globalCharacterCount=-1;
	int characterHeadNum=0;


	char currentCharacter;
	while ((currentCharacter = fgetc(file)) != EOF){
		
		if(currentCharacter=='\n'){//fill in what == whiteSpace means
			readingWhiteSpace=1;			
		} else{
			if(readingWhiteSpace==1&&hitFirstWord){wordLengths[wordCount]=characterHeadNum;wordCount++;characterHeadNum=0;}
			readingWhiteSpace=0;
			hitFirstWord=1;
		}

		if(!readingWhiteSpace){
			globalCharacterCount++;		
			characterHeadNum++;	

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
			tempText[globalCharacterCount]=currentCharacter;						
		}
	}
	wordLengths[wordCount]=characterHeadNum;
	wordCount++;
	fclose(file);

	char **array = (char **)malloc((wordCount) * sizeof(char *));

	int characterCount=0;
	for (int i=0;i<wordCount;i++){
		array[i] = (char *)malloc((wordLengths[i]+1) * sizeof(char));
		for (int j = 0; j < wordLengths[i]; j++){		
			array[i][j]=tempText[characterCount];
			characterCount++;
		}
		array[i][wordLengths[i]]='\0';
	}	

	free(tempText);
	free(wordLengths);

	//put tempText into the text stack
    struct textStack text = {wordCount-1, array};	
	array=NULL;
	
	return text;
}

//1D to 2D arrayArithmatic (word, character)
// return (word*maxCharacterCount)+character


int flushOutputBuffer(){
	FILE *fptr;
	//a is for append mode
	fptr = fopen(outputFileName, "a");
	printf("\nattempting to print: %s\n", outputBuffer.array);
	fprintf(fptr, "%s" , outputBuffer.array);
	fclose(fptr);

	memset(outputBuffer.array, '\0', sizeof(outputBuffer.array));
	outputBuffer.head=-1;
	return 1;
}

int immediateFlush(){

}

int printToOutputBuffer(char string[]){
	size_t incomingStringLength=strlen(string);
	if(incomingStringLength>1024){
		printf("why is the ouptut bigger than 1024 chars?");
		//immediateFlush(string);
		return 0;
	}
	else if((incomingStringLength+outputBuffer.head+1)>1024){
		printf("why here?");
		flushOutputBuffer();
	};
	printf("attampting to add: %s, to buffer\n", string);
	for (int i = 0; i < incomingStringLength; i++){
		outputBuffer.array[outputBuffer.head+1]=string[i];
		outputBuffer.head++;
	}
	return 1;
}


//speak, kill, hold, take, instruct, do
char lexicalAnalyser(char currentWord[]){
	switch(currentWord[0]){
		case 'C': if(strcmp(currentWord, "Crazy?")==0){return 1;}break;
		case 'R': if(strcmp(currentWord, "RubberRoom")==0){return 2;}break;
		case 's': if(strcmp(currentWord, "speak")==0){return 3;}break;
		case 'k': if(strcmp(currentWord, "kill")==0){return 4;}break;
		case 'h': if(strcmp(currentWord, "hold")==0){return 5;}break;
		case 't': if(strcmp(currentWord, "take")==0){return 6;}break;
		case 'i': if(strcmp(currentWord, "instruct")==0){return 7;}break;
		case 'd': if(strcmp(currentWord, "do")==0){return 8;}break;
		case '\"':break;		
		case '\'':break;
	}
	if(isDigit(currentWord[0])){
		//int case
		//float case
	} else if(isAlpha(currentWord[0])){
	    //ratCase
	}
	return 0;
}


int throwSyntaxError(){

}
/*
char *translateWord(char currentWord[]){//takes a word, returns a pointer to the char[] which should be printed into the compiled file
	if(erno){return;}
	//if the current word is a long word, break it into short words and pass it back to this function
	
	char *t =strchr(currentWord,'.');
	if(t!=NULL){
		*t = '\0';
    	t++; 
		translateWord(currentWord);
		translateWord(t);
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

		case 11: if(currentState!=3){throwSyntaxError(10);break;}if(!ratHasValue(iPeek(&ratsInFocus))){throwSyntaxError(11);break;}
				printToOutputFile("printf(Value(RubberRoom[",iPeek(&ratsInFocus), "]))");iPop(&ratsInFocus);currentState=99;break;

		case 12: if(currentState!=3){throwSyntaxError(10);break;}if(!iPeek(&ratsInFocus)){throwSyntaxError(11);break;}
				printToOutputFile("RubberRoom[", iPeek(&ratsInFocus),"]=NULL");iPop(&ratsInFocus);currentState=99;break;	
		
		case 13: if(currentState!=3){throwSyntaxError(10);break;}				
				char value[]="extracted value";
				printToOutputFile("Save(RubberRoom[", iPeek(&ratsInFocus), "],");
				//extract value from in brackets, note it might be a value taken from a rat, check that it's a string or int or float
		
		case 14: if(currentState!=3){throwSyntaxError(10);break;}if(!ratHasValue(iPeek(&ratsInFocus))){throwSyntaxError(14);break;}



		case 0: if(currentState==2){focusOnRat(currentWord);break;}
	}		
}

*/

int main(int argc, char *argv[]){
	printf("active\n");
	//validateArgs();
	if (argc < 2) {
   		printf("No input file provided\n");
   		return 1;
	}
	struct textStack text=readFile(argv[1]);
	printf("\namount of lines in text: %d\n", text.head+1);
	for (int i = 0; i <= text.head; i++)
	{
    	printf("line%d: %s\n", i, text.array[i]);
	}
	printf("\n");
	
	char *partialOutputFileName=strtok(argv[1],".");
	int partialLength=strlen(partialOutputFileName);
	char fullOutputFileName[partialLength+2];
	for (int i = 0; i < partialLength; i++){
		fullOutputFileName[i]=partialOutputFileName[i];
	}
	fullOutputFileName[partialLength]='.';
	fullOutputFileName[partialLength+1]='c';
	fullOutputFileName[partialLength+2]='\0';
	printf("ouputFilename: %s\n",fullOutputFileName);
	outputFileName=fullOutputFileName;
	

	if (remove(outputFileName) == 0) {
        printf("Output File deleted successfully\n\n");
    } else {
        printf("Ouput file not already present\n\n");
    }

	for (int i = 0; i <= text.head; i++)
	{
    	printToOutputBuffer(text.array[i]);
	}
	
	

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
	flushOutputBuffer();
	textFree(&text);
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


