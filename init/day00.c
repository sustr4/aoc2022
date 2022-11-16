#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>

// Comparator function example
int comp(const void *a, const void *b)
{
  const int *da = (const int *) a;
  const int *db = (const int *) b;
  return (*da > *db) - (*da < *db);
}

// Example for calling qsort()
//qsort(array,count,sizeof(),comp);

// Full block character for maps █

// Read input file line by line (e.g., into an array)
int readInput() {
        FILE * input;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;

        input = fopen("input.txt", "r");
        if (input == NULL) exit(1);

        while ((read = getline(&line, &len, input)) != -1) {
	
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
