#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int gnl(char **line);

int main(void) {
	int		r;
	char	*line;

	line = NULL;
	while ((r = gnl(&line)) > 0)
	{
		printf("%s\n", line);
		free(line);
		line = NULL;
	}
	printf("%s", line);
	free(line);
	line = NULL;
}