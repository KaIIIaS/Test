CC = g++
CFLAGS = -std=c++11

MAIN = main.cpp
OBJS = DataReader.o DataWriter.o

all: program

program: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(MAIN) -o program

DataReader.o: DataReader.cpp DataReader.h
	$(CC) $(CFLAGS) -c DataReader.cpp

DataWriter.o: DataWriter.cpp DataWriter.h
	$(CC) $(CFLAGS) -c DataWriter.cpp

clean:
	rm -rf *.o program
