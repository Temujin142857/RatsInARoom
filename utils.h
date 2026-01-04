//util functions

#define ratSize 64

uint32_t hash(void *key, int length) {
  uint8_t *bytes = (uint8_t *)key;
  uint32_t hash = 2166136261u;

  for (int i = 0; i < length; i++) {
    hash ^= bytes[i];
    hash *= 16777619;
  }
  return hash;
}

struct rat {
	char* name; //capacity 32 letters, size 32 bytes
	uint8_t valueType; //size 1 byte
	//0 for no valueType
	//1 for int
	//2 for string

	uint8_t functionType; //size 1 byte
	uint8_t *functionParams; //capacity 30 params, size 30 bytes
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


struct rMap{
  struct rat *array; 
  int count;                             
  int capacity;                          
  size_t key_size;                       
  size_t value_size;    
};

void initrMap(struct rMap *map) {
  map->count = 0;
  map->capacity = 128;
  map->array = malloc(128 * ratSize);
  map->key_size = sizeof(uint32_t);
  map->value_size = ratSize;
}


void freeMap(struct rMap *map) {
  free(map->array);
  map->count = 0;
  map->capacity = 0;
  map->array = NULL;
  map->value_size = 0;
} 


int rResizeMap(struct rMap *map){
	uint32_t newCapacity=map->capacity*2;

	uint8_t *oldArray = (uint8_t *)map->array;
  	uint8_t *newArray = malloc(newCapacity * map->value_size);

  	// Zero out new buffer
  	memset(newArray, 0, newCapacity * map->value_size);
  	map->count = 0;

	for (int i = 0; i < map->capacity; i++) {
		if(map->array[i].name==NULL){continue;}
		uint32_t index =hash(map->array[i].name, strlen(map->array[i].name))%newCapacity;
		index=rLinearProbe(newArray, map->array[i].name, index, newCapacity);
		uint8_t *dest = newArray + index * map->value_size;
		uint8_t *src = (uint8_t *)&map->array[i];

		memcpy(dest, src, map->key_size);
		map->count++;
  	}

  free(map->array);
  map->array = newArray;
  map->capacity = newCapacity;

}

uint32_t rLinearProbe(struct rat *array, char newElement[], uint32_t index, int capacity){	
	if(array[index].name==NULL||strcmp(array[index].name, newElement)==0||strcmp(array[index].name, "tombstone")==0){return index;}
	uint32_t origionalIndex=index;
	index=(index+1)%capacity;
	while(index!=origionalIndex){
		if(strcmp(array[index].name, newElement)==0||strcmp(array[index].name, "tombstone")==0){return index;}
		index=(index+1)%capacity;
	}
	printf("error finding availible spot in hashmap");
	return -1;
}

uint32_t rMapAdd(struct rMap *map, struct rat newElement){
	if(map->count+1>map->capacity){
		if (!rResizeMap(&map))return 0;
	}	
	uint32_t index=hash(*newElement.name, strlen(newElement.name));
	index=sLinearProbe(map->array, newElement.name, index, map->capacity);
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