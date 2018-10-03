#FLAGS=-I/usr/include -L/usr/lib/x86_64-linux-gnu
FLAGS=-I/usr/local/cuda/include -L/usr/local/cuda/lib64
LIB=-lOpenCL
all: platform_query.x device_query.x
%.x: %.c
	cc ${FLAGS} -o $@ $< ${LIB}
clean:
	rm -f *.x
