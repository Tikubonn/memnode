
export CFLAGS = -I. -Iinclude -Llib

debug: .always 
	make libmemnode.a libmemnode.so CFLAGS="$(CFLAGS) -Og -g -Wall"

release: .always 
	make libmemnode.a libmemnode.so CFLAGS="$(CFLAGS) -O3 -Wall"

test: .always
	make debug 
	make test.exe CFLAGS="$(CFLAGS) -Og -g -Wall"
	gdb test.exe 
	#./test.exe

setup: .always
	git clone https://github.com/tikubonn/bitarray
	make release -C bitarray
	cp bitarray/bitarray.h include/bitarray.h
	cp bitarray/libbitarray.so lib/libbitarray.so
	cp bitarray/libbitarray.a lib/libbitarray.a

clean: .always
	rm -f memnode.o

libbitarray.so: lib/libbitarray.so
	cp lib/libbitarray.so libbitarray.so

test.exe: test.c libmemnode.so memnode.h libbitarray.so
	gcc $(CFLAGS) test.c libmemnode.so libbitarray.so -o test.exe 

.always:

memnode.o: memnode.c memnode.h 
	gcc $(CFLAGS) -c memnode.c -o memnode.o

libmemnode.so: memnode.o lib/libbitarray.so
	gcc $(CFLAGS) -shared memnode.o lib/libbitarray.so -o libmemnode.so

libmemnode.a: memnode.o 
	ar r libmemnode.a memnode.o
