#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>

// Boundary definitions, set as required
#define MAXX 10
#define MAXY 550

// Point structure definition
typedef struct {
	int count;
	int from;
	int to;
} TMove;

TMove *move;

char **map;

int *top;
int bottom=0;

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
void printMap () {
	int x,y;
	for(y=400; y<=bottom; y++) {
		printf("%3d ", y);
		for(x=0; x<MAXX; x++) {
			if(y==top[x]-1) printf(".");

			else if(map[y][x])
				printf("%c", map[y][x]);
			else printf(" ");
		}
		printf("\n");
	}
}
// Full block character for maps █


// Read input file line by line (e.g., into an array)
TMove *readInput() {
//int readInput() {
        FILE * input;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
	int count = 0;
	int i,iter;
	char r;

        input = fopen("input.txt", "r");
        if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate one-dimensional array of strings
	// char **inst=(char**)calloc(MAXX, sizeof(char*));
	TMove *inst=(TMove*)calloc(MAXY, sizeof(TMove));

	// Allocate a two-dimensional arrray of chars
	// int x=0, y=0;
        map=calloc(MAXY,sizeof(char*));
	for(iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));
	iter = MAXY-50;

	top=(int*)calloc(MAXY, sizeof(int));

        while ((read = getline(&line, &len, input)) != -1) {

		// Read into map
		// for(x=0; x<MAXX; x++) map[y][x] = line[x];
		// y++;

		// Copy to string
		//asprintf(&(inst[count]), "%s", line);	

		if(line[0]=='m') {
			// Read into array
			sscanf(line,"move %d from %d to %d",
				&(inst[count].count),
				&(inst[count].from),
				&(inst[count].to));
			inst[count].from--;
			inst[count].to--;
			count++;
		} else if(strlen(line)>1) {
			for(i=0; i*4+1<strlen(line); i++) {
				r=line[i*4+1];
				printf("%c", r);
				if((r>='A')&&(r<='Z')) {
					map[iter][i]=r;
					if(!top[i]) top[i]=iter;
					bottom=iter;
				}

			}
			iter ++;

			printf("\n");
		}


	}

	printf("bottom: %d\n", bottom);
	fclose(input);
        if (line)
        free(line);

//	printMap(map);

//	return 0;
	return inst;
//	return map;
}

char pop(int i) {
	char ret = map[top[i]][i];
	map[top[i]][i]=0;
	top[i]++;
	return ret;
}

void push(char c, int i) {
	top[i]--;
	map[top[i]][i]=c;
}

int main(int argc, char *argv[]) {

	int i=0;
	int y;
	int from, to;
	move  = readInput();


//	printMap();


	for(i=0; move[i].count; i+=1) {
		printf("%d %d %d\n", move[i].count, move[i].from, move[i].to);
		from=move[i].from;
		to=move[i].to;
		for(y=0; y<move[i].count; y++) {
			map[top[to]-y-1][to]=map[top[from]+move[i].count-1-y][from];
			map[top[from]+move[i].count-1-y][from]=0;
		}
		top[from]+=move[i].count;
		top[to]-=move[i].count;
	}

	printMap();

	for(i=0; i<MAXX; i++) printf("%c", map[top[i]][i]);
	printf("\n");

	return 0;
}
