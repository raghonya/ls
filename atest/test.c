#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	struct stat ape;
	int a = stat(argv[1], &ape);
	printf ("%ld\n", ape.st_size);
	printf ("%ld\n", (ape.st_blocks / 2) - ((ape.st_blocks / 2) % 4));
}