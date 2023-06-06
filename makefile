src = $(wildcard ./src/*.cpp)
obj = $(patsubst ./src/%.cpp, ./obj/%.o, $(src))

agrs = -Wall -g

ALL:hello

$(obj):./obj/%.o:./src/%.cpp
	g++ -c $< -o $@ $(args) -I ./inc

hello:$(obj)
	g++ $^ -o $@ $(args)

clean:
	-rm -rf $(obj)

.PHONY: ALL clean

