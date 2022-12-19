#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary definitions, set as required
#define MAXX 34
#define MAXY 26

int *max;
int *history;

int debug=1;

// Robot property
typedef struct {
	int ore;
	int clay;
	int obsidian;
} TProp;

// Blueprint structure definition
typedef struct {
	int no;
	TProp robot[4];
	TProp maxneed;
//	TProp ore;
//	TProp clay;
//	TProp obsidian;
//	TProp geode;
} TBlueprint;

TBlueprint *blueprint;

// Comparator function example
int comp(const void *a, const void *b)
{
  const int *da = (const int *) a;
  const int *db = (const int *) b;
  return (*da > *db) - (*da < *db);
}

// Example for calling qsort()
//qsort(blueprint,count,sizeof(),comp);


// Print a two-dimensional blueprint
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


// Read input file line by line (e.g., into an blueprint)
//TBlueprint *readInput() {
int readInput() {
        FILE * input;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
	int count = 1;

        input = fopen("input.txt", "r");
        if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Allocate one-dimensional blueprint of strings
	// char **blueprint=(char**)calloc(MAXX, sizeof(char*));
	blueprint=(TBlueprint*)calloc(MAXX, sizeof(TBlueprint));
	max=(int*)calloc(MAXX, sizeof(int));
	history=(int*)calloc(30, sizeof(int));

	// Allocate a two-dimensional arrray of chars
	// int x=0, y=0;
        // char **map=calloc(MAXY,sizeof(char*));
	// for(int iter=0; iter<MAXY; iter++) map[iter]=calloc(MAXX,sizeof(char));

        while ((read = getline(&line, &len, input)) != -1) {

		sscanf(line,"Blueprint %d: Each ore robot costs %d ore. Each clay robot costs %d ore. Each obsidian robot costs %d ore and %d clay. Each geode robot costs %d ore and %d obsidian.",
		&(blueprint[count].no),
		&(blueprint[count].robot[0].ore),
		&(blueprint[count].robot[1].ore),
		&(blueprint[count].robot[2].ore),
		&(blueprint[count].robot[2].clay),
		&(blueprint[count].robot[3].ore),
		&(blueprint[count].robot[3].obsidian));


		for(int j=0; j<4; j++) {
			if(blueprint[count].maxneed.ore<blueprint[count].robot[j].ore)
				blueprint[count].maxneed.ore=blueprint[count].robot[j].ore;
			if(blueprint[count].maxneed.clay<blueprint[count].robot[j].clay)
				blueprint[count].maxneed.clay=blueprint[count].robot[j].clay;
			if(blueprint[count].maxneed.obsidian<blueprint[count].robot[j].obsidian)
				blueprint[count].maxneed.obsidian=blueprint[count].robot[j].obsidian;
		}
//		printf("%d: %d (%d)\n ", count, blueprint[count].no, blueprint[count+1].no);

		count++;
	}

	fclose(input);
        if (line)
        free(line);

//	printMap(map);

	return 0;
//	return map;
}

void printType(int i) {
	switch(i) {

		case 0:
			printf("ore"); break;
		case 1:
			printf("clay"); break;
		case 2:
			printf("obsidian"); break;
		case 3:
			printf("geode"); break;
		case 4:
			printf("n"); break;
		default:
			printf("?");
	}
}

void printHistory() {
	for(int i=1; i<=24; i++) {
		printf("%d:", i);
		switch(history[i]) {

			case 0:
				printf("ore, "); break;
			case 1:
				printf("clay, "); break;
			case 2:
				printf("obsidian, "); break;
			case 3:
				printf("geode (%d), ", 24-i); break;
			case 4:
				printf("n, "); break;
			default:
				printf("?");
		}
	}
}

int build(int roboCount[4], int materialCount[4], TBlueprint bp, int time) {

	int *NEWroboCount;
	int *NEWmaterialCount;
	int i;
	int next;

	if(time>24) {
		if(materialCount[3]>max[bp.no]) {
			max[bp.no]=materialCount[3];
			printf("\nNew maximum for blueprint %d: %d\n", bp.no, max[bp.no]);
				printf("M%02d: with robots: [", time);
				for(i=0; i<4; i++) printf("%d,", roboCount[i]);
				printf("] and material: [");
				for(i=0; i<4; i++) printf("%d,", materialCount[i]);
				printf("]\n");
				printHistory();
				printf("\n");
		}
		return 1;
	}

	NEWroboCount = malloc(4 * sizeof(int));
	NEWmaterialCount = malloc(4 * sizeof(int));

	int waitrounds;
	int wr[3] = {0,0,0};
	int oreneed;
	int clayneed;
	int obsidianneed;

	// Try doing one of 4 things
	for(next=0; next<4; next++) {

				if(debug) {
					printf("\n== Minute %02d== \t\t Start of round:       [", time);
					for(i=0; i<4; i++) printf("%d,", roboCount[i]);
					printf("] and material: [");
					for(i=0; i<4; i++) printf("%d,", materialCount[i]);
					printf("]\n");
				}

		// In case there are insufficient resources and also no capacity to produce them
		if((materialCount[0]<bp.robot[next].ore)&&(!roboCount[0])) continue;
		if((materialCount[1]<bp.robot[next].clay)&&(!roboCount[1])) continue;
		if((materialCount[2]<bp.robot[next].obsidian)&&(!roboCount[2])) continue;

		for(int j=0; j<2; j++) wr[j]=0;

		if(debug) {
			printf("  Considering ");
			printType(next);
			if(next<4) {
				printf(" cost [%d, %d, %d]\n",
					bp.robot[next].ore,
					bp.robot[next].clay,
					bp.robot[next].obsidian);
			} else printf("\n");
		}

		oreneed=bp.robot[next].ore-materialCount[0];
		if(oreneed<0) oreneed=0;
		clayneed=bp.robot[next].clay-materialCount[1];
		if(clayneed<0) clayneed=0;
		obsidianneed=bp.robot[next].obsidian-materialCount[2];
		if(obsidianneed<0) obsidianneed=0;

		if(oreneed) wr[0]=oreneed/roboCount[0]+(oreneed%roboCount[0]?2:1);
		if(clayneed) wr[1]=clayneed/roboCount[1]+(clayneed%roboCount[1]?2:1);
		if(obsidianneed) wr[2]=obsidianneed/roboCount[2]+(obsidianneed%roboCount[2]?2:1);

		waitrounds=wr[0];
		if(wr[1]>waitrounds) waitrounds=wr[1];
		if(wr[2]>waitrounds) waitrounds=wr[2];

		if(debug) {
			printf("  Need to wait %d rounds (%d for materials and 1 for production.\n", waitrounds, waitrounds-1);
		}
		

		for(int j=0; j<waitrounds; j++) history[time+j]=4;
		history[time+waitrounds-1]=next; 

		// Copy stock from input
		memcpy(NEWroboCount, roboCount, 4 * sizeof(int));
		memcpy(NEWmaterialCount, materialCount, 4 * sizeof(int));

		// Remove build cost from resources
		if(next<4) {
			NEWmaterialCount[0]-=bp.robot[next].ore;
		    	NEWmaterialCount[1]-=bp.robot[next].clay;
		   	NEWmaterialCount[2]-=bp.robot[next].obsidian;
		}

		// Grow resources with robots available from the beginning
		for(i=0; i<4; i++)
			NEWmaterialCount[i]=NEWmaterialCount[i]+roboCount[i]*waitrounds;

		// Build the new robot
		if(next<4) {
			NEWroboCount[next]++;
		}

				if(debug) {
					printf("            \t\t End of round %02d:       [", time+waitrounds-1);
					for(i=0; i<4; i++) printf("%d,", NEWroboCount[i]);
					printf("] and material: [");
					for(i=0; i<4; i++) printf("%d,", NEWmaterialCount[i]);
					printf("]\n");
				}
		// Next minute
		build(NEWroboCount, NEWmaterialCount, bp, time+waitrounds);
		
		
	}
		


/*

//				printf("\n== Minue %02d== \t\t Start of round:          [", time);
//				for(i=0; i<4; i++) printf("%d,", roboCount[i]);
//				printf("] and material: [");
//				for(i=0; i<4; i++) printf("%d,", materialCount[i]);
//				printf("]\n");


	
	//update material
	for(i=0; i<4; i++) {
		NEWroboCount[i]=roboCount[i];
		NEWmaterialCount[i]=materialCount[i]+roboCount[i];
	}
	if((building>0)&&(building<4)) {
		printf("  Finished building a ");
		printType(building);
		printf("-collecting robot.\n");
		NEWroboCount[building]++;
	}

//				printf("M%02d: Look at building robots: [", time);
//				for(i=0; i<4; i++) printf("%d,", NEWroboCount[i]);
//				printf("] and material: [");
//				for(i=0; i<4; i++) printf("%d,", NEWmaterialCount[i]);
//				printf("]\n");


	//try building each type
	for(i=3; i>=0; i--) {
		if(materialCount[0]<bp.robot[i].ore) continue; // Not enough ore
		if(materialCount[1]<bp.robot[i].clay) continue; // Not enough clay
		if(materialCount[2]<bp.robot[i].obsidian) continue; // Not enough obsidian

		printf("  Spend ");
		if(bp.robot[i].ore) printf("%d ore ", bp.robot[i].ore);
		if(bp.robot[i].clay) printf("%d clay ", bp.robot[i].clay);
		if(bp.robot[i].obsidian) printf("%d obsidian ", bp.robot[i].obsidian);
		printf(" to start building a ");
		printType(i);
		printf("-collecting robot.\n");

		NEWmaterialCount[0]-=bp.robot[i].ore;
		NEWmaterialCount[1]-=bp.robot[i].clay;
		NEWmaterialCount[2]-=bp.robot[i].obsidian;
		history[time]=i;
		build(NEWroboCount, NEWmaterialCount, i, bp, time+1);
		NEWmaterialCount[0]+=bp.robot[i].ore;
		NEWmaterialCount[1]+=bp.robot[i].clay;
		NEWmaterialCount[2]+=bp.robot[i].obsidian;
	}
	//or try building nothing
	if((materialCount[0]<bp.maxneed.ore)||
	   (materialCount[1]<bp.maxneed.clay)||
	   (materialCount[2]<bp.maxneed.obsidian)) {
		printf("  Building nothing\n");
		history[time]=i;
		build(NEWroboCount, NEWmaterialCount, i, bp, time+1);
	}*/

	free(NEWroboCount);
	free(NEWmaterialCount);
	return 0;
}


int main(int argc, char *argv[]) {

	int i=0, j;	
	readInput();
	int *roboCount = calloc(4, sizeof(int));
	int *materialCount = calloc(4, sizeof(int));


	for(i=1; blueprint[i].no; i++) {
		printf("%d: %d\n", i, blueprint[i].no);
		for(j=0; j<4; j++) {
			printf("  %d:\t%4d ore\t%4d clay\t%4d obsidian\n",
				j,
				blueprint[i].robot[j].ore,
				blueprint[i].robot[j].clay,
				blueprint[i].robot[j].obsidian);
		}
		printf("max: \t%4d ore\t%4d clay\t%4d obsidian\n",
				blueprint[i].maxneed.ore,
				blueprint[i].maxneed.clay,
				blueprint[i].maxneed.obsidian);
	}

	roboCount[0]=1;
	debug=1;
	for(i=1; blueprint[i].no; i++) {
		build(roboCount, materialCount, blueprint[i], 1);
	}

	int sum=0;
	for(i=1; i<2; i++) {
//	for(i=1; blueprint[i].no; i++) {
		printf("%d * %d = %d\n", i, max[i], i*max[i]);
		sum+=i*max[i];
	}

	printf("%d\n", sum);

	return 0;
}
