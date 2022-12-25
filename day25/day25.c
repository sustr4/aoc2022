#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<malloc.h>
#include<limits.h>
#include<assert.h>

// Boundary definitions, set as required
#define MAXX 150
#define MAXY 1000

// Read input file line by line (e.g., into an array)
char **readInput() {
        FILE * input;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
	int count = 0;
	int maxlen = 0;


        input = fopen("input.txt", "r");
        if (input == NULL) {
		fprintf(stderr,"Failed to open input file\n");
		exit(1); }

	// Get longest line first
        while ((read = getline(&line, &len, input)) != -1) {
		if(maxlen<strlen(line)-1) maxlen=strlen(line)-1;
		count++;
	}
	fclose(input);

        input = fopen("input.txt", "r");

	char **inst=(char**)calloc(count+1, sizeof(char*));
	for(int i=0; i<count; i++) {
		inst[i]=(char*)calloc(maxlen,sizeof(char));
		memset(inst[i], ' ', maxlen-1);
	}

	count = 0;
        while ((read = getline(&line, &len, input)) != -1) {

		line[strlen(line)-1] = 0;

		for(int i = 0; i<strlen(line); i++) {
			inst[count][i+(maxlen-strlen(line))]=line[i];
		}
		

		count++;
	}
	fclose(input);


        if (line)
        free(line);

	return inst;
}

int val(char c) {
	switch(c) {
		case '=': return -2; break;
		case '-': return -1; break;
		case '0':
		case '1':
		case '2':
			return (int)(c-'0');
			break;
		case ' ':
			return 0;
	}
	assert(0);
	return 0;
}

char encode(int i) {
	switch(i) {
		case 0:
		case 1:
		case 2:
			return i+'0';
			break;
		case -1:
			return '-'; break;
		case -2:
			return '='; break;
	}
	assert(0);
	return 'X';
}

int main(int argc, char *argv[]) {

	char *res = (char*)calloc(MAXY, sizeof(char));
	int out = MAXY-2, o,carry=0,sum,i=0, part;	
	char **array = readInput();

	for(i=0; array[i]; i++) {
		printf("%s\n", array[i]);
	}

	memset(res, ' ', MAXY-1);
	
	o=strlen(array[0])-1;
	for(o=strlen(array[0])-1; o>=0; o--) {

		sum=carry;	
		for(i=0; array[i]; i++) {
			sum+=val(array[i][o]);
		}
		part=sum%5;
		carry=sum/5;
		while(part>2) {
			part-=5;
			carry++;
		}
		while(part<-2) {
			part+=5;
			carry--;
		}
		
		res[out--]=encode(part);

	}
	while((sum=carry)) {
		part=sum%5;
		carry=sum/5;
		while(part>2) {
			part-=5;
			carry++;
		}
		while(part<-2) {
			part+=5;
			carry--;
		}
		
		res[out--]=encode(part);

	}

	for(o=0; res[o]==' '; o++);

	printf("---------------------\n%s\n", res+o);

	return 0;
}
