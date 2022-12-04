#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>

// Boundary definitions, set as required
#define MAXX 1100
#define MAXY 26

// Point structure definition
typedef struct {
	int w;
	int x;
	int y;
	int z;
} TPair;

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
TPair *readInput() {
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
	TPair *inst=(TPair*)calloc(MAXX, sizeof(TPair));

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

		sscanf(line,"%d-%d,%d-%d",
			&(inst[count].w),
			&(inst[count].x),
			&(inst[count].y),
			&(inst[count].z));

		count++;
	}

	fclose(input);
        if (line)
        free(line);

//	printMap(map);

	return inst;
//	return map;
}

int main(int argc, char *argv[]) {

	int i;
	TPair *pair = readInput();


	for(i=0; pair[i].w; i++) {

		if((pair[i].w>=pair[i].y)&&(pair[i].x<=pair[i].z)) {
			printf(", %d %d %d %d\n", pair[i].w, pair[i].x, pair[i].y, pair[i].z);
			continue; }
		if((pair[i].w<=pair[i].y)&&(pair[i].x>=pair[i].z))
			printf(". %d %d %d %d\n", pair[i].w, pair[i].x, pair[i].y, pair[i].z);


	}

	return 0;
}
