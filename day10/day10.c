#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>

// Boundary definitions, set as required
#define MAXX 200
#define MAXY 26

// Point structure definition
typedef struct {
	int ins;
	int val;
} TCode;

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
TCode *readInput() {
//int readInput() {
        FILE * input;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
	int count = 0;

        input = fopen("input.txt", "r");
        if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate one-dimensional array of strings
	// char **inst=(char**)calloc(MAXX, sizeof(char*));
	TCode *inst=(TCode*)calloc(MAXX, sizeof(TCode));

	// Allocate a two-dimensional arrray of chars
	// int x=0, y=0;
        // char **map=calloc(MAXY,sizeof(char*));
	// for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

        while ((read = getline(&line, &len, input)) != -1) {

		// Read into map
		// for(x=0; x<MAXX; x++) map[y][x] = line[x];
		// y++;

		// Copy to string
		//asprintf(&(inst[count]), "%s", line);	

		// Read into array
		switch(line[0]) {
			case 'n':
				inst[count].ins=1;
				break;
			case 'a':
				inst[count].ins=2;
				sscanf(line+5,"%d",
					&(inst[count].val));
				break;
		}

		

		count++;
	}

	fclose(input);
        if (line)
        free(line);

//	printMap(map);

	return inst;
//	return map;
}

int run(TCode *code) {
	int i;
	int cycle=0;
	int x=1;
	int sum=0;

	for(i=0; code[i].ins; i++) {
		switch(code[i].ins) {
			case 1: //noop
				cycle++;
				if(!((cycle+20)%40)) {
					sum+=x*cycle;
					printf("%d %d %d %d\n", cycle, i, x, sum);
				}
				break;
			case 2: //addx
				cycle++;
				if(!((cycle+20)%40)) {
					sum+=x*cycle;
					printf("%d %d %d %d\n", cycle, i, x, sum);
				}
				x+=code[i].val;
				cycle++;
				if(!((cycle+20)%40)) {
					sum+=x*cycle;
					printf("%d %d %d %d\n", cycle, i, x, sum);
				}
				break;
		}
	}
	return sum;
}

int main(int argc, char *argv[]) {

	TCode *code;
	code = readInput();

	run(code);

	return 0;
}
