all: dieharder

dieharder: force
	if [ ! -d install ]; then cd dieharder-3.31.1 && autoreconf -i && ./configure --disable-shared --prefix=`pwd`/../install && make && make install && make clean; fi

force: ;

