# Author: Jake Wachs
# Date: 11/24/2017
#
# The University of Alabama

OPTS = -Wall -Wextra -std=c99
TYPE_OBJS = integer.o
UTIL_OBJS = scanner.o kruskal.o
STRUCT_OBJS = da.o set.o bst.o rbt.o queue.o cda.o
OBJECT_BUNDLE = $(TYPE_OBJS) $(STRUCT_OBJS) $(UTIL_OBJS)

kruskal: $(OBJECT_BUNDLE)
	gcc $(OPTS) $(OBJECT_BUNDLE) -o ./kruskal

test: kruskal
	kruskal assign3Tests/g0 >> outputs/g0.jake
	diff assign3Tests/g0.expected outputs/g0.jake

	kruskal assign3Tests/g-0-0 >> outputs/g-0-0.jake
	diff assign3Tests/g-0-0.expected outputs/g-0-0.jake

	kruskal assign3Tests/g-0-1 >> outputs/g-0-1.jake
	diff assign3Tests/g-0-1.expected outputs/g-0-1.jake

	kruskal assign3Tests/g-0-2 >> outputs/g-0-2.jake
	diff assign3Tests/g-0-2.expected outputs/g-0-2.jake

	kruskal assign3Tests/g-0-3 >> outputs/g-0-3.jake
	diff assign3Tests/g-0-3.expected outputs/g-0-3.jake

	kruskal assign3Tests/g-0-4 >> outputs/g-0-4.jake
	diff assign3Tests/g-0-4.expected outputs/g-0-4.jake

	kruskal assign3Tests/g-0-5 >> outputs/g-0-5.jake
	diff assign3Tests/g-0-5.expected outputs/g-0-5.jake

	kruskal assign3Tests/g-0-6 >> outputs/g-0-6.jake
	diff assign3Tests/g-0-6.expected outputs/g-0-6.jake

	kruskal assign3Tests/g-0-7 >> outputs/g-0-7.jake
	diff assign3Tests/g-0-7.expected outputs/g-0-7.jake

	kruskal assign3Tests/g-0-8 >> outputs/g-0-8.jake
	diff assign3Tests/g-0-8.expected outputs/g-0-8.jake

	kruskal assign3Tests/g-0-9 >> outputs/g-0-9.jake
	diff assign3Tests/g-0-9.expected outputs/g-0-9.jake

	kruskal assign3Tests/g-0-10 >> outputs/g-0-10.jake
	diff assign3Tests/g-0-10.expected outputs/g-0-10.jake



	rm outputs/*.jake

kruskal.o: kruskal.c
	gcc $(OPTS) kruskal.c -c

set.o: set.c set.h
	gcc $(OPTS) set.c -c

da.o: da.c da.h
	gcc $(OPTS) da.c -c

scanner.o: scanner.c scanner.h
	gcc $(OPTS) scanner.c -c

integer.o: integer.c integer.h
	gcc $(OPTS) integer.c -c

rbt.o: rbt.c rbt.h
	gcc $(OPTS) rbt.c -c

bst.o: bst.c bst.h
	gcc $(OPTS) bst.c -c

queue.o: queue.c queue.h
	gcc $(OPTS) queue.c -c

cda.o: cda.c cda.h
	gcc $(OPTS) cda.c -c

#real/string/int?

clean:
	rm -rf *.o ./a.out kruskal
