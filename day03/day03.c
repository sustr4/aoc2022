#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>

// Boundary definitions, set as required
#define MAXX 310

typedef struct {
	char **x;
	char **y;
	char **f;
} TRow;

// Read input file line by line (e.g., into an array)
TRow readInput() {
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
	TRow inst;
	inst.x=(char**)calloc(MAXX, sizeof(char*));
	inst.y=(char**)calloc(MAXX, sizeof(char*));
	inst.f=(char**)calloc(MAXX, sizeof(char*));

        while ((read = getline(&line, &len, input)) != -1) {

		asprintf(&(inst.x[count]),"%s",line);
		asprintf(&(inst.y[count]),"%s",line+strlen(line)/2);
		inst.x[count][strlen(line)/2]=0;
		inst.f[count]=strdup(line);
		inst.f[count][strlen(line)-1]=0;
		
		count++;
	}

	fclose(input);
        if (line)
        free(line);

	return inst;
}

int main(int argc, char *argv[]) {
	int i, x, y, z;
	TRow row;
	int sum=0, pt;

	row=readInput();

	for(i=0; row.x[i]; i++) {
		
		pt=0;
		for(x=0; x<strlen(row.x[i]); x++) {
			for(y=0; y<strlen(row.y[i]); y++) {
				if(row.x[i][x]==row.y[i][y]) {
					pt = row.y[i][y] >= 'a' ? row.y[i][y] - 'a' + 1 : row.y[i][y] - 'A' + 27;
					sum+=pt;
//					printf("%c %d\n",row.y[i][y], pt);
					break;
				}
			}
			if(pt) break;
		}
//		printf("%s\n%s\n", row.x[i], row.y[i]);
	}

	printf("Sum: %d\n", sum);

	sum=0;

	for(i=0; row.f[i]; i+=3) {
		pt=0;
//		printf("%s\n%s\n%s\n", row.f[i], row.f[i+1], row.f[i+2]);
		for(x=0; x<strlen(row.f[i]); x++) {
			for(y=0; y<strlen(row.f[i+1]); y++) {
				for(z=0; z<strlen(row.f[i+2]); z++) {
					if((row.f[i][x]==row.f[i+1][y])&&
					   (row.f[i][x]==row.f[i+2][z])) {
						pt = row.f[i+1][y] >= 'a' ? row.f[i+1][y] - 'a' + 1 : row.f[i+1][y] - 'A' + 27;
						sum+=pt;
//						printf("%c %d\n",row.f[i+1][y], pt);
						break;
					}
				}
				if(pt) break;
			}
			if(pt) break;
		}
//		printf("%s\n%s\n", row.f[i], row.f[i+1]);
	}
	printf("Sum: %d\n", sum);

	return 0;
}
