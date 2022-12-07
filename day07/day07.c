#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>

// Boundary definitions, set as required
#define MAXX 10000

// Point structure definition
typedef struct {
	char *name;
} TDir;

// Read input file line by line (e.g., into an array)
//TPoint *readInput() {
int readInput() {
        FILE * input;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
	int count = 0;
	int dircount = 0;
	int depth = -1;
	char *name=NULL;
	int size;

        input = fopen("input.txt", "r");
        if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	TDir *dir=(TDir*)calloc(MAXX, sizeof(TDir));

        while ((read = getline(&line, &len, input)) != -1) {

		if(line[0]=='$') {
			if(line[2]=='l') continue;
			if(line[5]=='.') {
				free(dir[depth].name);
				dir[depth].name=NULL;
				depth--;
			}
			else {
				depth++;
				dir[depth].name=strdup(line+5);
				dir[depth].name[strlen(dir[depth].name)-1]=0;
				dircount++;
			}


		}
		else {
			if(line[0]=='d') continue;
			name=(char*)calloc(strlen(line),sizeof(char));
			sscanf(line,"%d %s", &size, name);
			for(int y=0; y<=depth; y++) {
				for(int i=0; i<=depth-y; i++)
					printf("/%s", dir[i].name);
				printf("\t%d\n", size);
			}
			free(name);
		}

		count++;
	}

	fclose(input);
        if (line)
        free(line);


	return 0;
}

int main(int argc, char *argv[]) {

	readInput();

	return 0;
}
