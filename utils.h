//util functions

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
	//3 for char
	//4 for float
	//note you cannot put a rat in a rat

};

struct ratStack {
	struct rat *array;
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

int ccPush(struct ccStack *stack, char newElement[]){
	stack->head++;
	strcpy(stack->array[stack->head],newElement);
	return 1;	
}

unsigned int rPush(struct rStack *stack, char newElement[]){
	unsigned long hasValue=hash(newElement);
}

int textPush(struct textStack *stack, char newElement[]){
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

// end of util functions