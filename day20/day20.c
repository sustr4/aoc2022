#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>

// Boundary definitions, set as required
#define MAXX 5000
#define MAXY 26

int count = 0;
// Point structure definition
typedef struct {
	long long int val;
	void *prev;
	void *next;
} TElement;

typedef struct {
	long long int val;
	TElement *elem;
} TPoint;

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


// Read input file line by line (e.g., into an array)
TPoint *readInput() {
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

	TPoint *inst=(TPoint*)calloc(MAXX, sizeof(TPoint));

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
		sscanf(line,"%lld",
			&(inst[count].val));

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

int main(int argc, char *argv[]) {

//	TPoint array;
	int i=0, j, k;	
	TPoint *array = readInput();
	TElement *current;
	TElement *zero = NULL;

	for(i=0; i<count; i++) {
//		printf("%d\n", array[i].val);
		array[i].elem=(TElement*)calloc(1,sizeof(TElement));
		array[i].val*=811589153;
		array[i].elem->val=array[i].val;
		if(array[i].val==0) zero=array[i].elem;
	}

	array[0].elem->prev=array[count-1].elem;
	for(i=1; i<count; i++) {
		array[i].elem->prev=array[i-1].elem;
	}
	for(i=0; i<count; i++) {
		array[i].elem->next=array[(i+1)%count].elem;
	}

//	printf("    -- "); 
//	current=zero;
//	for(j=0; j<count; j++) {
//		printf("%d, ", current->val);
//		current=current->next;
//	}
//	printf("\n");


	TElement *pr, *nxt;


	for(int b=0; b<10;b++) {
	printf("b = %d\n", b);
	for(i=0; i<count; i++) {

		if(array[i].val==0) continue;
//		printf("%3d -- ", array[i].val); 
		current=array[i].elem;
		// take out
		pr=array[i].elem->prev;
		nxt=array[i].elem->next;

		pr->next=nxt;
		nxt->prev=pr;

		// move

		if(array[i].val>0) {
			current=nxt;
			for(k=0; k<array[i].val%(count-1); k++) {
				current=current->next;
			}
			nxt=current;
			pr=nxt->prev;

		}
		else if(array[i].val<0) {
			current=pr;
			for(k=0; k>array[i].val%(count-1); k--) {
				current=current->prev;
			}
			pr=current;
			nxt=pr->next;
		}

		// insert
		array[i].elem->next=nxt;
		array[i].elem->prev=pr;
		pr->next=array[i].elem;
		nxt->prev=array[i].elem;

//		current=zero;
//		for(j=0; j<count; j++) {
//			printf("%d, ", current->val);
//			current=current->next;
//		}
//		printf("\n");
	}
	}

	long long int sum=0;

	current=zero;

	for(j=0; j<3; j++) {
		for(k=0; k<1000; k++) {
			current=current->next;
		}
		sum+=current->val;
		printf("%lld\n", current->val);
	}	

	printf("Sum: %lld\n", sum);

	return 0;
}
