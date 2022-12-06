#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>


// Read input file line by line (e.g., into an array)
char *readInput() {
//int readInput() {
        FILE * input;
        char * line = NULL;
        size_t len = 0;
	char *ret;

        input = fopen("input.txt", "r");
        if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

        getline(&line, &len, input);
	asprintf(&ret, "%s", line);	

	fclose(input);
        if (line)
        free(line);

	return ret;
}

int main(int argc, char *argv[]) {
	int i, x, y, cnt=14, match;
	char *c;
	c = readInput();

	for(i=cnt; i<strlen(c); i++) {
		match=0;
		for(x=i-cnt; x<i-1; x++) {
			for(y=x+1; y<i; y++) {
				if(c[x]==c[y]) { match++;
				}
			}
		}
		if(!match) {
			printf("%d\n",i);
			break;
		}
	}


	return 0;
}
