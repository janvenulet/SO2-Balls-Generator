all: clean compile

compile:
	g++ -o projekt1  Ball.cpp main.cpp -pthread -lncurses -fpermissive

clean:
	rm projekt1

run:
	./projekt1
