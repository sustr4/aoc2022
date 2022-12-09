#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>

// Boundary definitions, set as required
#define MAXX 76
#define MAXY 26

// Point structure definition
typedef struct {
	int x;
	int y;
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
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			printf("%c", map[y][x]);
		}
		printf("\n");
	}
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
	char dir;
	int no;
	TPoint h,t;

        input = fopen("input.txt", "r");
        if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate one-dimensional array of strings
	// char **inst=(char**)calloc(MAXX, sizeof(char*));
	// TPoint *inst=(TPoint*)calloc(MAXX, sizeof(TPoint));

	// Allocate a two-dimensional arrray of chars
	// int x=0, y=0;
        // char **map=calloc(MAXY,sizeof(char*));
	// for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

	memset(&h,0,sizeof(TPoint));
	memset(&t,0,sizeof(TPoint));

		printf("[%d,%d]\n", t.x, t.y);
        while ((read = getline(&line, &len, input)) != -1) {

		// Read into map
		// for(x=0; x<MAXX; x++) map[y][x] = line[x];
		// y++;

		// Copy to string
		//asprintf(&(inst[count]), "%s", line);	

		// Read into array
		sscanf(line,"%c %d",
			&dir,
			&no);

//		printf("### %c %d\n", dir, no);

		for(int i = 0; i<no; i++) {
			switch(dir) {
				case 'U':
					h.y--;
					if(t.y>h.y+1) {
						t.y--;
						t.x=h.x;
					}
					break;
				case 'D':
					h.y++;
					if(t.y<h.y-1) {
						t.y++;
						t.x=h.x;
					}
					break;
				case 'L':
					h.x--;
					if(t.x>h.x+1) {
						t.x--;
						t.y=h.y;
					}
					break;
				case 'R':
					h.x++;
					if(t.x<h.x-1) {
						t.x++;
						t.y=h.y;
					}
					break;
			}
//			printf("[%d,%d] -- [%d,%d]\n", h.x, h.y, t.x, t.y);
			printf("[%d,%d]\n", t.x, t.y);
		
		}




		count++;
	}

	fclose(input);
        if (line)
        free(line);

//	printMap(map);

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
