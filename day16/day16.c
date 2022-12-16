#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>

// Boundary definitions, set as required
#define MAXX 2527
#define MAXY 26
int *stock;

int **steps;
int **score;

// Point structure definition
typedef struct {
	int flow;
	int open;
	int *next;
	char code[3];
} Tvalve;

// Comparator function example
int comp(const void *a, const void *b)
{
  const int *da = (const int *) a;
  const int *db = (const int *) b;
  return (*da > *db) - (*da < *db);
}

// Example for calling qsort()
//qsort(valve,count,sizeof(),comp);


// Print a two-dimensional valve
void printMap (char **map) {
	int x,y;
	for(y=0; y<MAXY; y++) {
		for(x=0; x<MAXX; x++) {
			printf("%c", map[y][x]);
		}
		printf("\n");
	}
}
// Full block character for maps █

int valveNo(char *s) {
	int i=1;
	int hash = (s[0]-'A')*100 + (s[1]-'A') + 1;

	while(stock[i]) {
		if(stock[i]==hash) return i;
		i++;
	}
	stock[i]=hash;
	return i;
}

// Read input file line by line (e.g., into an valve)
Tvalve *readInput() {
//int readInput() {
        FILE * input;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
	char des[100];
	int j;
	char *paths;

        input = fopen("input.txt", "r");
        if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate one-dimensional valve of strings
	// char **inst=(char**)calloc(MAXX, sizeof(char*));
	Tvalve *inst=(Tvalve*)calloc(MAXX, sizeof(Tvalve));
	for(int iter=0; iter<MAXX; iter++) inst[iter].next=calloc(10,sizeof(int));

	// Allocate a two-dimensional arrray of chars
	// int x=0, y=0;
        // char **map=calloc(MAXY,sizeof(char*));
	// for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

        while ((read = getline(&line, &len, input)) != -1) {
		sscanf(line,"Valve %s has flow rate=%d; tunnels lead to valves",
			des,
			&j);
		if(j>0) valveNo(des);
	}

	fclose(input);

        input = fopen("input.txt", "r");
        if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

        while ((read = getline(&line, &len, input)) != -1) {

		// Read into map
		// for(x=0; x<MAXX; x++) map[y][x] = line[x];
		// y++;

		// Copy to string
		//asprintf(&(inst[count]), "%s", line);	


		sscanf(line,"Valve %s has flow rate=%d; tunnels lead to valves",
			des,
			&j);

		inst[valveNo(des)].flow=j;
		strncpy(inst[valveNo(des)].code, des, 2);
		
		// Read tokens from single line
		char *token;
		j=0;
		paths = strstr(line, "valve");
		if(paths[5]=='s') paths+=7;
		else paths+=6;
		token = strtok(paths, ", ");
		
		inst[valveNo(des)].next[j]=valveNo(token);
		while( 1 ) {
			j++;
			if(!(token = strtok(NULL, ", "))) break;
			inst[valveNo(des)].next[j]=valveNo(token);
//			printf("%s %d\n", token, valveNo(token));
		}

	}

	fclose(input);
        if (line)
        free(line);

//	printMap(map);

	return inst;
//	return map;
}

int setBit(int stat, int no) {
	return(stat | (1<<(no-1)));
}

int checkBit(int stat, int no) {
	return(stat & (1<<(no-1)));
}

int capacity(Tvalve *valve, int stat) {
	int i;
	int sum=0;

	for(i = 0; valve[i+1].flow; i++) {
		if(stat & (1<<i)) sum+= valve[i+1].flow;
	}
	return sum;
}

void printSit(int stat, int pos, char *code) {
	int i;
        for(i=0; i<15; i++) {
               printf("%3d", i);
        }
        printf("\n");
        for(i=0; i<15; i++) {
                printf("%3d", (stat&1<<i)>>i);
        }

        printf("\tLoc: %2d (%s)\n", pos, code);
}


int main(int argc, char *argv[]) {

	stock = (int*)calloc(MAXX, sizeof(int));
	Tvalve *valve;
	int i=0;
	int pos, state;	
	valve = readInput();
	int step;
	int newstate;
	int newscore;

	int valves = 0;
	int positions = 0;

	for(i=0; i<MAXX; i++) {
		if(valve[i].next[0]) {
			printf("%4d (%s) %d\t", i, valve[i].code, valve[i].flow);
			for(int j=0; valve[i].next[j]; j++) printf("%d (%s), ", valve[i].next[j], valve[valve[i].next[j]].code);
			printf("\n");
		}
	}

	for(i=0; i<MAXX; i++) {
		if(valve[i].flow) valves++;
		if(valve[i].next[0]) positions++;
	}

	printf("Valves: %d\n", valves);
	printf("Positions: %d\n", positions);

	steps=(int**)calloc(positions+1, sizeof(int*));
	for(i=0; i<=positions; i++) steps[i]=(int*)calloc(1<<valves,sizeof(int));

	score=(int**)calloc(positions+1, sizeof(int*));
	for(i=0; i<=positions; i++) score[i]=(int*)calloc(1<<valves,sizeof(int));

	for(pos=0; pos<positions; pos++)
		if(!strncmp(valve[pos].code,"AA",2)) steps[pos][0]=1;

	for(step=1; step<=30; step++) {
		printf("## Starting step %d\n", step);
		for(pos=0; pos<positions; pos++) {
			for(state=0; state<(1<<valves); state++) {
				if(steps[pos][state]==step) {
					printf("Looking at position %d (%s), state %d\n", pos, valve[pos].code, state);

					printSit(state, pos, valve[pos].code);

					if(valve[pos].flow&&!checkBit(state,pos)) {
						// Open valve
						newstate=setBit(state, pos);

						if((!steps[pos][newstate])||
						   (score[pos][state]>score[pos][newstate])) {
							steps[pos][newstate]=step+1;
							newscore=score[pos][state]+capacity(valve, state);
							score[pos][newstate]=newscore;
							printf("    New score of %d, pos %d (%s), stat %d\n", newscore, pos, valve[pos].code, newstate);
						}
					}

					for(i=0; valve[pos].next[i]; i++) {
						printf("  Looking and neighbor %d (%s)\n", valve[pos].next[i], valve[valve[pos].next[i]].code);
						if((!steps[valve[pos].next[i]][state])||
						   (score[pos][state]>score[pos][valve[pos].next[i]])) {
							steps[valve[pos].next[i]][state]=step+1;
							newscore=score[pos][state]+capacity(valve, state);
							score[valve[pos].next[i]][state]=newscore;
						}
					}

				}
			}
		}
	}

//	printf("Valve No. for AA: %d\nValve No. for AA: %d\n", valveNo("AA"), valveNo("ZZ"));
	int max=0;
	for(pos=0; pos<positions; pos++) {
		for(state=0; state<(1<<valves); state++) {
			if(steps[pos][state]==31) {
				if(score[pos][state]>0) { 
					if(max<score[pos][state]) {
						max=score[pos][state];
						printf("%d/%d\n", score[pos][state], max);
					}
				}
			}
		}
	}


	return 0;
}
