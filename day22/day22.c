#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary definitions, set as required
#define MAXX 150
#define MAXY 200
#define MAXSTEP 5000

// Point structure definition
typedef struct {
	int x;
	int y;
	int dir;
} TPoint;

typedef struct {
	int steps;
	char rot;
} TInst;

char **visited;
TInst *inst;
int maxlen=0;
int maxheight=0;

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
			if(visited[y][x]) {
				switch(visited[y][x]) {
					case '0': printf(">"); break;
					case '1': printf("v"); break;
					case '2': printf("<"); break;
					case '3': printf("^"); break;
				}
			}
			else if(map[y][x]) printf("%c", map[y][x]);
		}
		printf("\n");
	}
}
// Full block character for maps █


// Read input file line by line (e.g., into an array)
char **readInput() {
//int readInput() {
        FILE * input;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
	int count = 0;
	int i;

        input = fopen("input.txt", "r");
        if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate one-dimensional array of strings
	// char **inst=(char**)calloc(MAXX, sizeof(char*));
	inst=(TInst*)calloc(MAXSTEP, sizeof(TInst));

	// Allocate a two-dimensional arrray of chars
	// int x=0, y=0;
        char **map=calloc(MAXY,sizeof(char*));
	for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

        visited=calloc(MAXY,sizeof(char*));
	for(int iter=0; iter<MAXY; iter++) visited[iter]=calloc(MAXX,sizeof(char));

        while ((read = getline(&line, &len, input)) != -1) {

		line[strlen(line)-1]=0;

		if(!strlen(line)) continue;
		
		if(line[0]>='0') {
			char *tmp = (char*)calloc(strlen(line)*2, sizeof(char));
			int j=1;

			tmp[0]=line[0];
			for(i=1; i<strlen(line); i++) {
				if(((line[i-1]<'A')&&(line[i]>='A'))||
				   ((line[i]<'A')&&(line[i-1]>='A'))) 
					tmp[j++]=',';
				tmp[j]=line[i];
				j++;
			}

			// Read tokens from single line
			j=0;
			char *token;
			token = strtok(tmp, ",");
			inst[j].steps=atoi(token);
			while( 1 ) {
				if(!(token = strtok(NULL, ","))) break;
				j++;
				if(token[0]>='A') inst[j].rot=token[0];
				else inst[j].steps=atoi(token);
			}

			free(tmp);
		
		}
		else {
			// Read into map
			strcpy(map[count], line);
			if(strlen(line)>maxlen) maxlen=strlen(line);
			maxheight=count;
			count++;
		}

		// for(x=0; x<MAXX; x++) map[y][x] = line[x];
		// y++;

		// Copy to string
		//asprintf(&(inst[count]), "%s", line);	

		// Read into array
		// sscanf(line,"%d,%d",
		//	&(inst[count].x),
		//	&(inst[count].y));


	}

	fclose(input);
        if (line)
        free(line);


	return map;
}

TPoint rotate(TPoint d, char rot) {
	TPoint ret=d;

	switch(rot) {
		case 'L':
			if(--ret.dir<0) ret.dir=3;
			break;
		case 'R':
			if(++ret.dir>3) ret.dir=0;
			break;
		default:
			assert(0);
	}

	switch(ret.dir) {
		case 0:
			ret.x=1; ret.y=0;
			break;
		case 1:
			ret.x=0; ret.y=1;
			break;
		case 2:
			ret.x=-1; ret.y=0;
			break;
		case 3:
			ret.x=0; ret.y=-1;
			break;
		default:
			assert(0);
	}

	return ret;
}

int main(int argc, char *argv[]) {

//	TPoint array;
	char **map;
	int i=0, j;	
//	array = readInput();
	map = readInput();
	int x=0, y=0;
	TPoint d = {1, 0, 0};

//	printMap(map);

	// Jump to the first actual tile
	while(map[y][x]!='.') x++;

	printf("Starting at [%d,%d]\n", x+1, y+1);

	for(i=0; inst[i].steps || inst[i].rot; i++) {
//		printf("%d %c\n", inst[i].steps, inst[i].rot);
		if(inst[i].rot) { // Change of direction
			d=rotate(d, inst[i].rot);
		}
		else { // Move
			for(j=0; j<inst[i].steps; j++) {
				// Move in x
				int nx=x+d.x;
				if ((nx<0)||
				    ((d.x==-1)&&(map[y][nx]<=' '))) {
					nx=maxlen-1;
					while((map[y][nx]!='.')&&(map[y][nx]!='#')) nx--;
					if(map[y][nx]=='#') goto noXmovement;
				}
				if ((nx>=MAXX)||
				    ((d.x==1)&&(map[y][nx]<=' '))) {
					nx=0;
					while((map[y][nx]!='.')&&(map[y][nx]!='#')) nx++;
					if(map[y][nx]=='#') goto noXmovement;
				}
				if (map[y][nx]=='#') goto noXmovement;

				x=nx;
noXmovement:
				// Move in y
				int ny=y+d.y;
				if ((ny<0)||
				    ((d.y==-1)&&(map[ny][x]<=' '))) {
					ny=maxheight;
					while((map[ny][x]!='.')&&(map[ny][x]!='#')) ny--;
					if(map[ny][x]=='#') goto noYmovement;
				}
				if ((ny>=MAXY)||
				    ((d.y==1)&&(map[ny][x]<=' '))) {
					ny=0;
					while((map[ny][x]!='.')&&(map[ny][x]!='#')) ny++;
					if(map[ny][x]=='#') goto noYmovement;
				}
				if (map[ny][x]=='#') goto noYmovement;

				y=ny;
noYmovement:

				visited[y][x]=(char)d.dir+'0';
			}
		}
	}

	printMap(map);

	printf("Result: 1000 * %d + 4 * %d + %d: %d\n", y+1, x+1, d.dir, 1000*(y+1) + 4*(x+1) + d.dir);

	return 0;
}
