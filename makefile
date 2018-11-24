OOPTS = -g -std=c99 -Wall -Wextra -c
LOPTS = -g -std=c99 -Wall -Wextra
CDAOBJS = cda.o cda-2-16.o integer.o real.o string.o
QOBJS = queue.o cda.o test-queue.o integer.o
BSTOBJS = tnode.o bst.o queue.o cda.o bst-2-3.o string.o real.o integer.o
GSTOBJS = tnode.o bst.o queue.o cda.o gst.o gst-0-4.o integer.o real.o string.o
RBTOBJS = tnode.o bst.o queue.o cda.o gst.o rbt-0-10.o string.o integer.o real.o rbt.o
TREESOBJS = tnode.o bst.o gst.o rbt.o cda.o queue.o string.o real.o integer.o scanner.o interpreter.o trees.o

all : cda queue bst gst rbt trees

cda : $(CDAOBJS)
	gcc $(LOPTS) $(CDAOBJS) -o cda
queue : $(QOBJS)
	gcc $(LOPTS) $(QOBJS) -o queue
bst : $(BSTOBJS)
	gcc $(LOPTS) $(BSTOBJS) -o bst
gst : $(GSTOBJS)
	gcc $(LOPTS) $(GSTOBJS) -o gst
rbt : $(RBTOBJS)
	gcc $(LOPTS) $(RBTOBJS) -o rbt
trees : $(TREESOBJS)
	gcc $(LOPTS) $(TREESOBJS) -o trees
integer.o : integer.c integer.h
	gcc $(OOPTS) integer.c
real.o : real.c real.h
	gcc $(OOPTS) real.c
string.o : string.c string.h
	gcc $(OOPTS) string.c
scanner.o : scanner.c scanner.h
	gcc $(OOPTS) scanner.c
cda.o : cda.c cda.h
	gcc $(OOPTS) cda.c
queue.o : queue.c queue.h cda.h
	gcc $(OOPTS) queue.c
tnode.o : tnode.c tnode.h
	gcc $(OOPTS) tnode.c
interpreter.o : interpreter.c interpreter.h rbt.h gst.h
	gcc $(OOPTS) interpreter.c
bst.o : bst.c bst.h tnode.h queue.h
	gcc $(OOPTS) bst.c
gst.o : gst.c gst.h bst.h tnode.h queue.h
	gcc $(OOPTS) gst.c
rbt.o : rbt.c rbt.h tnode.h gst.h
	gcc $(OOPTS) rbt.c
trees.o : trees.c gst.h rbt.h string.h interpreter.h
	gcc $(OOPTS) trees.c
cda-2-16.o : cda-2-16.c cda.h integer.h real.h string.h
	gcc $(OOPTS) cda-2-16.c
test-queue.o : test-queue.c queue.h cda.h
	gcc $(OOPTS) test-queue.c
bst-2-3.o : bst-2-3.c string.h queue.h bst.h integer.h real.h
	gcc $(OOPTS) bst-2-3.c
gst-0-4.o : gst-0-4.c integer.h string.h queue.h bst.h real.h
	gcc $(OOPTS) gst-0-4.c
rbt-0-10.o : rbt-0-10.c gst.h rbt.h integer.h real.h string.h
	gcc $(OOPTS) rbt-0-10.c
test : rbt gst bst trees #cda queue
	#./cda
	#./stack
	#./queue
	#./bst
	#./gst
	#./rbt
	./trees -g t-0-0-corpus.txt t-0-0-commands.txt
valgrind : bst gst rbt #trees cda queue
	#valgrind --leak-check=full ./da
	#valgrind --leak-check=full ./cda
	#valgrind --leak-check=full ./stack
	#valgrind --leak-check=full ./queue
	valgrind --tool=memcheck --leak-check=full bst
	valgrind --tool=memcheck --leak-check=yes gst
	valgrind --tool=memcheck --leak-check=yes rbt
clean :
	rm -f $(BSTOBJS) $(GSTOBJS) $(RBTOBJS) $(QOBJS) $(CDAOBJS) $(TREESOBJS) \
	bst gst cda queue rbt trees
copy :
	#cp ../objects/bst.o .
	#cp ../objects/gst.o .
	#cp ../objects/rbt.o .
	cp ../objects/cda.o .
