exec = theia.out
sources = $(wildcard src/*.cpp)
objects = $(sources:.cpp=.o)
flags = -g


$(exec): $(objects)
	g++ -std=c++17 $(objects) $(flags) -o $(exec)

%.o: %.cpp include/%.h
	g++ -c -std=c++17 $(flags) $< -o $@

install:
	make
	cp ./theia.out /usr/local/bin/theia

clean:
	-rm *.out
	-rm *.o
	-rm src/*.o