#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>

// Read input file line by line (e.g., into an array)
int readInput() {
        FILE * input;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
	int count = 0;
	int sum=0;
	int hold;

        input = fopen("input.txt", "r");
        if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }


        while ((read = getline(&line, &len, input)) != -1) {

		if(strlen(line)>1) {
			sscanf(line,"%d", &hold);
			sum+=hold;
		}
		else {
			printf("%d\n", sum);
			sum=0;
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
