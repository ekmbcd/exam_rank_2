  
gcc -w ft_printf.c main3.c -o ft_printf
./ft_printf > yy
cat -e yy > y
gcc -w -D REAL main3.c -o printf
./printf | cat -e > r
diff -y --suppress-common-lines r y
rm -rf yy y r ft_printf printf