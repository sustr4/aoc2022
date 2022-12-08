#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>

// Boundary definitions, set as required
#define MAXX 99
#define MAXY 99

// Print a two-dimensional array
void printMap (char **map) {
	int x,y;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			printf("%d", map[y][x]);
		}
		printf("\n");
	}
}

// Read input file line by line (e.g., into an array)
char **readInput() {
//int readInput() {
        FILE * input;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;

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
		for(x=0; x<strlen(line); x++) {
			if(line[x]>='0') map[y][x] = line[x]-'0';
		}
		y++;

	}

	fclose(input);
        if (line)
        free(line);

//	printMap(map);

	return map;
}

int main(int argc, char *argv[]) {

	char **map = readInput();
	int max;

	int sum = 2* MAXX + 2* MAXY - 4;
	int c,d;
	int i, left, right, top, down;

        char **seen=calloc(MAXY,sizeof(char*));
	for(int i=0; i<MAXY; i++) seen[i]=calloc(MAXX,sizeof(char));

	for(c=0; c<MAXX; c++) {
		seen[0][c]=1;
		seen[c][0]=1;
		seen[MAXY-1][c]=1;
		seen[c][MAXX-1]=1;
	}


	//left->right
	for(c=0; c<MAXX; c++) {
		max=map[c][0];
		for(d=1; d<MAXY; d++)
			if(map[c][d]>max) {
				seen[c][d]=1;
				max=map[c][d];
			}
	}
	//right>left
	for(c=0; c<MAXX; c++) {
		max=map[c][MAXY-1];
		for(d=MAXY-2; d>0; d--)
			if(map[c][d]>max) {
				seen[c][d]=1;
				max=map[c][d];
			}
	}
	//bottom->up
	for(c=0; c<MAXY; c++) {
		max=map[MAXX-1][c];
		for(d=MAXX-2; d>0; d--)
			if(map[d][c]>max) {
				seen[d][c]=1;
				max=map[d][c];
			}
	}
	//top->down
	for(c=0; c<MAXY; c++) {
		max=map[0][c];
		for(d=1; d<MAXY; d++)
			if(map[d][c]>max) {
				seen[d][c]=1;
				max=map[d][c];
			}
	}

	sum=0;
	for(c=0; c<MAXY; c++)
		for(d=0; d<MAXY; d++)
			if(seen[c][d])sum++;

	printf("%d\n", sum);

	sum=0;
	for(int y=0; y<MAXY; y++) {
		for(int x=0; x<MAXY; x++) {

			if(x==0) left=0;
			else for(left=1; x-left>0; left++)
				if(map[y][x-left]>=map[y][x]) break;

			if(x==MAXX-1) right=0;
			else for(right=1; x+right<MAXX-1; right++)
				if(map[y][x+right]>=map[y][x]) break;

			if(y==MAXY-1) down=0;
			else for(down=1; y+down<MAXY-1; down++)
				if(map[y+down][x]>=map[y][x]) break;

			if(y==0) top=0;
			else for(top=1; y-top>0; top++)
				if(map[y-top][x]>=map[y][x]) break;

			i=left*right*top*down;

			if(i>sum) sum=i;

//			printf("[%2d,%2d] l: %d r: %d d: %d t: %d \t%d/%d\n", x, y, left, right, down, top, i, sum);
		}
	}
	
	printf("%d\n", sum);

	return 0;
}
