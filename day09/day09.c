#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>


// Point structure definition
typedef struct {
	int x;
	int y;
} TPoint;

// Read input file line by line (e.g., into an array)
TPoint *readInput() {
//int readInput() {
        FILE * input;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
	int count = 0;
	char dir;
	int no;
	TPoint t[11];
	TPoint new;

        input = fopen("input.txt", "r");
        if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	memset(&t,0,11*sizeof(TPoint));


		printf("%d, %d\n", t[9].x, t[9].y);
        while ((read = getline(&line, &len, input)) != -1) {

		// Read into array
		sscanf(line,"%c %d",
			&dir,
			&no);


		for(int i = 0; i<no; i++) {
			switch(dir) {
				case 'U':
					t[0].y--;
					break;
				case 'D':
					t[0].y++;
					break;
				case 'L':
					t[0].x--;
					break;
				case 'R':
					t[0].x++;
					break;
			}
			for(int y=1; y<10; y++) {
				new=t[y];

				if((t[y].y<=t[y-1].y+1)&&
				   (t[y].y>=t[y-1].y-1)&&
				   (t[y].x<=t[y-1].x+1)&&
				   (t[y].x>=t[y-1].x-1)) {
					t[y]=new;
				}
				else {
					if(t[y].y>t[y-1].y) t[y].y--;
					if(t[y].y<t[y-1].y) t[y].y++;
					if(t[y].x>t[y-1].x) t[y].x--;
					if(t[y].x<t[y-1].x) t[y].x++;
				}
			}
			fprintf(stderr,"[%d,%d] -- [%d,%d]\n", t[0].x, t[0].y, t[9].x, t[9].y);
			printf("%d, %d\n", t[9].x, t[9].y);
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
