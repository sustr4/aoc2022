#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>

// Boundary definitions, set as required
#define MAXX 163
#define MAXY 41

//#define MAXX 8
//#define MAXY 5
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
void printMap (char **map, int **dist) {
	int x,y;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			printf("%c%0d ", map[y][x], dist[y][x]);
		}
		printf("\n");
	}
}
// Full block character for maps █


// Read input file line by line (e.g., into an array)
char **readInput() {
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
	// TPoint *inst=(TPoint*)calloc(MAXX, sizeof(TPoint));

	// Allocate a two-dimensional arrray of chars
	int x=0, y=0;
        char **map=calloc(MAXY,sizeof(char*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

        while ((read = getline(&line, &len, input)) != -1) {

		// Read into map
		for(x=0; x<MAXX; x++) map[y][x] = line[x];
		y++;

		// Copy to string
		//asprintf(&(inst[count]), "%s", line);	

		// Read into array
		// sscanf(line,"%d,%d",
		//	&(inst[count].x),
		//	&(inst[count].y));

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


//	return inst;
	return map;
}

int main(int argc, char *argv[]) {

//	TPoint array;
//	int i=0;	
//	array = readInput();

	char **map;
	int x=0, y=0;
	int change=1;
        int **dist=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) dist[iter]=calloc(MAXX,sizeof(int));
	int ny, nx;
	int steps=1;
	int ey,ex;

	TPoint dir[] = {{-1,0}, {1,0}, {0,-1}, {0,1}};

	map = readInput();

	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			if(map[y][x]=='S') dist[y][x]=1; 
			if(map[y][x]=='E') {
				ey=y; ex=x;
				map[y][x]='z'; 
			}
	}}
			
	while(change) {
		change=0;
		steps++;
		for(y=0; y<MAXY; y++) {
			for(x=0; x<MAXX; x++) {
				if(dist[y][x]) { // Down

					for(int j=0; j<4; j++) {
						ny=y+dir[j].y;
						nx=x+dir[j].x;
						if(nx<0) continue;
						if(ny<0) continue;
						if(nx>=MAXX) continue;
						if(ny>=MAXY) continue;
						if(dist[ny][nx]) continue;

						if((map[ny][nx]<=map[y][x]+1)||(map[y][x]=='S')) {
							change++;
							dist[ny][nx]=dist[y][x]+1;
						}
					}

				}
			}
		}

		printf("Round %d: %d changes\n", steps, change);

	}	


//	for(i=0; array[i]; i++) {
//		printf("%d\n", array[i]);
//	}

		printMap(map, dist);

		printf("Dist to E [%d,%d] is %d\n", ex, ey, dist[ey][ex]-1);
	return 0;
}
