#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>

// Boundary definitions, set as required
#define MAXX 23
#define MAXY 23
#define MAXZ 23
char map[MAXX][MAXY][MAXZ];

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
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			printf("%c", map[y][x]);
		}
		printf("\n");
	}
}
// Full block character for maps █


// Read input file line by line (e.g., into an array)
int readInput() {
        FILE * input;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
	int count = 0;
	int x, y, z;

        input = fopen("input.txt", "r");
        if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate one-dimensional array of strings
	// char **inst=(char**)calloc(MAXX, sizeof(char*));
	// TPoint *inst=(TPoint*)calloc(MAXX, sizeof(TPoint));

	// Allocate a two-dimensional arrray of chars
	// int x=0, y=0;


	memset(map, 0, MAXX*MAXY*MAXZ*sizeof(char));

//        char ***map=calloc(MAXZ,sizeof(char**));
//	for(int iter=0; iter<MAXZ; iter++) {
//		map[iter]=(char**)calloc(MAXY,sizeof(char**));
//		for(int iiter=0; iter<MAXY; iter++) map[iter][iiter]=(char*)calloc(MAXX,sizeof(char));
//	}

        while ((read = getline(&line, &len, input)) != -1) {

		// Read into map
		// for(x=0; x<MAXX; x++) map[y][x] = line[x];
		// y++;

		// Copy to string
		//asprintf(&(inst[count]), "%s", line);	

		// Read into array
		sscanf(line,"%d,%d,%d",
			&x,
			&y,
			&z);

		map[x][y][z]=1;

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

	return 0;
}

int main(int argc, char *argv[]) {

	int x, y, z, sum = 0;
	readInput();
	int change=1;


	while(change) {
		change=0;
		for(x=0; x<MAXX; x++) {
			for(y=0; y<MAXY; y++) {
				for(z=0; z<MAXX; z++) {
				if(map[x][y][z]) continue;
				//left
				if((x==0)||(map[x-1][y][z]==2)) {
					map[x][y][z]=2;
					sum++;
					change=1; }
				//right
				if((x==MAXX-1)||(map[x+1][y][z]==2)) {
					map[x][y][z]=2;
					sum++;
					change=1; }
				//front
				if((y==0)||(map[x][y-1][z]==2)) {
					map[x][y][z]=2;
					sum++;
					change=1; }
				//back
				if((y==MAXY-1)||(map[x][y+1][z]==2)) {
					map[x][y][z]=2;
					sum++;
					change=1; }
				//top
				if((z==0)||(map[x][y][z-1]==2)) {
					map[x][y][z]=2;
					sum++;
					change=1; }
				//back
				if((z==MAXZ-1)||(map[x][y][z+1]==2)) {
					map[x][y][z]=2;
					sum++;
					change=1; }
					

				}
			}
		}
	}
	
	printf("Filed %d with water\n", sum);

	sum=0;
	for(x=0; x<MAXX; x++) {
		for(y=0; y<MAXY; y++) {
			for(z=0; z<MAXX; z++) {
				if(map[x][y][z]!=1) continue;
				//left
				if((x==0)||(map[x-1][y][z]==2)) sum++;
				//right
				if((x==MAXX-1)||(map[x+1][y][z]==2)) sum++;
				//front
				if((y==0)||(map[x][y-1][z]==2)) sum++;
				//back
				if((y==MAXY-1)||(map[x][y+1][z]==2)) sum++;
				//top
				if((z==0)||(map[x][y][z-1]==2)) sum++;
				//back
				if((z==MAXZ-1)||(map[x][y][z+1]==2)) sum++;
			}
		}
	}

	printf("%d\n", sum);

	return 0;
}
