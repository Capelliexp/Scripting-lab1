CC=g++
CFLAGS=-I lua/src -I irrlicht/include/ irrlicht/lib/Linux/libIrrlicht.a -L/usr/lib/x86_64-linux-gnu/mesa -lGL -lX11 -lXxf86vm -otask3 -L lua/src -llua 

task3: task3.cc lua/src/liblua.a irrlicht/lib/Linux/libIrrlicht.a
	$(CC) task3.cc $(CFLAGS) -o $@ -ldl

lua/src/liblua.a:
	cd lua/src; make linux

irrlicht/lib/Linux/libIrrlicht.a:
	cd irrlicht/source/Irrlicht/; make linux