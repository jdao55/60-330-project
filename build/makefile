CC=gcc
IDIR=../src/
BINDIR=../bin/
CFlAGS=-Wall -Wextra -O3 -march=native -I

project: lruStack
	echo "Compiling project.."
	@$(CC) $(CFlAGS)$(IDIR) $(IDIR)main.c lruStack.o -o $(BINDIR)project
	printf("done \n");

lruStack: $(IDIR)lruStack.c $(IDIR)lruStack.h
	@$(CC) $(CFlAGS)$(IDIR) -c $(IDIR)lruStack.c -o lruStack.o
