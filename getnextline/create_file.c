#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	int i = 0;
	while(i < 100000)
	{
		printf("%d", i%10);
		i++;
	}
	printf("\n");
	i = 0;
	while(i < 100)
	{
		printf("%d\n", i);
		i++;
	}
}