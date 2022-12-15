#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>

// Boundary definitions, set as required
#define MAXX 40
#define MAXY 26

int miny=0;
int maxy=0;
int minx=0;
int maxx=0;
int maxrad=0;
int count = 0;
// Point structure definition
typedef struct {
	int x;
	int y;
	int bx;
	int by;
	int rad;
} TSensor;

typedef struct {
	int from;
	int to;
} TIntersect;

// Comparator function example
int comp(const void *a, const void *b)
{
  const int *da = (const int *) a;
  const int *db = (const int *) b;
  return (*da > *db) - (*da < *db);
}

// Example for calling qsort()
//qsort(array,count,sizeof(),comp);




int dist(int x1, int y1, int x2, int y2) {
	return abs(x1-x2) + abs(y1-y2);
}

// Read input file line by line (e.g., into an array)
TSensor *readInput() {
//int readInput() {
        FILE * input;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;

        input = fopen("input.txt", "r");
        if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate one-dimensional array of strings
	// char **inst=(char**)calloc(MAXX, sizeof(char*));
	TSensor *inst=(TSensor*)calloc(MAXX, sizeof(TSensor));

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
		sscanf(line,"Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d",
			&(inst[count].x),
			&(inst[count].y),
			&(inst[count].bx),
			&(inst[count].by));
		inst[count].rad=dist(inst[count].x, inst[count].y, inst[count].bx, inst[count].by);

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

	return inst;
//	return map;
}

int isFree(TSensor *array, int x, int y) {
	int i;
	for(i=0; i<count; i++) {
		if((array[i].x==x)&&(array[i].y==y)) return 0;
		if((array[i].bx==x)&&(array[i].by==y)) return 0;
	}
	return 1;
}

int printDot(TSensor *array, int x, int y, char c) {
	int i;
	for(i=0; i<count; i++) {
		if((array[i].x==x)&&(array[i].y==y)) {
			printf("S");
			return 2;
		}
		if((array[i].bx==x)&&(array[i].by==y)) {
			printf("B");
			return 3;
		}
	}
	printf("%c", c);

	return 0;
}

int main(int argc, char *argv[]) {

	TSensor *array;
	int i=0, x, y, cnt=0, cov;	
	array = readInput();

	for(i=0; i<count; i++) {
		printf("%d,", array[i].x);
		printf("%d,", array[i].y);
		printf("%d,", array[i].bx);
		printf("%d,", array[i].by);
		printf("%d\n", array[i].rad);
		if(array[i].x<minx) minx=array[i].x;
//		if(array[i].bx<minx) minx=array[i].bx;
		if(array[i].x>maxx) maxx=array[i].x;
//		if(array[i].bx>maxx) maxx=array[i].bx;
		if(array[i].rad>maxrad) maxrad=array[i].rad;
		if(array[i].y<miny) miny=array[i].y;
//		if(array[i].by<miny) miny=array[i].by;
		if(array[i].y>maxy) maxy=array[i].y;
//		if(array[i].by>maxy) maxy=array[i].by;
	}

//	printf("    ");
//	for(x=minx-maxrad; x<=maxx+maxrad; x+=5) printf("%5d", x+5);
//	printf("\n");
//	printf("    ");
//	for(x=minx-maxrad; x<=maxx+maxrad; x+=5) printf("    |");
//	printf("\n");

	printf("Limits:\n X: <%d,%d>\n Y: <%d,%d>\n", minx, maxx, miny, maxy);

	y=2000000;
//	for(y=miny-maxrad; y<=maxy+maxrad; y++) {
//		printf("%3d ",y);
	for(x=minx-maxrad; x<=maxx+maxrad; x++) {
		cov=0;
		for(i=0; i<count; i++) {
//			if(dist(x,2000000,array[i].x,array[i].y)<=array[i].rad) cov++;
			if(dist(x,y,array[i].x,array[i].y)<=array[i].rad) {
				cov++;
			}
		}
		if((y==2000000)&&cov&&isFree(array,x,y)) {
			cnt++;
		}
//		if(cov) printDot(array, x, y, '#');
//		else printDot(array, x, y, '.');
	}
//	printf("\n");
//	}
	printf("\nCount: %d\nMax rad: %d\nMin X: %d\n", cnt, maxrad, minx);

	return 0;
}
