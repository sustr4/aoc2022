#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>

// Boundary definitions, set as required
#define MAXX 40
#define MAXY 6

// Point structure definition
typedef struct {
	int ins;
	int val;
} TCode;

// Print a two-dimensional array
void printMap (char map[MAXY][MAXX]) {
	int x,y;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			switch(map[y][x]) {
				case 0:
					printf(" "); break;
				case 1:
					printf("."); break;
				case 2:
					printf("█");
			}
		}
		printf("\n");
	}
	printf("\n");
}


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

	TCode *inst=(TCode*)calloc(200, sizeof(TCode));

        while ((read = getline(&line, &len, input)) != -1) {

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

	return inst;
}

char draw(int i, int x) {

	int hor=i%40;
	int spri=x%40;

	if(spri>hor+1) return 1;
	if(spri<hor-1) return 1;

	return 2;
}

int run(TCode *code) {
	int i;
	int cycle=0;
	int x=1;
	int sum=0;
	char map[MAXY][MAXX];

	memset(map,0,MAXY*MAXX*sizeof(char));

	for(i=0; code[i].ins; i++) {
		switch(code[i].ins) {
			case 1: //noop
				map[cycle/40][cycle%40]=draw(cycle, x);
				cycle++;
				break;
			case 2: //addx
				map[cycle/40][cycle%40]=draw(cycle, x);
				cycle++;
				map[cycle/40][cycle%40]=draw(cycle, x);
				x+=code[i].val;
				cycle++;
				break;
		}
	}

	printMap(map);
	return sum;
}

int main(int argc, char *argv[]) {

	TCode *code;
	code = readInput();

	run(code);

	return 0;
}
