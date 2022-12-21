#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary definitions, set as required
#define MAXX 76
#define MAXY 3000

char **ids;

int monkID(char *s) {
	int i;
	for(i=0; ids[i]; i++) {
		if(!strncmp(ids[i],s,4)) return i;
	}
	ids[i]=(char*)calloc(5,sizeof(char));
	strncpy(ids[i],s,4);
	return i;
}

// Point structure definition
typedef struct {
	int op[2];
	char o;
	long long int val;
	int known;
} TFormula;

int root;
int humn;

// Comparator function example
int comp(const void *a, const void *b)
{
  const int *da = (const int *) a;
  const int *db = (const int *) b;
  return (*da > *db) - (*da < *db);
}

// Example for calling qsort()
//qsort(array,count,sizeof(),comp);


// Print a two-dimensional array
void printMap (char **map) {
	int x,y;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			printf("%c", map[y][x]);
		}
		printf("\n");
	}
}
// Full block character for maps █


// Read input file line by line (e.g., into an array)
TFormula *readInput() {
//int readInput() {
        FILE * input;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
	int count = 0;
	char s[5];
	int id;

        input = fopen("input.txt", "r");
        if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate one-dimensional array of strings
	// char **inst=(char**)calloc(MAXX, sizeof(char*));


	TFormula *inst=(TFormula*)calloc(MAXY, sizeof(TFormula));
	ids=(char**)calloc(MAXY, sizeof(char*));
	


	// Allocate a two-dimensional arrray of chars
	// int x=0, y=0;
        // char **map=calloc(MAXY,sizeof(char*));
	// for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

        while ((read = getline(&line, &len, input)) != -1) {

		line[strlen(line)-1]=0;

		strncpy(s, line, 4); s[4]=0;
		id=monkID(s);
		if(line[6]<'a') {
			// Number
			inst[id].val=atoi(line+6);
			inst[id].known=1;
//			printf("%04d: %lld\n", id, inst[id].val);
		}
		else {
			// Formula
			inst[id].op[0]=monkID(line+6);
			inst[id].op[1]=monkID(line+13);
			inst[id].o=line[11];
//			printf("%04d: %04d %c %04d\n", id, inst[id].op[0], inst[id].o, inst[id].op[1]);
		}

		if(!strncmp(line,"root",4)) root=id;
		if(!strncmp(line,"humn",4)) humn=id;

		// Read into map
		// for(x=0; x<MAXX; x++) map[y][x] = line[x];
		// y++;

		// Copy to string
		//asprintf(&(inst[count]), "%s", line);	

		// Read into array
		// sscanf(line,"%d,%d",
		//	&(inst[count].x),
		//	&(inst[count].y));

		// Read tokens from single line
		//char *token;
		//token = strtok(line, ",");
		//while( 1 ) {
		//	if(!(token = strtok(NULL, ","))) break;
		//}

		count++;
	}

	fclose(input);
        if (line)
        free(line);

//	printMap(map);

	return inst;
//	return map;
}

long long int calculate(TFormula *array, int form) {

	long long int op[2];

	if(array[form].known) return array[form].val;

	op[0]=calculate(array, array[form].op[0]);
	op[1]=calculate(array, array[form].op[1]);

	switch(array[form].o) {
		case '+':
			return op[0]+op[1];
			break;
		case '-':
			return op[0]-op[1];
			break;
		case '/':
			return op[0]/op[1];
			break;
		case '*':
			return op[0]*op[1];
			break;
	}

	assert(0);

	return 0;
}

int main(int argc, char *argv[]) {

	TFormula *array;
//	int i=0;	
	array = readInput();

//	for(i=0; array[i]; i++) {
//		printf("%d\n", array[i]);
//	}

	long long int rop, lop;

	printf("Root ID: %04d\nHumn ID: %04d\n", root, humn);

	printf("Root value: %lld\n", calculate(array,root));
	printf("Root lop: %lld\n", calculate(array,array[root].op[0]));

	rop=calculate(array,array[root].op[1]);
	printf("Root rop: %lld\n", rop);

	for(long long int j=0; 1; j++) {
		array[humn].val=j;
		lop=calculate(array,array[root].op[0]);
//		if(!(j%100000)) {
//			printf("Root lop with hmn %15lld: %15lld \tdiff: %15lld\n", array[humn].val, lop, rop-lop);
//		}
		if(rop>=lop) {
			printf("Root lop with hmn %15lld: %15lld\n", array[humn].val, lop);
			break;
		}
	}


	return 0;
}
