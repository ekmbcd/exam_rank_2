#include <stdio.h>
int		ft_printf(const char *s, ...);

int main(void) {
	char *s = 0;
  int i = ft_printf("Hello World (%2.4d)\n" ,12 );
  int j = printf("Hello World (%2.4d)\n" , 12);
  printf("%d %d\n", i, j);
  return 0;
}
