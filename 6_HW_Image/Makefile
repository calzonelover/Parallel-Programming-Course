#FLAGS=-I/usr/include -L/usr/lib/x86_64-linux-gnu
FLAGS=-I/usr/local/cuda/include -L/usr/local/cuda/lib64
LIB=-lOpenCL
all: main.x main_gpu2.x
%.x: %.cc
	g++ ${FLAGS} -o $@ $< ${LIB}
clean:
	rm -f *.x
