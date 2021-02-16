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

//used to copy string in case of %s
size_t	ft_strcpy(char *dst, const char *src)
{
	int	index = 0;

	if (!dst && !src)
		return (0);
	while (src[index])
	{
		dst[index] = src[index];
		index++;
	}
	dst[index] = 0;
	return (index);
}

void	ft_printchar(char c)
{
	write(1, &c, 1);
	//increment global to count printed chars
	ret++;
}

//adds zeros before numbers (if precision > len)
int		add_zero(int len)
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
	return (0);
}

//utoa used to transform all numbers in strings
char	*utoa_base(unsigned int n, const char *base)
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
unsigned int		ft_atoi(const char *n)
{
	unsigned int	nb = 0;

	while (*n <= '9' && *n >= '0')
	{
		nb = nb * 10 + (*n - '0');
		n++;
	}
	return (nb);
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

int		printword(int len)
{
	int i;

	i = 0;
	//we have a number with precision > len -> add zeroes
	if (!(is_str) && checkp && precision > len)
	{
		add_zero(len);
		//update len
		len = ft_strlen(save);
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
	{
		//stop printing when i == precision (if it's "0" or not a number)
		if ((checkp && i == precision) && (is_str ||
			(*save == '0' && *(save + 1) == 0)))
			break ;
		ft_printchar((save)[i++]);
	}
	return (0);
}

int		prepare_print(void)
{
	int len;

	len = ft_strlen(save);
	//cut string if necessary ("0" with precision 0 gets cut)
	if (checkp && precision < len && (is_str || (*save == '0' && *(save + 1) == 0)))
		len = precision;
	//if we need to print '-' we put one less space
	if (negative)
		width -= 1;
	printword(len);
	return (0);
}


//if string pointer is null print "(null)"
int		printnull(void)
{
	//if precision exists and < 6 print nothing
	if(checkp && precision < 6)
	{
		save = malloc(1);
		save[0] = 0;
		return 0;
	}
	//all strings need to be malloced
	save = malloc(7);
	save[0] = '(';
	save[1] = 'n';
	save[2] = 'u';
	save[3] = 'l';
	save[4] = 'l';
	save[5] = ')';
	save[6] = 0;
	return (0);
}


int	d_case(const char **s)
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
	return (0);
}

int	s_case(const char **s)
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
	return (0);
}

int	x_case(const char **s)
{
	int n;

	// %x are unsigned
	n = va_arg(args, int);
	//everything is saved as a string
	save = utoa_base((unsigned)n, BASE_16);
	prepare_print();
	//skip the 'x'
	(*s)++;
	return (0);
}

int	base_case(const char **s)
{
	//case s, d or x
	if (**s == 's')
	{
		if (s_case(s) == -1)
			return (-1);
	}
	else if (**s == 'd')
	{
		if (d_case(s) == -1)
			return (-1);
	}
	else if (**s == 'x')
	{
		if (x_case(s) == -1)
			return (-1);
	}
	//REMEMBER to free after printing
	if (save)
		free(save);
	return (0);
}

int	width_case(const char **s)
{
	//there are numbers
	if (**s >= '0' && **s <= '9')
	{
		width = ft_atoi(*s);
		//skip width after reading it
		while (**s >= '0' && **s <= '9')
			(*s)++;
		return (1);
	}
	return (0);
}

int	precision_case(const char **s)
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
			return (1);
		}
	}
	return (0);
}

int		percentagecase(const char **s)
{
	//reset globals
	init();
	//get width and precision
	while (width_case(s) || precision_case(s))
		;
	//convert and print the variable
	if (base_case(s) == -1)
		return (-1);
	return (0);
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
			if (percentagecase(&s) == -1)
				return -1;
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
 