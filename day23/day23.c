#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary definitions, set as required
#define MAXX 2500
#define MAXY 26

// Point structure definition
typedef struct {
	int x;
	int y;
	char order[5];
} TElf;

char firstCons[] = {'N', 'S', 'W', 'E', 0};
char globalOrder[] = {'N', 'S', 'W', 'E', 0};

// Comparator function example
int comp(const void *a, const void *b)
{
  const int *da = (const int *) a;
  const int *db = (const int *) b;
  return (*da > *db) - (*da < *db);
}


// Example for calling qsort()
//qsort(elf,count,sizeof(),comp);


// Print a two-dimensional elf
void printMap (TElf *elf) {
	int i;
	int x,y;
	int minx = elf[0].x;
	int maxx = elf[0].x;
	int miny = elf[0].y;
	int maxy = elf[0].y;
	char draw;
	
	for(i=0; elf[i].order[0]; i++) {
		if(minx>elf[i].x) minx=elf[i].x;
		if(maxx<elf[i].x) maxx=elf[i].x;
		if(miny>elf[i].y) miny=elf[i].y;
		if(maxy<elf[i].y) maxy=elf[i].y;
	}

	for(y=miny-1; y<=maxy+1; y++) {
		printf("%3d ", y%100);
		for(x=minx-1; x<=maxx+1; x++) {
			draw='.';
			for(i=0; elf[i].order[0]; i++)
				if((elf[i].x==x)&&(elf[i].y==y)) {
//					draw='#';
					draw=i%10+'0';
					break;
				}
			
			printf("%c", draw);
		}
		printf("\n");
	}
}
// Full block character for maps █


// Read input file line by line (e.g., into an elf)
TElf *readInput() {
        FILE * input;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
	int count = 0;
	int eno = 0;

        input = fopen("input.txt", "r");
        if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	TElf *inst=(TElf*)calloc(MAXX, sizeof(TElf));

	// Allocate a two-dimensional arrray of chars
	// int x=0, y=0;
        // char **map=calloc(MAXY,sizeof(char*));
	// for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

        while ((read = getline(&line, &len, input)) != -1) {

		for(int i = 0; i<strlen(line); i++) {
			if(line[i]=='#') {
				inst[eno].x=i;
				inst[eno].y=count;
				memcpy(inst[eno].order,firstCons,5);
				eno++;
			}
		}
		count++;


	}

	fclose(input);
        if (line)
        free(line);

//	printMap(map);

	return inst;
}

int checkNB(TElf *elf, int i) {
	int j;
	for(j=0; elf[j].order[0]; j++) {
		if(i==j) continue;
		if((elf[i].x>=elf[j].x-1)&&(elf[i].x<=elf[j].x+1)&&
		   (elf[i].y>=elf[j].y-1)&&(elf[i].y<=elf[j].y+1)) return 1;
	}

	return 0;
}

int check(TElf *elf, int i, int dir) {
	int j;
//	switch(elf[i].order[dir]) {
	switch(globalOrder[dir]) {
		case 'S':
			for(j=0; elf[j].order[0]; j++) {
				if(i==j) continue;
				if((elf[i].y+1==elf[j].y)&&
				   (elf[i].x>=elf[j].x-1)&&(elf[i].x<=elf[j].x+1)) return 0;
			}
			break;
		case 'N':
			for(j=0; elf[j].order[0]; j++) { 
				if(i==j) continue;
				if((elf[i].y-1==elf[j].y)&&
				   (elf[i].x>=elf[j].x-1)&&(elf[i].x<=elf[j].x+1)) return 0;
			}
			break;
		case 'W':
			for(j=0; elf[j].order[0]; j++) { 
				if(i==j) continue;
				if((elf[i].x-1==elf[j].x)&&
				   (elf[i].y>=elf[j].y-1)&&(elf[i].y<=elf[j].y+1)) return 0;
			}
			break;
		case 'E':
			for(j=0; elf[j].order[0]; j++) { 
				if(i==j) continue;
				if((elf[i].x+1==elf[j].x)&&
				   (elf[i].y>=elf[j].y-1)&&(elf[i].y<=elf[j].y+1)) return 0;
			}
			break;
		default:
			assert(0);
	}
	return 1;
}

void move(TElf *ret, TElf *elf, int i, int d) {

	ret[i]=elf[i];
//	switch(elf[i].order[d]) {
	switch(globalOrder[d]) {
		case 'S': ret[i].y++; break;
		case 'N': ret[i].y--; break;
		case 'W': ret[i].x--; break;
		case 'E': ret[i].x++; break;
		default: assert(0);
	}
/*	char stor=ret[i].order[d];
//	printf("%d: %d %c %s -> ", i, d, stor, ret[i].order);
	for(int j=d; j<3; j++) ret[i].order[j]=ret[i].order[j+1];
	ret[i].order[3]=stor;
//	printf("%s\n", ret[i].order);
	*/
}

int square(TElf *elf) {
	int i;
	int minx = elf[0].x;
	int maxx = elf[0].x;
	int miny = elf[0].y;
	int maxy = elf[0].y;
	int count = 0;
	
	for(i=0; elf[i].order[0]; i++) {
		if(minx>elf[i].x) minx=elf[i].x;
		if(maxx<elf[i].x) maxx=elf[i].x;
		if(miny>elf[i].y) miny=elf[i].y;
		if(maxy<elf[i].y) maxy=elf[i].y;
		count++;
	}

	return (maxx-minx+1)*(maxy-miny+1)-count;
}

TElf *step(TElf *elf, int *somemove) {
	TElf *ret = (TElf*)calloc(MAXX, sizeof(TElf));
	int i, j, d;
	int moved;
	int *conflict = (int*)calloc(MAXX, sizeof(int));

//	printf("Global Order: %s\n", globalOrder);

	// propose moves	
	for(i=0; elf[i].order[0]; i++) {

		// Check for free directions
		moved=0;
		if(checkNB(elf, i)) {
			for(d=0; d<4; d++) {
				if(check(elf, i, d)) {
					move(ret, elf, i, d);
					moved=1;
					break;
				}
			}
		}
		if(!moved) ret[i]=elf[i];
	}

	//  identify conflicts
	for(i=0; elf[i+1].order[0]; i++) {
		for(j=i+1; elf[j].order[0]; j++) {
			if((ret[i].x==ret[j].x)&&(ret[i].y==ret[j].y)) {
				conflict[i]=1;
				conflict[j]=1;
			}
		}
	}

	for(i=0; elf[i].order[0]; i++) {
//		printf("%4d: [%d,%d], %s - > ", i, elf[i].x, elf[i].y, elf[i].order);
//		printf("[%d,%d], %s - > ", ret[i].x, ret[i].y, ret[i].order);
		if(conflict[i]) ret[i]=elf[i];
//		printf("[%d,%d], %s\n", ret[i].x, ret[i].y, ret[i].order);
	}

	moved=0;
	for(i=0; elf[i].order[0]; i++) {
		if((elf[i].x!=ret[i].x)||(elf[i].y!=ret[i].y)) {moved = 1; break;}
	}
	if(!moved) *somemove=0;
	else *somemove=1;

	// Shift global order
	char stor = globalOrder[0];
	for(i=0; i<3; i++) globalOrder[i]=globalOrder[i+1];
	globalOrder[3] = stor;

	free(elf);
	
	return ret;
}

int main(int argc, char *argv[]) {

	TElf *elf;
	int i=0;	
	int moved=0;	
	elf = readInput();
	int round;

	for(i=0; elf[i].order[0]; i++) {
//		printf("[%d.%d], %s\n", elf[i].x, elf[i].y, elf[i].order);
	}

	for(round=0; 1; round++) {
		elf=step(elf, &moved);
		if(!moved) {
			printMap(elf);
			printf("=== Round %d === \n", round+1);
			printf("Square: %d\n", square(elf));
			break;
		}
	}

	return 0;
}
