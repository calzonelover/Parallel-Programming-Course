FLAGS=-I/usr/include -L/usr/lib/x86_64-linux-gnu
#FLAGS=-I/usr/local/cuda/include -L/usr/local/cuda/lib64
LIB=-lOpenCL
all: vectoradd.x vectoradd2.x

%.x: %.c
	cc ${FLAGS} -o $@ $< ${LIB}
clean:
	rm -f *.x
