#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// This program takes 2 inputs: 1.characrter to be repeated 2. time between each repeat
int main(int argc, char* argv[])
{
	if (argc != 3) {
		fprintf(stderr, "Usage: inf tag interval\n");
	} else {
		const char* tag = argv[1];
		int interval = atoi(argv[2]);
		int i=0;
		while(i<5) {
			printf("%s\n", tag);
			sleep(interval);
			i++;
		}
	}
	exit(0);
}
