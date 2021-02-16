# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>

//define bases for utoa
# define BASE_16	"0123456789abcdef"
# define BASE_10	"0123456789"

//GLOBALS
int width;
int precision;
//set to 1 if there is a '.' (so 0 precision can be considered valid)
int checkp;
//set to 1 if str (numbers and str are different with precision)
int is_str;
//set to 1 if number is negative
int negative;
//where we save our string
char *save;
//the list of variables
va_list args;
//the number of chars printed
int ret;

int		ft_strlen(char *str)
{
	int len = 0;

	while (str[len])
		len++;
	return (len);
}

int isnum(char c)
{
	if (c >= '0' && c <= '9')
		return 1;
	return 0;
}

//used to copy string in case of %s
void	ft_strcpy(char *dst,  char *src)
{
	int	index = 0;

	if (!dst && !src)
		return ;
	while (src[index])
	{
		dst[index] = src[index];
		index++;
	}
	dst[index] = 0;
}

void	ft_printchar(char c)
{
	write(1, &c, 1);
	//increment global to count printed chars
	ret++;
}

//utoa used to transform all numbers in strings
char	*utoa_base(unsigned int n, char *base)
{
	//len starts at 1
	int				len = 1;
	char			*str;
	unsigned int	tmp = n;
	unsigned int	baselen = ft_strlen((char *)base);

	//find len to malloc
	while (tmp /= baselen)
		len++;
	str = malloc(len + 1);
	//terminate at len
	str[len] = 0;
	//print the last number
	str[--len] = base[n % baselen];
	//print the other numbers
	while (n /= baselen)
		str[--len] = base[n % baselen];
	return (str);
}

//atoi used to get width and precision
int ft_atoi(char *s)
{
	int n = 0, i = 0;
	while(isnum(s[i]))
		n = n * 10 + s[i++] - '0';
	return n;
}


//REMEMBER to reset globals
void	init(void)
{
	width = 0;
	checkp = 0;
	precision = 0;
	is_str = 0;
	negative = 0;
	save = 0;
}

//adds zeros before numbers (if precision > len)
void		add_zero(int len)
{
	int		i = 0;
	int		j = 0;
	char	*tmp;

	tmp = malloc(precision + 1);
	//puts (precision - len) zeros at the start
	while ((i + len) < precision)
		tmp[i++] = '0';
	//copy the old number
	while (save[j])
		tmp[i++] = save[j++];
	//remember to terminate
	tmp[i] = 0;
	//free the old string (if it exists)
	if (save)
		free(save);
	//put the new string in the global
	save = tmp;
}

void		printword(int len)
{
	int i;

	i = 0;
	//we have a number with precision > len -> add zeroes
	if (!(is_str) && precision > len)
	{
		add_zero(len);
		//update len
		len = precision;
	}
	//after we have correct len, start printing spaces
	while (len < width)
	{
		ft_printchar(' ');
		len++;
	}
	//print the '-' after spaces
	if (negative)
		ft_printchar('-');
	//start printing our saved string
	while ((save)[i])
		ft_printchar((save)[i++]);
}

void		prepare_print(void)
{
	int len;

	len = ft_strlen(save);
	//cut string if necessary ("0" with precision 0 gets cut)
	if (checkp && precision < len && (is_str || (*save == '0' && *(save + 1) == 0)))
	{
		len = precision;
		save[len] = 0;
	}
	//if we need to print '-' we put one less space
	if (negative)
		width -= 1;
	printword(len);
}


//if string pointer is null print "(null)"
void		printnull(void)
{
	//all strings need to be malloced
	save = malloc(7);
	ft_strcpy(save, "(null)");
}


void	d_case( char **s)
{
	int n;

	n = va_arg(args, int);
	// %d can be negative
	if (n<0)
	{
		//remember to set the global to print the '-' (and switch sign)
		negative = 1;
		n = -n;
	}
	//everything is saved as a string
	save = utoa_base((unsigned int)n, BASE_10);
	prepare_print();
	//skip the 'd'
	(*s)++;
}

void	s_case( char **s)
{
	char *tmp;

	//string are special (precision cuts them)
	is_str = 1;
	//if null string print "(null)"
	if (!(tmp = va_arg(args, char *)))
		printnull();
	else
	{
		save = malloc(ft_strlen(tmp) + 1);
		//need to malloc a copy of the string
		ft_strcpy(save, tmp);
	}
	prepare_print();
	//skip the 's'
	(*s)++;
}

void	x_case( char **s)
{
	int n;

	// %x are unsigned
	n = va_arg(args, int);
	//everything is saved as a string
	save = utoa_base((unsigned)n, BASE_16);
	prepare_print();
	//skip the 'x'
	(*s)++;
}

void	base_case( char **s)
{
	//case s, d or x
	if (**s == 's')
		s_case(s);
	else if (**s == 'd')
		d_case(s);
	else if (**s == 'x')
		x_case(s);
	//REMEMBER to free after printing
	if (save)
		free(save);
}

void	width_case( char **s)
{
	//there are numbers
	if (**s >= '0' && **s <= '9')
	{
		width = ft_atoi(*s);
		//skip width after reading it
		while (**s >= '0' && **s <= '9')
			(*s)++;
	}
}

void	precision_case( char **s)
{
	//there is precision
	if (**s == '.')
	{
		//remember to set the global
		checkp = 1;
		//skip the point
		(*s)++;
		if (**s >= '0' && **s <= '9')
		{
			precision = ft_atoi(*s);
			//skip the precision after reading it
			while ((**s >= '0' && **s <= '9'))
				(*s)++;
		}
	}
}

void	percentagecase( char **s)
{
	//reset globals
	init();
	//get width and precision
	width_case(s);
	precision_case(s);
	//convert and print the variable
	base_case(s);
}

int		ft_printf(const char *s, ...)
{
	//set return to 0
	ret = 0;
	//REMEMBER TO va_start(args, s)
	va_start(args, s);
	//start reading s
	while (*s)
	{
		//if *s = '%' start checking flags
		if (*s == '%')
		{
			//skip the %
			s++;
			//remember to pass the address (to move the pointer)
			percentagecase((char **)&s);
		}
		//else just print the char
		else
		{
			ft_printchar(*s);
			s++;
		}
	}
	//REMEMBER TO va_end(args)
	va_end(args);
	return (ret);
}
