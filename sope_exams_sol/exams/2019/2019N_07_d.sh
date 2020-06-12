gcc -Wall -lrt -pthread 2019N_07_d.c -o 2019N_07_d
echo "Going to run"
echo -e "3\n" | ./2019N_07_d > out.txt
