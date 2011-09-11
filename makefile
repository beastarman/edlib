install:
	@gcc -Wall -c main.c edlib.c
	@gcc -o main main.o edlib.o
	
remove:
	@rm -rf main.o list.o stack.o main tree tree.o queue.o
	@echo "Files removed"