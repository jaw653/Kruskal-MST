# Author: Jake Wachs
# Date: 11/24/2017
#
# The University of Alabama

OPTS = -Wall -Wextra -std=c99
TYPE_OBJS = integer.o
UTIL_OBJS = scanner.o kruskal.o
STRUCT_OBJS = da.o set.o
OBJECT_BUNDLE = $(TYPE_OBJS) $(STRUCT_OBJS) $(UTIL_OBJS)

kruskal: $(OBJECT_BUNDLE)
	gcc $(OPTS) $(OBJECT_BUNDLE) -o ./kruskal

test: kruskal
	#pass

kruskal.o: kruskal.c
	gcc $(OPTS) kruskal.c -c

da.o: da.c da.h
	gcc $(OPTS) da.c -c

scanner.o: scanner.c scanner.h
	gcc $(OPTS) scanner.c -c

integer.o: integer.c integer.h
	gcc $(OPTS) integer.c -c

#real/string/int?

clean:
	rm -rf *.o ./a.out kruskal
