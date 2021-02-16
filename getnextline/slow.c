#include <stdlib.h>
#include <unistd.h>

//	questo programma POTREBBE andare in timeout
//	per linee più lunghe di 20000 caratteri
//	ma e' molto piu' corto e facile di fast.c

char *charjoin(char *s, char c, int max)
{
	char *ret = malloc(max + 2);
	int i = -1;
	while(s[++i])
		ret[i] = s[i];
	ret[i++] = c;
	ret[i] = 0;
	free(s);
	return (ret);
}

int gnl(char **line)
{
	char c;
	int i, max = 0;
	if(line == 0)
		return -1;
	*line = malloc(1);
	*line[0] = 0;
	while((i = read(0, &c, 1)) > 0)
	{
		if(c == '\n')
			break;
		*line = charjoin(*line, c, max++);
	}
	if (i == -1)
		return(-1);
	return i == 0 ? 0 : 1;
}
