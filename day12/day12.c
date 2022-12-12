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

// Read input file line by line (e.g., into an array)
char **readInput() {
        FILE * input;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
	int count = 0;

        input = fopen("input.txt", "r");
        if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate a two-dimensional arrray of chars
	int x=0, y=0;
        char **map=calloc(MAXY,sizeof(char*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

        while ((read = getline(&line, &len, input)) != -1) {

		// Read into map
		for(x=0; x<MAXX; x++) map[y][x] = line[x];
		y++;

		count++;
	}

	fclose(input);
        if (line)
        free(line);


//	return inst;
	return map;
}

int main(int argc, char *argv[]) {

	char **map;
	int x=0, y=0;
	int change=1;
        int **dist=calloc(MAXY,sizeof(int*));
	for(int iter=0; iter<MAXY; iter++) dist[iter]=calloc(MAXX,sizeof(int));
	int ny, nx;
	int steps=1;

	TPoint dir[] = {{-1,0}, {1,0}, {0,-1}, {0,1}};

	map = readInput();

	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			if(map[y][x]=='E') {
				map[y][x]='z';
				dist[y][x]=1;
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

						if((map[ny][nx]>=map[y][x])||(map[ny][nx]==map[y][x]-1)||(map[y][x]=='E')) {
							change++;
							dist[ny][nx]=dist[y][x]+1;
						}
					}

				}
			}
		}

	}	

	int min=INT_MAX;

	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			if((map[y][x]=='a')&&(dist[y][x])&&(dist[y][x]-1<min)) {
				min=dist[y][x]-1;
				printf("New best minimum at [%d,%d]: %d\n", x, y, min);
			} 
	}}



//	printMap(map, dist);

	return 0;
}
