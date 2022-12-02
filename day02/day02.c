#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>

// Boundary definitions, set as required
#define MAXX 2600
#define MAXY 26

// Point structure definition
typedef struct {
	char x;
	char y;
} TMatch;

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
TMatch *readInput() {
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
	TMatch *inst=(TMatch*)calloc(MAXX, sizeof(TMatch));

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

		sscanf(line, "%c %c", &(inst[count].x), &(inst[count].y));

		count++;
	}

	fclose(input);
        if (line)
        free(line);

//	printMap(map);

	return inst;
//	return map;
}

int score(char x, char y) {

	switch(x) {
	case 'A': // Rock
		switch(y) {
		case 'X': return 3 + 1; // Rock
		case 'Y': return 6 + 2; // Paper
		case 'Z': return 0 + 3; // Scissors
		}
	case 'B': // Paper
		switch(y) {
		case 'X': return 0 + 1; // Rock
		case 'Y': return 3 + 2; // Paper
		case 'Z': return 6 + 3; // Scissors
		}
	case 'C': // Scissors
		switch(y) {
		case 'X': return 6 + 1; // Rock
		case 'Y': return 0 + 2; // Paper
		case 'Z': return 3 + 3; // Scissors
		}
	}

	return INT_MAX;
}


int score2(char x, char y) {

	switch(x) {
	case 'A': // Rock
		switch(y) {
		case 'Y': return 3 + 1; // Rock
		case 'Z': return 6 + 2; // Paper
		case 'X': return 0 + 3; // Scissors
		}
	case 'B': // Paper
		switch(y) {
		case 'X': return 0 + 1; // Rock
		case 'Y': return 3 + 2; // Paper
		case 'Z': return 6 + 3; // Scissors
		}
	case 'C': // Scissors
		switch(y) {
		case 'Z': return 6 + 1; // Rock
		case 'X': return 0 + 2; // Paper
		case 'Y': return 3 + 3; // Scissors
		}
	}

	return INT_MAX;
}
int main(int argc, char *argv[]) {

	int sum=0,s,s2,sum2=0;
	int i;
	TMatch *match = readInput();

	for (i=0; match[i].x; i++) {
		s=score(match[i].x, match[i].y);
		s2=score2(match[i].x, match[i].y);
		printf("%c %c\t%d\t%d\n", match[i].x, match[i].y, s, s2);
		sum+=s;
		sum2+=s2;

	}


	printf("%d\n%d\n", sum, sum2);


	return 0;
}
