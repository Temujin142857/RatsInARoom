//util functions

static inline uint32_t hash(void *key, int length) {
  uint8_t *bytes = (uint8_t *)key;
  uint32_t hash = 2166136261u;

  for (int i = 0; i < length; i++) {
    hash ^= bytes[i];
    hash *= 16777619;
  }
  return hash;
}

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
	int head;
	struct rat *array;	
	int capacity;
};


struct ccStack {
	int head;
	char **array;
	int capacity;
};

struct cStack{
	int head;
	char *array;
	int capacity;
};

struct fStack{
	int head;
	float *array;
	int capacity;
};

struct textStack {
	int head;
	char **array;
	int capacity;
};

struct iStack {
	int head;
	int *array;
	int capacity;
};

struct outputBufferStack{
	int head;
	char array[1024];
	int capacity;
};


int initCCStack(struct ccStack *stack, int c){
	char* arr[stack->capacity];
    for (char i= 0; i < stack->capacity; i++){
        arr[i] = (char*)malloc(c * sizeof(char));
	}
	stack->array=arr;
}


struct ratMap{
  char** entries; 
  int head;                             
  int capacity;                          
  size_t key_size;                       
  size_t value_size;    
};

void initMap(struct ratMap *map, size_t value_size) {
  map->head = -1;
  map->capacity = 0;
  map->entries = NULL;
  map->key_size = sizeof(uint32_t);
  map->value_size = value_size;
}


void freeMap(struct ratMap *map) {
  free(map->entries);
  map->head = 0;
  map->capacity = 0;
  map->entries = NULL;
  map->key_size = 0;
  map->value_size = 0;
} 

uint32_t ratMapAdd(struct ratMap *map, struct rat newElement){
	if(map->head+1>map->capacity){
		if (!resizeMap(map))return 0;
	}

	uint32_t index=hash(*newElement.name, strlen(newElement.name));

}

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

int cPush (struct cStack *stack, char newElement){	
	if(stack->head+1>=stack->capacity){
		size_t newCapacity=stack->capacity*2;
		char* temp=realloc(stack->array, newCapacity * sizeof *stack->array);
		if(temp==NULL){return 0;}
		stack->array=temp;
		stack->capacity=newCapacity;
	}
	stack->head++;
	stack->array[stack->head]=newElement;
	return 1;
}

int iPush (struct iStack *stack, int newElement){
	if(stack->head+1>=stack->capacity){
		size_t newCapacity=stack->capacity*2;
		int* temp=realloc(stack->array, newCapacity * sizeof *stack->array);
		if(temp==NULL){return 0;}
		stack->array=temp;
		stack->capacity=newCapacity;
	}
	stack->head++;
	stack->array[stack->head]=newElement;
	return 1;
}

int fPush (struct fStack *stack, float newElement){
	if(stack->head+1>=stack->capacity){
		size_t newCapacity=stack->capacity*2;
		float* temp=realloc(stack->array, newCapacity * sizeof *stack->array);
		if(temp==NULL){return 0;}
		stack->array=temp;
		stack->capacity=newCapacity;
	}
	stack->head++;
	stack->array[stack->head]=newElement;
	return 1;
}

int ccPush(struct ccStack *stack, char newElement[]){
	if(stack->head+1>=stack->capacity){
		size_t newCapacity=stack->capacity*2;
		char **temp = realloc( stack->array, sizeof *stack->array * newCapacity);
		if(temp==NULL){return 0;}
		stack->array=temp;
		stack->capacity=newCapacity;
	}
	stack->head++;
	stack->array[stack->head]=malloc(sizeof *newElement * strlen(newElement));
	strcpy(stack->array[stack->head], newElement);
	return 1;
}

int rPush(struct ratStack *stack, char newElement[]){
	//unsigned long hasValue=hash(newElement);
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