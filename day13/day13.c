#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary definitions, set as required
#define MAXX 450
#define MAXY 50

// Point structure definition
typedef struct {
	int type; // 0: null; 1: int; 2: list
	int val;
	void *right;
	void *up;
} TList;

TList *list;
int cnt = 0;

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

int compItem(TList *a, TList *b) {

	int ret;

	printf("A %d:%d, B %d:%d\n", a->type, a->val, b->type, b->val);

	if(a==NULL) return -1;
	if(b==NULL) return 1;

	if((a->type==0)&&(b->type>0)) return -1;
	if((a->type>0)&&(b->type==0)) return 1;

	if((a->type==0)&&(b->type==0)) return 0;
	if((a->type==1)&&(b->type==1)) {
		if(a->val < b->val) return -1;
		if(a->val > b->val) return 1;
	}

	if((a->type==2)&&(b->type==2)) {
		ret = compItem(a->up, b->up);
		if(ret) return ret;
	}

	
	if((a->type==1)&&(b->type==2)) {
		TList wrap;
		TList wrapped;
		wrap.type=2;
		wrap.right=NULL;
		wrap.up=&wrapped;
		wrapped.right=NULL;
		wrapped.up=NULL;
		wrapped.type=1;
		wrapped.val=a->val;
		ret = compItem(&wrap, b);
		if(ret) return ret;
	}
	if((a->type==2)&&(b->type==1)) {
		TList wrap;
		TList wrapped;
		wrap.type=2;
		wrap.right=NULL;
		wrap.up=&wrapped;
		wrapped.right=NULL;
		wrapped.up=NULL;
		wrapped.type=1;
		wrapped.val=b->val;
		ret = compItem(a, &wrap);
		if(ret) return ret;
	}


	if((!a->right)&&(b->right)) return -1;
	if((a->right)&&(!b->right)) return 1;

	if((a->right)&&(b->right)) {
		ret=compItem(a->right, b->right);
		if(ret) return ret;
	}

	return 0;
}

int printItem(TList *a) {

	switch(a->type) {
		case 0: 
			return 0;
		case 1:
			printf("%d",a->val);
			if(a->right) {
				printf(",");
				printItem(a->right);
			}
			return 1;
		case 2:
			printf("[");
			if(a->up) printItem(a->up);
			printf("]");
			if(a->right) {
				printf(",");
				printItem(a->right);
			}
			return(2);
	}

	return 0;
}

int addItem(TList *a, char *s) {

	char *sub;
	int depth;
	int i=0;
	static int d = 0;

//	printf("%d ###IN: \"%s\"\n", d, s);

	d++;

	while(i<strlen(s)) {
//		printf("     %d Loop start looking at %d of \"%s\", which is %c\n", d, i, s, s[i]);
		if((s[i]>='0')&&(s[i]<='9')) {
//			printf("     %d Add No. %d\n", d, s[i]-'0'); 
			a->type=1;
			a->val=atoi(s+i);
			while((s[i]>='0')&&(s[i]<='9')) i++;
		} else
		if(s[i]=='[') {
			sub=strdup(s+i);
			depth=1;
			for(int y=1; y<strlen(sub); y++) {
				if(depth) {
					if(sub[y]=='[') depth++;
					if(sub[y]==']') depth--;
				}
				else sub[y]=0;
			}
//			printf("     %d Add new list %s\n", d, sub); 
			sub[strlen(sub)-1]=0;
		
			a->type=2;
			a->up=(TList*)calloc(1,sizeof(TList));
			addItem(a->up, sub+1);
			i+=strlen(sub)+1;
			free(sub);
		}

		if(s[i]==',') i++;


		if(i<strlen(s)) {		
//			printf("     %d Shifting right\n", d); 
			a->right=(TList*)calloc(1,sizeof(TList));
			a=a->right;
		}
	}

	d--;

	return i;
}

// Read input file line by line (e.g., into an array)
TList *readInput() {
//int readInput() {
        FILE * input;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
	int count = 0;

        input = fopen("input.txt", "r");
        if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate one-dimensional array of strings
	// char **inst=(char**)calloc(MAXX, sizeof(char*));
	list=(TList*)calloc(MAXX, sizeof(TList));

	// Allocate a two-dimensional arrray of chars
	// int x=0, y=0;
        // char **map=calloc(MAXY,sizeof(char*));
	// for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

        while ((read = getline(&line, &len, input)) != -1) {

		line[strlen(line)-1]=0;

		if(strlen(line)>0) {
//			printf(" orig: %s\n", line);
			addItem(&(list[cnt]), line);

//			printf(" orig: %s\n  new: ", line);
//			printItem(&(list[cnt]));
//			printf("\n");
//			if(cnt%2) printf("\n");
			cnt++;
		}

		count++;
	}

	fclose(input);
        if (line)
        free(line);

//	printMap(map);

	return 0;
//	return inst;
//	return map;
}

int main(int argc, char *argv[]) {

	TList a,b;
	int i;
	int c;
	int sum = 0;
	int pairno = 1;
	int eq = 0;

	readInput();

	for(i=0; i<cnt; i+=2) {
		printf("pairno %d\n", pairno);
		a=list[i];
		b=list[i+1];
		c=compItem(&a,&b);
		printf("%c ", c<0?'*':' ');
		printItem(&(a));
		printf("\n");
		printf("%c ", c>0?'*':' ');
		printItem(&(b));
		printf("\n\n");
		if(c<0) sum+=pairno;
		if(c==0) eq++;
		pairno++;
	}

	printf("Pair sum %d, %d equal\n", sum, eq);

	return 0;
}
