# Makefile for the Rastarizer project

OBJS = color.o image.o vectormath.o model.o

main: main.cpp $(OBJS)
	g++ -o main main.cpp $(OBJS)

color.o: color.cpp
	g++ -c color.cpp

image.o: image.cpp
	g++ -c image.cpp

vectormath.o: vectormath.cpp
	g++ -c vectormath.cpp

model.o: model.cpp
	g++ -c model.cpp

clean:
	del $(OBJS)