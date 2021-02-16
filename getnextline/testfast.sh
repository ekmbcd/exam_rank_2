gcc create_file.c -o mk
./mk > long
gcc fast.c main.c -o fst
time cat long | ./fst > f_out
echo "difference = "
diff -y --suppress-common-line long f_out
rm mk f_out fst long