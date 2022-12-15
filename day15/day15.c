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
  const TIntersect *da = (const TIntersect *) a;
  const TIntersect *db = (const TIntersect *) b;
  if((!da->from)&&(!da->to)) {
	if((!db->from)&&(!db->to)) return 0;
	return 1;
  }
  if((!db->from)&&(!db->to)) {
	return -1;
  }

  int mr = (da->from > db->from) - (da->from < db->from);
  if(mr) return mr;
  else return  (da->to > db->to) - (da->to < db->to);
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

TIntersect *intersect(TSensor *array, int y) {
	int i;
	TIntersect *is;

	is=(TIntersect*)malloc(count*sizeof(TIntersect));

	for(i=0; i<count; i++) {
		if((y>=array[i].y-array[i].rad)&&(y<=array[i].y+array[i].rad)) {
			is[i].from=array[i].x-(array[i].rad-abs(array[i].y-y));
			is[i].to=array[i].x+(array[i].rad-abs(array[i].y-y));
		}
		else memset(&(is[i]), 0, sizeof(TIntersect));
	}

	qsort(is,count,sizeof(TIntersect),comp);

	return is;
}

int main(int argc, char *argv[]) {

	TSensor *array;
	int i=0, y, j, k, found, candi;	
	array = readInput();
	TIntersect *is;

	for(i=0; i<count; i++) {
		printf("%d,", array[i].x);
		printf("%d,", array[i].y);
		printf("%d,", array[i].bx);
		printf("%d,", array[i].by);
		printf("%d\n", array[i].rad);


	}

	for(y=0; y<=4000000; y++) {
		is=intersect(array, y);
		
/*		if(y==20) {

			for(i=0; (is[i].from)&&(is[i].to); i++) {
				printf("%3d -- %3d \t(%3d, %3d -> %2d)\n", is[i].from, is[i].to,
					array[i].x,
					array[i].y,
					array[i].rad);
			}
		}*/

		for(i=0; (is[i].from)&&(is[i].to); i++) {
			for(j=i+1; (is[j].from)&&(is[j].to); j++) {
				if(is[i].to==is[j].from-2) {
					candi=is[i].to+1;
					found=1;
					for(k=0; (is[k].from)&&(is[k].to); k++) {
						if((candi>=is[k].from)&&(candi<=is[k].to)) {
							found=0;
							break;
						}
					}
					if(found) printf("Candidate: %d, %d \t %ld \t(<%d,%d> <%d,%d>)\n", is[i].to+1, y, (long)4000000*((long)is[i].to+1) + (long)y, is[i].from, is[i].to, is[j].from, is[j].to);
				}
			}
		}


		free(is);
	}


	return 0;
}
