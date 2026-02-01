#makefile for programs

compile:
	gcc -Wall leakcount.c -o leakcount
	gcc -Wall -fPIC -shared -o mem_shim.so mem_shim.c
	gcc -Wall sctracer.c -o sctracer

run:
	./leakcount
	./sctracer

clean:
	rm mem_shim.so leakcount sctracer

