# SOURCE_CODE=*.h *.cpp

# CFLAGS= -Wall -g -pthread -lwiringPi

# make:
# 	clear
# 	g++ $(CFLAGS) $(SOURCE_CODE) -o main
# 	./main

# clean:
# 	rm main.o main
# 	clear

# Compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS  = -Wall -Wextra -g

# The build target
TARGET = main

CC = g++

# Targets needed to bring the executable up to date
all: main.o Conveyor.o Sensor.o Worker.o
	$(CC) $(CFLAGS) -pthread -o $(TARGET) main.o Conveyor.o Sensor.o Worker.o -lwiringPi

main.o: main.cpp Conveyor.h library.h Sensor.h Worker.h
	$(CC) $(CFLAGS) -pthread -o main.o -c main.cpp -lwiringPi

Conveyor.o: Conveyor.h Sensor.h Worker.h library.h 
	$(CC) $(CFLAGS) -pthread -o Conveyor.o -c Conveyor.cpp -lwiringPi

Sensor.o: Sensor.h library.h
	$(CC) $(CFLAGS) -pthread -o Sensor.o -c Sensor.cpp -lwiringPi

Worker.o: Worker.h Sensor.h library.h 
	$(CC) $(CFLAGS) -pthread -o Worker.o -c Worker.cpp -lwiringPi

run:
	chmod +x main
	./main

clean:
	rm -r main main.o Conveyor.o Sensor.o Worker.o