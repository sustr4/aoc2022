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
void printMap (char ***map, int ***dist, int r) {
	int x,y;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			if(dist[r][y][x]) printf("%d", dist[r][y][x]%10);
			else printf("%c", map[r][y][x]?map[r][y][x]:' ');
		}
		printf("\n");
	}
	printf("\n");
}
// Full block character for maps █


// Read input file line by line (e.g., into an array)
char ***readInput() {
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

//	for(int i=0; i<10; i++) {
//		printMap(map, NULL, i);
//	}

	return map;
}

int main(int argc, char *argv[]) {

	int i=0,r,y,x;	
	char ***map = readInput();
        int ***dist=calloc(MAXT,sizeof(int**));
	for(int iter=0; iter<MAXT; iter++) {
		dist[iter]=calloc(MAXY,sizeof(int*));
		for(int iiter=0; iiter<MAXY; iiter++) {
			dist[iter][iiter]=calloc(MAXX,sizeof(int));
		}
	}

	dist[0][0][1]=1;

	for(r=1; r<MAXT; r++) {
		for(y=0; y<MAXY; y++) {
			for(x=0; x<MAXX; x++) {
				if(map[r][y][x]==0) { // Free space, can move to it
					if((dist[r-1][y][x])||
					   ((x>0)&&(dist[r-1][y][x-1]))||
					   ((x<MAXY-1)&&(dist[r-1][y][x+1]))||
					   ((y>0)&&(dist[r-1][y-1][x]))||
					   ((y<MAXY-1)&&(dist[r-1][y+1][x]))) dist[r][y][x]=r;
				}
			}
		}

		printMap(map, dist, r);
	}

	for(r=0; r<MAXT; r++) if(dist[r][MAXY-1][MAXX-2]) break;
	printf("Distance: %d\n", dist[r][MAXY-1][MAXX-2]);

	


//	for(i=0; array[i]; i++) {
//		printf("%d\n", array[i]);
//	}

	return 0;
}
