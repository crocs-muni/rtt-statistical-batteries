export LD_LIBRARY_PATH=${PWD}/install/lib
export LIBRARY_PATH=${PWD}/install/lib
export C_INCLUDE_PATH=${PWD}/install/include

all: binary testu01

binary: testu01
	cd binary && make && make clean

testu01: force 
	if [ ! -d install ]; then cd TestU01-1.2.3 && autoreconf -i && ./configure --prefix=`pwd`/../install && make && make install && make clean; fi

force: ;
