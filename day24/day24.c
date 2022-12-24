#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary definitions, set as required
#define MAXX 122
#define MAXY 27
#define MAXT 900

// Point structure definition
typedef struct {
	int x;
	int y;
	int z;
} TPoint;

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
void printMap (char ***map, int r) {
	int x,y;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			printf("%c", map[r][y][x]?map[r][y][x]:' ');
		}
		printf("\n");
	}
	printf("\n");
}
// Full block character for maps █


// Read input file line by line (e.g., into an array)
TPoint *readInput() {
//int readInput() {
        FILE * input;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
	int count = 0;
	int r, x, y;
	int nx, ny;
	int maxy=0;


        input = fopen("input.txt", "r");
        if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate one-dimensional array of strings
	// char **inst=(char**)calloc(MAXX, sizeof(char*));
	// TPoint *inst=(TPoint*)calloc(MAXX, sizeof(TPoint));

	// Allocate a two-dimensional arrray of chars
        char ***map=calloc(MAXT,sizeof(char**));
	for(int iter=0; iter<MAXT; iter++) {
		map[iter]=calloc(MAXY,sizeof(char*));
		for(int iiter=0; iiter<MAXY; iiter++) {
			map[iter][iiter]=calloc(MAXX,sizeof(char));
		}
	}

        while ((read = getline(&line, &len, input)) != -1) {

		line[strlen(line)-1]=0;
		// Read into map
		strcpy(map[0][count], line);

		if(strlen(line)>maxy) maxy=strlen(line);
		count++;
	}


	for(y=0; y<count; y++) {
		for(x=0; map[0][y][x]; x++) {
			nx=x; ny=y;
			switch(map[0][y][x]) {
				case '#':
					for(r=1; r<MAXT; r++) map[r][y][x]=map[r-1][y][x];
					break;
				case '.':
					map[0][y][x]=0;
					break;
				case '<':
					for(r=1; r<MAXT; r++) {
						nx--;
						if(nx==0) nx=maxy-2;
						map[r][ny][nx]='<'; }
					break;
				case '>': 
					for(r=1; r<MAXT; r++) {
						nx++;
						if(nx>=maxy-1) nx=1;
						map[r][ny][nx]='>'; }
					break;
				case '^': 
					for(r=1; r<MAXT; r++) {
						ny--;
						if(ny==0) ny=count-2;
						map[r][ny][nx]='^'; }
					break;
				case 'v': 
					for(r=1; r<MAXT; r++) {
						ny++;
						if(ny>=count-1) ny=1;
						map[r][ny][nx]='v'; }
					break;
				default: assert(0);
			}
		}
	}


	fclose(input);
        if (line)
        free(line);

	for(int i=0; i<10; i++) {
		printMap(map, i);
	}

	return 0;
//	return inst;
//	return map;
}

int main(int argc, char *argv[]) {

//	TPoint array;
//	int i=0;	
//	array = readInput();
	readInput();

//	for(i=0; array[i]; i++) {
//		printf("%d\n", array[i]);
//	}

	return 0;
}
