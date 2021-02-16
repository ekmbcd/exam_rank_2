#include <stdio.h>
int		ft_printf(const char *s, ...);

int main(void) {
	char *s = "ciaone";
  int i = ft_printf("Hello World (%10.0d) (%2.10x) (%2.5s)\n" ,2147483647 + 1 , 80,s );
  int j = printf("Hello World (%10.0d) (%2.10x) (%2.5s)\n" ,2147483647 + 1, 80, s);
  printf("%d %d\n", i, j);
  return 0;
}