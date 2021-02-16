gcc -Wall -Werror -Wextra slow.c main.c -o get_next_line
./get_next_line < slow.c > yours_.res
cat -e yours_.res > yours.res
cat -e < slow.c > original.res
diff -y --suppress-common-line original.res yours.res
rm -rf original.res yours_.res yours.res get_next_line