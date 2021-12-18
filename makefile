project: user_interface.o database.o
        gcc -o project user_interface.o database.o
user_interface.o: user_interface.c database.c
        gcc -c -Wall -ansi -pedantic-errors user_interface.c
database.o: database.c database.h record.h
        gcc -c -Wall -ansi -pedantic-errors database.c
