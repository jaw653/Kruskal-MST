#!\bin\bash

printf "\nMAKING...\n"
make
printf "MADE.\n"

printf "COMPILING...\n"
gcc -Wall -Wextra -std=c99 da.o set.o integer.o testSet.c -o testSet
printf "COMPILED.\n"

printf "\nEXECUTING...\n"
./testSet
printf "EXECUTED.\n"

printf "\nMAKING CLEAN...\n"
make clean
rm testSet
printf "CLEAN.\n"
