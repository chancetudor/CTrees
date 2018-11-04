OOPTS = -g -std=c99 -Wall -Wextra -c
LOPTS = -g -std=c99 -Wall -Wextra
CDAOBJS = cda.o test-cda3.o integer.o
QOBJS = queue.o cda.o test-queue.o integer.o
BSTOBJS = tnode.o bst.o queue.o cda.o test-bst.o
GSTOBJS = tnode.o bst.o gst.o
RBTOBJS = tnode.o bst.o gst.o rbt.o
TREESOBJS = tnode.o bst.o gst.o rbt.o string.o interpreter.o

all : cda queue bst gst rbt trees
cda : $(CDAOBJS)
	gcc $(LOPTS) $(CDAOBJS) -o cda
queue : $(QOBJS)
	gcc $(LOPTS) $(QOBJS) -o queue
bst : $(BSTOBJS)
	gcc $(LOPTS) $(BSTOBJS) -o bst
#gst : $(GSTOBJS)
#	gcc $(LOPTS) $(GSTOBJS) -o gst
#rbt : $(RBTOBJS)
#	gcc $(LOPTS) $(RBTOBJS) -o rbt
#trees : $(TREESOBJS)
#	gcc $(LOPTS) $(TREESOBJS) -o trees
integer.o : integer.c integer.h
	gcc $(OOPTS) integer.c
string.o : string.c string.h
	gcc $(OOPTS) string.c
cda.o : cda.c cda.h
	gcc $(OOPTS) cda.c
queue.o : queue.c queue.h cda.h
	gcc $(OOPTS) queue.c
tnode.o : tnode.c tnode.h
	gcc $(OOPTS) tnode.c
bst.o : bst.c bst.h tnode.c tnode.h queue.c queue.h
	gcc $(OOPTS) bst.c
#gst.o : bst.c bst.h tnode.c tnode.h queue.h
#	gcc $(OOPTS) bst.c
#rbt.o : bst.c bst.h tnode.c tnode.h queue.h
#	gcc $(OOPTS) bst.c
#trees.o :
#	gcc
test-cda3.o : test-cda3.c cda.h
	gcc $(OOPTS) test-cda3.c
test-queue.o : test-queue.c queue.h cda.h
	gcc $(OOPTS) test-queue.c
test-bst.o : test-bst.c integer.h string.h queue.h bst.h
	gcc $(OOPTS) test-bst.c
test : bst #gst rbt trees cda queue
	#./da
	#./cda
	#./stack
	#./queue
	#./bst
#valgrind : maze #da stack cda queue
	#valgrind --leak-check=full ./da
	#valgrind --leak-check=full ./cda
	#valgrind --leak-check=full ./stack
	#valgrind --leak-check=full ./queue
#	valgrind --tool=memcheck --leak-check=yes amaze -r 386 -s unready.maze ready.maze -d ready.maze -c 40 28 unready.maze
clean :
	rm -f $(BSTOBJS) $(GSTOBJS) $(RBTOBJS) $(QOBJS) $(CDAOBJS) $(TREESOBJS) \
	bst gst cda queue rbt trees