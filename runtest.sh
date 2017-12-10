!# \bin\bash
# Runs make test then cleans outputs/ folder

printf "\nPreliminary clean...\n"
make clean
printf "\nOK, clean. Now running make test...\n"
make test

printf "\nCleaning outputs/ folder...\n"
rm outputs/*
printf "Clean.\n"
