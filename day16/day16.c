#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>

// Boundary definitions, set as required
#define MAXX 2527
#define MAXY 26

// Point structure definition
typedef struct {
	int flow;
	int open;
	int *next;
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
	return (s[0]-'A')*100 + (s[1]-'A');
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

		// Read into map
		// for(x=0; x<MAXX; x++) map[y][x] = line[x];
		// y++;

		// Copy to string
		//asprintf(&(inst[count]), "%s", line);	


		sscanf(line,"Valve %s has flow rate=%d; tunnels lead to valves",
			des,
			&j);

		inst[valveNo(des)].flow=j;
		
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
			printf("%s %d\n", token, valveNo(token));
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

	Tvalve *valve;
	int i=0;	
	valve = readInput();

	for(i=0; i<MAXX; i++) {
		if(valve[i].next[0]) {
			printf("%4d %d\t", i, valve[i].flow);
			for(int j=0; valve[i].next[j]; j++) printf("%d, ", valve[i].next[j]);
			printf("\n");
		}
	}

//	printf("Valve No. for AA: %d\nValve No. for AA: %d\n", valveNo("AA"), valveNo("ZZ"));

	return 0;
}
