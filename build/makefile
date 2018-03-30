CC=gcc
IDIR=../src/
BINDIR=../bin/
CFlAGS=-Wall -Wextra -march=native -O3 -I

project: lruStack
	@echo "Compiling project.."
	@$(CC) $(CFlAGS)$(IDIR) $(IDIR)main.c lruStack.o -o $(BINDIR)project
	@echo "done" 

lruStack: $(IDIR)lruStack.c $(IDIR)lruStack.h
	@$(CC) $(CFlAGS)$(IDIR) -c $(IDIR)lruStack.c -o lruStack.o
