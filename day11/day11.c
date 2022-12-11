#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary definitions, set as required
#define MAXX 76
#define MAXY 26


typedef struct {
	int op;
	int val;
} TOperation;

// Point structure definition
typedef struct {
	int items[40];
	TOperation operation;
	int divby;
	int throw[2];
	int inspections;
} TMonkey;

// Comparator function example
int comp(const void *a, const void *b)
{
  const int *da = (const int *) a;
  const int *db = (const int *) b;
  return (*da < *db) - (*da > *db);
}

// Example for calling qsort()
//qsort(array,count,sizeof(),comp);


// Print a two-dimensional array
void printMonkey (TMonkey *monk) {
	int i;
	for(i=0; monk[i].operation.op; i++) {
		printf("Monkey %d\n", i);
		printf("  Starting items: ");
		for(int y=0; monk[i].items[y]; y++) printf("%d, ", monk[i].items[y]);
		printf("\n");
		printf("  Operation: new = %d %d\n",  monk[i].operation.op, monk[i].operation.val);
		printf("  Test: divisible by %d\n", monk[i].divby);
		printf("    If true: throw to monkey %d\n", monk[i].throw[0]);
		printf("    If false: throw to monkey %d\n", monk[i].throw[1]);
		printf("  // inspections performed %d\n", monk[i].inspections);
		printf("\n");
	}
}
// Full block character for maps █

int operate(TOperation op, int val) {
	switch(op.op) {
		case 1: // Add
			return val + op.val;
		case 2:
			return val * op.val;
		case 3:
			return val * val;

	}
	assert((op.op>0)&&(op.op<4));
	return 0;
}

void resort(TMonkey *monk) {
	int i;
	for(i=0; monk[i].operation.op; i++) {
		qsort(monk[i].items,40,sizeof(int),comp);

	}
}

void push(TMonkey *monk, int no, int val) {
	int i;
	for(i=0; monk[no].items[i]; i++);
	monk[no].items[i]=val;
}

int gameround(TMonkey *monk) {
	int m, i;
	int new;

	for(m=0; monk[m].operation.op; m++) { // Iterate through Monkeys
		printf("Monkey %d:\n", m);
		for(i=0; monk[m].items[i]; i++) {
			printf("  Monkey inspects an item with a worry level of %d.\n", monk[m].items[i]);
			new=operate(monk[m].operation, monk[m].items[i]);
			printf("    Worry level is %d'd by %d to %d.\n",
				monk[m].operation.op,
				monk[m].operation.val,
				new);
			new/=3;
			printf("    Monkey gets bored with item. Worry level is divided by 3 to %d.\n", new);
			if(!(new%monk[m].divby)) { // Divisible
				printf("    Current worry level is divisible by %d.\n", monk[m].divby);
				printf("    Item with worry level %d is thrown to monkey %d.\n", new, monk[m].throw[0]);
				monk[m].items[i]=0;
				push(monk, monk[m].throw[0], new);
			} 
			else { // Non-divisible
				printf("    Current worry level is not divisible by %d.\n", monk[m].divby);
				printf("    Item with worry level %d is thrown to monkey %d.\n", new, monk[m].throw[1]);
				monk[m].items[i]=0;
				push(monk, monk[m].throw[1], new);
			}
			monk[m].inspections++;
		}
	}
	resort(monk);

	return 0;
}

// Read input file line by line (e.g., into an array)
TMonkey *readInput() {
//int readInput() {
        FILE * input;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
	int count = 0;
	int monkey = 0;

        input = fopen("input.txt", "r");
        if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate one-dimensional array of strings
	// char **inst=(char**)calloc(MAXX, sizeof(char*));
	TMonkey *inst=(TMonkey*)calloc(MAXX, sizeof(TMonkey));

	// Allocate a two-dimensional arrray of chars
	// int x=0, y=0;
        // char **map=calloc(MAXY,sizeof(char*));
	// for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

        while ((read = getline(&line, &len, input)) != -1) {

		if(line[0]=='M') { // Monkey No.
			sscanf(line+7,"%d",&monkey);
		} else
		if(line[2]=='S') { // List of items
			char *token;
			int i=0;

			token = strtok(line+18, ", ");
			inst[monkey].items[i]=atoi(token);
			while( 1 ) {
				i++;
				token = strtok(NULL, ", ");
				if (!token) break;
				inst[monkey].items[i]=atoi(token);
			}
		} else
		if(line[2]=='O') { // Operation
			switch(line[23]) {
				case '+': // Plus
					inst[monkey].operation.op=1;
					inst[monkey].operation.val=atoi(line+25);
					break;
				case '*': // Times
					if(line[25]!='o') { // Times
						inst[monkey].operation.op=2;
						inst[monkey].operation.val=atoi(line+25);
					}
					else inst[monkey].operation.op=3;
					break;
			}
		} else
		if(line[2]=='T') { // Test
			inst[monkey].divby=atoi(line+21);
		} else
		if(!strncmp(line,"    If true:", 8)) {
			inst[monkey].throw[0]=atoi(line+29);
		} else
		if(!strncmp(line,"    If false:", 9)) {
			inst[monkey].throw[1]=atoi(line+30);
		}

		// Read into map
		// for(x=0; x<MAXX; x++) map[y][x] = line[x];
		// y++;

		// Copy to string
		//asprintf(&(inst[count]), "%s", line);	

		// Read into array
		// sscanf(line,"%d,%d",
		//	&(inst[count].x),
		//	&(inst[count].y));

		count++;
	}

	fclose(input);
        if (line)
        free(line);

//	printMap(map);

	return inst;
//	return map;
}

int main(int argc, char *argv[]) {

	int i;
	TMonkey *monk = readInput();

	for(i=0; i<20; i++) {
		gameround(monk);
	}

	printMonkey(monk);

	return 0;
}
