#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>

// Boundary definitions, set as required
#define MAXX 600 // 457--517
#define MAXY 170 // 13--161

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
void printMap (char **map) {
	int x,y;
	for(y=0; y<163; y++) {
		for(x=456; x<519; x++) {
			switch(map[y][x]) {
				case 0:
					printf(" ");
					break;
				case 1:
					printf("█");
					break;
				case 2:
					printf("o");
					break;
			}
		}
		printf("\n");
	}
}
// Full block character for maps █

int drawLine(char** map, int fx, int fy, int tx, int ty) {
	int i;
	int a,b;
	
	if(fx==tx) {
		if(fy<ty) { a=fy; b=ty; }
		else { a=ty; b=fy; }
		for(i=a; i<=b; i++) map[i][fx]=1;
	}
	else {
		if(fx<tx) { a=fx; b=tx; }
		else { a=tx; b=fx; }
		for(i=a; i<=b; i++) map[fy][i]=1;
	}

	return 0;
}

int drop(char **map) {
	int x=500;
	int y=0;

	while(1) {
		if(map[y+1][x]==0) { // down
			y++;
		} else if(map[y+1][x-1]==0) {
			y++;
			x--;
		} else if(map[y+1][x+1]==0) {
			y++;
			x++;
		} else {
			map[y][x]=2;
			return 1;
		}
		if(y>=MAXY-2) return 0;
	}
}

// Read input file line by line (e.g., into an array)
char **readInput() {
//int readInput() {
        FILE * input;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
	int count = 0;
	char *token;
	int fy, ty, fx, tx;

        input = fopen("input.txt", "r");
        if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate one-dimensional array of strings
	// char **inst=(char**)calloc(MAXX, sizeof(char*));
	// TPoint *inst=(TPoint*)calloc(MAXX, sizeof(TPoint));

	// Allocate a two-dimensional arrray of chars
	// int x=0, y=0;
        char **map=calloc(MAXY,sizeof(char*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

        while ((read = getline(&line, &len, input)) != -1) {

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

		token = strtok(line, " -> ");
		printf("%s\n", token);
		sscanf(token, "%d,%d", &fx, &fy);
		while( 1 ) {
			if(!(token = strtok(NULL, " -> "))) break;
			sscanf(token, "%d,%d", &tx, &ty);
			drawLine(map, fx, fy, tx, ty);
			fx=tx; fy=ty;
		}


		count++;
	}

	fclose(input);
        if (line)
        free(line);


	return map;
}

int main(int argc, char *argv[]) {


//	TPoint array;
//	int i=0;	
//	array = readInput();
	char **map = readInput();
	int count = 0;

	while(drop(map)) count++;
	printMap(map);

	printf("Count: %d\n", count);

	return 0;
}
