#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>

// Boundary definitions, set as required
#define MAXX 2527
#define MAXY 26

#define TIME 30

int *stock;

int **steps;
int **score;
int **dist;

int max=0;

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
void printMap (int **map, int size) {
	int x,y;
	for(y=1; y<=size; y++) {
		for(x=1; x<=size; x++) {
			printf("%2d", map[y][x]);
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

int dostep(Tvalve *valve, int tgt, int time, int score) {
	int i;
	int dura;
	int some = 0;

	if(time>=TIME) {
		if(score>max) {
			max=score;
			printf("New maximum %d/%d\n", score, max);
		}
		return 0;
	}

	for(i=1; valve[i].flow; i++) {
		if(i==tgt) continue;
		if(valve[i].open) continue;

		some=1;
		dura=dist[tgt][i]+1;
		valve[i].open=1;
		dostep(valve, i, time+dura, score + (TIME-dura-time) * valve[i].flow);
		valve[i].open=0;

	}

	if(!some) {
		if(score>max) {
			max=score;
			printf("No more moves. New maximum %d/%d\n", score, max);
		}
	}
	return 0;
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





int main(int argc, char *argv[]) {

	stock = (int*)calloc(MAXX, sizeof(int));
	Tvalve *valve;
	int i=0,j;
	valve = readInput();
	int step;

	int valves = 0;
	int positions = 0;

	int start = 0;

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

	dist=(int**)calloc(positions+1, sizeof(int*));
	for(i=0; i<=positions; i++) dist[i]=(int*)calloc(positions+1, sizeof(int));

	for(i=0; i<=positions; i++) {
		for(j=0; j<=positions; j++) {
			dist[i][j]=-1;
		}
	}

	for(i=1; i<=positions; i++) {
		dist[i][i]=0;
		for(step=0; step<positions; step++) {
			for(j=1; j<=positions; j++) {
				if(dist[i][j]==step) {
					for(int k=0; valve[j].next[k]; k++)
						if(dist[i][valve[j].next[k]]<0) dist[i][valve[j].next[k]]=step+1;
				}
			}
		}
		
	}

	printMap(dist, positions);

	for(i=1; i<positions; i++)
               if(!strncmp(valve[i].code,"AA",2)) start=i;

	printf("Starting at position %d (%s)\n", start, valve[start].code);

	dostep(valve, start, 0, 0);

	return 0;
}
