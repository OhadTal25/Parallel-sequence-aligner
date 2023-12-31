CC = mpicc
CFLAGS = -Wall -O2 -Wextra -fopenmp -g
LDFLAGS = -fopenmp
EXECUTABLE = parallel_alignment
OBJS = main.o functions.o

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(LDFLAGS) -o $(EXECUTABLE) $(OBJS)

main.o: main.c functions.h
	$(CC) $(CFLAGS) -c main.c -o main.o

functions.o: functions.c functions.h
	$(CC) $(CFLAGS) -c functions.c -o functions.o

clean:
	rm -f $(EXECUTABLE) $(OBJS)

run:
	@echo "Input File:"
	@cat input.txt
	@echo
	@echo "Output:"
	@mpiexec -np 2 ./$(EXECUTABLE) < input.txt
	@echo ------------------

runST:
	@echo "Input File:"
	@cat input.txt
	@echo
	@echo "Output:"
	@mpiexec -np 2 ./$(EXECUTABLE) scoreTable.txt < input.txt
	@echo ------------------


.PHONY: all clean run runWithScoreT
