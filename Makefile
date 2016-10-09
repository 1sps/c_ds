# Makefile for mylib

# St: 2016-10-05 Wed 08:21 PM
# Up: 2016-10-07 Fri 10:39 PM

# Author: SPS

CC=gcc
CFLAGS=-Wall -pedantic -g

SRC=src
TEST=test

all: stack bst avl

##
 ###############################################################################
 # Include dependency files
 ###############################################################################
 ##

DEPFILES=./dep/stack.d ./dep/bst.d ./dep/avl.d ./dep/funcutils.d \
         ./dep/bst_test.d ./dep/avl_test.d

##
 ###############################################################################
 # Phony targets to build inividual structures and test programs
 ###############################################################################
 ##

stack: $(SRC)/stack.o
bst: $(SRC)/bst.o
avl: $(SRC)/avl.o

bst_test: $(TEST)/bst_test

##
 ###############################################################################
 #  Build object files of individual structures
 ###############################################################################
 ##

$(SRC)/stack.o: $(SRC)/stack.c
	$(CC) $(CFLAGS) -MMD -MP -MF ./dep/stack.d -c -o $@ $<

$(SRC)/queue.o: $(SRC)/queue.c
	$(CC) $(CFLAGS) -MMD -MP -MF ./dep/queue.d -c -o $@ $<

$(SRC)/bst.o: $(SRC)/bst.c $(SRC)/mylib.h
	$(CC) $(CFLAGS) -MMD -MP -MF ./dep/bst.d -c -o $@ $<

$(SRC)/avl.o: $(SRC)/avl.c $(SRC)/mylib.h
	$(CC) $(CFLAGS) -MMD -MP -MF ./dep/avl.d -c -o $@ $<

##
 ###############################################################################
 # Build object files of test utilities
 ###############################################################################
 ##

$(TEST)/funcutils.o: $(TEST)/funcutils.c
	$(CC) $(CFLAGS) -MMD -MP -MF ./dep/funcutils.d -c -o $@ $<

##
 ################################################################################
 # Build object files of test programs
 ################################################################################
 ##

$(TEST)/bst_test.o: $(TEST)/bst_test.c
	$(CC) $(CFLAGS) -MMD -MP -MF ./dep/bst_test.d -c -o $@ $<

$(TEST)/avl_test.o: $(TEST)/avl_test.c
	$(CC) $(CFLAGS) -MMD -MP -MF ./dep/avl_test.d -c -o $@ $<

##
 ###############################################################################
 # Build Test executables which will be run to perform test
 ###############################################################################
 ##

$(TEST)/bst_test: $(SRC)/bst.o $(SRC)/stack.o $(SRC)/queue.o \
	          $(TEST)/bst_test.o $(TEST)/funcutils.o
	$(CC) $(CFLAGS) -o $@ $^

$(TEST)/avl_test: $(SRC)/avl.o $(SRC)/stack.o $(TEST)/avl_test.o \
                  $(TEST)/funcutils.o
	$(CC) $(CFLAGS) -o $@ $^

##
 ###############################################################################
 # Run Tests
 ###############################################################################
 ##

bst-test: bst $(TEST)/bst_test
	./$(TEST)/bst_test

bst-vtest: bst $(TEST)/bst_test
	valgrind -q --leak-check=yes ./$(TEST)/bst_test

avl-test: avl $(TEST)/avl_test
	./$(TEST)/avl_test

avl-vtest: avl $(TEST)/avl_test
	valgrind -q --leak-check=yes ./$(TEST)/avl_test

##
 ###############################################################################
 # Hanlde include dependency
 ###############################################################################
 ##

-include $(DEPFILES)

##
 ################################################################################
 # Clean
 ################################################################################
 ##

clean:
	-$(RM) $(TEST)/bst_test
	-$(RM) $(TEST)/avl_test
	-$(RM) ./dep/*.d
	-$(RM) $(SRC)/*.o $(TEST)/*.o

##
 ################################################################################
 # Phony targets declaration
 ################################################################################
 ##

.PHONY: all avl stack

