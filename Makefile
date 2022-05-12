SOURCE_CODE=*.h *.cpp

CFLAGS= -Wall -g -pthread

make:
	clear
	g++ $(CFLAGS) $(SOURCE_CODE) -o main
	./main

clean:
	rm main.o main
	clear