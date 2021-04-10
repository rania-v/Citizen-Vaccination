CC=gcc
DEPS = dataprocessing.h hash.h virus.h entries.h skip_list.h questions.h bloom_filter.h
OBJ = main.o dataprocessing.o entries.o skip_list.o hash.o  questions.o bloom_filter.o
LIBS = -lm

%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $<

init: $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

run:
	./init -c citizenRecordsFile.txt -b 100000
	# ./init -c 10k.txt -b bloomSize

val:
	valgrind -s --leak-check=full ./init -c citizenRecordsFile.txt -b 100000

clean:
	rm -f *.o
