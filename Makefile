exec = theia.out
sources = $(wildcard src/*.cpp)
objects = $(sources:.cpp=.o)
flags = -g


$(exec): $(objects)
	g++ $(objects) $(flags) -o $(exec)

%.o: %.cpp include/%.h
	g++ -c $(flags) $< -o $@

install:
	make
	cp ./theia.out /usr/local/bin/theia

clean:
	-rm *.out
	-rm *.o
	-rm src/*.o