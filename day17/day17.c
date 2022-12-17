#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>

// Boundary definitions, set as required
#define MAXX 10
#define MAXY 100000
#define STARTY 10000000000000

char **map;

char height[] = {1, 3, 3, 4, 2};

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

/*int findBottom(char **map) {
	int x,y;
	
	for(y=0; y<=MAXY; y++) {
		for(x=1; x<8; x++) if(map[y][x]) return y;
	}

	return MAXY;
}*/

long long int placeShape(char **map, int sh, int x, long long int y) {
	int px, py;
	long long int ret = y;
	
	for(py=3; py>=0; py--) {
		for(px=0; px<=3; px++) {
			if(shape[sh][(py+MAXY)%MAXY][px]) {
				map[(y-3+py+MAXY)%MAXY][x+px]=2+sh;
				ret=(y-3+py);
			}
		}
	}

	return ret;
}
int checkShape(char **map, int sh, int x, long long int y) {
	int px, py;

//	printf("  Checking shape %d at coord %d,%d\n", sh, x, y);

	for(py=3; py>=0; py--) {
		for(px=0; px<=3; px++) {
			if(shape[sh][py][px]) {
				if(map[(y-3+py+MAXY)%MAXY][x+px]) return 0;
			}
		}
	}

	return 1;
}

// Print a two-dimensional array
void printMap (char **map, long long int sy) {
	long long int x,y,fb;
	
	fb = sy-3;
	for(y=fb-3; y<=sy+10; y++) {
		printf("%lld\t(%lld)\t", y, (y+MAXY)%MAXY);
		for(x=0; x<MAXX; x++) {
			switch(map[(y+MAXY)%MAXY][x]) {
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
	printf("\n");
}
// Full block character for maps █


// Read input file line by line (e.g., into an array)
void readInput() {
        FILE * input;
        size_t len = 0;

        map=(char**)calloc(MAXY,sizeof(char*));
        for(int iter=0; iter<MAXY; iter++) {
		map[iter]=calloc(MAXX,sizeof(char));
		map[iter][0]=1;
		map[iter][8]=1;
	}
	for(int iter=1; iter<8; iter++) map[MAXY-1][iter]=1;


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
	int x, cx;
	long long int y;
	long long int prevy;
	int gust = 0;
	long long int ny;
	long long int py;
	long long int head, multiples, points;

	// After certain point, there are 2647 new levels for every 1730 bricks

	multiples=1000000000000/1730-4;
	head=1000000000000%1730+4*1730;
	points=multiples*2647;

	y=STARTY-4;
	prevy=STARTY;
//	for(i=0; i<1000000000000; i++) {
	for(i=0; i<head; i++) {
		// Starting position for every new stone
		x=3;
//		printf("Starting %dth shape %d at %d,%d\n", i, sh, x, y);

		//cleanup around new shape
		for(py=y-3; py<y+3; py++) {
			for(cx=1; cx<8; cx++) {
				map[py%MAXY][cx]=0;
			}
		}

		while(1) {
			// Blow
			if(!blow[gust]) gust=0;
//			printf(" Trying gust %d (%c) x=%d depth=%d\n", gust, blow[gust], x, y);
			if(blow[gust++]=='>') cx=1; else cx=-1;
			if(checkShape(map, sh, x+cx, y)) x=x+cx;

			//Drop
//			printf(" Trying to drop to depth=%d\n", (y+1+MAXY)%MAXY);
			if(checkShape(map, sh, x, y+1)) y=(y+1);
			else {
				ny=placeShape(map, sh, x, y);
//				printf("%d. at ny=%lld, prevy=%lld, y on entry=%lld", i, ny, prevy, y);
				if(ny<prevy) {
					prevy=ny;
				}
				y=prevy-4;
//				printf("%lld\n", prevy);
//				printf(" (-> %lld), calc. y: %lld, sum %lld\n", prevy, y, STARTY-prevy);
				break;
			}

		}

//		printMap(map,y);
		sh = ( sh + 1 ) % 5;
	}
	printMap(map,y);
//	for(i=0; i<strlen(blow); i++) {
//		printf("%c\n", blow[i]);
//	}

	printf("Total sum: %lld + %lld = %lld (head was %lld)\n", STARTY-prevy, points, STARTY-prevy + points, head);

	return 0;
}
