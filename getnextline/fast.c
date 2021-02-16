#include <stdlib.h>
#include <unistd.h>

//dimensione del temp array
#define MAX 128

int ftstrlen(char *s)
{
	int i = 0;
	while (s[i])
		i++;
	return i;
}

//strjoin
char *arrjoin(char *s, char temp[MAX])
{
	int len1 = ftstrlen(s);
	//teoricamente malloc andrebbe protetta
	char *ret = malloc(len1 + ftstrlen(temp) + 1);
	int i = -1;

	while(s[++i])
		ret[i] = s[i];
	i = -1;
	while(temp[++i])
		ret[len1 + i] = temp[i];
	ret[i + len1] = 0;
	//libero vecchia line che non mi serve più
	free(s);
	return (ret);
}

int gnl(char **line)
{
	//più grande è max, più veloce è il programma, più memoria usa
	char temp[MAX];
	int i, n = 0;
	if(line == 0)
		return -1;
	//malloc 1 per ritornare linea freeabile in caso di file vuoto
	*line = malloc(1);
	//per sicurezza rendo line una stringa vuota
	*line[0] = 0;
	//finchè read legge almeno un carattere
	while((i = read(0, &temp[n], 1)) > 0)
	{
		//se trovo \n smetto di leggere
		if(temp[n] == '\n')
			break;
		//array temp è pieno
		if(n == MAX - 2)
		{
			//termino temp e lo metto alla fine di line
			temp[n + 1] = 0;
			*line = arrjoin(*line, temp);
			//n ritorna a 0 per poter riutilizzare temp
			n = -1;
		}
		n++;
	}
	//read ha fallito per qualche motivo	
	if (i == -1)
		return(-1);
	//unisco ultimo temp a line
	temp[n] = 0;
	*line = arrjoin(*line, temp);
	//ritorno 0 se read mi ha dato 0
	return i == 0 ? 0 : 1;
}