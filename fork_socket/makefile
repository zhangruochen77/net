src = $(wildcard ./*.c)
obj = $(patsubst ./%., ./%.o, $(src))

agrs = -Wall -g

ALL:out

$(obj):
	gcc -c $< -o $@ $(args)

hello:$(obj)
	gcc $^ -o $@ $(args)

clean:
	-rm -rf $(obj)

.PHONY: ALL clean

