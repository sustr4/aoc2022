#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>

// Boundary definitions, set as required
#define MAXX 10000
#define MAXY 10000

// Point structure definition
typedef struct {
	char *name;
} TDir;

typedef struct {
	int x;
	int y;
	int z;
} TFile;

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
//TPoint *readInput() {
int readInput() {
        FILE * input;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
	int count = 0;
	int dircount = 0;
	int depth = -1;
	char *name=NULL;
	int size;

        input = fopen("input.txt", "r");
        if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate one-dimensional array of strings
	// char **inst=(char**)calloc(MAXX, sizeof(char*));
	// char **inst=(char**)calloc(MAXX, sizeof(char*));
	// TPoint *inst=(TPoint*)calloc(MAXX, sizeof(TPoint));
	TDir *dir=(TDir*)calloc(MAXX, sizeof(TDir));

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
		// sscanf(line,"%d,%d",
		//	&(inst[count].x),
		//	&(inst[count].y));

//		printf("ORIG %s", line);
		if(line[0]=='$') {
			if(line[2]=='l') continue;
			if(line[5]=='.') {
				free(dir[depth].name);
				dir[depth].name=NULL;
				depth--;
			}
			else {
				depth++;
				dir[depth].name=strdup(line+5);
				dir[depth].name[strlen(dir[depth].name)-1]=0;
//				printf("cding %d %s\n", depth, dir[depth].name);
				dircount++;
			}


		}
		else {
			if(line[0]=='d') continue;
			name=(char*)calloc(strlen(line),sizeof(char));
			sscanf(line,"%d %s", &size, name);
			for(int y=0; y<=depth; y++) {
				for(int i=0; i<=depth-y; i++)
					printf("/%s", dir[i].name);
				printf("\t%d\n", size);
			}
			free(name);
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
