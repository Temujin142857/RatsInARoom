/*Welcome to Rats In A Room, the first Rat Oriented Programming Language! (Some experts argue it's a Room
oriented language, but pay them no mind, they're in the pocket of Big Cheese)*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <utils.h>



//max length of a shortword will be 25 characters

const int MAXWORDLENGTH=32;
const int MAXBUFFERLENGTH=1024;
const int AMOUNTOFSYMBOLS=14;

const int iIdentifier=0;
const int cIdentifier=1;
const int sIdentifier=2;
const int fIdentifier=3;
const int rIdentifier=4;
const int identifierNum=5;


const char symbols[AMOUNTOFSYMBOLS][10]={";", "Crazy?","RubberRoom", "speak", "kill", "hold", "show", "take", "instruct", "do", "spawn", ".", "(", ")" };
const int SEMICOLON=-1;
const int CRAZY=1;
const int RUBBERROOM=2;
const int SPEAK=3;
const int KILL=4;
const int HOLD=5;
const int SHOW=6;
const int TAKE=7;
const int INSTRUCT=8;
const int DO=9;
const int SPAWN=10;
const int DOT=11;
const int OPENBRACKET=12;
const int CLOSEBRACKET=13;

const int symbolsFoot=1;
const int variablesFoot=symbolsFoot+AMOUNTOFSYMBOLS;
/*
const int literalsFoot=iRatsFoot+MAXRATS;
int iLiteralsFoot=iRatsFoot+MAXRATS;
int cLiteralsFoot=iRatsFoot+MAXRATS+1;
int SLiteralsFoot=iRatsFoot+MAXRATS+2;
int fLiteralsFoot=iRatsFoot+MAXRATS+3;
*/



int erno=0;//bool to detect if an error has occured in the program
int active = 0;//bool to indicate if reader is active
struct outputBufferStack outputBuffer={-1};
char *outputFileName;


struct ratStack activeRats={-1};//for garbage collection, maybe organise by Crazy?
struct iStack ratsInFocus={-1};
struct iStack currentValues={-1};

struct iStack iLiterals={-1};
struct ccStack sLiterals={-1};
struct cStack cLiterals={-1};
struct fStack fLiterals={-1};
/* literal storage will follow the following logic:
token=symbolsFoot + 4*indexInStorage+(0 or 1 or 2 or 3) depending on if it's int, string, char or float
Retreival will work like indexInStorage=(token-symbolsFoot)%4 determine if it's int, string, char or float, use floor(token/4) to deternime it's indexInStorage
*/



int currentState=0;
//2 is in the rubber room
//3 is focused on a rat
//4 is value has been extracted from rat

//99 is expecting endline


int validateArgs(){

}

//working and done
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

//working and done
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


int throwSyntaxError(char message[], char token[]){

}

int paramsAnalyser(char params[], int *output[], int single){
	if(single){
		int length=strlen(params);
		if(params[0]== '\"'){
			if(params[length-1]=='\"'){int location=ccPush(&sLiterals, params); return location*identifierNum+sIdentifier+variablesFoot;} 
			else{throwSyntaxError("string not terminated by \"", params); return 0;}
		}	
		else if(params[0]=='\''){if(params[length-1]=='\''&&length==3){int location =cPush(&cLiterals, params); return location*identifierNum+cIdentifier+variablesFoot;} 
			else{throwSyntaxError("invalid character literal", params); return 0;}	
		}		
		else if(isDigit(params[0])){
			int foundDot=0;
			for (size_t i = 0; i < strlen(params); i++)
			{
				if(!isDigit(params[i])){
					if(params[i]=='.'&&!foundDot){foundDot=1;}
					else if(params[i]=='.'&&foundDot){throwSyntaxError("multiple . in number ", params);return 0;}
					else {throwSyntaxError("invalid character in number", params); return 0;}
				}
			}
			if (!foundDot){int location =iPush(&iLiterals, params); return location*identifierNum+iIdentifier+variablesFoot;}
			else {int location =fPush(&fLiterals, params); return location*identifierNum+fIdentifier+variablesFoot;}
		} 
		//TODO Implement hashing
		else if(isAlpha(params[0])){
			//first make sure it starts with RubberRoom.
			const int wordlength=10; char temp[wordlength];
			for (size_t i = 0; i < wordlength; i++)
			{
				temp[i]=params[i];
			}
			int token= lexicalTokenAnalyser(temp);
			if(token!=RUBBERROOM||params[10]!='.'){throwSyntaxError("invalid params", params);return 0;}
			output[0]=RUBBERROOM;
			output[1]=DOT;

			//then check it has a valid rat and ends in .
			int i=11;
			int j=0;
			char temp2[MAXWORDLENGTH];
			while(params[i]!='.'){
				temp2[j]=params[i];
				j++;
				i++;
				if(i>length-1){throwSyntaxError("invalid params", params);return 0;}
			}
			i++;
			int token= lexicalTokenAnalyser(temp);
			if(token<variablesFoot||(token-variablesFoot)%identifierNum!=rIdentifier||token>activeRats.head){throwSyntaxError("invalid params", params);return 0;}
			output[2]=token;

			//check if it's do, take or show
			char temp3[10];
			j=0;
			while(params[i]!='('){
				if(i>length-1){throwSyntaxError("invalid params", params);return 0;}
				temp3[j]=params[i];
			}
			if (strcmp(temp3,"show")==0){

			} 
			else if (strcmp(temp3,"take")==0){

			}
			else if (strcmp(temp3,"do")==0){

			}
			else {throwSyntaxError("invalid params", params);return 0;}

	    	int location =rPush(&activeRats, params);
			return location*identifierNum+variablesFoot+rIdentifier;
		}
	}	
	else if(!single){

	}
}


//This one takes a token and replaced it with it's numeric value
int lexicalTokenAnalyser(char currentWord[]){

	//switch for efficiency, pretty sure I save more time with it, even with the extra if statment behind it
	switch(currentWord[0]){
		case 'C': if(strcmp(currentWord, "Crazy?")==0){return CRAZY;}break;
		case 'R': if(strcmp(currentWord, "RubberRoom")==0){return RUBBERROOM;}break;
		case 's': if(strcmp(currentWord, "speak")==0){return SPEAK;}
				else if(strcmp(currentWord, "show")==0){return SHOW;}break;
		case 'k': if(strcmp(currentWord, "kill")==0){return KILL;}break;
		case 'h': if(strcmp(currentWord, "hold")==0){return HOLD;}break;
		case 't': if(strcmp(currentWord, "take")==0){return TAKE;}break;
		case 'i': if(strcmp(currentWord, "instruct")==0){return INSTRUCT;}break;
		case 'd': if(strcmp(currentWord, "do")==0){return DO;}break;
		//TODO should hash the strings too, also needs some checks for valid strings
		case '\"': if(currentWord[strlen(currentWord)-1]=='\"'){int location =ccPush(&sLiterals, currentWord); return location*identifierNum+sIdentifier+variablesFoot;} 
			else{throwSyntaxError("string not terminated by \"", currentWord); return 0;}		
		case '\'': if(currentWord[strlen(currentWord)-1]=='\''&&strlen(currentWord)==3){int location =cPush(&cLiterals, currentWord); return location*identifierNum+cIdentifier+variablesFoot;} 
			else{throwSyntaxError("invalid character literal at", currentWord); return 0;}	

	}
	if(isDigit(currentWord[0])){
		int foundDot=0;
		for (size_t i = 0; i < strlen(currentWord); i++)
		{
			if(!isDigit(currentWord[i])){
				if(currentWord[i]=='.'&&!foundDot){foundDot=1;}
				else if(currentWord[i]=='.'&&foundDot){throwSyntaxError("multiple . in number ", currentWord);return 0;}
				else {throwSyntaxError("invalid character in number", currentWord); return 0;}
			}
		}
		if (!foundDot){int location =iPush(&iLiterals, currentWord); return location*identifierNum+iIdentifier+variablesFoot;}
		else {int location =fPush(&fLiterals, currentWord); return location*identifierNum+fIdentifier+variablesFoot;}
	} 
	//TODO Implement hashing
	else if(isAlpha(currentWord[0])){
	    rPush(&activeRats, currentWord);
		return activeRats.head*identifierNum+variablesFoot+rIdentifier;
	}
	throwSyntaxError("unrecognizable token", currentWord);
	return 0;
}


//good start, not complete
int lexicalAnalyser(struct textStack inputText, struct iStack *outputText){
	int state=0;
	int lineCount=0;
	int characterCount=0;
	int expectingDot=0;

	for (size_t i = 0; i < inputText.head+1; i++)
	{		
		characterCount++;
		if(inputText.array[i]=='.'){
			if(expectingDot){outputText->head++;outputText->array[outputText->head]=DOT;expectingDot=0;}
			else{throwLexicalError("invalid spot for a .", lineCount, characterCount);}		
			continue;
		}
		switch (state){

			//newline expected
			case SEMICOLON: 
				if(inputText.array[i]==';'){outputText->head++;outputText->array[outputText->head]=SEMICOLON; state=0; lineCount++; characterCount=0;}
				else {throwLexicalError("expecting a semicolon", lineCount, characterCount);}
				break;	

			//following newline	
			case 0:
				if(inputText.array[i]=='C'){
					const int wordlength=6; char temp[wordlength];
					for (size_t j = 0; j < wordlength; j++)
					{
						temp[j]=inputText.array[i];
						if(j<wordlength-1){i++; characterCount++;}
					}
					int token= lexicalTokenAnalyser(temp);
					if(token==CRAZY) {outputText->head++;outputText->array[outputText->head]=token;state=SEMICOLON;}							
					else {throwLexicalError("invalid token", lineCount, characterCount);}
					break;	
				}
				else if(inputText.array[i]=='R'){
					const int wordlength=10; char temp[wordlength];
					for (size_t j = 0; j < wordlength; j++)
					{
						temp[j]=inputText.array[i];
						if(j<wordlength-1){i++;characterCount++;}
					}
					int token= lexicalTokenAnalyser(temp);
					if(token==RUBBERROOM) {outputText->head++;outputText->array[outputText->head]=token;state=RUBBERROOM;expectingDot=1;}							
					else {throwLexicalError("invalid token", lineCount, characterCount);}
					break;	
				}
				else{throwLexicalError("didn't start line with crazy or rubber room", lineCount);}
				break; 

			//following RubberRoom
			case RUBBERROOM:
				char temp[MAXWORDLENGTH];
				int j=0;
				while (inputText.array[i]!='.'&&inputText.array[i]!='(')
				{
					temp[j]=inputText.array[i];
					j++;
					i++;
					characterCount++;
				}				
				
				int token=lexicalTokenAnalyser(temp);
				if(token==0){throwLexicalError("Invalid rat", lineCount, characterCount);}
				if(token==SPAWN){state=SPAWN;}else {/*maybe check if the rat (the token) exists*/state=99;expectingDot=1;}
				outputText->head++;
				outputText->array[outputText->head]=token;

				break;	

			//following Spawn
			case SPAWN:
				//making sure it starts witih (
				if(inputText.array[i]=='('){i++;characterCount++;outputText->head++;outputText->array[outputText->head]=OPENBRACKET;}
				else{throwLexicalError("invalid token: spawn must have parameters", lineCount, characterCount);}
				
				//extracting ratname (string literal) and replacing it with numerical value
				char temp[MAXWORDLENGTH];
				int j=0;
				int openBracketLocation=characterCount-1;
				while (inputText.array[i]!=')')
				{
					if(i>inputText.head){throwLexicalError("missing closing bracket",lineCount, characterCount);}
					temp[j]=inputText.array[i];
					j++;
					i++;
					characterCount++;
				}

				int token=lexicalTokenAnalyser(temp);
				if(token==0){throwLexicalError("invalid spawn parameter",lineCount, characterCount);}
				outputText->head++;
				outputText->array[outputText->head]=token;
				
				//making sure there are closing )
				if(inputText.array[i]==')'){i++;characterCount++;outputText->head++;outputText->array[outputText->head]=CLOSEBRACKET;}
				state=SEMICOLON;
				break;	

			//following a rat	
			case 99:
				char temp[MAXWORDLENGTH];
				int j=0;
				while (inputText.array[i]!='(')
				{
					temp[j]=inputText.array[i];
					j++;
					i++;
					characterCount++;
				}
				int token= lexicalTokenAnalyser(temp);
				state=token;	
				outputText->array[outputText->head]=token;		 	
				if(2<state<10){throwLexicalError("invalid token", lineCount, characterCount);}
				break;	

			//following speak	
			case SPEAK:
				//making sure it starts witih (
				if(inputText.array[i]=='('){i++;characterCount++;outputText->head++;outputText->array[outputText->head]=OPENBRACKET;}
				else{throwLexicalError("invalid token: speak must have brackets", lineCount, characterCount);}
				
				//extracting ratname (string literal) and replacing it with numerical value
				char temp[MAXWORDLENGTH];
				int j=0;
				int openBracketLocation=characterCount-1;
				while (inputText.array[i]!=')')
				{
					if(i>inputText.head){throwLexicalError("missing closing bracket",lineCount, characterCount);}
					temp[j]=inputText.array[i];
					j++;
					i++;
					characterCount++;
				}
				if(strlen(temp)!=1){throwLexicalError("speak should have empty params", lineCount, characterCount);}
				
				//making sure there are closing )
				if(inputText.array[i]==')'){i++;characterCount++;outputText->head++;outputText->array[outputText->head]=CLOSEBRACKET;}
				state=SEMICOLON;
				break;	

			//following kill
			case KILL:
				//making sure it starts witih (
				if(inputText.array[i]=='('){i++;characterCount++;outputText->head++;outputText->array[outputText->head]=OPENBRACKET;}
				else{throwLexicalError("invalid token: kill must have brackets", lineCount, characterCount);}
				
				//extracting ratname (string literal) and replacing it with numerical value
				char temp[MAXWORDLENGTH];
				int j=0;
				int openBracketLocation=characterCount-1;
				while (inputText.array[i]!=')')
				{
					if(i>inputText.head){throwLexicalError("missing closing bracket",lineCount, characterCount);}
					temp[j]=inputText.array[i];
					j++;
					i++;
					characterCount++;
				}
				if(strlen(temp)!=1){throwLexicalError("kill should have empty params", lineCount, characterCount);}
				
				//making sure there are closing )
				if(inputText.array[i]==')'){i++;characterCount++;outputText->head++;outputText->array[outputText->head]=CLOSEBRACKET;}
				state=SEMICOLON;
				break;

			//following hold
			case HOLD:
				//making sure it starts witih (
				if(inputText.array[i]=='('){i++;characterCount++;outputText->head++;outputText->array[outputText->head]=OPENBRACKET;}
				else{throwLexicalError("invalid token: spawn must have parameters", lineCount, characterCount);}
				
				//extracting ratname (string literal) and replacing it with numerical value
				char temp[MAXWORDLENGTH];
				int j=0;
				int openBracketLocation=characterCount-1;
				int depth=1;
				while (depth!=0)
				{
					if(i>inputText.head){throwLexicalError("missing closing bracket",lineCount, characterCount);}
					if(inputText.array[i]=='('){depth++;}
					else if(inputText.array[i]==')'){depth--;}
					temp[j]=inputText.array[i];
					j++;
					i++;characterCount++;
				}
				int token=paramsAnalyser(temp, 1);				
				outputText->head++;
				outputText->array[outputText->head]=token;			
				
				//making sure there are closing )
				if(inputText.array[i]==')'){i++;characterCount++;outputText->head++;outputText->array[outputText->head]=CLOSEBRACKET;}

				if(state==0){throwLexicalError("invalid token", lineCount, characterCount);}
				break;

			//following show
			case SHOW://expect ();
			break;

			//following take
			case TAKE://expect ();
			break;

			//following instruct
			case INSTRUCT://expect ();
			break;

			//following do
			case DO://expect ();
			break;

			case 32://expect ();
			break;

			case 33://expect ();
			break;

			case 34://expect ();
			break;

			case 35://expect ();
			break;

			case 64://expect ();
			break;

			case 65://expect ();
			break;
		}
		
	}
	
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

	//10 since everything is going to be hashed into ints, none of which will be longer than 10 digits
	int array[text.head];

	//this will be the next check
	struct iStack cleanText={.head=-1, .array=array};
	for (size_t i = 0; i < text.head+1; i++)
	{
		lexicalAnalyser(text, &cleanText) ;
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


