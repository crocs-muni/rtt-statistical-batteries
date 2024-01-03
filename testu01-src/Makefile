export LD_LIBRARY_PATH=${PWD}/install/lib
export LIBRARY_PATH=${PWD}/install/lib
export C_INCLUDE_PATH=${PWD}/install/include

export CFLAGS=-g -O2 -Wno-stringop-overflow

all: binary testu01

binary: testu01
	make -C binary
	cp binary/TestU01 install/bin/
	make -C binary clean

testu01: force 
	if [ ! -d install ]; then cd TestU01-1.2.3 && autoreconf -i -f && ./configure --prefix=`pwd`/../install && make && make install && make clean; fi

clean:
	make -C TestU01-1.2.3 distclean
	make -C binary clean
	rm -rf install

force: ;
