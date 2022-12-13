#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>

// Boundary definitions, set as required
#define MAXX 450
#define MAXY 26

// Point structure definition
typedef struct {
	int x;
	int y;
	int z;
} TPoint;

int comp(char *a, char *b)
{
	int shorter = strlen(a);
	if(shorter>strlen(b)) shorter=strlen(b);

	for(int i=0; i<shorter; i++) {
		if((a[i]>='0')&&
		   (a[i]<='9')&&
		   (b[i]>='0')&&
		   (b[i]<='9')) {
			if(a[i]<b[i]) return -1;
			if(a[i]>b[i]) return 1;
		}

		if((a[i]=='[')&&(b[i]==']')) return 1;
		if((a[i]==']')&&(b[i]=='[')) return -1;

	}

	if(strlen(a)<strlen(b)) return -1;
	if(strlen(a)>strlen(b)) return 1;

	return 0;	
}

int insert(char *s, int p) {
	char *tail;
	int i;
	int depth =0;

	//Make room left
	tail=strdup(s+p);
	strcpy(s+p+1, tail);
	free(tail);

	s[p]='[';

	for(i=p+1; i<strlen(s); i++) {
		if(s[i]=='[') {
			depth++;
			continue;
		}
		if(s[i]==']') {
			if(depth>0) {
				depth--;
				continue;
			}
			else break;
		}
		if(s[i]==',')
			if(depth==0) break;
	}

	//Make room left
	tail=strdup(s+i);
	strcpy(s+i+1, tail);
	free(tail);

	s[i]=']';
	
	return 1;
}

// Read input file line by line (e.g., into an array)
char **readInput() {
        FILE * input;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
	int count = 0;
	int stringno = 0;

        input = fopen("input.txt", "r");
        if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	char **strings=(char**)calloc(MAXX, sizeof(char*));
	
        while ((read = getline(&line, &len, input)) != -1) {

		if(strlen(line)>1) {
			strings[stringno]=calloc(2048,sizeof(char));
			line[strlen(line)-1]=0;
			strcpy(strings[stringno],line);
			stringno++;
		}
			
		count++;
	}

	fclose(input);
        if (line)
        free(line);

	return strings;
}

int main(int argc, char *argv[]) {


//	TPoint array;
	int i=0, ya,yb;	
//	array = readInput();
	char *a, *b;
	char **strings = readInput();
	int change;
	int pairno = 1;
	int c;
	int sum=0;

//	for(i=4; i<6; i+=2) {
	for(i=0; (strings[i])&&(strings[i+1]); i+=2) {
		a=strings[i];
		b=strings[i+1];

		change=1;

		while(change) {
			change=0;

			ya=0; yb=0;

			while((ya<strlen(a))&&(yb<strlen(b))) {
				if((a[ya]=='[')&&(b[yb]!='[')) {
//					printf("compAring %c %c\n", a[ya], b[yb]);
					if(a[ya+1]==']') {
						ya+=2;
						continue;
					}
					change = insert(b, yb);
				}
				if((a[ya]!='[')&&(b[yb]=='[')) {
//					printf("compBring %c %c\n", a[ya], b[yb]);
					if(b[yb+1]==']') {
						yb+=2;
						continue;
					}
					change = insert(a, ya);
				}

				ya++;
				yb++;
			}

		}

		c=comp(a,b);

		printf("%c a: %s\n%c b: %s\n", (c<0)?'*':' ', a, (c>0)?'*':' ', b);

		if(c<=0) sum+=pairno;
		pairno++;
	}

	printf("Sum: %d\n", sum);

	return 0;
}
