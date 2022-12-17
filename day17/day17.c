#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>

// Boundary definitions, set as required
#define MAXX 10
#define MAXY 10000

char **map;

char shape[][4][4] = {
	{{ 0, 0, 0, 0 },
	 { 0, 0, 0, 0 },
	 { 0, 0, 0, 0 },
	 { 1, 1, 1, 1 }},
	{{ 0, 0, 0, 0 },
	 { 0, 1, 0, 0 },
	 { 1, 1, 1, 0 },
	 { 0, 1, 0, 0 }},
	{{ 0, 0, 0, 0 },
	 { 0, 0, 1, 0 },
	 { 0, 0, 1, 0 },
	 { 1, 1, 1, 0 }},
	{{ 1, 0, 0, 0 },
	 { 1, 0, 0, 0 },
	 { 1, 0, 0, 0 },
	 { 1, 0, 0, 0 }},
	{{ 0, 0, 0, 0 },
	 { 0, 0, 0, 0 },
	 { 1, 1, 0, 0 },
	 { 1, 1, 0, 0 }}};

// Point structure definition
typedef struct {
	int x;
	int y;
	int z;
} TPoint;

char *blow = NULL;

// Comparator function example
int comp(const void *a, const void *b)
{
  const int *da = (const int *) a;
  const int *db = (const int *) b;
  return (*da > *db) - (*da < *db);
}

// Example for calling qsort()
//qsort(array,count,sizeof(),comp);

int findBottom(char **map) {
	int x,y;
	
	for(y=0; y<=MAXY; y++) {
		for(x=1; x<8; x++) if(map[y][x]) return y;
	}

	return MAXY;
}

int placeShape(char **map, int sh, int x, int y) {
	int px, py;
	
	for(py=3; py>=0; py--) {
		for(px=0; px<=3; px++) {
			if(shape[sh][py][px]) {
				map[y-3+py][x+px]=2+sh;
			}
		}
	}

	return 1;
}
int checkShape(char **map, int sh, int x, int y) {
	int px, py;

	for(py=3; py>=0; py--) {
		for(px=0; px<=3; px++) {
			if(shape[sh][py][px]) {
				if(map[y-3+py][x+px]) return 0;
			}
		}
	}

	return 1;
}

// Print a two-dimensional array
void printMap (char **map) {
	int x,y,fb;
	
	fb = findBottom(map);
	for(y=fb-3; y<=MAXY; y++) {
		printf("%d\t", y);
		for(x=0; x<MAXX; x++) {
			switch(map[y][x]) {
				case 0:	printf(" "); break;
				case 1:	printf("█"); break;
				case 2:	printf("@"); break;
				case 3:	printf("H"); break;
				case 4:	printf("X"); break;
				case 5:	printf("#"); break;
				case 6:	printf("O"); break;
				default: printf("X");
				
			}
		}
		printf("\n");
	}
	printf("The bottom is at %d, stack is %d deep\n\n", fb, MAXY-fb);
}
// Full block character for maps █


// Read input file line by line (e.g., into an array)
void readInput() {
        FILE * input;
        size_t len = 0;

        map=(char**)calloc(MAXY+1,sizeof(char*));
        for(int iter=0; iter<=MAXY; iter++) {
		map[iter]=calloc(MAXX,sizeof(char));
		map[iter][0]=1;
		map[iter][8]=1;
	}
	for(int iter=1; iter<8; iter++) map[MAXY][iter]=1;


        input = fopen("input.txt", "r");
        if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

        getline(&blow, &len, input);

	blow[strlen(blow)-1]=0;

	fclose(input);
}

int main(int argc, char *argv[]) {

	int i;
	readInput();
	int sh=0;
	int x, y, cx;
	int gust = 0;

	for(i=0; i<2022; i++) {
		// Starting position for every new stone
		x=3;
		y=findBottom(map)-4;

		while(1) {
			// Blow
			if(!blow[gust]) gust=0;
			if(blow[gust++]=='>') cx=1; else cx=-1;
			if(checkShape(map, sh, x+cx, y)) x=x+cx;

			//Drop
			if(checkShape(map, sh, x, y+1)) y++;
			else {
				placeShape(map, sh, x, y);
				break;
			}

		}

		sh = ( sh + 1 ) % 5;
	}
	printMap(map);
//	for(i=0; i<strlen(blow); i++) {
//		printf("%c\n", blow[i]);
//	}

	return 0;
}
